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

@author Sergey Polichnoy
*/
#include <omni/sync.hpp>

#include <stdexcept>
#include <assert.h>

namespace omni
{

//////////////////////////////////////////////////////////////////////////
/// @brief Synchronization tools.
/**
		This namespace contains a intra-process
	and inter-process synchronization tools.
*/
	namespace sync
	{
	} // sync namespace


	// CriticalSection
	namespace sync
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		Initializes the synchronization object.
*/
CriticalSection::CriticalSection()
{
	::InitializeCriticalSection(&m_impl);
}


#if 0x0403 <= _WIN32_WINNT
//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
		Initializes the synchronization object and spin count.

@param[in] spinCount The spin count.
*/
CriticalSection::CriticalSection(long spinCount)
{
	::InitializeCriticalSectionAndSpinCount(&m_impl, spinCount);
}
#endif


//////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
/**
		Releases the synchronization object.
*/
CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection(&m_impl);
}


#if 0x0403 <= _WIN32_WINNT
//////////////////////////////////////////////////////////////////////////
/// @brief Change the spin count.
/**
		This method changes the spin count.

@param[in] spinCount The new spin count.
@return The previous spin count.
*/
long CriticalSection::setSpinCount(long spinCount)
{
	return ::SetCriticalSectionSpinCount(&m_impl, spinCount);
}
#endif


#if 0x0400 <= _WIN32_WINNT
//////////////////////////////////////////////////////////////////////////
/// @brief Try to lock synchronization object.
/**
@return @b true If sychronization object is locked, otherwise @b false.
*/
bool CriticalSection::try_enter()
{
	return ::TryEnterCriticalSection(&m_impl) != 0;
}
#endif


//////////////////////////////////////////////////////////////////////////
/// @brief Lock synchronization object.
/**
		If synchronization object is not locked by another
	thread, then it will be locked.

		If synchronization object is already locked by another thread
	or process, then this method waits while this object will be unlocked
	and then lock it.

		After synchronization object was locked
	it should be unlocked by calling leave() method.

	@warning There is @b deadlock possible.
*/
void CriticalSection::enter()
{
	::EnterCriticalSection(&m_impl);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Unlock synchronization object.
/**
		This method unlocks the synchronization object
	previously locked by enter() or try_enter() method.
*/
void CriticalSection::leave()
{
	::LeaveCriticalSection(&m_impl);
}

	} // CriticalSection


	// Event
	namespace sync
	{


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor (UNICODE).
/**
@param[in] manualReset If @b true the event object is manual-reset event,
	otherwise the event object is auto-reset event.
@param[in] initialSignaled The initial event state.
@param[in] name The event object name.
@throw std::runtime_error If can't create the event object.
*/
Event::Event(bool manualReset, bool initialSignaled, wchar_t const* name)
{
	m_impl = ::CreateEventW(NULL, manualReset, initialSignaled, name);
	if (!m_impl) throw std::runtime_error("can't create event object");
}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor (ANSI).
/**
@param[in] manualReset If @b true the event object is manual-reset event,
	otherwise the event object is auto-reset event.
@param[in] initialSignaled The initial event state.
@param[in] name The event object name.
@throw std::runtime_error If can't create the event object.
*/
Event::Event(bool manualReset, bool initialSignaled, char const* name)
{
	m_impl = ::CreateEventA(NULL, manualReset, initialSignaled, name);
	if (!m_impl) throw std::runtime_error("can't create event object");
}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] manualReset If @b true the event object is manual-reset event,
	otherwise the event object is auto-reset event.
@param[in] initialSignaled The initial event state.
@throw std::runtime_error If can't create the event object.
*/
Event::Event(bool manualReset, bool initialSignaled)
{
	m_impl = ::CreateEvent(NULL, manualReset, initialSignaled, NULL);
	if (!m_impl) throw std::runtime_error("can't create event object");
}


//////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
/**
		Closes the event object.
*/
Event::~Event()
{
	BOOL ret = ::CloseHandle(m_impl);
	assert(0!=ret && "can't close event object");
}


//////////////////////////////////////////////////////////////////////////
/// @brief Reset the event.
/**
		This method switches the event object to non-signaled state.
*/
bool Event::reset()
{
	return ::ResetEvent(m_impl) != 0;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the event.
/**
		This method switches the event object to signaled state.
*/
bool Event::set()
{
	return ::SetEvent(m_impl) != 0;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Wait for the event.
/**
		Wait while the event object will be in signaled state.

@param[in] timeout_ms The wait timeout, milliseconds.
@return @b true If the event in the signled state, otherwise @b false.
*/
bool Event::wait(DWORD timeout_ms)
{
	DWORD ret = ::WaitForSingleObject(m_impl, timeout_ms);
	return ret == WAIT_OBJECT_0;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Wait for the event.
/**
@return @b true If the event in the signled state, otherwise @b false.
*/
bool Event::wait()
{
	return wait(INFINITE);
}

	} // Event

} // omni namespace
