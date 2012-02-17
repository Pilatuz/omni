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
@brief The unit-test of <omni/util.hpp>.
@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include <omni/util.hpp>
#include <test/test.hpp>

#include <algorithm>
#include <ostream>
#include <iomanip>
#include <vector>

#include <float.h>
#include <math.h>

// helpers
namespace
{
	// largest integer
	typedef unsigned long long UInt64;

	// dummy flp2()
	template<typename T>
		T dummy_flp2(T x)
	{
		UInt64 res = UInt64(1) << (sizeof(T)*8 - 1);

		while (UInt64(x) < res)
			res >>= 1;

		return (T)res;
	}


	// dummy clp2()
	template<typename T>
		T dummy_clp2(T x)
	{
		UInt64 res = UInt64(1);

		while (res < UInt64(x))
			res <<= 1;

		return (T)res;
	}


	// dummy parity()
	template<typename T>
		T dummy_parity(T x)
	{
		size_t n = 0;

		for (size_t i = 0; i < (sizeof(T)*8 - 1); ++i)
			if (x & (UInt64(1)<<i)) ++n;

		return T(n&1);
	}

	// dummy poly()
	template<typename T, typename In>
		T dummy_poly(const T &x, In first, In last)
	{
		if (first != last)
		{
			T ret = T();

			size_t N = std::distance(first, last);
			for (size_t n = 0; first != last; ++first, ++n)
				ret += *first * T(pow(double(x), int(N-n-1)));

			return ret;
		}
		else
			return T();
	}

	// log2() testing
	template<typename T>
		bool log2_test(T x)
		{
			for (size_t i = 0; i < (sizeof(T)*8-1); ++i)
			{
				x = T(UInt64(1) << i);
				if (omni::util::log2(x) != (T)i)
					return false;
			}

			return true;
		}

	// clp2(), flp2() parity() testing
	template<typename T>
		bool xxx_test(T)
		{
			for (T x = 1; x < (T(1) << sizeof(T)*8/4); ++x)
			{
				if (dummy_clp2(x) != omni::util::clp2(x))
					return false;
				if (dummy_flp2(x) != omni::util::flp2(x))
					return false;
				if (dummy_parity(x) != omni::util::parity(x))
					return false;
			}

			return true;
		}

} // helpers


// unit test
namespace
{

///////////////////////////////////////////////////////////////////////////////
/// @brief The omni::util unit test.
class UtilTest:
	public omni::test::UnitTest
{
private:

	// test title
	virtual const char* title() const
	{
		return "omni::util";
	}


private:

	// test function
	virtual bool do_test(std::ostream &os) const
	{
#define TEST(expr) if (expr) {} else { os << "expression failed: \"" \
	<< #expr << "\" at line " << __LINE__ << "\n"; return false; }

		using namespace omni::util;

		os << " some examples...\n";
		os << std::setw(8) << "x:"
			<< std::setw(8) << "clp2:"
			<< std::setw(8) << "flp2:"
			<< std::setw(8) << "parity:"
			<< "\n";
		for (int i = 0; i < 18; ++i)
		{
			os << std::setw(8) << i
				<< std::setw(8) << clp2(i)
				<< std::setw(8) << flp2(i)
				<< std::setw(8) << parity(i)
				<< "\n";
		}
		os << "\n";

		// constants test
		os << " constants testing.......";
		TEST(fabs(SQRT3 - sqrt(3.0)) <= DBL_EPSILON);
		TEST(fabs(SQRT2 - sqrt(2.0)) <= DBL_EPSILON);
		TEST(fabs(LG2 - log10(2.0)) <= DBL_EPSILON);
		TEST(fabs(LN2 - log(2.0)) <= DBL_EPSILON);
		TEST(fabs(PI - acos(-1.0)) <= DBL_EPSILON);
		os << "done\n";

		// log2 test
		os << " log2 testing............";
		TEST(log2_test<char>('\0'));
		TEST(log2_test<short>(0));
		TEST(log2_test<int>(0));
		TEST(log2_test<long>(0L));
		TEST(log2_test<long long>(0LL));
		TEST(log2_test<unsigned char>('\0'));
		TEST(log2_test<unsigned short>(0U));
		TEST(log2_test<unsigned int>(0U));
		TEST(log2_test<unsigned long>(0UL));
		TEST(log2_test<unsigned long long>(0ULL));
		os << "done\n";

		// clp2, flp2, parity
		os << " clp2, flp2 testing......";
		TEST(xxx_test<char>('\0'));
		TEST(xxx_test<short>(0));
		TEST(xxx_test<int>(0));
		TEST(xxx_test<long>(0L));
		TEST(xxx_test<long long>(0LL));
		TEST(xxx_test<unsigned char>('\0'));
		TEST(xxx_test<unsigned short>(0U));
		TEST(xxx_test<unsigned int>(0U));
		TEST(xxx_test<unsigned long>(0UL));
		TEST(xxx_test<unsigned long long>(0ULL));
		os << "done\n";

		{ // pack/unpack
		os << " bi2de, de2bi testing....";
		int bits1[] = {1,1,1,0,0,0}; enum { N1 = sizeof(bits1)/sizeof(bits1[0]) };
		int bits2[] = {1,0,0,0,0,1}; enum { N2 = sizeof(bits2)/sizeof(bits2[0]) };
		int test1_lsb[N1] = { 0 };
		int test2_lsb[N2] = { 0 };
		int test1_msb[N1] = { 0 };
		int test2_msb[N2] = { 0 };

		TEST(bi2de_lsb<int>(bits1, N1) == 0x07);
		TEST(bi2de_lsb<int>(bits2, N2) == 0x21);
		TEST(bi2de_msb<int>(bits1, N1) == 0x38);
		TEST(bi2de_msb<int>(bits2, N2) == 0x21);
		de2bi_lsb(0x07, N1, test1_lsb);
		de2bi_lsb(0x21, N2, test2_lsb);
		de2bi_msb(0x38, N1, test1_msb);
		de2bi_msb(0x21, N2, test2_msb);
		TEST(std::equal(bits1, bits1+N1, test1_lsb));
		TEST(std::equal(bits2, bits2+N2, test2_lsb));
		TEST(std::equal(bits1, bits1+N1, test1_msb));
		TEST(std::equal(bits2, bits2+N2, test2_msb));

		for (size_t k = 0; k < 100; ++k)
		{
			size_t N = rand() % 30;
			std::vector<int> msb(N);
			std::vector<int> lsb(N);
			std::vector<int> xx(N);

			for (size_t q = 0; q < 100; ++q)
			{
				size_t x = rand();

				for (size_t i = 0; i < N; ++i)
					xx[i] = 0 < (x&(1<<i)); // LSB

				de2bi_lsb(x, N, lsb.begin());
				de2bi_msb(x, N, msb.begin());
				TEST((x%(1<<N)) == bi2de_lsb(lsb.begin(), N, 0U));
				TEST((x%(1<<N)) == bi2de_msb(msb.begin(), N, 0U));

				std::reverse(msb.begin(), msb.end());
				TEST(std::equal(xx.begin(), xx.end(), lsb.begin()));
				TEST(std::equal(xx.begin(), xx.end(), msb.begin()));
			}
		}
		os << "done\n";
		}

		// bits_flip
		os << " bits testing............";
		TEST(bits_flip<int>(0, sizeof(int)*8) == 0);
		TEST(bits_flip<int>(-1, sizeof(int)*8) == -1);
		TEST(bits_flip(0x38, 6) == 0x07);
		TEST(bits_flip(0x21, 6) == 0x21);
		TEST(bits_flip(0x21, 3) == 0x04);
		os << "done\n";

		// polynomial testing
		os << " poly testing............";
		for (size_t k = 0; k < 1000; ++k)
		{
			size_t n = rand() % 10;
			std::vector<size_t> cf(n);
			for (size_t i = 0; i < n; ++i)
				cf[i] = rand() % 5;

			for (size_t x = 0; x < 20; ++x)
			{
				size_t y1 = poly(x, cf.begin(), cf.end());
				size_t y2 = dummy_poly(x, cf.begin(), cf.end());
				if (y1 != y2)
					return false;
			}
		}
		os << "done\n";

		return true;
	}
};

	// global instance
	UtilTest g_UtilTest;

} // unit test
