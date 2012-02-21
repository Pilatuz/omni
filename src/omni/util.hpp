///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
/** @file
@brief Auxiliary constants and functions.

		This header file contains declaration of the
	some useful constants and auxiliary functions.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_UTIL_HPP_
#define __OMNI_UTIL_HPP_

#include <omni/defs.hpp>

#include <assert.h>
#include <stdlib.h>

namespace omni
{
	/// @brief Utility.
	/**
			This namespace contains some useful
		constants and auxiliary functions.
	*/
	namespace util
	{
/// @name Constants
/// @{

///////////////////////////////////////////////////////////////////////////////
/// @brief Square root of 2. @hideinitializer
const double SQRT2 = 1.4142135623730950488016887242097;


///////////////////////////////////////////////////////////////////////////////
/// @brief Square root of 3. @hideinitializer
const double SQRT3 = 1.7320508075688772935274463415059;


///////////////////////////////////////////////////////////////////////////////
/// @brief Decimal logarithm of 2. @hideinitializer
const double LG2 = 0.3010299956639811952137388947245;


///////////////////////////////////////////////////////////////////////////////
/// @brief Natural logarithm of 2. @hideinitializer
const double LN2 = 0.6931471805599453094172321214582;


///////////////////////////////////////////////////////////////////////////////
/// @brief The @b Pi value. @hideinitializer
const double PI = 3.1415926535897932384626433832795;

/// @}
	}


	namespace util
	{
/// @name Conversions
/// @{

///////////////////////////////////////////////////////////////////////////////
/// @brief Convert degrees to the radians.
/**
		This function converts the @a deg value
	in degrees to the corresponding value in radians.

@code
	double rad = deg * 3.14/180;
@endcode

@param[in] deg The value in degrees.
@return The value in radians.

@see rad2deg()
*/
double deg2rad(double deg);


///////////////////////////////////////////////////////////////////////////////
/// @brief Convert radians to the degrees.
/**
		This function converts the @a rad value in radians
	to the corresponding value in degrees.

@code
	double deg = rad * 180/3.14;
@endcode

@param[in] rad The value in radians.
@return The value in degrees.

@see deg2rad()
*/
double rad2deg(double rad);


///////////////////////////////////////////////////////////////////////////////
/// @brief Convert @b dB to the value in linear scale.
/**
		This function converts the @a dB value in logarithmic (@b dB) scale
	to the corresponding value in linear scale.

@code
	double L = pow(10, dB/10);
@endcode

@param[in] dB The value in logarithmic scale.
@return The value in linear scale.

@see line2dB()
*/
double dB2line(double dB);


///////////////////////////////////////////////////////////////////////////////
/// @brief Convert value in linear scale to the @b dB.
/**
		This function converts the @a L value in linear scale to the
	corresponding value in logarithmic (@b dB) scale.

@code
	double dB = 10*log10(L);
@endcode

@param[in] L The value in linear scale. Should be positive nonzero value!
@return The value in logarithmic scale.

@see dB2line()
*/
double line2dB(double L);


///////////////////////////////////////////////////////////////////////////////
/// @brief Convert @b dBm to the watts.
/**
		This function converts the @a dBm value in logarithmic (@b dBm) scale
	to the value in watts.

@code
	double W = pow(10, dBm/10) / 1000;
@endcode

@param[in] dBm The value in @b dBm.
@return The value in watts.

@see watt2dBm()
*/
double dBm2watt(double dBm);


///////////////////////////////////////////////////////////////////////////////
/// @brief Convert watts to the @b dBm.
/**
		This function converts the @a W value in watts
	to the value in logarithmic (@b dBm) scale.

@code
	double dBm = 10*log10(W*1000);
@endcode

@param[in] W The value in watts. Should be positive nonzero value!
@return The value in @b dBm.

@see dBm2watt()
*/
double watt2dBm(double W);


///////////////////////////////////////////////////////////////////////////////
/// @brief Convert @b kph to the @b mps.
/**
		This function converts the value in kilometers per hour (@b kph)
	to the value in meters per second (@b mps).

@code
	double mps = kph / 3.6;
@endcode

@param[in] kph The value in @b kph.
@return The value in @b mps.

@see mps2kph()
*/
double kph2mps(double kph);


///////////////////////////////////////////////////////////////////////////////
/// @brief Convert @b mps to the @b kph.
/**
		This function converts the value in meters per second (@b mps)
	to the value in kilometers per hour (@b kph).

@code
	double kph = mps * 3.6;
@endcode

@param[in] mps The value in @b mps.
@return The value in @b kph.

@see kph2mps()
*/
double mps2kph(double mps);

/// @}
	}


	namespace util
	{
/// @name Power of two and parity
/// @{

		/// @brief Utility: implementation.
		namespace details
		{
			/// @brief Type size check.
			template<typename T, size_t shift>
				struct ShiftCheck
			{
				enum
				{
					/// @brief Does it make sense?
					sense = (shift < 8*sizeof(T))
				};
			};


			/// @brief Do the shift.
			template<int sense>
				struct ShiftImpl
			{
				/// @brief Shift right and XOR.
				template<typename T>
				static inline void xor_shr(T &x, size_t shift)
				{
					x ^= (x >> shift);
				}

				/// @brief Shift right and OR.
				template<typename T>
				static inline void or_shr(T &x, size_t shift)
				{
					x |= (x >> shift);
				}
			};

			/// @brief Do nothing.
			template<>
				struct ShiftImpl<0>
			{
				/// @brief Shift right and XOR.
				template<typename T>
				static inline void xor_shr(T&, size_t)
				{} // do nothing

				/// @brief Shift right and OR.
				template<typename T>
				static inline void or_shr(T&, size_t)
				{} // do nothing
			};


			/// @brief The shift helper.
			/**
				This class performs right shift operations on custom type:
					- xor_shr() is equal to x ^= x >> shift;
					-  or_shr() is equal to x |= x >> shift;

					If shift value is greater than or equal to the type size,
				then no actual shift performed.
			*/
			template<size_t shift>
				class Shift
			{
			public:

				/// @brief Shift right and XOR.
				/**
				@param[in,out] x Operand.
				*/
				template<typename T>
				static inline void xor_shr(T &x)
				{
					typedef ShiftCheck<T, shift> Check;
					typedef ShiftImpl<Check::sense> Impl;

					Impl::xor_shr(x, shift);
				}

				/// @brief Shift right and OR.
				/**
				@param[in,out] x Operand.
				*/
				template<typename T>
				static inline void or_shr(T &x)
				{
					typedef ShiftCheck<T, shift> Check;
					typedef ShiftImpl<Check::sense> Impl;

					Impl::or_shr(x, shift);
				}
			};
		}


///////////////////////////////////////////////////////////////////////////////
/// @brief Is integer power of two?
/**
		This functions checks the @a x argument - is it integer power of two?

	The valid integer powers of two are: 0, 1, 2, 4, 8, 16, 32, 64, 128, etc...

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


///////////////////////////////////////////////////////////////////////////////
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
	assert(T(0)<x && is_ipow2(x)
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


///////////////////////////////////////////////////////////////////////////////
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
	details::Shift< 1>::or_shr(x);
	details::Shift< 2>::or_shr(x);
	details::Shift< 4>::or_shr(x);
	details::Shift< 8>::or_shr(x);
	details::Shift<16>::or_shr(x);
	details::Shift<32>::or_shr(x);

	return x - (x>>1);
}


///////////////////////////////////////////////////////////////////////////////
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

	details::Shift< 1>::or_shr(x);
	details::Shift< 2>::or_shr(x);
	details::Shift< 4>::or_shr(x);
	details::Shift< 8>::or_shr(x);
	details::Shift<16>::or_shr(x);
	details::Shift<32>::or_shr(x);

	return x + 1;
}


///////////////////////////////////////////////////////////////////////////////
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
	details::Shift<32>::xor_shr(x);
	details::Shift<16>::xor_shr(x);
	details::Shift< 8>::xor_shr(x);
	details::Shift< 4>::xor_shr(x);
	details::Shift< 2>::xor_shr(x);
	details::Shift< 1>::xor_shr(x);

	return (x&1);
}
/// @}

	}


	namespace util
	{
/// @name Bits packing/unpacking and flip
/// @{

///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
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

/// @}
	}


	namespace util
	{
/// @name Polynomials
/// @{

///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
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

/// @}
	}

} // omni namespace

#endif // __OMNI_UTIL_HPP_
