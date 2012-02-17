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
	@brief Implementation of the TurboCodec class.

@see @ref omni_turbo
@author Sergey Polichnoy
*/
#include <omni/turbo.h>

#include <math.h>


namespace omni
{
	namespace dsp
	{


//////////////////////////////////////////////////////////////////////////
// LogMAP metric
double LogMAP::operator()(double x, double y) const
{
	double z = log(1.0 + exp(-fabs(x-y)));
	return ((y < x) ? x : y) + z;
}


//////////////////////////////////////////////////////////////////////////
// MaxLogMAP metric
double MaxLogMAP::operator()(double x, double y) const
{
	return (y < x) ? x : y;
}
	} 


	// Turbo_WiMAX
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
// default constructor
Turbo_WiMAX::Turbo_WiMAX()
	: m_Niterations(8), m_Nsubiterations(6),
	  m_trellis("0x01 0x03", "0x08 0x08", "0x08 0x0E", 4, 0x05)
{}


//////////////////////////////////////////////////////////////////////////
// constructor
Turbo_WiMAX::Turbo_WiMAX(size_t Niterations, size_t Nsubiterations)
	: m_Niterations(Niterations), m_Nsubiterations(Nsubiterations),
	  m_trellis("0x01 0x03", "0x08 0x08", "0x08 0x0E", 4, 0x05)
{}


//////////////////////////////////////////////////////////////////////////
const Trellis& Turbo_WiMAX::trellis() const
{
	return m_trellis;
}


//////////////////////////////////////////////////////////////////////////
Turbo_WiMAX::size_type Turbo_WiMAX::N_subiterations() const
{
	return m_Niterations;
}

//////////////////////////////////////////////////////////////////////////
Turbo_WiMAX::size_type Turbo_WiMAX::N_iterations() const
{
	return m_Nsubiterations;
}


//////////////////////////////////////////////////////////////////////////
// Alpha default construction
Turbo_WiMAX::Alpha::Alpha()
{
	memset(m_A, 0, sizeof(m_A));
}

//////////////////////////////////////////////////////////////////////////
double Turbo_WiMAX::Alpha::operator[](state_type state) const
{
	assert(state < NO_STATES && "invalid state");
	return m_A[state];
}

//////////////////////////////////////////////////////////////////////////
double& Turbo_WiMAX::Alpha::operator[](state_type state)
{
	assert(state < NO_STATES && "invalid state");
	return m_A[state];
}


//////////////////////////////////////////////////////////////////////////
// Beta default construction
Turbo_WiMAX::Beta::Beta()
{
	memset(m_B, 0, sizeof(m_B));
}

//////////////////////////////////////////////////////////////////////////
double Turbo_WiMAX::Beta::operator[](state_type state) const
{
	assert(state < NO_STATES && "invalid state");
	return m_B[state];
}

//////////////////////////////////////////////////////////////////////////
double& Turbo_WiMAX::Beta::operator[](state_type state)
{
	assert(state < NO_STATES && "invalid state");
	return m_B[state];
}


//////////////////////////////////////////////////////////////////////////
// Gamma default construction
Turbo_WiMAX::Gamma::Gamma()
{
	memset(m_G, 0, sizeof(m_G));
}

//////////////////////////////////////////////////////////////////////////
double Turbo_WiMAX::Gamma::operator[](bits_type abwy) const
{
	assert(abwy < NO_BRANCH && "invalid ABWY");
	return m_G[abwy];
}

//////////////////////////////////////////////////////////////////////////
double& Turbo_WiMAX::Gamma::operator[](bits_type abwy)
{
	assert(abwy < NO_BRANCH && "invalid ABWY");
	return m_G[abwy];
}


//////////////////////////////////////////////////////////////////////////
// Duo-binary LLR
Turbo_WiMAX::DuoLLR::DuoLLR()
{
	memset(m_L, 0, sizeof(m_L));
}

//////////////////////////////////////////////////////////////////////////
Turbo_WiMAX::DuoLLR& Turbo_WiMAX::DuoLLR::operator+=(const DuoLLR &x)
{
	m_L[A0B0] += x.m_L[A0B0];
	m_L[A0B1] += x.m_L[A0B1];
	m_L[A1B0] += x.m_L[A1B0];
	m_L[A1B1] += x.m_L[A1B1];

	return *this;
}

//////////////////////////////////////////////////////////////////////////
Turbo_WiMAX::DuoLLR& Turbo_WiMAX::DuoLLR::operator-=(const DuoLLR &x)
{
	m_L[A0B0] -= x.m_L[A0B0];
	m_L[A0B1] -= x.m_L[A0B1];
	m_L[A1B0] -= x.m_L[A1B0];
	m_L[A1B1] -= x.m_L[A1B1];

	return *this;
}

//////////////////////////////////////////////////////////////////////////
Turbo_WiMAX::DuoLLR& Turbo_WiMAX::DuoLLR::operator+=(double x)
{
	m_L[A0B0] += x;
	m_L[A0B1] += x;
	m_L[A1B0] += x;
	m_L[A1B1] += x;

	return *this;
}

//////////////////////////////////////////////////////////////////////////
Turbo_WiMAX::DuoLLR& Turbo_WiMAX::DuoLLR::operator-=(double x)
{
	m_L[A0B0] -= x;
	m_L[A0B1] -= x;
	m_L[A1B0] -= x;
	m_L[A1B1] -= x;

	return *this;
}


//////////////////////////////////////////////////////////////////////////
double Turbo_WiMAX::DuoLLR::operator[](size_t ab) const
{
	assert(ab < (1U<<NO_IBITS) && "invalid AB");
	return m_L[ab];
}

//////////////////////////////////////////////////////////////////////////
double& Turbo_WiMAX::DuoLLR::operator[](size_t ab)
{
	assert(ab < (1U<<NO_IBITS) && "invalid AB");
	return m_L[ab];
}


//////////////////////////////////////////////////////////////////////////
Turbo_WiMAX::state_type Turbo_WiMAX::start_state(size_type blockSize, state_type end_state)
{
	// Table 328 from IEEE 802.16-2004
	static const state_type TABLE_308[6][8] =
	{
		{ 0, 6, 4, 2, 7, 1, 3, 5 },
		{ 0, 3, 7, 4, 5, 6, 2, 1 },
		{ 0, 5, 3, 6, 2, 7, 1, 4 },
		{ 0, 4, 1, 5, 6, 2, 7, 3 },
		{ 0, 2, 5, 7, 1, 3, 4, 6 },
		{ 0, 7, 6, 1, 3, 4, 5, 2 }
	};

	const size_type N = blockSize/2;

	assert(end_state < NO_STATES && "invalid end state");
	assert(0 < N%7 && "invalid block size");

	return TABLE_308[N%7 - 1][end_state];
}


//////////////////////////////////////////////////////////////////////////
void Turbo_WiMAX::make_irule(size_type blockSize) const
{
	if (m_irule.size() == blockSize)
		return; // (!)

	const size_type N = blockSize/2;
	size_type P0=0, P1=0, P2=0, P3=0;

	// TABLE 327
	switch (N)
	{
		case   24: P0 =  5; P1 =   0; P2 =   0; P3 =   0; break;
		case   36: P0 = 11; P1 =  18; P2 =   0; P3 =  18; break;
		case   48: P0 = 13; P1 =  24; P2 =   0; P3 =  24; break;
		case   72: P0 = 11; P1 =   6; P2 =   0; P3 =   6; break;
		case   96: P0 =  7; P1 =  48; P2 =  24; P3 =  72; break;
		case  108: P0 = 11; P1 =  54; P2 =  56; P3 =   2; break;
		case  120: P0 = 13; P1 =  60; P2 =   0; P3 =  60; break;
		case  144: P0 = 17; P1 =  74; P2 =  72; P3 =   2; break;
		case  180: P0 = 11; P1 =  90; P2 =   0; P3 =  90; break;
		case  192: P0 = 11; P1 =  96; P2 =  48; P3 = 144; break;
		case  216: P0 = 13; P1 = 108; P2 =   0; P3 = 108; break;
		case  240: P0 = 13; P1 = 120; P2 =  60; P3 = 180; break;
		case  480: P0 = 53; P1 =  62; P2 =  12; P3 =   2; break;
		case  960: P0 = 43; P1 =  64; P2 = 300; P3 = 824; break;
		case 1440: P0 = 43; P1 = 720; P2 = 360; P3 = 540; break;
		case 1920: P0 = 31; P1 =   8; P2 =  24; P3 =  16; break;
		case 2400: P0 = 53; P1 =  66; P2 =  24; P3 =   2; break;
		default: assert(!"unknown data block size");
	}

	// make rule
	m_irule.resize(blockSize);
	for (size_type j = 0; j < N; ++j)
	{
		size_type i = 0;

		switch (j%4)
		{
			case 0: i = (P0*j + 1)%N;            break;
			case 1: i = (P0*j + 1 + N/2 + P1)%N; break;
			case 2: i = (P0*j + 1 + P2)%N;       break;
			case 3: i = (P0*j + 1 + N/2 + P3)%N; break;
		}

		if (j%2)
		{
			m_irule[2*j + 0] = 2*i + 0;
			m_irule[2*j + 1] = 2*i + 1;
		}
		else
		{
			m_irule[2*j + 0] = 2*i + 1;
			m_irule[2*j + 1] = 2*i + 0;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void Turbo_WiMAX::interleave(size_type Ncouples, const DuoLLR *Li, DuoLLR *Lo) const
{
	for (size_type k = 0; k < Ncouples; ++k)
	{
		const size_type ind = m_irule[2*k];

		Lo[k][A0B0] = Li[ind/2][A0B0];
		Lo[k][A1B1] = Li[ind/2][A1B1];
		if (ind % 2)
		{
			Lo[k][A1B0] = Li[ind/2][A0B1];
			Lo[k][A0B1] = Li[ind/2][A1B0];
		}
		else
		{
			Lo[k][A0B1] = Li[ind/2][A0B1];
			Lo[k][A1B0] = Li[ind/2][A1B0];
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void Turbo_WiMAX::deinterleave(size_type Ncouples, const DuoLLR *Li, DuoLLR *Lo) const
{
	for (size_type k = 0; k < Ncouples; ++k)
	{
		const size_type ind = m_irule[2*k];

		Lo[ind/2][A0B0] = Li[k][A0B0];
		Lo[ind/2][A1B1] = Li[k][A1B1];
		if (ind % 2)
		{
			Lo[ind/2][A1B0] = Li[k][A0B1];
			Lo[ind/2][A0B1] = Li[k][A1B0];
		}
		else
		{
			Lo[ind/2][A0B1] = Li[k][A0B1];
			Lo[ind/2][A1B0] = Li[k][A1B0];
		}
	}
}

	} // Turbo_WiMAX


	// Turbo_WCDMA
	namespace dsp
	{


//////////////////////////////////////////////////////////////////////////
// default constructor
Turbo_WCDMA::Turbo_WCDMA()
	: m_Niterations(8),
	  m_trellis("0x0D", "0x08", 4, 0x03)
{}


//////////////////////////////////////////////////////////////////////////
// constructor
Turbo_WCDMA::Turbo_WCDMA(size_t Niterations)
	: m_Niterations(Niterations),
	  m_trellis("0x0D", "0x08", 4, 0x03)
{}


//////////////////////////////////////////////////////////////////////////
const Trellis& Turbo_WCDMA::trellis() const
{
	return m_trellis;
}


//////////////////////////////////////////////////////////////////////////
Turbo_WCDMA::size_type Turbo_WCDMA::N_iterations() const
{
	return m_Niterations;
}


//////////////////////////////////////////////////////////////////////////
// Alpha default construction
Turbo_WCDMA::Alpha::Alpha()
{
	memset(m_A, 0, sizeof(m_A));
}

//////////////////////////////////////////////////////////////////////////
double Turbo_WCDMA::Alpha::operator[](state_type state) const
{
	assert(state < NO_STATES && "invalid state");
	return m_A[state];
}

//////////////////////////////////////////////////////////////////////////
double& Turbo_WCDMA::Alpha::operator[](state_type state)
{
	assert(state < NO_STATES && "invalid state");
	return m_A[state];
}


//////////////////////////////////////////////////////////////////////////
// Beta default construction
Turbo_WCDMA::Beta::Beta()
{
	memset(m_B, 0, sizeof(m_B));
}

//////////////////////////////////////////////////////////////////////////
double Turbo_WCDMA::Beta::operator[](state_type state) const
{
	assert(state < NO_STATES && "invalid state");
	return m_B[state];
}

//////////////////////////////////////////////////////////////////////////
double& Turbo_WCDMA::Beta::operator[](state_type state)
{
	assert(state < NO_STATES && "invalid state");
	return m_B[state];
}


//////////////////////////////////////////////////////////////////////////
// Gamma default construction
Turbo_WCDMA::Gamma::Gamma()
{
	memset(m_G, 0, sizeof(m_G));
}

//////////////////////////////////////////////////////////////////////////
double Turbo_WCDMA::Gamma::operator[](bits_type xz) const
{
	assert(xz < NO_BRANCH && "invalid XZ");
	return m_G[xz];
}

//////////////////////////////////////////////////////////////////////////
double& Turbo_WCDMA::Gamma::operator[](bits_type xz)
{
	assert(xz < NO_BRANCH && "invalid XZ");
	return m_G[xz];
}


//////////////////////////////////////////////////////////////////////////
void Turbo_WCDMA::make_irule(size_type blockSize) const
{
	if (m_irule.size() == blockSize)
		return; // (!)

	struct Aux
	{
		// mod operation
		static int mod(int k, int n)
		{
			if (0 == n)
				return k;
			else
				return (k - n * (int)floor((double)k/n) );
		}

		// gcd operation
		static int gcd(int a, int b)
		{
			while (0 < b)
			{
				int q = a / b;
				int t = a - b*q;
				a = b; b = t;
			}

			return a;
		}
	};

	typedef std::vector<int, pool::Allocator<int> > int_vector;

	// primes and associated primitive roots:
	static const int PRIMES[] =
	{
		  2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,
		 37, 41,   43,  47,  53,  59,  61,  67,  71,  73,  79,
		 83,  89,  97, 101, 103, 107, 109, 113, 127, 131, 137,
		139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193,
		197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257
	};
	static const int ROOTS[] =
	{
		0, 0, 0, 3, 2, 2, 3, 2, 5,  2, 3,
		2, 6, 3, 5, 2, 2, 2, 2, 7,  5, 3,
		2, 3, 5, 2, 5, 2, 6, 3, 3,  2, 3,
		2, 2, 6, 5, 2, 5, 2, 2, 2, 19, 5,
		2, 3, 2, 3, 2, 6, 3, 7, 7,  6, 3
	};
	const size_t N_PRIMES = sizeof(PRIMES)/sizeof(PRIMES[0]);


	// check the range of the interleaver size
	assert(40<=blockSize && blockSize<=5114
		&& "Invalid data block size");

	size_type Nrows;  // number of rows of rectangular matrix
	size_type Ncols;  // number of columns of rectangular matrix
	int p = 0;  // prime number
	int v = 0;  // primitive root

	// determine Nrows
	if (40<=blockSize && blockSize<=159)
		Nrows = 5;
	else if (160<=blockSize && blockSize<=200)
		Nrows = 10;
	else if (481<=blockSize && blockSize<=530)
		Nrows = 10;
	else
		Nrows = 20;

	// determine Ncols
	if (481<=blockSize && blockSize<=530)
	{
		p = 53;
		v = 2;
		Ncols = p;
	}
	else
	{
		// find minimum prime p such that (p+1) - blockSize/Nrows >= 0 ...
		for (size_type i = 0; i < N_PRIMES; ++i)
			if (0.0 <= (PRIMES[i]+1.0 - double(blockSize)/Nrows))
			{
				p = PRIMES[i];
				v = ROOTS[i];
				break;
			}

		// ... and determine Ncols such that
		if (0.0 <= (p - double(blockSize)/Nrows))
		{
			if (0.0 <= (p-1.0 - double(blockSize)/Nrows))
				Ncols = p-1;
			else
				Ncols = p;
		}
		else
		{
			Ncols = p+1;
		}
	}

	// construct the base sequence s for intra-row permutations
	int_vector s(p-1, 0); // base sequence for intra-row permutation
	s[0] = 1;
	for (int i = 1; i < p-1; ++i)
		s[i] = Aux::mod(v*s[i-1], p);

	// Let q[0] = 1 be the first prime integer in {q[j]}, and select the consecutive
	// minimum prime integers {q[j]}, j = 1, 2, ..., (Nrows-1) such that gcd( q[j], p-1)==1, q[j] > 6, and q[j] > q[j-1]
	int_vector q(Nrows, 0); // minimum prime integers
	q[0] = 1;
	for (size_type j = 1; j < Nrows; ++j)
		for (size_type i = 0; i < N_PRIMES; ++i)
		{
			const int qj = PRIMES[i];
			if (6<qj && q[j-1]<qj)
				if (1 == Aux::gcd(qj, p-1))
				{
					q[j] = qj;
					break;
				}
		}

	const size_type *T; // inter-row permutation pattern
	{ // T - inter-row permutation
		static const size_type P1[] = {19, 9, 14, 4, 0, 2, 5, 7, 12, 18, 10, 8, 13, 17, 3, 1, 16, 6, 15, 11};
		static const size_type P2[] = {19, 9, 14, 4, 0, 2, 5, 7, 12, 18, 16, 13, 17, 15, 3, 1, 6, 11, 8, 10};
		static const size_type P3[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
		static const size_type P4[] = {4, 3, 2, 1, 0};

		// T[j] is the inter-row permutation patters defined as one of the following four
		// kinds of patterns: P1, P2, P3, and P4 depending on the number of input bits
		if (3211 <= blockSize)
			T = P1;
		else if (3161 <= blockSize)
			T = P2;
		else if (2481 <= blockSize)
			T = P1;
		else if (2281 <= blockSize)
			T = P2;
		else if (531 <= blockSize)
			T = P1;
		else if (481 <= blockSize)
			T = P3;
		else if (201 <= blockSize)
			T = P1;
		else if (160 <= blockSize)
			T = P3;
		else
			T = P4;
	}

	// permute {q[j]} to make {r[j]} such that r[T[j]] = q[j], j = 0, 1, ..., (Nrows-1),
	// where T[j] indicates the original row position of the j-th permuted row
	int_vector r(Nrows, 0); // permuted prime integers
	for (size_type j = 0; j < Nrows; ++j)
		r[ T[j] ] = q[j];

	// U[j][i] is the input bit position of i-th output after the permutation of j-th row
	// Perform the j-th (j=0, 1, 2, ..., (Nrows-1)) intra-row permutation as
	mx::Matrix<size_type> U(Nrows, Ncols, 0);
	if (Ncols == size_t(p))
	{
		for (size_type j = 0; j < Nrows; ++j)
		{
			for (int i = 0; i < p-1; ++i)
				U[j][i] = s[Aux::mod(i*r[j], p-1)];
			U[j][p-1] = 0;
		}
	}
	else if (Ncols == size_t(p+1))
	{
		for (size_type j = 0; j < Nrows; ++j)
		{
			for (int i = 0; i < p-1; ++i)
				U[j][i] = s[Aux::mod(i*r[j], p-1)];
			U[j][p-1] = 0;
			U[j][p] = p;
		}
		if (blockSize == (Ncols*Nrows))
		{
			size_type temp = U[Nrows-1][p]; // (!) swap
			U[Nrows-1][p] = U[Nrows-1][0];
			U[Nrows-1][0] = temp;
		}
	}
	else if (Ncols == size_t(p-1))
	{
		for (size_type j = 0; j < Nrows; ++j)
		{
			for (int i = 0; i < p-1; ++i)
				U[j][i] = s[Aux::mod(i*r[j], p-1)] - 1;
		}
	}

	{ // calculate the interleaver sequence:
		m_irule.assign(blockSize, 0);
		size_type count = 0;

		for (size_type i = 0; i < Ncols; ++i)
			for (size_type j = 0; j < Nrows; ++j)
			{
				const size_t row = T[j];
				const size_t col = U[row][i];
				const size_type index = row*Ncols + col;

				if (index < blockSize)
					m_irule[count++] = index;
			}

		assert(count == blockSize
			&& "invalid rule!");
	}
}


//////////////////////////////////////////////////////////////////////////
void Turbo_WCDMA::interleave(size_type blockSize, const LLR_vector &Li, LLR_vector &Lo) const
{
	for (size_type i = 0; i < blockSize; ++i)
		Lo[i] = Li[ m_irule[i] ];
}


//////////////////////////////////////////////////////////////////////////
void Turbo_WCDMA::deinterleave(size_type blockSize, const LLR_vector &Li, LLR_vector &Lo) const
{
	for (size_type i = 0; i < blockSize; ++i)
		Lo[ m_irule[i] ] = Li[i];
}

	} // Turbo_WCDMA
} // omni namespace
