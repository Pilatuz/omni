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
	@brief Implementation of some miscellaneous classes.

		This file contains implementation of the omni::misc::Timer class.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include <omni/misc.hpp>

#include <time.h>

namespace omni
{
	// Timer class
	namespace misc
	{

///////////////////////////////////////////////////////////////////////////////
Timer::Timer(tick_type ms)
	: m_ref_point(clock()),
	  m_interval(ms*(CLOCKS_PER_SEC/1000)) // ms -> clocks
{}


///////////////////////////////////////////////////////////////////////////////
Timer::tick_type Timer::interval() const
{
	return m_interval/(CLOCKS_PER_SEC/1000); // clocks -> ms
}


///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
void Timer::force()
{
	const tick_type now = clock();
	m_ref_point = now - (m_interval+1);
}

	} // Timer
} // omni namespace
