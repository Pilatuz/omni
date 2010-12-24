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
		Файл пока содержит только документацию.

@author Сергей Поличной
*/
#include <omni/filter.h>
#include <omni/util.hpp>

namespace omni
{
	namespace dsp
	{
		namespace details
		{

	// complex<double> * complex<double>
	std::complex<double> filter_dot(const std::vector< std::complex<double> > &xbuf,
		const std::vector< std::complex<double> > &coef, size_t wpos, size_t N)
	{
		double res_re = 0.0;
		double res_im = 0.0;

		for (size_t i = 0; i < wpos; ++i)
		{
			const std::complex<double> &x  = xbuf[i];
			const std::complex<double> &cf = coef[i+N-wpos];

			// res += cf * x;
			res_re += x.real()*cf.real() - x.imag()*cf.imag();
			res_im += x.real()*cf.imag() + x.imag()*cf.real();
		}
		for (size_t i = wpos; i < N; ++i)
		{
			const std::complex<double> &x  = xbuf[i];
			const std::complex<double> &cf = coef[i-wpos];

			// res += cf * x;
			res_re += x.real()*cf.real() - x.imag()*cf.imag();
			res_im += x.real()*cf.imag() + x.imag()*cf.real();
		}

		return std::complex<double>(res_re, res_im);
	}

	// complex<double> * double
	std::complex<double> filter_dot(const std::vector< std::complex<double> > &xbuf,
		const std::vector<double> &coef, size_t wpos, size_t N)
	{
		double res_re = 0.0;
		double res_im = 0.0;

		for (size_t i = 0; i < wpos; ++i)
		{
			const std::complex<double> &x = xbuf[i];
			const double &cf = coef[i+N-wpos];

			// res += cf * x;
			res_re += x.real()*cf;
			res_im += x.imag()*cf;
		}
		for (size_t i = wpos; i < N; ++i)
		{
			const std::complex<double> &x = xbuf[i];
			const double &cf = coef[i-wpos];

			// res += cf * x;
			res_re += x.real()*cf;
			res_im += x.imag()*cf;
		}

		return std::complex<double>(res_re, res_im);
	}

	// complex<float> * complex<float>
	std::complex<float> filter_dot(const std::vector< std::complex<float> > &xbuf,
		const std::vector< std::complex<float> > &coef, size_t wpos, size_t N)
	{
		float res_re = 0.0;
		float res_im = 0.0;

		for (size_t i = 0; i < wpos; ++i)
		{
			const std::complex<float> &x  = xbuf[i];
			const std::complex<float> &cf = coef[i+N-wpos];

			// res += cf * x;
			res_re += x.real()*cf.real() - x.imag()*cf.imag();
			res_im += x.real()*cf.imag() + x.imag()*cf.real();
		}
		for (size_t i = wpos; i < N; ++i)
		{
			const std::complex<float> &x  = xbuf[i];
			const std::complex<float> &cf = coef[i-wpos];

			// res += cf * x;
			res_re += x.real()*cf.real() - x.imag()*cf.imag();
			res_im += x.real()*cf.imag() + x.imag()*cf.real();
		}

		return std::complex<float>(res_re, res_im);
	}

	// complex<float> * float
	std::complex<float> filter_dot(const std::vector< std::complex<float> > &xbuf,
		const std::vector<float> &coef, size_t wpos, size_t N)
	{
		float res_re = 0.0;
		float res_im = 0.0;

		for (size_t i = 0; i < wpos; ++i)
		{
			const std::complex<float> &x = xbuf[i];
			const float &cf = coef[i+N-wpos];

			// res += cf * x;
			res_re += x.real()*cf;
			res_im += x.imag()*cf;
		}
		for (size_t i = wpos; i < N; ++i)
		{
			const std::complex<float> &x = xbuf[i];
			const float &cf = coef[i-wpos];

			// res += cf * x;
			res_re += x.real()*cf;
			res_im += x.imag()*cf;
		}

		return std::complex<float>(res_re, res_im);
	}

		} // details namespace


//////////////////////////////////////////////////////////////////////////
// design raised cosine filter
std::vector<double> rcosine(double R, size_t len, size_t Ns)
{
	using omni::util::PI;

	assert(0.0<=R && R<=1.0 && "roll-off factor must be in range [0,1]");
	assert(0<len && !(len&1) && "filter length must be even integer");
	assert(0<Ns && "upsampling factor can't be zero");

#define SINC(x) (0.0==x ? 1.0 : sin(PI*x)/(PI*x))

	std::vector<double> cf(len*Ns + 1, 0.0);
	for (size_t i = 0; i < cf.size(); ++i)
	{
		double t = (i - Ns*len/2.0) / Ns;
		double den = 1.0 - (2*R*t)*(2*R*t);

		cf[i] = SINC(t) * (0.0==den
			? PI/4.0 : cos(R*PI*t)/den);
	}

#undef SINC

	// normalization...
	//const double nf = 1.0 / sqrt(double(Ns));
	//for (size_t i = 0; i < cf.size(); ++i)
	//	cf[i] *= nf;

	return cf;
}


//////////////////////////////////////////////////////////////////////////
// design square root raised cosine filter
std::vector<double> rrcosine(double R, size_t len, size_t Ns)
{
	using omni::util::SQRT2;
	using omni::util::PI;

	assert(0.0<=R && R<=1.0 && "roll-off factor must be in range [0,1]");
	assert(0<len && !(len&1) && "filter length must be even integer");
	assert(0<Ns && "upsampling factor can't be zero");

	std::vector<double> cf(len*Ns + 1, 0.0);
	for (size_t i = 0; i < cf.size(); ++i)
	{
		double t = (i - Ns*len/2.0) / Ns;
		double den = 1.0 - (4*R*t)*(4*R*t);

		if (0.0 == t)
			cf[i] = 1.0 + (4*R/PI) - R;
		else if (0.0 == den)
			cf[i] = R/SQRT2 * ((1+2/PI) * sin(PI/(4*R)) + (1-2/PI) * cos(PI/(4*R)));
		else
			cf[i] = (sin(PI * (1-R)*t) + cos(PI * (1+R)*t) * 4*R*t) / (PI*t*den);
	}

	// normalization...
	const double nf = 1.0 / sqrt(double(Ns));
	for (size_t i = 0; i < cf.size(); ++i)
		cf[i] *= nf;

	return cf;
}



//////////////////////////////////////////////////////////////////////////
/** @class FIR_Filter

@section omni_FIR_Filter_overview Обзор

		Класс FIR_Filter реализует КИХ-фильтр (фильтр с конечной импульсной
	характеристикой). Отсчет сигнала, поступающий на вход КИХ-фильтра,
	попадает в линию задержки. Выходом КИХ-фильтра является взвешенная
	сумма содержимого линии задержки.

@image html "FIR_Filter.png" Логическая схема КИХ-фильтра

		Параметр шаблона @a T определяет тип отсчетов сигнала,
	а параметр @a CF - тип кэффициентов фильтра. Наиболее часто
	используются следующие типы КИХ-фильтров:

@code
FIR_Filter<double, double>
FIR_Filter< std::complex<double>, double>
@endcode

		Основным параметром КИХ-фильтра являются его коэффициенты
	(импульсная характеристика), которые и определяют размер КИХ-фильтра
	(размер внутренней линии задержки). Задать коэффициенты можно
	в одном из конструкторов. После создания фильтра коэффициенты
	можно изменить только через присваивание нового фильтра,
	например так:

@code
typedef FIR_Filter<double, double> MyFilter;
MyFilter filter(coef1.begin(), coef1.end());
// ...
filter = MyFilter(coef2.begin(), coef2.end());
@endcode

		Исли создать фильтр без коэффициентов, то на выходе фильтра всегда
	будут нули, в отличие от конструктора по умолчанию, который создает
	всепропоскающий фильтр.

		Доступ для чтения к коэффициентам фильтра можно получить
	с помощью пары методов cf_begin() и cf_end().


@section omni_FIR_Filter_example Примеры использования

		Следующие фрагменты кода демонстрируют примеры использования
	КИХ-фильтра.

@subsection omni_FIR_Filter_example1 Загрузка коэффициентов из файла

		Пример демонстрирует закрузку коэффициентов фильтра из
	текстового файла "coef.dat".

@code
// открываем текстовый файл
std::ifstream i_file("coef.dat");
if (!i_file) throw "file not found!";

// читаем коэффициенты во временный буфер
std::vector<double> coef;
std::copy(std::istream_iterator<double>(i_file),
	std::istream_iterator(), std::back_inserter(coef));
if (coef.empty()) throw "no coefficients!";

// создаем КИХ-фильтр
FIR_Filter<double, double> filter(coef.begin(), coef.end());

// ...
@endcode


@subsection omni_FIR_Filter_example2 Импульсная характеристика

		Функция выводит импульсную характеристику КИХ-фильтра. Для этого
	на вход фильтра подается единица, а остальные нули.

@code
template<typename T, typename CF>
	void f(const FIR_Filter<T, CF> &f)
	{
		f.reset(); // обнуляем фильтр
		for (size_t i = 0; i < f.size(); ++i)
		{
			T x = i ? T(0) : T(1); // первая единица, остальные нули
			T y = f(x); // выход фильтра

			std::cout << y << "\n";
		}
	}
@endcode

@author Сергей Поличной
*/


//////////////////////////////////////////////////////////////////////////
/** @fn FIR_Filter<T, CF>::FIR_Filter()

		Конструктор по умолчанию создает всепропускающий КИХ-фильтр
	(единственный коэффициент которого равен единице). Отсчеты
	попадающие на вход КИХ-фильтра, сразу же оказываются на его выходе.
	Эквивалентный код работы:

@code
const_reference operator()(const_reference x)
{
	return x;
}
@endcode
*/

//////////////////////////////////////////////////////////////////////////
/** @fn FIR_Filter<T, CF>::FIR_Filter(In first, In last)

		Конструктор позволяет задать коэффициенты фильтра.
	Размер фильтра становится равным (@a last - @a first).
	Содержимое внутренней линии задержки инициализируется
	значениями по умолчанию.

		Если (@a first == @a last), т.е. нет коэффициентов,
	то на выходе фильтра всегда будут нули.

@param first Итератор на первый коэффициент
@param last Итератор за последний коэффициент
*/

//////////////////////////////////////////////////////////////////////////
/** @fn FIR_Filter<T, CF>::FIR_Filter(In first, In last, const_reference x)

		Конструктор позволяет задать коэффициенты фильтра и инициализировать
	его содержимое. Размер фильтра становится равным (@a last - @a first).
	Содержимое внутренней линии задержки инициализируется значениями @a x.

		Если (@a first == @a last), т.е. нет коэффициентов,
	то на выходе фильтра всегда будут нули.

@param first Итератор на первый коэффициент
@param last Итератор за последний коэффициент
@param x Значение, которым будет инициализирована внутренняя линия задержки
*/

//////////////////////////////////////////////////////////////////////////
/** @fn void FIR_Filter<T, CF>::reset()

		Метод обнуляет фильтр. Содержимое внутренней линии задержки
	заполняется значениями по умолчанию.
*/

//////////////////////////////////////////////////////////////////////////
/** @fn void FIR_Filter<T, CF>::reset(const_reference x)

		Метод заполняет содержимое внутренней линии задержки значениями @a x.

@param x Значение, которым будет заполнена внутренняя линия задержки
*/

//////////////////////////////////////////////////////////////////////////
/** @fn value_type FIR_Filter<T, CF>::operator()(const_reference x)

		Метод фильтрует новый отсчет @a x, который добавляется во внутреннюю
	линию задержки. Возвращается взвешенная сумма содержимого КИХ-фильра.

@param x Фильтруемый отсчет
@return Фильтрованный отсчет
*/

//////////////////////////////////////////////////////////////////////////
/** @fn size_type FIR_Filter<T, CF>::size() const

		Метод возвращает размер КИХ-фильтра, который определяется количеством
	коэффициентов, заданным при создании фильтра.

@return Размер КИХ-фильтра
*/

//////////////////////////////////////////////////////////////////////////
/** @fn coef_iterator FIR_Filter<T, CF>::cf_begin() const

		Метод возвращает итератор на первый коэффициент фильтра.
	Коэффициенты доступны только для чтения.

@return Итератор на первый коэффициент
*/

//////////////////////////////////////////////////////////////////////////
/** @fn coef_iterator FIR_Filter<T, CF>::cf_end() const

		Метод возвращает итератор за последний коэффициент фильтра.
	Коэффициенты доступны только для чтения.

@return Итератор за последний коэффициент
*/

	} // dsp namespace
} // omni namespace
