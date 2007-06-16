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
	@brief Implementation of the modem tools.

@author Sergey Polichnoy
*/
#include <omni/modem.h>

#include <cmath>

namespace omni
{
	// ModulationMap
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/**
		This static method creates phase-shift keying (PSK) constellation.
	For example: BPSK, QPSK, 8-PSK, etc. There is a Gray code used.

@code
	ModulationMap mmap = ModulationMap::PSK(8); // 8-PSK
@endcode

@param map_size Modulation map size, should be integer power of two.
@return PSK modulation map.
*/
ModulationMap ModulationMap::PSK(size_type map_size)
{
	assert(2<=map_size && util::is_ipow2(map_size)
		&& "PSK map size should be integer power of 2");

	std::vector<symbol_type> mod_map(map_size);
	if (2 < map_size)
	{
		for (codeword_type cw = 0; cw < map_size; ++cw)
		{
			mod_map[cw ^ (cw>>1)] = std::polar(1.0,
				2.0*util::PI * (cw+0.5)/map_size);
		}
	}
	else // BPSK (real values)
	{
		mod_map[0] = +1.0;
		mod_map[1] = -1.0;
	}

	return ModulationMap(mod_map);
}


//////////////////////////////////////////////////////////////////////////
/**
		This static method creates a quadrature amplitude modulation (QAM)
	constellation. For example: 16-QAM, 64-QAM. There is a Gray code used.

@code
	ModulationMap mmap = ModulationMap::QAM(64); // 64-QAM
@endcode

@param map_size Modulation map size, should be integer power of two.
@return QAM modulation map.
*/
ModulationMap ModulationMap::QAM(size_type map_size)
{
	assert(2<=map_size && util::is_ipow2(map_size)
		&& "QAM map size should be integer power of 2");
	const size_type BPS = util::log2(map_size);

	// number of bits per RE and IM
	const size_type re_bps = BPS / 2;
	const size_type im_bps = BPS - re_bps;

	// RE and IM maximum
	const codeword_type RE_MAX = (1U<<re_bps) - 1;
	const codeword_type IM_MAX = (1U<<im_bps) - 1;

	std::vector<symbol_type> mod_map(map_size);
	for (codeword_type re = 0; re <= RE_MAX; ++re)
		for (codeword_type im = 0; im <= IM_MAX; ++im)
	{
		const codeword_type re_gray = re ^ (re>>1);
		const codeword_type im_gray = im ^ (im>>1);
		const codeword_type cw = (re<<im_bps) | im;

		mod_map[cw] = symbol_type(
			RE_MAX - 2.0*re_gray,
			IM_MAX - 2.0*im_gray);
	}

	return ModulationMap(mod_map);
}


//////////////////////////////////////////////////////////////////////////
/**
		This method performs modulation map normalization.
	So each modulation symbol has unit average power.
*/
void ModulationMap::normalize()
{
	const size_type N = size();

	double norm_sum = 0.0;
	for (size_type i = 0; i < N; ++i)
		norm_sum += std::norm(m_map[i]);

	assert(0.0 < norm_sum
		&& "invalid map");

	norm_sum = sqrt(N / norm_sum);
	for (size_type i = 0; i < N; ++i)
		m_map[i] *= norm_sum;
}

	} // ModulationMap
} // omni namespace


//////////////////////////////////////////////////////////////////////////
/** @page omni_modem Digital modulation.

		There are several classes that is used for digital modulation:
	omni::dsp::ModulationMap, omni::dsp::Modem, omni::dsp::Modulator and
	omni::dsp::Demodulator. All of these classes are in <omni/modem.h>
	header file.

@section omni_modem_start Getting started

		For quick using a modem tools you should perform the following steps:

@code
	#include <omni/modem.h>
	using namespace omni::dsp;

	int main()
	{
		// Create modulation map.
		ModulationMap modMap = ModulationMap::PSK(4); // QPSK

		// Create modulator and demodulator.
		Modulator mod(modMap);
		Demodulator dem(modMap);

		// Prepare source data for modulation.
		std::vector<int> data(100);
		for (size_t i = 0; i < data.size(); ++i)
			data[i] = ::rand() % 2;

		// Perform modulation.
		mod(data);

		// Simulate propagation channel
		std::vector< std::complex<double> > rx(mod.size());
		for (size_t i = 0; i < mod.size(); ++i)
			rx[i] = mod[i] + noise();

		// Perform demodulation
		dem(rx);

		// Calculate errors
		size_t N_err = 0;
		for (size_t i = 0; i < data.size(); ++i)
			if (data[i] != dem.soft2hard(dem[i]))
				N_err += 1;

		std::cout << "No errors: "
			<< N_err << "\n";
		return 0;
	}
@endcode


TODO: Modem using with channel coefficients


@section omni_modem_map Popular modulation maps

@subsection omni_modem_BPSK BPSK

	<center>
	<table border="1" cellspacing="1" cellpadding="3">
		<caption>BPSK modulation map</caption>
		<tr align="center"> <th>codeword</th> <th>modulation symbol</th> </tr>
		<tr align="center"> <td> 0b </td> <td> (+1,0) </td> </tr>
		<tr align="center"> <td> 1b </td> <td> (-1,0) </td> </tr>
	</table>
	</center>

@subsection omni_modem_QPSK QPSK

	<center>
	<table border="1" cellspacing="1" cellpadding="3">
		<caption>QPSK modulation map</caption>
		<tr align="center"> <th>codeword</th> <th>modulation symbol</th> </tr>
		<tr align="center"> <td> 00b </td> <td> (+1,+1) </td> </tr>
		<tr align="center"> <td> 01b </td> <td> (-1,+1) </td> </tr>
		<tr align="center"> <td> 10b </td> <td> (+1,-1) </td> </tr>
		<tr align="center"> <td> 11b </td> <td> (-1,-1) </td> </tr>
	</table>
	</center>

@subsection omni_modem_16QAM 16QAM

	<center>
	<table border="1" cellspacing="1" cellpadding="3">
		<caption>QPSK modulation map</caption>
		<tr align="center"> <th>codeword</th> <th>modulation symbol</th> </tr>
		<tr align="center"> <td> 0000b </td> <td> (+3,+3) </td> </tr>
		<tr align="center"> <td> 0001b </td> <td> (+3,+1) </td> </tr>
		<tr align="center"> <td> 0010b </td> <td> (+3,-3) </td> </tr>
		<tr align="center"> <td> 0011b </td> <td> (+3,-1) </td> </tr>
		<tr align="center"> <td> 0100b </td> <td> (+1,+3) </td> </tr>
		<tr align="center"> <td> 0101b </td> <td> (+1,+1) </td> </tr>
		<tr align="center"> <td> 0110b </td> <td> (+1,-3) </td> </tr>
		<tr align="center"> <td> 0111b </td> <td> (+1,-1) </td> </tr>
		<tr align="center"> <td> 1000b </td> <td> (-3,+3) </td> </tr>
		<tr align="center"> <td> 1001b </td> <td> (-3,+1) </td> </tr>
		<tr align="center"> <td> 1010b </td> <td> (-3,-3) </td> </tr>
		<tr align="center"> <td> 1011b </td> <td> (-3,-1) </td> </tr>
		<tr align="center"> <td> 1100b </td> <td> (-1,+3) </td> </tr>
		<tr align="center"> <td> 1101b </td> <td> (-1,+1) </td> </tr>
		<tr align="center"> <td> 1110b </td> <td> (-1,-3) </td> </tr>
		<tr align="center"> <td> 1111b </td> <td> (-1,-1) </td> </tr>
	</table>
	</center>
*/
