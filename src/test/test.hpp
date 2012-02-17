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
	@brief Implementation of the test tools.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include <test/test.hpp>

#include <ostream>
#include <string>
#include <list>

#include <time.h>

namespace omni
{
	namespace test
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief The unit-test list.
typedef std::list<UnitTest*> UnitTestList;

//////////////////////////////////////////////////////////////////////////
/// @brief Get the global unit-test list.
UnitTestList& unit_test_list()
{
	static UnitTestList g_list;
	return g_list;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The speed-test list.
typedef std::list<SpeedTest*> SpeedTestList;

//////////////////////////////////////////////////////////////////////////
/// @brief Get the global speed-test list.
SpeedTestList& speed_test_list()
{
	static SpeedTestList g_list;
	return g_list;
}

		} // details namespace
	} // test namespace


	// UnitTest
	namespace test
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Run all registered unit-tests.
/**
@param os The output stream.
*/
void UnitTest::testAll(std::ostream &os)
{
	const std::string SEPARATOR(64, '>');
	size_t N_success = 0;
	size_t N_failed = 0;

	details::UnitTestList::const_iterator it = details::unit_test_list().begin();
	details::UnitTestList::const_iterator ite = details::unit_test_list().end();
	for (size_t i = 1; it != ite; ++it, ++i)
	{
		UnitTest *ptest = (*it);

		os << "\n" << SEPARATOR << "\n" << ">> " << i
			<< ". " << ptest->title() << "\n";

		try {
			N_failed += 1;
			if (!ptest->do_test(os))
				os << "\tTEST FAILED!!!\n";
			else
			{
				os << "\tSUCCESS\n";
				N_success += 1;
				N_failed -= 1;
			}
		} catch (const std::exception &e) {
			os << "\tERROR: " << e.what() << "\n";
		} catch (...) {
			os << "\tFATAL ERROR\n";
		}
	}

	os << "\n" << SEPARATOR << "\n"
		<< ">> success: " << N_success << "\n"
		<< ">> failed:  " << N_failed << "\n";
}


//////////////////////////////////////////////////////////////////////////
/// @brief Register unit-test.
UnitTest::UnitTest()
{
	details::unit_test_list().push_back(this);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Unregister unit-test.
UnitTest::~UnitTest()
{
	details::unit_test_list().remove(this);
}

	} // UnitTest


	// SpeedTest
	namespace test
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Run all registered speed-tests.
/**
@param os The output stream.
*/
void SpeedTest::testAll(std::ostream &os)
{
	const std::string SEPARATOR(64, '>');
	clock_t N_total = 0;

	details::SpeedTestList::const_iterator it = details::speed_test_list().begin();
	details::SpeedTestList::const_iterator ite = details::speed_test_list().end();
	for (size_t i = 1; it != ite; ++it, ++i)
	{
		SpeedTest *ptest = (*it);

		os << "\n" << SEPARATOR << "\n" << ">> " << i
			<< ". " << ptest->title() << "\n";

		try {
			clock_t start = clock();
			ptest->do_test(os);
			clock_t stop = clock();
			N_total += (stop - start);

			os << "DURATION: "
				<< (stop-start)/double(CLOCKS_PER_SEC)
				<< " seconds\n";

		} catch (const std::exception &e) {
			os << "\tERROR: " << e.what() << "\n";
		} catch (...) {
			os << "\tFATAL ERROR\n";
		}
	}

	os << "\n" << SEPARATOR << "\n"
		<< ">> TOTAL DURATION: "
		<< N_total/double(CLOCKS_PER_SEC)
		<< " seconds\n";
}


//////////////////////////////////////////////////////////////////////////
/// @brief Register speed-test.
SpeedTest::SpeedTest()
{
	details::speed_test_list().push_back(this);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Unregister speed-test.
SpeedTest::~SpeedTest()
{
	details::speed_test_list().remove(this);
}

	} // SpeedTest
} // omni namespace
