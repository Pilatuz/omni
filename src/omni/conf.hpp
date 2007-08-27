//////////////////////////////////////////////////////////////////////////
//		This material is provided "as is", with absolutely no warranty
//	expressed or implied. Any use is at your own risk.
//
//		Permission to use or copy this software for any purpose is hereby
//	granted without fee, provided the above notices are retained on all
//	copies. Permission to modify the code and to distribute modified code
//	is granted, provided the above notices are retained, and a notice that
//	the code was modified is included with the above copyright notice.
//
//		http://omni.sourceforge.net
//////////////////////////////////////////////////////////////////////////
/** @file
	@brief Configuration tools.

		This header file contains a several tools,
	that helps to work with configurations.

@author Sergey Polichnoy
*/
#ifndef __OMNI_CONF_HPP_
#define __OMNI_CONF_HPP_

#include <omni/defs.hpp>

#include <algorithm>
#include <stdexcept>
#include <assert.h>
#include <sstream>
#include <string>
#include <vector>

namespace omni
{
	// forward declarations and typedefs...
	namespace conf
	{
		// main classes
		template<typename Str> class ElementT;
		template<typename Str> class SectionT;
		template<typename Str> class ElementListT;
		template<typename Str> class SectionListT;

		// exceptions
		namespace err
		{
			template<typename Str> class FailureT;
			template<typename Str>   class AccessFailureT;
			template<typename Str>     class ElementNotFoundT;
			template<typename Str>     class NameIsAmbiguousT;
			template<typename Str>   class ParsingFailureT;
			template<typename Str>     class SyntaxErrorT;
			template<typename Str>     class NameMismatchT;
			template<typename Str>   class WritingFailureT;
			template<typename Str>     class NameIsEmptyT;
		} // exceptions

		// input/output
		namespace io
		{
			template<typename Str> class ParserT;
			template<typename Str> class WriterT;
		} // input/output

		// implementation defined
		namespace details
		{
			template<typename Ch> class CharConst;
			template<typename Val> struct ConstTraits;
			template<typename Val> struct NConstTraits;
			template<typename Base, typename Tr> class Iterator;
			template<typename In> bool equal(In, In, In);
		} // implementation defined


#if OMNI_UNICODE
	/// @brief Default string container.
	typedef std::wstring String;
#else
	/// @brief Default string container.
	typedef std::string String;
#endif // OMNI_UNICODE

// main classes
typedef ElementT<String> Element; ///< @brief Configuration element.
typedef SectionT<String> Section; ///< @brief Configuration section.
typedef ElementListT<String> ElementList; ///< @brief List of configuration elements.
typedef SectionListT<String> SectionList; ///< @brief List of configuration sections.

namespace err
{
	typedef FailureT<String>          Failure;          ///< @brief Basic exception.
	typedef AccessFailureT<String>    AccessFailure;    ///< @brief Access failure.
	typedef ElementNotFoundT<String>  ElementNotFound;  ///< @brief Element not found.
	typedef NameIsAmbiguousT<String>  NameIsAmbiguous;  ///< @brief Ambiguous element name.
	typedef ParsingFailureT<String>   ParsingFailure;   ///< @brief Basic parsing failure.
	typedef SyntaxErrorT<String>      SyntaxError;      ///< @brief Invalid syntax.
	typedef NameMismatchT<String>     NameMismatch;     ///< @brief Mismatch failure.
	typedef WritingFailureT<String>   WritingFailure;   ///< @brief Basic writing failure.
	typedef NameIsEmptyT<String>      NameIsEmpty;      ///< @brief Empty name failure.
}

namespace io
{
	typedef ParserT<String> Parser; ///< @brief Basic parser.
	typedef WriterT<String> Writer; ///< @brief Basic writer.
}

	} // forward declarations and typedefs


	// ElementT<> template class...
	namespace conf
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Configuration element.
/**
		This class represents a custom configuration element. The configuration
	element contains name, value and optional prefix and suffix comments.
	For example:

@code
# prefix comment
elem_name = "elem_value" # suffix comment
@endcode

		If the example will be parsed, then the element
	will have the following properties:
		- name() will be equal to the "elem_name",
		- val() will be equal to the "elem_value",
		- prefix() will be equal to the " prefix comment",
		- suffix() will be equal to the " suffix comment".

		The fullName() method returns a configuration element's full name,
	including full path (i.e. names of all parents, see SectionT class).

		It is possible to implicit assign the configuration element's value.

@code
	void f(Element &elem)
	{
		elem.val() = "new value";
		// -or-
		elem = "new value";
	}
@endcode

		The equal() method compares two configuration elements.
	Two configuration elements are equal if they have the same names,
	same values, and same prefix and suffix comments.
	The @b == and @b != operators also available.

		The @a Str template parameter defines the string container type.
	A usual @a Str types are @a std::wstring or @a std::string.

@see @ref omni_config
*/
template<typename Str>
class ElementT {
	typedef ElementT<Str> ThisType;
	typedef SectionT<Str> ParentType;

	friend class io::WriterT<Str>;
	friend class SectionT<Str>;

	typedef Str implementation_defined_1;
	typedef typename Str::traits_type Traits;
	typedef typename Traits::char_type implementation_defined_2;

//////////////////////////////////////////////////////////////////////////
/// @name Main typedefs
/// @{
public:
	typedef implementation_defined_1 String;  ///< @brief String type.
	typedef implementation_defined_2 Char;    ///< @brief Char type.

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Constructors & destructor
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
/**
		This constructor creates a empty configuration element:
	the name, value, prefix and suffix comments are empty.
*/
	ElementT()
		: m_parent(0) // will be set later
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Create with specified name.
/**
		This constructor initializes element name by @a element_name.
	The value, prefix and suffix comments are empty.

@param[in] element_name The element's name.
*/
	explicit ElementT(const String &element_name)
		: m_parent(0), // will be set later
		  m_name(element_name)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Create with specified name.
/**
		This constructor initializes element name by @a element_name.
	The value, prefix and suffix comments are empty.

@param[in] element_name The element's name.
*/
	explicit ElementT(const Char *element_name)
		: m_parent(0), // will be set later
		  m_name(element_name)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Copy constructor.
/**
		This constructor initializes the element by @a other.

@param[in] other The other element.
*/
	ElementT(const ThisType &other)
		: m_parent(0), // will be set later
		  m_name(other.m_name),
		  m_val(other.m_val),
		  m_prefix(other.m_prefix),
		  m_suffix(other.m_suffix)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Destructor.
/**
		The virtual destructor is used for derived classes.
*/
	virtual ~ElementT()
	{}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Assignments
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Assignment operator.
/**
@param[in] other The other element.
@return Self reference.
*/
	ThisType& operator=(const ThisType &other)
	{
		ThisType t(other);
		swap(t);

		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign a new element's value.
/**
		This assignment operator allows to implicit set the element's value.

@param[in] new_val The new element's value.
@return Self reference.
*/
	ThisType& operator=(const String &new_val)
	{
		m_val = new_val;
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign a new element's value.
/**
		This assignment operator allows to implicit set the element's value.

@param[in] new_val The new element's value.
@return Self reference.
*/
	ThisType& operator=(const Char *new_val)
	{
		m_val = new_val;
		return *this;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Name & value
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get element's name.
/**
		This method returns a constant reference to the element's name.

@return Constant name reference.
*/
	const String& name() const
	{
		return m_name;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get/set element's name.
/**
		This method returns a non-constant reference to the element's name.

@return Non-constant name reference.
*/
	String& name()
	{
		return m_name;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element's value.
/**
		This method returns a constant reference to the element's value.

@return Constant value reference.
*/
	const String& val() const
	{
		return m_val;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get/set element's value.
/**
		This method returns a non-constant reference to the element's value.

@return Non-constant value reference.
*/
	String& val()
	{
		return m_val;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Prefix & suffix comments
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get element's prefix comment.
/**
		This method returns a constant reference
	to the element's prefix comment.

@return Constant prefix comment reference.
*/
	const String& prefix() const
	{
		return m_prefix;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get/set element's prefix comment.
/**
		This method returns a non-constant reference
	to the element's prefix comment.

@return Non-constant prefix comment reference.
*/
	String& prefix()
	{
		return m_prefix;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element's suffix comment.
/**
		This method returns a constant reference
	to the element's suffix comment.

@return Constant suffix comment reference.
*/
	const String& suffix() const
	{
		return m_suffix;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get/set element's suffix comment.
/**
		This method returns a non-constant reference
	to the element's suffix comment.

@return Non-constant suffix comment reference.
*/
	String& suffix()
	{
		return m_suffix;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Full name
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get element's full name.
/**
		This method returns the element's full name, including all parents.
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
@return The element's full name.
*/
	const String fullName(const String &sep) const
	{
		if (m_parent)
		{
			String full_name = m_parent->fullName(sep);
			if (!full_name.empty())
			{
				full_name += sep;
				full_name += m_name;
				return full_name;
			}
		}

		return m_name;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element's full name.
/**
		This method returns the element's full name, including all parents.
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
@return The element's full name.
*/
	const String fullName(const Char *sep) const
	{
		if (m_parent)
		{
			String full_name = m_parent->fullName(sep);
			if (!full_name.empty())
			{
				full_name += sep;
				full_name += m_name;
				return full_name;
			}
		}

		return m_name;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element's full name.
/**
		This method returns the element's full name
	using default separator ":".

@return The element's full name.
*/
	const String fullName() const
	{
		return fullName(details::CharConst<Char>::SEPARATOR);
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Auxiliary
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Are two elements equal?
/**
		The two elements are equal if they have the same names,
	the same values and the same prefix and suffix comments.

@param[in] other The other element.
@return @b true if two elements are equal, otherwise @b false.
*/
	bool equal(const ThisType &other) const
	{
		return (m_name == other.m_name)
			&& (m_val == other.m_val)
			&& (m_prefix == other.m_prefix)
			&& (m_suffix == other.m_suffix);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Swap two elements.
/**
		This method swaps the two elements.
	The parents are not changed.

@param[in,out] other The other element.
*/
	void swap(ThisType &other)
	{
		// (!) do not swap parents

		m_name.swap(other.m_name);
		m_val.swap(other.m_val);

		m_prefix.swap(other.m_prefix);
		m_suffix.swap(other.m_suffix);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get empty element.
/**
		This static method returns a constant reference to the empty element.
	This empty element may be used as default value.

@code
	void f(const Section &cfg)
	{
		const Element &elem = cfg.elements.get("param", Element::EMPTY());

		// ...
	}
@endcode

@return The empty element.
*/
	static const ThisType& EMPTY()
	{
		static ThisType g_empty;
		return g_empty;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Is it section?
/**
		This method is used for dynamic type identification.

@return Always @b false.
*/
	virtual bool is_section() const
	{
		return false;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get parent section.
/**
		This method returns a pointer to the parent
	or null if element has no parent (i.e. element is root).

@return Pointer to the parent or null.
*/
	const ParentType* parent() const
	{
		return m_parent;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Set parent section.
/**
		This method sets the new element's parent.

@param[in] new_parent Pointer to the parent or null.
*/
	void set_parent(ParentType *new_parent)
	{
		m_parent = new_parent;
	}


private:
	ParentType *m_parent; ///< @brief The parent or null.

	String m_name;        ///< @brief The name.
	String m_val;         ///< @brief The value.

	String m_prefix;      ///< @brief The prefix comment.
	String m_suffix;      ///< @brief The suffix comment.
};


//////////////////////////////////////////////////////////////////////////
/// @brief Are two elements equal?
/** @relates ElementT

		This operator is equivalent to the:

@code
	x.equal(y);
@endcode

@param[in] x The first element.
@param[in] y The second element.
@return @b true if two elements are equal, otherwise @b false.
*/
template<typename Str> inline
	bool operator==(const ElementT<Str> &x, const ElementT<Str> &y)
{
	return x.equal(y);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Are two elements non-equal?
/** @relates ElementT

		This operator is equivalent to the:

@code
	!x.equal(y);
@endcode

@param[in] x The first element.
@param[in] y The second element.
@return @b true if two elements are non-equal, otherwise @b false.
*/
template<typename Str> inline
	bool operator!=(const ElementT<Str> &x, const ElementT<Str> &y)
{
	return !x.equal(y);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Swap two elements.
/** @relates ElementT

		This function is equivalent to the:

@code
	x.swap(y);
@endcode

@param[in,out] x The first element.
@param[in,out] y The second element.
*/
template<typename Str> inline
	void swap(ElementT<Str> &x, ElementT<Str> &y)
{
	x.swap(y);
}

	} // ElementT<> template class


	// SectionT<> template class...
	namespace conf
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Configuration section.
/**
		The configuration section contains the list of the child elements
	and the list of the child sections.

		Because the SectionT class is derived from the ElementT class,
	it also contains name, value (!), prefix and suffix comments.

		Единый список секций и параметров служит для организации проивольного
	порядка секций и параметров. Т.е. секции и параметры могут чередоваться.
	Так, например, при чтении секции из файла конфигурации сохраняется
	относительный порядок дочерних секций и параметров.

		The @a Str template parameter defines the string container type.
	A usual @a Str types are @a std::wstring or @a std::string.

@see @ref omni_config
*/
template<typename Str>
class SectionT: public ElementT<Str> {
	typedef ElementT<Str> inherited;
	typedef SectionT<Str> ThisType;

	friend class ElementListT<Str>;
	friend class SectionListT<Str>;
	friend class io::WriterT<Str>;

//////////////////////////////////////////////////////////////////////////
/// @name Main typedefs
/// @{
public:
	typedef typename inherited::String String;  ///< @brief String type.
	typedef typename inherited::Char   Char;    ///< @brief Char type.

	typedef ElementListT<String> ElementList; ///< @brief List of child elements.
	typedef SectionListT<String> SectionList; ///< @brief List of child sections.

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Constructors & destructor
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
/**
		The created section has no child elements and no child sections.
*/
	SectionT()
	{
		sections.set_owner(this);
		elements.set_owner(this);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Create with specified name.
/**
		This constructor creates the empty section with name @a section_name.

@param[in] section_name The section's name.
*/
	explicit SectionT(const String &section_name)
		: inherited(section_name)
	{
		sections.set_owner(this);
		elements.set_owner(this);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Create with specified name.
/**
		This constructor creates the empty section with name @a section_name.

@param[in] section_name The section's name.
*/
	explicit SectionT(const Char *section_name)
		: inherited(section_name)
	{
		sections.set_owner(this);
		elements.set_owner(this);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Copy constructor.
/**
		This constructor initializes the section by @a other.

@param[in] other The other section.
*/
	SectionT(const ThisType &other)
		: inherited(other)
	{
		sections.set_owner(this);
		elements.set_owner(this);

		// TODO: speed optimization if possible?

		// copy all child elements
		typename Container::const_iterator i = other.m_childs.begin();
		typename Container::const_iterator ie = other.m_childs.end();
		for (; i != ie; ++i)
		{
			const inherited &elem = *(*i);

			if (elem.is_section())
				sections.push_back(static_cast<const ThisType&>(elem));
			else
				elements.push_back(elem);
		}
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Destructor.
/**
		The destructor removes all child elements and all child sections.
*/
	virtual ~SectionT()
	{
		sections.clear();
		elements.clear();
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Assignments
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Assignment operator.
/**
@param[in] other The other section.
@return Self reference.
*/
	ThisType& operator=(const ThisType &other)
	{
		ThisType t(other);
		swap(t);

		return *this;
	}

	// using inherited::operator=;


//////////////////////////////////////////////////////////////////////////
/// @brief Merge configuration.
/**
		This method replaces the some child elements and some child sections
	with corresponding child elements and child sections of @a other section.
	The only elements with the same names will be replaces. If element with
	specified name not exists yet, it will be added.

	For examples, let two section A and B:

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
	</B>
@endcode

@param[in] other The other section.
@return Self reference.
*/
	ThisType& merge(const ThisType &other)
	{
		{ // merge child sections
			typename SectionList::const_iterator i = other.sections.begin();
			typename SectionList::const_iterator ie = other.sections.end();

			for (; i != ie; ++i)
				sections[i->name()].merge(*i);
		}

		{ // merge child elements
			typename ElementList::const_iterator i = other.elements.begin();
			typename ElementList::const_iterator ie = other.elements.end();
			for (; i != ie; ++i)
				elements[i->name()] = (*i);
		}

		return *this;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Child elements & child sections
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief List of the child elements.
/**
		Because ElementList has operator()() and operator[]()
	you cat use the following code:

@code
	void f(const Section &cfg)
	{
		cfg.elements().get("elem_name");
		cfg.elements.get("elem_name");
		cfg.elements["elem_name"];
	}
@endcode

@see ElementListT
*/
	ElementList elements;


//////////////////////////////////////////////////////////////////////////
/// @brief List of the child sections.
/**
		Because ElementList has operator()() and operator[]()
	you cat use the following code:

@code
	void f(const Section &cfg)
	{
		cfg.sections().get("child_name");
		cfg.sections.get("child_name");
		cfg.sections["child_name"];
	}
@endcode

@see SectionListT
*/
	SectionList sections;

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Auxiliary
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Are two sections equal?
/**
		The two sections are equal is they have the same names,
	the same values, the same prefix and suffix comments and
	the same child elements and child sections.

@param[in] other The other section.
@return @b true if two sections are equal, otherwise @b false.
*/
	bool equal(const ThisType &other) const
	{
		return inherited::equal(other)
			&& sections.equal(other.sections)
			&& elements.equal(other.elements);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Swap sections.
/**
		This method swaps the two sections.

@param[in,out] other The other section.
*/
	void swap(ThisType &other)
	{
		inherited::swap(other);

		// change child elements
		m_childs.swap(other.m_childs);
		sections.swap(other.sections);
		elements.swap(other.elements);

		{ // change parent of all child elements
			typename Container::iterator i = this->m_childs.begin();
			typename Container::iterator ie = this->m_childs.end();

			for (; i != ie; ++i)
				(*i)->set_parent(this);
		}

		{ // change parent of all child elements
			typename Container::iterator i = other.m_childs.begin();
			typename Container::iterator ie = other.m_childs.end();

			for (; i != ie; ++i)
				(*i)->set_parent(&other);
		}
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get empty section.
/**
		This static method returns a constant reference to the empty section.
	This empty section may be used as default value.

@code
	void f(const Section &cfg)
	{
		const Section &sub_cfg = cfg.sections.get("child", Section::EMPTY());

		// ...
	}
@endcode

@return The empty section.
*/
	static const ThisType& EMPTY()
	{
		static ThisType g_empty;
		return g_empty;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Is it section?
/**
		This method is used for dynamic type identification.

@return Always @b true.
*/
	virtual bool is_section() const
	{
		return true;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Insert child element or child section.
/**
		The element will be inserted into the internal order list.

@param[in] elem The child element or child section.
*/
	void do_insert(inherited *elem)
	{
		m_childs.push_back(elem);
		elem->set_parent(this);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Remove child element or child section.
/**
		The element will be removed from the internal order list.

@param[in] elem The child element or child section.
*/
	void do_remove(inherited *elem)
	{
		typename Container::iterator found = std::find(
			m_childs.begin(), m_childs.end(), elem);
		assert(found != m_childs.end()
			&& "element not exists");

		if (found != m_childs.end())
		{
			m_childs.erase(found);

			assert(this == elem->parent()
				&& "invalid parent");
			elem->set_parent(0);
		}
	}


private:
	typedef std::vector<inherited*> Container; ///< @brief The order list type.

	Container m_childs; ///< @brief The order list.
};


//////////////////////////////////////////////////////////////////////////
/// @brief Are two sections equal?
/** @relates SectionT

		This operator is equivalent to the:

@code
	x.equal(y);
@endcode

@param[in] x The first section.
@param[in] y The second section.
@return @b true if two sections are equal, otherwise @b false.
*/
template<typename Str> inline
	bool operator==(const SectionT<Str> &x, const SectionT<Str> &y)
{
	return x.equal(y);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Are two sections non-equal?
/** @relates SectionT

		This operator is equivalent to the:

@code
	!x.equal(y);
@endcode

@param[in] x The first section.
@param[in] y The second section.
@return @b true if two sections are non-equal, otherwise @b false.
*/
template<typename Str> inline
	bool operator!=(const SectionT<Str> &x, const SectionT<Str> &y)
{
	return !x.equal(y);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Swap two sections.
/** @relates SectionT

		This function is equivalent to the:

@code
	x.swap(y);
@endcode

@param[in,out] x The first section.
@param[in,out] y The second section.
*/
template<typename Str> inline
	void swap(SectionT<Str> &x, SectionT<Str> &y)
{
	x.swap(y);
}

	} // SectionT<> template class


	// ElementListT<> template classes
	namespace conf
	{

//////////////////////////////////////////////////////////////////////////
/// @brief List of configuration elements.
/**
		This is auxiliary class. You can't create instances of this class.
	Instead use SectionT::elements property.

		The class is a container of configuration elements. It contains
	a several manipulators: push_back(), remove(), exists(). It also
	supports iterator methods: begin() and end().

		The class contains operator()(), which returns self reference.
	So, you can use one of the following:

@code
	void f(Section &s)
	{
		s.elements().push_back();
		// -or-
		s.elements.push_back();
	}
@endcode

		To access child element by name you can use one of the overloaded
	get() methods. To access child element value you can use one of the
	overloaded getv() method. Also you can use operator[]():

@code
	// element's value access
	void f(const Section &s)
	{
		s.elements.get("elem_name").val();
		s.elements["elem_name"].val();
		s.elements.getv("elem_name");
	}
@endcode

		You can access element by name only if the name is unique. Otherwise
	you can use begin() and end() methods to iterate all child elements.

@see @ref omni_config
*/
template<typename Str>
class ElementListT: private omni::NonCopyable {
	typedef ElementListT<Str> ThisType;
	typedef SectionT<Str>* OwnerType;

	typedef typename String::allocator_type Allocator;
	typedef std::vector<ElementT<Str>*, typename Allocator::
		template rebind<ElementT<Str>*>::other > Container;

	typedef details::Iterator<typename Container::const_iterator,
		details::ConstTraits< ElementT<Str> > > implementation_defined_1;
	typedef details::Iterator<typename Container::iterator,
		details::NConstTraits< ElementT<Str> > > implementation_defined_2;

	friend class SectionT<Str>;

//////////////////////////////////////////////////////////////////////////
/// @name Main typedefs
/// @{
public:
	typedef ElementT<Str> value_type; ///< @brief Value type.

	typedef const value_type& const_reference; ///< @brief Constant reference.
	typedef       value_type&       reference; ///< @brief Non-constant reference.

	typedef typename value_type::String String;   ///< @brief String type.
	typedef typename value_type::Char Char;       ///< @brief Char type.

	typedef typename Container::size_type size_type; ///< @brief The size type.

/// @}
//////////////////////////////////////////////////////////////////////////

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
/**
		This constructor creates empty list.

	Only friends may create the element list.
*/
	ElementListT()
		: m_owner(0) // will be set later
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Destructor.
/**
		Only friends may destroy the element list.
*/
	~ElementListT()
	{
		assert(empty() && "list not empty");
	}


//////////////////////////////////////////////////////////////////////////
/// @name Iterators
/// @{
public:

	typedef implementation_defined_1 const_iterator; ///< @brief Constant iterator.
	typedef implementation_defined_2 iterator; ///< @brief Non-constant iterator.

//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the list.
/**
@return The constant iterator.
*/
	const const_iterator begin() const
	{
		return const_iterator(m_items.begin());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the list.
/**
@return The non-constant iterator.
*/
	const iterator begin()
	{
		return iterator(m_items.begin());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief End of the list.
/**
@return The constant iterator.
*/
	const const_iterator end() const
	{
		return const_iterator(m_items.end());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief End of the list.
/**
@return The non-constant iterator.
*/
	const iterator end()
	{
		return iterator(m_items.end());
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Front & Back
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief First element.
/**
@return Constant reference.
*/
	const_reference front() const
	{
		return *m_items.front();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief First element.
/**
@return Non-constant reference.
*/
	reference front()
	{
		return *m_items.front();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Last element.
/**
@return Constant reference.
*/
	const_reference back() const
	{
		return *m_items.back();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Last element.
/**
@return Non-constant reference.
*/
	reference back()
	{
		return *m_items.back();
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Get element by name
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name.
/**
		This method returns a element with specified name @a name.
	If the element with that name is not exists or the name
	is not unique the exception will be thrown.

@param[in] name The element's name.
@return Constant reference.
@throw omni::conf::err::ElementNotFoundT If element not found.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	const_reference get(const String &name) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			throw err::ElementNotFoundT<String>(name, m_owner->fullName());

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name.
/**
		This method returns a element with specified name @a name.
	If the element with that name is not exists or the name
	is not unique the exception will be thrown.

@param[in] name The element's name.
@return Constant reference.
@throw omni::conf::err::ElementNotFoundT If element not found.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	const_reference get(const Char *name) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			throw err::ElementNotFoundT<String>(name, m_owner->fullName());

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name or default.
/**
		This method returns a element with specified name @a name.
	If the element with that name is not exists, then the @a def element
	will be returned. If the element's name is not unique the exception
	will be thrown.

@param[in] name The element's name.
@param[in] def The default element.
@return Constant reference.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	const_reference get(const String &name, const_reference def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name or default.
/**
		This method returns a element with specified name @a name.
	If the element with that name is not exists, then the @a def element
	will be returned. If the element's name is not unique the exception
	will be thrown.

@param[in] name The element's name.
@param[in] def The default element.
@return Constant reference.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	const_reference get(const Char *name, const_reference def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name or create.
/**
		Метод возвращает элемент с именем @a name. Если элемента с таким
	именем нет, но установлен флаг @a create, то будет создан новый элемент.
	Если же флаг @a create не установлен, будет сгенерировано исключение.
	Если есть два или более элементов с таким именем, также будет
	сгенерировано исключение.

@param[in] name The element's name.
@param[in] create Create if not exists flag.
@return Non-constant reference.
@throw omni::conf::err::ElementNotFoundT IF element not found and @a create flag not set.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	reference get(const String &name, bool create = false)
	{
		iterator found = find(name, begin());
		if (found == end())
		{
			if (create)
				return push_back(name);
			else
				throw err::ElementNotFoundT<String>(name, m_owner->fullName());
		}

		reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name or create.
/**
		Метод возвращает элемент с именем @a name. Если элемента с таким
	именем нет, но установлен флаг @a create, то будет создан новый элемент.
	Если же флаг @a create не установлен, будет сгенерировано исключение.
	Если есть два или более элементов с таким именем, также будет
	сгенерировано исключение.

@param[in] name The element's name.
@param[in] create Create if not exists flag.
@return Non-constant reference.
@throw omni::conf::err::ElementNotFoundT IF element not found and @a create flag not set.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	reference get(const Char *name, bool create = false)
	{
		iterator found = find(name, begin());
		if (found == end())
		{
			if (create)
				return push_back(name);
			else
				throw err::ElementNotFoundT<String>(name, m_owner->fullName());
		}

		reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Get element's value by name
/// @{

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем
/**
		Метод возвращает значение элемента с именем @a name. Если элемента с
	таким именем нет или есть два или более, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@return Значение найденного элемента списка
@throw omni::config::NotFoundFailureT Если элемент с таким именем не найден
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const String& getv(const String &name) const
	{
		return get(name).val();
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем
/**
		Метод возвращает значение элемента с именем @a name. Если элемента с
	таким именем нет или есть два или более, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@return Значение найденного элемента списка
@throw omni::config::NotFoundFailureT Если элемент с таким именем не найден
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const String& getv(const Char *name) const
	{
		return get(name).val();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем (по умолчанию)
/**
		Метод возвращает значение элемента с именем @a name. Если элемента
	с таким именем нет, то будет возвращено значение @a def. Если с таким
	именем есть два или более элементов, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@param[in] def Возвращаемое значение по умолчанию
@return Значение найденного элемента списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const String& getv(const String &name, const String &def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem.val();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем (по умолчанию)
/**
		Метод возвращает значение элемента с именем @a name. Если элемента
	с таким именем нет, то будет возвращено значение @a def. Если с таким
	именем есть два или более элементов, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@param[in] def Возвращаемое значение по умолчанию
@return Значение найденного элемента списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const String& getv(const Char *name, const String &def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem.val();
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем (по умолчанию)
/**
		Метод возвращает значение элемента с именем @a name. Если элемента
	с таким именем нет, то будет возвращено значение @a def. Если с таким
	именем есть два или более элементов, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@param[in] def Возвращаемое значение по умолчанию
@return Значение найденного элемента списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const Char* getv(const Char *name, const Char *def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem.val().c_str();
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Auxiliary operators
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Элемент с заданным именем
/**
		Метод является псевдонимом для get(name).

@param[in] name Имя элемента списка
@return Константная ссылка на найденный элемент списка
@throw omni::config::NotFoundFailureT Если элемент с таким именем не найден
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const_reference operator[](const String &name) const
	{
		return get(name);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Элемент с заданным именем
/**
		Метод является псевдонимом для get(name).

@param[in] name Имя элемента списка
@return Константная ссылка на найденный элемент списка
@throw omni::config::NotFoundFailureT Если элемент с таким именем не найден
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const_reference operator[](const Char *name) const
	{
		return get(name);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Элемент с заданным именем (с созданием)
/**
		Метод является псевдонимом для get(name, true).
	Т.е. если элемент не существует, он будет создан!

@param[in] name Имя элемента списка
@return Неконстантная ссылка на найденный элемент списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	reference operator[](const String &name)
	{
		// (!) create if not exists
		return get(name, true);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Элемент с заданным именем (с созданием)
/**
		Метод является псевдонимом для get(name, true).
	Т.е. если элемент не существует, он будет создан!

@param[in] name Имя элемента списка
@return Неконстантная ссылка на найденный элемент списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	reference operator[](const Char *name)
	{
		// (!) create if not exists
		return get(name, true);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Ссылка на себя
/**
		Метод возвращает ссылку на себя.

@return Self reference.
*/
	const ThisType& operator()() const
	{
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Ссылка на себя
/**
		Метод возвращает ссылку на себя.

@return Self reference.
*/
	ThisType& operator()()
	{
		return *this;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Manipulators
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать новый элемент в конце списка (копия)
/**
		Метод создаёт новый элемент копию @a prototype,
	который добавляется в конец списка.

@param[in] prototype Прототип создаваемого элемента
@return Неконстантная ссылка на созданный элемент
*/
	reference push_back(const_reference prototype)
	{
		return do_push(create(prototype));
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Создать новый элемент в конце списка (имя)
/**
		Метод создаёт новый элемент с именем @a name,
	который добавляется в конец списка.

@param[in] name Имя элемента списка
@return Неконстантная ссылка на созданный элемент
*/
	reference push_back(const String &name)
	{
		return do_push(create(name));
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Создать новый элемент в конце списка (имя)
/**
		Метод создаёт новый элемент с именем @a name,
	который добавляется в конец списка.

@param[in] name Имя элемента списка
@return Неконстантная ссылка на созданный элемент
*/
	reference push_back(const Char *name)
	{
		return do_push(create(name));
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Создать новый элемент в конце списка
/**
		Метод создаёт новый элемент с именем по умолчанию,
	который добавляется в конец списка.

@return Неконстантная ссылка на созданный элемент
*/
	reference push_back()
	{
		return do_push(create());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Удалить существующий элемент
/**
		Метод удаляет элемент из списка.

	Элемент должен принадлежать этому же списку!

@param[in] x Неконстантная ссылка на удаляемый элемент списка
*/
	void remove(reference x)
	{
		typename Container::iterator found = std::find(
			m_items.begin(), m_items.end(), &x);
		assert(found != m_items.end()
			&& "element not found");

		if (found != m_items.end())
			remove(iterator(found));
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Удалить существующий элемент
/**
		Метод удаляет элемент из списка.

	Элемент должен принадлежать этому же списку!

@param[in] pos Неконстантная ссылка на удаляемый элемент списка
*/
	void remove(iterator pos)
	{
		value_type *elem = *pos.base();

		m_items.erase(pos.base());
		m_owner->do_remove(elem);

		destroy(elem);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Удалить существующий элемент
/**
		Метод удаляет элемент из списка.

	Элемент должен принадлежать этому же списку!

@param[in] name элемент списка
*/
	void remove(const String &name)
	{
		// TODO: speed optimization
		iterator found = find(name, begin());
		while (found != end())
		{
			remove(found);
			found = find(name,
				begin());
		}
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Удалить существующий элемент
/**
		Метод удаляет элемент из списка.

	Элемент должен принадлежать этому же списку!

@param[in] name элемент списка
*/
	void remove(const Char *name)
	{
		// TODO: speed optimization
		iterator found = find(name, begin());
		while (found != end())
		{
			remove(found);
			found = find(name,
				begin());
		}
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Удалить все элементы списка
/**
		Метод удяляет все элементы списка.
*/
	void clear()
	{
		typename Container::reverse_iterator i = m_items.rbegin();
		typename Container::reverse_iterator ie = m_items.rend();
		for (; i != ie; ++i)
		{
			value_type *elem = (*i);
			m_owner->do_remove(elem);

			destroy(elem);
		}
		m_items.clear();
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Selectors
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Проверить наличие элемента с заданным именем
/**
		Метод проверяет, существует ли хотя бы один элемент с именем @a name.

@param[in] name Имя элемента списка
@return @b true если существует хотя бы один элемент с заданным именем,
	иначе @b false
*/
	bool exists(const String &name) const
	{
		return find(name, begin()) != end();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить наличие элемента с заданным именем
/**
		Метод проверяет, существует ли хотя бы один элемент с именем @a name.

@param[in] name Имя элемента списка
@return @b true если существует хотя бы один элемент с заданным именем,
	иначе @b false
*/
	bool exists(const Char *name) const
	{
		return find(name, begin()) != end();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Количество элементов в списке
/**
		Метод возвращает количество элементов в списке.

@return Количество элементов в списке
*/
	size_type size() const
	{
		return m_items.size();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить список на пустоту
/**
		Метод проверяет, является ли список элементов пустым.

@return @b true Если список пуст, иначе @b false
*/
	bool empty() const
	{
		return m_items.empty();
	}

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Равенство двух списков
/**
		Метод сравнивает два списка между собой. Два списка считаются равными,
	если оба имеют одинаковое количество элементов и если каждый элемент
	одного списка равен соответствующему элементу второго списка.

@param[in] x Второй список для сравнения
@return @b true Если списка равны, иначе @b false
*/
	bool equal(const ThisType &other) const
	{
		if (size() != other.size())
			return false;

		return details::equal(begin(),
			end(), other.begin());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Найти элемент с заданным именем
/**
		Метод ищет элемент списка с именем @a name начиная с позиции @a where.

@param[in] name Имя элемента списка
@param[in] pos Элемент с которого начинается поиск
@return Найденный элемент списка или end(), если элемент не найден
*/
	const const_iterator find(const String &name, const_iterator pos) const
	{
		const_iterator last = end();
		for (; pos != last; ++pos)
			if (pos->name() == name)
				break;

		return pos;
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Найти элемент с заданным именем
/**
		Метод ищет элемент списка с именем @a name начиная с позиции @a where.

@param[in] name Имя элемента списка
@param[in] pos Элемент с которого начинается поиск
@return Найденный элемент списка или end(), если элемент не найден
*/
	const const_iterator find(const Char *name, const_iterator pos) const
	{
		const_iterator last = end();
		for (; pos != last; ++pos)
			if (pos->name() == name)
				break;

		return pos;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Найти элемент с заданным именем
/**
		Метод ищет элемент списка с именем @a name начиная с позиции @a where.

@param[in] name Имя элемента списка
@param[in] pos Элемент с которого начинается поиск
@return Найденный элемент списка или end(), если элемент не найден
*/
	const iterator find(const String &name, iterator pos)
	{
		iterator last = end();
		for (; pos != last; ++pos)
			if (pos->name() == name)
				break;

		return pos;
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Найти элемент с заданным именем
/**
		Метод ищет элемент списка с именем @a name начиная с позиции @a where.

@param[in] name Имя элемента списка
@param[in] pos Элемент с которого начинается поиск
@return Найденный элемент списка или end(), если элемент не найден
*/
	const iterator find(const Char *name, iterator pos)
	{
		iterator last = end();
		for (; pos != last; ++pos)
			if (pos->name() == name)
				break;

		return pos;
	}


private:

//////////////////////////////////////////////////////////////////////////
/// @brief Create the new element.
/**
@param arg The constructor argument.
@return The created element.
*/
	template<typename T>
	value_type* create(const T &arg)
	{
		value_type *elem = m_alloc.allocate(1);

		try
		{
			new (elem) value_type(arg); // (!) constructor call
		}
		catch (...)
		{
			m_alloc.deallocate(elem, 1);
			throw;
		}

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Create the new element.
/**
		The element's default constructor is used.

@return The created element.
*/
	value_type* create()
	{
		value_type *elem = m_alloc.allocate(1);

		try
		{
			new (elem) value_type(); // (!) constructor call
		}
		catch (...)
		{
			m_alloc.deallocate(elem, 1);
			throw;
		}

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Destroy the element.
/**
		This method calls the destructor and releases the allocated memory.

@param elem The element.
*/
	void destroy(value_type *elem)
	{
		elem->~value_type(); // (!) destructor call
		m_alloc.deallocate(elem, 1);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Добавить элемент в список
/**
	Метод добавляет уже созданный элемент @a p в начало или конец списка.

@param[in] p Добавляемый элемент
*/
	reference do_push(value_type *p)
	{
		try
		{
			m_items.push_back(p);
			try
			{
				m_owner->do_insert(p);
			}
			catch (...)
			{
				m_items.pop_back();
				throw;
			}
		}
		catch (...)
		{
			destroy(p);
			throw;
		}

		return *p;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Установить владельца списком
/**
		Метод устанавливает нового владельца списка.

@param[in] owner Новый владелец списка
*/
	void set_owner(OwnerType owner)
	{
		assert(!m_owner || !owner
			&& "invalid operation");
		m_owner = owner;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Обменять два списка
/**
		Метод меняет содержимое двух списков местами.
	Владельцы списка не меняются.

@param[in,out] x Список для обмена
*/
	void swap(ThisType &x)
	{
		m_items.swap(x.m_items);
	}


private:
	typename Allocator::template
		rebind<value_type>::other m_alloc;  ///< @brief The allocator.

	OwnerType m_owner;  ///< @brief The list's owner.
	Container m_items;  ///< @brief The elements list.
};

	} // ElementListT<> template classes


	// SectionListT<> template classes
	namespace conf
	{

//////////////////////////////////////////////////////////////////////////
/// @brief List of configuration sections.
/**
		This is auxiliary class. You can't create instances of this class.
	Instead use SectionT::sections property.

		The class is a contains of configuration sections. It contains
	a several manipulators: push_back(), remove(), exists(). It also
	supports iterator methods: begin() and end().

		The class contains operator()(), which returns self reference.
	So, you can use one of the following:

@code
	void f(Section &s)
	{
		s.sections().push_back();
		// -or-
		s.sections.push_back();
	}
@endcode

		To access child section by name you can use one of the overloaded
	get() methods. To access child section value you can use one of the
	overloaded getv() method. Also you can use operator[]():

@code
	// child sections access
	void f(const Section &s)
	{
		s.sections.get("child_name");
		s.sections["child_name"];
		s.sections.getv("child_name"); // (!) value
	}
@endcode

		You can access section by name only if the name is unique. Otherwise
	you can use begin() and end() methods to iterate all child sections.

@see @ref omni_config
*/
template<typename Str>
class SectionListT: private omni::NonCopyable {
	typedef SectionListT<Str> ThisType;
	typedef SectionT<Str>* OwnerType;

	typedef typename String::allocator_type Allocator;
	typedef std::vector<SectionT<Str>*, typename Allocator::
		template rebind<SectionT<Str>*>::other > Container;

	typedef details::Iterator<typename Container::const_iterator,
		details::ConstTraits< SectionT<Str> > > implementation_defined_1;
	typedef details::Iterator<typename Container::iterator,
		details::NConstTraits< SectionT<Str> > > implementation_defined_2;

	friend class SectionT<Str>;

//////////////////////////////////////////////////////////////////////////
/// @name Main typedefs
/// @{
public:
	typedef SectionT<Str> value_type; ///< @brief Value type.

	typedef const value_type& const_reference; ///< @brief Constant reference.
	typedef       value_type&       reference; ///< @brief Non-constant reference.

	typedef typename value_type::String String;   ///< @brief String type.
	typedef typename value_type::Char Char;       ///< @brief Char type.

	typedef typename Container::size_type size_type; ///< @brief Size type.

/// @}
//////////////////////////////////////////////////////////////////////////

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
/**
		This constructor creates empty list.

	Only friends may create the section list.
*/
	SectionListT()
		: m_owner(0) // will be set later
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Destructor.
/**
		Only friends may destroy the list.
*/
	~SectionListT()
	{
		assert(empty() && "list not empty");
	}


//////////////////////////////////////////////////////////////////////////
/// @name Iterators
/// @{
public:

	typedef implementation_defined_1 const_iterator; ///< @brief Constant iterator.
	typedef implementation_defined_2 iterator; ///< @brief Non-constant iterator.

//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the list.
/**
@return The constant iterator.
*/
	const const_iterator begin() const
	{
		return const_iterator(m_items.begin());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the list.
/**
@return The non-constant iterator.
*/
	const iterator begin()
	{
		return iterator(m_items.begin());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief End of the list.
/**
@return The constant iterator.
*/
	const const_iterator end() const
	{
		return const_iterator(m_items.end());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief End of the list.
/**
@return The non-constant iterator.
*/
	const iterator end()
	{
		return iterator(m_items.end());
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Front & Back
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief First element.
/**
@return Constant reference.
*/
	const_reference front() const
	{
		return *m_items.front();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief First element.
/**
@return Non-constant reference.
*/
	reference front()
	{
		return *m_items.front();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Last element.
/**
@return Constant reference.
*/
	const_reference back() const
	{
		return *m_items.back();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Last element.
/**
@return Non-constant reference.
*/
	reference back()
	{
		return *m_items.back();
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Get section by name
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name.
/**
		This method returns a element with specified name @a name.
	If the element with that name is not exists or the name
	is not unique the exception will be thrown.

@param[in] name The element's name.
@return Constant reference.
@throw omni::conf::err::ElementNotFoundT If element not found.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	const_reference get(const String &name) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			throw err::ElementNotFoundT<String>(name, m_owner->fullName());

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name.
/**
		This method returns a element with specified name @a name.
	If the element with that name is not exists or the name
	is not unique the exception will be thrown.

@param[in] name The element's name.
@return Constant reference.
@throw omni::conf::err::ElementNotFoundT If element not found.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	const_reference get(const Char *name) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			throw err::ElementNotFoundT<String>(name, m_owner->fullName());

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name or default.
/**
		This method returns a element with specified name @a name.
	If the element with that name is not exists, then the @a def element
	will be returned. If the element's name is not unique the exception
	will be thrown.

@param[in] name The element's name.
@param[in] def The default element.
@return Constant reference.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	const_reference get(const String &name, const_reference def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name or default.
/**
		This method returns a element with specified name @a name.
	If the element with that name is not exists, then the @a def element
	will be returned. If the element's name is not unique the exception
	will be thrown.

@param[in] name The element's name.
@param[in] def The default element.
@return Constant reference.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	const_reference get(const Char *name, const_reference def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name or create.
/**
		Метод возвращает элемент с именем @a name. Если элемента с таким
	именем нет, но установлен флаг @a create, то будет создан новый элемент.
	Если же флаг @a create не установлен, будет сгенерировано исключение.
	Если есть два или более элементов с таким именем, также будет
	сгенерировано исключение.

@param[in] name The element's name.
@param[in] create Create if not exists flag.
@return Non-constant reference.
@throw omni::conf::err::ElementNotFoundT IF element not found and @a create flag not set.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	reference get(const String &name, bool create = false)
	{
		iterator found = find(name, begin());
		if (found == end())
		{
			if (create)
				return push_back(name);
			else
				throw err::ElementNotFoundT<String>(name, m_owner->fullName());
		}

		reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element by name or create.
/**
		Метод возвращает элемент с именем @a name. Если элемента с таким
	именем нет, но установлен флаг @a create, то будет создан новый элемент.
	Если же флаг @a create не установлен, будет сгенерировано исключение.
	Если есть два или более элементов с таким именем, также будет
	сгенерировано исключение.

@param[in] name The element's name.
@param[in] create Create if not exists flag.
@return Non-constant reference.
@throw omni::conf::err::ElementNotFoundT IF element not found and @a create flag not set.
@throw omni::conf::err::NameIsAmbiguousT If element's name is not unique.
*/
	reference get(const Char *name, bool create = false)
	{
		iterator found = find(name, begin());
		if (found == end())
		{
			if (create)
				return push_back(name);
			else
				throw err::ElementNotFoundT<String>(name, m_owner->fullName());
		}

		reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Get section's value by name
/// @{

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем
/**
		Метод возвращает значение элемента с именем @a name. Если элемента с
	таким именем нет или есть два или более, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@return Значение найденного элемента списка
@throw omni::config::NotFoundFailureT Если элемент с таким именем не найден
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const String& getv(const String &name) const
	{
		return get(name).val();
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем
/**
		Метод возвращает значение элемента с именем @a name. Если элемента с
	таким именем нет или есть два или более, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@return Значение найденного элемента списка
@throw omni::config::NotFoundFailureT Если элемент с таким именем не найден
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const String& getv(const Char *name) const
	{
		return get(name).val();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем (по умолчанию)
/**
		Метод возвращает значение элемента с именем @a name. Если элемента
	с таким именем нет, то будет возвращено значение @a def. Если с таким
	именем есть два или более элементов, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@param[in] def Возвращаемое значение по умолчанию
@return Значение найденного элемента списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const String& getv(const String &name, const String &def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem.val();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем (по умолчанию)
/**
		Метод возвращает значение элемента с именем @a name. Если элемента
	с таким именем нет, то будет возвращено значение @a def. Если с таким
	именем есть два или более элементов, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@param[in] def Возвращаемое значение по умолчанию
@return Значение найденного элемента списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const String& getv(const Char *name, const String &def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem.val();
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Значение элемента с заданным именем (по умолчанию)
/**
		Метод возвращает значение элемента с именем @a name. Если элемента
	с таким именем нет, то будет возвращено значение @a def. Если с таким
	именем есть два или более элементов, то будет сгенерировано исключение.

@param[in] name Имя элемента списка
@param[in] def Возвращаемое значение по умолчанию
@return Значение найденного элемента списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const Char* getv(const Char *name, const Char *def) const
	{
		const_iterator found = find(name, begin());
		if (found == end())
			return def;

		const_reference elem = *found;
		if (find(name, ++found) != end())
			throw err::NameIsAmbiguousT<String>(name, m_owner->fullName());

		return elem.val().c_str();
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Auxiliary operators
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Элемент с заданным именем
/**
		Метод является псевдонимом для get(name).

@param[in] name Имя элемента списка
@return Константная ссылка на найденный элемент списка
@throw omni::config::NotFoundFailureT Если элемент с таким именем не найден
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const_reference operator[](const String &name) const
	{
		return get(name);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Элемент с заданным именем
/**
		Метод является псевдонимом для get(name).

@param[in] name Имя элемента списка
@return Константная ссылка на найденный элемент списка
@throw omni::config::NotFoundFailureT Если элемент с таким именем не найден
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	const_reference operator[](const Char *name) const
	{
		return get(name);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Элемент с заданным именем (с созданием)
/**
		Метод является псевдонимом для get(name, true).
	Т.е. если элемент не существует, он будет создан!

@param[in] name Имя элемента списка
@return Неконстантная ссылка на найденный элемент списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	reference operator[](const String &name)
	{
		// (!) create if not exists
		return get(name, true);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Элемент с заданным именем (с созданием)
/**
		Метод является псевдонимом для get(name, true).
	Т.е. если элемент не существует, он будет создан!

@param[in] name Имя элемента списка
@return Неконстантная ссылка на найденный элемент списка
@throw omni::config::AmbiguousFailureT Если имя элемента не уникально
*/
	reference operator[](const Char *name)
	{
		// (!) create if not exists
		return get(name, true);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Ссылка на себя
/**
		Метод возвращает ссылку на себя.

@return Self reference.
*/
	const ThisType& operator()() const
	{
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Ссылка на себя
/**
		Метод возвращает ссылку на себя.

@return Self reference.
*/
	ThisType& operator()()
	{
		return *this;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Manipulators
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать новый элемент в конце списка (копия)
/**
		Метод создаёт новый элемент копию @a prototype,
	который добавляется в конец списка.

@param[in] prototype Прототип создаваемого элемента
@return Неконстантная ссылка на созданный элемент
*/
	reference push_back(const_reference prototype)
	{
		return do_push(create(prototype));
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Создать новый элемент в конце списка (имя)
/**
		Метод создаёт новый элемент с именем @a name,
	который добавляется в конец списка.

@param[in] name Имя элемента списка
@return Неконстантная ссылка на созданный элемент
*/
	reference push_back(const String &name)
	{
		return do_push(create(name));
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Создать новый элемент в конце списка (имя)
/**
		Метод создаёт новый элемент с именем @a name,
	который добавляется в конец списка.

@param[in] name Имя элемента списка
@return Неконстантная ссылка на созданный элемент
*/
	reference push_back(const Char *name)
	{
		return do_push(create(name));
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Создать новый элемент в конце списка
/**
		Метод создаёт новый элемент с именем по умолчанию,
	который добавляется в конец списка.

@return Неконстантная ссылка на созданный элемент
*/
	reference push_back()
	{
		return do_push(create());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Удалить существующий элемент
/**
		Метод удаляет элемент из списка.

	Элемент должен принадлежать этому же списку!

@param[in] x Неконстантная ссылка на удаляемый элемент списка
*/
	void remove(reference x)
	{
		typename Container::iterator found = std::find(
			m_items.begin(), m_items.end(), &x);
		assert(found != m_items.end()
			&& "element not found");

		if (found != m_items.end())
			remove(iterator(found));
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Удалить существующий элемент
/**
		Метод удаляет элемент из списка.

	Элемент должен принадлежать этому же списку!

@param[in] pos Неконстантная ссылка на удаляемый элемент списка
*/
	void remove(iterator pos)
	{
		value_type *elem = *pos.base();

		m_items.erase(pos.base());
		m_owner->do_remove(elem);

		destroy(elem);
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Удалить существующий элемент
/**
		Метод удаляет элемент из списка.

	Элемент должен принадлежать этому же списку!

@param[in] name элемент списка
*/
	void remove(const String &name)
	{
		// TODO: speed optimization
		iterator found = find(name, begin());
		while (found != end())
		{
			remove(found);
			found = find(name,
				begin());
		}
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Удалить существующий элемент
/**
		Метод удаляет элемент из списка.

	Элемент должен принадлежать этому же списку!

@param[in] name элемент списка
*/
	void remove(const Char *name)
	{
		// TODO: speed optimization
		iterator found = find(name, begin());
		while (found != end())
		{
			remove(found);
			found = find(name,
				begin());
		}
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Удалить все элементы списка
/**
		Метод удяляет все элементы списка.
*/
	void clear()
	{
		typename Container::reverse_iterator i = m_items.rbegin();
		typename Container::reverse_iterator ie = m_items.rend();
		for (; i != ie; ++i)
		{
			value_type *elem = (*i);
			m_owner->do_remove(elem);

			destroy(elem);
		}
		m_items.clear();
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Selectors
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Проверить наличие элемента с заданным именем
/**
		Метод проверяет, существует ли хотя бы один элемент с именем @a name.

@param[in] name Имя элемента списка
@return @b true если существует хотя бы один элемент с заданным именем,
	иначе @b false
*/
	bool exists(const String &name) const
	{
		return find(name, begin()) != end();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить наличие элемента с заданным именем
/**
		Метод проверяет, существует ли хотя бы один элемент с именем @a name.

@param[in] name Имя элемента списка
@return @b true если существует хотя бы один элемент с заданным именем,
	иначе @b false
*/
	bool exists(const Char *name) const
	{
		return find(name, begin()) != end();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Количество элементов в списке
/**
		Метод возвращает количество элементов в списке.

@return Количество элементов в списке
*/
	size_type size() const
	{
		return m_items.size();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить список на пустоту
/**
		Метод проверяет, является ли список элементов пустым.

@return @b true Если список пуст, иначе @b false
*/
	bool empty() const
	{
		return m_items.empty();
	}

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Равенство двух списков
/**
		Метод сравнивает два списка между собой. Два списка считаются равными,
	если оба имеют одинаковое количество элементов и если каждый элемент
	одного списка равен соответствующему элементу второго списка.

@param[in] x Второй список для сравнения
@return @b true Если списка равны, иначе @b false
*/
	bool equal(const ThisType &other) const
	{
		if (size() != other.size())
			return false;

		return details::equal(begin(),
			end(), other.begin());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Найти элемент с заданным именем
/**
		Метод ищет элемент списка с именем @a name начиная с позиции @a where.

@param[in] name Имя элемента списка
@param[in] where Элемент с которого начинается поиск
@return Найденный элемент списка или end(), если элемент не найден
*/
	const const_iterator find(const String &name, const_iterator pos) const
	{
		const_iterator last = end();
		for (; pos != last; ++pos)
			if (pos->name() == name)
				break;

		return pos;
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Найти элемент с заданным именем
/**
		Метод ищет элемент списка с именем @a name начиная с позиции @a where.

@param[in] name Имя элемента списка
@param[in] where Элемент с которого начинается поиск
@return Найденный элемент списка или end(), если элемент не найден
*/
	const const_iterator find(const Char *name, const_iterator pos) const
	{
		const_iterator last = end();
		for (; pos != last; ++pos)
			if (pos->name() == name)
				break;

		return pos;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Найти элемент с заданным именем
/**
		Метод ищет элемент списка с именем @a name начиная с позиции @a where.

@param[in] name Имя элемента списка
@param[in] where Элемент с которого начинается поиск
@return Найденный элемент списка или end(), если элемент не найден
*/
	const iterator find(const String &name, iterator pos)
	{
		iterator last = end();
		for (; pos != last; ++pos)
			if (pos->name() == name)
				break;

		return pos;
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Найти элемент с заданным именем
/**
		Метод ищет элемент списка с именем @a name начиная с позиции @a where.

@param[in] name Имя элемента списка
@param[in] where Элемент с которого начинается поиск
@return Найденный элемент списка или end(), если элемент не найден
*/
	const iterator find(const Char *name, iterator pos)
	{
		iterator last = end();
		for (; pos != last; ++pos)
			if (pos->name() == name)
				break;

		return pos;
	}


private:

//////////////////////////////////////////////////////////////////////////
/// @brief Create the new element.
/**
@param arg The constructor argument.
@return The created element.
*/
	template<typename T>
	value_type* create(const T &arg)
	{
		value_type *elem = m_alloc.allocate(1);

		try
		{
			new (elem) value_type(arg); // (!) constructor call
		}
		catch (...)
		{
			m_alloc.deallocate(elem, 1);
			throw;
		}

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Create the new element.
/**
		The element's default constructor is used.

@return The created element.
*/
	value_type* create()
	{
		value_type *elem = m_alloc.allocate(1);

		try
		{
			new (elem) value_type(); // (!) constructor call
		}
		catch (...)
		{
			m_alloc.deallocate(elem, 1);
			throw;
		}

		return elem;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Destroy the element.
/**
		This method calls the destructor and releases the allocated memory.

@param elem The element.
*/
	void destroy(value_type *elem)
	{
		elem->~value_type(); // (!) destructor call
		m_alloc.deallocate(elem, 1);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Добавить элемент в список
/**
	Метод добавляет уже созданный элемент @a p в начало или конец списка.

@param[in] p Добавляемый элемент
*/
	reference do_push(value_type *p)
	{
		try
		{
			m_items.push_back(p);
			try
			{
				m_owner->do_insert(p);
			}
			catch (...)
			{
				m_items.pop_back();
				throw;
			}
		}
		catch (...)
		{
			destroy(p);
			throw;
		}

		return *p;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Установить владельца списком
/**
		Метод устанавливает нового владельца списка.

@param[in] owner Новый владелец списка
*/
	void set_owner(OwnerType owner)
	{
		assert(!m_owner || !owner
			&& "invalid operation");
		m_owner = owner;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Обменять два списка
/**
		Метод меняет содержимое двух списков местами.
	Владельцы списка не меняются.

@param[in,out] x Список для обмена
*/
	void swap(ThisType &x)
	{
		m_items.swap(x.m_items);
	}


private:
	typename Allocator::template
		rebind<value_type>::other m_alloc;  ///< @brief The allocator.

	OwnerType m_owner;  ///< @brief Владелец списка
	Container m_items;  ///< @brief Список элементов
};

	} // SectionListT<> template classes


	// exception template classes...
	namespace conf
	{
		namespace err
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Базовое исключение
/**
		Базовое исключение конфигураций. Класс наследует стандартное исключение
	@a std::runtime_error и, соответственно, его метод @b what(). Однако
	это сообщение не является самым информативным. Дополнительная информация
	об исключении содержится в свойствах производных классов.

		Параметр шаблона @a Ch определяет тип символов строки FailureT::string_type.
	Допустимо использование @b wchar_t и @b char. Также в зависимости от
	макроса #OMNI_UNICODE определяется тип omni::config::Failure.
*/
template<typename Str>
class FailureT: public std::runtime_error {
	typedef std::runtime_error inherited;
protected:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об исключении
*/
	explicit FailureT(const std::string &msg)
		: inherited(msg)
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об исключении
*/
	explicit FailureT(const char *msg)
		: inherited(msg)
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Destructor.
	virtual ~FailureT() OMNI_THROW0()
	{}

protected:
	typedef Str String; ///< @brief Тип строки
};


//////////////////////////////////////////////////////////////////////////
/// @brief Ошибка доступа к элементам
/**
		Ошибка доступа к списку секций или параметров. Класс содержит имя
	секции или параметра elementName(), при обращении к которому произошло
	исключение. Также класс содержит полное имя родительской
	секции parentFullName().

		Параметр шаблона @a Ch определяет тип символов строки AccessFailureT::string_type.
	Допустимо использование @b wchar_t и @b char. Также в зависимости от
	макроса #OMNI_UNICODE определяется тип omni::config::AccessFailure.
*/
template<typename Str>
class AccessFailureT: public FailureT<Str> {
	typedef FailureT<Str> inherited;

public:
	typedef typename inherited::String String;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об исключении
@param[in] element_name Имя секции или параметра
@param[in] element_path Полное имя родительской секции
*/
	AccessFailureT(const std::string &msg,
		const String &element_name,
		const String &element_path)
			: inherited(msg),
			  m_name(element_name),
			  m_path(element_path)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об исключении
@param[in] element_name Имя секции или параметра
@param[in] element_path Полное имя родительской секции
*/
	AccessFailureT(const char *msg,
		const String &element_name,
		const String &element_path)
			: inherited(msg),
			  m_name(element_name),
			  m_path(element_path)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Virtual destructor.
	virtual ~AccessFailureT() OMNI_THROW0()
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Полное имя родительской секции
/**
		Метод возвращает полное имя секции, при доступе к элементам которой
	произошло исключение.

@brief Полное имя
*/
	const String& path() const
	{
		return m_path;
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Наименование секции или параметра
/**
		Метод возвращает имя секции или параметра, при обращении к которому
	произошло исключение.

@return Имя элемента
*/
	const String& name() const
	{
		return m_name;
	}

private:
	String m_path; ///< @brief Полное имя родительской секции
	String m_name; ///< @brief Наименование элемента
};


//////////////////////////////////////////////////////////////////////////
/// @brief Элемент не найден
/**
		Исключение генерируется если секция или параметр с заданным именем
	не найдены в родительской секции. Метод elementName() возвращает имя
	искомой секции или параметра. Метод parentFullName() возвращает полное
	имя родительской секции. Метод @b what() возвращает
	строку "element not found".

		Параметр шаблона @a Ch определяет тип символов строки NotFoundFailureT::string_type.
	Допустимо использование @b wchar_t и @b char. Также в зависимости от
	макроса #OMNI_UNICODE определяется тип omni::config::NotFoundFailure.
*/
template<typename Str>
class ElementNotFoundT: public AccessFailureT<Str> {
	typedef AccessFailureT<Str> inherited;

public:
	typedef typename inherited::String String;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] element_name Имя секции или параметра
@param[in] element_path Полное имя родительской секции
*/
	ElementNotFoundT(const String &element_name, const String &element_path)
		: inherited("element not found", element_name, element_path)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Virtual destructor.
	virtual ~ElementNotFoundT() OMNI_THROW0()
	{}
};



//////////////////////////////////////////////////////////////////////////
/// @brief Неуникальное имя элемента
/**
		Исключение генерируется если имя секции или параметра не уникальны
	в родительской секции. Метод elementName() возвращает имя искомой
	секции или параметра. Метод parentFullName() возвращает полное
	имя родительской секции. Метод @b what() возвращает
	строку "ambiguous element name".

		Параметр шаблона @a Ch определяет тип символов строки AmbiguousFailureT::string_type.
	Допустимо использование @b wchar_t и @b char. Также в зависимости от
	макроса #OMNI_UNICODE определяется тип omni::config::AmbiguousFailure.
*/
template<typename Str>
class NameIsAmbiguousT: public AccessFailureT<Str> {
	typedef AccessFailureT<Str> inherited;

public:
	typedef typename inherited::String String;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] element_name Имя секции или параметра
@param[in] element_path Полное имя родительской секции
*/
	NameIsAmbiguousT(const String &element_name, const String &element_path)
		: inherited("ambiguous element name", element_name, element_path)
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Virtual destructor.
	virtual ~NameIsAmbiguousT() OMNI_THROW0()
	{}
};


//////////////////////////////////////////////////////////////////////////
/// @brief Ошибка разбора конфигурации
/**
		Исключение генерируется, если встретилась ошибка при разборе
	конфигурации из потока ввода. Метод lineNumber() возвращает номер
	строки, где произошла ошибка.

		Параметр шаблона @a Ch определяет тип символов строки ParsingFailureT::string_type.
	Допустимо использование @b wchar_t и @b char. Также в зависимости от
	макроса #OMNI_UNICODE определяется тип omni::config::ParsingFailure.
*/
template<typename Str>
class ParsingFailureT: public FailureT<Str> {
	typedef FailureT<Str> inherited;

public:
	typedef typename inherited::String String;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
@param[in] line_number The line number.
*/
	ParsingFailureT(const std::string &msg, long line_number)
		: inherited(msg), m_line(line_number)
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
@param[in] line_number The line number.
*/
	ParsingFailureT(const char *msg, long line_number)
		: inherited(msg), m_line(line_number)
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Virtual destructor.
	virtual ~ParsingFailureT() OMNI_THROW0()
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Номер строки
/**
		Метод возвращает номер строки, при разборе которой произошло исключение.

@return Номер строки
*/
	long line() const
	{
		return m_line;
	}

private:
	long m_line; ///< @brief Номер строки
};


//////////////////////////////////////////////////////////////////////////
/// @brief Синтаксическая ошибка
/**
		Исключение генерируется, если встретилась синтаксическая ошибка при
	разборе конфигурации из потока ввода. Метод sectionName() возвращает
	полное имя текущей секции конфигурации, при разборе которой произошло
	исключение.

		Параметр шаблона @a Ch определяет тип символов строки SyntaxFailureT::string_type.
	Допустимо использование @b wchar_t и @b char. Также в зависимости от
	макроса #OMNI_UNICODE определяется тип omni::config::SyntaxFailure.
*/
template<typename Str>
class SyntaxErrorT: public ParsingFailureT<Str> {
	typedef ParsingFailureT<Str> inherited;

public:
	typedef typename inherited::String String;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
@param[in] section_path Полное имя текущей секции
@param[in] line Номер строки
*/
	SyntaxErrorT(const std::string &msg,
			const String &section_path, long line)
		: inherited(msg, line),
		  m_path(section_path)
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
@param[in] section_path Полное имя текущей секции
@param[in] line Номер строки
*/
	SyntaxErrorT(const char *msg,
			const String &section_path, long line)
		: inherited(msg, line),
		  m_path(section_path)
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Virtual destructor.
	virtual ~SyntaxErrorT() OMNI_THROW0()
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Полное имя текущей секции
/**
		Метод возвращает полное имя секции, при разборе
	которой произошло исключение.

@return Имя текущей секции
*/
	const String& path() const
	{
		return m_path;
	}

private:
	String m_path; /// < @brief Имя секции
};


//////////////////////////////////////////////////////////////////////////
/// @brief Несоответствие имени секции
/**
		Исключение генерируется, если закрываемое имя секции не соответствует
	ранее открытой. Например:

	@code
		<section1>
			# ...
		</section2>
	@endcode

		Метод expectedName() возвращает ожидаемое имя секции, а метод
	foundName() возвращает встретившееся имя секции.

		Параметр шаблона @a Ch определяет тип символов строки MismatchFailureT::string_type.
	Допустимо использование @b wchar_t и @b char. Также в зависимости от
	макроса #OMNI_UNICODE определяется тип omni::config::MismatchFailure.
*/
template<typename Str>
class NameMismatchT: public ParsingFailureT<Str> {
	typedef ParsingFailureT<Str> inherited;

public:
	typedef typename inherited::String String;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
	@param[in] expected_name Ожидаемое имя
	@param[in] found_name Найденное имя
	@param[in] line_number Номер строки
*/
	NameMismatchT(const String &expected_name,
			const String &found_name, long line_number)
		: inherited("name mismatch", line_number),
		  m_expected(expected_name),
		  m_found(found_name)
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Virtual destructor.
	virtual ~NameMismatchT() OMNI_THROW0()
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Ожидаемое имя
/**
		Метод возвращает ожидаемое имя секции.

@return Имя секции
*/
	const String& expected() const
	{
		return m_expected;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Найденное имя
/**
		Метод возвращает найденное имя секции.

@return Имя секции
*/
	const String& found() const
	{
		return m_found;
	}

private:
	String m_expected; ///< @brief Ожидаемое имя
	String m_found;    ///< @brief Найденное имя
};


//////////////////////////////////////////////////////////////////////////
/// @brief Ошибка сохранения конфигурации
/** @class WritingFailureT

		Исключение генерируется, если при сохранении конфигурации в поток
	вывода встретилось пустое имя секции или параметра. Метод fullName()
	возвращает полное имя секции или параметра.

		Параметр шаблона @a Ch определяет тип символов строки WritingFailureT::string_type.
	Допустимо использование @b wchar_t и @b char. Также в зависимости от
	макроса #OMNI_UNICODE определяется тип omni::config::WritingFailure.
*/
template<typename Str>
class WritingFailureT: public FailureT<Str> {
	typedef FailureT<Str> inherited;

public:
	typedef typename inherited::String String;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
@param[in] element_path Полное имя секции или параметра
*/
	WritingFailureT(const std::string &msg, const String &element_path)
		: inherited(msg), m_path(element_path)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
@param[in] element_path Полное имя секции или параметра
*/
	WritingFailureT(const char *msg, const String &element_path)
		: inherited(msg), m_path(element_path)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Virtual destructor.
	virtual ~WritingFailureT() OMNI_THROW0()
	{}


public:

//////////////////////////////////////////////////////////////////////////
/// @brief Полное имя секции или параметра
/**
		Метод возвращает полное имя секции или параметра, при записи которого
	произошло исключение.

@return Имя секции или параметра
*/
	const String& path() const
	{
		return m_path;
	}


private:
	String m_path; ///< @brief Полное имя секции или параметра
};



//////////////////////////////////////////////////////////////////////////
/// @brief Name is Empty
/**
		Исключение генерируется, если при сохранении конфигурации в поток
	вывода встретилось пустое имя секции или параметра. Метод fullName()
	возвращает полное имя секции или параметра.

		Параметр шаблона @a Ch определяет тип символов строки WritingFailureT::string_type.
	Допустимо использование @b wchar_t и @b char. Также в зависимости от
	макроса #OMNI_UNICODE определяется тип omni::config::WritingFailure.
*/
template<typename Str>
class NameIsEmptyT: public WritingFailureT<Str> {
	typedef WritingFailureT<Str> inherited;

public:
	typedef typename inherited::String String;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] element_path Полное имя секции или параметра
*/
	explicit NameIsEmptyT(const String &element_path)
		: inherited("name is empty", element_path)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Virtual destructor.
	virtual ~NameIsEmptyT() OMNI_THROW0()
	{}
};

		} // err namespace
	} // exception template classes


	// ParserT template class
	namespace conf
	{
		namespace io
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Парсер конфигурации.
/* @class ParserT

		Вспомогательный класс используется для разбора входного потока.
	Выполняет подсчёт строк, ассоциирование элементов и комментариев,
	и проверку корректности открываемых и закрываемых скобок.
*/
template<typename Str>
class ParserT {
public:
	typedef Str String;
	typedef typename String::traits_type Traits;
	typedef typename Traits::char_type Char;
	typedef typename String::allocator_type Allocator;
	typedef std::basic_istream<Char, Traits> IStream;
	typedef std::basic_ostringstream<Char, Traits, Allocator> OSStream;

	typedef SectionT<String> Section;
	typedef ElementT<String> Element;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Инициализировать парсер
/**
		Конструктор начинает подсчёт строк с единицы. Выполняет начальную
	инициализацию парсера. Делает текущей секцию @a root.

@param[in,out] root Текущая секция
*/
	explicit ParserT(Section &root)
		: m_last_element(0),
		  m_last_element_line(0),
		  m_last_comment_begin(0),
		  m_last_comment_end(0),
		  m_line_counter(1),
		  m_brace_depth(0)
	{
		push(root);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Разобрать секцию из потока ввода
/**
		Функция осуществляет разбор входного потока на секции и параметры
	конфигурации. Секция @a cfg должна быть пустой, иначе разбираемые секции
	и параметры будут добавляться к уже существующим.

@param[in] is Поток ввода
@throw omni::config::ParsingFailureT Неверный поток вывода
@throw omni::config::SyntaxFailureT Неверный синтаксис
@throw omni::config::MismatchFailureT Если имя закрываемой секции не соответствует открываемой
*/
	virtual void parse(IStream &is)
	{
		typedef details::CharConst<Char> ChConst;

		String token;

		while (is)
		{
			skip_ws(is);

			typename Traits::int_type meta = is.peek();
			if (Traits::eq_int_type(meta, Traits::eof()))
				break;

			typename Traits::char_type cx = Traits::to_char_type(meta);

			// comment (#)
			if (Traits::eq(cx, ChConst::COMMENT))
			{
				parse_comment(is.ignore());
				assign_suffix(); // suffix comment - the same line
			}

			// open bracket '<'
			else if (Traits::eq(cx, ChConst::BEGIN))
			{
				bool is_metadata = false;
				bool is_open = true;

				// open or close section? '/'
				cx = Traits::to_char_type(is.ignore().peek());
				if (Traits::eq(cx, ChConst::CLOSE))
				{
					is_open = false;
					is.ignore();
				}
				else if (Traits::eq(cx, ChConst::METADATA))
				{
					is_metadata = true;
					is.ignore();
				}

				if (!is_metadata)
				{
					get_token(is, token); // section name
					if (token.empty())
						throw err::SyntaxErrorT<String>("empty section's name",
							top().fullName(), lineNumber());
					brace_open();

					if (is_open) // create new section
					{
						Section &child = top().sections.push_back(token);
						push(child); // push to the section stack

						assign_prefix(child);
						parse_value(is, child);
					}
					else // close section
					{
						skip_ws(is);

						cx = Traits::to_char_type(is.peek());
						if (!Traits::eq(cx, ChConst::END)) // syntax error (>)
							throw err::SyntaxErrorT<String>("expected \">\" char",
								top().fullName(), lineNumber());

						is.ignore(); // ignore '>' char
						brace_close();

						if (token != top().name())
							throw err::NameMismatchT<String>(top().name(),
								token, lineNumber());

						set_last_element(&top());
						pop();
					}
				}
				else // parse meta data section <? ... ?>
					parse_metadata(is);
			}

			// close section (/>)
			else if (Traits::eq(cx, ChConst::CLOSE))
			{
				cx = Traits::to_char_type(is.ignore().peek());
				if (!Traits::eq(cx, ChConst::END)) // syntax error '>'
					throw err::SyntaxErrorT<String>("expected slash and closing bracket",
						top().fullName(), lineNumber());

				is.ignore(); // ignore '>' char
				brace_close();

				set_last_element(&top());
				pop();
			}

			// close section '>'
			else if (Traits::eq(cx, ChConst::END))
			{
				is.ignore(); // ignore '>' char
				brace_close();
			}

			// add new element
			else
			{
				get_token(is, token); // parse element name
				if (token.empty())
					throw err::SyntaxErrorT<String>("empty element's name",
						top().fullName(), lineNumber());

				Element &elem = top().elements.push_back(token);
				set_last_element(&elem);

				assign_prefix(elem);
				if (parse_value(is, elem))
					set_last_element(&elem);
			}
		}

		if (1 < m_stack.size())
			throw err::ParsingFailureT<String>("unexpected end of input stream", lineNumber());
	}

protected: /// @name Стэк секций

//////////////////////////////////////////////////////////////////////////
/// @brief Новая текущая секция
/**
		Метод запоминает предыдущую секцию и делает секцию @a cfg текущей.
	Вызывается при открытии секции.

@param[in] section Новая текущая секция
*/
	void push(Section &section)
	{
		m_stack.push_back(&section);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Текущая секция
/**
		Метод возвращает текущую секцию.

@return Текущая секция
*/
	Section& top() const
	{
		return *m_stack.back();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Старая текущая секция
/**
		Метод возвращается к предыдущей текущей секции.
	Вызывается при закрытии секции.

@throw omni::config::ParsingFailureT Если закрыты все секции.
*/
	void pop()
	{
		m_stack.pop_back();

		if (m_stack.empty()) // if all sections are closed
			throw err::ParsingFailureT<String>("root section is closed", lineNumber());
	}

protected: /// @name Скобки

//////////////////////////////////////////////////////////////////////////
/// @brief Открыть скобку
/**
		Метод увеличивает на единицу количество открытых скобок.
*/
	void brace_open()
	{
		m_brace_depth += 1;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Закрыть скобку
/**
		Метод уменьшает на единицу количество открытых скобок. Если закрытий
	больше чем открытий будет сгенерировано исключение.

@throw omni::config::SyntaxFailureT Если закрытие встретилось раньше открытия
*/
	void brace_close()
	{
		m_brace_depth -= 1;
		if (m_brace_depth < 0)
			throw err::SyntaxErrorT<String>("unexpected symbol (>)",
				top().fullName(), lineNumber());
	}

protected: /// @name Разбор потока ввода

//////////////////////////////////////////////////////////////////////////
/// @brief Прочитать токен из потока ввода
/**
		Метод читает из потока ввода @a is строку, пока не встретится
	один из заданных символов разделителей из @a delimiters.
	Считанная строка сохраняется в @a token.

@param[in,out] is Поток ввода
@param[out] token Буфер для прочитанного токена
*/
	static void get_pure_token(IStream &is, String &token)
	{
		std::ios_base::iostate state = std::ios_base::goodbit;
		const typename IStream::sentry ok(is, true);

		if (ok)
		{
			for (;;)
			{
				typename Traits::int_type meta = is.peek();
				if (Traits::eq_int_type(meta, Traits::eof()))
				{
					state |= std::ios_base::eofbit;
					break;
				}

				typename Traits::char_type cx = Traits::to_char_type(meta);
				if (details::CharConst<Char>::is_delim(cx))
					break;

				is.ignore();
				token += cx;
			}
		}

		is.setstate(state);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Прочитать строку в кавычках из потока ввода
/**
		Метод читает строку в кавычках. Обязательно первым символом должен
	быть символ кавычки! Если строка должна содержать кавычки, то они
	дублируются. Например, строка "Ах, эти "чёрные" глаза." должна выглядеть
	так: "Ах, эти ""чёрные"" глаза." При чтении двойные кавычки заменяются
	одинарными.

@param[in,out] is Поток ввода
@param[out] token Буфер для прочитанной строки
@param[in] quote Символ кавычки ('\"' или '\'')
*/
	static void get_quoted_token(IStream &is, String &token, const Char quote)
	{
		std::ios_base::iostate state = std::ios_base::goodbit;
		const typename IStream::sentry ok(is, true);

		if (ok)
		{
			// ignore first quote char (if present)
			if (Traits::eq_int_type(is.peek(),
				Traits::to_int_type(quote)))
					is.ignore();

			for (;;)
			{
				typename Traits::int_type meta = is.peek();
				if (Traits::eq_int_type(meta, Traits::eof()))
				{
					state |= std::ios_base::eofbit;
					break;
				}

				typename Traits::char_type cx = Traits::to_char_type(meta);
				if (Traits::eq(cx, quote))
				{
					is.ignore();

					// test for second quote
					meta = is.peek();
					if (Traits::eq_int_type(meta, Traits::eof()))
					{
						state |= std::ios_base::eofbit;
						break;
					}

					cx = Traits::to_char_type(meta);
					if (!Traits::eq(cx, quote))
						break;
				}

				is.ignore();
				token += cx;
			}
		}

		is.setstate(state);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Разобрать токен
/**
		Метод читает из потока ввода @a is имя секции или параметра или их
	значение. Имя может быть заключено в одинарные или двойные кавычки.

@param[in,out] is Поток ввода
@param[out] token The string token.
@return Имя или значение
*/
	void get_token(IStream &is, String &token)
	{
		typedef details::CharConst<Char> ChConst;

		skip_ws(is);
		token.resize(0); // (!) clear

		typename Traits::int_type meta = is.peek();
		if (!Traits::eq_int_type(meta, Traits::eof()))
		{
			typename Traits::char_type cx = Traits::to_char_type(meta);

			if (Traits::eq(cx, ChConst::DQUOTE)) // "token"
				get_quoted_token(is, token, ChConst::DQUOTE);
			else if (Traits::eq(cx, ChConst::SQUOTE)) // 'token'
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


//////////////////////////////////////////////////////////////////////////
/// @brief Разобрать метаданные
/**
		Метод разбирает служебную секцию. Пока секция "<? ... ?>" просто
	игнорируется. Вложенность не поддерживается.

@param[in,out] is Поток ввода
@throw omni::config::SyntaxFailureT Если не найден закрывающий тэг "?>"
*/
	void parse_metadata(IStream &is)
	{
		typedef details::CharConst<Char> ChConst;
		long metadata_started = m_line_counter;

		bool section_closed = false;
		String metadata;
		for (;;)
		{
			get_token(is, metadata);

			typename Traits::int_type meta = is.peek();
			if (Traits::eq_int_type(meta, Traits::eof()))
				break;

			typename Traits::char_type cx = Traits::to_char_type(meta);
			if (Traits::eq(cx, ChConst::METADATA))
			{
				is.ignore();

				meta = is.peek();
				if (Traits::eq_int_type(meta, Traits::eof()))
					break;

				cx = Traits::to_char_type(meta);
				if (Traits::eq(cx, ChConst::END))
				{
					section_closed = true;
					is.ignore(); // ignore '>' char
					break;
				}
			}
			else if (ChConst::is_delim(cx))
				is.ignore();
		}

		if (!section_closed) // meta data section must be closed
			throw err::SyntaxErrorT<String>("metadata section must be closed",
				top().fullName(), metadata_started);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Разобрать комментарий
/**
		Метод читает из потока ввода блок комментариев. Блок комментариев
	должен быть расположен на смежных строках.

@param[in,out] is Поток ввода
*/
	void parse_comment(IStream &is)
	{
		typedef details::CharConst<Char> ChConst;

		OSStream os;
		os.imbue(is.getloc());
		bool one_line = true;

		String line;

		m_last_comment_begin = m_line_counter;
		while (is && !is.eof())
		{
			std::getline(is, line, ChConst::ENDLINE);
			m_line_counter += 1;

			if (one_line)
				one_line = false;
			else
				os.put(ChConst::ENDLINE);
			os << line;

			m_last_comment_end = m_line_counter;
			if (skip_ws(is))
				break; // (!)

			typename Traits::int_type meta = is.peek();
			if (Traits::eq_int_type(meta, Traits::eof()))
				break;

			typename Traits::char_type cx = Traits::to_char_type(meta);
			if (Traits::eq(cx, ChConst::COMMENT))
				is.ignore(); // ignore (#)
			else
				break;
		}

		m_last_comment = os.str();
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Разобрать значение
/**
		Метод пытается прочитать из потока ввода @a is
	значение параметра @a element.

@param[in,out] is Поток ввода
@param[in,out] element Параметр
@return @b true Если присвоено значение, иначе @b false
*/
	bool parse_value(IStream &is, Element &element)
	{
		typedef details::CharConst<Char> ChConst;

		skip_ws(is);

		typename Traits::int_type meta = is.peek();
		if (!Traits::eq_int_type(meta, Traits::eof()))
		{
			typename Traits::char_type cx = Traits::to_char_type(meta);
			if (Traits::eq(cx, ChConst::EQUAL))
			{
				is.ignore(); // ignore (=)
				get_token(is, element.val());
				return true;
			}
		}

		return false;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Пропустить пробелы и служебные символы
/**
		Метод пропускает пробелы и служебные символы из потока @a is.
	Является аналогом @a std::ws(). Подсчитывает строки.

@param[in,out] is Поток ввода
@return Количество пропущенных строк
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
				for (typename Traits::int_type meta = is.rdbuf()->sgetc();
					; meta = is.rdbuf()->snextc())
				{
					if (Traits::eq_int_type(meta, Traits::eof()))
					{
						state |= std::ios::eofbit;
						break;
					}

					typename Traits::char_type cx = Traits::to_char_type(meta);
					if (Traits::eq(cx, ChConst::ENDLINE))
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


//////////////////////////////////////////////////////////////////////////
/// @brief Номер текущей строки
/**
		Метод возвращает номер текущей строки.

@return Номер строки
*/
	long lineNumber() const
	{
		return m_line_counter;
	}

protected: /// @name Комментарии

//////////////////////////////////////////////////////////////////////////
/// @brief Новый текущий элемент
/**
		Метод устанавливает новый текущий элемент. Используется для
	постфиксных комментариев.

@param[in] element Новый элемент
*/
	void set_last_element(Element *element)
	{
		m_last_element_line = m_line_counter;
		m_last_element = element;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Установить префиксный комментарий
/**
		Метод устанавливает префиксный комментарий для элемента @a cfg.

@param[in,out] element Элемент конфигурации
*/
	void assign_prefix(Element &element)
	{
		if (!m_last_comment.empty())
			if (m_last_comment_end == m_line_counter) // (!)
		{
			element.prefix() = m_last_comment;
			m_last_comment.resize(0);
		}
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Установить постфиксный комментарий
/**
		Метод устанавливает суффиксный комментарий для текущего элемента.
*/
	void assign_suffix()
	{
		if (m_last_element && !m_last_comment.empty())
			if (m_last_element_line == m_last_comment_begin) // (!)
		{
			m_last_element->suffix() = m_last_comment;
			m_last_comment.resize(0);
			m_last_element = 0;
		}
	}

private:
	std::vector<Section*> m_stack; ///< @brief Стэк секций

	Element *m_last_element;  ///< @brief Последний параметр или секция
	long m_last_element_line;      ///< @brief Номер строки последнего параметра или секции
	String m_last_comment;    ///< @brief Последний блок комментариев
	long m_last_comment_begin;     ///< @brief Номер первой строки блока комментариев
	long m_last_comment_end;       ///< @brief Номер последней строки блока комментариев

	long m_line_counter;           ///< @brief Счётчик строк
	long m_brace_depth;            ///< @brief Счётчик вложенности скобок
};

		} // io namespace


//////////////////////////////////////////////////////////////////////////
/// @brief Разобрать из потока ввода
/** @relates SectionT

		Оператор разбирает секцию @a cfg из потока ввода @a is.

@param[in] is Поток ввода
@param[out] cfg Секция
*/
template<typename Ch, typename Tr, typename Str> inline
	std::basic_istream<Ch, Tr>& operator>>(
		std::basic_istream<Ch, Tr> &is,
		SectionT<Str> &cfg)
{
	SectionT<Str> tmp;
	io::ParserT<Str> parser(tmp);
	parser.parse(is);

	tmp.swap(cfg);
	return is;
}

	} // ParserT template class


	// WriterT template class...
	namespace conf
	{
		namespace io
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Форматирование вывода секций и параметров
/** @class WriterT

		Базовый класс форматирования вывода секций и параметров. Определяет
	несколько абстрактных методов, используемых для вывода конфигураций:
	open_section(), close_section(), put_element(). Определяя эти методы
	можно реализовать свой стиль форматирования.

		Для сохранения секции в поток вывода используется метод print().
*/
template<typename Str>
class WriterT {
protected:
	typedef Str String;
	typedef typename String::traits_type Traits;
	typedef typename Traits::char_type Char;
	typedef typename String::allocator_type Allocator;
	typedef std::basic_ostream<Char, Traits> OStream;
	typedef std::basic_istringstream<Char, Traits, Allocator> ISStream;
	typedef std::basic_ostringstream<Char, Traits, Allocator> OSStream;

	typedef ElementT<String> Element;
	typedef SectionT<String> Section;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Параметры по умолчанию
/**
		Устанавливает параметры по умолчанию: 2 пробела на отступ.
	Двойные кавычки. Значения всегда в кавычках.
*/
	WriterT()
		: tabSize(2), indent(0),
		  rootName(false),
		  newLine(true)
{
	doubleQuote = true;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Вывести конфигурацию в поток вывода
/**
		Метод выполняет перечисление дочерних секций и параметров и вызывает
	для них методы open_section(), close_section() и put_element()
	соответственно. Порядок секций и параметров соответствует прочитанному
	из файла при разборе или задаваемому при создании секции. Т.е. в общем
	случае дочерние секции могут чередоваться с дочерними параметрами.

		Для дочерних секций метод вызывается рекурсивно.

		Может генерировать исключение, например, если, параметр не имеет имени.

@param[in,out] os Поток вывода
@param[in] section Секция конфигурации
@throw omni::config::WritingFailureT если какие-либо секция или параметр
	не соответствуют требованиям форматирования
*/
	void print(OStream &os, const Section &section) const
	{
		open_section(os, section);

		typedef typename Section::Container::const_iterator item_iterator;
		item_iterator i  = section.m_childs.begin();
		item_iterator ie = section.m_childs.end();
		for (; i != ie; ++i)
		{
			Element *elem = *i;

			if (elem->is_section())
			{
				Section *child = static_cast<Section*>(elem);
				print(os, *child);
			}
			else
				put_element(os, *elem);
		}

		close_section(os, section);
	}

protected:

//////////////////////////////////////////////////////////////////////////
/// @brief Начало секции
/**
		Метод выводит префиксный комментарий (если не пустой), имя секции
	и её значение (если не пустое).

	@code
	# prefix comment
	# prefix comment continue
	<section_name = section_value>
	@endcode

		Если секция является главной и флаг rootName не установлен,
	то имя секции не будет выводиться, как и комментарий.

		Также, если установлено выравнивание дочерних элементов, то
	определяется максимальная ширина поля.

@param[in,out] os Поток вывода
@param[in] section Секция конфигурации
@throw omni::config::WritingFailureT Если нет имени секции
*/
	virtual void open_section(OStream &os, const Section &section) const
	{
		typedef details::CharConst<Char> ChConst;
		const Char quote = doubleQuote ? ChConst::DQUOTE : ChConst::SQUOTE;

		if ((rootName || !is_root(section)) && section.name().empty())
			throw err::NameIsEmptyT<String>(section.fullName());

		const String indent_str(indent, ChConst::SPACE);

		if (rootName || !is_root(section))
		{
			if (newLine && !is_front(section))
				os.put(ChConst::ENDLINE);

			if (!section.prefix().empty()) // prefix comment
				put_comment_block(os, section.prefix(),
					indent_str, false);

			// open section
			(os << indent_str).put(ChConst::BEGIN);
			if (need_quote(section.name()))
				put_qstring(os, section.name(), quote);
			else
				os << section.name();
			if (!section.val().empty())
			{
				os.put(ChConst::SPACE).put(ChConst::EQUAL).put(ChConst::SPACE);
				put_qstring(os, section.val(), quote);
			}
			os.put(ChConst::END).put(ChConst::ENDLINE);
		}

		if (rootName || !is_root(section))
			indent += tabSize;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Окончание секции
/**
		Метод выводит завершение секции и постфиксный комментарий
	(если не пустой).

	@code
	</section_name> # suffix comment
	                # suffix comment continue
	@endcode

@param[in,out] os Поток вывода
@param[in] section Секция конфигурации
@throw omni::config::WritingFailureT Если нет имени секции
*/
	virtual void close_section(OStream &os, const Section &section) const
	{
		typedef details::CharConst<Char> ChConst;

		const Char quote = doubleQuote ? ChConst::DQUOTE : ChConst::SQUOTE;

		if ((rootName || !is_root(section)) && section.name().empty())
			throw err::NameIsEmptyT<String>(section.fullName());

		if (rootName || !is_root(section))
			indent -= tabSize;

		const String indent_str(indent, ChConst::SPACE);

		if (rootName || !is_root(section))
		{
			// close section
			OSStream o_line;
			o_line.imbue(os.getloc());

			(o_line << indent_str).put(ChConst::BEGIN).put(ChConst::CLOSE);
			if (need_quote(section.name()))
				put_qstring(o_line, section.name(), quote);
			else
				o_line << section.name();
			o_line.put(ChConst::END);

			const String line_str = o_line.str();
			os << line_str;

			if (!section.suffix().empty()) // suffix comment
			{
				const String indent_str(line_str.size()+1, ChConst::SPACE);
				put_comment_block(os.put(ChConst::SPACE),
					section.suffix(), indent_str, true);
			}
			else
				os.put(ChConst::ENDLINE);
		}
		else
			os.put(ChConst::ENDLINE);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Параметр
/**
		Метод выводит параметр @a element в поток вывода @a os. Выводится
	префиксный комментарий (если не пустой), имя параметра, знак равно и
	значение параметра (если не пустое), постфиксный комментарий
	(если не пустой).

	@code
		# prefix comment
		# prefix comment continue
		element_name = element_value # suffix comment
		                             # suffix comment continue
	@endcode

@param[in,out] os Поток вывода
@param[in] element Параметр конфигурации
@throw omni::config::WritingFailureT Если нет имени параметра
*/
	virtual void put_element(OStream &os, const Element &element) const
	{
		typedef details::CharConst<Char> ChConst;

		const Char quote = doubleQuote ? ChConst::DQUOTE : ChConst::SQUOTE;

		if (element.name().empty())
			throw err::NameIsEmptyT<String>(element.fullName());

		const String indent_str(indent, ChConst::SPACE);

		if (!element.prefix().empty()) // prefix comment
			put_comment_block(os.put(ChConst::ENDLINE),
				element.prefix(), indent_str, false);

		OSStream o_line;
		o_line.imbue(os.getloc());
		o_line << indent_str;

		// print "key"="val"
		if (need_quote(element.name()))
			put_qstring(o_line, element.name(), quote);
		else
			o_line << element.name();

		if (!element.val().empty())
		{
			o_line.put(ChConst::SPACE).put(ChConst::EQUAL).put(ChConst::SPACE);
			put_qstring(o_line, element.val(), quote);
		}

		const String line_str = o_line.str();
		os << line_str;

		if (!element.suffix().empty()) // suffix comment
		{
			const String indent_str(line_str.size()+1, ChConst::SPACE);
			put_comment_block(os.put(ChConst::SPACE),
				element.suffix(), indent_str, true);
		}
		else
			os.put(ChConst::ENDLINE);
	}

protected: // auxiliary functions

//////////////////////////////////////////////////////////////////////////
/// @brief Вывести блок комментариев
/**
		Метод перед каждой строкой добавляет отступ и символ '#'. Если
	установлен флаг @a skip_first_indent, то отступ перед первой строкой
	не добавляется. Это позволяет выводить суффиксные комментарии.

	@code
		....# prefix first line
		....# prefix second line
		....< XXX >
		....</ XXX > # suffix first line
		.............# suffix second line
	@endcode

		Блок комментариев @a comment может содержать несколько строк,
	разделённых символом перевода строки.

@param[in,out] os Поток вывода
@param[in] comment Блок комментариев
@param[in] indent Текущий отступ
@param[in] skip_first_indent Не выводит отступ для первой строки
*/
	static void put_comment_block(OStream &os, const String &comment,
		const String &indent, bool skip_first_indent)
	{
		typedef details::CharConst<Char> ChConst;

		ISStream is(comment);
		is.imbue(os.getloc());

		String line;

		do {
			std::getline(is, line, ChConst::ENDLINE);

			if (skip_first_indent)
				skip_first_indent = false;
			else
				os << indent;

			os.put(ChConst::COMMENT);
			os << line;
			os.put(ChConst::ENDLINE);
		} while (is && !is.eof());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Получить строку в кавычках
/**
		Метод позволяет преобразовать строку @a text в строку в кавычках.
	Если флаг @a alwaysQuoted не установлен, и исходная строка не содержит
	служебных символов, то кавычки не добавляются.

@param[in] text Исходная строка
@return @b true if text need quote, otherwise @b false.
*/
	static bool need_quote(const String &text)
	{
		typedef details::CharConst<Char> ChConst;

		for (size_t i = 0; ChConst::DELIMITERS[i]; ++i)
			if (text.find(ChConst::DELIMITERS[i]) != String::npos)
				return true;

		return false;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Записать строку в кавычках в поток вывода
/**
		Метод пишет строку @a str в кавычках в поток вывода @a os. Если
	внутри строки встречаются кавычки, то они дублируются.

@param[in,out] os Поток вывода
@param[in] str Строка для записи
@param[in] quote Символ кавычки ('\"' или '\'')
*/
	static void put_qstring(OStream &os, const String &str, Char quote)
	{
		os.put(quote);
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (Traits::eq(str[i], quote))
				os.put(quote);
			os.put(str[i]);
		}
		os.put(quote);
	}


protected:

//////////////////////////////////////////////////////////////////////////
/// @brief Главная секция?
/**
		Метод проверяет является ли секция @a section главной,
	т.е. у неё нет родительской секции.

@param[in] section Секция конфигурации
@return @b true если секция является главной
*/
	static bool is_root(const Section &section)
	{
		return (0 == section.parent());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Первая секция?
/**
		Метод проверяет является ли секция @a section
	первой в списке её родительской секции.

@param[in] section Секция конфигурации
@return @b true если секция является первой
*/
	static bool is_front(const Section &section)
	{
		const Section *parent = section.parent();

		if (parent && !parent->sections.empty()
			&& &parent->sections.front() == &section)
				return true;

		return false;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Последняя секция?
/**
		Метод проверяет является ли секция @a section
	последней в списке её родительской секции.

@param[in] section Секция конфигурации
@return @b true если секция является последней
*/
	static bool is_back(const Section &section)
	{
		const Section *parent = section.parent();

		if (parent && !parent->sections.empty()
			&& &parent->sections.back() == &section)
				return true;

		return false;
	}

public:
	mutable size_t indent;    ///< @brief Текущий размер отступа
	        size_t tabSize;   ///< @brief Количество пробелов в одном отступе

	bool rootName;
	bool newLine;
	bool doubleQuote;
};

		} // io namespace


//////////////////////////////////////////////////////////////////////////
/// @brief Write to output stream.
/** @relates SectionT

		Оператор выводит секцию @a cfg в поток вывода @a os.

	Используется простое форматирование с параметрами по умолчанию.

@param[in] os Output stream.
@param[in] section Configuration section.
@return Output stream.
*/
template<typename Ch, typename Tr, typename Str> inline
	std::basic_ostream<Ch, Tr>& operator<<(
		std::basic_ostream<Ch, Tr> &os,
		const SectionT<Str> &section)
{
	io::WriterT<Str> writer;
	writer.print(os, section);
	return os;
}

	} // WriterT template class


	// CharConst & Iterator...
	namespace conf
	{
		/// @cond details
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Char constants.
/**
		Шаблонный класс содержит набор строковых и символьных констант
	специализированных для @b wchar_t и @b char.

		Разделитель CharConst::SEPARATOR используется методом
	omni::config::ElementT::fullName() между именами родительских
	и дочерних элементов.

		Остальные константы используются при разборе конфигурации
	из потока ввода и при сохранении конфигурации в поток вывода.
*/
template<typename Ch>
class CharConst {
public:
	typedef Ch Char; ///< @brief Char type.

public: // string constants
	static const Char DELIMITERS[]; ///< @brief Delimiters string.
	static const Char SEPARATOR[];  ///< @brief Name separator.

	/// @brief Is char delimiter?
	static bool is_delim(Char cx);

public: // char constants
	static const Char ENDLINE;  ///< @brief New line char ('\n').
	static const Char SPACE;    ///< @brief Space char (' ').
	static const Char COMMENT;  ///< @brief Comment char ('#').
	static const Char METADATA; ///< @brief Metadata section ('?').
	static const Char EQUAL;    ///< @brief Equal char ('=').
	static const Char BEGIN;    ///< @brief Section begin ('<').
	static const Char CLOSE;    ///< @brief Close section ('/').
	static const Char END;      ///< @brief Section end ('>').
	static const Char SQUOTE;   ///< @brief Single quote ('\'').
	static const Char DQUOTE;   ///< @brief Double quote ('\"').
};


//////////////////////////////////////////////////////////////////////////
/// @brief Constants traits.
template<typename Val>
struct ConstTraits
{
	typedef Val value_type;        ///< @brief Value type.
	typedef const Val& reference;  ///< @brief Reference type.
	typedef const Val* pointer;    ///< @brief Pointer type.
};


//////////////////////////////////////////////////////////////////////////
/// @brief Non-constant traits.
template<typename Val>
struct NConstTraits
{
	typedef Val value_type;     ///< @brief Value type.
	typedef Val& reference;     ///< @brief Reference type.
	typedef Val* pointer;       ///< @brief Pointer type.
};


//////////////////////////////////////////////////////////////////////////
/// @brief The iterator template class.
/**
		This class is used as const and non-cont iterator class for a list
	of configuration elements and sections.
*/
template<typename Base, typename Tr>
class Iterator
{
	typedef Iterator<Base, Tr> ThisType;
	typedef Base BaseType;

public: // typedefs
	typedef typename std::iterator_traits<BaseType>::iterator_category iterator_category; ///< @brief The iterator category.
	typedef typename std::iterator_traits<BaseType>::difference_type difference_type; ///< @brief The difference type.

	typedef typename Tr::value_type value_type; ///< @brief The value type.
	typedef typename Tr::reference reference;   ///< @brief The reference type.
	typedef typename Tr::pointer pointer;       ///< @brief The pointer type.


//////////////////////////////////////////////////////////////////////////
/// @name Constructors
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
	Iterator()
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param base_it The base iterator.
*/
	explicit Iterator(const BaseType &base_it)
		: m_base(base_it)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The auxiliary copy constructor.
/**
@param other The other iterator.
*/
	template<typename Base2, typename Tr2>
	Iterator(const Iterator<Base2, Tr2> &other)
		: m_base(other.base())
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The auxiliary assignment operator.
/**
@param other The other iterator.
@return The self reference.
*/
	template<typename Base2, typename Tr2>
	ThisType& operator=(const Iterator<Base2, Tr2> &other)
	{
		m_base = other.base();
		return *this;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Access
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Access by index.
/**
@param i The index.
@return The element at specified index.
*/
	reference operator[](difference_type i) const
	{
		return *m_base[i];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Dereference.
/**
@return The object reference.
*/
	reference operator*() const
	{
		return *(*m_base);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Dereference.
/**
@return The object pointer.
*/
	pointer operator->() const
	{
		return *m_base;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the base iterator.
/**
@return The base iterator.
*/
	const BaseType& base() const
	{
		return m_base;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Increment and decrement
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Prefix increment.
/**
@return The self reference.
*/
	ThisType& operator++()
	{
		++m_base;
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Prefix decrement.
/**
@return The self reference.
*/
	ThisType& operator--()
	{
		--m_base;
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Postfix increment.
/**
@return The incremented iterator.
*/
	const ThisType operator++(int)
	{
		return ThisType(m_base++);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Postfix decrement.
/**
@return The decremented iterator.
*/
	const ThisType operator--(int)
	{
		return ThisType(m_base--);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Increment.
/**
@param d The increment distance.
@return The self reference.
*/
	ThisType& operator+=(difference_type d)
	{
		m_base += d;
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrement.
/**
@param d The decrement distance.
@return The self reference.
*/
	ThisType& operator-=(difference_type d)
	{
		m_base -= d;
		return *this;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

private:
	BaseType m_base; ///< @brief The base iterator.
};


//////////////////////////////////////////////////////////////////////////
/// @brief Are two iterators equal?
/** @relates Iterator
@param x The first iterator.
@param y The second iterator.
@return @b true If two iterators are equal, otherwise @b false.
*/
template<typename Base1, typename Tr1, typename Base2, typename Tr2> inline
bool operator==(const Iterator<Base1, Tr1> &x, const Iterator<Base2, Tr2> &y)
{
	return x.base() == y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Are two iterators non-equal?
/** @relates Iterator
@param x The first iterator.
@param y The second iterator.
@return @b true If two iterators are non-equal, otherwise @b false.
*/
template<typename Base1, typename Tr1, typename Base2, typename Tr2> inline
bool operator!=(const Iterator<Base1, Tr1> &x, const Iterator<Base2, Tr2> &y)
{
	return x.base() != y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator less than second?
/** @relates Iterator
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is less than second iterator, otherwise @b false.
*/
template<typename Base1, typename Tr1, typename Base2, typename Tr2> inline
bool operator<(const Iterator<Base1, Tr1> &x, const Iterator<Base2, Tr2> &y)
{
	return x.base() < y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator greater than second?
/** @relates Iterator
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is greater than second iterator, otherwise @b false.
*/
template<typename Base1, typename Tr1, typename Base2, typename Tr2> inline
bool operator>(const Iterator<Base1, Tr1> &x, const Iterator<Base2, Tr2> &y)
{
	return x.base() > y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator less than or equal to second?
/** @relates Iterator
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is less than or equal to second iterator, otherwise @b false.
*/
template<typename Base1, typename Tr1, typename Base2, typename Tr2> inline
bool operator<=(const Iterator<Base1, Tr1> &x, const Iterator<Base2, Tr2> &y)
{
	return x.base() <= y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator greater than or equal to second?
/** @relates Iterator
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is greater than or equal to second iterator, otherwise @b false.
*/
template<typename Base1, typename Tr1, typename Base2, typename Tr2> inline
bool operator>=(const Iterator<Base1, Tr1> &x, const Iterator<Base2, Tr2> &y)
{
	return x.base() >= y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increment.
/** @relates Iterator
@param x The iterator.
@param d The increment distance.
@return The incremented iterator.
*/
template<typename Base, typename Tr> inline
const Iterator<Base, Tr> operator+(const Iterator<Base, Tr> &x, typename Iterator<Base, Tr>::difference_type d)
{
	return Iterator<Base, Tr>(x.base() + d);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increment.
/** @relates Iterator
@param d The increment distance.
@param x The iterator.
@return The incremented iterator.
*/
template<typename Base, typename Tr> inline
const Iterator<Base, Tr> operator+(typename Iterator<Base, Tr>::difference_type d, const Iterator<Base, Tr> &x)
{
	return Iterator<Base, Tr>(d + x.base());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrement.
/** @relates Iterator
@param x The iterator.
@param d The decrement distance.
@return The decremented iterator.
*/
template<typename Base, typename Tr> inline
const Iterator<Base, Tr> operator-(const Iterator<Base, Tr> &x, typename Iterator<Base, Tr>::difference_type d)
{
	return Iterator<Base, Tr>(x.base() - d);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get distance between two iterators.
/** @relates Iterator
@param x The first iterator.
@param y The second iterator.
@return The distance between two iterators.
*/
template<typename Base, typename Tr> inline
typename Iterator<Base, Tr>::difference_type operator-(const Iterator<Base, Tr> &x, const Iterator<Base, Tr> &y)
{
	return x.base() - y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Are two sequences equal?
/**
@param first1 The begin of the first sequence.
@param last1 The end of the first sequence.
@param first2 The begin of the second sequence.
@return @b true if two sequences are equal, otherwise @b false.
*/
template<typename In> inline
	bool equal(In first1, In last1, In first2)
{
#if defined(_MSC_VER) && (1400 == _MSC_VER)
	// avoid VS8.0 checked iterators
	return stdext::unchecked_equal(first1, last1, first2);
#else
	return std::equal(first1, last1, first2);
#endif // (1400 == _MSC_VER)
}

		} // details namespace
		/// @endcond
	} // CharConst & Iterator

} // omni namespace

#endif // __OMNI_CONF_HPP_
