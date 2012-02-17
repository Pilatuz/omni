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
	namespace util
	{

///////////////////////////////////////////////////////////////////////////////
double deg2rad(double deg)
{
	return deg * (PI/180.0);
}


///////////////////////////////////////////////////////////////////////////////
double rad2deg(double rad)
{
	return rad * (180.0/PI);
}


///////////////////////////////////////////////////////////////////////////////
double dB2line(double dB)
{
	return pow(10.0, 0.1*dB);
}


///////////////////////////////////////////////////////////////////////////////
double line2dB(double L)
{
	assert(0.0 < L && "line2dB() "
		"argument should be positive");
	return 10.0 * log10(L);
}


///////////////////////////////////////////////////////////////////////////////
double dBm2watt(double dBm)
{
	return pow(10.0, 0.1*dBm - 3.0);
}


///////////////////////////////////////////////////////////////////////////////
double watt2dBm(double W)
{
	assert(0.0 < W && "watt2dBm() "
		"argument should be positive");
	return 10.0 * log10(W) + 30.0;
}


///////////////////////////////////////////////////////////////////////////////
double kph2mps(double kph)
{
	return kph * 10.0/36.0;
}


///////////////////////////////////////////////////////////////////////////////
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
