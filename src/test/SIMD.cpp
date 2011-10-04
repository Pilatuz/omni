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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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
	if (Capability::SSE3) os << "SSE3 ";
	if (Capability::SSSE3) os << "SSSE3 ";
	if (Capability::SSE4_1) os << "SSE4.1 ";
	if (Capability::SSE4_2) os << "SSE4.2 ";
	os << "\n";

	const size_t N = 1024;
	const size_t D = 64;

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

	{ // add
		{ // Complex testing
			os << "\n\t[add]Complex:    ";

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						add_T(N-d, Z1cd, Xcd, Ycd);
						add_SSE2(N-d, Z2cd, Xcd, Ycd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						add_T(N-d, Z1cd, Xcd, Ycd);
						add(N-d, Z2cd, Xcd, Ycd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // ComplexF testing
			os << "\n\t[add]ComplexF:   ";

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						add_T(N-d, Z1cf, Xcf, Ycf);
						add_SSE(N-d, Z2cf, Xcf, Ycf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						add_T(N-d, Z1cf, Xcf, Ycf);
						add(N-d, Z2cf, Xcf, Ycf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // double testing
			os << "\n\t[add]double:     ";

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						add_T(N-d, Z1d, Xd, Yd);
						add_SSE2(N-d, Z2d, Xd, Yd);
						if (!equal(N-d, Z1d, Z2d, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						add_T(N-d, Z1d, Xd, Yd);
						add(N-d, Z2d, Xd, Yd);
						if (!equal(N-d, Z1d, Z2d, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // float testing
			os << "\n\t[add]float:      ";

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						add_T(N-d, Z1f, Xf, Yf);
						add_SSE(N-d, Z2f, Xf, Yf);
						if (!equal(N-d, Z1f, Z2f, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						add_T(N-d, Z1f, Xf, Yf);
						add(N-d, Z2f, Xf, Yf);
						if (!equal(N-d, Z1f, Z2f, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		os << "\n";
	} // add


	{ // sub
		{ // Complex testing
			os << "\n\t[sub]Complex:    ";

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						sub_T(N-d, Z1cd, Xcd, Ycd);
						sub_SSE2(N-d, Z2cd, Xcd, Ycd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						sub_T(N-d, Z1cd, Xcd, Ycd);
						sub(N-d, Z2cd, Xcd, Ycd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // ComplexF testing
			os << "\n\t[sub]ComplexF:   ";

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						sub_T(N-d, Z1cf, Xcf, Ycf);
						sub_SSE(N-d, Z2cf, Xcf, Ycf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						sub_T(N-d, Z1cf, Xcf, Ycf);
						sub(N-d, Z2cf, Xcf, Ycf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // double testing
			os << "\n\t[sub]double:     ";

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						sub_T(N-d, Z1d, Xd, Yd);
						sub_SSE2(N-d, Z2d, Xd, Yd);
						if (!equal(N-d, Z1d, Z2d, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						sub_T(N-d, Z1d, Xd, Yd);
						sub(N-d, Z2d, Xd, Yd);
						if (!equal(N-d, Z1d, Z2d, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // float testing
			os << "\n\t[sub]float:      ";

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						sub_T(N-d, Z1f, Xf, Yf);
						sub_SSE(N-d, Z2f, Xf, Yf);
						if (!equal(N-d, Z1f, Z2f, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						sub_T(N-d, Z1f, Xf, Yf);
						sub(N-d, Z2f, Xf, Yf);
						if (!equal(N-d, Z1f, Z2f, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		os << "\n";
	} // sub


	{ // mul
		{ // Complex testing
			os << "\n\t[mul]Complex:    ";

			// SSE3
			if (Capability::SSE3)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cd, Xcd, Ycd);
						mul_SSE3(N-d, Z2cd, Xcd, Ycd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE3"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cd, Xcd, Ycd);
						mul_SSE2(N-d, Z2cd, Xcd, Ycd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cd, Xcd, Ycd);
						mul(N-d, Z2cd, Xcd, Ycd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // Complex' testing
			os << "\n\t[mul]Complex*:   ";

			// SSE3
			if (Capability::SSE3)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cd, Xcd, Yd);
						mul_SSE3(N-d, Z2cd, Xcd, Yd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE3"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cd, Xcd, Yd);
						mul_SSE2(N-d, Z2cd, Xcd, Yd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cd, Xcd, Yd);
						mul(N-d, Z2cd, Xcd, Yd);
						if (!equal(N-d, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // ComplexF testing
			os << "\n\t[mul]ComplexF:   ";

			// SSE3
			if (Capability::SSE3)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cf, Xcf, Ycf);
						mul_SSE3(N-d, Z2cf, Xcf, Ycf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE3"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cf, Xcf, Ycf);
						mul_SSE(N-d, Z2cf, Xcf, Ycf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cf, Xcf, Ycf);
						mul(N-d, Z2cf, Xcf, Ycf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // ComplexF' testing
			os << "\n\t[mul]ComplexF*:  ";

			// SSE3
			if (Capability::SSE3)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cf, Xcf, Yf);
						mul_SSE3(N-d, Z2cf, Xcf, Yf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE3"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cf, Xcf, Yf);
						mul_SSE(N-d, Z2cf, Xcf, Yf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1cf, Xcf, Yf);
						mul(N-d, Z2cf, Xcf, Yf);
						if (!equal(N-d, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // double testing
			os << "\n\t[mul]double:     ";

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1d, Xd, Yd);
						mul_SSE2(N-d, Z2d, Xd, Yd);
						if (!equal(N-d, Z1d, Z2d, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1d, Xd, Yd);
						mul(N-d, Z2d, Xd, Yd);
						if (!equal(N-d, Z1d, Z2d, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // float testing
			os << "\n\t[mul]float:      ";

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1f, Xf, Yf);
						mul_SSE(N-d, Z2f, Xf, Yf);
						if (!equal(N-d, Z1f, Z2f, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						mul_T(N-d, Z1f, Xf, Yf);
						mul(N-d, Z2f, Xf, Yf);
						if (!equal(N-d, Z1f, Z2f, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		os << "\n";
	} // mul


	{ // dot
		{ // Complex testing
			os << "\n\t[dot]Complex:    ";

			// SSE3
			if (Capability::SSE3)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cd[0] = dot_T(N-d, Xcd, Ycd);
						Z2cd[0] = dot_SSE3(N-d, Xcd, Ycd);
						if (!equal(1, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE3"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cd[0] = dot_T(N-d, Xcd, Ycd);
						Z2cd[0] = dot_SSE2(N-d, Xcd, Ycd);
						if (!equal(1, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cd[0] = dot_T(N-d, Xcd, Ycd);
						Z2cd[0] = dot(N-d, Xcd, Ycd);
						if (!equal(1, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // Complex' testing
			os << "\n\t[dot]Complex*:   ";

			// SSE3
			if (Capability::SSE3)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cd[0] = dot_T(N-d, Xcd, Yd);
						Z2cd[0] = dot_SSE3(N-d, Xcd, Yd);
						if (!equal(1, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE3"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cd[0] = dot_T(N-d, Xcd, Yd);
						Z2cd[0] = dot_SSE2(N-d, Xcd, Yd);
						if (!equal(1, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cd[0] = dot_T(N-d, Xcd, Yd);
						Z2cd[0] = dot(N-d, Xcd, Yd);
						if (!equal(1, Z1cd, Z2cd, DBL_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // ComplexF testing
			os << "\n\t[dot]ComplexF:   ";

			// SSE3
			if (Capability::SSE3)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cf[0] = dot_T(N-d, Xcf, Ycf);
						Z2cf[0] = dot_SSE3(N-d, Xcf, Ycf);
						if (!equal(1, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE3"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cf[0] = dot_T(N-d, Xcf, Ycf);
						Z2cf[0] = dot_SSE(N-d, Xcf, Ycf);
						if (!equal(1, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cf[0] = dot_T(N-d, Xcf, Ycf);
						Z2cf[0] = dot(N-d, Xcf, Ycf);
						if (!equal(1, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // ComplexF' testing
			os << "\n\t[dot]ComplexF*:  ";

			// SSE3
			if (Capability::SSE3)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cf[0] = dot_T(N-d, Xcf, Yf);
						Z2cf[0] = dot_SSE3(N-d, Xcf, Yf);
						if (!equal(1, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE3"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cf[0] = dot_T(N-d, Xcf, Yf);
						Z2cf[0] = dot_SSE(N-d, Xcf, Yf);
						if (!equal(1, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1cf[0] = dot_T(N-d, Xcf, Yf);
						Z2cf[0] = dot(N-d, Xcf, Yf);
						if (!equal(1, Z1cf, Z2cf, FLT_EPSILON))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // double testing
			os << "\n\t[dot]double:     ";
			const double EPS = DBL_EPSILON*1e4;

			// SSE2
			if (Capability::SSE2)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1d[0] = dot_T(N-d, Xd, Yd);
						Z2d[0] = dot_SSE2(N-d, Xd, Yd);
						if (!equal(1, Z1d, Z2d, EPS))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE2"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1d[0] = dot_T(N-d, Xd, Yd);
						Z2d[0] = dot(N-d, Xd, Yd);
						if (!equal(1, Z1d, Z2d, EPS))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		{ // float testing
			os << "\n\t[dot]float:      ";
			const float EPS = FLT_EPSILON*1e3f;

			// SSE
			if (Capability::SSE)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1f[0] = dot_T(N-d, Xf, Yf);
						Z2f[0] = dot_SSE(N-d, Xf, Yf);
						if (!equal(1, Z1f, Z2f, EPS))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "SSE"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}

			// auto
			if (1)
			{
				bool ok = true;

				__try
				{
					for (size_t d = 0; d < D; ++d)
					{
						Z1f[0] = dot_T(N-d, Xf, Yf);
						Z2f[0] = dot(N-d, Xf, Yf);
						if (!equal(1, Z1f, Z2f, EPS))
						{
							ok = false;
							break;
						}
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					ok = false;
				}

				os << "auto"
					<< (ok ? "+" : "-")
					<< "\t";
				ret &= ok;
			}
		}

		os << "\n";
	} // dot

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


// TODO: speed test
