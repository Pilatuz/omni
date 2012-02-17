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
//		https://bitbucket.org/pilatuz/omni
//////////////////////////////////////////////////////////////////////////
/** @file
	@brief Main OMNI definitions.

		This header file is included by all other OMNI files.
	So this header file is a root of OMNI library.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_DEFS_HPP_
#define __OMNI_DEFS_HPP_

namespace omni
{

//////////////////////////////////////////////////////////////////////////
/// @brief Non-Copyable interface.
/**
		Some classes should not support copying due to design purposes.
	A common practice is to declare copy constructor and assignment operator
	in the @b private section and not to define them.

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

		This practice is good, but too verbose. You should add these
	@b private declarations for each appropriated classes. Moreover,
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


#if !defined(OMNI_DOCUMENTATION_MODE)

//////////////////////////////////////////////////////////////////////////
// compiler specific
#if defined(_MSC_VER) && (1400 <= _MSC_VER)      // Visual C++ 8.0
#	define OMNI_EXCEPTION_SPEC
#	ifndef OMNI_MT
#		define OMNI_MT 1 // always multi-thread
#	endif // OMNI_MT

#elif defined(_MSC_VER) && (1300 <= _MSC_VER)    // Visual C++ 7.0/7.1
// ...

#elif defined(_MSC_VER) && (1200 <= _MSC_VER)    // Visual C++ 6.0
#	pragma warning(disable: 4786) // identifier was truncated to '255' characters in the debug information
#	define OMNI_USE_FACET(loc, fac) std::_USE(loc, fac) // (!)
#	define OMNI_FIX_FOR_SCOPE

#elif defined(__BORLANDC__) && (0x0560 <= __BORLANDC__) // Borland C++ Builder 6.0
#	define OMNI_EXCEPTION_SPEC

#elif defined(__GNUC__)                     // gcc
#	define OMNI_EXCEPTION_SPEC

#elif defined(__EDG__) && defined(__COMO__) // Comeau C++
// ...

#else
#	pragma message("OMNI warning: unknown compiler")
#endif


//////////////////////////////////////////////////////////////////////////
// OMNI_DEBUG macro
#if !defined(OMNI_DEBUG)
#	if defined(_DEBUG)
#		define OMNI_DEBUG 1
#	else
#		define OMNI_DEBUG 0
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// OMNI_DEBUG_CODE macro
#if OMNI_DEBUG
#	define OMNI_DEBUG_CODE(code) code
#	if defined(OMNI_SHOW_REMARK)
#		pragma message("OMNI remark: debug mode enabled")
#	endif
#else
#	define OMNI_DEBUG_CODE(code)
#	if defined(OMNI_SHOW_REMARK)
#		pragma message("OMNI remark: debug mode disabled")
#	endif
#endif


//////////////////////////////////////////////////////////////////////////
// OMNI_MT macro
#if !defined(OMNI_MT)
#	if defined(_MT)
#		define OMNI_MT 1
#	else
#		define OMNI_MT 0
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
// OMNI_MT_CODE macro
#if OMNI_MT
#	define OMNI_MT_CODE(code) code
#	if defined(OMNI_SHOW_REMARK)
#		pragma message("OMNI remark: multi-thread mode enabled")
#	endif
#else
#	define OMNI_MT_CODE(code)
#	if defined(OMNI_SHOW_REMARK)
#		pragma message("OMNI remark: multi-thread mode disabled")
#	endif
#endif


//////////////////////////////////////////////////////////////////////////
// OMNI_UNICODE macro
#if !defined(OMNI_UNICODE)
#	if defined(UNICODE) || defined(_UNICODE)
#		define OMNI_UNICODE 1
#	else
#		define OMNI_UNICODE 0
#	endif
#endif


//////////////////////////////////////////////////////////////////////////
// for scope correction
#if defined(OMNI_FIX_FOR_SCOPE)
#	define for if (false) {} else for
#	if defined(OMNI_SHOW_REMARK)
#		pragma message("OMNI remark: for scope fixed")
#	endif
#endif


//////////////////////////////////////////////////////////////////////////
// exception specification
#if defined(OMNI_EXCEPTION_SPEC)
#	define OMNI_THROW0() throw()
#else
#	define OMNI_THROW0()
#endif


//////////////////////////////////////////////////////////////////////////
// use facet macro
#if !defined(OMNI_USE_FACET)
#	define OMNI_USE_FACET(loc, fac) std::use_facet<fac>(loc)
#endif

#endif // !OMNI_DOCUMENTATION_MODE

#endif // __OMNI_DEFS_HPP_
