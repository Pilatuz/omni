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
	@brief The unit-test of "smart.hpp".

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include <omni/smart.hpp>
#include <test/test.hpp>

#include <ostream>

using omni::smart::SharedObj;
using omni::smart::SharedPtr;

namespace
{
	// TestObj1 class
	class TestObj1 {
	public:
		TestObj1() { N_count+=1; N_makes+=1; }
		TestObj1(const TestObj1&) { N_count+=1; N_makes+=1; }
		~TestObj1() { N_count-=1; }

		int v;

	public:
		static int N_count;
		static int N_makes;
	};
	int TestObj1::N_count = 0;
	int TestObj1::N_makes = 0;

	// TestObj2 class
	class TestObj2: public omni::smart::SharedObj {
	public:
		TestObj2() { N_count+=1; N_makes+=1; }
		TestObj2(const TestObj2&) { N_count+=1; N_makes+=1; }
		~TestObj2() { N_count-=1; }

		int v;

	public:
		static int N_count;
		static int N_makes;
	};
	int TestObj2::N_count = 0;
	int TestObj2::N_makes = 0;


	// test function
	template<typename T>
		bool do_test(SharedPtr<T> x)
	{
		typedef T TestObj;
		typedef SharedPtr<T> PTestObj;

		PTestObj p1;
		PTestObj p2(new TestObj());
		PTestObj p3 = PTestObj(new TestObj());
		PTestObj p4(p2);
		PTestObj p5 = p3;

		if (p1) return false;
		if (&*p1) return false;
		if (p1 != 0) return false;
		if (!(p1 == 0)) return false;
		if (!p2) return false;

		(*p2).v = 1;
		if (p4->v != 1) return false;

		if (!p4 || !p5) return false;
		if (!p2 || !p3) return false;
		if (!!p1) return false;

		p1 = PTestObj(new TestObj());
		p2 = p3 = p1;

		if (!p1) return false;
		if (p2 != p3 || p2 != p1) return false;
		if (p2 == p4 || p5 == p3) return false;

		return true;
	}

} // namespace


// SharedPtr unit test
bool test_smart(std::ostream&)
{
	// SharedPtr<TestObj1> test
	if (!do_test(SharedPtr<TestObj1>(0)))
		return false;

	// SharedPtr<TestObj2> test
	if (!do_test(SharedPtr<TestObj2>(0)))
		return false;

	return (0 == TestObj1::N_count)
		&& (0 == TestObj2::N_count)
		&& (0 < TestObj1::N_makes)
		&& (0 < TestObj2::N_makes);
}


//////////////////////////////////////////////////////////////////////////
// unit test
namespace
{
	// Test1 class
	class Test1: public omni::test::UnitTest {
		// test title
		virtual const char* title() const
		{
			return "omni::smart";
		}

		// test function
		virtual bool do_test(std::ostream &os) const
		{
			return test_smart(os);
		}
	} test1;
} // namespace
