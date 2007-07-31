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

#include <vector>

namespace omni
{
	// Timer
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

	} // Timer


	// Events
	namespace misc
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The Events template class.
template<typename HANDLER, typename AL = std::allocator<HANDLER*> >
class Events {
	typedef std::vector<HANDLER*, AL> ListType;

public:
	typedef Events<HANDLER, AL> this_type;
	typedef HANDLER handler_type;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Insert new events handler.
	void insert(handler_type &handler)
	{
		m_handlers.push_back(&handler);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Remove existing events handler.
	bool remove(handler_type &handler)
	{
		typename ListType::reverse_iterator i = m_handlers.rbegin();
		typename ListType::reverse_iterator ie = m_handlers.rend();

		for (; i != ie; ++i)
			if (*i == &handler)
		{
			m_handlers.erase(i.base()-1);
			return true;
		}

		return false;
	}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Insert new events handler.
	this_type& operator+=(handler_type &handler)
	{
		insert(handler);
		return *this;
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Remove existing events handler.
	this_type& operator-=(handler_type &handler)
	{
		remove(handler);
		return *this;
	}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Emit event (no arguments).
	void emit(void (HANDLER::*f)()) const
	{
		typename ListType::const_iterator i = m_handlers.begin();
		typename ListType::const_iterator ie = m_handlers.end();

		for (; i != ie; ++i)
			((*i)->*f)();
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Emit event (one argument).
	template<typename A1>
		void emit(void (HANDLER::*f)(A1), A1 a1) const
	{
		typename ListType::const_iterator i = m_handlers.begin();
		typename ListType::const_iterator ie = m_handlers.end();

		for (; i != ie; ++i)
			((*i)->*f)(a1);
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Emit event (two arguments).
	template<typename A1, typename A2>
		void emit(void (HANDLER::*f)(A1, A2), A1 a1, A2 a2) const
	{
		typename ListType::const_iterator i = m_handlers.begin();
		typename ListType::const_iterator ie = m_handlers.end();

		for (; i != ie; ++i)
			((*i)->*f)(a1, a2);
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Emit event (three arguments).
	template<typename A1, typename A2, typename A3>
		void emit(void (HANDLER::*f)(A1, A2, A3), A1 a1, A2 a2, A3 a3) const
	{
		typename ListType::const_iterator i = m_handlers.begin();
		typename ListType::const_iterator ie = m_handlers.end();

		for (; i != ie; ++i)
			((*i)->*f)(a1, a2, a3);
	}

private:
	ListType m_handlers;
};

	} // Events
} // omni namespace

#endif // __OMNI_MISC_HPP_
