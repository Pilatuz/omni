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
	@brief Implementation of the fading tools.

@author Andrey Savinkov
@author Sergey Polichnoy
@see @ref omni_calc
*/
#include <omni/fading.h>

#include <omni/util.hpp>
#include <omni/rand.hpp>
#include <omni/FHT.hpp>

#include <assert.h>
#include <math.h>

namespace omni
{
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/**
		This constructor initializes the Rayleigh fading generator.

	The actual number of fading processes will be calculated automatically.

@param Fdop The Doppler frequency in Hertz.
@param type The fading type.
@param N Number of fading processes.
*/
RayleighFading::RayleighFading(double Fdop, FadingType type, size_type N)
{
	using util::PI;

	enum { MIN_SIN_COUNT = 16 };

	// calculate the number of processes
	N = util::clp2(N);
	if (N < MIN_SIN_COUNT)
		N = MIN_SIN_COUNT;

	m_process.resize(N);
	m_samples.resize(N);

	// calculate complex amplitudes and initial phases
	const double FREQ_DELTA = 1.0e-5;
	const double AMPL = sqrt(2.0*N);
	for (size_type i = 0; i < N; ++i)
	{
		Process &px = m_process[i];
		px.ampl = std::polar(AMPL, PI*(i+1.0)/N);
		px.phase = rnd::runif(0.0, 2.0*PI);

		// calculate the fading frequencies
		switch (type)
		{
			case TYPE_JAKES:
				px.freq = 2*PI * Fdop
					* cos(0.5*PI * (i+0.5)/N)
					* (1.0 + rnd::runif(-FREQ_DELTA, +FREQ_DELTA));
				break;

			case TYPE_FLAT:
				px.freq = 2*PI * Fdop
					* ((i+0.5)/N)
					* (1.0 + rnd::runif(-FREQ_DELTA, +FREQ_DELTA));
				break;

			default:
				assert(!"invalid fading type");
				break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/**
	Generate the new fading samples.

@param t The time.
*/
void RayleighFading::operator()(double t)
{
	const size_type N = size();
	for (size_type i = 0; i < N; ++i)
	{
		const Process &px = m_process[i];

		m_samples[i] = px.ampl * sin(px.freq*t + px.phase);
	}

	dsp::fht(m_samples);
}

	} // dsp namespace
} // omni namespace
