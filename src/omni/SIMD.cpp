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
@author Sergey Polichnoy
*/
#include <omni/SIMD.hpp>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <intrin.h>

// capability detection
namespace
{

	// Capability detection
	class Capability {
	public:
		static const bool MMX;
		static const bool SSE;
		static const bool SSE2;

	public:

		/// @brief Is MMX supported?
		static bool is_MMX()
		{
			__try
			{
				__asm
				{
					pxor mm0,mm0
					emms
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}

			return true;
		}


		/// @brief Is 3DNow! supported?
		static bool is_3DNow()
		{
			__try
			{
				__asm
				{
					pfrcp mm0,mm0
					emms
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}

			return true;
		}



		/// @brief Is SSE supported?
		static bool is_SSE()
		{
			__try
			{
				__asm
				{
					xorps xmm0,xmm0
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}

			return true;
		}



		/// @brief Is SSE2 supported?
		static bool is_SSE2()
		{
			__try
			{
				__asm
				{
					xorpd xmm0,xmm0
				}
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}

			return true;
		}
	};

// TODO: manual capability setup (at compile time)?
const bool Capability::MMX = Capability::is_MMX();
const bool Capability::SSE = Capability::is_SSE();
const bool Capability::SSE2 = Capability::is_SSE2();

} // capability detection


namespace omni
{
	// add
	namespace SIMD
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Vector add helper.
class Add {
public: // x87
	// based on tadd() template function!

public: // SSE2

	// Complex
	static void SSE2_add_cd(size_t N, Complex *Z, const Complex *X, const Complex *Y)
	{
		assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
		assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
		assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

		for (size_t i = 0; i < N; ++i)
		{
			__m128d x = _mm_load_pd((const double*)X);
			__m128d y = _mm_load_pd((const double*)Y);
			__m128d z = _mm_add_pd(x,y);
			_mm_stream_pd((double*)Z,z);

			++X; ++Y; ++Z;
		}
	}

	// ComplexF
	static void SSE2_add_cf(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
	{
		assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
		assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
		assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

		// couples
		for (size_t i = 0; i < N/2; ++i)
		{
			__m128 x = _mm_load_ps((const float*)X);
			__m128 y = _mm_load_ps((const float*)Y);
			__m128 z = _mm_add_ps(x,y);
			_mm_stream_ps((float*)Z,z);

			X+=2; Y+=2; Z+=2;
		}

		if (N%2)
		{
			Z[0] = X[0] + Y[0];
			// ++X; ++Y; ++Z;
		}
	}

	// double
	static void SSE2_add_d(size_t N, double *Z, const double *X, const double *Y)
	{
		assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
		assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
		assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

		// couples
		for (size_t i = 0; i < N/2; ++i)
		{
			__m128d x = _mm_load_pd((const double*)X);
			__m128d y = _mm_load_pd((const double*)Y);
			__m128d z = _mm_add_pd(x,y);
			_mm_stream_pd((double*)Z,z);

			X+=2; Y+=2; Z+=2;
		}

		if (N%2)
		{
			Z[0] = X[0] + Y[0];
			// ++X; ++Y; ++Z;
		}
	}

	// float
	static void SSE2_add_f(size_t N, float *Z, const float *X, const float *Y)
	{
		assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
		assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
		assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

		// quartets
		for (size_t i = 0; i < N/4; ++i)
		{
			__m128 x = _mm_load_ps((const float*)X);
			__m128 y = _mm_load_ps((const float*)Y);
			__m128 z = _mm_add_ps(x,y);
			_mm_stream_ps((float*)Z,z);

			X+=4; Y+=4; Z+=4;
		}

		switch (N%4)
		{
			case 3: Z[0] = X[0] + Y[0]; ++X; ++Y; ++Z; // (!) no break
			case 2: Z[0] = X[0] + Y[0]; ++X; ++Y; ++Z; // (!) no break
			case 1: Z[0] = X[0] + Y[0]; ++X; ++Y; ++Z; // (!) no break
		}
	}

public: // typedefs

	template<typename T>
	struct Func {
		typedef void (*Ptr)(size_t, T*, const T*, const T*);
	};

public: // automatic selected
	static Func<Complex>::Ptr  add_cd;
	static Func<ComplexF>::Ptr add_cf;
	static Func<double>::Ptr   add_d;
	static Func<float>::Ptr    add_f;
};

// initialization
Add::Func<Complex>::Ptr  Add::add_cd = Capability::SSE2 ? &Add::SSE2_add_cd : &tadd<Complex>;
Add::Func<ComplexF>::Ptr Add::add_cf = Capability::SSE2 ? &Add::SSE2_add_cf : &tadd<ComplexF>;
Add::Func<double>::Ptr   Add::add_d  = Capability::SSE2 ? &Add::SSE2_add_d  : &tadd<double>;
Add::Func<float>::Ptr    Add::add_f  = Capability::SSE2 ? &Add::SSE2_add_f  : &tadd<float>;


// Complex
void add(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	Add::add_cd(N, Z, X, Y);
}

// ComplexF
void add(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	Add::add_cf(N, Z, X, Y);
}

// double
void add(size_t N, double *Z, const double *X, const double *Y)
{
	Add::add_d(N, Z, X, Y);
}

// float
void add(size_t N, float *Z, const float *X, const float *Y)
{
	Add::add_f(N, Z, X, Y);
}

	} // add


	// sub
	namespace SIMD
	{

void sub(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void sub(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void sub(size_t N, double *Z, const double *X, const double *Y);
void sub(size_t N, float *Z, const float *X, const float *Y);

	} // sub


	// mul
	namespace SIMD
	{

void mul(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void mul(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void mul(size_t N, double *Z, const double *X, const double *Y);
void mul(size_t N, float *Z, const float *X, const float *Y);

	} // mul


	// div
	namespace SIMD
	{

void div(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void div(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void div(size_t N, double *Z, const double *X, const double *Y);
void div(size_t N, float *Z, const float *X, const float *Y);

	} // div

} // omni namespace
