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
	@brief Auxiliary constants and functions.

		This source file contains definition of the
	some useful constants and auxiliary functions.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include <omni/util.hpp>

#include <math.h>

namespace omni
{

//////////////////////////////////////////////////////////////////////////
/// @brief Auxiliary constants and functions.
/**
		This namespace contains some useful constants and auxiliary functions.
*/
	namespace util
	{

//////////////////////////////////////////////////////////////////////////
/**
		This function converts the @a deg value in degrees
	to the corresponding value in radians.

@code
	double rad = deg * 3.14/180;
@endcode

@param[in] deg The value in degrees.
@return The value in radians.

@see rad2deg()
*/
double deg2rad(double deg)
{
	return deg * (PI/180.0);
}


//////////////////////////////////////////////////////////////////////////
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
double rad2deg(double rad)
{
	return rad * (180.0/PI);
}


//////////////////////////////////////////////////////////////////////////
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
double dB2line(double dB)
{
	return pow(10.0, 0.1*dB);
}


//////////////////////////////////////////////////////////////////////////
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
double line2dB(double L)
{
	assert(0.0 < L && "line2dB() "
		"argument should be positive");
	return 10.0 * log10(L);
}


//////////////////////////////////////////////////////////////////////////
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
double dBm2watt(double dBm)
{
	return pow(10.0, 0.1*dBm - 3.0);
}


//////////////////////////////////////////////////////////////////////////
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
double watt2dBm(double W)
{
	assert(0.0 < W && "watt2dBm() "
		"argument should be positive");
	return 10.0 * log10(W) + 30.0;
}


//////////////////////////////////////////////////////////////////////////
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
double kph2mps(double kph)
{
	return kph * 10.0/36.0;
}


//////////////////////////////////////////////////////////////////////////
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
double mps2kph(double mps)
{
	return mps * 36.0/10.0;
}

	} // util namespace
} // omni namespace


//////////////////////////////////////////////////////////////////////////
// draft or unused code...

//    enmap     demap
// 0   ->   +1   ->   0
// 1   ->   -1   ->   1

//// bit enmap
//template<typename T> inline
//	T bit_enmap(T x)
//{
//	assert((0==x || 1==x)
//		&& "invalid argument");
//	return 1 - 2*x;
//}

//// bit demap
//template<typename T> inline
//	T bit_demap(T x)
//{
//	assert((-1==x || +1==x)
//		&& "invalid argument");
//	return (1 - x)/2;
//}

//// gray encode
//template<typename T>
//	T gray_encode(T g)
//{
//	return g ^ (g >> 1);
//}

//// gray decode
//template<typename T>
//	T gray_decode(T g)
//{
//	T b = T();
//
//	for (; g; g >>= 1)
//		b ^= g;
//
//	return b;
//}
