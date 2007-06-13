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
	@brief Main OMNI definitions.

		This source file contains documentation only.

@author Sergey Polichnoy
*/
#include <omni/defs.hpp>


//////////////////////////////////////////////////////////////////////////
/// @brief Main OMNI namespace.
/**
		All classes and function of the OMNI library are in this namespace.

		You can use @b using directive:

@code
	using namespace omni;
@endcode
*/
namespace omni
{
} // omni namespace


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_DEBUG
//////////////////////////////////////////////////////////////////////////
/** @brief Enable/disable @a DEBUG mode.

		This macro is used to enable or disable @a DEBUG mode. If it defined
	to nonzero value @a DEBUG mode is enabled, otherwise @a DEBUG mode
	is disabled.

		If #OMNI_DEBUG macro is not quite defined, then it is
	defined automatically:
		- if @b _DEBUG system macro is defined, then #OMNI_DEBUG macro
				is defined to nonzero value,
		- otherwise #OMNI_DEBUG macro is defined to zero value.

		Note: #OMNI_DEBUG should be defined to integral value (1 or 0).
	So definition "/DOMNI_DEBUG" will not properly work (@a DEBUG mode
	will be disabled). Instead always use the following definitions:
	"/DOMNI_DEBUG=1" or "/DOMNI_DEBUG=0".

@see @ref omni_defs_debug
*/
#define OMNI_DEBUG
#endif


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_DEBUG_CODE
//////////////////////////////////////////////////////////////////////////
/** @brief Custom code in @a DEBUG mode.

		This macro is used to insert custom code in @a DEBUG mode only
	(i.e. if #OMNI_DEBUG is defined to nonzero value). The following
	two approach are equivalent, but second is shorten.

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
#endif


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_MT
//////////////////////////////////////////////////////////////////////////
/** @brief Enable/disable @a MULTI-THREAD mode.

This macro is used to enable or disable @a MULTI-THREAD mode. If it
defined to nonzero value @a MULTI-THREAD mode is enabled, otherwise
@a MULTI-THREAD mode is disabled.

If #OMNI_MT macro is not quite defined, then it is
defined automatically:
- if @b _MT system macro is defined, then #OMNI_MT macro
is defined to nonzero value,
- otherwise #OMNI_MT macro is defined to zero value.

Note: #OMNI_MT should be defined to integral value (1 or 0).
So definition "/DOMNI_MT" will not properly work (@a MULTI-THREAD mode
will be disabled). Instead always use the following definitions:
"/DOMNI_MT=1" or "/DOMNI_MT=0".

		For MS Visual C++ 2005 #OMNI_MT macro is
	automatically defined to nonzero value.

@see @ref omni_defs_multithread
*/
#define OMNI_MT
#endif


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_MT_CODE
//////////////////////////////////////////////////////////////////////////
/** @brief Custom code in @a MULTI-THREAD mode.

		This macro is used to insert custom code in @a MULTI-THREAD mode only
	(i.e. if #OMNI_MT is defined to nonzero value). The following
	two approach are equivalent, but second is shorten.

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
#endif


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_UNICODE
//////////////////////////////////////////////////////////////////////////
/** @brief Enable/disable @a UNICODE mode.

		This macro is used to enable or disable @a UNICODE mode. If it
	defined to nonzero value @a UNICODE mode is enabled, otherwise
	@a UNICODE mode is disabled.

		If #OMNI_UNICODE macro is not quite defined, then it is
	defined automatically:
		- if @b _UNICODE or @b UNICODE system macro are defined,
				then #OMNI_UNICODE macro is defined to nonzero value,
		- otherwise #OMNI_UNICODE macro is defined to zero value.

		Note: #OMNI_UNICODE should be defined to integral value (1 or 0).
	So definition "/DOMNI_UNICODE" will not properly work (@a UNICODE mode
	will be disabled). Instead always use the following definitions:
	"/DOMNI_UNICODE=1" or "/DOMNI_UNICODE=0".

@see @ref omni_defs_unicode
*/
#define OMNI_UNICODE
#endif


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_FIX_FOR_SCOPE
//////////////////////////////////////////////////////////////////////////
/** @brief Fix @b for operator's scope.

		If this macro is defined, then @b for operator's scope will
	be changed as follows:

@code
	#define for if (false) {} else for
@endcode

		For some compilers (MS Visual C++ 6.0) #OMNI_FIX_FOR_SCOPE
	macro is automatically defined.

@see @ref omni_defs_for_scope
*/
#define OMNI_FIX_FOR_SCOPE
#endif


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_USE_MKL
//////////////////////////////////////////////////////////////////////////
/// @brief Enable/disable using Intel MKL math library.
/**
		TODO: Intel MKL math library using detail description.

@see ref omni_defs_math
*/
#define OMNI_USE_MKL
#endif


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_USE_ATLAS
//////////////////////////////////////////////////////////////////////////
/// @brief Enable/disable using ATLAS math library.
/**
		TODO: ATLAS math library using detail description.

@see ref omni_defs_math
*/
#define OMNI_USE_ATLAS
#endif


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_SHOW_REMARK
//////////////////////////////////////////////////////////////////////////
/// @brief Show remark messages.
/**
		If this macro is defined, then during compiling OMNI library some
	remark messages will be shown. For example: @a DEBUG mode enable/disable
	status, @b MULTI-THREAD mode enable/disable status, etc.

@code
	>defs.cpp
	>OMNI remark: debug mode disabled
	>OMNI remark: multi-thread mode enabled
@endcode
*/
#define OMNI_SHOW_REMARK
#endif


// (!) for documentation only
#ifdef OMNI_DOCUMENTATION_MODE
#undef OMNI_EXCEPTION_SPEC
//////////////////////////////////////////////////////////////////////////
/// @brief Use strict exception specifiers.
/**
		TODO: detail exception specifiers description.
*/
#define OMNI_EXCEPTION_SPEC
#endif


//////////////////////////////////////////////////////////////////////////
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
	is defined automatically.


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

		TODO: detail math libraries using description.


@author Sergey Polichnoy
*/
