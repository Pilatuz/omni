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
	@brief Random number generators.

@author Sergey Polichnoy
*/
#include <omni/rand.hpp>
#include <omni/util.hpp>

#if OMNI_MT
#	include <omni/sync.hpp>
#endif // OMNI_MT

#include <assert.h>
#include <math.h>
#include <time.h>

// global generators
namespace
{
	using namespace omni::rnd;

//////////////////////////////////////////////////////////////////////////
// @brief The global discrete PRS generator.
Random& g_rand()
{
	static Random DEF;
	return DEF;
}


//////////////////////////////////////////////////////////////////////////
// @brief The global uniform distributed PRS generator.
Uniform& g_unif()
{
	static Uniform DEF;
	return DEF;
}


//////////////////////////////////////////////////////////////////////////
// @brief The global normal distributed PRS generator.
Normal& g_norm()
{
	static Normal DEF;
	return DEF;
}


//////////////////////////////////////////////////////////////////////////
// @brief The global exponential distributed PRS generator.
Exponential& g_exp()
{
	static Exponential DEF;
	return DEF;
}


#if OMNI_MT
//////////////////////////////////////////////////////////////////////////
// @brief The global critical section.
omni::sync::Lockable& g_lock()
{
	static omni::sync::CriticalSection LOCK;
	return LOCK;
}
#endif // OMNI_MT


//////////////////////////////////////////////////////////////////////////
// @brief The default seed value.
RandomValue& g_seed()
{
	static RandomValue SEED = 0;
	return SEED;
}

} // global generators


namespace omni
{

//////////////////////////////////////////////////////////////////////////
/// @brief Random number generators.
	namespace rnd
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Get random number in specified range.
/**
		This function generates the random number in range [@a lo, @a up).

	The @a lo argument must be less than @a hi!

@param lo The lower bound (inclusive).
@param up The upper bound (exclusive).
@return The random number.
*/
RandomValue rand(RandomValue lo, RandomValue up)
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_rand()(lo, up);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get random number less than specified.
/**
		This function generates the random number in range [0, @a up).

@param up The upper bound (exclusive). Can't be zero!
@return The random number.
*/
RandomValue rand(RandomValue up)
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_rand()(up);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get random number.
/**
		This function generates the random number in range [0, rand_max()].

@return The random number.
*/
RandomValue rand()
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_rand()();
}


//////////////////////////////////////////////////////////////////////////
/// @brief The maximum random value.
/**
		This function returns the maximum possible random value.

@return The maximum possible random value.
*/
RandomValue rand_max()
{
	return g_rand().rand_max();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set a new seed value.
/**
		This function initializes all global generators by @a seed value.

@param seed The seed value of all generators.
*/
void srand(RandomValue seed)
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));

	g_rand() = Random(seed);
	g_unif() = Uniform(seed);
	g_norm() = Normal(seed);
	g_exp() = Exponential(seed);

	g_seed() = seed;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Randomize all generators.
/**
		This function initializes all generators by system time value.

@return The new seed value.
*/
RandomValue randomize()
{
	RandomValue seed = RandomValue(time(0));

	srand(seed);
	return seed;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the uniform distributed random number in specified range.
/**
		This function generates the uniform distributed random number
	in range [@a lo, @a up].

@param lo Lower bound (inclusive)
@param up Upper bound (inclusive)
@return The random number.
*/
double runif(double lo, double up)
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_unif()(lo, up);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the uniform distributed random number less than specified.
/**
		This function generates the uniform distributed random number
	in range [0, @a up].

@param up Upper bound (inclusive).
@return The random number.
*/
double runif(double up)
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_unif()(up);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate uniform distributed random number in range [0,1].
/**
		This function generates the uniform distributed random number
	in range [0,1].

@return The random number.
*/
double runif()
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_unif()();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the normal distributed random number.
/**
		This method generates the normal distributed random number
	with standard deviation @a stdev and mean @a mean.

@param mean The mean value.
@param stdev The standard deviation.
@return The random number.
*/
double rnorm(double mean, double stdev)
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_norm()(mean, stdev);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the normal distributed random number.
/**
		This method generates the normal distributed random number
	with standard deviation @a stdev and zero mean.

@param stdev The standard deviation.
@return The random number.
*/
double rnorm(double stdev)
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_norm()(stdev);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the normal distributed random number.
/**
		This method generates the normal distributed random
	number with unit standard deviation and zero mean.

@return The random number.
*/
double rnorm()
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_norm()();
}



//////////////////////////////////////////////////////////////////////////
/// @brief Get WGN sample.
/**
		This function returns the White Gaussian Noise (WGN)
	sample with specified standard deviation @a stdev.

	The standard deviation specified for whole complex sample.

@param stdev The standard deviation.
@return The WGN sample.
*/
std::complex<double> wgn(double stdev)
{
	double re, im, nrm;

	do {
		re = rnd::runif(-1.0, +1.0);
		im = rnd::runif(-1.0, +1.0);
		nrm = re*re + im*im;
	} while (0.0==nrm || 1.0<=nrm);

	nrm = stdev * sqrt(-log(nrm) / nrm);
	return std::complex<double>(re*nrm, im*nrm);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate exponential distributed random number.
/**
		This function generates exponential distributed random
	number with standard deviation @a stdev.

@param stdev The standard deviation.
@return The random number.
*/
double rexp(double stdev)
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_exp()(stdev);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate exponential distributed random number.
/**
		This function generates exponential distributed
	random number with unit standard deviation.

@return The random number.
*/
double rexp()
{
	OMNI_MT_CODE(sync::Locker guard(g_lock()));
	return g_exp()();
}

	} // auxiliary functions


	// Random
	namespace rnd
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		The default constructor initializes the PRS by 5489 seed value.
*/
Random::Random()
{
	srand(0); // (!) see srand() method
}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
		The constructor initializes the PRS by @a seed value.

@param seed The seed value of the PRS.
*/
Random::Random(seed_type seed)
{
	srand(seed);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The maximum random value.
/**
		This static method returns the maximum possible random value.

@return The maximum possible random value.
*/
Random::value_type Random::rand_max()
{
	return 0xFFFFFFFFUL;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random number in specified range.
/**
		This method generates the random number in range [@a lo, @a up).

	The @a lo argument must be less than the @a up argument!

@param lo Lower bound (inclusive)
@param up Upper bound (exclusive)
@return The random number.
*/
Random::value_type Random::operator()(value_type lo, value_type up)
{
	assert(lo < up && "lower bound must be less than upper bound");
	return lo + (*this)(up - lo);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random number.
/**
		This method generates the random number in range [0, @a up).

@param up Upper bound (exclusive). Can't be zero.
@return The random number.
*/
Random::value_type Random::operator()(value_type up)
{
	assert(0!=up && "upper bound can't be zero");

	return (*this)() % up;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random number.
/**
		This method generates the random number in range [0, rand_max()].

@return The random number.
*/
Random::value_type Random::operator()()
{
	if (N <= m_curr)
		reload();

	value_type y = m_rand[m_curr++];

	// tempering
	y ^= (y >> 11);
	y ^= (y <<  7) & 0x9D2C5680UL;
	y ^= (y << 15) & 0xEFC60000UL;
	y ^= (y >> 18);

	return y & 0xFFFFFFFFUL;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Set seed value.
/**
		This method initializes the PRS by @a seed value.

@param seed The seed value of the PRS.
*/
void Random::srand(seed_type seed)
{
	assert(4 <= sizeof(seed_type)); // (!) STATIC_ASSERT

	m_rand[0]= seed ? seed : 5489;
	for (size_t i = 1; i < N; ++i)
	{
		// See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier
		m_rand[i] = 1812433253UL * (m_rand[i-1] ^ (m_rand[i-1] >> 30)) + i;
	}

	m_curr = N;
}


//////////////////////////////////////////////////////////////////////////
// generate N words at one time
void Random::reload()
{
	const value_type UP_MASK = 0x80000000UL; // most significant w-r bits
	const value_type LO_MASK = 0x7FFFFFFFUL; // least significant r bits
	const value_type A = 0x9908B0DFUL;

	enum { M = 397 };

	for (size_t i = 0; i < N-M; ++i)
	{
		value_type y = (m_rand[i]&UP_MASK) | (m_rand[i+1]&LO_MASK);
		m_rand[i] = m_rand[i+M] ^ (y >> 1) ^ (y&1 ? A : 0UL);
	}

	for (size_t i = N-M; i < N-1; ++i)
	{
		value_type y = (m_rand[i]&UP_MASK) | (m_rand[i+1]&LO_MASK);
		m_rand[i] = m_rand[i+M-N] ^ (y >> 1) ^ (y&1 ? A : 0UL);
	}

	value_type y = (m_rand[N-1]&UP_MASK) | (m_rand[0]&LO_MASK);
	m_rand[N-1] = m_rand[M-1] ^ (y >> 1) ^ (y&1 ? A : 0UL);

	m_curr = 0;
}

	} // Random


	// Uniform
	namespace rnd
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		The default constructor uses the seed value
	from the global Random generator.
*/
Uniform::Uniform()
	: inherited(g_seed())
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
		This constructor initializes the generator by @a seed value.

@param seed The seed value.
*/
Uniform::Uniform(seed_type seed)
	: inherited(seed)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random value in specified range.
/**
		This method generates the random number in range [@a lo, @a up].

@param lo Lower bound (inclusive)
@param up Upper bound (inclusive)
@return The random number.
*/
Uniform::value_type Uniform::operator()(value_type lo, value_type hi)
{
	// assert(lo < hi && "lower bound must be less than upper bound");
	return lo + (hi-lo)*(*this)();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random number less than specified.
/**
		This method generates the random number in range [0, @a up].

@param up Upper bound (inclusive).
@return The random number.
*/
Uniform::value_type Uniform::operator()(value_type up)
{
	return (*this)() * up;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random number in range [0,1].
/**
		This method generates the random number in range [0,1].

@return The random number.
*/
Uniform::value_type Uniform::operator()()
{
	const inherited::value_type a = inherited::operator()() >> 5;
	const inherited::value_type b = inherited::operator()() >> 6;
	return (a*67108864.0+b) * (1.0/9007199254740991.0);
}

	} // Uniform


	// Normal
	namespace rnd
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		The default constructor uses the seed value
	from the global Random generator.
*/
Normal::Normal()
	: inherited(g_seed()),
	  m_buf_empty(true)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
		This constructor initializes the generator by @a seed value.

@param seed The seed value.
*/
Normal::Normal(seed_type seed)
	: inherited(seed),
	  m_buf_empty(true)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random value.
/**
		This method generates the normal distributed random number with
	standard deviation @a stdev and mean @a mean.

@param mean The mean value.
@param stdev The standard deviation.
@return The random number.
*/
Normal::value_type Normal::operator()(value_type mean, value_type stdev)
{
	return mean + stdev*(*this)();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random value.
/**
		This method generates the normal distributed random number with
	standard deviation @a stdev and zero mean.

@param stdev The standard deviation.
@return The random number.
*/
Normal::value_type Normal::operator()(value_type stdev)
{
	return stdev * (*this)();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random number.
/**
		This method generates the normal distributed random number with
	unit standard deviation and zero mean.

@return The random number.
*/
Normal::value_type Normal::operator()()
{
	if (m_buf_empty)
	{
		value_type x;

		do { x = inherited::operator()(); }
		while (value_type() == x);

		const value_type z = sqrt(-2.0 * log(x));
		const value_type n = inherited::operator()();

		m_buf = z * sin(2.0*util::PI * n);
		m_buf_empty = false;

		return z * cos(2.0*util::PI * n);
	}
	else
	{
		m_buf_empty = true;
		return m_buf;
	}
}

	} // Normal


	// Exponential
	namespace rnd
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		The default constructor uses the seed value
	from the global Random generator.
*/
Exponential::Exponential()
	: inherited(g_seed())
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
		This constructor initializes the generator by @a seed value.

@param seed The seed value.
*/
Exponential::Exponential(seed_type seed)
	: inherited(seed)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random number.
/**
		This method generates exponential distributed random number with
	standard deviation @a stdev.

@param stdev The standard deviation.
@return The random number.
*/
Exponential::value_type Exponential::operator()(value_type stdev)
{
	return stdev * (*this)();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Generate the random value.
/**
		This method generates exponential distributed random number with
	unit standard deviation.

@return The random number.
*/
Exponential::value_type Exponential::operator()()
{
	value_type x;

	do { x = inherited::operator()(); }
	while (value_type() == x);

	return -log(x);
}

	} // Exponential

} // omni namespace
