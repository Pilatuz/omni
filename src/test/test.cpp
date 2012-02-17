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
@brief Implementation of the test tools.
@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include "test.hpp"

#include <ostream>
#include <string>
#include <list>

#include <time.h>

// locals
namespace
{

///////////////////////////////////////////////////////////////////////////////
/// @brief The unit-test list type.
typedef std::list<omni::test::UnitTest*> UnitTestList;


///////////////////////////////////////////////////////////////////////////////
/// @brief The speed-test list type.
typedef std::list<omni::test::SpeedTest*> SpeedTestList;


///////////////////////////////////////////////////////////////////////////////
/// @brief Get the global unit-test list.
/**
@return The reference to global instance of unit-test list.
*/
UnitTestList& unit_test_list()
{
	static UnitTestList g_list;
	return g_list;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Get the global speed-test list.
/**
@return The reference to global instance of speed-test list.
*/
SpeedTestList& speed_test_list()
{
	static SpeedTestList g_list;
	return g_list;
}

} // locals


namespace omni
{
	// UnitTest
	namespace test
	{

///////////////////////////////////////////////////////////////////////////////
UnitTest::UnitTest()
{
	unit_test_list().push_back(this);
}


///////////////////////////////////////////////////////////////////////////////
UnitTest::~UnitTest()
{
	unit_test_list().remove(this);
}


///////////////////////////////////////////////////////////////////////////////
bool UnitTest::test(std::ostream &os) const
{
	return do_test(os);
}


///////////////////////////////////////////////////////////////////////////////
void UnitTest::testAll(std::ostream &os)
{
	const std::string SEPARATOR(64, '>');
	size_t N_success = 0;
	size_t N_failed = 0;

	UnitTestList::const_iterator it = unit_test_list().begin();
	UnitTestList::const_iterator const ite = unit_test_list().end();
	for (size_t i = 1; it != ite; ++it, ++i)
	{
		UnitTest *ptest = (*it);

		os << "\n" << SEPARATOR << "\n" << ">> " << i
			<< ". " << ptest->title() << "\n";

		try
		{
			N_failed += 1;
			if (!ptest->test(os))
			{
				os << "\tFAILED!!!\n";
			}
			else
			{
				os << "\tSUCCESS\n";
				N_success += 1;
				N_failed -= 1;
			}
		}
		catch (std::exception const& ex)
		{
			os << "\tERROR: "
				<< ex.what() << "\n";
		}
		catch (...)
		{
			os << "\tFATAL ERROR\n";
		}
	}

	os << "\n" << SEPARATOR << "\n"
		<< ">> success: " << N_success << "\n"
		<< ">> failed:  " << N_failed << "\n";
}

	} // UnitTest


	// SpeedTest
	namespace test
	{

///////////////////////////////////////////////////////////////////////////////
SpeedTest::SpeedTest()
{
	speed_test_list().push_back(this);
}


///////////////////////////////////////////////////////////////////////////////
SpeedTest::~SpeedTest()
{
	speed_test_list().remove(this);
}


///////////////////////////////////////////////////////////////////////////////
bool SpeedTest::test(std::ostream &os) const
{
	return do_test(os);
}


///////////////////////////////////////////////////////////////////////////////
void SpeedTest::testAll(std::ostream &os)
{
	const std::string SEPARATOR(64, '>');
	clock_t N_total = 0;

	SpeedTestList::const_iterator it = speed_test_list().begin();
	SpeedTestList::const_iterator const ite = speed_test_list().end();
	for (size_t i = 1; it != ite; ++it, ++i)
	{
		SpeedTest *ptest = (*it);

		os << "\n" << SEPARATOR << "\n" << ">> " << i
			<< ". " << ptest->title() << "\n";

		try
		{
			const clock_t start = clock();
			ptest->test(os);
			const clock_t stop = clock();
			N_total += (stop - start);

			os << "DURATION: "
				<< (stop-start)/double(CLOCKS_PER_SEC)
				<< " seconds\n";

		}
		catch (std::exception const& ex)
		{
			os << "\tERROR: "
				<< ex.what() << "\n";
		}
		catch (...)
		{
			os << "\tFATAL ERROR\n";
		}
	}

	os << "\n" << SEPARATOR << "\n"
		<< ">> TOTAL DURATION: "
		<< N_total/double(CLOCKS_PER_SEC)
		<< " seconds\n";
}

	} // SpeedTest
} // omni namespace
