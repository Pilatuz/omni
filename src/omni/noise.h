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
	@brief Implementation of the omni::dsp::GaussNoiseT class.

@author Sergey Polichnoy
*/
#ifndef __OMNI_NOISE_H_
#define __OMNI_NOISE_H_

#include <omni/defs.hpp>
#include <complex>

namespace omni
{
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
// GaussNoiseT template class
template<typename T>
class GaussNoiseT {
public:
	typedef std::complex<T> sample_type;

public:
	GaussNoiseT();
	explicit GaussNoiseT(const T &noise_stdev);

public:
	sample_type operator()() const;
	const T& stdev() const;

public:
	static sample_type get_sample(const T &noise_stdev);

private:
	T m_stdev;
};


// GaussNoise typedef
typedef GaussNoiseT<double> GaussNoise;

	} // dsp namespace
} // omni namespace

#endif // __OMNI_NOISE_H_
