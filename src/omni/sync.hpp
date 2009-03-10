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
	@brief Synchronization tools.

		This header file contains declaration of the intra-process
	and inter-process synchronization tools.

@author Sergey Polichnoy
*/
#ifndef __OMNI_SYNC_HPP_
#define __OMNI_SYNC_HPP_

#include <omni/defs.hpp>

// use Win32 CRITICAL_SECTION
#if !defined(_WIN32_WINNT)
#	define _WIN32_WINNT 0x0500
#endif
#define WIN32_LEAN_AND_MEAN // avoid unuseful stuff
#include <windows.h>


namespace omni
{
	namespace sync
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The auto lock.
/**
		This class holds the synchronization object:
	constructor locks it and destructor unlocks.

		For example, the following code:

@code
	void f(Lockable &x)
	{
		x.enter();
		// ...
		x.leave();
	}
@endcode

		can be rewritten as:

@code
	void f(Lockable &x)
	{
		AutoLockT<Lockable> guard(x);
		// ...
	}
@endcode

		Even exception was thrown during function execution, the
	synchronization object will be properly unlocked.

		It is recommended to use appropriate typedef, for example:

@code
	typedef AutoLockT<CriticalSection> AutoLock;
@endcode

@tparam LOCK The type of syncronization object.
*/
template<typename LOCK>
class AutoLockT:
	private omni::NonCopyable
{
public:

	/// @brief Lock synchronization object.
	/**
			This constructor holds the synchronization object @a lock
		and locks it by calling LOCK::enter() method.

	@param[in] lock The synchronization object.
	*/
	explicit AutoLockT(LOCK &lock)
		: m_lock(lock)
	{
		m_lock.enter();
	}


	/// @brief Unlock synchronization object.
	/**
			The destructor unlocks the locked in constructor
		synchronization object by calling LOCK::leave() method.
	*/
	~AutoLockT()
	{
		m_lock.leave();
	}

private:
	LOCK &m_lock; ///< @brief The synchronization object.
};


//////////////////////////////////////////////////////////////////////////
/// @brief Critical Section synchronization object.
/**
		This class is used for intra-process synchronization.

	Based on the Win32 CRITICAL_SECTION object.
*/
class CriticalSection:
	private omni::NonCopyable
{
public:
	CriticalSection();
	explicit CriticalSection(long spinCount);
	~CriticalSection();

public:
	long setSpinCount(long spinCount);

public:
	bool try_enter();
	void enter();
	void leave();

private:
	CRITICAL_SECTION m_impl; ///< @brief The critical section.
};


/// @brief The critical section auto lock.
typedef AutoLockT<CriticalSection> AutoLock;


//////////////////////////////////////////////////////////////////////////
/// @brief The event.
/**
		If the event object is manual-reset, then you should call the reset()
	method to switch event to non-signaled state. Otherwise the event object
	is auto-reset, i.e. event switches to non-signaled state once a single
	waiting thread has been released.
*/
class Event:
	private NonCopyable
{
public:
	Event(bool manualReset, bool initialSignaled, wchar_t const* name);
	Event(bool manualReset, bool initialSignaled, char const* name);
	Event(bool manualReset, bool initialSignaled);
	~Event();

public:
	bool reset();
	bool set();

public:
	bool wait(DWORD timeout_ms);
	bool wait();

private:
	HANDLE m_impl; ///< @brief The event object.
};

	} // sync namespace
} // omni namespace

#endif // __OMNI_SYNC_HPP_
