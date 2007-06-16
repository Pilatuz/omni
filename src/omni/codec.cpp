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
	@brief Implementation of the coding tools.

@see @ref omni_codec
@author Sergey Polichnoy
*/
#include <omni/codec.h>
#include <omni/calc.hpp>

#include <algorithm>
#include <strstream>


namespace omni
{
	// Trellis
	namespace dsp
	{

// (!) Trellis is faster with column major matrix

//////////////////////////////////////////////////////////////////////////
/** @class Trellis
	@brief Coding trellis.

	Examples of the simple trellis, IS-95 trellis, 3GPP trellis, WiMAX trellis.

	Simple and advanced constructors.

	Trellis manipulation (Rep, Cut, Sys).

	Trellis properties.

	Auxiliary functions.
*/


//////////////////////////////////////////////////////////////////////////
/** @typedef unsigned long Trellis::state_type
	@brief State type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef unsigned long Trellis::bits_type
	@brief Uncoded and coded words type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef unsigned int Trellis::size_type
	@brief Size type.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Create non-systematic convolutional trellis.
/**
		This simple constructor creates non-systematic convolutional trellis.
	For example, to create Trellis for the convolutional encoder with
	polynomials @a G1=0171 and @a G2=0133 and constraint length @a K=7
	you can use the following code:

@code
	Trellis trellis("0171 0133", 7);
@endcode

		The argument @a Ni determine the number of input bits
	per one encoder's tick. By default this argument is 1.

@param opolynomials String with encoder's polynomials.
		You can use octal (0 first), hexadecimal (0x first) or decimal numbers.
@param constraint_length Constraint length of the code.
@param Ni Number of input bits per one encoder's tick.
*/
Trellis::Trellis(const char *opolynomials,
	size_type constraint_length, size_type Ni)
{
	calc::Calculator<int> ev;

	poly_list opoly;
	{ // parse polynomials
		std::istrstream is(opolynomials);
		while (is && !is.eof())
			opoly.push_back(ev(is));
	}

	init(opoly, constraint_length, Ni);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Create recursive systematic convolutional trellis.
/**
		This simple constructor creates recursive systematic convolutional
	trellis. For example, to create Trellis for the 3GPP Convolutional
	Turbo Code with polynomials @a G1=??? and @a G2=??? and constraint
	length @a K=4 you can use the following code:

@code
	Trellis trellis("0x0D", "0x08", 4, 0x03)
@endcode

@param opolynomials Output polynomials.
@param ipolynomials Input polynomials.
@param constraint_length Constraint length of the code.
@param feedback Feedback polynomial.
*/
Trellis::Trellis(const char *opolynomials, const char *ipolynomials,
	size_type constraint_length, bits_type feedback)
{
	calc::Calculator<int> ev;

	poly_list opoly2;
	{ // parse polynomials
		std::istrstream is(opolynomials);
		while (is && !is.eof())
			opoly2.push_back(ev(is));
	}

	poly_list opoly1(opoly2.size(), 0U);

	poly_list ipoly;
	{ // parse polynomials
		std::istrstream is(ipolynomials);
		while (is && !is.eof())
			ipoly.push_back(ev(is));
	}

	init(ipoly, opoly1, opoly2,
		constraint_length, feedback);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Create recursive systematic convolutional trellis.
/**
		This simple constructor creates recursive systematic convolutional
	trellis. For example, to create duo-binary Trellis for the WiMAX
	Convolutional Turbo Code with polynomials @a G1=??? and @a G2=???
	and constraint length @a K=4 you can use the following code:

@code
	Trellis trellis("0x03 0x01", "0x08 0x08", "0x08 0x0E", 4, 0x05);
@endcode

@param pre_opolynomials First output polynomials.
@param opolynomials Second output polynomials.
@param ipolynomials Input polynomials.
@param constraint_length Constraint length of the code.
@param feedback Feedback polynomial.
*/
Trellis::Trellis(const char *pre_opolynomials,
		const char *opolynomials,
		const char *ipolynomials,
		size_type constraint_length,
		bits_type feedback)
{
	calc::Calculator<int> ev;

	poly_list ipoly;
	{ // parse polynomials
		std::istrstream is(ipolynomials);
		while (is && !is.eof())
			ipoly.push_back(ev(is));
	}

	poly_list opoly1;
	{ // parse polynomials
		std::istrstream is(pre_opolynomials);
		while (is && !is.eof())
			opoly1.push_back(ev(is));
	}

	poly_list opoly2;
	{ // parse polynomials
		std::istrstream is(opolynomials);
		while (is && !is.eof())
			opoly2.push_back(ev(is));
	}

	init(ipoly, opoly1, opoly2,
		constraint_length, feedback);
}


//////////////////////////////////////////////////////////////////////////
/** @fn Trellis::Trellis(In ofirst, In olast, size_type constraint_length, size_type Ni)
	@brief Create non-systematic convolutional trellis.

	TODO: description

@param ofirst Custom iterator to the begin of polynomials.
@param olast Custom iterator to the end of polynomials.
@param constraint_length Constraint length of the code.
@param Ni Number of input bits per one encoder's tick.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn Trellis::Trellis(In1 ofirst1, In1 olast1, In2 ofirst2, In2 olast2, In3 ifirst, In3 ilast, size_type constraint_length, bits_type feedback)
	@brief Create recursive systematic convolutional trellis.

	TODO: description

@param ofirst1 Custom iterator to the begin of first output polynomials.
@param olast1 Custom iterator to the end of first output polynomials.
@param ofirst2 Custom iterator to the begin of second output polynomials.
@param olast2 Custom iterator to the end of second output polynomials.
@param ifirst Custom iterator to the begin of input polynomials.
@param ilast Custom iterator to the end of input polynomials.
@param constraint_length Constraint length of the code.
@param feedback Feedback polynomial.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Construct non-systematic convolutional trellis.
/**
@param poly Polynomials.
@param constraint_length Constraint length of the code.
@param Ni Number of input bits per one encoder's tick.
*/
void Trellis::init(const poly_list &poly,
	size_type constraint_length, size_type Ni)
{
	// check input arguments...
	assert(2U<=constraint_length && constraint_length<=30U
		&& "constraint length must be in range [2,30]");
	assert(2U<=poly.size() && poly.size()<=30U
		&& "number of polynomials must be in range [2,30]");
	assert(Ni < constraint_length && "number of input"
		" bits must be less than constraint length");

	const size_type K = constraint_length - Ni;
	const size_type Ni_len = (1U<<Ni);
	const size_type No = poly.size();

	// assign Trellis properties
	m_length = constraint_length;
	m_N_states = (1U << K);
	m_N_obits = No;
	m_N_ibits = Ni;

	m_fwd.resize(N_states(), Ni_len);
	std::vector<bits_type> obits(No);

	// all states, all uncoded (input) bits
	for (state_type state = 0; state < N_states(); ++state)
		for (bits_type ibits = 0; ibits < Ni_len; ++ibits)
	{
		const bits_type reg = (ibits << K) | state;  // shift register

		// calculate coded (output) bits
		for (size_type k = 0; k < No; ++k)
			obits[k] = util::parity(reg & poly[k]);

		m_fwd(state, ibits) = Fwd(reg>>Ni,
			bi2de(obits.begin(), No));
	}

	post_init();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Construct recursive systematic trellis.
/**
@param ipoly Input polynomials.
@param opoly1 First output polynomials.
@param opoly2 Second output polynomials.
@param constraint_length Constraint length of the code.
@param feedback Feedback polynomial.
*/
void Trellis::init(const poly_list &ipoly,
	const poly_list &opoly1, const poly_list &opoly2,
	size_type constraint_length, bits_type feedback)
{
	// check input arguments...
	assert(2U<=constraint_length && constraint_length<=30U
		&& "constraint length must be in range [2,30]");
	assert(1U<=ipoly.size() && ipoly.size()<=30U
		&& "number of input polynomials must be in range [1,30]");
	assert(opoly1.size() == opoly2.size()
		&& "output polynomials must be equal sizes");
	assert(1U<=opoly2.size() && opoly2.size()<=30U
		&& "number of output polynomials must be in range [1,30]");

	const size_type K = constraint_length-1;
	const size_type No = opoly2.size();
	const size_type Ni = ipoly.size();
	const size_type Ni_len = (1U<<Ni);

	// assign Trellis properties
	m_length = constraint_length;
	m_N_states = (1U << K);
	m_N_obits = No;
	m_N_ibits = Ni;

	m_fwd.resize(N_states(), Ni_len);

	std::vector<bits_type> ibits(Ni);
	std::vector<bits_type> obits(No);

	// all states, all uncoded (input) bits
	for (state_type state = 0; state < N_states(); ++state)
		for (bits_type n = 0; n < Ni_len; ++n)
	{
		bits_type ibit = util::parity(state&feedback); // feedback
		bits_type reg = (ibit<<K) | state;   // shift register

		// calculate coded (output) bits (stage 1)
		for (size_type k = 0; k < No; ++k)
			obits[k] = util::parity(reg & opoly1[k]);

		// put input bits
		de2bi(n, Ni, ibits.begin());
		for (size_type k = 0; k < Ni; ++k)
			if (ibits[k]) reg ^= ipoly[k];

		// calculate coded (output) bits (stage 2)
		for (size_type k = 0; k < No; ++k)
			obits[k] ^= util::parity(reg & opoly2[k]);

		m_fwd(state, n) = Fwd(reg>>1,
			bi2de(obits.begin(), No));
	}

	post_init();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Post initialization.
/**
		This method calculates backward matrix (entering branches)
	and number of tail bits.
*/
void Trellis::post_init()
{
	const size_type Ni = N_ibits();
	const size_type Ni_len = (1U<<Ni);

	m_bwd.resize(Ni_len, N_states());

	std::vector<size_type> counter(N_states(), 0);

	// all states, all uncoded (input) bits
	for (state_type state = 0; state < N_states(); ++state)
		for (bits_type ibits = 0; ibits < Ni_len; ++ibits)
	{
		const state_type n = fwd(state, ibits).state;
		m_bwd(counter[n], n) = Bwd(state, ibits);
		counter[n] += 1;

		assert(counter[n] <= Ni_len
			&& "invalid source trellis");
	}

	for (state_type state = 0; state < N_states(); ++state)
		assert(counter[state] == Ni_len
			&& "invalid source trellis");


	// calculate number of tail bits
	m_N_Ztails = 0;
	m_N_Rtails = length()-1; // TODO: number of tails for recursive trellis?
	for (state_type state = 0; state < N_states(); ++state)
	{
		size_type n = 0;

		state_type s = state;
		for (; s && n < length(); ++n)
			s = fwd(s, 0).state;

		if (m_N_Ztails < n)
			m_N_Ztails = n;
	}
}


//////////////////////////////////////////////////////////////////////////
/** @class Trellis::Rep
	@brief Parameters of trellis repetition.

		Contains repetition factor.

	TODO: description.
*/

//////////////////////////////////////////////////////////////////////////
/** @var Trellis::Rep::Nrep
	@brief Repetition factor.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Initialize parameters of trellis repetition.
/**
		This constructor initializes repetition factor by @a n.

@param n Repetition factor.
*/
Trellis::Rep::Rep(size_type n)
	: Nrep(n)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Create trellis by repetition.
/**
		This constructor creates new Trellis using repetition of the @a x.

		TODO: example of repetition.

		TODO: Repetition factor constraints.

@param x Source trellis.
@param p Parameters of trellis repetition.

@see rep()
*/
Trellis::Trellis(const Trellis &x, const Rep &p)
{
	// check input arguments...
	assert(2U<=p.Nrep && p.Nrep*x.N_obits()<=30U
		&& "number of repeats must be in range [2,30]");

	// assign Trellis properties
	m_N_states = x.N_states();
	m_N_ibits = x.N_ibits() * p.Nrep;
	m_N_obits = x.N_obits() * p.Nrep;
	m_length = x.length();

	const size_type Ni_len = (1U << N_ibits());
	m_fwd.resize(N_states(), Ni_len);

	const size_type MASK = ~0U % (1U << x.N_ibits());
	// all states, all uncoded (input) bits
	for (state_type state = 0; state < N_states(); ++state)
		for (bits_type ibits = 0; ibits < Ni_len; ++ibits)
	{
		state_type next_state = state;
		bits_type obits = 0;

		for (size_type k = 0; k < p.Nrep; ++k)
		{
			const size_type i_off = k * x.N_ibits();
			const size_type o_off = k * x.N_obits();

			bits_type ibits_ = (ibits & (MASK<<i_off)) >> i_off;
			obits |= (x.fwd(next_state, ibits_).obits << o_off);
			next_state = x.fwd(next_state, ibits_).state;
		}

		m_fwd(state, ibits) = Fwd(next_state, obits);
	}

	post_init();
}


//////////////////////////////////////////////////////////////////////////
/** @class Trellis::Cut
	@brief Parameters of trellis cutting.

		Contains cutting mask.

	TODO: description.
*/

//////////////////////////////////////////////////////////////////////////
/** @var Trellis::Cut::Mcut
	@brief Cutting mask.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Initialize parameters of trellis cutting.
/**
		This constructor initializes cutting mask by @a m.

@param m Cutting mask.
*/
Trellis::Cut::Cut(bits_type m)
	: Mcut(m)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Create trellis by cutting.
/**
		This constructor creates new Trellis using cutting of the @a x.

		TODO: example of cutting.

		TODO: Cutting mask constraints.

@param x Source trellis.
@param p Parameters of trellis cutting.

@see cut()
*/
Trellis::Trellis(const Trellis &x, const Cut &p)
{
	// check input arguments...
	assert(p.Mcut <= (1U << x.N_obits())
		&& "invalid puncturing mask");

	// assign Trellis properties
	m_N_states = x.N_states();
	m_N_ibits = x.N_ibits();
	m_N_obits = 0;
	m_length = x.length();

	// calculate number of coded (output) bits
	for (size_type k = 0; k < x.N_obits(); ++k)
		if (p.Mcut & (1U << k)) ++m_N_obits;

	const size_type Ni_len = (1U << N_ibits());
	m_fwd.resize(N_states(), Ni_len);

	// all states, all uncoded (input) bits
	for (state_type state = 0; state < N_states(); ++state)
		for (bits_type ibits = 0; ibits < Ni_len; ++ibits)
	{
		bits_type obits = x.fwd(state, ibits).obits;
		size_type shift = 0;

		// puncturing
		for (size_type k = 0; k < x.N_obits(); ++k)
			if (~p.Mcut & (1U << (x.N_obits()-k-1))) // (!) MSB-first
			{
				const size_type off = k - shift++;
				const size_type l_mask = (~0U >> off) << off;
				const size_type r_mask =  ~0U % (1U << off);

				obits = ((obits>>1)&l_mask) | (obits&r_mask);
			}

		m_fwd(state, ibits) = Fwd(x.fwd(state, ibits).state, obits);
	}

	post_init();
}


//////////////////////////////////////////////////////////////////////////
/** @class Trellis::Sys
	@brief Parameters of adding systematic bits.

		Contains number of systematic bits and placement flag.

	TODO: description.
*/

//////////////////////////////////////////////////////////////////////////
/** @var Trellis::Sys::Nsys
	@brief Number of systematic bits.
*/

//////////////////////////////////////////////////////////////////////////
/** @var Trellis::Sys::sys_first
	@brief Systematic bits first flag.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Initialize parameters of adding systematic bits.
/**
		This constructor initializes number of systematic bits by @a n
	and placement flag by @a before.

@param n Number of systematic bits.
@param before Placement flag.
*/
Trellis::Sys::Sys(size_type n, bool before)
	: Nsys(n), sys_first(before)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Create trellis by adding systematic bits.
/**
		This constructor creates new Trellis by adding
	systematic bits to the @a x.

		TODO: example of adding systematic bits.

@param x Source trellis.
@param p Parameters of adding sytematic bits.

@see sys()
*/
Trellis::Trellis(const Trellis &x, const Sys &p)
{
	// check input arguments ...
	assert(1U<=p.Nsys && (p.Nsys + x.N_obits())<=30U
		&& "number of free bits must be in range [1,30]");

	const size_type xNi = x.N_ibits();
	const size_type xNo = x.N_obits();

	// assign Trellis properties
	m_N_states = x.N_states();
	m_N_ibits = xNi + p.Nsys;
	m_N_obits = xNo + p.Nsys;
	m_length = x.length();

	const size_type Ni_len = (1U << N_ibits());
	m_fwd.resize(N_states(), Ni_len);

	// all states, all uncoded (input) bits
	for (state_type state = 0; state < N_states(); ++state)
		for (bits_type ibits = 0; ibits < Ni_len; ++ibits)
	{
		const bits_type xibits = p.sys_first
			? (ibits >> p.Nsys) : ibits % (1U<<xNi);
		const bits_type sbits = p.sys_first
			? ibits % (1U<<p.Nsys) : ibits >> xNi;

		const bits_type xobits = x.fwd(state, xibits).obits;
		const bits_type obits = p.sys_first
			? (xobits << p.Nsys) | sbits
			: (sbits << xNo) | xobits;

		m_fwd(state, ibits) = Fwd(x.fwd(state, xibits).state, obits);
	}

	post_init();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
/**
		This constructor creates empty trellis.

		Empty trellis can't be used for encoding or decoding.
*/
Trellis::Trellis()
	: m_N_states(0),
	  m_N_ibits(0),
	  m_N_obits(0),
	  m_N_Ztails(0),
	  m_N_Rtails(0),
	  m_length(0)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get Optimum Distance Spectrum code.
/**
		Static method creates optimum distance spectrum trellis
	for specified constraint length and code rate.

		Only the following combination of the constraint length
	and code rate are valid:

	<center>
	<table border="1" cellspacing="1" cellpadding="3">
		<caption>Code rate and constraint length</caption>
		<tr align="center">
			<th>code rate</th>
			<th>constraint lengths</th>
		</tr>
		<tr align="center">
			<td> 1/2 </td>
			<td> 3,4,5,6,7,8,9,10,11,12,13,14,15,16 </td>
		</tr>
		<tr align="center">
			<td> 1/3 </td>
			<td> 3,4,5,6,7,8,9,10,11,12,13 </td>
		</tr>
		<tr align="center">
			<td> 1/4 </td>
			<td> 3,4,5,6,7,8,9,10,11 </td>
		</tr>
	</table>
	</center>

@param constraint_length Constraint length of the code. See table above.
@param code_rate Invarse code rate. Should be 2, 3 or 4.
@return New trellis.
*/
Trellis Trellis::ODS_code(size_type constraint_length, size_type code_rate)
{
	switch (code_rate)
	{
		case 2: // 1/2
			switch (constraint_length)
			{
				case  3: return Trellis("05 07", 3);
				case  4: return Trellis("015 017", 4);
				case  5: return Trellis("023 035", 5);
				case  6: return Trellis("053 075", 6);
				case  7: return Trellis("0133 0171", 7);
				case  8: return Trellis("0247 0371", 8);
				case  9: return Trellis("0561 0753", 9);
				case 10: return Trellis("01151 01753", 10);
				case 11: return Trellis("03345 03613", 11);
				case 12: return Trellis("05261 07173", 12);
				case 13: return Trellis("012767 016461", 13);
				case 14: return Trellis("027251 037363", 14);
				case 15: return Trellis("063057 044735", 15);
				case 16: return Trellis("0126723 0152711", 16);
				default: assert(!"invalid constraint length for ODS code (1/2)");
			}
		break;

		case 3: // 1/3
			switch (constraint_length)
			{
				case  3: return Trellis("05 07 07", 3);
				case  4: return Trellis("013 015 017", 4);
				case  5: return Trellis("025 033 037", 5);
				case  6: return Trellis("047 053 075", 6);
				case  7: return Trellis("0133 0165 0171", 7);
				case  8: return Trellis("0225 0331 0367", 8);
				case  9: return Trellis("0575 0623 0727", 9);
				case 10: return Trellis("01233 01375 01671", 10);
				case 11: return Trellis("02335 02531 03477", 11);
				case 12: return Trellis("05745 06471 07553", 12);
				case 13: return Trellis("013261 015167 017451", 13);
				default: assert(!"invalid constraint length for ODS code (1/3)");
			}
		break;

		case 4: // 1/4
			switch (constraint_length)
			{
				case  3: return Trellis("05 05 07 07", 3);
				case  4: return Trellis("013 015 015 017", 4);
				case  5: return Trellis("025 027 033 037", 5);
				case  6: return Trellis("051 055 067 077", 6);
				case  7: return Trellis("0117 0127 0155 0171", 7);
				case  8: return Trellis("0231 0273 0327 0375", 8);
				case  9: return Trellis("0473 0513 0671 0765", 9);
				case 10: return Trellis("01173 01325 01467 01751", 10);
				case 11: return Trellis("02565 02747 03311 03723", 11);
				default: assert(!"invalid constraint length for ODS code (1/4)");
			}
		break;

		default:
			assert(!"invalid code rate for ODS codes");
	}

	return Trellis();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get Maximum Free Distance code.
/**
		Static method creates maximum free distance trellis
	for specified constraint length and code rate.

		Only the following combination of the constraint length
	and code rate are valid:

	<center>
	<table border="1" cellspacing="1" cellpadding="3">
		<caption>Code rate and constraint length</caption>
		<tr align="center">
			<th>code rate</th>
			<th>constraint lengths</th>
		</tr>
		<tr align="center">
			<td> 1/2 </td>
			<td> 3,4,5,6,7,8,9,10,11,12,13,14 </td>
		</tr>
		<tr align="center">
			<td> 1/3 </td>
			<td> 3,4,5,6,7,8,9,10,11,12,13,14 </td>
		</tr>
		<tr align="center">
			<td> 1/4 </td>
			<td> 3,4,5,6,7,8,9,10,11,12,13,14 </td>
		</tr>
		<tr align="center">
			<td> 1/5 </td>
			<td> 3,4,5,6,7,8 </td>
		</tr>
		<tr align="center">
			<td> 1/6 </td>
			<td> 3,4,5,6,7,8 </td>
		</tr>
		<tr align="center">
			<td> 1/7 </td>
			<td> 3,4,5,6,7,8 </td>
		</tr>
		<tr align="center">
			<td> 1/8 </td>
			<td> 3,4,5,6,7,8 </td>
		</tr>
	</table>
	</center>

@param constraint_length Constraint length of the code. See table above.
@param code_rate Invarse code rate. Should be 2, 3, 4, 5, 6, 7, or 8.
@return New trellis.
*/
Trellis Trellis::MFD_code(size_type constraint_length, size_type code_rate)
{
	switch (code_rate)
	{
		case 2: // 1/2
			switch (constraint_length)
			{
				case  3: return Trellis("05 07", 3);
				case  4: return Trellis("015 017", 4);
				case  5: return Trellis("023 035", 5);
				case  6: return Trellis("053 075", 6);
				case  7: return Trellis("0133 0171", 7);
				case  8: return Trellis("0247 0371", 8);
				case  9: return Trellis("0561 0753", 9);
				case 10: return Trellis("01167 01545", 10);
				case 11: return Trellis("02335 03661", 11);
				case 12: return Trellis("04335 05723", 12);
				case 13: return Trellis("010533 017661", 13);
				case 14: return Trellis("021675 027123", 14);
				default: assert(!"invalid constraint length for MFD code (1/2)");
			}
		break;

		case 3: // 1/3
			switch (constraint_length)
			{
				case  3: return Trellis("05 07 07", 3);
				case  4: return Trellis("013 015 017", 4);
				case  5: return Trellis("025 033 037", 5);
				case  6: return Trellis("047 053 075", 6);
				case  7: return Trellis("0133 0145 0175", 7);
				case  8: return Trellis("0225 0331 0367", 8);
				case  9: return Trellis("0557 0663 0711", 9);
				case 10: return Trellis("0117 01365 01633", 10);
				case 11: return Trellis("02353 02671 03175", 11);
				case 12: return Trellis("04767 05723 06265", 12);
				case 13: return Trellis("010533 010675 017661", 13);
				case 14: return Trellis("021645 035661 037133", 14);
				default: assert(!"invalid constraint length for MFD code (1/3)");
			}
			break;

		case 4: // 1/4
			switch (constraint_length)
			{
				case  3: return Trellis("05 07 07 07", 3);
				case  4: return Trellis("013 015 015 017", 4);
				case  5: return Trellis("025 027 033 037", 5);
				case  6: return Trellis("053 067 071 075", 6);
				case  7: return Trellis("0135 0135 0147 0163", 7);
				case  8: return Trellis("0235 0275 0313 0357", 8);
				case  9: return Trellis("0463 0535 0733 0745", 9);
				case 10: return Trellis("0117 01365 01633 01653", 10);
				case 11: return Trellis("02327 02353 02671 03175", 11);
				case 12: return Trellis("04767 05723 06265 07455", 12);
				case 13: return Trellis("011145 012477 015573 016727", 13);
				case 14: return Trellis("021113 023175 035527 035537", 14);
				default: assert(!"invalid constraint length for MFD code (1/4)");
			}
			break;

		case 5: // 1/5
			switch (constraint_length)
			{
				case 3: return Trellis("07 07 07 05 05", 3);
				case 4: return Trellis("017 017 013 015 015", 4);
				case 5: return Trellis("037 027 033 025 035", 5);
				case 6: return Trellis("075 071 073 065 057", 6);
				case 7: return Trellis("0175 0131 0135 0135 0147", 7);
				case 8: return Trellis("0257 0233 0323 0271 0357", 8);
				default: assert(!"invalid constraint length for MFD code (1/5)");
			}
			break;

		case 6: // 1/6
			switch (constraint_length)
			{
				case 3: return Trellis("07 07 07 07 05 05", 3);
				case 4: return Trellis("017 017 013 013 015 015", 4);
				case 5: return Trellis("037 035 027 033 025 035", 5);
				case 6: return Trellis("073 075 055 065 047 057", 6);
				case 7: return Trellis("0173 0151 0135 0135 0163 0137", 7);
				case 8: return Trellis("0253 0375 0331 0235 0313 0357", 8);
				default: assert(!"invalid constraint length for MFD code (1/6)");
			}
			break;

		case 7: // 1/7
			switch (constraint_length)
			{
				case 3: return Trellis("07 07 07 07 05 05 05", 3);
				case 4: return Trellis("017 017 013 013 013 015 015", 4);
				case 5: return Trellis("035 027 025 027 033 035 037", 5);
				case 6: return Trellis("053 075 065 075 047 067 057", 6);
				case 7: return Trellis("0165 0145 0173 0135 0135 0147 0137", 7);
				case 8: return Trellis("0275 0253 0375 0331 0235 0313 0357", 8);
				default: assert(!"invalid constraint length for MFD code (1/7)");
			}
			break;

		case 8: // 1/8
			switch (constraint_length)
			{
				case 3: return Trellis("07 07 05 05 05 07 07 07", 3);
				case 4: return Trellis("017 017 013 013 013 015 015 017", 4);
				case 5: return Trellis("037 033 025 025 035 033 027 037", 5);
				case 6: return Trellis("057 073 051 065 075 047 067 057", 6);
				case 7: return Trellis("0153 0111 0165 0173 0135 0135 0147 0137", 7);
				case 8: return Trellis("0275 0275 0253 0371 0331 0235 0313 0357", 8);
				default: assert(!"invalid constraint length for MFD code (1/8)");
			}
			break;

		default:
			assert(!"invalid code rate for MFD codes");
	}

	return Trellis();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Repeat this trellis.
/**
		Method repeats the current trellis @a Nrep times.

@param Nrep Repetition factor.
@see Rep
*/
Trellis& Trellis::rep(size_type Nrep)
{
	*this = Trellis(*this, Rep(Nrep));
	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Cut this trellis.
/**
		Method cut the current trellis with mask @a Mcut.

@param Mcut Cutting mask.
@see Cut
*/
Trellis& Trellis::cut(bits_type Mcut)
{
	*this = Trellis(*this, Cut(Mcut));
	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Add systematic bits.
/**
		Method adds systematic bits to the current trellis.

@param Nsys Number of systematic bits.
@param before Placement flag.
@see Sys
*/
Trellis& Trellis::sys(size_type Nsys, bool before)
{
	*this = Trellis(*this, Sys(Nsys, before));
	return *this;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get number of states.
/**
		Method returns number of states of the current trellis.

@return Number of states.
*/
Trellis::size_type Trellis::N_states() const
{
	return m_N_states;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get number of input bits.
/**
		Method returns number of input (uncoded) bits per one encoder tick.

@return Number of input bits.
*/
Trellis::size_type Trellis::N_ibits() const
{
	return m_N_ibits;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get number of output bits.
/**
		Method returns number of output (encoded) bits per one encoder tick.

@return Number of output bits.
*/
Trellis::size_type Trellis::N_obits() const
{
	return m_N_obits;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get number of tail bits.
/**
		Method returns number of tail bits.

		For non-recursive trellis number of tail bits is number of zeros,
	needed to put into encoder to reset encoder in zero state.

		For recursive trellis number of tails is number of bits (non-zero),
	needed to put into encoder to reset encoder in zero state.

@param recursive Recursive trellis flag.
@return Number of tail bits.
*/
Trellis::size_type Trellis::N_tails(bool recursive) const
{
	return recursive ? m_N_Rtails : m_N_Ztails;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get constraint length.
/**
		Method returns constraint length of the code.

@return Constraint length.
*/
Trellis::size_type Trellis::length() const
{
	return m_length;
}


//////////////////////////////////////////////////////////////////////////
/** @class Trellis::Fwd
	@brief Forward state transition.

		This structure contains properties of the forward state transition:
	target (next or entering) state and corresponding output (encoded) bits.
*/

//////////////////////////////////////////////////////////////////////////
/** @var Trellis::Fwd::state
	@brief Target (next or entering) state.
*/

//////////////////////////////////////////////////////////////////////////
/** @var Trellis::Fwd::obits
	@brief Output (encoded) bits.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Create forward state transition.
/**
		This constructor creates forward state
	transition with specified properties.

@param s Target (next or entering) state.
@param o Output (encoded) bits.
*/
Trellis::Fwd::Fwd(state_type s, bits_type o)
	: state(s), obits(o)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
/**
		This constructor creates forward state
	transition with default properties.
*/
Trellis::Fwd::Fwd()
	: state(0), obits(0)
{}


//////////////////////////////////////////////////////////////////////////
/** @class Trellis::Bwd
	@brief Backward state transition.

		This structure contains properties of the backward state transition:
	source (previous or leaving) state and corresponding input (uncoded) bits.
*/

//////////////////////////////////////////////////////////////////////////
/** @var Trellis::Bwd::state
	@brief Source (previous or leaving) state.
*/

//////////////////////////////////////////////////////////////////////////
/** @var Trellis::Bwd::ibits
	@brief Input (uncoded) bits.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Create backward state transition.
/**
		This constructor creates backward state
	transition with specified properties.

@param s Source (previous or leaving) state.
@param i Input (uncoded) bits.
*/
Trellis::Bwd::Bwd(state_type s, bits_type i)
	: state(s), ibits(i)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
/**
		This constructor creates backward state
	transition with default properties.
*/
Trellis::Bwd::Bwd()
	: state(0), ibits(0)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Forward state transition.
/**
		Method returns the forward state transition's properties
	for the current state @a state and specified input (uncoded)
	bits @a ibits.

@param state Current (leaving) state.
@param ibits Input (uncoded) bits.
@return Forward state transition properties.
*/
const Trellis::Fwd& Trellis::fwd(state_type state, bits_type ibits) const
{
	assert(state < m_fwd.N_rows() && "invalid state");
	assert(ibits < m_fwd.N_cols() && "invalid input bits");
	return m_fwd(state, ibits);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Backward state transition.
/**
		Method returns the backward state transition's properties
	for the current state @a state and specified transition number @a n.

@param state Current (entering) state.
@param n Transition's number.
@return Backward state transition properties.
*/
const Trellis::Bwd& Trellis::bwd(state_type state, size_type n) const
{
	assert(state < m_bwd.N_cols() && "invalid state");
	assert(n < m_bwd.N_rows() && "invalid transition number");
	return m_bwd(n, state);
}


//////////////////////////////////////////////////////////////////////////
/** @fn bits_type Trellis::bi2de(In first, size_t Nbits)
	@brief Convert the binary sequence to the code word.

		Static method convert binary sequence @a first
	to the code word with @a Nbits significant bits.

	First bit in the binary sequence is MSB bit of the code word.

@param first Begin of the binary sequence.
@param Nbits Number of bits to be converted.
@return Converted code word.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn void Trellis::de2bi(bits_type x, size_t Nbits, Out first)
	@brief Convert the code word to the binary sequence.

		Static method convert @a Nbits significant bits of the code word @a x
	to the binary sequence @a first.

		First bit in the binary sequence is MSB bit of the code word.

@param x Code word to be converted.
@param Nbits Number of bits to be converted.
@param first Begin of the binary sequence.
*/

	} // Trellis


	// ConvCodec
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/** @class ConvCodec
	@brief Convolutional encoding and decoding tool.

		TODO: tail and tailbite encoding.
*/


//////////////////////////////////////////////////////////////////////////
/** @typedef Trellis::state_type ConvCodec::state_type
	@brief State type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef Trellis::bits_type ConvCodec::bits_type
	@brief Uncoded and coded words type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef Trellis::size_type ConvCodec::size_type
	@brief Size type.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Object constructor.
/**
		This constructor creates ConvCodec object with trellis @a tr.

@param tr Convolutional trellis object.
*/
ConvCodec::ConvCodec(const Trellis &tr)
	: m_trellis(tr)
{
	check_fast(tr);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get current trellis.
/**
		Method returns current trellis.

@return Current trellis.
*/
const Trellis& ConvCodec::trellis() const
{
	return m_trellis;
}


//////////////////////////////////////////////////////////////////////////
/** @fn void ConvCodec::encode_tail(In first, In last, Out out) const
	@brief Encode block with tail zeros.

		Method encodes data block [@a first, @a last), add tail zeros,
	and store results to the output buffer @a out.

		Размер буфера результатов должен быть достаточным,
	чтобы вместить все кодированные данные.

@param first Begin of the uncoded buffer.
@param last End of the uncoded buffer.
@param out Begin of the encoded buffer.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn void ConvCodec::encode_bite(In first, In last, Out out) const
	@brief Encode block with tailbite technique.

		Method encodes data block [@a first, @a last) and store results
	to the output buffer @a out.

		Размер буфера результатов должен быть достаточным,
	чтобы вместить все кодированные данные.

@param first Begin of the uncoded buffer.
@param last End of the uncoded buffer.
@param out Begin of the encoded buffer.
*/


//////////////////////////////////////////////////////////////////////////
/** @class ConvCodec::BranchMetrics
	@brief Metrics set for each branch.

		This class contains branch metrics for each branch for all
	possible combinations of output (encoded) bits.
*/

// TODO: BranchMetrics - matrix row/column major adaptation

//////////////////////////////////////////////////////////////////////////
/// @brief Object constructor.
/**
		This constructor create branch metrics for @a length state
	transitions and for @a Nbits output (encoded) bits.
*/
ConvCodec::BranchMetrics::BranchMetrics(size_type length, size_type Nbits)
	: m_metrics(1U<<Nbits, length, 0.0)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get branch metric.
/**
		Method returns the branch metric for the state transition @a k
	and the specified bits combination @a bits.

@param k State transition number.
@param bits Bits combination.
@return Branch metric.
*/
double ConvCodec::BranchMetrics::operator()(size_type k, size_type bits) const
{
	return m_metrics(bits, k);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set branch metric.
/**
		Method returns reference to the branch metric for the state transition @a k
	and the specified bits combination @a bits.

@param k State transition number.
@param bits Bits combination.
@return Branch metric reference.
*/
double& ConvCodec::BranchMetrics::operator()(size_type k, size_type bits)
{
	return m_metrics(bits, k);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get number of state transitions.
/**
		Method returns number of state transitions.

@return Number of state transitions.
*/
ConvCodec::size_type ConvCodec::BranchMetrics::length() const
{
	return m_metrics.N_cols();
}


//////////////////////////////////////////////////////////////////////////
/** @fn void ConvCodec::decode_tail(const BranchMetrics &bm, Out out) const
	@brief Decode from zero state.

@param bm Branch metrics.
@param out Output buffer.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn void ConvCodec::decode_bite(size_type Niter, const BranchMetrics &bm, Out out) const
	@brief Decode from unknown state.

@param Niter Number of iterations.
@param bm Branch metrics.
@param out Output buffer.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn void ConvCodec::decode_tail(In first, In last, Out out) const
	@brief Decode from zero state.

@param first Begin of the demodulated LLR values.
@param last End of demodulated LLR values.
@param out Output buffer.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn void ConvCodec::decode_bite(size_type Niter, In first, In last, Out out) const
	@brief Decode from unknown state.

@param Niter Number of iterations.
@param first Begin of the demodulated LLR values.
@param last End of demodulated LLR values.
@param out Output buffer.
*/


//////////////////////////////////////////////////////////////////////////
// Viterbi iteration
void ConvCodec::viterbi_iteration(const BranchMetrics &bm,
	XPathMem &path, metric_vector &metrics) const
{
	const size_type Nstates = trellis().N_states();
	const bits_type K_MASK = trellis().N_states()-1;

	assert(!m_X_trellis.empty() && "invalid fast trellis");
	metric_vector metrics_(metrics.size()); // new metrics

	for (size_type k = 0; k < bm.length(); ++k)
	{
		// TODO: path_k = path.col_begin(k);
		// TODO: bm_k = bm.get(k);

		for (state_type state = 0; state < Nstates; ++state)
		{
			const bits_type reg0 = (state << 1) | 0;
			const bits_type reg1 = (state << 1) | 1;
			const bits_type obits0 = m_X_trellis[reg0];
			const bits_type obits1 = m_X_trellis[reg1];
			const state_type prev0 = reg0 & K_MASK;
			const state_type prev1 = reg1 & K_MASK;

			double metric0 = metrics[prev0] + bm(k, obits0);
			double metric1 = metrics[prev1] + bm(k, obits1);
			if (metric0 < metric1)
			{
				metrics_[state] = metric1;
				path(state, k) = prev1;
			}
			else
			{
				metrics_[state] = metric0;
				path(state, k) = prev0;
			}
		}

		// apply new metrics
		metrics.swap(metrics_);
	}
}


//////////////////////////////////////////////////////////////////////////
// Viterbi iteration
void ConvCodec::viterbi_iteration(const BranchMetrics &bm,
	PathMem &path, metric_vector &metrics) const
{
	const size_type Ni_len = (1U << trellis().N_ibits());
	const size_type Nstates = trellis().N_states();

	assert(m_X_trellis.empty() && "use fast trellis");
	metric_vector metrics_(metrics.size()); // new metrics

	for (size_type k = 0; k < bm.length(); ++k)
	{
		// TODO: path_k = path.col_begin(k);
		// TODO: bm_k = bm.get(k);

		for (state_type state = 0; state < Nstates; ++state)
		{
			double max_metric = -std::numeric_limits<double>::infinity();
			state_type max_state = 0; // переход с максимальной метрикой
			bits_type max_ibits = 0;

			for (size_type n = 0; n < Ni_len; ++n) // all variants
			{
				const Trellis::Bwd &bwd = trellis().bwd(state, n);
				const Trellis::Fwd &fwd = trellis().fwd(bwd.state, bwd.ibits);

				// determine maximum metric
				double metric = metrics[bwd.state] + bm(k, fwd.obits);
				if (max_metric < metric)
				{
					max_metric = metric;
					max_ibits = bwd.ibits;
					max_state = bwd.state;
				}
			}

			// store max metric
			metrics_[state] = max_metric;
			path(state, k) = Trellis::Bwd(max_state, max_ibits);
		}

		// apply new metrics
		metrics.swap(metrics_);
	}
}


//////////////////////////////////////////////////////////////////////////
// get decoded bits
void ConvCodec::trace_back(const XPathMem &path, state_type state, bit_vector &out) const
{
	const size_type SHIFT = trellis().length()-2;

	assert(!m_X_trellis.empty() && "invalid fast trellis");

	bit_vector::reverse_iterator out_it = out.rbegin();
	for (ptrdiff_t k = path.N_cols()-1; 0 <= k; --k)
	{
		*out_it = bit_vector::value_type(state >> SHIFT);
		state = path(state, k);
		++out_it;
	}
}


//////////////////////////////////////////////////////////////////////////
// get decoded bits
void ConvCodec::trace_back(const PathMem &path, state_type state, bit_vector &out) const
{
	const size_type Ni = trellis().N_ibits();

	assert(m_X_trellis.empty() && "use fast trellis");

	bit_vector::iterator out_it = out.begin() + out.size() - Ni;
	for (ptrdiff_t k = path.N_cols()-1; 0 <= k; --k)
	{
		const Trellis::Bwd &item = path(state, k);
		const state_type prev = item.state;
		const bits_type ibits = item.ibits;

		Trellis::de2bi(ibits, Ni, out_it);

		state = prev;
		if (k) out_it -= Ni;
	}
}


//////////////////////////////////////////////////////////////////////////
// find start state
ConvCodec::state_type ConvCodec::find_start_state(const XPathMem &path, state_type state)
{
	for (ptrdiff_t k = path.N_cols()-1; 0 <= k; --k)
		state = path(state, k);

	return state;
}


//////////////////////////////////////////////////////////////////////////
// find start state
ConvCodec::state_type ConvCodec::find_start_state(const PathMem &path, state_type state)
{
	for (ptrdiff_t k = path.N_cols()-1; 0 <= k; --k)
		state = path(state, k).state;

	return state;
}


//////////////////////////////////////////////////////////////////////////
// check for fast trellis
bool ConvCodec::check_fast(const Trellis &tr)
{
	if (1U != tr.N_ibits())
		return false;

	const size_type Nstates = tr.N_states();
	const size_type K = tr.length() - 1;
	const bits_type K_MASK = (1U<<K) - 1;

	std::vector<state_type> prev_states(2);
	std::vector<state_type> prev_states_(2);
	for (state_type state = 0; state < Nstates; ++state)
	{
		for (size_type i = 0; i < 2; ++i) // all variants
		{
			prev_states_[i] = tr.bwd(state, i).state;
			prev_states[i] = ((state<<1) | i) & K_MASK;
		}

		// check prev_states
		std::sort(prev_states_.begin(), prev_states_.end());
		std::sort(prev_states.begin(),  prev_states.end());
		if (!std::equal(prev_states_.begin(),
			prev_states_.end(), prev_states.begin()))
				return false;

		// check ibits
		bits_type ibits = state >> (K-1);
		for (size_type n = 0; n < 2; ++n)
			if (tr.bwd(state, n).ibits != ibits)
				return false;
	}

	// form fast trellis
	const size_type FAST_LEN = 1U << (K+1);
	m_X_trellis.resize(FAST_LEN);
	for (bits_type reg = 0; reg < FAST_LEN; ++reg)
	{
		const state_type state = reg % Nstates;
		const bits_type ibits = reg >> K;
		m_X_trellis[reg] = tr.fwd(state, ibits).obits;
	}

	return true;
}

	} // ConvCodec


	// ConvEncoder
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/** @class ConvEncoder
	@brief Convolutional encoder.
*/


//////////////////////////////////////////////////////////////////////////
/** @typedef Trellis::state_type ConvEncoder::state_type
	@brief State type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef Trellis::bits_type ConvEncoder::bits_type
	@brief Uncoded and coded words type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef Trellis::size_type ConvEncoder::size_type
	@brief Size type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef std::vector<bits_type> ConvEncoder::output_type
	@brief Encoded buffer type.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Object constructor.
/**
		This constructor creates ConvEncoder object with trellis @a tr.

@param tr Trellis.
*/
ConvEncoder::ConvEncoder(const Trellis &tr)
	: inherited(tr)
{}


//////////////////////////////////////////////////////////////////////////
/** @fn const output_type& ConvEncoder::encode_tail(In first, In last)
	@brief Encode from zero state.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn const output_type& ConvEncoder::encode_bite(In first, In last)
	@brief Encode from final state.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn const output_type& ConvEncoder::operator()(In first, In last)
	@brief Encode from final state.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn const output_type& ConvEncoder::operator()(const std::vector<TX, AX> &x)
	@brief Encode from final state.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Get output data.
/**
		Method returns output (encoded) data after last encoding process.

@return Output (encoded) data.
*/
const ConvEncoder::output_type& ConvEncoder::out() const
{
	return m_out;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get output bit.
/**
	Method returns output (encoded) k-th bit after last encoding process.
*/
ConvEncoder::bits_type ConvEncoder::operator[](size_type k) const
{
	assert(k < size() && "index out of range");
	return m_out[k];
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get output data size.
/**
		Method returns output (encoded) data size after last encoding process.

@return Output (encoded) data.
*/
ConvEncoder::size_type ConvEncoder::size() const
{
	return m_out.size();
}

	} // ConvEncoder


	// ConvDecoder
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/** @class ConvDecoder
	@brief Viterbi decoder.

	TODO: description
*/


//////////////////////////////////////////////////////////////////////////
/** @typedef inherited::state_type ConvDecoder::state_type
	@brief State type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef inherited::bits_type ConvDecoder::bits_type
	@brief Uncoded and coded words type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef inherited::size_type ConvDecoder::size_type
	@brief Size type.
*/

//////////////////////////////////////////////////////////////////////////
/** @typedef std::vector<bits_type> ConvDecoder::output_type
	@brief Ddecoded buffer type.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Object construction.
/**
@param tr Convolutional trellis.
@param N_iters Number of iterations for tailbite decoding.
*/
ConvDecoder::ConvDecoder(const Trellis &tr, size_type N_iters)
	: inherited(tr), m_N_iters(N_iters)
{}


//////////////////////////////////////////////////////////////////////////
/** @fn const output_type& ConvDecoder::decode_tail(In first, In last)
	@brief Decode from known state.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn const output_type& ConvDecoder::decode_bite(In first, In last)
	@brief Decode from unknown state.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn const output_type& ConvDecoder::operator()(In first, In last)
	@brief Decode from unknown state.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn const output_type& ConvDecoder::operator()(const std::vector<TX, AX> &x)
	@brief Decode from unknown state.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Get output (decoded) data.
/**
@return Output (decoded) data.
*/
const ConvDecoder::output_type& ConvDecoder::out() const
{
	return m_out;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get output (decoded) bit.
/**
@return Output (decoded) bit.
*/
ConvDecoder::bits_type ConvDecoder::operator[](size_type k) const
{
	assert(k < size() && "index out of range");
	return m_out[k];
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get output (decoded) data size.
/**
@return Output (decoded) data size.
*/
ConvDecoder::size_type ConvDecoder::size() const
{
	return m_out.size();
}

	} // ConvDecoder


	// Interleaver
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/** @class Interleaver
	@brief interleaver.

	TODO: descritpion
*/


//////////////////////////////////////////////////////////////////////////
/** @typedef std::vector<size_t> Interleaver::rule_type
	@brief Interleaving rule type.
*/


//////////////////////////////////////////////////////////////////////////
/// @brief Object construction.
/**
@param irule Interleaving rule.
*/
Interleaver::Interleaver(const rule_type &irule)
	: m_irule(irule)
{
	assert(check_rule(m_irule)
		&& "Invalid interleaving rule");
}


//////////////////////////////////////////////////////////////////////////
/// @brief Create matrix interleaving rule.
/**
@param row2col If @b true then write row-by-row, read column-by-column.
               If @b false then write column-by-column, read row-by-row.
@param Nrows Number of rows.
@param Ncols Number of columns.
@param blockSize Block size.
*/
Interleaver::rule_type Interleaver::create_matrix(bool row2col, size_t Nrows, size_t Ncols, size_t blockSize)
{
	assert(blockSize <= Nrows*Ncols
		&& "invalid block size");

	rule_type r; r.reserve(blockSize);

	if (row2col)
	{
		for (size_t j = 0; j < Ncols; ++j)
			for (size_t i = 0; i < Nrows; ++i)
		{
			size_t kk = i*Ncols + j;
			if (kk < blockSize)
				r.push_back(kk);
		}
	}
	else
	{
		for (size_t i = 0; i < Nrows; ++i)
			for (size_t j = 0; j < Ncols; ++j)
		{
			size_t kk = j*Nrows + i;
			if (kk < blockSize)
				r.push_back(kk);
		}
	}

	assert(r.size() == blockSize
		&& "invalid rule");

	return r;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Create matrix interleaving rule.
/**
@param Nrows Number of rows.
@param Ncols Number of columns.
*/
Interleaver::rule_type Interleaver::create_matrix(bool row2col, size_t Nrows, size_t Ncols)
{
	return create_matrix(row2col, Nrows, Ncols, Nrows*Ncols);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Check interleaving rule
/**
@param irule Interleaving rule.
@return @b true if @a irule is a valid interleaving rule, otherwise @b false.
*/
bool Interleaver::check_rule(const rule_type &irule)
{
	rule_type tmp = irule;
	std::sort(tmp.begin(), tmp.end());

	for (size_t i = 0; i < tmp.size(); ++i)
		if (tmp[i] != i)
			return false;

	return true;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get interleaving rule.
/**
@return Interleaving rule.
*/
const Interleaver::rule_type& Interleaver::rule() const
{
	return m_irule;
}


//////////////////////////////////////////////////////////////////////////
/** @fn void Interleaver::interleave(In first, In last, Out out) const
	@brief Interleave input sequence.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn void Interleaver::interleave(const std::vector<TX, AX> &x, std::vector<TY, AY> &y) const
	@brief Interleave input vector.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn void Interleaver::deinterleave(In first, In last, Out out) const
	@brief Deinterleave input sequence.
*/


//////////////////////////////////////////////////////////////////////////
/** @fn void Interleaver::deinterleave(const std::vector<TX, AX> &x, std::vector<TY, AY> &y) const
	@brief Deinterleave input vector.
*/

	} // Interleaver
} // omni namespace


//////////////////////////////////////////////////////////////////////////
/** @page omni_codec Convolutional Coding

		Classes omni::dsp::Trellis omni::dsp::ConvCodec
	omni::dsp::ConvEncoder omni::dsp::ConvDecoder
	omni::dsp::Interleaver.
*/
