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

	typedef std::complex<double> Complex;
	typedef std::complex<float> ComplexF;


//////////////////////////////////////////////////////////////////////////
/// @brief The SIMD capability.
	class Capability {
	public:
		static const bool MMX;  ///< @brief Is MMX supported?
		static const bool _3DNow; ///< @brief Is 3DNow! supported?
		static const bool SSE;  ///< @brief Is SSE supported?
		static const bool SSE2; ///< @brief Is SSE2 supported?

	private:
		static bool is_MMX();
		static bool is_SSE();
		static bool is_SSE2();
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
	add_T(N,Z,X,Y);
}

// automatic
void add(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void add(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void add(size_t N, double *Z, const double *X, const double *Y);
void add(size_t N, float *Z, const float *X, const float *Y);

// SSE specific
void add_SSE(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void add_SSE(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void add_SSE(size_t N, double *Z, const double *X, const double *Y);
void add_SSE(size_t N, float *Z, const float *X, const float *Y);


//////////////////////////////////////////////////////////////////////////
/// @brief Vector sub.
template<typename T>
void sub(T *Z, const T *X, const T *Y, size_t N)
{
	for (size_t i = 0; i < N; ++i)
		Z[i] = X[i] - Y[i];
}
void sub(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void sub(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void sub(size_t N, double *Z, const double *X, const double *Y);
void sub(size_t N, float *Z, const float *X, const float *Y);


//////////////////////////////////////////////////////////////////////////
/// @brief Vector mul.
template<typename T>
void mul(T *Z, const T *X, const T *Y, size_t N)
{
	for (size_t i = 0; i < N; ++i)
		Z[i] = X[i] * Y[i];
}
void mul(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void mul(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void mul(size_t N, double *Z, const double *X, const double *Y);
void mul(size_t N, float *Z, const float *X, const float *Y);


//////////////////////////////////////////////////////////////////////////
/// @brief Vector div.
template<typename T>
void div(T *Z, const T *X, const T *Y, size_t N)
{
	for (size_t i = 0; i < N; ++i)
		Z[i] = X[i] / Y[i];
}
void div(size_t N, Complex *Z, const Complex *X, const Complex *Y);
void div(size_t N, ComplexF *Z, const ComplexF *X, const ComplexF *Y);
void div(size_t N, double *Z, const double *X, const double *Y);
void div(size_t N, float *Z, const float *X, const float *Y);

	} // SIMD namespace
} // omni namespace

#endif // __OMNI_SIMD_H_
