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
//		https://bitbucket.org/pilatuz/omni
//////////////////////////////////////////////////////////////////////////
/** @file
	@brief Fast Hadamard Transorm.
*/
#ifndef __OMNI_FHT_HPP_
#define __OMNI_FHT_HPP_

#include <omni/defs.hpp>
#include <omni/util.hpp>

#include <vector>

namespace omni
{
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Inverse Hadamard Transform.
/**
		This function performs the Inverse Fast Hadamard Transform of the vector @a x.

	The vector size should be integer power of two.

@param[in,out] x The vector.
*/
template<typename T, typename A>
void ifht(std::vector<T,A> &x)
{
	const size_t N = x.size();
	const size_t N_log2 = util::log2(N);

	size_t P = N/2;
	for (size_t i = 0; i < N_log2; ++i, P/=2)
		for (size_t j = 0; j < (1U<<i); ++j)
	{
		const size_t s1 = 2*j*P;
		const size_t s2 = P + s1;

		for (size_t k = 0; k < P; ++k)
		{
			x[k+s1] += x[k+s2];
			x[k+s2] = x[k+s1] - (x[k+s2] + x[k+s2]);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/// @brief Hadamard Transform.
/**
		This function performs the Fast Hadamard Transform of the vector @a x.

	The vector size should be integer power of two.

@param[in,out] x The vector.
*/
template<typename T, typename A>
void fht(std::vector<T,A> &x)
{
	ifht(x);

	// normalization
	const size_t N = x.size();
	const T nrm = T(1.0 / N);
	for (size_t i = 0; i < N; ++i)
		x[i] *= nrm;
}

	} // dsp namespace
} // omni namespace

#endif // __OMNI_FHT_HPP_
