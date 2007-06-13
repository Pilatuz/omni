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
	@brief The unit-test of "calc.hpp".

@author Sergey Polichnoy
*/
#include <omni/calc.hpp>
#include <omni/util.hpp>
#include <test/test.hpp>

#include <ostream>
#include <float.h>
#include <math.h>

#include <tchar.h>

using namespace omni::calc;

template class omni::calc::Calculator<double>;
template class omni::calc::Calculator<float>;
template class omni::calc::Calculator<long>;
template class omni::calc::Calculator<int>;

template double omni::calc::evalf(const std::wstring&);
template double omni::calc::evalf(const std::string&);
template long omni::calc::evali(const std::wstring&);
template long omni::calc::evali(const std::string&);

namespace {

//////////////////////////////////////////////////////////////////////////
// float testing
	bool ftest_(const _TCHAR *str, double x, const Calculator<double> &c)
	{
		double a = c(str);
		return fabs(a - x) <= DBL_EPSILON;
	}

	bool ftest(const _TCHAR *str, double x)
	{
		double a = evalf(str);
		return fabs(a - x) <= DBL_EPSILON;
	}

	bool itest(const _TCHAR *str, long x)
	{
		long a = evali(str);
		return a == x;
	}

} // namespace

// test function
bool test_calc(std::ostream &os)
{
#define TEST(expr) if (!(expr)) { os << "expression failed: \"" \
	<< #expr << "\" at line " << __LINE__ << "\n"; return false; }

# define TESTF(a) ftest(_T(#a), a)
# define TESTI(a) itest(_T(#a), a)

	TEST(TESTF( 2 + 2 * 2 ));
	TEST(TESTI( 2 + 2 * 2 ));
	TEST(TESTI( 0 -0+ 00 + 0x0 + 0xFF + 045 * 15));
	TEST(TESTF(  (  2  +  2  )  *  2  ));
	TEST(itest(_T("0"), 0));
	TEST(TESTI(  (  2  +  2  )  *  2  ));
	TEST(TESTI(  -2+2*2+2-(-2+2)*(2+2)*2-2  ));
	//TEST(TESTI( abs(-1) - 1 ));
	//TEST(TESTF(  sin(3.1)+cos(2.7) - 4.0));

	using namespace omni::util;

	TEST(ftest_(_T("1000 ms"), 1.0, time()));
	TEST(ftest_(_T("-5 dB"), dB2line(-5.0), ratio()));

	TEST(ftest_(_T("sin(30 degr) + cos(60 degr)"),
		sin(deg2rad(30)) + cos(deg2rad(60)), sci()));

# undef TESTF
# undef TESTI

	return true;
}

namespace {

	// Test1 class
	class Test1: public omni::test::UnitTest {
		// test title
		virtual const char* title() const
		{
			return "omni::calc";
		}

		// test function
		virtual bool do_test(std::ostream &os) const
		{
			return test_calc(os);
		}
	} test1;

} // namespace
