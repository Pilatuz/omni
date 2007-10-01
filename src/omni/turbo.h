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
	@brief Interface of the TurboCodec class.

@see @ref omni_turbo
@author Sergey Polichnoy
*/
#ifndef __OMNI_TURBO_H_
#define __OMNI_TURBO_H_

#include <omni/codec.h>

namespace omni
{
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
// LogMAP class
class LogMAP {
public:
	double operator()(double x, double y) const;
};


//////////////////////////////////////////////////////////////////////////
// MaxLogMAP class
class MaxLogMAP {
public:
	double operator()(double x, double y) const;
};



//////////////////////////////////////////////////////////////////////////
// WiMAX (IEEE 802.16) Convolutional Turbo Code
class Turbo_WiMAX {
public:
	typedef Trellis::state_type state_type;
	typedef Trellis::bits_type bits_type;
	typedef Trellis::size_type size_type;

public:
	Turbo_WiMAX();
	Turbo_WiMAX(size_type Niters,
		size_type Nsubiters);

public:
	const Trellis& trellis() const;
	size_type N_subiterations() const;
	size_type N_iterations() const;

public: // 1/3 encoding
	template<typename TX, typename AX, typename TY, typename AY>
	void encode(const std::vector<TX,AX> &uncoded, std::vector<TY,AY> &encoded) const
	{
		const size_type blockSize = uncoded.size();
		const size_type Ncouples = blockSize / 2;

		// create interleaving rule
		make_irule(blockSize);

		// interleaved data block
		bit_vector uncoded2(blockSize);
		for (size_type i = 0; i < blockSize; ++i)
			uncoded2[i] = uncoded[ m_irule[i] ];

		// first CRSC encoder
		bit_vector encoded1(blockSize);
		CRSC_encode(Ncouples, uncoded, encoded1);

		// second CRSC encoder
		bit_vector encoded2(blockSize);
		CRSC_encode(Ncouples, uncoded2, encoded2);

		// grouping - { A B Y1 Y2 W1 W2 }
		encoded.resize(3 * blockSize);
		for (size_type k = 0; k < Ncouples; ++k)
		{
			encoded[0*Ncouples + k] = uncoded[2*k + 0];  // A
			encoded[1*Ncouples + k] = uncoded[2*k + 1];  // B

			encoded[2*Ncouples + k] = encoded1[2*k + 1]; // Y1
			encoded[3*Ncouples + k] = encoded2[2*k + 1]; // Y2

			encoded[4*Ncouples + k] = encoded1[2*k + 0]; // W1
			encoded[5*Ncouples + k] = encoded2[2*k + 0]; // W2
		}
	}

public: // 1/3 decoding
	template<typename M, typename TX, typename AX, typename TY, typename AY, typename TZ, typename AZ>
	void decode(const M &metric, const std::vector<TX,AX> &received, std::vector<TY,AY> &decoded, const std::vector<TZ,AZ> &true_bits) const
	{
		const size_type blockSize = received.size()/3;
		const size_type Ncouples = blockSize / 2;
		decoded.resize(blockSize);

		// create interleaving rule
		make_irule(blockSize);

		typedef std::vector<TX, AX>::value_type LLR_type;
		const LLR_type *A  = &received[0] + 0*Ncouples;
		const LLR_type *B  = &received[0] + 1*Ncouples;
		const LLR_type *Y1 = &received[0] + 2*Ncouples;
		const LLR_type *Y2 = &received[0] + 3*Ncouples;
		const LLR_type *W1 = &received[0] + 4*Ncouples;
		const LLR_type *W2 = &received[0] + 5*Ncouples;

		DuoLLR_vector Li(Ncouples);
		DuoLLR_vector Lo1(Ncouples);
		DuoLLR_vector Lo2(Ncouples);
		DuoLLR_vector ext(Ncouples);

		// Turbo Decoder iteration
		for (size_type iter = 0; iter < m_Niterations; ++iter)
		{
			for (size_type k = 0; k < Ncouples; ++k)
			{
				Li[k][A0B0] = ext[k][A0B0] + (+A[k] + B[k]); // a=+1, b=+1
				Li[k][A0B1] = ext[k][A0B1] + (+A[k] - B[k]); // a=+1, b=-1
				Li[k][A1B0] = ext[k][A1B0] + (-A[k] + B[k]); // a=-1, b=+1
				Li[k][A1B1] = ext[k][A1B1] + (-A[k] - B[k]); // a=-1, b=-1
			}

			// first Duo-binary CRSC decoder
			CRSC_decode(metric, Ncouples, &Li[0], W1, Y1, &Lo1[0]);
			for (size_type k = 0; k < Ncouples; ++k)
				Lo1[k] -= ext[k];


			{ // second Duo-binary CRSC decoder
				interleave(Ncouples, &Lo1[0], &Li[0]);
				CRSC_decode(metric, Ncouples, &Li[0], W2, Y2, &Lo2[0]);
				deinterleave(Ncouples, &Lo2[0], &ext[0]);

				// copy ext to Lo2
				std::copy(ext.begin(),
					ext.end(), Lo2.begin());
			}

			for (size_type k = 0; k < Ncouples; ++k)
				ext[k] -= Lo1[k];

			// adaptive stop
			if (!true_bits.empty())
			{
				hard_decision(metric, &Lo2[0], decoded);

				bool done = std::equal(decoded.begin(),
					decoded.end(), true_bits.begin());

				if (done)
					break; // stop iterations
			}
		}

		if (true_bits.empty()) // final decision
			hard_decision(metric, &Lo2[0], decoded);
	}

	template<typename M, typename TX, typename AX, typename TY, typename AY>
	void decode(const M &metric, const std::vector<TX,AX> &received, std::vector<TY,AY> &decoded) const
	{
		decode(metric, received, decoded, std::vector<TY,AY>()); // no true bits
	}

private:
	enum Const {
		NO_STATES = 8,
		NO_IBITS = 2,
		NO_OBITS = 2
	};

	enum {
		A0B0 = 0,
		A0B1 = 1,
		A1B0 = 2,
		A1B1 = 3,

		W0Y0 = 0,
		W0Y1 = 1,
		W1Y0 = 2,
		W1Y1 = 3
	};

private:

	// Alpha
	class Alpha {
	public:
		Alpha();

	public:
		double operator[](state_type state) const;
		double& operator[](state_type state);

	private:
		double m_A[NO_STATES];
	};

	// Beta
	class Beta {
	public:
		Beta();

	public:
		double operator[](state_type state) const;
		double& operator[](state_type state);

	private:
		double m_B[NO_STATES];
	};

	// Gamma
	class Gamma {
	public:
		Gamma();

	public:
		double operator[](bits_type abwy) const;
		double& operator[](bits_type abwy);

	private:
		enum {
			NO_BRANCH = 1U<<(NO_IBITS+NO_OBITS)
		};

	private:
		double m_G[NO_BRANCH];
	};

	// Duo-binary LLR
	class DuoLLR {
	public:
		DuoLLR();

	public:
		DuoLLR& operator+=(const DuoLLR &x);
		DuoLLR& operator-=(const DuoLLR &x);

		DuoLLR& operator+=(double x);
		DuoLLR& operator-=(double x);

	public:
		double operator[](size_t ab) const;
		double& operator[](size_t ab);

	private:
		double m_L[1U << NO_IBITS];
	};

private:
	typedef std::vector<size_t, pool::Allocator<size_t> > bit_vector;
	typedef std::vector<double, pool::Allocator<double> > LLR_vector;
	typedef std::vector<DuoLLR, pool::Allocator<DuoLLR> > DuoLLR_vector;

private:
	static state_type start_state(size_type blockSize, state_type end_state);

private: // interleaving
	void make_irule(size_type blockSize) const;

	void interleave(size_type Ncouples,
		const DuoLLR *Li, DuoLLR *Lo) const;
	void deinterleave(size_type Ncouples,
		const DuoLLR *Li, DuoLLR *Lo) const;

private:
	template<typename TX, typename AX, typename TY, typename AY>
	void CRSC_encode(size_type Ncouples, const std::vector<TX,AX> &uncoded, std::vector<TY,AY> &coded) const
	{
		state_type s = 0;

		// determine last state
		for (size_type k = 0; k < Ncouples; ++k)
		{
			const bits_type ab = Trellis::bi2de(&uncoded[2*k], 2);
			const Trellis::Fwd &fwd = m_trellis.fwd(s, ab);

			s = fwd.state;
		}

		// determine start state
		state_type Sc = s = start_state(2*Ncouples, s);

		// real encoding
		for (size_type k = 0; k < Ncouples; ++k)
		{
			const bits_type ab = Trellis::bi2de(&uncoded[2*k], 2);
			const Trellis::Fwd &fwd = m_trellis.fwd(s, ab);

			Trellis::de2bi(fwd.obits, 2, &coded[2*k]);
			s = fwd.state;
		}

		assert(Sc==s && "mismatch "
			"start and finish states");

		Sc;
	}

	template<typename M, typename T>
	void CRSC_decode(const M &metric, size_type Ncouples,
		const DuoLLR *Li, const T *Lw, const T *Ly, DuoLLR *Lo) const
	{
		std::vector< Alpha, pool::Allocator<Alpha> > alpha(Ncouples+1);
		std::vector<  Beta, pool::Allocator< Beta> >  beta(Ncouples+1);
		std::vector< Gamma, pool::Allocator<Gamma> > gamma(Ncouples+0);

		const double ALPHA_DIFF = 1.0e-2;
		const double  BETA_DIFF = 1.0e-2;

		// calculate gamma...
		for (size_type k = 0; k < Ncouples; ++k)
		{
			gamma[k][(A0B0<<2)|W0Y0] = Li[k][A0B0] + Lw[k] + Ly[k];
			gamma[k][(A0B0<<2)|W0Y1] = Li[k][A0B0] + Lw[k] - Ly[k];
			gamma[k][(A0B0<<2)|W1Y0] = Li[k][A0B0] - Lw[k] + Ly[k];
			gamma[k][(A0B0<<2)|W1Y1] = Li[k][A0B0] - Lw[k] - Ly[k];

			gamma[k][(A0B1<<2)|W0Y0] = Li[k][A0B1] + Lw[k] + Ly[k];
			gamma[k][(A0B1<<2)|W0Y1] = Li[k][A0B1] + Lw[k] - Ly[k];
			gamma[k][(A0B1<<2)|W1Y0] = Li[k][A0B1] - Lw[k] + Ly[k];
			gamma[k][(A0B1<<2)|W1Y1] = Li[k][A0B1] - Lw[k] - Ly[k];

			gamma[k][(A1B0<<2)|W0Y0] = Li[k][A1B0] + Lw[k] + Ly[k];
			gamma[k][(A1B0<<2)|W0Y1] = Li[k][A1B0] + Lw[k] - Ly[k];
			gamma[k][(A1B0<<2)|W1Y0] = Li[k][A1B0] - Lw[k] + Ly[k];
			gamma[k][(A1B0<<2)|W1Y1] = Li[k][A1B0] - Lw[k] - Ly[k];

			gamma[k][(A1B1<<2)|W0Y0] = Li[k][A1B1] + Lw[k] + Ly[k];
			gamma[k][(A1B1<<2)|W0Y1] = Li[k][A1B1] + Lw[k] - Ly[k];
			gamma[k][(A1B1<<2)|W1Y0] = Li[k][A1B1] - Lw[k] + Ly[k];
			gamma[k][(A1B1<<2)|W1Y1] = Li[k][A1B1] - Lw[k] - Ly[k];
		}


		{ // forward metrics calculation (alpha)...
			Alpha old_metrics;

			for (size_type iter = 0; iter < m_Nsubiterations; ++iter)
			{
				for (size_type k = 0; k < Ncouples; ++k)
				{
					for (state_type state = 0; state < NO_STATES; ++state) // all states
					{
						const Trellis::Bwd &bwd0 = m_trellis.bwd(state, 0);
						const Trellis::Bwd &bwd1 = m_trellis.bwd(state, 1);
						const Trellis::Bwd &bwd2 = m_trellis.bwd(state, 2);
						const Trellis::Bwd &bwd3 = m_trellis.bwd(state, 3);

						const Trellis::Fwd &fwd0 = m_trellis.fwd(bwd0.state, bwd0.ibits);
						const Trellis::Fwd &fwd1 = m_trellis.fwd(bwd1.state, bwd1.ibits);
						const Trellis::Fwd &fwd2 = m_trellis.fwd(bwd2.state, bwd2.ibits);
						const Trellis::Fwd &fwd3 = m_trellis.fwd(bwd3.state, bwd3.ibits);

						const bits_type abwy0 = (bwd0.ibits << 2) | fwd0.obits;
						const bits_type abwy1 = (bwd1.ibits << 2) | fwd1.obits;
						const bits_type abwy2 = (bwd2.ibits << 2) | fwd2.obits;
						const bits_type abwy3 = (bwd3.ibits << 2) | fwd3.obits;

						const double a0 = alpha[k][bwd0.state] + gamma[k][abwy0];
						const double a1 = alpha[k][bwd1.state] + gamma[k][abwy1];
						const double a2 = alpha[k][bwd2.state] + gamma[k][abwy2];
						const double a3 = alpha[k][bwd3.state] + gamma[k][abwy3];

						alpha[k+1][state] = metric(metric(a0, a1), metric(a2, a3));
					}

					// normalization...
					const double A0 = alpha[k+1][0];
					for (state_type state = 0; state < NO_STATES; ++state)
						alpha[k+1][state] -= A0;
				}

				// copy last alpha to the first?
				alpha[0] = alpha[Ncouples];

				{ // stop criteria
					bool done = true;
					for (state_type state = 0; state < NO_STATES; ++state)
					{
						const double A = alpha[Ncouples][state];
						const double dA = A - old_metrics[state];
						if (ALPHA_DIFF < dA*dA)
						{
							done = false;
							break;
						}
					}

					if (!done)
						old_metrics = alpha[Ncouples];
					else
						break; // break iterations
				}
			}
		} // forward metrics calculation (alpha)


		{ // backward metrics calculation (beta) ...
			Beta old_metrics;

			for (size_type iter = 0; iter < m_Nsubiterations; ++iter)
			{
				for (ptrdiff_t k = Ncouples-1; 0 <= k; --k)
				{
					for (state_type state = 0; state < NO_STATES; ++state) // all states
					{
						const Trellis::Fwd &fwd0 = m_trellis.fwd(state, A0B0);
						const Trellis::Fwd &fwd1 = m_trellis.fwd(state, A0B1);
						const Trellis::Fwd &fwd2 = m_trellis.fwd(state, A1B0);
						const Trellis::Fwd &fwd3 = m_trellis.fwd(state, A1B1);

						const bits_type abwy0 = (A0B0 << 2) | fwd0.obits;
						const bits_type abwy1 = (A0B1 << 2) | fwd1.obits;
						const bits_type abwy2 = (A1B0 << 2) | fwd2.obits;
						const bits_type abwy3 = (A1B1 << 2) | fwd3.obits;

						const double b0 = beta[k+1][fwd0.state] + gamma[k][abwy0];
						const double b1 = beta[k+1][fwd1.state] + gamma[k][abwy1];
						const double b2 = beta[k+1][fwd2.state] + gamma[k][abwy2];
						const double b3 = beta[k+1][fwd3.state] + gamma[k][abwy3];

						beta[k][state] = metric(metric(b0, b1), metric(b2, b3));
					}

					// normalization...
					const double B0 = beta[k][0];
					for (state_type state = 0; state < NO_STATES; ++state)
						beta[k][state] -= B0;
				}

				// copy first beta to the last?
				beta[Ncouples] = beta[0];

				{ // stop criteria ...
					bool done = true;

					for (state_type state = 0; state < NO_STATES; ++state)
					{
						const double B = beta[0][state];
						const double dB = B - old_metrics[state];

						if (BETA_DIFF < dB*dB)
						{
							done = false;
							break;
						}
					}

					if (!done)
						old_metrics = beta[0];
					else
						break; // break iterations
				}
			}
		} // backward metrics calculation (beta)


		// output LLR calculation...
		for (size_type k = 0; k < Ncouples; ++k)
		{
			for (bits_type ab = 0; ab < (1U<<NO_IBITS); ++ab)
			{
				state_type state = 0;
				const Trellis::Fwd &fwd = m_trellis.fwd(state, ab);
				const bits_type abwy = (ab << 2) | fwd.obits;

				double Zmax = alpha[k][state]
					+ beta[k+1][fwd.state] + gamma[k][abwy];

				for (++state; state < NO_STATES; ++state)
				{
					const Trellis::Fwd &fwd = m_trellis.fwd(state, ab);
					const bits_type abwy = (ab << 2) | fwd.obits;

					const double Z = alpha[k][state]
						+ beta[k+1][fwd.state] + gamma[k][abwy];

					Zmax = metric(Zmax, Z);
				}

				Lo[k][ab] = Zmax;
			}

			Lo[k] -= Lo[k][0]; // decision
		}
	}


	template<typename M, typename TY, typename AY>
	void hard_decision(const M &metric, const DuoLLR *Lo, std::vector<TY, AY> &decoded) const
	{
		const size_type Ncouples = decoded.size() / 2;
		for (size_type k = 0; k < Ncouples; ++k)
		{
			const DuoLLR &duo_LLR = Lo[k];

			const double A0 = metric(duo_LLR[A0B0], duo_LLR[A0B1]);
			const double A1 = metric(duo_LLR[A1B0], duo_LLR[A1B1]);

			const double B0 = metric(duo_LLR[A0B0], duo_LLR[A1B0]);
			const double B1 = metric(duo_LLR[A0B1], duo_LLR[A1B1]);

			decoded[2*k + 0] = ((A1-A0) < 0.0) ? 0:1; // A decision
			decoded[2*k + 1] = ((B1-B0) < 0.0) ? 0:1; // B decision
		}
	}

private:
	size_type m_Niterations;
	size_type m_Nsubiterations;

	mutable std::vector<size_type> m_irule;
	Trellis m_trellis;
};


//////////////////////////////////////////////////////////////////////////
// WCDMA (UMTS, 3GPP) Convolutional Turbo Code
class Turbo_WCDMA {
public:
	typedef Trellis::state_type state_type;
	typedef Trellis::bits_type bits_type;
	typedef Trellis::size_type size_type;

public:
	Turbo_WCDMA();
	explicit Turbo_WCDMA(size_type Niters);

public:
	const Trellis& trellis() const;
	size_type N_iterations() const;

public: // 1/3 encoding
	template<typename TX, typename AX, typename TY, typename AY>
	void encode(const std::vector<TX,AX> &uncoded, std::vector<TY,AY> &encoded) const
	{
		const size_type blockSize = uncoded.size();

		// create interleaving rule
		make_irule(blockSize);

		// interleaved data block
		bit_vector uncoded2(blockSize);
		for (size_type i = 0; i < blockSize; ++i)
			uncoded2[i] = uncoded[ m_irule[i] ];

		// first CRSC encoder
		bit_vector encoded1(blockSize);
		bit_vector tail1(2*NO_TAILS);
		RSC_encode(blockSize, uncoded, encoded1, tail1);

		// second CRSC encoder
		bit_vector encoded2(blockSize);
		bit_vector tail2(2*NO_TAILS);
		RSC_encode(blockSize, uncoded2, encoded2, tail2);

		// grouping
		encoded.resize(3*blockSize + 4*NO_TAILS);
		for (size_type i = 0; i < blockSize; ++i)
		{
			encoded[3*i + 0] = uncoded[i];  // X
			encoded[3*i + 1] = encoded1[i]; // Z1
			encoded[3*i + 2] = encoded2[i]; // Z2
		}

		size_type k = 3*blockSize;

		// first tail
		for (size_type i = 0; i < 2*NO_TAILS; ++i, ++k)
			encoded[k] = tail1[i];

		// second tail
		for (size_type i = 0; i < 2*NO_TAILS; ++i, ++k)
			encoded[k] = tail2[i];
	}

public: // 1/3 decoding
	template<typename M, typename TX, typename AX, typename TY, typename AY, typename TZ, typename AZ>
	void decode(const M &metric, const std::vector<TX,AX> &received, std::vector<TY,AY> &decoded, const std::vector<TZ,AZ> &true_bits) const
	{
		const size_type blockSize = (received.size() - 4*NO_TAILS)/3;
		decoded.resize(blockSize);

		// create interleaving rule
		make_irule(blockSize);

		LLR_vector X1(blockSize + NO_TAILS);
		LLR_vector X2(blockSize + NO_TAILS);
		LLR_vector Z1(blockSize + NO_TAILS);
		LLR_vector Z2(blockSize + NO_TAILS);

		LLR_vector Lo1(blockSize, 0.0);
		LLR_vector Lo2(blockSize, 0.0);
		LLR_vector t1(blockSize);
		LLR_vector t2(blockSize);

		// extract systematic and parity LLRs
		for (size_type i = 0; i < blockSize; ++i)
		{
			X1[i] = received[3*i + 0];  // X
			Z1[i] = received[3*i + 1];  // Z1
			Z2[i] = received[3*i + 2];  // Z2
		}

		interleave(blockSize, X1, X2);

		// extract first tail
		for (size_type i = 0; i < NO_TAILS; ++i)
		{
			X1[blockSize + i] = received[3*blockSize + 2*i + 0];
			Z1[blockSize + i] = received[3*blockSize + 2*i + 1];
		}

		// extract second tail
		for (size_type i = 0; i < NO_TAILS; ++i)
		{
			X2[blockSize + i] = received[3*blockSize + 2*NO_TAILS + 2*i + 0];
			Z2[blockSize + i] = received[3*blockSize + 2*NO_TAILS + 2*i + 1];
		}

		// Turbo Decoder iteration
		for (size_type iter = 0; iter < m_Niterations; ++iter)
		{
			// first RSC decoder
			RSC_decode(metric, blockSize, X1, Z1, Lo2, Lo1);

			// second RSC decoder
			interleave(blockSize, Lo1, t1);
			RSC_decode(metric, blockSize, X2, Z2, t1, t2);
			deinterleave(blockSize, t2, Lo2);

			// adaptive stop
			if (!true_bits.empty())
			{
				hard_decision(X1, Lo1, Lo2, decoded);

				bool done = std::equal(decoded.begin(),
					decoded.end(), true_bits.begin());

				if (done)
					break; // stop iterations
			}
		}

		if (true_bits.empty()) // final decision
			hard_decision(X1, Lo1, Lo2, decoded);
	}

	template<typename M, typename TX, typename AX, typename TY, typename AY>
	void decode(const M &metric, const std::vector<TX,AX> &received, std::vector<TY,AY> &decoded) const
	{
		decode(metric, received, decoded, std::vector<TY, AY>()); // no true bits
	}

private:
	enum Const {
		NO_STATES = 8,
		NO_IBITS = 1,
		NO_OBITS = 1,
		NO_TAILS = 3
	};

private:

	// Alpha
	class Alpha {
	public:
		Alpha();

	public:
		double operator[](state_type state) const;
		double& operator[](state_type state);

	private:
		double m_A[NO_STATES];
	};

	// Beta
	class Beta {
	public:
		Beta();

	public:
		double operator[](state_type state) const;
		double& operator[](state_type state);

	private:
		double m_B[NO_STATES];
	};

	// Gamma
	class Gamma {
	public:
		Gamma();

	public:
		double operator[](bits_type xz) const;
		double& operator[](bits_type xz);

	private:
		enum {
			NO_BRANCH = 1U<<(NO_IBITS+NO_OBITS)
		};

	private:
		double m_G[NO_BRANCH];
	};

private:
	typedef std::vector<size_t, pool::Allocator<size_t> > bit_vector;
	typedef std::vector<double, pool::Allocator<double> > LLR_vector;

private: // interleaving
	void make_irule(size_type blockSize) const;

	void interleave(size_type blockSize,
		const LLR_vector &Li, LLR_vector &Lo) const;
	void deinterleave(size_type blockSize,
		const LLR_vector &Li, LLR_vector &Lo) const;

private:
	template<typename TX, typename AX, typename TY, typename AY>
	void RSC_encode(size_type blockSize, const std::vector<TX,AX> &uncoded,
		std::vector<TY,AY> &coded, std::vector<TY,AY> &tail) const
	{
		state_type s = 0;

		// encoding
		for (size_type i = 0; i < blockSize; ++i)
		{
			const bits_type x = uncoded[i] != 0;
			const Trellis::Fwd &fwd = m_trellis.fwd(s, x);

			coded[i] = fwd.obits;
			s = fwd.state;
		}

		// tail
		for (size_type i = 0; i < NO_TAILS; ++i)
		{
			state_type new_s = s >> 1;
			bits_type x, z;

			const Trellis::Fwd &fwd0 = m_trellis.fwd(s, 0);
			const Trellis::Fwd &fwd1 = m_trellis.fwd(s, 1);

			if (fwd0.state == new_s)
			{
				x = 0;
				z = fwd0.obits;
			}
			else
			{
				x = 1;
				z = fwd1.obits;
			}

			tail[2*i + 0] = x;
			tail[2*i + 1] = z;

			s = new_s;
		}

		assert(0==s && "Invalid final state");
	}

	template<typename M>
	void RSC_decode(const M &metric, size_type blockSize,
		const LLR_vector &Lx, const LLR_vector &Lz,
		const LLR_vector &Ei, LLR_vector &Eo) const
	{
		std::vector< Alpha, pool::Allocator<Alpha> > alpha(blockSize+NO_TAILS+1);
		std::vector<  Beta, pool::Allocator< Beta> >  beta(blockSize+NO_TAILS+1);
		std::vector< Gamma, pool::Allocator<Gamma> > gamma(blockSize+NO_TAILS);

		// calculate gamma...
		for (size_type k = 0; k < blockSize; ++k)
		{
			gamma[k][0] = +Ei[k] + Lx[k] + Lz[k]; // x=+1, z=+1
			gamma[k][1] = +Ei[k] + Lx[k] - Lz[k]; // x=+1, z=-1
			gamma[k][2] = -Ei[k] - Lx[k] + Lz[k]; // x=-1, z=+1
			gamma[k][3] = -Ei[k] - Lx[k] - Lz[k]; // x=-1. z=-1
		}
		for (size_type k = blockSize; k < blockSize+NO_TAILS; ++k)
		{
			gamma[k][0] = +Lx[k] + Lz[k]; // x=+1, z=+1
			gamma[k][1] = +Lx[k] - Lz[k]; // x=+1, z=-1
			gamma[k][2] = -Lx[k] + Lz[k]; // x=-1, z=+1
			gamma[k][3] = -Lx[k] - Lz[k]; // x=-1. z=-1
		}


		{ // forward metrics calculation (alpha)...
			for (state_type s = 1; s < NO_STATES; ++s)
				alpha[0][s] = -std::numeric_limits<double>::infinity();

			for (size_type k = 0; k < blockSize+NO_TAILS; ++k)
			{
				for (state_type state = 0; state < NO_STATES; ++state) // all states
				{
					const Trellis::Bwd &bwd0 = m_trellis.bwd(state, 0);
					const Trellis::Bwd &bwd1 = m_trellis.bwd(state, 1);

					const Trellis::Fwd &fwd0 = m_trellis.fwd(bwd0.state, bwd0.ibits);
					const Trellis::Fwd &fwd1 = m_trellis.fwd(bwd1.state, bwd1.ibits);

					const bits_type xz0 = (bwd0.ibits << NO_OBITS) | fwd0.obits;
					const bits_type xz1 = (bwd1.ibits << NO_OBITS) | fwd1.obits;

					const double a0 = alpha[k][bwd0.state] + gamma[k][xz0];
					const double a1 = alpha[k][bwd1.state] + gamma[k][xz1];

					alpha[k+1][state] = metric(a0, a1);
				}

				// normalization...
				const double A0 = alpha[k+1][0];
				for (state_type s = 0; s < NO_STATES; ++s)
					alpha[k+1][s] -= A0;
			}
		} // forward metrics calculation (alpha)


		{ // backward metrics calculation (beta) ...
			for (state_type s = 1; s < NO_STATES; ++s)
				beta[blockSize+NO_TAILS][s] = -std::numeric_limits<double>::infinity();

			for (ptrdiff_t k = blockSize+NO_TAILS-1; 0 <= k; --k)
			{
				for (state_type state = 0; state < NO_STATES; ++state) // all states
				{
					const Trellis::Fwd &fwd0 = m_trellis.fwd(state, 0);
					const Trellis::Fwd &fwd1 = m_trellis.fwd(state, 1);

					const bits_type xz0 = (0 << NO_OBITS) | fwd0.obits;
					const bits_type xz1 = (1 << NO_OBITS) | fwd1.obits;

					const double b0 = beta[k+1][fwd0.state] + gamma[k][xz0];
					const double b1 = beta[k+1][fwd1.state] + gamma[k][xz1];

					beta[k][state] = metric(b0, b1);
				}

				// normalization...
				const double B0 = beta[k][0];
				for (state_type s = 0; s < NO_STATES; ++s)
					beta[k][s] -= B0;
			}
		} // backward metrics calculation (beta)


		// extrinsic output calculation...
		for (size_type k = 0; k < blockSize; ++k)
		{
			state_type state = 0;

			const Trellis::Fwd &fwd0 = m_trellis.fwd(state, 0);
			const Trellis::Fwd &fwd1 = m_trellis.fwd(state, 1);

			const double Z0 = fwd0.obits ? -Lz[k] : +Lz[k];
			const double Z1 = fwd1.obits ? -Lz[k] : +Lz[k];

			double Lmax0 = alpha[k][state] + beta[k+1][fwd0.state] + Z0;
			double Lmax1 = alpha[k][state] + beta[k+1][fwd1.state] + Z1;

			for (++state; state < NO_STATES; ++state)
			{
				const Trellis::Fwd &fwd0 = m_trellis.fwd(state, 0);
				const Trellis::Fwd &fwd1 = m_trellis.fwd(state, 1);

				const double Z0 = fwd0.obits ? -Lz[k] : +Lz[k];
				const double Z1 = fwd1.obits ? -Lz[k] : +Lz[k];

				const double L0 = alpha[k][state] + beta[k+1][fwd0.state] + Z0;
				const double L1 = alpha[k][state] + beta[k+1][fwd1.state] + Z1;

				Lmax0 = metric(Lmax0, L0);
				Lmax1 = metric(Lmax1, L1);
			}

			Eo[k] = (Lmax0 - Lmax1)*0.5; // decision
		}
	}


	template<typename TY, typename AY>
	void hard_decision(const LLR_vector &Lx, const LLR_vector &Lo1,
		const LLR_vector &Lo2, std::vector<TY,AY> &decoded) const
	{
		const size_type blockSize = decoded.size();
		for (size_type i = 0; i < blockSize; ++i)
		{
			const double L = Lx[i] + Lo1[i] + Lo2[i];

			decoded[i] = (L < 0.0) ? 1:0; // X decision
		}
	}

private:
	size_type m_Niterations;

	mutable std::vector<size_type> m_irule;
	Trellis m_trellis;
};


	} // dsp namespace
} // omni namespace

#endif // __OMNI_TURBO_H_
