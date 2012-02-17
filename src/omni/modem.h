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
	@brief Interface of the modem tools.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_MODEM_H_
#define __OMNI_MODEM_H_

#include <omni/util.hpp>

#include <iterator>
#include <cassert>
#include <complex>
#include <limits>
#include <vector>

namespace omni
{
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The modulation map (constellation) class.
/**
		Modulation map (or constellation) contains a set of the modulation
	symbols. Each modulation symbol is a complex value that corresponds
	to the certain codeword. One codeword contains bps() significant bits.

		ModulationMap class is used to map the codewords
	to the corresponding modulation symbols. It is automatically normalized, 
	i.e. each modulation symbol in the map has unit average power.

		PSK() and QAM() static methods create a new modulation map
	(phase-shift keying (PSK) and quadrature amplitude modulation (QAM)
	corresponding).

@code
	ModulationMap mmap;              // BPSK
	mmap = ModulationMap::PSK(4);    // QPSK
	mmap = ModulationMap::QAM(16);   // 16-QAM
@endcode
*/
class ModulationMap {
public:
	typedef std::complex<double> symbol_type; ///< @brief The modulation symbol type.
	typedef size_t codeword_type;             ///< @brief The codeword type.
	typedef size_t size_type;                 ///< @brief The size type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Create modulation map from the custom sequence.
/**
		This constructor creates modulation map from the custom
	sequence [@a first, @a last).

		Sequence size should be integer power of two!

@param first Begin of the sequence.
@param last End of the sequence.
*/
	template<typename In>
	ModulationMap(In first, In last)
	{
		init(first, last);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Create modulation map from the vector.
/**
		This constructor creates modulation map from the vector @a x.

	Vector size should be integer power of two!

@param x The vector of modulation symbols.
*/
	template<typename TX, typename AX>
		explicit ModulationMap(const std::vector<TX, AX> &x)
	{
		init(x.begin(), x.end());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Create default modulation map.
/**
		This constructor creates default modulation map - BPSK.
*/
	ModulationMap()
		: m_map(2),
		  m_bps(1)
	{
		m_map[0] = +1.0;
		m_map[1] = -1.0;
	}


//////////////////////////////////////////////////////////////////////////
/// @name Properties
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get modulation symbol.
/**
		The method returns corresponding modulation symbol
	for the given codeword @a codeword.

@param codeword The codeword.
@return The modulation symbol.
*/
	const symbol_type& operator[](codeword_type codeword) const
	{
		assert(codeword < size()
			&& "codeword out of range");
		return m_map[codeword];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get modulation map size.
/**
		The method returns modulation map size,
	i.e. the number of points in the constellation.

@return The modulation map size.
*/
	size_type size() const
	{
		return m_map.size();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get codeword size.
/**
		This method returns codeword size,
	i.e. the number of bits per modulation symbol.

@return Codeword size.
*/
	size_type bps() const
	{
		return m_bps;
	}


//////////////////////////////////////////////////////////////////////////
/// @name Predefined
public:

	/// @brief Create PSK modulation map.
	static ModulationMap PSK(size_type map_size);

	/// @brief Create QAM modulation map.
	static ModulationMap QAM(size_type map_size);


//////////////////////////////////////////////////////////////////////////
/// @name Auxiliary
public:

	/// @brief Swap the two modulation maps.
	void swap(ModulationMap &other);


private:

//////////////////////////////////////////////////////////////////////////
/// @brief Modulation map initialization.
/**
		This method initializes the modulation map:
			- calculate codeword size;
			- copy custom sequence [@a first, @a last) to the internal array;
			- perform modulation map normalization

@param first Begin of the sequence.
@param last End of the sequence.
*/
	template<typename In>
	void init(In first, In last)
	{
		const size_type N = std::distance(first, last);
		assert(2<=N && "modulation map size should be >= 2");

		m_bps = util::log2(util::clp2(N));

		m_map.resize(N);
		std::copy(first, last,
			m_map.begin());

		normalize();
	}

	/// @brief Normalize the map by the power.
	void normalize();

private:
	std::vector<symbol_type> m_map;  // modulation map - array of modulation symbols
	size_type m_bps;                 // number of bits per modulation symbol
};



//////////////////////////////////////////////////////////////////////////
/// @brief The basic modem class.
/**
		The Modem class is a basic modulation and demodulation tool.
	It provides static methods only for modulation and demodulation.

		The derived Modulator and Demodulator classes extend functionality
	of this class and provide internal buffers, where results of modulation
	or demodulation operations are stored.
*/
class Modem {
public:
	typedef ModulationMap::symbol_type symbol_type;     ///< @brief Modulation symbol type.
	typedef ModulationMap::codeword_type codeword_type; ///< @brief Codeword type.
	typedef ModulationMap::size_type size_type;         ///< @brief Size type.
	typedef double LLR_type;                            ///< @brief Log-Likelihood Ratio type.


//////////////////////////////////////////////////////////////////////////
/// @name Modulation
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Modulate bit sequence.
/**
		This static method modulates input bit sequence [@a ifirst, @a ilast).

		Input bit sequence is converted into MSB-first order codewords.

		Input bit sequence size should be multiple of codeword size!

		The output buffer volume should be sufficient to store a
	@a std::distance(@a ifirst, @a ilast) / @a mmap.bps() modulation symbols.

@param mmap Modulation map.
@param ifirst Begin of the input bit sequence.
@param ilast End of the input bit sequence.
@param ofirst Begin of the output modulation symbols sequence.
@return End of the output modulation symbols sequence.
*/
	template<typename In, typename Out>
		static Out modulate_bits(const ModulationMap &mmap,
			In ifirst, In ilast, Out ofirst)
	{
		const size_type i_size = std::distance(ifirst, ilast);
		const size_type BPS = mmap.bps();

		assert((i_size%BPS) == 0
			&& "input size should be multiple of BPS");
		const size_type o_size = i_size / BPS;

		for (size_type i = 0; i < o_size; ++i, ++ofirst)
		{
			codeword_type cw = util::bi2de_msb(
				ifirst, BPS, codeword_type(0));             // (!) MSB-first
			*ofirst = mmap[cw];

			std::advance(ifirst, BPS);
		}

		return ofirst;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Modulate codeword sequence.
/**
		This static method modulates the codeword sequence [@a ifirst, @a ilast).
	One modulation symbol corresponds to the one input codeword.

		The output buffer volume should be sufficient to store
	a @a std::distance(@a ifirst, @a ilast) modulation symbols.

@param mmap Modulation map.
@param ifirst Begin of the input codeword sequence.
@param ilast End of the input codeword sequence.
@param ofirst Begin of the output modulation symbols sequence.
@return End of the output modulation symbols sequence.
*/
	template<typename In, typename Out>
		static Out modulate_codewords(const ModulationMap &mmap,
			In ifirst, In ilast, Out ofirst)
	{
		for (; ifirst != ilast; ++ifirst, ++ofirst)
		{
			codeword_type cw = *ifirst;
			*ofirst = mmap[cw];
		}

		return ofirst;
	}


//////////////////////////////////////////////////////////////////////////
/// @name Demodulation
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Demodulation of the one received modulation symbol.
/**
		This static method calculates log-likelihood ratio (LLR)
	for each bit in the codeword, corresponding to the received
	modulation symbol @a symbol.

		The meaning of the @a symbol and @a LLR_scale arguments
	is shown in the following example:

@code
	t = ...;     // TX modulation symbol

	h = noise(); // channel coefficient (FLAT fading)
	n = noise(); // additive white Gaussian noise
	r = h*t + n; // propagation channel

	LLR_scale = norm(h);
	symbol = r * conj(h) / norm(h);
	...
@endcode

		The output buffer volume should be sufficient to store a @a mmap.bps() LLR values.

@param mmap Modulation map.
@param symbol Received modulation symbol.
@param LLR_scale LLR scale.
@param ofirst Begin of the output LLR sequence.
@return End of the output LLR sequence.
*/
	template<typename T, typename U, typename Out>
		static Out demodulate_1(const ModulationMap &mmap,
			const T &symbol, const U &LLR_scale, Out ofirst)
	{
		// modulation map properties
		const size_type MM_size = mmap.size();
		const size_type BPS = mmap.bps();

		enum { MAX_BPS = 16 }; // (!)
		assert(BPS <= MAX_BPS
			&& "BPS too big");

		LLR_type min_0[MAX_BPS];
		LLR_type min_1[MAX_BPS];

		{ // initialize by INF
			typedef std::numeric_limits<LLR_type> LLR_limits;
			for (size_type b = 0; b < BPS; ++b)
			{
				min_0[b] = LLR_limits::infinity();
				min_1[b] = LLR_limits::infinity();
			}
		}

		// find minimum (0,1) for each bit
		for (size_type m = 0; m < MM_size; ++m)
		{
			LLR_type d = std::norm(mmap[m] - symbol);

			for (size_type b = 0; b < BPS; ++b)
			{
				if (m & (1U<<(BPS-b-1))) // (!) MSB first
				{
					if (d < min_1[b])
						min_1[b] = d;
				}
				else
				{
					if (d < min_0[b])
						min_0[b] = d;
				}
			}
		}

		// LLRs of all bits of one modulation symbol
		for (size_type b = 0; b < BPS; ++b, ++ofirst)
			*ofirst = (min_1[b] - min_0[b]) * LLR_scale;

		return ofirst;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Demodulation of the received symbols sequence with custom channel.
/**
		This static method calculates LLRs for the
	sequence of received modulation symbols [@a ifirst, @a ilast).

		For each received symbol the custom channel coefficient is used.
	The custom channel coefficients sequence size should be equal to the
	received symbols sequence size.

		Calculated LLRs are stored in the @a ofirst.
	The output buffer volume should be sufficient to store a
	@a std::distance(@a ifirst, @a ilast) * mmap.bps() LLR values.

@param mmap Modulation map.
@param ifirst Begin of the received symbols sequence.
@param ilast End of the received symbols sequence.
@param channel Begin of the custom channel coefficients sequence.
@param ofirst Begin of the output LLR sequence.
@return End of the output LLR sequence.
*/
	template<typename In, typename In2, typename Out>
		static Out demodulate(const ModulationMap &mmap,
			In ifirst, In ilast, In2 channel, Out ofirst)
	{
		for (; ifirst != ilast; ++ifirst, ++channel)
		{
			const double LLR_scale = std::norm(*channel);
			ofirst = demodulate_1(mmap,
				(*ifirst) * std::conj(*channel)/LLR_scale,
					LLR_scale, ofirst);
		}

		return ofirst;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Demodulation of the received modulation symbols sequence.
/**
	This static method calculates LLRs for the received modulation symbols 
sequence [@a ifirst, @a ilast).

	This static method is meaningful for AWGN channels only.
The unit channel coefficient is used for each received modulation symbol.

		The calculated LLRs are stored in the @a ofirst.
	The output buffer volume should be sufficient to store a
	@a std::distance(@a ifirst, @a ilast) * mmap.bps() LLR values.

@param mmap Modulation map.
@param ifirst Begin of the received symbols sequence.
@param ilast End of the received symbols sequence.
@param ofirst Begin of the output LLRs sequence.
@return End of the output LLRs sequence.
*/
	template<typename In, typename Out>
		static Out demodulate(const ModulationMap &mmap,
			In ifirst, In ilast, Out ofirst)
	{
		for (; ifirst != ilast; ++ifirst)
		{
			ofirst = demodulate_1(mmap,
				*ifirst, 1.0, ofirst);
		}

		return ofirst;
	}


//////////////////////////////////////////////////////////////////////////
/// @name Auxiliary
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Convert LLR value to the hard decision.
/**
		This static method converts the floating point LLR value
	to the bit hard decision using the following rule:

		- if (LLR >= 0) return 0;
		- if (LLR <  0) return 1;

@param LLR The LLR value.
@return The hard decision.
*/
	static codeword_type soft2hard(LLR_type LLR)
	{
		return LLR < 0.0;
	}
};


//////////////////////////////////////////////////////////////////////////
/// @brief The modulation tool.
/**
		This class extends the Modem modulation functionality. In addition
	this class has a internal buffer out(), where the results of the last
	modulation operation are stored.

		Example:

@code
		Modulator mod(ModulationMap::PSK(4)); // QPSK
		std::vector<int> data;
		// ....

		mod(data); // modulation
		mod.out(); // modulation result

		// ...
@endcode
*/
class Modulator: private Modem {
	typedef Modem inherited;

public:
	typedef inherited::symbol_type symbol_type;     ///< @brief Modulation symbol type.
	typedef std::vector<symbol_type> output_type;   ///< @brief Array of modulation symbols.
	typedef inherited::codeword_type codeword_type; ///< @brief Codeword type.
	typedef inherited::size_type size_type;         ///< @brief Size type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Create the modulator.
/**
		This constructor creates Modulator object
	with the modulation map @a mmap.

@param mmap Modulation map.
*/
	explicit Modulator(const ModulationMap &mmap)
		: m_map(mmap)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the modulation map.
/**
		This method changes the current modulation map to the @a mmap.

@param mmap New modulation map.
*/
	void setMap(const ModulationMap &mmap)
	{
		m_map = mmap;
	}


//////////////////////////////////////////////////////////////////////////
/// @name Modulation
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Modulate the bit sequence.
/**
		This method modulates the bit sequence [@a first, @a last).

		The input bit sequence size should be multiple of the codeword size!

		The result of modulation operation is stored in
	the internal buffer and available through out() method.

@param first Begin of the input bit sequence.
@param last End of the input bit sequence.

@see Modem::modulate_bits()
*/
	template<typename In>
		void modulate_bits(In first, In last)
	{
		const size_type i_size = std::distance(first, last);
		const size_type BPS = map().bps();

		assert((i_size%BPS) == 0
			&& "input size should be multiple of BPS");

		// prepare output buffer
		m_out.resize(i_size / BPS);

		inherited::modulate_bits(map(),
			first, last, m_out.begin());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Modulate the codeword sequence.
/**
		This method modulates the codeword sequence [@a first, @a last).

		The result of modulation operation is stored in
	the internal buffer and available through out() method.

@param first Begin of the input codeword sequence.
@param last End of the input codeword sequence.

@see Modem::modulate_codewords()
*/
	template<typename In>
		void modulate_codewords(In first, In last)
	{
		// prepare output buffer
		m_out.resize(std::distance(first, last));

		inherited::modulate_codewords(map(),
			first, last, m_out.begin());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Modulate the bit vector.
/**
		This method modulates the input bit vector @a x
	and it is similar to the modulate_bits() method.

		The input bit vector size should be multiple of the codeword size!

		The result of modulation operation is stored in
	the internal buffer and available through out() method.

@param x The input bit vector.
@return The modulation result.
*/
	template<typename TX, typename AX>
		const output_type& operator()(const std::vector<TX, AX> &x)
	{
		modulate_bits(x.begin(), x.end());
		return out();
	}


//////////////////////////////////////////////////////////////////////////
/// @name Properties
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get the modulation map.
/**
@return The modulation map.
*/
	const ModulationMap& map() const
	{
		return m_map;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the modulated data.
/**
		This method returns the modulated data
	after the last modulation operation.

@return Modulated data.
*/
	const output_type& out() const
	{
		return m_out;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the k-th modulated symbol.
/**
		This method returns k-th modulated symbol
	after the last modulation operation.

@param k Modulated symbol index, should be in range [0, size()).
@return Modulated symbol.
*/
	const symbol_type& operator[](size_type k) const
	{
		assert(k < size()
			&& "index out of range");
		return m_out[k];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get modulated data array size.
/**
		This method returns the modulated data size
	after the last modulation operation.

@return Modulated data size.
*/
	size_type size() const
	{
		return m_out.size();
	}

private:
	ModulationMap m_map; // modulation map
	output_type m_out;   // modulated data
};


//////////////////////////////////////////////////////////////////////////
/// @brief The demodulation tool.
/**
		This class extends the Modem demodulation functionality. In addition
	this class has a internal buffer out(), where the results of the last
	demodulation operation are stored.

		Example:

@code
		Modulator mod(ModulationMap::PSK(4)); // QPSK
		Demodulator dem(mod.map());           // same as in modulator
		std::vector<int> data;
		// ....

		mod(data); // modulation
		mod.out(); // modulation result

		dem(mod.out()); // demodulation
		dem.out();      // demodulation result (LLRs)
@endcode
*/
class Demodulator: private Modem {
	typedef Modem inherited;

public:
	typedef inherited::symbol_type symbol_type;     ///< @brief Modulation symbol type.
	typedef inherited::codeword_type codeword_type; ///< @brief Codeword type.
	typedef inherited::size_type size_type;         ///< @brief Size type.

	typedef inherited::LLR_type LLR_type;       ///< @brief Log-Likelihood Ratio type.
	typedef std::vector<LLR_type> output_type;  ///< @brief Array of LLR values.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Create demodulator.
/**
		This constructor creates Demodulator object
	with the modulation map @a mmap.

@param mmap Modulation map.
*/
	explicit Demodulator(const ModulationMap &mmap)
		: m_map(mmap)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the modulation map.
/**
		This method changes the current modulation map to the @a mmap.

@param mmap New modulation map.
*/
	void setMap(const ModulationMap &mmap)
	{
		m_map = mmap;
	}


	// publish Modem::soft2hard
	using inherited::soft2hard;


//////////////////////////////////////////////////////////////////////////
/// @name Demodulation
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Demodulate received modulation symbols sequence with custom channel.
/**
		This method calculates LLRs for the received modulation symbols 
		sequence [@a first, @a last).

		Custom channel coefficient is implemented for each received symbol.
	Custom channel coefficients sequence size should be same as the
	received symbols sequence size.

		The result of demodulation operation is stored in
	the internal buffer and available through out() method.

@param first Begin of the received symbols sequence.
@param last End of the received symbols sequence.
@param channel Begin of the custom channel coefficients sequence.

@see Modem::demodulate()
*/
	template<typename In, typename In2>
		void demodulate(In first, In last, In2 channel)
	{
		// prepare output buffer
		const size_type i_size = std::distance(first, last);
		m_out.resize(i_size * map().bps());

		inherited::demodulate(map(),
			first, last, channel, m_out.begin());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Demodulate sequence of received modulation symbols.
/**
		This method calculates LLRs for the
	sequence of received modulation symbols [@a first, @a last).

		This method is meaningful for AWGN channels only.
	For each received symbol the unit channel coefficient is used.

		The result of demodulation operation is stored in
	the internal buffer and available through out() method.

@param first Begin of the received symbols sequence.
@param last End of the received symbols sequence.

@see Modem::demodulate()
*/
	template<typename In>
		void demodulate(In first, In last)
	{
		// prepare output buffer
		const size_type i_size = std::distance(first, last);
		m_out.resize(i_size * map().bps());

		inherited::demodulate(map(),
			first, last, m_out.begin());
	}


public:

//////////////////////////////////////////////////////////////////////////
/// @brief Demodulate vector of received symbols with custom channel.
/**
		This method calculates LLRs (log-likelihood ratios) for the
	vector of received modulation symbols @a x.

		For each received symbol the custom channel coefficient is used.
	The custom channel coefficients vector size should be same as the
	received symbols vector size.

		The result of demodulation operation is stored in
	the internal buffer and available through out() method.

@param x Received symbols vector.
@param channel Custom channel coefficients vector.
@return Demodulated LLR values.

@see Modem::demodulate()
*/
	template<typename TX, typename AX, typename TY, typename AY>
		const output_type& operator()(const std::vector<TX, AX> &x, const std::vector<TY, AY> &channel)
	{
		assert(x.size() <= channel.size()
			&& "both vectors should be same size");

		demodulate(x.begin(), x.end(),
			channel.begin());

		return out();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Demodulate vector of received symbols.
/**
		This method calculates LLRs for the
	sequence of received modulation symbols [@a first, @a last).

		This method is meaningful for AWGN channels only.
	For each received symbol the unit channel coefficient is used.

		The result of demodulation operation is stored in
	the internal buffer and available through out() method.

@param x Received symbols vector.
@return Demodulated LLR values.

@see Modem::demodulate()
*/
	template<typename TX, typename AX>
		const output_type& operator()(const std::vector<TX, AX> &x)
	{
		demodulate(x.begin(), x.end());

		return out();
	}


//////////////////////////////////////////////////////////////////////////
/// @name Properties
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get modulation map.
/**
@return Modulation map.
*/
	const ModulationMap& map() const
	{
		return m_map;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the demodulated LLR values.
/**
		This method returns the demodulated LLR values
	after the last demodulation operation.

@return The demodulated LLR values.
*/
	const output_type& out() const
	{
		return m_out;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get k-th demodulated LLR value.
/**
		This method returns k-th demodulated LLR value
	after the last demodulation operation.

@param k Demodulated LLR value index, should be in range [0, size()).
@return Demodulated LLR value.
*/
	LLR_type operator[](size_type k) const
	{
		assert(k < size()
			&& "index out of range");
		return m_out[k];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get demodulated data size.
/**
		This method returns demodulated data size
	after the last demodulation operation.

@return Demodulated data size.
*/
	size_type size() const
	{
		return m_out.size();
	}

private:
	ModulationMap m_map; // Modulation map
	output_type m_out;   // Demodulated data
};

	} // dsp namespace
} // omni namespace

#endif // __OMNI_MODEM_H_
