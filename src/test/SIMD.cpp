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
	@brief The unit-test of "SIMD.hpp".

@author Sergey Polichnoy
*/
#include <omni/SIMD.hpp>
#include <test/test.hpp>

#include <ostream>
#include <float.h>
#include <math.h>

namespace {

	// check the 2 vectors
	template<typename T>
	bool equal(size_t N, const T *X, const T *Y, const T &eps)
	{
		for (size_t i = 0; i < N; ++i)
			if (fabs(X[i] - Y[i]) > eps)
				return false;

		return true;
	}

	// check the 2 complex vectors
	template<typename T>
	bool equal(size_t N, const std::complex<T> *X, const std::complex<T> *Y, const T &eps)
	{
		for (size_t i = 0; i < N; ++i)
			if (norm(X[i] - Y[i]) > eps)
				return false;

		return true;
	}

} // local


// SIMD unit test
bool test_SIMD(std::ostream &os)
{
	using namespace omni::SIMD;
	bool ret = true;

	os << "\tCapability: ";
	if (Capability::MMX) os << "MMX ";
	if (Capability::_3DNow) os << "3DNow! ";
	if (Capability::SSE) os << "SSE ";
	if (Capability::SSE2) os << "SSE2 ";
	os << "\n\n";

	const size_t N = 1024;

	__declspec(align(16)) Complex Xcd[N], Ycd[N], Z1cd[N], Z2cd[N];
	__declspec(align(16)) ComplexF Xcf[N], Ycf[N], Z1cf[N], Z2cf[N];
	__declspec(align(16)) double Xd[N], Yd[N], Z1d[N], Z2d[N];
	__declspec(align(16)) float Xf[N], Yf[N], Z1f[N], Z2f[N];

	// initialize
	for (size_t i = 0; i < N; ++i)
	{
		Xcd[i] = Complex(rand()/double(RAND_MAX), rand()/double(RAND_MAX));
		Ycd[i] = Complex(rand()/double(RAND_MAX), rand()/double(RAND_MAX));
		Xcf[i] = ComplexF(rand()/float(RAND_MAX), rand()/float(RAND_MAX));
		Ycf[i] = ComplexF(rand()/float(RAND_MAX), rand()/float(RAND_MAX));

		Xd[i] = rand()/double(RAND_MAX);
		Yd[i] = rand()/double(RAND_MAX);
		Xf[i] = rand()/float(RAND_MAX);
		Yf[i] = rand()/float(RAND_MAX);
	}

	{ // Complex testing
		os << "\tComplex:  ";
		bool ok = true;

		for (size_t d = 0; d < 24; ++d)
		{
			add_T(N-d, Z1cd, Xcd, Ycd);
			add(N-d, Z2cd, Xcd, Ycd);
			if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
			{
				ok = false;
				break;
			}
		}

		os << (ok ? "OK" : "FAILED") << "\n";
		ret &= ok;
	}

	{ // ComplexF testing
		os << "\tComplexF: ";
		bool ok = true;

		for (size_t d = 0; d < 24; ++d)
		{
			add_T(N-d, Z1cf, Xcf, Ycf);
			add(N-d, Z2cf, Xcf, Ycf);
			if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
			{
				ok = false;
				break;
			}
		}

		os << (ok ? "OK" : "FAILED") << "\n";
		ret &= ok;
	}

	{ // double testing
		os << "\tdouble:   ";
		bool ok = true;

		for (size_t d = 0; d < 24; ++d)
		{
			add_T(N-d, Z1d, Xd, Yd);
			add(N-d, Z2d, Xd, Yd);
			if (!equal(N-d, Z1d, Z2d, DBL_EPSILON))
			{
				ok = false;
				break;
			}
		}

		os << (ok ? "OK" : "FAILED") << "\n";
		ret &= ok;
	}

	{ // float testing
		os << "\tfloat:    ";
		bool ok = true;

		for (size_t d = 0; d < 24; ++d)
		{
			add_T(N-d, Z1f, Xf, Yf);
			add(N-d, Z2f, Xf, Yf);
			if (!equal(N-d, Z1f, Z2f, FLT_EPSILON))
			{
				ok = false;
				break;
			}
		}

		os << (ok ? "OK" : "FAILED") << "\n";
		ret &= ok;
	}

	os << "\n";
	return ret;
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
			return "omni::SIMD";
		}

		// test function
		virtual bool do_test(std::ostream &os) const
		{
			return test_SIMD(os);
		}
	} test1;
} // namespace


// TODO: spped test
