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
	@brief Auxiliary constants and functions.

		This header file contains declaration of the
	some useful constants and auxiliary functions.

@author Sergey Polichnoy
*/
#ifndef __OMNI_UTIL_HPP_
#define __OMNI_UTIL_HPP_

#include <omni/defs.hpp>

#include <assert.h>

namespace omni
{
	namespace util
	{ /// @name Constants

//////////////////////////////////////////////////////////////////////////
/// @brief Square root of 2. @hideinitializer
const double SQRT2 = 1.4142135623730950488016887242097;

//////////////////////////////////////////////////////////////////////////
/// @brief Square root of 3. @hideinitializer
const double SQRT3 = 1.7320508075688772935274463415059;

//////////////////////////////////////////////////////////////////////////
/// @brief Decimal logarithm of 2. @hideinitializer
const double LG2 = 0.3010299956639811952137388947245;

//////////////////////////////////////////////////////////////////////////
/// @brief Natural logarithm of 2. @hideinitializer
const double LN2 = 0.6931471805599453094172321214582;

//////////////////////////////////////////////////////////////////////////
/// @brief The @b Pi value. @hideinitializer
const double PI = 3.1415926535897932384626433832795;

	} // Constants


	namespace util
	{ /// @name Conversions

// radians and degrees
double deg2rad(double deg);   ///< @brief Convert degrees to the radians.
double rad2deg(double rad);   ///< @brief Convert radians to the degrees.

// dB and line
double dB2line(double dB);    ///< @brief Convert @b dB to the value in linear scale.
double line2dB(double L);     ///< @brief Convert value in linear scale to the @b dB.

// dBm and watt
double dBm2watt(double dBm);  ///< @brief Convert @b dBm to the watts.
double watt2dBm(double W);    ///< @brief Convert watts to the @b dBm.

// kph and mps
double kph2mps(double kph);   ///< @brief Convert @b kph to the @b mps.
double mps2kph(double mps);   ///< @brief Convert @b mps to the @b kph.

	} // Conversions


	namespace util
	{ /// @name Power of two and parity

//////////////////////////////////////////////////////////////////////////
/// @brief Is integer power of two?
/**
		This functions checks the @a x arguments - is it integer power of two?

		The valid integer powers of two: 0, 1, 2, 4, 8, 16, 32, 64, 128, etc...

@note The template argument @a T should be unsigned integer type.

@param[in] x The unsigned integer.
@return @b true if argument is integer power of two, otherwise @b false.

@see log2()
@see flp2()
@see clp2()
*/
template<typename T> inline
	bool is_ipow2(T x)
{
	return !(x & (x-1));
}


//////////////////////////////////////////////////////////////////////////
/// @brief Binary integer logarithm.
/**
		This function calculates a binary integer logarithm
	of the @a x argument. The input argument should be
	integer power of two and can't be zero.

@code
	log2(1) == 0;
	log2(2) == 1;
	log2(4) == 2;
	log2(8) == 3;
@endcode

@note The template argument @a T should be unsigned integer type.

@param[in] x The unsigned integer.
@return The binary logarithm.

@see is_ipow2()
@see flp2()
@see clp2()
*/
template<typename T>
	T log2(T x)
{
	assert(x!=T() && is_ipow2(x)
		&& "log2() argument should "
		"be integer power of two");

	T res = T();
	while (!(x&1))
	{
		x >>= 1;
		++res;
	}

	return res;
}


// disable some MSVC++ warnings
#if defined(_MSC_VER)
#	pragma warning(push)
#	if (1400 <= _MSC_VER)
#		pragma warning(disable: 4293) // warning C4293: '>>' : shift count negative or too big, undefined behavior
#		pragma warning(disable: 4333) // warning C4333: '>>' : right shift by too large amount, data loss
#	endif // (1400 <= _MSC_VER)
#	pragma warning(disable: 4127) // warning C4127: conditional expression is constant
#endif // (MSC_VER)


//////////////////////////////////////////////////////////////////////////
/// @brief Nearest (floor) integer power of two.
/**
		This function calculates an integer value representing the largest
	integer power of two that is less than or equal to @a x argument.

@code
	flp2(4) == 4;
	flp2(5) == 4;
	flp2(7) == 4;
	flp2(9) == 8;
@endcode

@note The template argument @a T should be unsigned integer type.

@param[in] x The unsigned integer.
@return The nearest (floor) integer power of two.

@see is_ipow2()
@see log2()
@see clp2()
*/
template<typename T>
	T flp2(T x)
{
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;

	if (1 < sizeof(T))
		x |= x >> 8;
	if (2 < sizeof(T))
		x |= x >> 16;
	if (4 < sizeof(T))
		x |= x >> 32;

	return x - (x>>1);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Nearest (ceil) integer power of two.
/**
		This function calculates an integer value representing the smallest
	integer power of two that is greater than or equal to @a x argument.

@code
	clp2(4) == 4;
	clp2(5) == 8;
	clp2(7) == 8;
	clp2(9) == 16;
@endcode

@note The template argument @a T should be unsigned integer type.

@param[in] x The unsigned integer.
@return The nearest (ceil) integer power of two.

@see is_ipow2()
@see log2()
@see flp2()
*/
template<typename T>
	T clp2(T x)
{
	x -= 1;

	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;

	if (1 < sizeof(T))
		x |= x >> 8;
	if (2 < sizeof(T))
		x |= x >> 16;
	if (4 < sizeof(T))
		x |= x >> 32;

	return x + 1;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Parity bit.
/**
		This function calculates a parity bit of the input argument @a x.

	The parity bit is equal to:
		- @b 1, if @a x has even number of nonzero bits,
		- @b 0, if @a x has odd number of nonzero bits.

@note The template argument @a T should be unsigned integer type.

@param[in] x The unsigned integer.
@return The parity bit.
*/
template<typename T>
	T parity(T x)
{
	if (4 < sizeof(T))
		x ^= x >> 32;
	if (2 < sizeof(T))
		x ^= x >> 16;
	if (1 < sizeof(T))
		x ^= x >> 8;

	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;

	return (x&1);
}

// restore some MSVC++ warnings
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // (MSC_VER)

	} // Power of two and parity


	namespace util
	{ /// @name Bits packing/unpacking and flip

//////////////////////////////////////////////////////////////////////////
/// @brief Binary to decimal (MSB first).
/**
		This function converts input binary sequence [@a first,
	@a first + @a Nbits) to the one "decimal" value. First element
	of the input bit sequence is correspond to the most significant
	bit of the returned "decimal" value.

		The number of bits @a Nbits should be less than or equal
	to the size of returned "decimal" value (i.e. 8*sizeof(T)).

		For example, the following code will print "13" (1101b):

@code
	const int bits[] = { 1, 1, 0, 1 };
	std::cout << bi2de_msb<unsigned>(bits, 4) << "\n";
@endcode

		The last argument @a x is used for result type deduction.
	The following two lines are equivalent:

@code
	bi2de_msb<unsigned>(bits, 4);
	bi2de_msb(bits, 4, unsigned(0));
@endcode

@note The template argument @a T should be unsigned integer type.

@param[in] first Begin of the input bit sequence.
@param[in] Nbits Number of bits to converting.
@param[in] x Initial value, by default is zero.
@return The "decimal" value.
*/
template<typename T, typename In>
	T bi2de_msb(In first, size_t Nbits, T x = T())
{
	assert(Nbits <= 8*sizeof(T)
		&& "number of bits too big");

	for (; Nbits; --Nbits)
	{
		x <<= 1;
		if (*first)
			x |= 1;

		++first;
	}

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decimal to binary (MSB first).
/**
		This function convert "decimal" value @a x to the output bit
	sequence [@a first, @a first + @a Nbits). First element of the
	output bit sequence is correspond to the most significant bit
	of the input "decimal" value.

		The number of bits @a Nbits should be less than or equal
	to the size of input "decimal" value (i.e. 8*sizeof(T)).

		For example, the following code will print "1101b" (13):

@code
	std::vector<int> bits(4);
	de2bi_msb(13, bits.size(), bits.begin());
	std::copy(bits.begin(), bits.end(),
		std::ostream_iterator<int>(std::cout, ""));
	std::cout << "b\n";
@endcode

@note The template argument @a T should be unsigned integer type.

@param[in] x Input "decimal" value.
@param[in] Nbits Number of bits to converting.
@param[in] first Begin of the output bit sequence.
@return End of the output bit sequence.
*/
template<typename T, typename Out>
	Out de2bi_msb(T x, size_t Nbits, Out first)
{
	assert(Nbits <= 8*sizeof(T)
		&& "number of bits too big");

	T b = T(1) << Nbits;

	for (; Nbits; --Nbits)
	{
		b >>= 1;
		*first = 0 < (x&b);

		++first;
	}

	return first;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Binary to decimal (LSB first).
/**
		This function converts input binary sequence [@a first,
	@a first + @a Nbits) to the one "decimal" value. First element
	of the input bit sequence is correspond to the least significant
	bit of the returned "decimal" value.

		The number of bits @a Nbits should be less than or equal
	to the size of returned "decimal" value (i.e. 8*sizeof(T)).

		For example, the following code will print "13" (1101b):

@code
	const int bits[] = { 1, 0, 1, 1 };
	std::cout << bi2de_lsb<unsigned>(bits, 4) << "\n";
@endcode

		The last argument @a x is used for result type deduction.
	The following two lines are equivalent:

	@code
	bi2de_lsb<unsigned>(bits, 4);
	bi2de_lsb(bits, 4, unsigned(0));
	@endcode

@note The template argument @a T should be unsigned integer type.

@param[in] first Begin of the input bit sequence.
@param[in] Nbits Number of bits to converting.
@param[in] x Initial value, by default is zero.
@return The "decimal" value.
*/
template<typename T, typename In>
	T bi2de_lsb(In first, size_t Nbits, T x = T())
{
	assert(Nbits <= 8*sizeof(T)
		&& "number of bits too big");

	T b = T(1);

	for (; Nbits; --Nbits)
	{
		if (*first)
			x |= b;

		++first;
		b <<= 1;
	}

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decimal to binary (LSB first).
/**
		This function convert "decimal" value @a x to the output bit
	sequence [@a first, @a first + @a Nbits). First element of the
	output bit sequence is correspond to the least significant bit
	of the input "decimal" value.

		The number of bits @a Nbits should be less than or equal
	to the size of input "decimal" value (i.e. 8*sizeof(T)).

		For example, the following code will print "1011b" (13):

@code
	std::vector<int> bits(4);
	de2bi_lsb(13, bits.size(), bits.begin());
	std::copy(bits.begin(), bits.end(),
		std::ostream_iterator<int>(std::cout, ""));
	std::cout << "b\n";
@endcode

@note The template argument @a T should be unsigned integer type.

@param[in] x Input "decimal" value.
@param[in] Nbits Number of bits to converting.
@param[in] first Begin of the output bit sequence.
@return End of the output bit sequence.
*/
template<typename T, typename Out>
	Out de2bi_lsb(T x, size_t Nbits, Out first)
{
	assert(Nbits <= 8*sizeof(T)
		&& "number of bits too big");

	T b = T(1);

	for (; Nbits; --Nbits)
	{
		*first = 0 < (x&b);

		++first;
		b <<= 1;
	}

	return first;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Reverse the bit order.
/**
		This function returns the flipped @a Nbits
	least significant bits of the argument @a x.

		To flip all bits, you can use the following code:

@code
	bits_flip(x, sizeof(x)*8);
@endcode

@note The template argument @a T should be unsigned integer type.

@param[in] x The input integer value.
@param[in] Nbits Number of least significant bits.
@return The flipped value.
*/
template<typename T>
	T bits_flip(T x, size_t Nbits)
{
	assert(Nbits <= 8*sizeof(T)
		&& "number of bits too big");

	T res = T();
	while (Nbits--)
	{
		res <<= 1;
		res |= x&1;
		x >>= 1;
	}

	return res;
}

	} // Bits packing/unpacking and flip


	namespace util
	{ /// @name Polynomials

//////////////////////////////////////////////////////////////////////////
/// @brief Calculate the polynomial's function.
/**
		This function calculates the value of polynomial's function:

@code
	A[0]*pow(x,N-1) + A[1]*pow(x,N-2)... + A[N-2]*x + A[N-1]
@endcode

	where
	- @b A is a polynomial's coefficients
			defined by sequence [@a first, @a last),
	- @b N is polynomial's order.

		The last argument is used for result type deduction.

@param[in] x The polynomial's argument.
@param[in] first Begin of polynomial's coefficients.
@param[in] last End of polynomial's coefficients.
@return The value of polynomial's function.
*/
template<typename TY, typename TX, typename Bi>
	TY poly(const TX &x, Bi first, Bi last, const TY&)
{
	if (first != last)
	{
		TY y = (*first);
		++first;

		for (; first != last; ++first)
			y = y*x + (*first);

		return y;
	}
	else
		return TY();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Calculate the polynomial's function.
/**
		This function calculates the value of polynomial's function:

@code
	A[0]*pow(x,N-1) + A[1]*pow(x,N-2)... + A[N-2]*x + A[N-1]
@endcode

	where
	- @b A is a polynomial's coefficients
			defined by sequence [@a first, @a last),
	- @b N is polynomial's order.

@note The result's type is equal to the @a x argument's type.

@param[in] x The polynomial's argument.
@param[in] first Begin of polynomial's coefficients.
@param[in] last End of polynomial's coefficients.
@return The value of polynomial's function.
*/
template<typename T, typename Bi> inline
	T poly(const T &x, Bi first, Bi last)
{
	return poly(x, first, last, x);
}

	} // Polynomials

} // omni namespace

#endif // __OMNI_UTIL_HPP_
