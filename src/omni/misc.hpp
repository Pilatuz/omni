////////////////////////////////////////////////////////////////////////////////////
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
@brief Interface of some miscellaneous classes.
@author Sergey Polichnoy <pilatuz@gmail.com>
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

////////////////////////////////////////////////////////////////////////////////////
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

	/// @brief Tick type (one millisecond).
	typedef long tick_type;

public:

	/// @brief The main constructor.
	/**
		This constructor sets the timer's interval to @a ms milliseconds.

	@param[in] ms The timer's interval in milliseconds.
	*/
	explicit Timer(tick_type ms = 1000);

public:

	/// @brief Get the timer's interval.
	/**
	@return The timer's interval in milliseconds.
	*/
	tick_type interval() const;


	/// @brief Is time interval elapsed?
	/**
	@return @b true if timer's interval is elapsed, otherwise @b false.
	*/
	bool elapsed() const;


	/// @brief Force elapsed.
	/**
			This method forces the timer, so next
		elapsed() method call will return @b true.
	*/
	void force();

private:
	mutable tick_type m_ref_point; ///< @brief The last time point.
	        tick_type m_interval; ///< @brief The interval.
};

	} // Timer


	// Events
	namespace misc
	{

////////////////////////////////////////////////////////////////////////////////////
/// @brief The Events class template.
/**
	This class is used to manage subscribers, i.e. insert or remove.
To emit some events use EventsImpl class.

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
	omni::misc::EventsImpl<MyEvents> m_events;

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
		typedef typename CallbackList::reverse_iterator Iterator;
		Iterator i = m_subscribers.rbegin();
		Iterator const ie = m_subscribers.rend();

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
		remove(subscriber);
		return *this;
	}

protected:

	/// @brief The list of subscribers.
	CallbackList m_subscribers;
};


////////////////////////////////////////////////////////////////////////////////////
/// @brief The EventsImpl class template.
/**
	This class is used to emit events.

@see Events
*/
template<typename Callback, typename CallbackPtr = Callback*>
class EventsImpl:
	public Events<Callback, CallbackPtr>
{
	typedef Events<Callback, CallbackPtr> base_type;
	typedef typename base_type::CallbackList::const_iterator Iterator;

public:

	/// @brief Emit event (no arguments).
	/**
	@param[in] f The callback method to call.
	*/
	void emit(void (Callback::*f)()) const
	{
		Iterator i = this->m_subscribers.begin();
		Iterator const ie = this->m_subscribers.end();

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
		Iterator i = this->m_subscribers.begin();
		Iterator const ie = this->m_subscribers.end();

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
		Iterator i = this->m_subscribers.begin();
		Iterator const ie = this->m_subscribers.end();

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
		Iterator i = this->m_subscribers.begin();
		Iterator const ie = this->m_subscribers.end();

		for (; i != ie; ++i)
			((*i)->*f)(a, b, c);
	}


	/// @brief Emit event (four arguments).
	/**
	@param[in] f The callback method to call.
	@param[in] a The first argument.
	@param[in] b The second argument.
	@param[in] c The third argument.
	@param[in] d The fourth argument.
	*/
	template<typename A, typename B, typename C, typename D>
	void emit(void (Callback::*f)(A,B,C,D), A a, B b, C c, D d) const
	{
		Iterator i = this->m_subscribers.begin();
		Iterator const ie = this->m_subscribers.end();

		for (; i != ie; ++i)
			((*i)->*f)(a, b, c, d);
	}
};

	} // Events
} // omni namespace

#endif // __OMNI_MISC_HPP_
