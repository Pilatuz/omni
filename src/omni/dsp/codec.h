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
	@brief Interface of the coding tools.

@see @ref omni_codec
@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_CODEC_H_
#define __OMNI_CODEC_H_

#include <omni/matrix.hpp>
#include <omni/util.hpp>

#include <assert.h>
#include <limits>
#include <vector>

namespace omni
{
	// Trellis
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The coding trellis.
/**
		The trellis class stores all coding information: constraint length, polynomials etc.

@see omni_codec_trellis
*/
class Trellis
{
public:

	typedef size_t state_type; ///< @brief The codec state type.
	typedef size_t bits_type;  ///< @brief The uncoded and coded words type.
	typedef size_t size_type;  ///< @brief The size type.


public: // simple constructors

	Trellis(const char *opolynomials,
		size_type constraint_length,
		size_type Ni = 1);

	Trellis(const char *opolynomials,
		const char *ipolynomials,
		size_type constraint_length,
		bits_type feedback);

	Trellis(const char *pre_opolynomials,
		const char *opolynomials,
		const char *ipolynomials,
		size_type constraint_length,
		bits_type feedback);


public: // advanced constructors

	// construct trellis
	template<typename In>
		Trellis(In ofirst, In olast,
			size_type constraint_length,
			size_type Ni = 1)
	{
		PolyList opoly(std::distance(ofirst, olast));
		std::copy(ofirst, olast, opoly.begin());
		init(opoly, constraint_length, Ni);
	}

	// construct recursive trellis
	template<typename In1, typename In2, typename In3>
		Trellis(In1 ofirst1, In1 olast1,
			In2 ofirst2, In2 olast2, In3 ifirst, In3 ilast,
			size_type constraint_length, bits_type feedback)
	{
		PolyList opoly1(std::distance(ofirst1, olast1));
		std::copy(ofirst1, olast1, opoly1.begin());

		PolyList opoly2(std::distance(ofirst2, olast2));
		std::copy(ofirst2, olast2, opoly2.begin());

		PolyList ipoly(std::distance(ifirst, ilast));
		std::copy(ifirst, ilast, ipoly.begin());

		init(ipoly, opoly1, opoly2,
			constraint_length, feedback);
	}


public: // auxiliary constructors

	// Rep parameters (repetition)
	struct Rep {
		explicit Rep(size_type n);

		size_type Nrep;
	};

	// construct "Rep" trellis
	Trellis(const Trellis &x, const Rep &p);


	// Cut parameters (cutting)
	struct Cut {
		explicit Cut(bits_type m);

		bits_type Mcut;
	};

	// construct "Cut" trellis
	Trellis(const Trellis &x, const Cut &p);


	// Sys parameters (systematic)
	struct Sys {
		Sys(size_type n, bool before);

		size_type Nsys;
		bool sys_first;
	};

	// construct "Sys" trellis
	Trellis(const Trellis &x, const Sys &p);


	// default constructor
	Trellis();


public: // trellis generation

	static Trellis ODS_code(size_type constraint_length, size_type code_rate);
	static Trellis MFD_code(size_type constraint_length, size_type code_rate);


public: // trellis manipulation

	Trellis& rep(size_type Nrep);
	Trellis& cut(bits_type Mcut);
	Trellis& sys(size_type Nsys, bool before);


public: // properties

	size_type N_states() const; // number of states
	size_type N_ibits() const;  // number of uncoded bits (input)
	size_type N_obits() const;  // number of encoded bits (output)
	size_type N_tails(bool recursive) const;  // number of ticks to reset register
	size_type length() const;   // constraint length


public: // trellis

	/// @brief Forward state transition (leaving branches).
	struct Fwd {
		Fwd(state_type s,
			bits_type o);
		Fwd();

		state_type state; ///< @brief The target (next or entering) state.
		bits_type obits;  ///< @brief The corresponding output (encoded) bits.
	};

	/// @brief Backward state transition (entering branches).
	struct Bwd {
		Bwd(state_type s,
			bits_type i);
		Bwd();

		state_type state; ///< @brief The source (previous or leaving) state.
		bits_type ibits;  ///< @brief The corresponding input (uncoded) bits.
	};

	// forward and backward matrices
	const Fwd& fwd(state_type state, bits_type ibits) const;
	const Bwd& bwd(state_type state, size_type n) const;


public: // auxiliary functions

	// pack (MSB-first)
	template<typename In> inline
	static bits_type bi2de(In first, size_t Nbits)
	{
		return util::bi2de_msb(first, Nbits, bits_type(0));
	}

	// unpack (MSB-first)
	template<typename Out> inline
	static Out de2bi(bits_type x, size_t Nbits, Out first)
	{
		return util::de2bi_msb(x, Nbits, first);
	}


private: // initialization

	/// @brief The auxiliary list of polynomials.
	typedef std::vector<bits_type> PolyList;

	// construct convolutional trellis
	void init(const PolyList &poly,
		size_type constraint_length,
			size_type Ni);

	// construct recursive trellis
	void init(const PolyList &ipoly,
		const PolyList &opoly1, const PolyList &opoly2,
		size_type constraint_length, bits_type feedback);

	// construct Previous matrix
	// calculate number of tails
	void post_init();

private: // properties
	mx::Matrix<Fwd> m_fwd;  // forward matrix
	mx::Matrix<Bwd> m_bwd;  // backward matrix

	size_type m_N_states; // number of states
	size_type m_N_ibits;  // number of uncoded bits (input)
	size_type m_N_obits;  // number of encoded bits (output)
	size_type m_N_Ztails;  // number of zero tails
	size_type m_N_Rtails;  // number of tails for recursive trellis
	size_type m_length;   // constraint length
};

	} // Trellis


	// ConvCodec
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
// ConvCodec
class ConvCodec {
public:
	typedef Trellis::state_type state_type; ///< @brief The codec state type.
	typedef Trellis::bits_type bits_type;   ///< @brief The uncoded and coded words type.
	typedef Trellis::size_type size_type;   ///< @brief The size type.

public:
	explicit ConvCodec(const Trellis &tr);

public:
	const Trellis& trellis() const;

public: // encoding

	// encode tail
	template<typename In, typename Out>
	Out encode_tail(In first, In last, Out out) const
	{
		const Trellis &tr = trellis();

		const size_type Ni = tr.N_ibits(); // number of uncoded bits
		const size_type No = tr.N_obits(); // number of encoded bits

		const size_type i_size = std::distance(first, last);
		assert(0 == i_size%Ni && "invalid input size");

		// encoding
		state_type state = 0; // (!) start from zero state
		while (first != last)
		{
			const bits_type ibits = tr.bi2de(first, Ni);
			std::advance(first, Ni);

			const Trellis::Fwd &fwd = tr.fwd(state, ibits);
			out = tr.de2bi(fwd.obits, No, out);
			state = fwd.state;
		}

		// add tail zeros
		const size_type Nzeros = tr.N_tails(false);
		for (size_type i = 0; i < Nzeros; ++i)
		{
			const bits_type ibits = 0;

			const Trellis::Fwd &fwd = tr.fwd(state, ibits);
			out = tr.de2bi(fwd.obits, No, out);
			state = fwd.state;
		}

		assert(0==state && "invalid final state");
		return out;
	}


	// encode tailbite
	template<typename In, typename Out>
	Out encode_bite(In first, In last, Out out) const
	{
		const size_type Ni = trellis().N_ibits(); // number of uncoded bits
		const size_type No = trellis().N_obits(); // number of encoded bits

		const size_type i_size = std::distance(first, last);
		assert(0 == i_size%Ni && "invalid input size");

		state_type start_state = 0;
		{ // determine start state
			size_type Nticks = trellis().N_tails(false) + 1;
			assert(Nticks<i_size && "invalid input size");

			In tmp = first;
			std::advance(tmp, i_size - Nticks*Ni);

			for (size_type k = 0; k < Nticks; ++k)
			{
				const bits_type ibits = Trellis::bi2de(tmp, Ni);
				std::advance(tmp, Ni);

				const Trellis::Fwd &fwd = m_trellis.fwd(start_state, ibits);
				start_state = fwd.state;
			}
		}

		state_type state = start_state;
		while (first != last)
		{
			const bits_type ibits = Trellis::bi2de(first, Ni);
			std::advance(first, Ni);

			const Trellis::Fwd &fwd = trellis().fwd(state, ibits);

			Trellis::de2bi(fwd.obits, No, out);
			std::advance(out, No);
			state = fwd.state;
		}

		assert(start_state==state && "invalid final state");
		return out;
	}

public:

	// BranchMetrics
	class BranchMetrics {
	public:
		BranchMetrics(size_type length, size_type Nbits);

	public:
		double operator()(size_type k, size_type bits) const;
		double& operator()(size_type k, size_type bits);

		size_type length() const;

	private:
		mx::Matrix<double> m_metrics;
	};

public: // decoding

	// decode tail
	template<typename Out>
		void decode_tail(const BranchMetrics &bm, Out out) const
	{
		const size_type Nstates = trellis().N_states();

		if (m_X_trellis.empty())
		{
			PathMem path(Nstates, bm.length());
			decode_tail_(path, bm, out);
		}
		else
		{
			XPathMem path(Nstates, bm.length());
			decode_tail_(path, bm, out);
		}
	}

	// decode tailbite
	template<typename Out>
		void decode_bite(size_type Niter, const BranchMetrics &bm, Out out) const
	{
		const size_type Nstates = trellis().N_states();

		if (m_X_trellis.empty())
		{
			PathMem path(Nstates, bm.length());
			decode_bite_(Niter, path, bm, out);
		}
		else
		{
			XPathMem path(Nstates, bm.length());
			decode_bite_(Niter, path, bm, out);
		}
	}

public:

	// decode tail
	template<typename In, typename Out>
		void decode_tail(In first, In last, Out out) const
	{
		const size_type i_size = std::distance(first, last);
		const size_type No = trellis().N_obits(); // number of encoded bits

		assert(0 == i_size%No && "invalid input size");
		const size_type length = i_size / No;

		BranchMetrics bm(length, No);
		init_branch_metrics(bm, first);
		decode_tail(bm, out);
	}

	// decode tailbite
	template<typename In, typename Out>
		void decode_bite(size_type Niter, In first, In last, Out out) const
	{
		const size_type i_size = std::distance(first, last);
		const size_type No = trellis().N_obits(); // number of encoded bits

		assert(0 == i_size%No && "invalid input size");
		const size_type length = i_size / No;

		BranchMetrics bm(length, No);
		init_branch_metrics(bm, first);
		decode_bite(Niter, bm, out);
	}

private:
	typedef std::vector<double, pool::Allocator<double> > metric_vector;
	typedef std::vector<int,    pool::Allocator<int> >       bit_vector;

	// TODO: matrix row/column major adaptation
	typedef mx::Matrix<state_type>  XPathMem;
	typedef mx::Matrix<Trellis::Bwd> PathMem;

	// decode tail
	template<typename PM, typename Out>
		void decode_tail_(PM &path, const BranchMetrics &bm, Out out) const
	{
		const size_type Nstates = trellis().N_states();
		const size_type Ntails = trellis().N_tails(false);
		const size_type Ni = trellis().N_ibits();

		assert(Ntails < bm.length() && "input too small");

		metric_vector metrics(Nstates,
			-std::numeric_limits<double>::infinity());
		metrics[0] = 0.0; // (!) start from zero state

		bit_vector dec_bits(bm.length() * Ni);

		viterbi_iteration(bm, path, metrics);
		trace_back(path, 0, dec_bits); // (!) final state is zero

		std::copy(dec_bits.begin(),    // remove tail bits
			dec_bits.end() - Ntails*Ni, out);
	}

	// decode tailbite
	template<typename PM, typename Out>
		void decode_bite_(size_type Niter, PM &path, const BranchMetrics &bm, Out out) const
	{
		const size_type Nstates = trellis().N_states();
		const size_type Ni = trellis().N_ibits();

		metric_vector metrics(Nstates, 0.0);

		state_type start_state = 0;
		state_type final_state = 0;

		for (size_type it = 0; it < Niter; ++it)
		{
			viterbi_iteration(bm, path, metrics);

			// find state with max metrics
			final_state = std::distance( metrics.begin(),
					std::max_element(metrics.begin(), metrics.end()));
			start_state = find_start_state(path, final_state);

			if (start_state == final_state)
				break;

			// TODO: normalize metrics?
		}

		// WAVA: find path with max metrics
		// where "begin state" == "end state"
		if (start_state != final_state)
		{
			metrics[final_state] = -std::numeric_limits<double>::infinity();
			for (size_type s = 1; s < Nstates; ++s)
			{
				state_type final_state_ = std::distance( metrics.begin(),
					std::max_element(metrics.begin(), metrics.end()));
				metrics[final_state_] = -std::numeric_limits<double>::infinity();

				if (find_start_state(path, final_state_) == final_state_)
				{
					final_state = final_state_;
					break;
				}
			}
		}

		bit_vector dec_bits(bm.length() * Ni);
		trace_back(path, final_state, dec_bits);

		// store decoded bits
		std::copy(dec_bits.begin(),
			dec_bits.end(), out);
	}

private:

	// initialize branch metrics
	template<typename In>
		void init_branch_metrics(BranchMetrics &bm, In first) const
	{
		const size_type No = trellis().N_obits(); // number of encoded bits
		const size_type No_len = 1U<<No;
		const size_type O_MASK = No_len-1;

		for (size_type k = 0; k < bm.length(); ++k)
		{
			for (bits_type obits = 0; obits < No_len/2; ++obits)
			{
				double metric = 0.0;
				In tmp = first;

				for (size_type i = 0; i < No; ++i, ++tmp)
					if (obits & (1<<(No-i-1))) // (!) MSB, (!) +- map
						metric -= (*tmp);
					else
						metric += (*tmp);

				bm(k,  obits       ) = +metric;
				bm(k, ~obits&O_MASK) = -metric;
			}

			std::advance(first, No);
		}
	}

	// calculate final metrics
	void viterbi_iteration(const BranchMetrics &bm,
		XPathMem &path, metric_vector &metrics) const;
	void viterbi_iteration(const BranchMetrics &bm,
		PathMem &path, metric_vector &metrics) const;

	// get decoded bits
	void trace_back(const XPathMem &path,
		state_type state, bit_vector &out) const;
	void trace_back(const PathMem &path,
		state_type state, bit_vector &out) const;

	// find start state
	static state_type find_start_state(const XPathMem &path, state_type state);
	static state_type find_start_state(const PathMem &path, state_type state);

	// check for fast trellis
	bool check_fast(const Trellis &tr);

private:
	Trellis m_trellis;

	// in simple case of 1/x code rate
	std::vector<bits_type> m_X_trellis;
};


//////////////////////////////////////////////////////////////////////////
// ConvEncoder class
class ConvEncoder: private ConvCodec {
	typedef ConvCodec inherited;

public:
	typedef Trellis::state_type state_type;
	typedef Trellis::bits_type bits_type;
	typedef Trellis::size_type size_type;
	typedef std::vector<bits_type> output_type;

public:
	explicit ConvEncoder(const Trellis &tr);

	// publish ConvCodec::trellis()
	using inherited::trellis;

public:
	// encode tail
	template<typename In>
		const output_type& encode_tail(In first, In last)
	{
		const size_type i_size = std::distance(first, last);

		const size_type Ni = trellis().N_ibits();
		const size_type No = trellis().N_obits();
		const size_type Nz = trellis().N_tails(false);

		// prepare output buffer
		m_out.resize((i_size/Ni + Nz) * No);

		inherited::encode_tail(first,
			last, m_out.begin());

		return out();
	}

	// encode tailbite
	template<typename In>
		const output_type& encode_bite(In first, In last)
	{
		const size_type i_size = std::distance(first, last);

		const size_type Ni = trellis().N_ibits();
		const size_type No = trellis().N_obits();

		// prepare output buffer
		m_out.resize((i_size/Ni) * No);

		inherited::encode_bite(first,
			last, m_out.begin());

		return out();
	}

	// encode tail
	template<typename In>
		const output_type& operator()(In first, In last)
	{
		return encode_bite(first, last);
	}
	template<typename TX, typename AX>
		const output_type& operator()(const std::vector<TX, AX> &x)
	{
		return encode_bite(x.begin(), x.end());
	}

public:
	const output_type& out() const;
	bits_type operator[](size_type k) const;
	size_type size() const;

private:
	output_type m_out; // coded (output) data
};


//////////////////////////////////////////////////////////////////////////
// ConvDecoder class
class ConvDecoder: public ConvCodec {
	typedef ConvCodec inherited;

public:
	typedef inherited::state_type state_type;
	typedef inherited::bits_type bits_type;
	typedef inherited::size_type size_type;
	typedef std::vector<bits_type> output_type;

public:
	explicit ConvDecoder(const Trellis &tr,
		size_type N_iters = 4);

	// publish ConvCodec::trellis()
	using inherited::trellis;

public:
	// decode tail
	template<typename In>
		const output_type& decode_tail(In first, In last)
	{
		const size_type i_size = std::distance(first, last);
		const size_type Ni = trellis().N_ibits();
		const size_type No = trellis().N_obits();
		const size_type Nz = trellis().N_tails(false);

		// prepare output buffer
		m_out.resize((i_size/No - Nz) * Ni);

		inherited::decode_tail(first,
			last, m_out.begin());

		return out();
	}

	// decode tailbite
	template<typename In>
		const output_type& decode_bite(In first, In last)
	{
		const size_type i_size = std::distance(first, last);
		const size_type Ni = trellis().N_ibits();
		const size_type No = trellis().N_obits();

		// prepare output buffer
		m_out.resize((i_size/No) * Ni);

		inherited::decode_bite(m_N_iters,
			first, last, m_out.begin());

		return out();
	}

	// decode tailbite
	template<typename In>
		const output_type& operator()(In first, In last)
	{
		return decode_bite(first, last);
	}

	// decode tailbite
	template<typename TX, typename AX>
		const output_type& operator()(const std::vector<TX, AX> &x)
	{
		return decode_bite(x.begin(), x.end());
	}

public:
	const output_type& out() const;
	bits_type operator[](size_type k) const;
	size_type size() const;

private:
	output_type m_out; // decoded (output) data
	size_type m_N_iters;
};

	} // ConvCodec


	// Interleaver
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
// Interleaver class
class Interleaver {
public:
	typedef std::vector<size_t> rule_type;

public:
	explicit Interleaver(const rule_type &irule);
	template<typename In>
		Interleaver(In first, In last)
	{
		m_irule.resize(std::distance(first, last));
		std::copy(first, last, m_irule.begin());

		assert(check_rule(m_irule)
			&& "Invalid interleaving rule");
	}

public:
	static rule_type create_matrix(bool row2col, size_t Nrows, size_t Ncols, size_t blockSize);
	static rule_type create_matrix(bool row2col, size_t Nrows, size_t Ncols);
	static bool check_rule(const rule_type &irule);

	const rule_type& rule() const;

public:
	template<typename In, typename Out>
		void interleave(In first, In last, Out out) const
	{
		const size_t N = std::distance(first, last);
		assert(N==m_irule.size() && "invalid input size!");

		for (size_t i = 0; i < N; ++i)
			out[i] = first[ m_irule[i] ];
	}
	template<typename TX, typename AX, typename TY, typename AY>
		void interleave(const std::vector<TX, AX> &x, std::vector<TY, AY> &y) const
	{
		y.resize(x.size());
		interleave(x.begin(),
			x.end(), y.begin());
	}

public:
	template<typename In, typename Out>
		void deinterleave(In first, In last, Out out) const
	{
		const size_t N = std::distance(first, last);
		assert(N==m_irule.size() && "invalid input size!");

		for (size_t i = 0; i < N; ++i)
			out[ m_irule[i] ] = first[i];
	}
	template<typename TX, typename AX, typename TY, typename AY>
		void deinterleave(const std::vector<TX, AX> &x, std::vector<TY, AY> &y) const
	{
		y.resize(x.size());
		deinterleave(x.begin(),
			x.end(), y.begin());
	}

private:
	rule_type m_irule;
};

	} // Interleaver
} // omni namespace

#endif // __OMNI_CODEC_H_
