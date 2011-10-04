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

///////////////////////////////////////////////////////////////////////////////
/// @brief The passive timer.
/**
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
	Timer t; // 1 second by default
	t = Timer(2000); // 2 seconds
	t = Timer(500);  // half of second
@endcode
*/
class Timer
{
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

///////////////////////////////////////////////////////////////////////////////
/// @brief The Events template class.
/**
	This class is used to manage subscribers, i.e. insert or remove.
To emit some events use EventsDriver class.

	The Callback interface is used to provide a set of callback methods.

@code
class Document
{
public:
	struct MyEvents
	{
		virtual void onChanging(Document *pDoc) = 0;
		virtual void onChanged(Document *pDoc) = 0;
	};

	omni::misc::Events<MyEvents>& events()
	{
		return m_events;
	}

private:
	omni::misc::EventsDriver<MyEvents> m_events;

	void doChaning()
	{
		m_events.emit(&MyEvents::onChanging, this);
	}

	void doChanged()
	{
		m_events.emit(&MyEvents::onChanged, this);
	}
};

@endcode

@warning By default this class stores the pointers to the subscribers.
	Be sure to remove subscriber reference before destroying.

It is also possible to use shread pointers here.
*/
template<typename Callback, typename CallbackPtr = Callback*>
class Events
{
public:
	typedef Events<Callback> ThisType;             ///< @brief The this type.
	typedef std::vector<CallbackPtr> CallbackList; ///< @brief The list of callbacks.

public:

	/// @brief The default constructor.
	Events()
	{}

	/// @brief The trivial destructor.
	~Events()
	{}

public:

	/// @brief Insert new events subscriber.
	/**
	@param[in] subscriber The subscriber pointer.
	*/
	void insert(CallbackPtr const& subscriber)
	{
		m_subscribers.push_back(subscriber);
	}


	/// @brief Remove existing events subscriber.
	/**
		The last occurence will be removed.

	@param[in] subscriber The subscriber pointer.
	@return true if subscriber removed.
	*/
	bool remove(CallbackPtr const& subscriber)
	{
		typedef typename CallbackList::reverse_iterator RevIterator;
		RevIterator i = m_subscribers.rbegin();
		RevIterator const ie = m_subscribers.rend();

		// we do not use std::find() here
		// to minimize number of included header files
		for (; i != ie; ++i)
			if (*i == subscriber)
		{
			m_subscribers.erase(i.base()-1);
			return true;
		}

		return false;
	}

public:

	/// @brief Insert new events subscriber.
	/**
	@param[in] subscriber The subscriber pointer.
	@return The this reference.
	@see insert()
	*/
	ThisType& operator+=(CallbackPtr const& subscriber)
	{
		insert(subscriber);
		return *this;
	}


	/// @brief Remove existing events subscriber.
	/**
	@param[in] subscriber The subscriber pointer.
	@return The this reference.
	@see remove()
	*/
	ThisType& operator-=(CallbackPtr const& subscriber)
	{
		remove(handler);
		return *this;
	}

protected:

	/// @brief The list of subscribers.
	CallbackList m_subscribers;
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The EventsDriver template class.
/**
	This class is used to emit events.

@see Events
*/
template<typename Callback, typename CallbackPtr = Callback*>
class EventsDriver:
	public Events<Callback, CallbackPtr>
{
	typedef typename CallbackList::const_iterator Iterator;

public:

	/// @brief Emit event (no arguments).
	/**
	@param[in] f The callback method to call.
	*/
	void emit(void (Callback::*f)()) const
	{
		Iterator i = m_subscribers.begin();
		Iterator const ie = m_subscribers.end();

		for (; i != ie; ++i)
			((*i)->*f)();
	}


	/// @brief Emit event (one argument).
	/**
	@param[in] f The callback method to call.
	@param[in] a The first argument.
	*/
	template<typename A>
	void emit(void (Callback::*f)(A), A a) const
	{
		Iterator i = m_subscribers.begin();
		Iterator const ie = m_subscribers.end();

		for (; i != ie; ++i)
			((*i)->*f)(a);
	}


	/// @brief Emit event (two arguments).
	/**
	@param[in] f The callback method to call.
	@param[in] a The first argument.
	@param[in] b The second argument.
	*/
	template<typename A, typename B>
	void emit(void (Callback::*f)(A,B), A a, B b) const
	{
		Iterator i = m_subscribers.begin();
		Iterator const ie = m_subscribers.end();

		for (; i != ie; ++i)
			((*i)->*f)(a, b);
	}


	/// @brief Emit event (three arguments).
	/**
	@param[in] f The callback method to call.
	@param[in] a The first argument.
	@param[in] b The second argument.
	@param[in] c The third argument.
	*/
	template<typename A, typename B, typename C>
	void emit(void (Callback::*f)(A,B,C), A a, B b, C c) const
	{
		Iterator i = m_subscribers.begin();
		Iterator const ie = m_subscribers.end();

		for (; i != ie; ++i)
			((*i)->*f)(a, b, c);
	}
};

	} // Events
} // omni namespace

#endif // __OMNI_MISC_HPP_
