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
@brief Implementation of the miscellaneous classes.

		This file contains implementation of the omni::misc::Timer class.

@author Sergey Polichnoy
*/
#include <omni/misc.hpp>

#include <time.h>

namespace omni
{
	// Timer class
	namespace misc
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Default interval.
/**
		This constructor initializes the timer's interval by one second.
*/
Timer::Timer()
	: m_ref_point(clock()),
	  m_interval(CLOCKS_PER_SEC)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Set timer's interval.
/**
		This constructor initializes the timer's interval by @a ms milliseconds.

@param[in] ms Timer's interval in milliseconds.
*/
Timer::Timer(tick_type ms)
	: m_ref_point(clock()),
	  m_interval(ms * 1000/CLOCKS_PER_SEC)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get timer's interval.
/**
		This method returns the current timer's interval.

@return Timer's interval in milliseconds.
*/
Timer::tick_type Timer::interval() const
{
	return m_interval * CLOCKS_PER_SEC/1000;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Check timer.
/**
		This method checks the timer's interval is elapsed.

@return @b true if timer's interval is elapsed, otherwise @b false.
*/
bool Timer::elapsed() const
{
	const tick_type now = clock();
	if (m_interval < (now - m_ref_point))
	{
		m_ref_point = now;
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Force timer.
/**
		This method forces the timer, so next
	elapsed() method call will return @b true.
*/
void Timer::force()
{
	const tick_type now = clock();
	m_ref_point = now - (m_interval+1);
}

	} // Timer
} // omni namespace
