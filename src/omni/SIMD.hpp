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
#ifndef __OMNI_SIMD_H_
#define __OMNI_SIMD_H_

#include <omni/defs.hpp>
#include <complex>

namespace omni
{
	namespace SIMD
	{

	// float point
	typedef std::complex<double> Complex;
	typedef std::complex<float> ComplexF;

	// signed integers
	typedef signed __int8  Int8;
	typedef signed __int16 Int16;
	typedef signed __int32 Int32;
	typedef signed __int64 Int64;

	// unsigned integers
	typedef unsigned __int8  UInt8;
	typedef unsigned __int16 UInt16;
	typedef unsigned __int32 UInt32;
	typedef unsigned __int64 UInt64;


//////////////////////////////////////////////////////////////////////////
/// @brief The SIMD capability.
	class Capability {
	public:
		static const bool MMX;  ///< @brief Is MMX supported?
		static const bool SSE;  ///< @brief Is SSE supported?
		static const bool SSE2; ///< @brief Is SSE2 supported?
		static const bool SSE3; ///< @brief Is SSE3 supported?
		static const bool SSSE3; ///< @brief Is SSSE3 supported?
		static const bool SSE4_1; ///< @brief Is SSE4.1 supported?
		static const bool SSE4_2; ///< @brief Is SSE4.2 supported?
		static const bool _3DNow; ///< @brief Is 3DNow! supported?

	private:
		static bool is_MMX();
		static bool is_SSE();
		static bool is_SSE2();
		static bool is_SSE3();
		static bool is_SSSE3();
		static bool is_SSE4_1();
		static bool is_SSE4_2();
		static bool is_3DNow();
	};


//////////////////////////////////////////////////////////////////////////
/// @brief Vector add.
template<typename T>
void add_T(size_t N, T *Z, const T *X, const T *Y)
{
	for (size_t i = 0; i < N; ++i)
		Z[i] = X[i] + Y[i];
}

// general
template<typename T> inline
void add(size_t N, T *Z, const T *X, const T *Y)
{
	add_T(N, Z, X, Y);
}

// automatic
void add(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void add(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void add(size_t N, double *Z, const double *X, const double *Y);
void add(size_t N, float *Z, const float *X, const float *Y);

// SSE
void add_SSE2(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void add_SSE(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void add_SSE2(size_t N, double *Z, const double *X, const double *Y);
void add_SSE(size_t N, float *Z, const float *X, const float *Y);


//////////////////////////////////////////////////////////////////////////
/// @brief Vector sub.
template<typename T>
void sub_T(size_t N, T *Z, const T *X, const T *Y)
{
	for (size_t i = 0; i < N; ++i)
		Z[i] = X[i] - Y[i];
}

// general
template<typename T> inline
void sub(size_t N, T *Z, const T *X, const T *Y)
{
	sub_T(N, Z, X, Y);
}

// automatic
void sub(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void sub(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void sub(size_t N, double *Z, const double *X, const double *Y);
void sub(size_t N, float *Z, const float *X, const float *Y);

// SSE
void sub_SSE2(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void sub_SSE(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void sub_SSE2(size_t N, double *Z, const double *X, const double *Y);
void sub_SSE(size_t N, float *Z, const float *X, const float *Y);


//////////////////////////////////////////////////////////////////////////
/// @brief Vector mul.
template<typename T1, typename T2>
void mul_T(size_t N, T1 *Z, const T1 *X, const T2 *Y)
{
	for (size_t i = 0; i < N; ++i)
		Z[i] = X[i] * Y[i];
}

// general
template<typename T1, typename T2> inline
void mul(size_t N, T1 *Z, const T1 *X, const T2 *Y)
{
	mul_T(N, Z, X, Y);
}

// automatic
void mul(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void mul(size_t N, Complex *Z, const Complex *X, const double *Y);
void mul(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void mul(size_t N, ComplexF *Z, const ComplexF *X, const float *Y);
void mul(size_t N, double *Z, const double *X, const double *Y);
void mul(size_t N, float *Z, const float *X, const float *Y);

// SSE
void mul_SSE3(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void mul_SSE2(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void mul_SSE3(size_t N, Complex *Z, const Complex *X, const double *Y);
void mul_SSE2(size_t N, Complex *Z, const Complex *X, const double *Y);
void mul_SSE3(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void mul_SSE(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void mul_SSE3(size_t N, ComplexF *Z, const ComplexF *X, const float *Y);
void mul_SSE(size_t N, ComplexF *Z, const ComplexF *X, const float *Y);
void mul_SSE2(size_t N, double *Z, const double *X, const double *Y);
void mul_SSE(size_t N, float *Z, const float *X, const float *Y);


//////////////////////////////////////////////////////////////////////////
/// @brief Vector dot.
template<typename T1, typename T2>
T1 dot_T(size_t N, const T1 *X, const T2 *Y)
{
	T1 z = T1();

	for (size_t i = 0; i < N; ++i)
		z += X[i] * Y[i];

	return z;
}

// general
template<typename T1, typename T2> inline
T1 dot(size_t N, const T1 *X, const T2 *Y)
{
	return dot_T(N, Z, X, Y);
}

// automatic
Complex dot(size_t N, const Complex *X, const Complex *Y);
Complex dot(size_t N, const Complex *X, const double *Y);
ComplexF dot(size_t N, const ComplexF *X, const ComplexF *Y);
ComplexF dot(size_t N, const ComplexF *X, const float *Y);
double dot(size_t N, const double *X, const double *Y);
float dot(size_t N, const float *X, const float *Y);

// SSE
Complex dot_SSE3(size_t N, const Complex *X, const Complex *Y);
Complex dot_SSE2(size_t N, const Complex *X, const Complex *Y);
Complex dot_SSE3(size_t N, const Complex *X, const double *Y);
Complex dot_SSE2(size_t N, const Complex *X, const double *Y);
ComplexF dot_SSE3(size_t N, const ComplexF *X, const ComplexF *Y);
ComplexF dot_SSE(size_t N, const ComplexF *X, const ComplexF *Y);
ComplexF dot_SSE3(size_t N, const ComplexF *X, const float *Y);
ComplexF dot_SSE(size_t N, const ComplexF *X, const float *Y);
double dot_SSE2(size_t N, const double *X, const double *Y);
float dot_SSE(size_t N, const float *X, const float *Y);

	} // SIMD namespace
} // omni namespace

#endif // __OMNI_SIMD_H_
