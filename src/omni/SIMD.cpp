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

namespace omni
{
	// Capability
	namespace SIMD
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Is MMX supported?
bool Capability::is_MMX()
{
	__try
	{
		{ // check feature
			int info[4] = {0};

			// TODO: check processor?
			__cpuid(info, 1);
			if (~info[3]&(1<<23)) // EDX, bit 23
				return false;
		}

		// check OS support
		__asm
		{
			PXOR mm0,mm0
			EMMS
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is SSE supported?
bool Capability::is_SSE()
{
	__try
	{
		{ // check feature
			int info[4] = {0};

			// TODO: check processor?
			__cpuid(info, 1);
			if (~info[3]&(1<<25)) // EDX, bit 25
				return false;
		}

		// check OS support
		__asm
		{
			XORPS xmm0,xmm0
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is SSE2 supported?
bool Capability::is_SSE2()
{
	__try
	{
		{ // check feature
			int info[4] = {0};

			// TODO: check processor?
			__cpuid(info, 1);
			if (~info[3]&(1<<26)) // EDX, bit 26
				return false;
		}

		// check OS support
		__asm
		{
			XORPD xmm0,xmm0
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is SSE3 supported?
bool Capability::is_SSE3()
{
	__try
	{
		{ // check feature
			int info[4] = {0};

			// TODO: check processor?
			__cpuid(info, 1);
			if (~info[2]&(1<<0)) // ECX, bit 0
				return false;
		}

		// check OS support
		//__asm
		//{
		//	XORPD xmm0,xmm0
		//}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is SSSE3 supported?
bool Capability::is_SSSE3()
{
	__try
	{
		{ // check feature
			int info[4] = {0};

			// TODO: check processor?
			__cpuid(info, 1);
			if (~info[2]&(1<<9)) // ECX, bit 9
				return false;
		}

		// check OS support
		//__asm
		//{
		//	XORPD xmm0,xmm0
		//}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is SSE4.1 supported?
bool Capability::is_SSE4_1()
{
	__try
	{
		{ // check feature
			int info[4] = {0};

			// TODO: check processor?
			__cpuid(info, 1);
			if (~info[2]&(1<<19)) // ECX, bit 19
				return false;
		}

		// check OS support
		//__asm
		//{
		//	XORPD xmm0,xmm0
		//}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/// @brief Is SSE4.2 supported?
bool Capability::is_SSE4_2()
{
	__try
	{
		{ // check feature
			int info[4] = {0};

			// TODO: check processor?
			__cpuid(info, 1);
			if (~info[2]&(1<<20)) // ECX, bit 20
				return false;
		}

		// check OS support
		//__asm
		//{
		//	XORPD xmm0,xmm0
		//}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is 3DNow! supported?
bool Capability::is_3DNow()
{
	__try
	{
		// TODO: check processor?

		// check OS support
		__asm
		{
			PFRCP mm0,mm0
			EMMS
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}


//// TODO: manual capability setup (at compile time)?
const bool Capability::MMX = Capability::is_MMX();
const bool Capability::SSE = Capability::is_SSE();
const bool Capability::SSE2 = Capability::is_SSE2();
const bool Capability::SSE3 = Capability::is_SSE3();
const bool Capability::SSSE3 = Capability::is_SSSE3();
const bool Capability::SSE4_1 = Capability::is_SSE4_1();
const bool Capability::SSE4_2 = Capability::is_SSE4_2();
const bool Capability::_3DNow = Capability::is_3DNow();

	} // Capability


	// add
	namespace SIMD
	{

// Complex
void add(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, Complex*, const Complex*, const Complex*);

		static FuncPtr select()
		{
			if (Capability::SSE2)
				return &omni::SIMD::add_SSE2;

			return &omni::SIMD::add_T<Complex>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// ComplexF
void add(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, ComplexF*, const ComplexF*, const ComplexF*);

		static FuncPtr select()
		{
			if (Capability::SSE)
				return &omni::SIMD::add_SSE;

			return &omni::SIMD::add_T<ComplexF>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// double
void add(size_t N, double *Z, const double *X, const double *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, double*, const double*, const double*);

		static FuncPtr select()
		{
			if (Capability::SSE2)
				return &omni::SIMD::add_SSE2;

			return &omni::SIMD::add_T<double>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// float
void add(size_t N, float *Z, const float *X, const float *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, float*, const float*, const float*);

		static FuncPtr select()
		{
			if (Capability::SSE)
				return &omni::SIMD::add_SSE;

			return &omni::SIMD::add_T<float>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}


// Complex (SSE2)
void add_SSE2(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_load_pd((const double*)Y);
		__m128d z = _mm_add_pd(x, y);
		_mm_store_pd((double*)Z, z);

		++X; ++Y; ++Z;
	}
}


// ComplexF (SSE)
void add_SSE(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// couples of ComplexF
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);
		__m128 z = _mm_add_ps(x, y);
		_mm_store_ps((float*)Z, z);

		X+=2; Y+=2; Z+=2;
	}

	// remain
	if (N%2)
	{
		// TODO: complex scalar add
		Z[0] = X[0] + Y[0];
		// ++X; ++Y; ++Z;
	}
}


// double (SSE2)
void add_SSE2(size_t N, double *Z, const double *X, const double *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// couples
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128d x = _mm_load_pd(X);
		__m128d y = _mm_load_pd(Y);
		__m128d z = _mm_add_pd(x, y);
		_mm_store_pd(Z, z);

		X+=2; Y+=2; Z+=2;
	}

	// remain
	if (N%2)
	{
		__m128d x = _mm_load_sd(X);
		__m128d y = _mm_load_sd(Y);
		__m128d z = _mm_add_sd(x, y);
		_mm_store_sd(Z, z);

		//X+=1; Y+=1; Z+=1;
	}
}


// float (SSE)
void add_SSE(size_t N, float *Z, const float *X, const float *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// quartets
	for (size_t i = 0; i < N/4; ++i)
	{
		__m128 x = _mm_load_ps(X);
		__m128 y = _mm_load_ps(Y);
		__m128 z = _mm_add_ps(x, y);
		_mm_store_ps(Z, z);

		X+=4; Y+=4; Z+=4;
	}

	// remain
	switch (N%4)
	{
		case 3:
		{
			__m128 x = _mm_load_ss(X);
			__m128 y = _mm_load_ss(Y);
			__m128 z = _mm_add_ss(x, y);
			_mm_store_ss(Z, z);

			X+=1; Y+=1; Z+=1;
		} // (!) no break;

		case 2:
		{
			__m128 x = _mm_load_ss(X);
			__m128 y = _mm_load_ss(Y);
			__m128 z = _mm_add_ss(x, y);
			_mm_store_ss(Z, z);

			X+=1; Y+=1; Z+=1;
		} // (!) no break;

		case 1:
		{
			__m128 x = _mm_load_ss(X);
			__m128 y = _mm_load_ss(Y);
			__m128 z = _mm_add_ss(x, y);
			_mm_store_ss(Z, z);

			X+=1; Y+=1; Z+=1;
		} // (!) no break;
	}
}

	} // add


	// sub
	namespace SIMD
	{

// Complex
void sub(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, Complex*, const Complex*, const Complex*);

		static FuncPtr select()
		{
			if (Capability::SSE2)
				return &omni::SIMD::sub_SSE2;

			return &omni::SIMD::sub_T<Complex>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// ComplexF
void sub(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, ComplexF*, const ComplexF*, const ComplexF*);

		static FuncPtr select()
		{
			if (Capability::SSE)
				return &omni::SIMD::sub_SSE;

			return &omni::SIMD::sub_T<ComplexF>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// double
void sub(size_t N, double *Z, const double *X, const double *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, double*, const double*, const double*);

		static FuncPtr select()
		{
			if (Capability::SSE2)
				return &omni::SIMD::sub_SSE2;

			return &omni::SIMD::sub_T<double>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// float
void sub(size_t N, float *Z, const float *X, const float *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, float*, const float*, const float*);

		static FuncPtr select()
		{
			if (Capability::SSE)
				return &omni::SIMD::sub_SSE;

			return &omni::SIMD::sub_T<float>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}


// Complex (SSE2)
void sub_SSE2(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_load_pd((const double*)Y);
		__m128d z = _mm_sub_pd(x, y);
		_mm_store_pd((double*)Z, z);

		++X; ++Y; ++Z;
	}
}


// ComplexF (SSE)
void sub_SSE(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// couples of ComplexF
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);
		__m128 z = _mm_sub_ps(x, y);
		_mm_store_ps((float*)Z, z);

		X+=2; Y+=2; Z+=2;
	}

	// remain
	if (N%2)
	{
		// TODO: complex scalar sub
		Z[0] = X[0] - Y[0];
		// ++X; ++Y; ++Z;
	}
}


// double (SSE2)
void sub_SSE2(size_t N, double *Z, const double *X, const double *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// couples
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128d x = _mm_load_pd(X);
		__m128d y = _mm_load_pd(Y);
		__m128d z = _mm_sub_pd(x, y);
		_mm_store_pd(Z, z);

		X+=2; Y+=2; Z+=2;
	}

	// remain
	if (N%2)
	{
		__m128d x = _mm_load_sd(X);
		__m128d y = _mm_load_sd(Y);
		__m128d z = _mm_sub_sd(x, y);
		_mm_store_sd(Z, z);

		//X+=1; Y+=1; Z+=1;
	}
}


// float (SSE)
void sub_SSE(size_t N, float *Z, const float *X, const float *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// quartets
	for (size_t i = 0; i < N/4; ++i)
	{
		__m128 x = _mm_load_ps(X);
		__m128 y = _mm_load_ps(Y);
		__m128 z = _mm_sub_ps(x, y);
		_mm_store_ps(Z, z);

		X+=4; Y+=4; Z+=4;
	}

	// remain
	switch (N%4)
	{
		case 3:
		{
			__m128 x = _mm_load_ss(X);
			__m128 y = _mm_load_ss(Y);
			__m128 z = _mm_sub_ss(x, y);
			_mm_store_ss(Z, z);

			X+=1; Y+=1; Z+=1;
		} // (!) no break;

		case 2:
		{
			__m128 x = _mm_load_ss(X);
			__m128 y = _mm_load_ss(Y);
			__m128 z = _mm_sub_ss(x, y);
			_mm_store_ss(Z, z);

			X+=1; Y+=1; Z+=1;
		} // (!) no break;

		case 1:
		{
			__m128 x = _mm_load_ss(X);
			__m128 y = _mm_load_ss(Y);
			__m128 z = _mm_sub_ss(x, y);
			_mm_store_ss(Z, z);

			X+=1; Y+=1; Z+=1;
		} // (!) no break;
	}
}

	} // sub


	// mul
	namespace SIMD
	{

// Complex*Complex
void mul(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, Complex*, const Complex*, const Complex*);

		static FuncPtr select()
		{
			if (Capability::SSE3)
				return &omni::SIMD::mul_SSE3;
			if (Capability::SSE2)
				return &omni::SIMD::mul_SSE2;

			return &omni::SIMD::mul_T<Complex, Complex>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// Complex*double
void mul(size_t N, Complex *Z, const Complex *X, const double *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, Complex*, const Complex*, const double*);

		static FuncPtr select()
		{
			if (Capability::SSE3)
				return &omni::SIMD::mul_SSE3;
			if (Capability::SSE2)
				return &omni::SIMD::mul_SSE2;

			return &omni::SIMD::mul_T<Complex, double>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// ComplexF*ComplexF
void mul(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, ComplexF*, const ComplexF*, const ComplexF*);

		static FuncPtr select()
		{
			if (Capability::SSE3)
				return &omni::SIMD::mul_SSE3;
			if (Capability::SSE)
				return &omni::SIMD::mul_SSE;

			return &omni::SIMD::mul_T<ComplexF, ComplexF>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// ComplexF*float
void mul(size_t N, ComplexF *Z, const ComplexF *X, const float *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, ComplexF*, const ComplexF*, const float*);

		static FuncPtr select()
		{
			if (Capability::SSE3)
				return &omni::SIMD::mul_SSE3;
			if (Capability::SSE)
				return &omni::SIMD::mul_SSE;

			return &omni::SIMD::mul_T<ComplexF, float>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// double*double
void mul(size_t N, double *Z, const double *X, const double *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, double*, const double*, const double*);

		static FuncPtr select()
		{
			if (Capability::SSE2)
				return &omni::SIMD::mul_SSE2;

			return &omni::SIMD::mul_T<double, double>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}

// float*float
void mul(size_t N, float *Z, const float *X, const float *Y)
{
	// auxiliary
	struct Aux {
		typedef void(*FuncPtr)(size_t, float*, const float*, const float*);

		static FuncPtr select()
		{
			if (Capability::SSE)
				return &omni::SIMD::mul_SSE;

			return &omni::SIMD::mul_T<float, float>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	run(N, Z, X, Y);
}


// Complex*Complex (SSE3)
void mul_SSE3(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_load_pd((const double*)Y);
		__m128d t1 = _mm_mul_pd(x, _mm_unpacklo_pd(y, y));
		__m128d t2 = _mm_mul_pd(x, _mm_unpackhi_pd(y, y));
		__m128d z = _mm_addsub_pd(t1, _mm_shuffle_pd(t2, t2, 1));
		_mm_store_pd((double*)Z, z);

		++X; ++Y; ++Z;
	}
}


// Complex*Complex (SSE2)
void mul_SSE2(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_load_pd((const double*)Y);
		__m128d t1 = _mm_mul_pd(x, _mm_unpacklo_pd(y, y));
		__m128d t2 = _mm_mul_pd(x, _mm_unpackhi_pd(y, y));
		__m128d t3 = _mm_shuffle_pd(t2, t2, 1);
		__m128d z1 = _mm_add_pd(t1, t3);
		__m128d z2 = _mm_sub_pd(t1, t3);
		__m128d z = _mm_move_sd(z1, z2);
		_mm_store_pd((double*)Z, z);

		++X; ++Y; ++Z;
	}
}


// Complex*double (SSE3)
void mul_SSE3(size_t N, Complex *Z, const Complex *X, const double *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_loaddup_pd(Y);
		__m128d z = _mm_mul_pd(x, y);
		_mm_store_pd((double*)Z, z);

		++X; ++Y; ++Z;
	}
}


// Complex*double (SSE2)
void mul_SSE2(size_t N, Complex *Z, const Complex *X, const double *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_load_sd(Y); y = _mm_unpacklo_pd(y, y);
		__m128d z = _mm_mul_pd(x, y);
		_mm_store_pd((double*)Z, z);

		++X; ++Y; ++Z;
	}
}


// ComplexF (SSE3)
void mul_SSE3(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// couples
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);
		__m128 t1 = _mm_mul_ps(_mm_moveldup_ps(x), y);
		__m128 t2 = _mm_mul_ps(_mm_movehdup_ps(x), y);
		__m128 z = _mm_addsub_ps(t1, _mm_shuffle_ps(t2, t2, 0xB1));
		_mm_store_ps((float*)Z, z);

		X+=2; Y+=2; Z+=2;
	}

	if (N%2)
	{
		Z[0] = X[0] * Y[0];
		// ++X; ++Y; ++Z;
	}
}


// ComplexF (SSE)
void mul_SSE(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// couples
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);

		__m128 t1 = _mm_mul_ps(_mm_shuffle_ps(x,x,0xA0), y);
		__m128 t2 = _mm_mul_ps(_mm_shuffle_ps(x,x,0xF5), y);
		__m128 t3 = _mm_shuffle_ps(t2, t2, 0xB1);

		__m128 z1 = _mm_add_ps(t1, t3);
		__m128 z2 = _mm_sub_ps(t1, t3);
		__m128 z = _mm_shuffle_ps(z1,z2,0x8D);

		z = _mm_shuffle_ps(z,z,0x72);
		_mm_store_ps((float*)Z, z);

		X+=2; Y+=2; Z+=2;
	}

	if (N%2)
	{
		Z[0] = X[0] * Y[0];
		// ++X; ++Y; ++Z;
	}
}


// ComplexF*float (SSE3)
void mul_SSE3(size_t N, ComplexF *Z, const ComplexF *X, const float *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// couples
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y1 = _mm_load1_ps(Y + 0);
		__m128 y2 = _mm_load1_ps(Y + 1);
		__m128 y = _mm_movelh_ps(y1,y2);
		__m128 z = _mm_mul_ps(x, y);
		_mm_store_ps((float*)Z, z);

		X+=2; Y+=2; Z+=2;
	}

	if (N%2)
	{
		Z[0] = X[0] * Y[0];
		// ++X; ++Y; ++Z;
	}
}


// ComplexF*float (SSE)
void mul_SSE(size_t N, ComplexF *Z, const ComplexF *X, const float *Y)
{
	mul_SSE3(N, Z, X, Y);
}


// double (SSE2)
void mul_SSE2(size_t N, double *Z, const double *X, const double *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// couples
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128d x = _mm_load_pd(X);
		__m128d y = _mm_load_pd(Y);
		__m128d z = _mm_mul_pd(x, y);
		_mm_store_pd(Z, z);

		X+=2; Y+=2; Z+=2;
	}

	// remain
	if (N%2)
	{
		__m128d x = _mm_load_sd(X);
		__m128d y = _mm_load_sd(Y);
		__m128d z = _mm_mul_sd(x, y);
		_mm_store_sd(Z, z);

		//X+=1; Y+=1; Z+=1;
	}
}


// float (SSE)
void mul_SSE(size_t N, float *Z, const float *X, const float *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// quartets
	for (size_t i = 0; i < N/4; ++i)
	{
		__m128 x = _mm_load_ps(X);
		__m128 y = _mm_load_ps(Y);
		__m128 z = _mm_mul_ps(x, y);
		_mm_store_ps(Z, z);

		X+=4; Y+=4; Z+=4;
	}

	switch (N%4)
	{
		case 3:
		{
			__m128 x = _mm_load_ss(X);
			__m128 y = _mm_load_ss(Y);
			__m128 z = _mm_mul_ss(x, y);
			_mm_store_ss(Z, z);

			X+=1; Y+=1; Z+=1;
		} // (!) no break;

		case 2:
		{
			__m128 x = _mm_load_ss(X);
			__m128 y = _mm_load_ss(Y);
			__m128 z = _mm_mul_ss(x, y);
			_mm_store_ss(Z, z);

			X+=1; Y+=1; Z+=1;
		} // (!) no break;

		case 1:
		{
			__m128 x = _mm_load_ss(X);
			__m128 y = _mm_load_ss(Y);
			__m128 z = _mm_mul_ss(x, y);
			_mm_store_ss(Z, z);

			X+=1; Y+=1; Z+=1;
		} // (!) no break;
	}
}

	} // mul


	// dot
	namespace SIMD
	{

// Complex
Complex dot(size_t N, const Complex *X, const Complex *Y)
{
	// auxiliary
	struct Aux {
		typedef Complex (*FuncPtr)(size_t, const Complex*, const Complex*);

		static FuncPtr select()
		{
			if (Capability::SSE3)
				return &omni::SIMD::dot_SSE3;
			if (Capability::SSE2)
				return &omni::SIMD::dot_SSE2;

			return &omni::SIMD::dot_T<Complex, Complex>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	return run(N, X, Y);
}

// Complex*double
Complex dot(size_t N, const Complex *X, const double *Y)
{
	// auxiliary
	struct Aux {
		typedef Complex (*FuncPtr)(size_t, const Complex*, const double*);

		static FuncPtr select()
		{
			if (Capability::SSE3)
				return &omni::SIMD::dot_SSE3;
			if (Capability::SSE2)
				return &omni::SIMD::dot_SSE2;

			return &omni::SIMD::dot_T<Complex, double>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	return run(N, X, Y);
}

// ComplexF
ComplexF dot(size_t N, const ComplexF *X, const ComplexF *Y)
{
	// auxiliary
	struct Aux {
		typedef ComplexF (*FuncPtr)(size_t, const ComplexF*, const ComplexF*);

		static FuncPtr select()
		{
			if (Capability::SSE3)
				return &omni::SIMD::dot_SSE3;
			if (Capability::SSE)
				return &omni::SIMD::dot_SSE;

			return &omni::SIMD::dot_T<ComplexF, ComplexF>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	return run(N, X, Y);
}

// ComplexF*float
ComplexF dot(size_t N, const ComplexF *X, const float *Y)
{
	// auxiliary
	struct Aux {
		typedef ComplexF (*FuncPtr)(size_t, const ComplexF*, const float*);

		static FuncPtr select()
		{
			if (Capability::SSE3)
				return &omni::SIMD::dot_SSE3;
			if (Capability::SSE)
				return &omni::SIMD::dot_SSE;

			return &omni::SIMD::dot_T<ComplexF, float>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	return run(N, X, Y);
}

// double
double dot(size_t N, const double *X, const double *Y)
{
	// auxiliary
	struct Aux {
		typedef double (*FuncPtr)(size_t, const double*, const double*);

		static FuncPtr select()
		{
			if (Capability::SSE2)
				return &omni::SIMD::dot_SSE2;

			return &omni::SIMD::dot_T<double, double>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	return run(N, X, Y);
}

// float
float dot(size_t N, const float *X, const float *Y)
{
	// auxiliary
	struct Aux {
		typedef float (*FuncPtr)(size_t, const float*, const float*);

		static FuncPtr select()
		{
			if (Capability::SSE)
				return &omni::SIMD::dot_SSE;

			return &omni::SIMD::dot_T<float, float>;
		}
	};

	static Aux::FuncPtr run = Aux::select();

	return run(N, X, Y);
}


// Complex (SSE3)
Complex dot_SSE3(size_t N, const Complex *X, const Complex *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	__m128d z = _mm_setzero_pd();
	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_load_pd((const double*)Y);
		__m128d t1 = _mm_mul_pd(x, _mm_unpacklo_pd(y, y));
		__m128d t2 = _mm_mul_pd(x, _mm_unpackhi_pd(y, y));
		__m128d t3 = _mm_addsub_pd(t1, _mm_shuffle_pd(t2, t2, 1));
		z = _mm_add_pd(z, t3);

		++X; ++Y;
	}

	__declspec(align(16)) Complex Z[1];
	_mm_store_pd((double*)Z, z);

	return Z[0];
}


// Complex (SSE2)
Complex dot_SSE2(size_t N, const Complex *X, const Complex *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	__m128d z = _mm_setzero_pd();
	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_load_pd((const double*)Y);
		__m128d t1 = _mm_mul_pd(x, _mm_unpacklo_pd(y, y));
		__m128d t2 = _mm_mul_pd(x, _mm_unpackhi_pd(y, y));
		__m128d t3 = _mm_shuffle_pd(t2, t2, 1);
		__m128d z1 = _mm_add_pd(t1, t3);
		__m128d z2 = _mm_sub_pd(t1, t3);
		__m128d z3 = _mm_move_sd(z1, z2);
		z = _mm_add_pd(z, z3);

		++X; ++Y;
	}

	__declspec(align(16)) Complex Z[1];
	_mm_store_pd((double*)Z, z);

	return Z[0];
}


// Complex*double (SSE3)
Complex dot_SSE3(size_t N, const Complex *X, const double *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	__m128d z = _mm_setzero_pd();
	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_loaddup_pd(Y);
		__m128d t = _mm_mul_pd(x, y);
		z = _mm_add_pd(z, t);

		++X; ++Y;
	}

	__declspec(align(16)) Complex Z[1];
	_mm_store_pd((double*)Z, z);

	return Z[0];
}


// Complex*double (SSE2)
Complex dot_SSE2(size_t N, const Complex *X, const double *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	__m128d z = _mm_setzero_pd();
	for (size_t i = 0; i < N; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_load_sd(Y); y = _mm_unpacklo_pd(y, y);
		__m128d t = _mm_mul_pd(x, y);
		z = _mm_add_pd(z, t);

		++X; ++Y;
	}

	__declspec(align(16)) Complex Z[1];
	_mm_store_pd((double*)Z, z);

	return Z[0];
}


// ComplexF (SSE3)
ComplexF dot_SSE3(size_t N, const ComplexF *X, const ComplexF *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	// couples
	__m128 z = _mm_setzero_ps();
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);
		__m128 t1 = _mm_mul_ps(_mm_moveldup_ps(x), y);
		__m128 t2 = _mm_mul_ps(_mm_movehdup_ps(x), y);
		__m128 t = _mm_addsub_ps(t1, _mm_shuffle_ps(t2, t2, 0xB1));
		z = _mm_add_ps(z, t);

		X+=2; Y+=2;
	}

	__declspec(align(16)) ComplexF Z[2];
	_mm_store_ps((float*)Z, z);
	Z[0] += Z[1];

	if (N%2)
	{
		Z[0] += X[0] * Y[0];
		// ++X; ++Y;
	}

	return Z[0];
}


// ComplexF (SSE)
ComplexF dot_SSE(size_t N, const ComplexF *X, const ComplexF *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	// couples
	__m128 z = _mm_setzero_ps();
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);

		__m128 t1 = _mm_mul_ps(_mm_shuffle_ps(x,x,0xA0), y);
		__m128 t2 = _mm_mul_ps(_mm_shuffle_ps(x,x,0xF5), y);
		__m128 t3 = _mm_shuffle_ps(t2, t2, 0xB1);

		__m128 z1 = _mm_add_ps(t1, t3);
		__m128 z2 = _mm_sub_ps(t1, t3);
		__m128 t = _mm_shuffle_ps(z1,z2,0x8D);

		t = _mm_shuffle_ps(t,t,0x72);
		z = _mm_add_ps(z, t);

		X+=2; Y+=2;
	}

	__declspec(align(16)) ComplexF Z[2];
	_mm_store_ps((float*)Z, z);
	Z[0] += Z[1];

	if (N%2)
	{
		Z[0] += X[0] * Y[0];
		// ++X; ++Y;
	}

	return Z[0];
}


// ComplexF*float (SSE3)
ComplexF dot_SSE3(size_t N, const ComplexF *X, const float *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	// couples
	__m128 z = _mm_setzero_ps();
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y1 = _mm_load1_ps(Y + 0);
		__m128 y2 = _mm_load1_ps(Y + 1);
		__m128 y = _mm_movelh_ps(y1,y2);
		__m128 t = _mm_mul_ps(x, y);
		z = _mm_add_ps(z, t);

		X+=2; Y+=2;
	}

	__declspec(align(16)) ComplexF Z[2];
	_mm_store_ps((float*)Z, z);
	Z[0] += Z[1];

	if (N%2)
	{
		Z[0] += X[0] * Y[0];
		// ++X; ++Y; ++Z;
	}

	return Z[0];
}


// ComplexF*float (SSE)
ComplexF dot_SSE(size_t N, const ComplexF *X, const float *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	// couples
	__m128 z = _mm_setzero_ps();
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y1 = _mm_load1_ps(Y + 0);
		__m128 y2 = _mm_load1_ps(Y + 1);
		__m128 y = _mm_movelh_ps(y1,y2);
		__m128 t = _mm_mul_ps(x, y);
		z = _mm_add_ps(z, t);

		X+=2; Y+=2;
	}

	__declspec(align(16)) ComplexF Z[2];
	_mm_store_ps((float*)Z, z);
	Z[0] += Z[1];

	if (N%2)
	{
		Z[0] += X[0] * Y[0];
		// ++X; ++Y; ++Z;
	}

	return Z[0];
}

// double (SSE2)
double dot_SSE2(size_t N, const double *X, const double *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	// couples
	__m128d z = _mm_setzero_pd();
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128d x = _mm_load_pd(X);
		__m128d y = _mm_load_pd(Y);
		__m128d t = _mm_mul_pd(x, y);
		z = _mm_add_pd(z, t);

		X+=2; Y+=2;
	}

	__declspec(align(16)) double Z[2];
	_mm_store_pd(Z, z);
	Z[0] += Z[1];

	if (N%2)
	{
		Z[0] += X[0] * Y[0];
		// ++X; ++Y; ++Z;
	}

	return Z[0];
}


// float (SSE)
float dot_SSE(size_t N, const float *X, const float *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");

	// quartets
	__m128 z = _mm_setzero_ps();
	for (size_t i = 0; i < N/4; ++i)
	{
		__m128 x = _mm_load_ps(X);
		__m128 y = _mm_load_ps(Y);
		__m128 t = _mm_mul_ps(x, y);
		z = _mm_add_ps(z, t);

		X+=4; Y+=4;
	}

	__declspec(align(16)) float Z[4];
	_mm_store_ps(Z, z);
	Z[0] += Z[1] + Z[2] + Z[3];

	switch (N%4)
	{
		case 3: Z[0] += X[0] * Y[0]; ++X; ++Y; // (!) no break
		case 2: Z[0] += X[0] * Y[0]; ++X; ++Y; // (!) no break
		case 1: Z[0] += X[0] * Y[0]; ++X; ++Y; // (!) no break
	}

	return Z[0];
}

	} // dot

} // omni namespace
