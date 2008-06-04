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

// base type
template<typename T>
struct BaseType {
	typedef typename T type;
};

// complex's base type
template<typename T>
struct BaseType< std::complex<T> > {
	typedef typename T type;
};


// type traits
template<typename T>
struct Traits;

// float
template<>
struct Traits<float> {
	enum {
		is_SSE_type = 1,
		is_SSE2_type = 1
	};
};

// double
template<>
struct Traits<double> {
	enum {
		is_SSE_type = 0,
		is_SSE2_type = 1
	};
};


// is SSE type?
template<typename T> inline
bool is_SSE_type()
{
	return Traits<BaseType<T>::type>::is_SSE_type;
}

// is SSE2 type?
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

	/// @brief Function pointer.
	typedef void (*FuncPtr)(size_t, T*, const T*, const T*);

	/// @brief Select the function.
	static FuncPtr select()
	{
		if ((is_SSE_type<T>() && Capability::SSE)
			|| (is_SSE2_type<T>() && Capability::SSE2))
		{
			return &add_SSE;
		}

		return &add_T<T>;
	}

public:

	/// @brief The engine.
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


// Complex (SSE2)
void add_SSE(size_t N, Complex *Z, const Complex *X, const Complex *Y)
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

	// couples
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);
		__m128 z = _mm_add_ps(x, y);
		_mm_store_ps((float*)Z, z);

		X+=2; Y+=2; Z+=2;
	}

	if (N%2)
	{
		Z[0] = X[0] + Y[0];
		// ++X; ++Y; ++Z;
	}
}


// double (SSE2)
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
		__m128d z = _mm_add_pd(x, y);
		_mm_store_pd((double*)Z, z);

		X+=2; Y+=2; Z+=2;
	}

	if (N%2)
	{
		Z[0] = X[0] + Y[0];
		// ++X; ++Y; ++Z;
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
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);
		__m128 z = _mm_add_ps(x, y);
		_mm_store_ps((float*)Z, z);

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
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Vector sub helper.
template<typename T>
class Sub {
private: // selector

	/// @brief Function pointer.
	typedef void (*FuncPtr)(size_t, T*, const T*, const T*);

	/// @brief Select the function.
	static FuncPtr select()
	{
		if ((is_SSE_type<T>() && Capability::SSE)
			|| (is_SSE2_type<T>() && Capability::SSE2))
		{
			return &sub_SSE;
		}

		return &sub_T<T>;
	}

public:

	/// @brief The engine.
	static FuncPtr run;
};

// initialization
template<typename T>
typename Sub<T>::FuncPtr Sub<T>::run = Sub<T>::select();

		} // details namespace


// Complex
void sub(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	details::Sub<Complex>::run(N, Z, X, Y);
}

// ComplexF
void sub(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	details::Sub<ComplexF>::run(N, Z, X, Y);
}

// double
void sub(size_t N, double *Z, const double *X, const double *Y)
{
	details::Sub<double>::run(N, Z, X, Y);
}

// float
void sub(size_t N, float *Z, const float *X, const float *Y)
{
	details::Sub<float>::run(N, Z, X, Y);
}


// Complex (SSE2)
void sub_SSE(size_t N, Complex *Z, const Complex *X, const Complex *Y)
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

	// couples
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);
		__m128 z = _mm_sub_ps(x, y);
		_mm_store_ps((float*)Z, z);

		X+=2; Y+=2; Z+=2;
	}

	if (N%2)
	{
		Z[0] = X[0] - Y[0];
		// ++X; ++Y; ++Z;
	}
}


// double (SSE2)
void sub_SSE(size_t N, double *Z, const double *X, const double *Y)
{
	assert(!(size_t(X)%16) && "vector X must be 16-byte aligned");
	assert(!(size_t(Y)%16) && "vector Y must be 16-byte aligned");
	assert(!(size_t(Z)%16) && "vector Z must be 16-byte aligned");

	// couples
	for (size_t i = 0; i < N/2; ++i)
	{
		__m128d x = _mm_load_pd((const double*)X);
		__m128d y = _mm_load_pd((const double*)Y);
		__m128d z = _mm_sub_pd(x, y);
		_mm_store_pd((double*)Z, z);

		X+=2; Y+=2; Z+=2;
	}

	if (N%2)
	{
		Z[0] = X[0] - Y[0];
		// ++X; ++Y; ++Z;
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
		__m128 x = _mm_load_ps((const float*)X);
		__m128 y = _mm_load_ps((const float*)Y);
		__m128 z = _mm_sub_ps(x, y);
		_mm_store_ps((float*)Z, z);

		X+=4; Y+=4; Z+=4;
	}

	switch (N%4)
	{
		case 3: Z[0] = X[0] - Y[0]; ++X; ++Y; ++Z; // (!) no break
		case 2: Z[0] = X[0] - Y[0]; ++X; ++Y; ++Z; // (!) no break
		case 1: Z[0] = X[0] - Y[0]; ++X; ++Y; ++Z; // (!) no break
	}
}

	} // sub


	// mul
	namespace SIMD
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Vector mul helper.
template<typename T1, typename T2>
class Mul {
private: // selector

	/// @brief Function pointer.
	typedef void (*FuncPtr)(size_t, T1*, const T1*, const T2*);

	/// @brief Select the function.
	static FuncPtr select()
	{
		if ((is_SSE_type<T1>() && Capability::SSE)
			|| (is_SSE2_type<T1>() && Capability::SSE2))
		{
			return &mul_SSE;
		}

		return &mul_T<T1,T2>;
	}

public:

	/// @brief The engine.
	static FuncPtr run;
};

// initialization
template<typename T1, typename T2>
typename Mul<T1,T2>::FuncPtr Mul<T1,T2>::run = Mul<T1,T2>::select();

		} // details namespace


// Complex
void mul(size_t N, Complex *Z, const Complex *X, const Complex *Y)
{
	details::Mul<Complex,Complex>::run(N, Z, X, Y);
}

// Complex*double
void mul(size_t N, Complex *Z, const Complex *X, const double *Y)
{
	details::Mul<Complex,double>::run(N, Z, X, Y);
}

// ComplexF
void mul(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y)
{
	details::Mul<ComplexF,ComplexF>::run(N, Z, X, Y);
}

// ComplexF*float
void mul(size_t N, ComplexF *Z, const ComplexF *X, const float *Y)
{
	details::Mul<ComplexF,float>::run(N, Z, X, Y);
}

// double
void mul(size_t N, double *Z, const double *X, const double *Y)
{
	details::Mul<double,double>::run(N, Z, X, Y);
}

// float
void mul(size_t N, float *Z, const float *X, const float *Y)
{
	details::Mul<float,float>::run(N, Z, X, Y);
}


// Complex (SSE2)
void mul_SSE(size_t N, Complex *Z, const Complex *X, const Complex *Y)
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


// Complex*double (SSE2)
void mul_SSE(size_t N, Complex *Z, const Complex *X, const double *Y)
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


// ComplexF*float (SSE)
void mul_SSE(size_t N, ComplexF *Z, const ComplexF *X, const float *Y)
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


// double (SSE2)
void mul_SSE(size_t N, double *Z, const double *X, const double *Y)
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

	if (N%2)
	{
		Z[0] = X[0] * Y[0];
		// ++X; ++Y; ++Z;
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
		case 3: Z[0] = X[0] * Y[0]; ++X; ++Y; ++Z; // (!) no break
		case 2: Z[0] = X[0] * Y[0]; ++X; ++Y; ++Z; // (!) no break
		case 1: Z[0] = X[0] * Y[0]; ++X; ++Y; ++Z; // (!) no break
	}
}

	} // mul


	// dot
	namespace SIMD
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Vector dot helper.
template<typename T1, typename T2>
class Dot {
private: // selector

	/// @brief Function pointer.
	typedef T1 (*FuncPtr)(size_t, const T1*, const T2*);

	/// @brief Select the function.
	static FuncPtr select()
	{
		if ((is_SSE_type<T1>() && Capability::SSE)
			|| (is_SSE2_type<T1>() && Capability::SSE2))
		{
			return &dot_SSE;
		}

		return &dot_T<T1,T2>;
	}

public:

	/// @brief The engine.
	static FuncPtr run;
};

// initialization
template<typename T1, typename T2>
typename Dot<T1,T2>::FuncPtr Dot<T1,T2>::run = Dot<T1,T2>::select();

		} // details namespace


// Complex
Complex dot(size_t N, const Complex *X, const Complex *Y)
{
	return details::Dot<Complex,Complex>::run(N, X, Y);
}

// Complex*double
Complex dot(size_t N, const Complex *X, const double *Y)
{
	return details::Dot<Complex,double>::run(N, X, Y);
}

// ComplexF
ComplexF dot(size_t N, const ComplexF *X, const ComplexF *Y)
{
	return details::Dot<ComplexF,ComplexF>::run(N, X, Y);
}

// ComplexF*float
ComplexF dot(size_t N, const ComplexF *X, const float *Y)
{
	return details::Dot<ComplexF,float>::run(N, X, Y);
}

// double
double dot(size_t N, const double *X, const double *Y)
{
	return details::Dot<double,double>::run(N, X, Y);
}

// float
float dot(size_t N, const float *X, const float *Y)
{
	return details::Dot<float,float>::run(N, X, Y);
}


// Complex (SSE2)
Complex dot_SSE(size_t N, const Complex *X, const Complex *Y)
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


// Complex*double (SSE2)
Complex dot_SSE(size_t N, const Complex *X, const double *Y)
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
		__m128 t1 = _mm_mul_ps(_mm_moveldup_ps(x), y);
		__m128 t2 = _mm_mul_ps(_mm_movehdup_ps(x), y);
		__m128 t3 = _mm_addsub_ps(t1, _mm_shuffle_ps(t2, t2, 0xB1));
		z = _mm_add_ps(z, t3);

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
double dot_SSE(size_t N, const double *X, const double *Y)
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
