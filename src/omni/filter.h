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

		���� �������� ��������� � ���������� ����������
	������ omni::dsp::FIR_Filter.

@author ������ ��������
*/
#ifndef __OMNI_FILTER_H_
#define __OMNI_FILTER_H_

#include <omni/defs.hpp>

#include <complex>
#include <vector>

namespace omni
{
	namespace dsp
	{

//////////////////////////////////////////////////////////////////////////
/// @brief ���-������
template<typename T, typename CF>
	class FIR_Filter {
		typedef std::vector<CF> coef_array;
		typedef std::vector<T> basic_type;

	public: /// @name ����������� �����
		typedef typename basic_type::const_reference const_reference;    ///< @brief ����������� ������ �� ������
		typedef typename basic_type::reference             reference;    ///< @brief ������ �� ������
		typedef typename basic_type::value_type value_type;              ///< @brief ��� �������
		typedef typename basic_type::size_type   size_type;              ///< @brief ��� �������� � �������

	public: /// @name ������������
		FIR_Filter();

		template<typename In>
			FIR_Filter(In first, In last)    /// @brief ������� ������ � ��������� ��������������
				{ __init(first, last, value_type()); }
		template<typename In>
			FIR_Filter(In first, In last, const_reference x)  /// @brief ������� ������ � ��������� �������������� � ���������
				{ __init(first, last, x); }

	public: /// @name ������ � ����������
		size_type size() const;            ///< @brief ������� ������ ���-�������

		void reset();                      ///< @brief �������� ������
		void reset(const_reference x);     ///< @brief ��������� ������ �������� ���������

	public: // ()
		value_type operator()(const_reference x); ///< @brief ����������� ��������

		// only put
		void put(const_reference x)
		{
			const size_type N = size();

			if (0 < N)
			{
				m_xbuf[m_wpos] = x;

				if (0 == m_wpos)
					m_wpos = N-1;
				else
					--m_wpos;
			}
		}

	public: /// @name ������������ �������
		typedef typename coef_array::const_iterator const_cf_iterator;       ///< @brief ����������� �������� ��� ������������� �������
		typedef typename coef_array::      iterator       cf_iterator;       ///< @brief �������� ��� ������������� �������

		const_cf_iterator cf_begin() const; ///< @brief ����������� �������� �� ������ ����������� �������
		      cf_iterator cf_begin();       ///< @brief �������� �� ������ ����������� �������
		const_cf_iterator cf_end() const;   ///< @brief ����������� �������� �� ��������� ����������� �������
		      cf_iterator cf_end();         ///< @brief �������� �� ��������� ����������� �������

	private: // initialization
		template<typename In>
			void __init(In first, In last, const_reference x)
		{
			size_type N = std::distance(first, last);

			m_coef.resize(N);
			std::copy(first, last,
				m_coef.begin());

			m_xbuf.assign(N, x);
			m_wpos = N-1;
		}

	private:
		basic_type m_xbuf;
		coef_array m_coef;
		size_type  m_wpos;
	};


//////////////////////////////////////////////////////////////////////////
// FIR filter design
std::vector<double>  rcosine(double R, size_t len, size_t Ns); // raised cosine
std::vector<double> rrcosine(double R, size_t len, size_t Ns); // root raise cosine


//////////////////////////////////////////////////////////////////////////
// FIR Filter implementation

namespace details
{

	// common implementation
	template<typename T, typename CF>
		T filter_dot(const std::vector<T> &xbuf, const std::vector<CF> &coef, size_t wpos, size_t N)
	{
		T res(coef[0] * xbuf[wpos]);

		for (size_t i = 0; i < wpos; ++i)
			res += coef[i+N-wpos] * xbuf[i];
		for (size_t i = wpos+1; i < N; ++i)
			res += coef[i-wpos] * xbuf[i];

		return res;
	}

	// complex<double> * complex<double>
	std::complex<double> filter_dot(const std::vector< std::complex<double> > &xbuf,
		const std::vector< std::complex<double> > &coef, size_t wpos, size_t N);

	// complex<double> * double
	std::complex<double> filter_dot(const std::vector< std::complex<double> > &xbuf,
		const std::vector<double> &coef, size_t wpos, size_t N);

	// complex<float> * complex<float>
	std::complex<float> filter_dot(const std::vector< std::complex<float> > &xbuf,
		const std::vector< std::complex<float> > &coef, size_t wpos, size_t N);

	// complex<float> * float
	std::complex<float> filter_dot(const std::vector< std::complex<float> > &xbuf,
		const std::vector<float> &coef, size_t wpos, size_t N);

} // details namespace


//////////////////////////////////////////////////////////////////////////
/// @brief ������� ��������������� ������
template<typename T, typename CF>
	FIR_Filter<T, CF>::FIR_Filter()
		: m_wpos(0)
{}

// fill by default value
template<typename T, typename CF> inline
	void FIR_Filter<T, CF>::reset()
{
	reset(value_type());
}

// fill by specified value
template<typename T, typename CF>
	void FIR_Filter<T, CF>::reset(const_reference x)
{
	std::fill(m_xbuf.begin(),
		m_xbuf.end(), x);
	m_wpos = size()-1;
}

// put value to filter
template<typename T, typename CF>
	typename FIR_Filter<T, CF>::value_type FIR_Filter<T, CF>::operator()(const_reference x)
{
	const size_type N = size();

	if (0 < N)
	{
		m_xbuf[m_wpos] = x;

		size_type wpos = m_wpos;
		if (0 == m_wpos)
			m_wpos = N-1;
		else
			--m_wpos;

		return details::filter_dot(m_xbuf, m_coef, wpos, N);
	}

	return x; // transparent
}

// get filter size
template<typename T, typename CF> inline
	typename FIR_Filter<T, CF>::size_type FIR_Filter<T, CF>::size() const
{
	return m_xbuf.size();
}

// get begin of filter's coefficients
template<typename T, typename CF> inline
	typename FIR_Filter<T, CF>::const_cf_iterator FIR_Filter<T, CF>::cf_begin() const
{
	return m_coef.begin();
}

// get begin of filter's coefficients
template<typename T, typename CF> inline
	typename FIR_Filter<T, CF>::cf_iterator FIR_Filter<T, CF>::cf_begin()
{
	return m_coef.begin();
}

// get end of filter's coefficients
template<typename T, typename CF> inline
	typename FIR_Filter<T, CF>::const_cf_iterator FIR_Filter<T, CF>::cf_end() const
{
	return m_coef.end();
}

// get end of filter's coefficients
template<typename T, typename CF> inline
	typename FIR_Filter<T, CF>::cf_iterator FIR_Filter<T, CF>::cf_end()
{
	return m_coef.end();
}

	} // dsp namespace
} // omni namespace

#endif // __OMNI_FILTER_H_
