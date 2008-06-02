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

namespace {

template<typename T>
struct BaseType {
	typedef typename T type;
};

template<typename T>
struct BaseType< std::complex<T> > {
	typedef typename T type;
};

template<typename T>
struct Traits;

template<>
struct Traits<double> {
	enum {
		is_SSE_type = 0,
		is_SSE2_type = 1
	};
};

template<>
struct Traits<float> {
	enum {
		is_SSE_type = 1,
		is_SSE2_type = 1
	};
};


template<typename T> inline
bool is_SSE_type()
{
	return Traits<BaseType<T>::type>::is_SSE_type;
}

template<typename T> inline
bool is_SSE2_type()
{
	return Traits<BaseType<T>::type>::is_SSE2_type;
}

} // local namespace


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


//////////////////////////////////////////////////////////////////////////
/// @brief Is SSE supported?
bool Capability::is_SSE()
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


//////////////////////////////////////////////////////////////////////////
/// @brief Is SSE2 supported?
bool Capability::is_SSE2()
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


//////////////////////////////////////////////////////////////////////////
/// @brief Is 3DNow! supported?
bool Capability::is_3DNow()
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


//// TODO: manual capability setup (at compile time)?
const bool Capability::MMX = Capability::is_MMX();
const bool Capability::_3DNow = Capability::is_3DNow();
const bool Capability::SSE = Capability::is_SSE();
const bool Capability::SSE2 = Capability::is_SSE2();

	} // Capability


	// add
	namespace SIMD
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Vector add helper.
template<typename T>
class Add {
private: // selector

	typedef void (*FuncPtr)(size_t, T*, const T*, const T*);

	static FuncPtr select()
	{
		if ((is_SSE_type<T>() && Capability::SSE)
			|| (is_SSE2_type<T>() && Capability::SSE2))
		{
				return &add_SSE;
		}
		else
			return &add_T<T>;
	}

public: // automatic selected
	static FuncPtr run;
};

// initialization
template<typename T>
typename Add<T>::FuncPtr Add<T>::run = Add<T>::select();

		} // details namespace


// Complex
void add(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	details::Add<Complex>::run(N, Z, X, Y);
}

// ComplexF
void add(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	details::Add<ComplexF>::run(N, Z, X, Y);
}

// double
void add(size_t N, double *Z, const double *X, const double *Y)
{
	details::Add<double>::run(N, Z, X, Y);
}

// float
void add(size_t N, float *Z, const float *X, const float *Y)
{
	details::Add<float>::run(N, Z, X, Y);
}


// Complex
void add_SSE(size_t N, Complex *Z, const Complex *X, const Complex *Y)
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
void add_SSE(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
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
void add_SSE(size_t N, double *Z, const double *X, const double *Y)
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
void add_SSE(size_t N, float *Z, const float *X, const float *Y)
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
