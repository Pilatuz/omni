///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
/** @file
@brief The main OMNI definitions.

		This header file has been included by all other OMNI files.
	So this header file is a root of the OMNI library.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_DEFS_HPP_
#define __OMNI_DEFS_HPP_

#if 1 // macro region

///////////////////////////////////////////////////////////////////////////////
// compiler specific switches
#if !defined(OMNI_DOXY_MODE)
#if defined(_MSC_VER) && (1400 <= _MSC_VER)      // Visual C++ 8.0+
#	define OMNI_EXCEPTION_SPEC
#	ifndef OMNI_MT
#		define OMNI_MT 1 // always multi-thread
#	endif // OMNI_MT

#elif defined(_MSC_VER) && (1300 <= _MSC_VER)    // Visual C++ 7.0/7.1

#elif defined(_MSC_VER) && (1200 <= _MSC_VER)    // Visual C++ 6.0
#	pragma warning(disable: 4786) // identifier was truncated to '255' characters in the debug information
#	define OMNI_USE_FACET(loc, fac) std::_USE(loc, fac) // (!)
#	define OMNI_FIX_FOR_SCOPE

#elif defined(__BORLANDC__) && (0x0560 <= __BORLANDC__) // Borland C++ Builder 6.0
#	define OMNI_EXCEPTION_SPEC

#elif defined(__GNUC__)                     // gcc
#	define OMNI_EXCEPTION_SPEC

#elif defined(__EDG__) && defined(__COMO__) // Comeau C++

#else
#	error Unknown compiler
#endif
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// platform detection
#if !defined(OMNI_DOXY_MODE)
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#	define OMNI_WIN
#elif defined(linux) || defined(__linux) || defined(__linux__)
#	define OMNI_LINUX
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#	define OMNI_OSX
#else
#	error Unknown platform
#endif
#endif // OMNI_DOXY_MODE



///////////////////////////////////////////////////////////////////////////////
// OMNI_DEBUG macro
#if defined(OMNI_DOXY_MODE)
/** @brief Enable/disable @a DEBUG mode.

		This macro is used to enable or disable @a DEBUG mode. If it's defined
	to nonzero value @a DEBUG mode is enabled, otherwise @a DEBUG mode
	is disabled.

		If #OMNI_DEBUG macro is not explicitly defined, then it will be
	defined automatically:
		- if @b _DEBUG system macro is defined, then #OMNI_DEBUG macro
				will be defined to nonzero value,
		- otherwise #OMNI_DEBUG macro will be defined to zero value.

		Note: #OMNI_DEBUG should be defined to integral value (1 or 0).
	So definition "-DOMNI_DEBUG" won't properly work (@a DEBUG mode
	will be disabled). Instead use the following definitions:
	"-DOMNI_DEBUG=1" or "-DOMNI_DEBUG=0".

@see @ref omni_defs_debug
*/
#define OMNI_DEBUG
#else
#if !defined(OMNI_DEBUG)
#	if defined(_DEBUG)
#		define OMNI_DEBUG 1
#	else
#		define OMNI_DEBUG 0
#	endif
#endif
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// OMNI_DEBUG_CODE macro
#if defined(OMNI_DOXY_MODE)
//////////////////////////////////////////////////////////////////////////
/** @brief Custom code in @a DEBUG mode.

		This macro is used to insert custom code in @a DEBUG mode only
	(i.e. if #OMNI_DEBUG is defined to nonzero value). The following
	two approaches are equivalent, but second is shorten.

@code
	void f1(int x)
	{
		#if OMNI_DEBUG
			if (x < 0)
				error();
		#endif

		// ...
	}
@endcode

@code
	void f2(int x)
	{
		OMNI_DEBUG_CODE(if (x < 0) error());

		// ...
	}
@endcode

		First approach (in function @a f1) is recommended for a debug code
	with complex multiline code. Second approach (in function @a f2) is
	recommended for a short (single line) debug code.

@param code Custom debug code.
@see @ref omni_defs_debug
*/
#define OMNI_DEBUG_CODE(code)
#else
#if OMNI_DEBUG
#	define OMNI_DEBUG_CODE(code) code
#else
#	define OMNI_DEBUG_CODE(code)
#endif
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// OMNI_MT macro
#if defined(OMNI_DOXY_MODE)
/** @brief Enable/disable @a MULTI-THREAD mode.

		This macro is used to enable or disable @a MULTI-THREAD mode. If it's
	defined to nonzero value @a MULTI-THREAD mode is enabled, otherwise
	@a MULTI-THREAD mode is disabled.

		If #OMNI_MT macro is not explicitly defined, then it will be
	defined automatically:
		- if @b _MT system macro is defined, then #OMNI_MT macro
			will be defined to nonzero value,
		- otherwise #OMNI_MT macro will be defined to zero value.

		Note: #OMNI_MT should be defined to integral value (1 or 0).
	So definition "-DOMNI_MT" will not properly work (@a MULTI-THREAD mode
	will be disabled). Instead use the following definitions:
	"-DOMNI_MT=1" or "-DOMNI_MT=0".

		For MS Visual C++ 2005 and later #OMNI_MT macro will be
	automatically defined to nonzero value.

@see @ref omni_defs_multithread
*/
#define OMNI_MT
#else
#if !defined(OMNI_MT)
#	if defined(_MT)
#		define OMNI_MT 1
#	else
#		define OMNI_MT 0
#	endif
#endif
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// OMNI_MT_CODE macro
#if defined(OMNI_DOXY_MODE)
/** @brief Custom code in @a MULTI-THREAD mode.

		This macro is used to insert custom code in @a MULTI-THREAD mode only
	(i.e. if #OMNI_MT is defined to nonzero value). The following
	two approaches are equivalent, but second is shorten.

@code
	void f1()
	{
		#if OMNI_MT
			_lock();
		#endif

		// ...

		#if OMNI_MT
			_unlock();
		#endif
	}
@endcode

@code
	void f2()
	{
		OMNI_MT_CODE(_lock());

		// ...

		OMNI_MT_CODE(_unlock());
	}
@endcode

		First approach (in function @a f1) is recommended for a multithread
	code with complex multiline code. Second approach (in function @a f2)
	is recommended for a short (single line) multithread code.

@param code Custom multithread code.
@see @ref omni_defs_multithread
*/
#define OMNI_MT_CODE(code)
#else
#if OMNI_MT
#	define OMNI_MT_CODE(code) code
#else
#	define OMNI_MT_CODE(code)
#endif
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// OMNI_UNICODE macro
#if defined(OMNI_DOXY_MODE)
/** @brief Enable/disable @a UNICODE mode.

		This macro is used to enable or disable @a UNICODE mode. If it's
	defined to nonzero value @a UNICODE mode is enabled, otherwise
	@a UNICODE mode is disabled.

		If #OMNI_UNICODE macro is not explicitly defined, then it will be
	defined automatically:
		- if @b _UNICODE or @b UNICODE system macro are defined,
				then #OMNI_UNICODE macro will be defined to nonzero value,
		- otherwise #OMNI_UNICODE macro will be defined to zero value.

		Note: #OMNI_UNICODE should be defined to integral value (1 or 0).
	So definition "-DOMNI_UNICODE" will not properly work (@a UNICODE mode
	will be disabled). Instead always use the following definitions:
	"-DOMNI_UNICODE=1" or "-DOMNI_UNICODE=0".

@see @ref omni_defs_unicode
*/
#define OMNI_UNICODE
#else
#if !defined(OMNI_UNICODE)
#	if defined(UNICODE) || defined(_UNICODE)
#		define OMNI_UNICODE 1
#	else
#		define OMNI_UNICODE 0
#	endif
#endif
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// for scope correction
#if defined(OMNI_DOXY_MODE)
/** @brief Fix @b for operator's scope.

		If this macro is defined, then @b for operator's scope will
	be changed as follows:

@code
	#define for if (false) {} else for
@endcode

		For some compilers (MS Visual C++ 6.0) #OMNI_FIX_FOR_SCOPE
	macro will be automatically defined.

@see @ref omni_defs_for_scope
*/
#define OMNI_FIX_FOR_SCOPE
#else
#if defined(OMNI_FIX_FOR_SCOPE)
#	define for if (false) {} else for
#endif
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// exception specification
#if defined(OMNI_DOXY_MODE)
/// @brief Use strict exception specifiers.
/**
		TODO: detail exception specifiers description.
*/
#define OMNI_EXCEPTION_SPEC
// TODO: OMNI_THROW0 macro documentation
#else
#if defined(OMNI_EXCEPTION_SPEC)
#	define OMNI_THROW0() throw()
#else
#	define OMNI_THROW0()
#endif
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// use facet macro
#if defined(OMNI_DOXY_MODE)
// TODO: OMNI_USE_FACET macro documentaion
#else
#if !defined(OMNI_USE_FACET)
#	define OMNI_USE_FACET(loc, fac) std::use_facet<fac>(loc)
#endif
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// MKL usage
#if defined(OMNI_DOXY_MODE)
/// @brief Enable/disable Intel MKL math library usage.
/**
		TODO: detail description of Intel MKL math library usage.

@see ref omni_defs_math
*/
#define OMNI_USE_MKL
#endif // OMNI_DOXY_MODE


///////////////////////////////////////////////////////////////////////////////
// ATLAS usage
#if defined(OMNI_DOXY_MODE)
/// @brief Enable/disable ATLAS math library usage.
/**
		TODO: detail description of ATLAS math library usage.

@see ref omni_defs_math
*/
#define OMNI_USE_ATLAS
#endif // OMNI_DOXY_MODE

#endif // macro region


///////////////////////////////////////////////////////////////////////////////
/// @brief The main namespace.
/**
		All classes and function of the OMNI library are in this namespace.

		You can use @b using directive:

@code
	using namespace omni;
@endcode
*/
namespace omni
{

///////////////////////////////////////////////////////////////////////////////
/// @brief Non-Copyable interface.
/**
		Some classes should not support copying due to design purposes.
	A common practice is to declare copy constructor and assignment operator
	in the @b private section and do not define them.

@code
	class X
	{
	public:
		// ...

	private: // copying not allowed
		X& operator=(X const&); // (!) not defined
		X(X const&);            // (!) not defined
	};
@endcode

		This practice is good, but too annoying. You should add these
	@b private declarations for each appropriated class. Moreover,
	it is not so clear for users. A better practice is to derive your
	class from the NonCopyable interface.

@code
	// default copying not allowed
	class Y:
		private omni::NonCopyable
	{
	public:
		// ...
	};
@endcode

		Class NonCopyable is used to protect derived classes from default
	copying (copy constructor and assignment operator which compiler
	generates by default). It is recommended to use @b private derivation.

		Note that NonCopyable is not strict copying protection.
	Derived classes may define their own copy constructor
	and/or assignment operator.
*/
class NonCopyable
{
protected:

	/// @brief Trivial constructor.
	NonCopyable()
	{}

	/// @brief Trivial destructor.
	~NonCopyable()
	{}

private: // copying not allowed
	NonCopyable& operator=(NonCopyable const&);
	NonCopyable(NonCopyable const&);
};

} // omni namespace


///////////////////////////////////////////////////////////////////////////////
/** @page omni_defs Main OMNI definitions

@section omni_defs_for_scope for operator's scope

		Some obsolete compilers uses incorrect scope of variables declared
	in the @b for loop. For example:

@code
	for (int i = 0; i < 10; ++i)
		{ ... }
	for (int i = 0; i < 15; ++i) // error, "i" already defined
		{ ... }
@endcode

		To fix this bug the following macro is used:

@code
	#define for if (false) {} else for
@endcode

		So the following code will be correct:

@code
	for (int i = 0; i < 10; ++i)
		{ ... }
	for (int i = 0; i < 15; ++i) // OK
		{ ... }
@endcode

		To fix @b for operator's scope you should define #OMNI_FIX_FOR_SCOPE
	macro. For some compilers (for example MS VC++ 6.0) this macro
	has been defined automatically.


@section omni_defs_debug DEBUG mode

		OMNI library has a @a DEBUG mode. In @a DEBUG mode library performs
	a lot of checking: indexes checking, iterators checking, etc. To enable
	@a DEBUG mode you should define #OMNI_DEBUG macro to nonzero value.
	Also this macro may be defined automatically.

		#OMNI_DEBUG_CODE macro includes debug code in to the source file
	only if @a DEBUG mode is enabled. If @a DEBUG mode disabled, then
	any debug code is ignored.

@code
	void f(int x)
	{
		OMNI_DEBUG_CODE(if (x<0) throw "value should be positive");

		// ...
	}
@endcode

		Note, any checking in @a DEBUG mode are important for the
	OMNI library's users (i.e. programmers). For application's user
	some checking should be presented although @a DEBUG mode.


@section omni_defs_multithread MULTI-THREAD mode

		OMNI library can be used in multithread environment. In this case
	access to any global or static resources (for example memory manager)
	should be synchronized between different threads.

		To enable @a MULTI-THREAD mode you should define #OMNI_MT macro
	to nonzero value. Also this macro may be defined automatically.

		#OMNI_MT_CODE macro includes code in to the source file
	only if @a MULTI-THREAD mode is enabled. If @a MULTI-THREAD
	mode disabled, then any code is ignored.

@code
#if OMNI_MT
	extern Lock __g_lock;
#endif // OMNI_MT

	void f()
	{
		OMNI_MT_CODE(__g_lock.lock());

		// ...

		OMNI_MT_CODE(__g_lock.unlock());
	}
@endcode


@section omni_defs_unicode UNICODE mode

		OMNI library can be used be in @a UNICODE mode. If @a UNICODE mode
	is enabled, then instead @b char the @b wchar_t type is used (instead
	std::string the std::wstring is used corresponding).

		To enable @a UNICODE mode you should define #OMNI_UNICODE macro
	to nonzero value. Also this macro may be defined automatically.

		Note: although @a UNICODE mode OMNI library implements both versions
	(@b char and @b wchar_t). @a UNICODE macro is used to select default
	behavior only (in common case through @b typedef).


@section omni_defs_math Math libraries

		OMNI library can use some math libraries:
			- Intel MKL
			- ATLAS

		TODO: detail math libraries usage description.


@author Sergey Polichnoy <pilatuz@gmail.com>
*/

#endif // __OMNI_DEFS_HPP_
