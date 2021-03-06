////////////////////////////////////////////////////////////////////////////////////
//		This material is provided "as is", with absolutely no warranty
//	expressed or implied. Any use is at your own risk.
//
//		Permission to use or copy this software for any purpose is hereby
//	granted without fee, provided the above notices are retained on all
//	copies. Permission to modify the code and to distribute modified code
//	is granted, provided the above notices are retained, and a notice that
//	the code was modified is included with the above copyright notice.
//
//		https://bitbucket.org/pilatuz/omni
////////////////////////////////////////////////////////////////////////////////////
/** @file
@brief Configuration tools.

		This header file contains a several tools,
	that helps to work with configurations.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_CONF_HPP_
#define __OMNI_CONF_HPP_

#include <omni/defs.hpp>

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <list>

#include <assert.h>
#include <stdarg.h>
#include <limits.h>
#include <stdio.h>

namespace omni
{
	/** @brief Configuration tools.

			This namespace contains configuration tools.

	@code
		#include <omni/conf.hpp>
	@endcode

	@see @ref omni_config
	*/
	namespace conf
	{
		template<typename Str>
			class ItemT;


		/// @brief Configuration tools: exceptions.
		/**
				This namespace contains exception classes
			related to the configuration tools.
		*/
		namespace err
		{
			template<typename Str> class FailureT;
			template<typename Str>   class AccessFailureT;
			template<typename Str>     class ChildNotFoundT;
			template<typename Str>     class NameIsAmbiguousT;
			template<typename Str>   class ParsingFailureT;
			template<typename Str>     class SyntaxErrorT;
			template<typename Str>     class NameMismatchT;
			template<typename Str>   class WritingFailureT;
			template<typename Str>     class NameIsEmptyT;
		}


		/// @brief Configuration tools: input/output.
		/**
				This namespace contains input/output tools.
		*/
		namespace io
		{
			template<typename Str>
				class ParserT;
			template<typename Str>
				class WriterT;
		}


		/// @brief Configuration tools: implementation.
		/**
				This namespace contains implementation specific tools.

		@warning Do not use this namespace.
		*/
		namespace details
		{
			template<typename Ch>
				class CharConst;
			template<typename Str, typename Ch>
				void formatv(Str &out, Ch const *fmt, va_list args);
		}
	}


	/// @brief The default configuration.
#if OMNI_UNICODE
	typedef conf::ItemT<std::wstring> Config;
#else
	typedef conf::ItemT<std::string> Config;
#endif // OMNI_UNICODE


	// ItemT
	namespace conf
	{

///////////////////////////////////////////////////////////////////////////////
/// @brief The configuration item.
/**
		This class represents a custom configuration. The configuration
	contains name, value and optional any number of child configurations.
	For example:

@code
# comment
elem_name = "elem_value" # comment
@endcode

		If the example will be parsed, then the element
	will have the following properties:
		- name() will be equal to the "elem_name",
		- val() will be equal to the "elem_value",

		The fullName() method returns a configuration's full name,
	including full path (i.e. names of all parents).

		It is possible to implicit assign the configuration's value.

@code
	void f(Config &cfg)
	{
		cfg.val() = "new value";
		// -or-
		cfg = "new value";
	}
@endcode

		The equal() method compares two configurations.
	Two configurations are equal if they have the same names,
	same values, and the same child configurations.
	The == and != operators are also available.

		The @a Str template parameter defines the string container type.
	A usual @a Str types are @a std::wstring for UNICODE projects
	or @a std::string for ANSI projects.


		The child configurations can be manipulated through several
	methods: push_back(), remove(), exists(). It is also possible
	to use iterator methods: begin() and end().

		To access child configuration by name you can use one of the overloaded
	get() methods. To access child configuration value directly you can use one of the
	overloaded getv() method. Also you can use operator[]():

@code
	// configuration's value access
	void f(Config const& s)
	{
		s.get("elem_name").val();
		s["elem_name"].val();
		s.getv("elem_name");
	}
@endcode

		You can access child configuration by name only if the name is unique. Otherwise
	you should use begin() and end() methods to iterate all child configurations.

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see @ref omni_config
*/
template<typename Str>
class ItemT
{
///////////////////////////////////////////////////////////////////////////////
/// @name Main typedefs
/// @{
public:
	typedef ItemT<Str> ThisType; ///< @brief The this type.
	typedef ItemT<Str> ChildType; ///< @brief The child type.

	typedef Str String; ///< @brief The string type.
	typedef typename Str::traits_type StrTraits; ///< @brief The string traits type.
	typedef typename StrTraits::char_type Char;  ///< @brief The character type.

	typedef typename String::allocator_type Allocator; ///< @brief The allocator type.
	typedef std::list< ChildType, typename Allocator::
		template rebind<ChildType>::other > Container; ///< @brief The childs container type.
	typedef typename Container::const_iterator ConstIterator; ///< @brief The constant iterator.
	typedef typename Container::iterator Iterator; ///< @brief The iterator.
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Exception typedefs
/// @{
public:
	typedef err::FailureT<String> Failure;                    ///< @brief The basic exception.
	typedef err::AccessFailureT<String> AccessFailure;        ///< @brief The child access exception.
	typedef err::ChildNotFoundT<String> ChildNotFound;        ///< @brief Child not found exception.
	typedef err::NameIsAmbiguousT<String> NameIsAmbiguous;    ///< @brief The child name is not unique.
	typedef err::ParsingFailureT<String> ParsingFailure;      ///< @brief Configuration parsing exception.
	typedef err::SyntaxErrorT<String> SyntaxError;            ///< @brief The syntax error.
	typedef err::NameMismatchT<String> NameMismatch;          ///< @brief The name mismatch exception.
	typedef err::WritingFailureT<String> WritingFailure;      ///< @brief Configuration writing exception.
	typedef err::NameIsEmptyT<String> NameIsEmpty;            ///< @brief The name is empty.
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Constructors and destructor
/// @{
public:

	/// @brief The default constructor.
	/**
			This constructor creates an empty configuration:
		the name, the value, and the childs are empty.
	*/
	ItemT()
		: m_parent(0)
	{}


	/// @brief Create with specified name.
	/**
			This constructor initializes the configuration name.
		The value, and the childs are empty.

	@param[in] theName The configuration name.
	*/
	explicit ItemT(String const& theName)
		: m_parent(0), // will be set later
		  m_name(theName)
	{}


	/// @brief Create with specified name (C-style string).
	/**
			This constructor initializes the configuration name.
		The value, and the childs are empty.

	@param[in] theName The configuration name.
	*/
	explicit ItemT(Char const* theName)
		: m_parent(0), // will be set later
		  m_name(theName)
	{}


	/// @brief Copy constructor.
	/**
			This constructor creates the copy of the @a other configuration.

	@param[in] other The other configuration.
	*/
	ItemT(ThisType const& other)
		: m_parent(0), // will be set later
		  m_name(other.m_name),
		  m_val(other.m_val),
		  m_childs(other.m_childs)
	{
		// update childs parent
		const Iterator ie = m_childs.end();
		Iterator i = m_childs.begin();
		for (; i != ie; ++i)
			i->m_parent = this;
	}


	// TODO: move constructor


	/// @brief Destructor.
	/**
			Checks the configuration consistency and releases all resources.
	*/
	~ItemT()
	{
#if OMNI_DEBUG
		// checks the childs parent
		ConstIterator const ie = m_childs.end();
		ConstIterator i = m_childs.begin();
		for (; i != ie; ++i)
			assert(this == i->m_parent
				&& "invalid parent");
#endif
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Assignments
/// @{
public:

	/// @brief Parse configuration.
	/**
			The current configuration context will be replaced with
		the configuration parsed from the input string.

	@param[in] str The configuration string.
	@return The self reference.
	@throw ThisType::ParsingFailure if configuration string is invalid.
	@see io::ParserT
	*/
	ThisType& parse(String const& str)
	{
		typedef std::basic_istringstream<Char,
			StrTraits, Allocator> ISStream;

		ISStream is(str); // input stream

		ThisType tmp;
		is >> tmp; // use default parser
		swap(tmp);

		return *this;
	}


	/// @brief Parse configuration (C-style string).
	/**
			The current configuration context will be replaced with
		the configuration parsed from the input string.

	@param[in] str The configuration string.
	@return The self reference.
	@throw ThisType::ParsingFailure if configuration string is invalid.
	@see io::ParserT
	*/
	ThisType& parse(Char const* str)
	{
		return parse(String(str));
	}


	/// @brief Merge configuration.
	/**
			This method replaces the some childs with corresponding childs
		of @a other configuration. The only childs with the same names will
		be replaced. If child with specified name not exists yet, it will be added.

		For examples, let two configuration A and B:

	@code
		<A>
			elem1 = "A_value1"
			elem2 = "A_value2"
		</A>

		<B>
			elem1 = "B_value1"
			elem3 = "B_value3"
		</B>
	@endcode

		After the command @a A.merge(B), the A section will be:

	@code
		<A>
			elem1 = "B_value1"
			elem2 = "A_value2"
			elem3 = "B_value3"
		</A>
	@endcode

			If the child configuration has ambiguous names, exception will be thrown.

	@param[in] other The other configuration.
	@return The self reference.
	@throw ThisType::NameIsAmbiguous if child configuration has ambiguous names.
	*/
	ThisType& merge(ThisType const& other)
	{
		ConstIterator const ie = other.m_childs.end();
		ConstIterator i = other.m_childs.begin();

		for (; i != ie; ++i)
		{
			ChildType &child = get(i->name(), true);
			child.val() = i->val(); // assign value

			if (!i->empty())
				child.merge(*i);
		}

		return *this;
	}


	/// @brief Assignment operator.
	/**
	@param[in] other The other configuration.
	@return The self reference.
	*/
	ThisType& operator=(ThisType const& other)
	{
		ThisType(other).swap(*this);
		return *this;
	}


	/// @brief Assign a new value.
	/**
			This assignment operator allows to implicit set the configuration value.

	@param[in] theVal The new value.
	@return The self reference.
	*/
	ThisType& operator=(String const& theVal)
	{
		m_val = theVal;
		return *this;
	}


	/// @brief Assign a new value (C-style string).
	/**
			This assignment operator allows to implicit set the configuration value.

	@param[in] theVal The new value.
	@return The self reference.
	*/
	ThisType& operator=(Char const* theVal)
	{
		m_val = theVal;
		return *this;
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Name and value
/// @{
public:

	/// @brief Get configuration name.
	/**
			This method returns a constant reference to the configuration name.

	@return The configuration name.
	*/
	String const& name() const
	{
		return m_name;
	}


	/// @brief Get/set configuration name.
	/**
			This method returns a non-constant reference to the configuration name.

	@return The confgiuration name.
	*/
	String& name()
	{
		return m_name;
	}


	/// @brief Get configuration value.
	/**
			This method returns a constant reference to the configuration value.

	@return The configuration value.
	*/
	String const& val() const
	{
		return m_val;
	}


	/// @brief Get/set configuration value.
	/**
			This method returns a non-constant reference to the configuration value.

	@return The configuration value.
	*/
	String& val()
	{
		return m_val;
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Full name
/// @{
public:

	/// @brief Get full name.
	/**
			This method returns the configuration full name, including all parents.
		The parent names are separated by @a sep string. For example,
		if @a sep is equal to the "|", then the full name of element "param1"
		will be "root|section|param1":

	@code
		<root>
			<section>
				param1 = "value1"
			</section>
		</root>
	@endcode

	@param[in] sep The separator.
	@return The full name.
	*/
	String const fullName(String const& sep) const
	{
		if (m_parent)
		{
			const size_t sep_len = sep.size();

			// calculate output size
			size_t len = m_name.size();
			for (ThisType const* p = m_parent; p != 0; p = p->m_parent)
				len += p->m_name.size() + sep_len;

			String full_name;
			full_name.resize(len);

			// current name
			len -= m_name.size();
			std::copy(m_name.begin(), m_name.end(),
				full_name.begin() + len);

			for (ThisType const* p = m_parent; p != 0; p = p->m_parent)
			{
				// separator
				len -= sep_len;
				std::copy(sep.begin(), sep.end(),
					full_name.begin() + len);

				// parent name
				len -= p->m_name.size();
				std::copy(p->m_name.begin(), p->m_name.end(),
					full_name.begin() + len);
			}

			assert(0 == len && "WTF?");
			return full_name;
		}

		return m_name;
	}


	/// @brief Get full name (C-style string).
	/**
			This method returns the configuration full name, including all parents.
		The parent names are separated by @a sep string. For example,
		if @a sep is equal to the "|", then the full name of element "param1"
		will be "root|section|param1":

	@code
		<root>
			<section>
				param1 = "value1"
			</section>
		</root>
	@endcode

	@param[in] sep The separator.
	@return The full name.
	*/
	String const fullName(Char const* sep) const
	{
		return fullName(String(sep));
	}


	/// @brief Get full name.
	/**
			This method returns the configuration full name
		using default separator ":".

	@return The full name.
	*/
	String const fullName() const
	{
		return fullName(details::CharConst<Char>::SEPARATOR());
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Auxiliary
/// @{
public:

	/// @brief Format output to the value.
	/**
	@param[in] fmt The format string.
	*/
	void format(Char const *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);

		details::formatv(m_val, fmt, args);

		va_end(args);
	}


	/// @brief Are two configurations equal?
	/**
			The two configurations are equal if they have the same names,
		the same values and the same child configurations.

	@param[in] other The other configuration.
	@return @b true if two configurations are equal, otherwise @b false.
	*/
	bool equal(ThisType const& other) const
	{
		if (m_name != other.m_name)
			return false;
		if (m_val != other.m_val)
			return false;

		if (m_childs.size() != other.m_childs.size())
			return false;
		return std::equal(m_childs.begin(), m_childs.end(),
			other.m_childs.begin());
	}


	/// @brief Swap two configurations.
	/**
			This method swaps the two configurations.
		The parents are not changed.

	@param[in,out] other The other configuration.
	*/
	void swap(ThisType &other)
	{
		// (!) do not swap parents

		m_name.swap(other.m_name);
		m_val.swap(other.m_val);

		m_childs.swap(other.m_childs);

		{ // update this.childs parent
			Iterator const ie = m_childs.end();
			Iterator i = m_childs.begin();
			for (; i != ie; ++i)
				i->m_parent = this;
		}

		{ // update other.childs parent
			Iterator const ie = other.m_childs.end();
			Iterator i = other.m_childs.begin();
			for (; i != ie; ++i)
				i->m_parent = &other;
		}
	}


	/// @brief Get empty configuration.
	/**
			This static method returns a constant reference to the empty configuration.
		This empty configuration may be used as default value in get() methods.

	@code
		void f(const omni::Config &cfg)
		{
			const omni::Config &elem = cfg.get("param", omni::Config::EMPTY());

			// ...
		}
	@endcode

	@return The empty configuration.
	*/
	static ThisType const& EMPTY()
	{
		static ThisType g_empty;
		return g_empty;
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Iterators
/// @{
public:

	/// @brief Get the begin of the childs.
	/**
	@return The constant iterator.
	*/
	ConstIterator const begin() const
	{
		return m_childs.begin();
	}


	/// @brief Get the begin of the childs.
	/**
	@return The non-constant iterator.
	*/
	Iterator const begin()
	{
		return m_childs.begin();
	}


	/// @brief Get the end of the childs.
	/**
	@return The constant iterator.
	*/
	ConstIterator const end() const
	{
		return m_childs.end();
	}


	/// @brief Get the end of the childs.
	/**
	@return The non-constant iterator.
	*/
	Iterator const end()
	{
		return m_childs.end();
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Front & Back
/// @{
public:

	/// @brief Get the first child.
	/**
	@return The constant reference.
	*/
	ChildType const& front() const
	{
		return m_childs.front();
	}


	/// @brief Get the first child.
	/**
	@return The non-constant reference.
	*/
	ChildType& front()
	{
		return m_childs.front();
	}


	/// @brief Get the last child.
	/**
	@return The constant reference.
	*/
	ChildType const& back() const
	{
		return m_childs.back();
	}


	/// @brief Get the last child.
	/**
	@return The non-constant reference.
	*/
	ChildType& back()
	{
		return m_childs.back();
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Get child by name
/// @{
public:

	/// @brief Get child by name.
	/**
			This method returns a child with specified name @a theName.
		If the child with that name is not exists or the name
		is not unique the exception will be thrown.

	@param[in] theName The child name.
	@return The child's constant reference.
	@throw ThisType::ChildNotFound If child not found.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType const& get(String const& theName) const
	{
		ConstIterator found = find(theName, m_childs.begin());
		if (found == m_childs.end())
			throw ChildNotFound(theName, fullName());

		ChildType const& child = *found;
		if (find(theName, ++found) != m_childs.end())
			throw NameIsAmbiguous(theName, fullName());

		return child;
	}


	/// @brief Get child by name (C-style string).
	/**
			This method returns a child with specified name @a theName.
		If the child with that name is not exists or the name
		is not unique the exception will be thrown.

	@param[in] theName The child name.
	@return The child's constant reference.
	@throw ThisType::ChildNotFound If child not found.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType const& get(Char const* theName) const
	{
		ConstIterator found = find(theName, m_childs.begin());
		if (found == m_childs.end())
			throw ChildNotFound(theName, fullName());

		ChildType const& child = *found;
		if (find(theName, ++found) != m_childs.end())
			throw NameIsAmbiguous(theName, fullName());

		return child;
	}


	/// @brief Get child by name or default.
	/**
			This method returns a child with specified name @a theName.
		If the child with that name is not exists, then the @a def configuration
		will be return. If the child's name is not unique the exception
		will be thrown.

	@param[in] theName The child name.
	@param[in] def The default configuration.
	@return The child's constant reference.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType const& get(String const& theName, ChildType const& def) const
	{
		ConstIterator found = find(theName, m_childs.begin());
		if (found == m_childs.end())
			return def;

		ChildType const& child = *found;
		if (find(theName, ++found) != m_childs.end())
			throw NameIsAmbiguous(theName, fullName());

		return child;
	}


	/// @brief Get child by name or default (C-style string).
	/**
			This method returns a child with specified name @a theName.
		If the child with that name is not exists, then the @a def configuration
		will be return. If the child's name is not unique the exception
		will be thrown.

	@param[in] theName The child name.
	@param[in] def The default configuration.
	@return The child's constant reference.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType const& get(Char const* theName, ChildType const& def) const
	{
		ConstIterator found = find(theName, m_childs.begin());
		if (found == m_childs.end())
			return def;

		ChildType const& child = *found;
		if (find(theName, ++found) != m_childs.end())
			throw NameIsAmbiguous(theName, fullName());

		return child;
	}


	/// @brief Get child by name or create.
	/**
			This method returns a child with specified name @a theName.
		If the child with that name is not exists, then the it will be created
		if @a create flag is set, otherwise the exception will be thrown.
		If the child's name is not unique the exception will be thrown.

	@param[in] theName The child name.
	@param[in] create Create if not exists flag.
	@return The child's non-constant reference.
	@throw ThisType::ChildNotFound If child not found and @a create flag is not set.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType& get(String const& theName, bool create = false)
	{
		Iterator found = find(theName, m_childs.begin());
		if (found == m_childs.end())
		{
			if (create)
			{
				push_back(theName);
				return back();
			}
			else
				throw ChildNotFound(theName, fullName());
		}

		ChildType &child = *found;
		if (find(theName, ++found) != end())
			throw NameIsAmbiguous(theName, fullName());

		return child;
	}


	/// @brief Get child by name or create (C-style string).
	/**
			This method returns a child with specified name @a theName.
		If the child with that name is not exists, then the it will be created
		if @a create flag is set, otherwise the exception will be thrown.
		If the child's name is not unique the exception will be thrown.

	@param[in] theName The child name.
	@param[in] create Create if not exists flag.
	@return The child's non-constant reference.
	@throw ThisType::ChildNotFound If child not found and @a create flag is not set.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType& get(Char const* theName, bool create = false)
	{
		Iterator found = find(theName, m_childs.begin());
		if (found == m_childs.end())
		{
			if (create)
			{
				push_back(theName);
				return back();
			}
			else
				throw ChildNotFound(theName, fullName());
		}

		ChildType &child = *found;
		if (find(theName, ++found) != m_childs.end())
			throw NameIsAmbiguous(theName, fullName());

		return child;
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Get child's value by name
/// @{
public:

	/// @brief Get child's value by name.
	/**
			This method returns a child's value with specified name @a theName.
		If the child with that name is not exists or the name
		is not unique the exception will be thrown.

	@param[in] theName The child name.
	@return The child's value.
	@throw ThisType::ChildNotFound If child not found.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	String const& getv(String const& theName) const
	{
		return get(theName).val();
	}


	/// @brief Get child's value by name (C-style string).
	/**
			This method returns a child's value with specified name @a theName.
		If the child with that name is not exists or the name
		is not unique the exception will be thrown.

	@param[in] theName The child name.
	@return The child's value.
	@throw ThisType::ChildNotFound If child not found.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	String const& getv(Char const* theName) const
	{
		return get(theName).val();
	}


	/// @brief Get child's value by name or default.
	/**
			This method returns a child's value with specified name @a theName.
		If the child with that name is not exists, then the @a def will be return.
		If the child's name is not unique the exception will be thrown.

	@param[in] theName The child name.
	@param[in] def The default value.
	@return The child's value.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	String const& getv(String const& theName, String const& def) const
	{
		ConstIterator found = find(theName, m_childs.begin());
		if (found == m_childs.end())
			return def;

		ChildType const& child = *found;
		if (find(theName, ++found) != m_childs.end())
			throw NameIsAmbiguous(theName, fullName());

		return child.val();
	}


	/// @brief Get child's value by name or default (C-style string).
	/**
			This method returns a child's value with specified name @a theName.
		If the child with that name is not exists, then the @a def will be return.
		If the child's name is not unique the exception will be thrown.

	@param[in] theName The child name.
	@param[in] def The default value.
	@return The child's value.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	String const& getv(Char const* theName, String const& def) const
	{
		ConstIterator found = find(theName, m_childs.begin());
		if (found == m_childs.end())
			return def;

		ChildType const& child = *found;
		if (find(theName, ++found) != m_childs.end())
			throw NameIsAmbiguous(theName, fullName());

		return child.val();
	}


	/// @brief Get child's value by name or default (C-style string).
	/**
			This method returns a child's value with specified name @a theName.
		If the child with that name is not exists, then the @a def will be return.
		If the child's name is not unique the exception will be thrown.

	@param[in] theName The child name.
	@param[in] def The default value.
	@return The child's value.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	Char const* getv(Char const* theName, Char const* def) const
	{
		ConstIterator found = find(theName, m_childs.begin());
		if (found == m_childs.end())
			return def;

		ChildType const& child = *found;
		if (find(theName, ++found) != m_childs.end())
			throw NameIsAmbiguous(theName, fullName());

		return child.val().c_str();
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Auxiliary operators
/// @{
public:

	/// @brief Get child by name.
	/**
			This method is equivalent for ThisType::get(theName).

	@param[in] theName The child name.
	@return The child's constant reference.
	@throw ThisType::ChildNotFound If child not found.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType const& operator[](String const& theName) const
	{
		return get(theName);
	}


	/// @brief Get child by name (C-style string).
	/**
			This method is equivalent for ThisType::get(theName).

	@param[in] theName The child name.
	@return The child's constant reference.
	@throw ThisType::ChildNotFound If child not found.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType const& operator[](Char const* theName) const
	{
		return get(theName);
	}


	/// @brief Get child by name or create.
	/**
			This method is equivalent for ThisType::get(theName, true), i.e.
		if the child with that name is not exists it will be created!

	@param[in] theName The child name.
	@return The child's non-constant reference.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType& operator[](String const& theName)
	{
		// (!) create if not exists
		return get(theName, true);
	}


	/// @brief Get child by name or create (C-style string).
	/**
			This method is equivalent for ThisType::get(theName, true), i.e.
		if the child with that name is not exists it will be created!

	@param[in] theName The child name.
	@return The child's non-constant reference.
	@throw ThisType::NameIsAmbiguous If child's name is not unique.
	*/
	ChildType& operator[](Char const* theName)
	{
		// (!) create if not exists
		return get(theName, true);
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Manipulators
/// @{
public:

	/// @brief Create a new child at the end.
	/**
			This method creates a new child as a copy of the @a other,
		and puts it to the end of child list.

	@param[in] child The prototype of the child configuration.
	@return The created child reference.
	*/
	ChildType& push_back(ChildType const& child)
	{
		m_childs.push_back(child);
		ChildType &res = m_childs.back();

		// (!) update the parent
		res.m_parent = this;

		return res;
	}


	/// @brief Create a new child at the end.
	/**
			This method creates a new child with name @a theName,
		and puts it to the end of child list.

	@param[in] theName The child's name.
	@return The created child reference.
	*/
	ChildType& push_back(String const& theName)
	{
		m_childs.push_back(ChildType());
		ChildType &res = m_childs.back();
		res.name() = theName;

		// (!) update the parent
		res.m_parent = this;

		return res;
	}


	/// @brief Create a new child at the end (C-style string).
	/**
			This method creates a new child with name @a theName,
		and puts it to the end of child list.

	@param[in] theName The child's name.
	@return The created child reference.
	*/
	ChildType& push_back(Char const* theName)
	{
		m_childs.push_back(ChildType());
		ChildType &res = m_childs.back();
		res.name() = theName;

		// (!) update the parent
		res.m_parent = this;

		return res;
	}


	/// @brief Remove the child configuration by iterator.
	/**
			This method removes the child configuration.

	@warning The input configuration should be a child of this configuration!
	@param[in] pos The child configuration iterator to remove.
	*/
	Iterator erase(Iterator pos)
	{
		return m_childs.erase(pos);
	}


	/// @brief Remove the child configuration.
	/**
			This method removes the child configuration.

	@warning The input configuration should be a child of this configuration!

	@param[in] child The child configuration to remove.
	*/
	void remove(ChildType const& child)
	{
		assert(this == child.m_parent
			&& "invalid child parent");

		m_childs.remove(child);
	}


	/// @brief Remove the child configurations.
	/**
			This methos removes the all child configurations with name @a theName.

	@param[in] theName The child's name to remove.
	*/
	void remove(String const& theName)
	{
		Iterator const ie = m_childs.end();
		for (Iterator i = m_childs.begin(); i != ie; )
		{
			if (i->name() == theName)
				i = erase(i);
			else
				++i;
		}
	}


	/// @brief Remove the child configurations (C-style string).
	/**
			This methos removes the all child configurations with name @a theName.

	@param[in] theName The child's name to remove.
	*/
	void remove(const Char *theName)
	{
		Iterator const ie = m_childs.end();
		for (Iterator i = m_childs.begin(); i != ie; )
		{
			if (i->name() == theName)
				i = erase(i);
			else
				++i;
		}
	}


	/// @brief Remove all child configurations.
	/**
			This method removes all child configurations.
	*/
	void clear()
	{
		m_childs.clear();
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Selectors
/// @{
public:

	/// @brief Is child exists?
	/**
			This method checks at least one child configuration
		with name @a theName.

	@param[in] theName The child name
	@return @b true if there is at least one child configuration with specified name,
		otherwise @b false
	*/
	bool exists(String const& theName) const
	{
		return find(theName, m_childs.begin()) != m_childs.end();
	}


	/// @brief Is child exists? (C-style string)
	/**
			This method checks at least one child configuration
		with name @a theName.

	@param[in] theName The child name
	@return @b true if there is at least one child configuration with specified name,
		otherwise @b false
	*/
	bool exists(Char const* theName) const
	{
		return find(theName, m_childs.begin()) != m_childs.end();
	}


	/// @brief Get number of child configurations.
	/**
	@return The number of child configurations.
	*/
	size_t size() const
	{
		return m_childs.size();
	}


	/// @brief Is there no child configurations?
	/**
	@return @b true If there are no child configurations, otherwise @b false.
	*/
	bool empty() const
	{
		return m_childs.empty();
	}


	/// @brief Get the parent.
	/**
	@return The parent pointer or null.
	*/
	ThisType const* parent() const
	{
		return m_parent;
	}

private:

	/// @brief Find the child configuration.
	/**
			This method search the child configuration with name @a theName
		starting at the @a pos position.

	@param[in] theName The child configuration name.
	@param[in] pos The search starting position.
	@return The found child configuration position or end(), if there is no childs with specified name.
	*/
	ConstIterator const find(String const& theName, ConstIterator pos) const
	{
		ConstIterator const last = m_childs.end();
		for (; pos != last; ++pos)
			if (pos->name() == theName)
				break;

		return pos;
	}


	/// @brief Find the child configuration (C-style string).
	/**
			This method search the child configuration with name @a theName
		starting at the @a pos position.

	@param[in] theName The child configuration name.
	@param[in] pos The search starting position.
	@return The found child configuration position or end(), if there is no childs with specified name.
	*/
	ConstIterator const find(Char const* theName, ConstIterator pos) const
	{
		ConstIterator const last = m_childs.end();
		for (; pos != last; ++pos)
			if (pos->name() == theName)
				break;

		return pos;
	}


	/// @brief Find the child configuration.
	/**
			This method search the child configuration with name @a theName
		starting at the @a pos position.

	@param[in] theName The child configuration name.
	@param[in] pos The search starting position.
	@return The found child configuration position or end(), if there is no childs with specified name.
	*/
	Iterator const find(String const& theName, Iterator pos)
	{
		Iterator const last = m_childs.end();
		for (; pos != last; ++pos)
			if (pos->name() == theName)
				break;

		return pos;
	}


	/// @brief Find the child configuration (C-style string).
	/**
			This method search the child configuration with name @a theName
		starting at the @a pos position.

	@param[in] theName The child configuration name.
	@param[in] pos The search starting position.
	@return The found child configuration position or end(), if there is no childs with specified name.
	*/
	Iterator const find(Char const* theName, Iterator pos)
	{
		Iterator const last = m_childs.end();
		for (; pos != last; ++pos)
			if (pos->name() == theName)
				break;

		return pos;
	}

private:
	ThisType *m_parent; ///< @brief The parent or null.

	String m_name; ///< @brief The item's name.
	String m_val;  ///< @brief The item's value.

	Container m_childs; ///< @brief The childs collection.
};


///////////////////////////////////////////////////////////////////////////////
/** @relates ItemT
@brief Are two configurations equal?

		This operator is equivalent to the:

@code
	x.equal(y);
@endcode

@param[in] x The first configuration.
@param[in] y The second configuration.
@return @b true if two configurations are equal, otherwise @b false.
*/
template<typename Str> inline
bool operator==(const ItemT<Str> &x, const ItemT<Str> &y)
{
	return x.equal(y);
}


///////////////////////////////////////////////////////////////////////////////
/** @relates ItemT
@brief Are two configurations non-equal?

		This operator is equivalent to the:

@code
	!x.equal(y);
@endcode

@param[in] x The first configuration.
@param[in] y The second configuration.
@return @b true if two configurations are non-equal, otherwise @b false.
*/
template<typename Str> inline
bool operator!=(const ItemT<Str> &x, const ItemT<Str> &y)
{
	return !x.equal(y);
}


///////////////////////////////////////////////////////////////////////////////
/** @relates ItemT
@brief Swap two configurations.

		This function is equivalent to the:

@code
	x.swap(y);
@endcode

@param[in,out] x The first configuration.
@param[in,out] y The second configuration.
*/
template<typename Str> inline
void swap(ItemT<Str> &x, ItemT<Str> &y)
{
	x.swap(y);
}

	} // ItemT


	// exceptions
	namespace conf
	{
		namespace err
		{

///////////////////////////////////////////////////////////////////////////////
/// @brief The basic exception.
/**
		This class is derived from the standard std::runtime_error, so you
	can use what() method. Detailed information are availalbe through
	various derived classes.

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see ItemT<Str>::Failure
*/
template<typename Str>
class FailureT:
	public std::runtime_error
{
	typedef std::runtime_error base_type;

public:

	/// @brief The string type.
	typedef Str String;

protected:

	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	*/
	explicit FailureT(std::string const& msg)
		: base_type(msg)
	{}


	/// @brief The main constructor (C-style string).
	/**
	@param[in] msg The error message.
	*/
	explicit FailureT(char const* msg)
		: base_type(msg)
	{}


	/// @brief The trivial destructor.
	virtual ~FailureT() OMNI_THROW0()
	{}
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The child access exception.
/**
		Child configuration access exception. This class contains the name of
	the child configuration (name()) and the full path of the parent configuration (path()).

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see ItemT<Str>::AccessFailure
*/
template<typename Str>
class AccessFailureT:
	public FailureT<Str>
{
	typedef FailureT<Str> base_type;

public:

	/// @brief The string type.
	typedef typename base_type::String String;

public:

	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	@param[in] theName The child configuration name.
	@param[in] thePath The parent configuration path.
	*/
	AccessFailureT(std::string const& msg,
		String const& theName,
		String const& thePath)
			: base_type(msg),
			  m_name(theName),
			  m_path(thePath)
	{}


	/// @brief The main constructor (C-style string).
	/**
	@param[in] msg The error message.
	@param[in] theName The child configuration name.
	@param[in] thePath The parent configuration path.
	*/
	AccessFailureT(char const* msg,
		String const& theName,
		String const& thePath)
			: base_type(msg),
			  m_path(thePath),
			  m_name(theName)
	{}


	/// @brief The trivial destructor.
	virtual ~AccessFailureT() OMNI_THROW0()
	{}

public:

	/// @brief Get the parent configuration full name.
	/**
	@return The parent configuration full name.
	*/
	String const& path() const
	{
		return m_path;
	}


	/// @brief Get the child configuration name.
	/**
	@return The child configuration name.
	*/
	String const& name() const
	{
		return m_name;
	}

private:
	String m_path; ///< @brief The parent configuration name.
	String m_name; ///< @brief The child configuration name.
};


///////////////////////////////////////////////////////////////////////////////
/// @brief Child not found exception.
/**
		This exception will be thrown if the child configuration not found.

		The @b what() method will be return "child not found" string.

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see ItemT<Str>::ChildNotFound
*/
template<typename Str>
class ChildNotFoundT:
	public AccessFailureT<Str>
{
	typedef AccessFailureT<Str> base_type;

public:

	/// @brief The string type.
	typedef typename base_type::String String;

public:

	/// @brief The main constructor.
	/**
	@param[in] theName The child configuration name.
	@param[in] thePath The parent configuration path.
	*/
	ChildNotFoundT(String const& theName, String const& thePath)
		: base_type("child not found", theName, thePath)
	{}


	/// @brief The trivial destructor.
	virtual ~ChildNotFoundT() OMNI_THROW0()
	{}
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The child name is not unique.
/**
		This exception will be thrown if the child configuration name is ambiguous,
	i.e. if there are two or more childs with the same name.

		The @b what() method will be return "ambiguous child name" string.

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see ItemT<Str>::AccessFailure
*/
template<typename Str>
class NameIsAmbiguousT:
	public AccessFailureT<Str>
{
	typedef AccessFailureT<Str> base_type;

public:

	/// @brief The string type.
	typedef typename base_type::String String;

public:

	/// @brief The main constructor.
	/**
	@param[in] theName The child configuration name.
	@param[in] thePath The parent configuration path.
	*/
	NameIsAmbiguousT(String const& theName, String const& thePath)
		: base_type("ambiguous child name", theName, thePath)
	{}


	/// @brief The trivial destructor.
	virtual ~NameIsAmbiguousT() OMNI_THROW0()
	{}
};


///////////////////////////////////////////////////////////////////////////////
/// @brief Configuration parsing exception.
/**
		This exception may be thrown during configuration parsing procedure.
	The line() method returns the line number where parser is stopped.

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see ItemT<Str>::ParsingFailure
*/
template<typename Str>
class ParsingFailureT:
	public FailureT<Str>
{
	typedef FailureT<Str> base_type;

public:

	/// @brief The string type.
	typedef typename base_type::String String;

public:

	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	@param[in] theLine The line number.
	*/
	ParsingFailureT(std::string const& msg, long theLine)
		: base_type(msg), m_line(theLine)
	{}


	/// @brief The main constructor (C-style string).
	/**
	@param[in] msg The error message.
	@param[in] theLine The line number.
	*/
	ParsingFailureT(char const* msg, long theLine)
		: base_type(msg), m_line(theLine)
	{}


	/// @brief The trivial destructor.
	virtual ~ParsingFailureT() OMNI_THROW0()
	{}

public:

	/// @brief Get the line number.
	/**
	@return The line number.
	*/
	long line() const
	{
		return m_line;
	}

private:
	long m_line; ///< @brief The line number.
};


///////////////////////////////////////////////////////////////////////////////
/// @brief Syntax error.
/**
		This exception will be thrown if the syntax error occurs during
	configuration parsing procedure. The path() method returns the configuration
	path.

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see ItemT<Str>::SyntaxError
*/
template<typename Str>
class SyntaxErrorT:
	public ParsingFailureT<Str>
{
	typedef ParsingFailureT<Str> base_type;

public:

	/// @brief The string type.
	typedef typename base_type::String String;

public:

	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	@param[in] thePath The configuration path.
	@param[in] theLine The line number.
	*/
	SyntaxErrorT(std::string const& msg,
			String const& thePath, long theLine)
		: base_type(msg, theLine),
		  m_path(thePath)
	{}


	/// @brief The main constructor (C-style string).
	/**
	@param[in] msg The error message.
	@param[in] thePath The configuration path.
	@param[in] theLine The line number.
	*/
	SyntaxErrorT(char const* msg,
			String const& thePath, long theLine)
		: base_type(msg, theLine),
		  m_path(thePath)
	{}


	/// @brief The trivial destructor.
	virtual ~SyntaxErrorT() OMNI_THROW0()
	{}

public:

	/// @brief Get the configuration path.
	/**
	@return The configuration path.
	*/
	String const& path() const
	{
		return m_path;
	}

private:
	String m_path; /// < @brief The configuration path.
};


///////////////////////////////////////////////////////////////////////////////
/// @brief Name mismatch exception.
/**
		This exception will be thrown if the closed name isn't equal to the open name.
	For example:

@code
	<section1>
		# ...
	</section2>
@endcode

		The expected() method returns the expected configuration name.
	The found() method returns the found configuration name.

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see ItemT<Str>::NameMismatch
*/
template<typename Str>
class NameMismatchT:
	public ParsingFailureT<Str>
{
	typedef ParsingFailureT<Str> base_type;

public:

	/// @brief The string type.
	typedef typename base_type::String String;

public:

	/// @brief The main constructor.
	/**
		@param[in] theExpected The expected configuration name.
		@param[in] theFound The found configuration name.
		@param[in] theLine The line number.
	*/
	NameMismatchT(String const& theExpected,
			String const& theFound, long theLine)
		: base_type("name mismatch", theLine),
		  m_expected(theExpected),
		  m_found(theFound)
	{}


	/// @brief The destructor.
	virtual ~NameMismatchT() OMNI_THROW0()
	{}

public:

	/// @brief Get the expected configuration name.
	/**
	@return The expected configuration name.
	*/
	String const& expected() const
	{
		return m_expected;
	}


	/// @brief Get the found configuration name.
	/**
	@return The found configuration name.
	*/
	String const& found() const
	{
		return m_found;
	}

private:
	String m_expected; ///< @brief The expected configuration name.
	String m_found;    ///< @brief The found configuration name.
};


///////////////////////////////////////////////////////////////////////////////
/// @brief Configuration writing exception.
/**
		This exception may be thrown during configuration writing procedure.

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see ItemT<Str>::WritingFailure
*/
template<typename Str>
class WritingFailureT:
	public FailureT<Str>
{
	typedef FailureT<Str> base_type;

public:

	/// @brief The string type.
	typedef typename base_type::String String;

public:

	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	@param[in] thePath The configuration path.
	*/
	WritingFailureT(std::string const& msg, String const& thePath)
		: base_type(msg), m_path(thePath)
	{}


	/// @brief The main constructor (C-style string).
	/**
	@param[in] msg The error message.
	@param[in] thePath The configuration path.
	*/
	WritingFailureT(char const* msg, String const& thePath)
		: base_type(msg), m_path(thePath)
	{}


	/// @brief The trivial destructor.
	virtual ~WritingFailureT() OMNI_THROW0()
	{}


public:

	/// @brief Get the configuration path.
	/**
	@return The configuration path.
	*/
	String const& path() const
	{
		return m_path;
	}

private:
	String m_path; ///< @brief The configuration path.
};



///////////////////////////////////////////////////////////////////////////////
/// @brief Name is empty.
/**
		This exception will be thrown during writing procedure if the empty
	configuration name is occurs.

@tparam Str The STL string container.
	Should be std::string or std::wstring.
@see ItemT<Str>::NameIsEmpty
*/
template<typename Str>
class NameIsEmptyT:
	public WritingFailureT<Str>
{
	typedef WritingFailureT<Str> base_type;

public:

	/// @brief The string type.
	typedef typename base_type::String String;

public:

	/// @brief The main constructor.
	/**
	@param[in] thePath The configuration path.
	*/
	explicit NameIsEmptyT(String const& thePath)
		: base_type("name is empty", thePath)
	{}


	/// @brief The trivial destructor.
	virtual ~NameIsEmptyT() OMNI_THROW0()
	{}
};

		} // err namespace
	} // exceptions


	// ParserT
	namespace conf
	{
		namespace io
		{

///////////////////////////////////////////////////////////////////////////////
/// @brief The configuration parser.
/**
		This class parses configuration from the input stream.
*/
template<typename Str>
class ParserT
{
public:
	typedef Str String; ///< @brief The string type.
	typedef typename Str::traits_type StrTraits; ///< @brief The string traits type.
	typedef typename StrTraits::char_type Char;  ///< @brief The character type.
	typedef std::basic_istream<Char, StrTraits> IStream; ///< @brief The input stream.

public:
	typedef err::ParsingFailureT<String> ParsingFailure;      ///< @brief Configuration parsing exception.
	typedef err::SyntaxErrorT<String> SyntaxError;            ///< @brief The syntax error.
	typedef err::NameMismatchT<String> NameMismatch;          ///< @brief The name mismatch exception.

public:

	/// @brief The main constructor.
	/**
			Initializes the parser and starts the line counting from one.

	@param[in,out] root The root configuration.
	*/
	explicit ParserT(ItemT<String> &root)
		: m_line_counter(1),
		  m_brace_depth(0)
	{
		push(root);
	}


	/// @brief Parse the configuration from the input stream.
	/**
	@param[in] is The input stream.
	@throw ParserT::ParsingFailure If the input stream is invalid.
	@throw ParserT::SyntaxError If there is invalid syntax.
	@throw ParserT::NameMismatch If there is name mismatch.
	*/
	virtual void parse(IStream &is)
	{
		typedef details::CharConst<Char> ChConst;

		String token;

		while (is)
		{
			skip_ws(is);

			typename StrTraits::int_type meta = is.peek();
			if (StrTraits::eq_int_type(meta, StrTraits::eof()))
				break;

			typename StrTraits::char_type cx = StrTraits::to_char_type(meta);

			// comment (#)
			if (StrTraits::eq(cx, ChConst::COMMENT))
				skip_comment(is.ignore());

			// open bracket '<'
			else if (StrTraits::eq(cx, ChConst::BEGIN))
			{
				bool is_open = true;

				// open or close section? '/'
				cx = StrTraits::to_char_type(is.ignore().peek());
				if (StrTraits::eq(cx, ChConst::CLOSE))
				{
					is_open = false;
					is.ignore();
				}

				get_token(is, token); // section name
				if (token.empty())
					throw SyntaxError("empty name",
						top().fullName(), getLineNumber());
				brace_open();

				if (is_open) // create new section
				{
					ItemT<String> &child = top().push_back(token);
					push(child); // push to the section stack

					parse_value(is, child);
				}
				else // close section
				{
					skip_ws(is);

					cx = StrTraits::to_char_type(is.peek());
					if (!StrTraits::eq(cx, ChConst::END)) // syntax error (>)
						throw SyntaxError("expected \">\" char",
							top().fullName(), getLineNumber());

					is.ignore(); // ignore '>' char
					brace_close();

					if (token != top().name())
						throw NameMismatch(top().name(),
							token, getLineNumber());

					pop(); // (!) pop the configuration
				}
			}

			// close section (/>)
			else if (StrTraits::eq(cx, ChConst::CLOSE))
			{
				cx = StrTraits::to_char_type(is.ignore().peek());
				if (!StrTraits::eq(cx, ChConst::END)) // syntax error '>'
					throw SyntaxError("expected slash and closing bracket",
						top().fullName(), getLineNumber());

				is.ignore(); // ignore '>' char
				brace_close();

				pop(); // (!) pop the configuration
			}

			// close section '>'
			else if (StrTraits::eq(cx, ChConst::END))
			{
				is.ignore(); // ignore '>' char
				brace_close();

				// check if section is closed
				// <a ... a> or <? ... ?> notation
				ItemT<String> &curr = top();
				if (!curr.empty())
				{
					typename ItemT<String>::Iterator child = curr.end();
					--child; // (!) now it is back() element

					if (child->val().empty()
						&& curr.name() == child->name())
					{
						curr.erase(child); // (!) remove unused child
						pop(); // (!) pop the configuration
					}
				}
			}

			// add new element
			else
			{
				get_token(is, token); // parse element name
				if (token.empty())
					throw SyntaxError("empty name",
						top().fullName(), getLineNumber());

				ItemT<String> &elem = top().push_back(token);
				parse_value(is, elem);
			}
		}

		if (1 < m_stack.size())
			throw ParsingFailure("unexpected end of input stream", getLineNumber());
	}

protected:

	/// @brief Push the new configuration.
	/**
			This method is called when the new configuration is open.

	@param[in] cfg The configuration.
	*/
	void push(ItemT<String> &cfg)
	{
		m_stack.push_back(&cfg);
	}


	/// @brief Get the current configuration.
	/**
	@return The current configuration.
	*/
	ItemT<String>& top() const
	{
		return *m_stack.back();
	}


	/// @brief Pop the configuration.
	/**
			This method is called when the configuration is closed.

	@throw ThisType::ParsingFailure If all configurations are closed.
	*/
	void pop()
	{
		m_stack.pop_back();

		if (m_stack.empty()) // if all sections are closed
			throw ParsingFailure("root section is closed", getLineNumber());
	}

protected:

	/// @brief Open brace.
	void brace_open()
	{
		m_brace_depth += 1;
	}


	/// @brief Close brace.
	/**
	@throw ParserT::SyntaxError If brace is closed before opening.
	*/
	void brace_close()
	{
		m_brace_depth -= 1;
		if (m_brace_depth < 0)
			throw SyntaxError("unexpected symbol (>)",
				top().fullName(), getLineNumber());
	}

protected:

	/// @brief Get the unquoted token from input stream.
	/**
	@param[in,out] is The input stream.
	@param[out] token The token.
	*/
	static void get_pure_token(IStream &is, String &token)
	{
		std::ios_base::iostate state = std::ios_base::goodbit;
		const typename IStream::sentry ok(is, true);

		if (ok)
		{
			for (;;)
			{
				typename StrTraits::int_type meta = is.peek();
				if (StrTraits::eq_int_type(meta, StrTraits::eof()))
				{
					state |= std::ios_base::eofbit;
					break;
				}

				typename StrTraits::char_type cx = StrTraits::to_char_type(meta);
				if (details::CharConst<Char>::is_delim(cx))
					break;

				is.ignore();
				token += cx;
			}
		}

		is.setstate(state);
	}


	/// @brief Get the quoted token from input stream.
	/**
	@param[in,out] is The input stream.
	@param[out] token The token.
	@param[in] quote The quote character. Should be '\"' or '\''.
	*/
	static void get_quoted_token(IStream &is, String &token, Char quote)
	{
		std::ios_base::iostate state = std::ios_base::goodbit;
		const typename IStream::sentry ok(is, true);

		if (ok)
		{
			// ignore first quote char (if present)
			if (StrTraits::eq_int_type(is.peek(),
				StrTraits::to_int_type(quote)))
					is.ignore();

			for (;;)
			{
				typename StrTraits::int_type meta = is.peek();
				if (StrTraits::eq_int_type(meta, StrTraits::eof()))
				{
					state |= std::ios_base::eofbit;
					break;
				}

				typename StrTraits::char_type cx = StrTraits::to_char_type(meta);
				if (StrTraits::eq(cx, quote))
				{
					is.ignore();

					// test for second quote
					meta = is.peek();
					if (StrTraits::eq_int_type(meta, StrTraits::eof()))
					{
						state |= std::ios_base::eofbit;
						break;
					}

					cx = StrTraits::to_char_type(meta);
					if (!StrTraits::eq(cx, quote))
						break;
				}

				is.ignore();
				token += cx;
			}
		}

		is.setstate(state);
	}


	/// @brief Parse token from input stream.
	/**
			This method parses the next token from the input stream and counts the lines.

	@param[in,out] is The input stream.
	@param[out] token The token.
	*/
	void get_token(IStream &is, String &token)
	{
		typedef details::CharConst<Char> ChConst;

		skip_ws(is);
		token.resize(0); // (!) clear

		typename StrTraits::int_type meta = is.peek();
		if (!StrTraits::eq_int_type(meta, StrTraits::eof()))
		{
			typename StrTraits::char_type cx = StrTraits::to_char_type(meta);

			if (StrTraits::eq(cx, ChConst::DQUOTE)) // "token"
				get_quoted_token(is, token, ChConst::DQUOTE);
			else if (StrTraits::eq(cx, ChConst::SQUOTE)) // 'token'
				get_quoted_token(is, token, ChConst::SQUOTE);
			else // one word
				get_pure_token(is, token);
		}

		{ // number of lines per token
			typename String::size_type found = 0;

			for (;;)
			{
				found = token.find(ChConst::ENDLINE, found);
				if (found != String::npos)
				{
					m_line_counter += 1;
					found += 1;
				}
				else
					break;
			}
		}
	}


	/// @brief Parse the configuration value.
	/**
			This method assigns the configuration value if present.

	@param[in,out] is The input stream.
	@param[in,out] cfg The configuration.
	@return @b true If value is assigned, otherwise @b false
	*/
	bool parse_value(IStream &is, ItemT<String> &cfg)
	{
		typedef details::CharConst<Char> ChConst;

		skip_ws(is);

		typename StrTraits::int_type meta = is.peek();
		if (!StrTraits::eq_int_type(meta, StrTraits::eof()))
		{
			typename StrTraits::char_type cx = StrTraits::to_char_type(meta);
			if (StrTraits::eq(cx, ChConst::EQUAL))
			{
				is.ignore(); // ignore (=)
				get_token(is, cfg.val());
				return true;
			}
		}

		return false;
	}


	/// @brief Skip the whole line.
	/**
			This method ignores the comment line.

	@param[in,out] is The input stream.
	*/
	void skip_comment(IStream &is)
	{
		typedef details::CharConst<Char> ChConst;

		is.ignore(INT_MAX,
			StrTraits::to_int_type(ChConst::ENDLINE));
		m_line_counter += 1;
	}


	/// @brief Skip the whitespaces.
	/**
			This method skips the whitespaces and counts the lines.

	@param[in,out] is The input stream.
	@return The number of skipped lines.
	*/
	long skip_ws(IStream &is)
	{
		std::ios::iostate state = std::ios::goodbit;
		const typename IStream::sentry guard(is, true);
		long N_lines = 0;

		if (guard)
		{
			typedef details::CharConst<Char> ChConst;
			typedef std::ctype<Char> Facet;

			const Facet &fac = OMNI_USE_FACET(is.getloc(), Facet);

			try
			{
				for (typename StrTraits::int_type meta = is.rdbuf()->sgetc();
					; meta = is.rdbuf()->snextc())
				{
					if (StrTraits::eq_int_type(meta, StrTraits::eof()))
					{
						state |= std::ios::eofbit;
						break;
					}

					typename StrTraits::char_type cx = StrTraits::to_char_type(meta);
					if (StrTraits::eq(cx, ChConst::ENDLINE))
						N_lines += 1;
					else if (!fac.is(Facet::space, cx))
						break;
				}
			}
			catch (...)
			{
				state |= std::ios::badbit;
			}
		}

		is.setstate(state);
		m_line_counter += N_lines;
		return N_lines;
	}

public:

	/// @brief Get the current line number.
	/**
	@return The current line number.
	*/
	long getLineNumber() const
	{
		return m_line_counter;
	}

private:
	std::vector<ItemT<String>*> m_stack; ///< @brief The configuration stack.

private:
	long m_line_counter;  ///< @brief The line counter.
	long m_brace_depth;   ///< @brief The brace depth.
};

		} // io namespace


///////////////////////////////////////////////////////////////////////////////
/** @relates ItemT
@brief Parse the configuration from the input stream.

@param[in] is The input stream.
@param[out] cfg The configuration.
@return The input stream.
@throw ThisType::ParsingFailure if the input stream is invalid.
*/
template<typename Ch, typename Tr, typename Str> inline
	std::basic_istream<Ch,Tr>& operator>>(std::basic_istream<Ch,Tr> &is, ItemT<Str> &cfg)
{
	ItemT<Str> tmp;
	io::ParserT<Str> parser(tmp);
	parser.parse(is);

	tmp.swap(cfg);
	return is;
}

	} // ParserT


	// WriterT
	namespace conf
	{
		namespace io
		{

///////////////////////////////////////////////////////////////////////////////
/// @brief The configuration writer.
/**
	TODO: writer description
*/
template<typename Str>
class WriterT
{
public:
	typedef Str String; ///< @brief The string type.
	typedef typename Str::traits_type StrTraits; ///< @brief The string traits type.
	typedef typename StrTraits::char_type Char;  ///< @brief The character type.
	typedef std::basic_ostream<Char, StrTraits> OStream; ///< @brief The output stream.

public:
	typedef err::WritingFailureT<String> WritingFailure;      ///< @brief Configuration writing exception.
	typedef err::NameIsEmptyT<String> NameIsEmpty;            ///< @brief The name is empty.

public:

	/// @brief The default constructor.
	/**
			The following default parameters are used by default:
				- tab size is 2 spaces
				- double quoted
				- print empty values
	*/
	WriterT()
		: tabSize(2), rootName(false),
		  newLine(true), doubleQuote(true),
		  printEmptyValues(true), indent(0)
{}


public:

	/// @brief Print the configuration to the output stream.
	/**
	@param[in,out] os The output stream.
	@param[in] cfg The configuration.
	@return The output stream.

	@throw WriterT::NameIsEmpty if configuration name is empty.
	*/
	OStream& print(OStream &os, ItemT<String> const& cfg) const
	{
		if (!cfg.empty() || is_root(cfg)) // (!) root is always section
		{
			open_section(os, cfg);

			typedef typename ItemT<String>::ConstIterator Iterator;
			Iterator i = cfg.begin();
			Iterator const ie = cfg.end();
			for (; i != ie; ++i)
				print(os, *i);

			close_section(os, cfg);
		}
		else
			put_element(os, cfg);

		return os;
	}

protected:

	/// @brief Section begin.
	/**
	@param[in,out] os The output stream.
	@param[in] cfg The configuration.
	@throw WriterT::NameIsEmpty If configuration name is empty.
	*/
	virtual void open_section(OStream &os, ItemT<String> const& cfg) const
	{
		typedef details::CharConst<Char> ChConst;
		const Char quote = doubleQuote ? ChConst::DQUOTE : ChConst::SQUOTE;

		if (rootName || !is_root(cfg))
		{
			if (cfg.name().empty())
				throw NameIsEmpty(cfg.fullName());

			if (newLine && !is_front(cfg))
				os.put(ChConst::ENDLINE);

			// open section
			put_indent(os, ChConst::SPACE).put(ChConst::BEGIN);
			if (need_quote(cfg.name()))
				put_qstring(os, cfg.name(), quote);
			else
				os << cfg.name();
			if (!cfg.val().empty())
			{
				os.put(ChConst::SPACE).put(ChConst::EQUAL).put(ChConst::SPACE);
				put_qstring(os, cfg.val(), quote); // (!) always quoted
			}
			os.put(ChConst::END).put(ChConst::ENDLINE);

			// increase indent
			indent += tabSize;
		}
	}


	/// @brief Section end.
	/**
	@param[in,out] os The output stream.
	@param[in] cfg The configuration.
	@throw WriterT::NameIsEmpty If configuration name is empty.
	*/
	virtual void close_section(OStream &os, ItemT<String> const& cfg) const
	{
		typedef details::CharConst<Char> ChConst;
		const Char quote = doubleQuote ? ChConst::DQUOTE : ChConst::SQUOTE;

		if (rootName || !is_root(cfg))
		{
			if (cfg.name().empty())
				throw NameIsEmpty(cfg.fullName());

			// decrease indent
			indent -= tabSize;

			// close section
			put_indent(os, ChConst::SPACE).put(ChConst::BEGIN).put(ChConst::CLOSE);
			if (need_quote(cfg.name()))
				put_qstring(os, cfg.name(), quote);
			else
				os << cfg.name();
			os.put(ChConst::END);

			os.put(ChConst::ENDLINE);
		}
		else
			os.put(ChConst::ENDLINE);
	}


	/// @brief Put the empty configuration.
	/**
	@param[in,out] os The output stream.
	@param[in] cfg The empty configuration.
	@throw WriterT::NameIsEmpty If configuration name is empty.
	*/
	virtual void put_element(OStream &os, ItemT<String> const& cfg) const
	{
		typedef details::CharConst<Char> ChConst;
		const Char quote = doubleQuote ? ChConst::DQUOTE : ChConst::SQUOTE;

		assert(cfg.empty() && "configuration should be empty");

		if (cfg.name().empty())
			throw NameIsEmpty(cfg.fullName());

		put_indent(os, ChConst::SPACE);

		// print "key"="val"
		if (need_quote(cfg.name()))
			put_qstring(os, cfg.name(), quote);
		else
			os << cfg.name();

		// print the value
		if (printEmptyValues || !cfg.val().empty())
		{
			os.put(ChConst::SPACE).put(ChConst::EQUAL).put(ChConst::SPACE);
			put_qstring(os, cfg.val(), quote); // (!) always quoted
		}

		os.put(ChConst::ENDLINE);
	}

protected:

	/// @brief Is quote needed?
	/**
			This static method checks the input string. If it contains any
		delimiters then the method returns @b true.

	@param[in] str The string.
	@return @b true if string need to be quoted, otherwise @b false.
	*/
	static bool need_quote(String const& str)
	{
		typedef details::CharConst<Char> ChConst;

		const size_t N = str.size();
		for (size_t i = 0; i < N; ++i)
			if (ChConst::is_delim(str[i]))
				return true;

		return false;
	}


	/// @brief Print the quoted string.
	/**
			This method prints the quoted string to the output stream.
		If string contains the quotes it will be doudled. For example,
		the following string

	@code
		this is "simple" text
	@endcode

			will be printed as

	@code
		"this is ""simple"" text"
	@endcode


	@param[in,out] os The output stream.
	@param[in] str The string to print.
	@param[in] quote The quote character. Should be '\"' or '\''.
	@return The output stream.
	*/
	static OStream& put_qstring(OStream &os, String const& str, Char quote)
	{
		os.put(quote);

		const size_t N = str.size();
		for (size_t i = 0; i < N; ++i)
		{
			if (StrTraits::eq(str[i], quote))
				os.put(quote);
			os.put(str[i]);
		}

		os.put(quote);
		return os;
	}


	/// @brief Put the indent string.
	/**
			This method prints the current indent number of spaces.

	@param[in,out] os The output stream.
	@param[in] space The space character.
	@return The output stream.
	*/
	OStream& put_indent(OStream &os, Char space) const
	{
		for (size_t i = 0; i < indent; ++i)
			os.put(space);

		return os;
	}

protected:

	/// @brief Is configuration a root?
	/**
			This method checks if the input configuration is a root, i.e. hasn't parent.

	@param[in] cfg The configuration.
	@return @b true if the configuration is a root, otherwise @b false.
	*/
	static bool is_root(ItemT<String> const& cfg)
	{
		return (0 == cfg.parent());
	}


	/// @brief Is configuration a first child?
	/**
			This method checks if the input configuration is a first child.

	@param[in] cfg The configuration.
	@return @b true if the configuration is a first child, otherwise @b false.
	*/
	static bool is_front(ItemT<String> const& cfg)
	{
		const ItemT<String> *parent = cfg.parent();

		if (parent && !parent->empty())
			if (&parent->front() == &cfg)
				return true;

		return false;
	}


	/// @brief Is configuration a last child?
	/**
			This method checks if the input configuration is a last child.

	@param[in] cfg The configuration.
	@return @b true if the configuration is a last child, otherwise @b false.
	*/
	static bool is_back(ItemT<String> const& cfg)
	{
		const ItemT<String> *parent = cfg.parent();

		if (parent && !parent->empty())
			if (&parent->back() == &cfg)
				return true;

		return false;
	}

public:
	size_t tabSize;   ///< @brief Number of spaces per one indent.

	bool rootName; ///< @brief Print the root configuration name.
	bool newLine;  ///< @brief Put the new line before configurations.
	bool doubleQuote; ///< @brief Use doule-quote instead of single-qoute.
	bool printEmptyValues; ///< @brief Print the values even it is empty.

public:
	mutable size_t indent; ///< @brief The current indent.
};

		} // io namespace


///////////////////////////////////////////////////////////////////////////////
/** @relates ItemT
@brief Print the configuration to the output stream.

		This operator prints the configuration to the output stream.

	There is default writer is used.

@param[in] os The output stream.
@param[in] cfg The configuration.
@return The output stream.
@throw ThisType::WritingFailure if the input stream is invalid.
*/
template<typename Ch, typename Tr, typename Str> inline
	std::basic_ostream<Ch,Tr>& operator<<(std::basic_ostream<Ch,Tr> &os, ItemT<Str> const& cfg)
{
	io::WriterT<Str> writer;
	writer.print(os, cfg);
	return os;
}

	} // WriterT


	// CharConst
	namespace conf
	{
		namespace details
		{

template<typename Ch>
	class CharConst;


///////////////////////////////////////////////////////////////////////////////
/// @brief The character constants (wchar_t).
/**
		This template class contains the set of character constants
	specialized for @b wchar_t and @b char. These constants are used
	during configuration parsing and/or writing.

		The name separator SEPARATOR() is used
	by omni::conf::ItemT::fullName() method.
*/
template<>
	class CharConst<wchar_t>
{
public:
	typedef wchar_t Char; ///< @brief The character type.

public: // string constants

	/// @brief Get default name separator.
	static Char const* SEPARATOR()
	{
		return L":";
	}

	/// @brief Is delimiter?
	/**
	@param[in] ch The character to test.
	@return @b true if the input character is delimiter, otherwise @b false.
	*/
	static bool is_delim(Char ch)
	{
		return ::iswspace(ch)
			|| (COMMENT == ch)
			|| (EQUAL == ch)
			|| (BEGIN == ch)
			|| (CLOSE == ch)
			|| (END == ch)
			|| (SQUOTE == ch)
			|| (DQUOTE == ch);
	}


	/// @brief Write formated output.
	/**
	@param[in] buf The output buffer.
	@param[in] len The maximum number of characters to write.
	@param[in] fmt The format string.
	@param[in] args The pointer to list of arguments.
	*/
	static int formatv(Char *buf, size_t len, Char const* fmt, va_list args)
	{
		return vswprintf(buf, len, fmt, args);
	}

public: // char constants

	enum Constants
	{
		ENDLINE = L'\n',  ///< @brief The new line character.
		SPACE   = L' ',   ///< @brief The space character.
		COMMENT = L'#',   ///< @brief The comment character.
		EQUAL   = L'=',   ///< @brief The equal character.
		BEGIN   = L'<',   ///< @brief The section begin.
		CLOSE   = L'/',   ///< @brief The section close.
		END     = L'>',   ///< @brief The section end.
		SQUOTE  = L'\'',  ///< @brief The single quote character.
		DQUOTE  = L'\"'   ///< @brief The double quote character.
	};
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The character constants (char).
/**
		This template class contains the set of character constants
	specialized for @b wchar_t and @b char. These constants are used
	during configuration parsing and/or writing.

		The name separator SEPARATOR() is used
	by omni::conf::ItemT::fullName() method.
*/
template<>
	class CharConst<char>
{
public:
	typedef char Char; ///< @brief The character type.

public: // string constants

	/// @brief Get default name separator.
	static Char const* SEPARATOR()
	{
		return ":";
	}

	/// @brief Is delimiter?
	/**
	@param[in] ch The character to test.
	@return @b true if the input character is delimiter, otherwise @b false.
	*/
	static bool is_delim(Char ch)
	{
		return ::isspace(ch)
			|| (COMMENT == ch)
			|| (EQUAL == ch)
			|| (BEGIN == ch)
			|| (CLOSE == ch)
			|| (END == ch)
			|| (SQUOTE == ch)
			|| (DQUOTE == ch);
	}


	/// @brief Write formated output.
	/**
	@param[in] buf The output buffer.
	@param[in] len The maximum number of characters to write.
	@param[in] fmt The format string.
	@param[in] args The pointer to list of arguments.
	*/
	static int formatv(Char *buf, size_t len, Char const* fmt, va_list args)
	{
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable:4996) // disable: "Consider using vsnprintf_s instead" warning
#endif

		return vsnprintf(buf, len, fmt, args);

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
	}

public: // char constants

	enum Constants
	{
		ENDLINE = '\n',  ///< @brief The new line character.
		SPACE   = ' ',   ///< @brief The space character.
		COMMENT = '#',   ///< @brief The comment character.
		EQUAL   = '=',   ///< @brief The equal character.
		BEGIN   = '<',   ///< @brief The section begin.
		CLOSE   = '/',   ///< @brief The section close.
		END     = '>',   ///< @brief The section end.
		SQUOTE  = '\'',  ///< @brief The single quote character.
		DQUOTE  = '\"'   ///< @brief The double quote character.
	};
};


///////////////////////////////////////////////////////////////////////////////
/// @brief Write formated output to the string.
/**
@param[out] out The output string.
@param[in] fmt The format string.
@param[in] args The pointer to list of arguments.
*/
template<typename Str, typename Ch>
void formatv(Str &out, Ch const *fmt, va_list args)
{
	std::vector<Ch> buf(16);

	int ret = -1;
	while (true)
	{
#if defined(va_copy)
		// make copy of arguments, otherwise next iteration will fail
		// because 'args' will be undefined after vsprintf() call
		va_list xargs;
		va_copy(xargs, args);
		ret = CharConst<Ch>::formatv(&buf[0], buf.size(), fmt, xargs);
		va_end(xargs);
#else
		ret = CharConst<Ch>::formatv(&buf[0], buf.size(), fmt, args);
#endif // va_copy

		if (ret < 0 || buf.size() < size_t(ret)) // buffer too small
			buf.resize(2*buf.size());
		else
			break;
	}

	out.assign(buf.begin(),
		buf.begin() + ret);
}

		} // details namespace
	} // CharConst

} // omni namespace


///////////////////////////////////////////////////////////////////////////////
/** @page omni_config Configuration tools

		omni::conf::ItemT class represents a very simple hierarchical data base.
	It can be used as an universal data format for various simulator's configurations
	or output results.

		Each instance of omni::conf::ItemT (called as configuration) contains name,
	value and optional any number of child configurations.

		The configuration's text format is similar to the XML data format:

@code
	<data>
		time = "1000" # [seconds]
	</data>
@endcode

		If name or value contains spaces or any other special symbols, then
	it should be quoted by " or '.


@section omni_config_section Configurations

	There are a few method to define configuration section:

@code
	<A>
		name = "val"
	</A>

	<B name = "val" />

	<C name = "val" C>
@endcode

		The last method is used to parse XML-like comments (<? ... ?>).

@section omni_config_comment Comments

		The line after the "#" char is ignored.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/

#endif // __OMNI_CONF_HPP_
