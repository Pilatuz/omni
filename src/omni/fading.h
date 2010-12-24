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
	@brief Interface of the fading tools.

@author Andrey Savinkov
@author Sergey Polichnoy
@see @ref omni_calc
*/
#ifndef __OMNI_FADING_H_
#define __OMNI_FADING_H_

#include <omni/defs.hpp>

#include <complex>
#include <vector>

namespace omni
{
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The Rayleigh fading generator.
class RayleighFading {
public:
	typedef size_t size_type; ///< @brief The size type.
	typedef std::complex<double> Complex; ///< @brief The sample type.

public:

	/// @brief The fading type.
	enum FadingType {
		TYPE_JAKES,  ///< @brief The Jakes fading (default).
		TYPE_FLAT    ///< @brief The Flat fading.
	};

public:

	/// @brief The main constructor.
	explicit RayleighFading(double Fdop,
		FadingType type = TYPE_JAKES,
			size_type N = 1);

	/// @brief Generate new fading samples.
	void operator()(double t);

public:

	/// @brief Get fading sample.
	const Complex& operator[](size_type i) const
	{
		return m_samples[i];
	}

	/// @brief Get number of process.
	size_type size() const
	{
		return m_samples.size();
	}

private:

	/// @brief The one process.
	struct Process {
		Complex ampl;   ///< @brief The complex amplitude.
		double phase;   ///< @brief The initial phase.
		double freq;    ///< @brief The Doppler frequency.
	};

	std::vector<Process> m_process;
	std::vector<Complex> m_samples;
};

	} // dsp namespace
} // omni namespace

#endif // __OMNI_FADING_H_
