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
		���� ���� �������� ������ ������������.

@author ������ ��������
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

@section omni_FIR_Filter_overview �����

		����� FIR_Filter ��������� ���-������ (������ � �������� ����������
	���������������). ������ �������, ����������� �� ���� ���-�������,
	�������� � ����� ��������. ������� ���-������� �������� ����������
	����� ����������� ����� ��������.

@image html "FIR_Filter.png" ���������� ����� ���-�������

		�������� ������� @a T ���������� ��� �������� �������,
	� �������� @a CF - ��� ������������ �������. �������� �����
	������������ ��������� ���� ���-��������:

@code
FIR_Filter<double, double>
FIR_Filter< std::complex<double>, double>
@endcode

		�������� ���������� ���-������� �������� ��� ������������
	(���������� ��������������), ������� � ���������� ������ ���-�������
	(������ ���������� ����� ��������). ������ ������������ �����
	� ����� �� �������������. ����� �������� ������� ������������
	����� �������� ������ ����� ������������ ������ �������,
	�������� ���:

@code
typedef FIR_Filter<double, double> MyFilter;
MyFilter filter(coef1.begin(), coef1.end());
// ...
filter = MyFilter(coef2.begin(), coef2.end());
@endcode

		���� ������� ������ ��� �������������, �� �� ������ ������� ������
	����� ����, � ������� �� ������������ �� ���������, ������� �������
	��������������� ������.

		������ ��� ������ � ������������� ������� ����� ��������
	� ������� ���� ������� cf_begin() � cf_end().


@section omni_FIR_Filter_example ������� �������������

		��������� ��������� ���� ������������� ������� �������������
	���-�������.

@subsection omni_FIR_Filter_example1 �������� ������������� �� �����

		������ ������������� �������� ������������� ������� ��
	���������� ����� "coef.dat".

@code
// ��������� ��������� ����
std::ifstream i_file("coef.dat");
if (!i_file) throw "file not found!";

// ������ ������������ �� ��������� �����
std::vector<double> coef;
std::copy(std::istream_iterator<double>(i_file),
	std::istream_iterator(), std::back_inserter(coef));
if (coef.empty()) throw "no coefficients!";

// ������� ���-������
FIR_Filter<double, double> filter(coef.begin(), coef.end());

// ...
@endcode


@subsection omni_FIR_Filter_example2 ���������� ��������������

		������� ������� ���������� �������������� ���-�������. ��� �����
	�� ���� ������� �������� �������, � ��������� ����.

@code
template<typename T, typename CF>
	void f(const FIR_Filter<T, CF> &f)
	{
		f.reset(); // �������� ������
		for (size_t i = 0; i < f.size(); ++i)
		{
			T x = i ? T(0) : T(1); // ������ �������, ��������� ����
			T y = f(x); // ����� �������

			std::cout << y << "\n";
		}
	}
@endcode

@author ������ ��������
*/


//////////////////////////////////////////////////////////////////////////
/** @fn FIR_Filter<T, CF>::FIR_Filter()

		����������� �� ��������� ������� ��������������� ���-������
	(������������ ����������� �������� ����� �������). �������
	���������� �� ���� ���-�������, ����� �� ����������� �� ��� ������.
	������������� ��� ������:

@code
const_reference operator()(const_reference x)
{
	return x;
}
@endcode
*/

//////////////////////////////////////////////////////////////////////////
/** @fn FIR_Filter<T, CF>::FIR_Filter(In first, In last)

		����������� ��������� ������ ������������ �������.
	������ ������� ���������� ������ (@a last - @a first).
	���������� ���������� ����� �������� ����������������
	���������� �� ���������.

		���� (@a first == @a last), �.�. ��� �������������,
	�� �� ������ ������� ������ ����� ����.

@param first �������� �� ������ �����������
@param last �������� �� ��������� �����������
*/

//////////////////////////////////////////////////////////////////////////
/** @fn FIR_Filter<T, CF>::FIR_Filter(In first, In last, const_reference x)

		����������� ��������� ������ ������������ ������� � ����������������
	��� ����������. ������ ������� ���������� ������ (@a last - @a first).
	���������� ���������� ����� �������� ���������������� ���������� @a x.

		���� (@a first == @a last), �.�. ��� �������������,
	�� �� ������ ������� ������ ����� ����.

@param first �������� �� ������ �����������
@param last �������� �� ��������� �����������
@param x ��������, ������� ����� ���������������� ���������� ����� ��������
*/

//////////////////////////////////////////////////////////////////////////
/** @fn void FIR_Filter<T, CF>::reset()

		����� �������� ������. ���������� ���������� ����� ��������
	����������� ���������� �� ���������.
*/

//////////////////////////////////////////////////////////////////////////
/** @fn void FIR_Filter<T, CF>::reset(const_reference x)

		����� ��������� ���������� ���������� ����� �������� ���������� @a x.

@param x ��������, ������� ����� ��������� ���������� ����� ��������
*/

//////////////////////////////////////////////////////////////////////////
/** @fn value_type FIR_Filter<T, CF>::operator()(const_reference x)

		����� ��������� ����� ������ @a x, ������� ����������� �� ����������
	����� ��������. ������������ ���������� ����� ����������� ���-������.

@param x ����������� ������
@return ������������� ������
*/

//////////////////////////////////////////////////////////////////////////
/** @fn size_type FIR_Filter<T, CF>::size() const

		����� ���������� ������ ���-�������, ������� ������������ �����������
	�������������, �������� ��� �������� �������.

@return ������ ���-�������
*/

//////////////////////////////////////////////////////////////////////////
/** @fn coef_iterator FIR_Filter<T, CF>::cf_begin() const

		����� ���������� �������� �� ������ ����������� �������.
	������������ �������� ������ ��� ������.

@return �������� �� ������ �����������
*/

//////////////////////////////////////////////////////////////////////////
/** @fn coef_iterator FIR_Filter<T, CF>::cf_end() const

		����� ���������� �������� �� ��������� ����������� �������.
	������������ �������� ������ ��� ������.

@return �������� �� ��������� �����������
*/

	} // dsp namespace
} // omni namespace
