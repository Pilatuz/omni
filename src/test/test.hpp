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
//		https://bitbucket.org/pilatuz/omni
//////////////////////////////////////////////////////////////////////////
/** @file
	@brief Interface of the test tools.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_TEST_HPP_
#define __OMNI_TEST_HPP_

#include <omni/defs.hpp>

#include <iosfwd>

namespace omni
{
	namespace test
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The unit-test class.
/**
*/
class UnitTest: private omni::NonCopyable {
public:
	virtual const char* title() const = 0;
	static void testAll(std::ostream &os);

protected:
	UnitTest();
	virtual ~UnitTest();

protected:
	virtual bool do_test(std::ostream &os) const = 0;
};


//////////////////////////////////////////////////////////////////////////
/// @brief The speed-test class.
/**
*/
class SpeedTest: private omni::NonCopyable {
public:
	virtual const char *title() const = 0;
	static void testAll(std::ostream &os);

protected:
	SpeedTest();
	virtual ~SpeedTest();

protected:
	virtual void do_test(std::ostream &os) const = 0;
};

	} // test namespace
} // omni namespace

#endif // __OMNI_TEST_HPP_
