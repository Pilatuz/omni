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
	@brief Discrete Fourie Transform
	@author Sergey Polichnoy
*/
#ifndef __OMNI_DFT_H_
#define __OMNI_DFT_H_

#include <omni/defs.hpp>

#include <complex>
#include <vector>

#include <assert.h>

namespace omni
{
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
// DFT class
template<typename T>
class DFT {
	typedef DFT<T> this_type;

public:
	typedef std::complex<T> value_type;
	typedef size_t size_type;
	typedef T scalar_type;

public:
	explicit DFT(size_type DFT_size);
	DFT(size_type DFT_size,
		scalar_type fwd_scale,
		scalar_type inv_scale);
	~DFT();

	this_type& operator=(const this_type &x);
	DFT(const this_type &x);

public:
	// forward transform
	template<typename A>
	void forward(std::vector<value_type, A> &data)
	{
		assert(data.size()==size()
			&& "invalid input data size");

		forward(&data[0]);
	}
	void forward(value_type *data);

	// inverse transform
	template<typename A>
	void inverse(std::vector<value_type, A> &data)
	{
		assert(data.size()==size()
			&& "invalid input data size");

		inverse(&data[0]);
	}
	void inverse(value_type *data);

public:
	// DFT size
	size_type size() const
	{
		return m_size;
	}

	// forward transform scale
	scalar_type forward_scale() const
	{
		return m_fwd_scale;
	}

	// inverse transform scale
	scalar_type inverse_scale() const
	{
		return m_inv_scale;
	}

private:
	void swap(this_type &x);
	void init();

private:
	scalar_type m_fwd_scale;
	scalar_type m_inv_scale;

	size_type m_size;
	size_type m_log2;
	void *m_impl;
};


//////////////////////////////////////////////////////////////////////////
// auxiliary functions
template<typename T, typename A> inline
void fft(std::vector<std::complex<T>, A> &x)
{
	DFT<T> ft(x.size());
	ft.forward(&x[0]);
}

template<typename T, typename A> inline
void ifft(std::vector<std::complex<T>, A> &x)
{
	DFT<T> ft(x.size());
	ft.inverse(&x[0]);
}

template<typename T, typename A>
void fft_shift(std::vector<T, A> &x)
{
	const size_t N_div2 = x.size()/2;
	for (size_t i = 0; i < N_div2; ++i)
		std::swap(x[i], x[i+N_div2]);
}

	} // dsp namespace
} // omni namespace

#endif // __OMNI_DFT_H_
