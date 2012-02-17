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

		Файл содержит интерфейс и реализацию шаблонного
	класса omni::dsp::FIR_Filter.

@author Сергей Поличной
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
/// @brief КИХ-фильтр
template<typename T, typename CF>
	class FIR_Filter {
		typedef std::vector<CF> coef_array;
		typedef std::vector<T> basic_type;

	public: /// @name Определения типов
		typedef typename basic_type::const_reference const_reference;    ///< @brief Константная ссылка на отсчет
		typedef typename basic_type::reference             reference;    ///< @brief Ссылка на отсчет
		typedef typename basic_type::value_type value_type;              ///< @brief Тип отсчета
		typedef typename basic_type::size_type   size_type;              ///< @brief Тип индексов и размера

	public: /// @name Конструкторы
		FIR_Filter();

		template<typename In>
			FIR_Filter(In first, In last)    /// @brief Создать фильтр с заданными коэффициентами
				{ __init(first, last, value_type()); }
		template<typename In>
			FIR_Filter(In first, In last, const_reference x)  /// @brief Создать фильтр с заданными коэффициентами и заполнить
				{ __init(first, last, x); }

	public: /// @name Размер и содержимое
		size_type size() const;            ///< @brief Текущий размер КИХ-фильтра

		void reset();                      ///< @brief Обнулить фильтр
		void reset(const_reference x);     ///< @brief Заполнить фильтр заданным значением

	public: // ()
		value_type operator()(const_reference x); ///< @brief Фильтровать значение

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

	public: /// @name Коэффициенты фильтра
		typedef typename coef_array::const_iterator const_cf_iterator;       ///< @brief Константный итератор для коэффициентов фильтра
		typedef typename coef_array::      iterator       cf_iterator;       ///< @brief Итератор для коэффициентов фильтра

		const_cf_iterator cf_begin() const; ///< @brief Константный итератор на первый коэффициент фильтра
		      cf_iterator cf_begin();       ///< @brief Итератор на первый коэффициент фильтра
		const_cf_iterator cf_end() const;   ///< @brief Константный итератор за последний коэффициент фильтра
		      cf_iterator cf_end();         ///< @brief Итератор за последний коэффициент фильтра

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
/// @brief Создать всепропускающий фильтр
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
