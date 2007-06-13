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
@brief Interface of the miscellaneous classes.

		This file contains interface of the omni::misc::Timer class.

@author Sergey Polichnoy
*/
#ifndef __OMNI_MISC_HPP_
#define __OMNI_MISC_HPP_

#include <omni/defs.hpp>

namespace omni
{
	namespace misc
	{

//////////////////////////////////////////////////////////////////////////
/** @class Timer
	@brief %Timer object.

		The Timer class is used for keeping time intervals.

		It may be used to perform some periodic actions.
	For example to print simulation statistics each second:

@code
	Timer t(1000);

	while (1)
	{
		// simulation...

		if (t.elapsed())
		{
			// print statistics...
		}
	}
@endcode

		One timer's tick is equal to one millisecond.

		To change timer's interval the following code can be used:

@code
	Timer t; // default interval - 1 second
	t = Timer(2000); // 2 seconds
	t = Timer(500);  // 1/2 second
@endcode
*/
class Timer {
public:
	typedef long tick_type; ///< @brief Tick type.

public:
	Timer();
	explicit Timer(tick_type ms);

public:
	tick_type interval() const;
	bool elapsed() const;
	void force();

private:
	mutable tick_type m_ref_point;
	        tick_type m_interval;
};

	} // misc namespace
} // omni namespace

#endif // __OMNI_MISC_HPP_
