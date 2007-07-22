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

		This source file contains definition of
	the omni::sync::CriticalSection class.

@author Sergey Polichnoy
*/
#include <omni/sync.hpp>

#if defined(_WIN32) || defined(_WIN64)
#	define __OMNI_USE_PTHREAD 0
#else
#	define __OMNI_USE_PTHREAD 1
#endif

#if __OMNI_USE_PTHREAD            // use pthread_mutex
#	include <pthread.h>
#else                             // use CRITICAL_SECTION
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

namespace omni
{

//////////////////////////////////////////////////////////////////////////
/// @brief Synchronization tools.
/**
		This namespace contains a intra-process
	and inter-process synchronization tools.
*/
	// Lockable
	namespace sync
	{


//////////////////////////////////////////////////////////////////////////
/// @brief Lock synchronization object.
/**
		If synchronization object is not locked by another
	thread or process, then it will be locked.

		If synchronization object is already locked by another thread
	or process, then this method waits for this object and lock it.

		After synchronization object was locked
	it should be unlocked by leave() method.

@warning There is @b deadlock possible.
*/
void Lockable::enter()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Unlock synchronization object.
/**
		This method unlocks the synchronization
	object locked by enter() method.
*/
void Lockable::leave()
{}

	} // Lockable


	// Locker
	namespace sync
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Lock synchronization object.
/**
		This constructor hold the synchronization object @a lock.
	And lock it by Lockable::enter() method calling.

@param[in,out] lock Synchronization object.
@see Lockable::enter()
*/
Locker::Locker(Lockable &lock)
	: m_lock(lock)
{
	m_lock.enter();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Unlock synchronization object.
/**
		The destructor unlock the locked in
	constructor synchronization object.

@see Lockable::leave()
*/
Locker::~Locker()
{
	m_lock.leave();
}

	} // Locker


	// CriticalSection
	namespace sync
	{

#if __OMNI_USE_PTHREAD

//////////////////////////////////////////////////////////////////////////
/// @brief @b *nix Critical Section implementation.
/*
		For @b *nix platform the @b pthread_mutex structure is used.
*/
class CriticalSection::Impl {
public:

	/// @brief Constructor.
	Impl()
	{
		int ret = ::pthread_mutex_init(&lock, 0);
		assert(ret == 0 && "can't initialize the mutex");
	}

	/// @brief Destructor.
	~Impl()
	{
		int ret = ::pthread_mutex_destroy(&lock);
		assert(ret == 0 && "can't destroy the mutex");
	}

public:

	/// @brief Enter.
	void enter()
	{
		int ret = ::pthread_mutex_lock(&lock);
		assert(ret == 0 && "can't lock the mutex");
	}

	/// @brief Leave.
	void leave()
	{
		int ret = ::pthread_mutex_unlock(&lock);
		assert(ret == 0 && "can't unlock the mutex");
	}

private:
	pthread_mutex_t lock; ///< @brief @b *nix pthread_mutex object.
};

#else

//////////////////////////////////////////////////////////////////////////
/// @brief @b Win32 Critical Section implementation.
/*
		For @b Win32 platform the @b CRITICAL_SECTION structure is used.
*/
class CriticalSection::Impl {
public:

	/// @brief Constructor.
	Impl()
	{
		::InitializeCriticalSection(&lock);
	}

	/// @brief Destructor.
	~Impl()
	{
		::DeleteCriticalSection(&lock);
	}

public:

	/// @brief Enter.
	void enter()
	{
		::EnterCriticalSection(&lock);
	}

	/// @brief Leave.
	void leave()
	{
		::LeaveCriticalSection(&lock);
	}

private:
	CRITICAL_SECTION lock; ///< @brief @b Win32 Critical Section object.
};

#endif


//////////////////////////////////////////////////////////////////////////
/// @brief Critical Section construction.
/**
		The constructor initializes the synchronization object.
*/
CriticalSection::CriticalSection()
	: impl(new Impl())
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Critical Section destruction.
/**
		The destructor deletes the synchronization object.
*/
CriticalSection::~CriticalSection()
{
	delete impl;
}


//////////////////////////////////////////////////////////////////////////
// enter critical section
void CriticalSection::enter()
{
	impl->enter();
}


//////////////////////////////////////////////////////////////////////////
// leave critical section
void CriticalSection::leave()
{
	impl->leave();
}

	} // CriticalSection

} // omni namespace
