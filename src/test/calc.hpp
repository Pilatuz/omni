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
@brief The unit-test of <omni/calc.hpp>.
@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include <omni/calc.hpp>
#include <omni/util.hpp>
#include <test/test.hpp>

#include <ostream>
#include <float.h>
#include <math.h>

template class omni::calc::Calculator<double>;
template class omni::calc::Calculator<float>;
template class omni::calc::Calculator<long>;
template class omni::calc::Calculator<int>;

template double omni::calc::atof(const std::wstring&);
template double omni::calc::atof(const std::string&);
template long omni::calc::atoi(const std::wstring&);
template long omni::calc::atoi(const std::string&);


// helpers
namespace
{

//////////////////////////////////////////////////////////////////////////
// float testing
template<typename Ch, typename T>
bool ftest(const omni::calc::Calculator<T> &c, const Ch *expr, T etalon, T epsilon)
{
	const T x = c(expr);
	return fabs(x - etalon) <= epsilon;
}


//////////////////////////////////////////////////////////////////////////
// integer testing
template<typename Ch, typename T>
bool itest(const omni::calc::Calculator<T> &c, const Ch *expr, T etalon)
{
	const T x = c(expr);
	return x == etalon;
}

} // helpers


// unit test
namespace
{

///////////////////////////////////////////////////////////////////////////////
/// @brief The omni::calc unit test.
class CalcTest:
	public omni::test::UnitTest
{
private:

	// test title
	virtual const char* title() const
	{
		return "omni::calc";
	}

private:

	// test function
	virtual bool do_test(std::ostream &os) const
	{
#define TEST(expr) if (expr) {} else { os << "expression failed: \"" \
	<< #expr << "\" at line " << __LINE__ << "\n"; return false; }

# define TESTF(expr, eps) (ftest(calc::Calculator<double>(), L#expr, expr, eps) && ftest(calc::Calculator<double>(), #expr, expr, eps))
# define TESTI(expr) (itest(calc::Calculator<int>(), L#expr, expr) && itest(calc::Calculator<int>(), #expr, expr))

		using namespace omni;

		TEST(TESTF( 2 + 2 * 2.0, DBL_EPSILON ));
		TEST(TESTI( 2 + 2 * 2 ));
		TEST(TESTI( 0 -0+ 00 + 0x0 + 0xFF + 045 * 15));
		TEST(TESTF(  (  2  +  2  )  *  2.0 , DBL_EPSILON ));
		TEST(TESTI( 0 ));
		TEST(TESTI(  (  2  +  2  )  *  2  ));
		TEST(TESTI(  -2+2*2+2-(-2+2)*(2+2)*2-2  ));
		//TEST(TESTI( abs(-1) - 1 ));
		//TEST(TESTF(  sin(3.1)+cos(2.7) - 4.0));

		using namespace omni::util;

		TEST(ftest(calc::time(), "1000 ms", 1.0, DBL_EPSILON));
		TEST(ftest(calc::ratio(), "-5 dB", dB2line(-5.0), DBL_EPSILON));
		TEST(ftest(calc::freq(), "22/4.5 kHz kHz", 22/4.5*1.0e6, 1.0e-5));

		TEST(ftest(calc::sci(), "sin(30 degr) + cos(60 degr)",
			sin(deg2rad(30)) + cos(deg2rad(60)), DBL_EPSILON));

#undef TESTF
#undef TESTI
#undef TEST

		return true;
	}
};

	// global instance
	CalcTest g_CalcTest;

} // unit test
