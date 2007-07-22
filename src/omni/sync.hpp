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

namespace omni
{
	namespace sync
	{

//////////////////////////////////////////////////////////////////////////
/// @brief %Lockable interface.
/**
		This class is used as interface for the some synchronization objects.
	The public interface contains two methods:
		- enter() - lock synchronization object,
		- leave() - unlock synchronization object.

		It is recommended to use Locker guard class
	instead direct enter()/leave() method calling.

@see Locker
*/
class Lockable {
protected:
	Lockable() {}    ///< @brief Trivial constructor.
	~Lockable() {}   ///< @brief Trivial destructor.

public:
	virtual void enter() = 0;
	virtual void leave() = 0;
};


//////////////////////////////////////////////////////////////////////////
/// @brief %Locker guard.
/**
		This class holds the synchronization object: constructor locks
	this object and destructor unlocks this object.

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
		Locker guard(x);
		// ...
	}
@endcode

		Even exception was thrown during function execution, the
	synchronization object will be properly unlocked.
*/
class Locker: private omni::NonCopyable {
public:
	explicit Locker(Lockable &lock);
	~Locker();

private:
	Lockable &m_lock;
};


//////////////////////////////////////////////////////////////////////////
/// @brief Critical Section synchronization object.
/**
		This class is used for intra-process synchronization.

		It implements Lockable interface for @b Win32 and @b *nix
	platform (based on @b CRITICAL_SECTION structure for @b Win32
	and on pthread_mutex for @b *nix).
*/
class CriticalSection: public Lockable,
	private omni::NonCopyable {
public:
	CriticalSection();
	~CriticalSection();

public: // Lockable interface
	virtual void enter();
	virtual void leave();

private:
	class Impl;
	Impl *impl;
};

	} // sync namespace
} // omni namespace

#endif // __OMNI_SYNC_HPP_
