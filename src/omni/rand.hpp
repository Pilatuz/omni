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
	@brief Random number generators.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_RAND_HPP_
#define __OMNI_RAND_HPP_

#include <omni/defs.hpp>
#include <complex>

namespace omni
{
	// auxiliary
	namespace rnd
	{

/// @brief The main random value type.
typedef size_t RandomValue;

RandomValue rand(RandomValue lo, RandomValue up);
RandomValue rand(RandomValue up);
RandomValue rand();

RandomValue rand_max();
void srand(RandomValue seed);
RandomValue randomize();

double runif(double lo, double up);
double runif(double up);
double runif();

double rnorm(double mean, double stdev);
double rnorm(double stdev);
double rnorm();

std::complex<double> wgn(double stdev);

double rexp(double stdev);
double rexp();

	} // auxiliary


	// Random
	namespace rnd
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The discrete PRS generator.
/**
		This class represents a discrete pseudo random sequence (PRS) generator.

		The returned numbers are uniform distributed
	random values in range [0, rand_max()].

		The generator has a seed value.

@see http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
@see M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally
	Equidistributed Uniform Pseudo-Random Number Generator",
	ACM Transactions on Modeling and Computer Simulation,
	Vol. 8, No. 1, January 1998, pp 3--30.
*/
class Random {
public:
	// value_type must be unsigned integer (at least 32 bit)
	typedef RandomValue value_type;  ///< @brief The value type.
	typedef value_type seed_type;    ///< @brief The seed type.

public:
	Random();
	explicit Random(seed_type seed);

public:
	value_type operator()(value_type lo, value_type up);
	value_type operator()(value_type up);
	value_type operator()();

	static value_type rand_max();

private:
	void srand(seed_type seed);
	void reload();

	enum { N = 624 };
	value_type m_rand[N];
	size_t     m_curr;
};

	} // Random


	// Uniform
	namespace rnd
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The uniform distributed random numbers generator.
/**
	The random numbers are floating point numbers.
*/
class Uniform: public Random {
	typedef Random inherited;
public:
	typedef double value_type; ///< @brief The value type.

public:
	Uniform();
	explicit Uniform(seed_type seed);

public:
	value_type operator()(value_type lo, value_type up);
	value_type operator()(value_type up);
	value_type operator()();
};

	} // Uniform


	// Normal
	namespace rnd
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The normal distributed random numbers generator.
/**
	The random numbers are floating point numbers.
*/
class Normal: public Uniform {
	typedef Uniform inherited;
public:
	typedef double value_type; ///< @brief The value type.

public:
	Normal();
	explicit Normal(seed_type seed);

public:
	value_type operator()(value_type mean, value_type stdev);
	value_type operator()(value_type stdev);
	value_type operator()();

private:
	value_type m_buf;
	bool m_buf_empty;
};

	} // Normal


	// Exponential
	namespace rnd
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The exponential distributed random numbers generator.
/**
	The random numbers are floating point numbers.

	The mean is equal to the standard deviation.
*/
class Exponential: public Uniform {
	typedef Uniform inherited;
public:
	typedef double value_type; ///< @brief The value type.

public:
	Exponential();
	explicit Exponential(seed_type seed);

public:
	value_type operator()(value_type stdev);
	value_type operator()();
};

	} // Exponential interface

} // omni namespace

#endif // __OMNI_RAND_HPP_
