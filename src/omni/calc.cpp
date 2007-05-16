//////////////////////////////////////////////////////////////////////////
//		This material is provided "as is", with absolutely no warranty
//	expressed or implied. Any use is at your own risk.
//
//		Permission to use or copy this software for any purpose is hereby
//	granted without fee, provided the above notices are retained on all
//	copies. Permission to modify the code and to distribute modified code
//	is granted, provided the above notices are retained, and a notice that
//	the code was modified is included with the above copyright notice.
//////////////////////////////////////////////////////////////////////////
/** @file
	@brief Implementation of the calculator tools.

@author Sergey Polichnoy
@see @ref omni_calc
*/
#include <omni/calc.hpp>
#include <omni/util.hpp>

#include <string.h>
#include <math.h>

namespace omni
{

//////////////////////////////////////////////////////////////////////////
/** @brief Evaluation tools.

		The calc namespace contains
		������������ ���� �������� �������� ��� ���������� ���������
	�� ������ ����� ��� ������.

@see @ref omni_calc
*/
	namespace calc
	{

//////////////////////////////////////////////////////////////////////////
/// @brief ������� �����������
/**
		������� ���������� �����������, ������� ����� ���������
	������������ ��������.

	������������ ��������� ���������� �������:
		- @b sin �����
		- @b cos �������
		- @b tan �������
		- @b sinh ��������������� �����
		- @b cosh ��������������� �������
		- @b tanh ��������������� �������
		- @b asin ��������
		- @b acos ����������
		- @b atan ����������
		- @b sqrt ���������� ������
		- @b exp ����������
		- @b ln ����������� ��������
		- @b lg ���������� ��������
		- @b floor ���������� ����
		- @b ceil ���������� �����
		- @b abs ������ �����

	� ��������� ��������� �������:
		- @b degr, @b deg �������������� �� �������� � �������

@return �����������
*/
const Calculator<double>& sci()
{
	// Aux
	struct Aux {
		static Calculator<double> init()
		{
			Calculator<double> calculator;

			calculator.prefix().insert(L"sin", UserFunc<double>(sin));
			calculator.prefix().insert(L"cos", UserFunc<double>(cos));
			calculator.prefix().insert(L"tan", UserFunc<double>(tan));

			calculator.prefix().insert(L"sinh", UserFunc<double>(sinh));
			calculator.prefix().insert(L"cosh", UserFunc<double>(cosh));
			calculator.prefix().insert(L"tanh", UserFunc<double>(tanh));

			calculator.prefix().insert(L"asin", UserFunc<double>(asin));
			calculator.prefix().insert(L"acos", UserFunc<double>(acos));
			calculator.prefix().insert(L"atan", UserFunc<double>(atan));

			calculator.prefix().insert(L"sqrt", UserFunc<double>(sqrt));
			calculator.prefix().insert(L"exp", UserFunc<double>(exp));
			calculator.prefix().insert(L"ln", UserFunc<double>(log));
			calculator.prefix().insert(L"lg", UserFunc<double>(log10));

			calculator.prefix().insert(L"floor", UserFunc<double>(floor));
			calculator.prefix().insert(L"ceil", UserFunc<double>(ceil));
			calculator.prefix().insert(L"abs", UserFunc<double>(fabs));

			calculator.suffix().insert(L"degr", UserFunc<double>(util::deg2rad));
			calculator.suffix().insert(L"deg", UserFunc<double>(util::deg2rad));

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Aux::init();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
/// @brief ����������� ������������ �������� (���������)
/**
		������� ���������� �����������, ������� ����� ���������
	������������ ��������.

	������������ ��������� ��������� �������:
		- @b dB ������� �� ��������� �� �������� � ����

@return �����������
*/
const Calculator<double>& ratio()
{
	// Aux
	struct Aux {
		static Calculator<double> init()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"dB", UserFunc<double>(util::dB2line));

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Aux::init();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
/// @brief ����������� �������� � ������
/**
		������� ���������� �����������, ������� ����� ���������
	�������� � ������.

	������������ ��������� ��������� �������:
		- @b MW ������� �� �������� (1e+6) � �����
		- @b KW ������� �� �������� (1e+3) � �����
		- @b W �������� ��� ����� ������� ���������
		- @b mW ������� �� �������� (1e-3) � �����
		- @b uW ������� �� ��������� (1e-6) � �����
		- @b dBm ������� �� dBm � �����
		- @b dBW ������� �� dBW � �����

@return �����������
*/
const Calculator<double>& power()
{
	// Aux
	struct Aux {
		static Calculator<double> init()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"MW", Multiplier<double, 1000000L>());
			calculator.suffix().insert(L"KW", Multiplier<double, 1000L>());
			calculator.suffix().insert(L"W",  Multiplier<double, 1L>());
			calculator.suffix().insert(L"mW", Divider<double, 1000L>());
			calculator.suffix().insert(L"uW", Divider<double, 1000000L>());
			calculator.suffix().insert(L"dBm", UserFunc<double>(util::dBm2watt));
			calculator.suffix().insert(L"dBW", UserFunc<double>(util::dB2line));

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Aux::init();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
/// @brief ����������� ������� � ��������
/**
		������� ���������� �����������, ������� ����� ���������
	����� � ��������.

	������������ ��������� ��������� �������:
		- @b day, @b days ������� �� ���� � �������
		- @b hour, @b hours ������� �� ����� � �������
		- @b min ������� �� ����� � �������
		- @b sec, @b s �������� ��� ����� ������� ���������
		- @b ms ������� ���������� (1e-3) � �������
		- @b us ������� ����������� (1e-6) � �������
		- @b ns ������� ���������� (1e-9) � �������

@return �����������
*/
const Calculator<double>& time()
{
	// Aux
	struct Aux {
		static Calculator<double> init()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"day", Multiplier<double, 24*60*60L>());
			calculator.suffix().insert(L"days", Multiplier<double, 24*60*60L>());
			calculator.suffix().insert(L"hour", Multiplier<double, 60*60L>());
			calculator.suffix().insert(L"hours", Multiplier<double, 60*60L>());
			calculator.suffix().insert(L"min", Multiplier<double, 60L>());
			calculator.suffix().insert(L"sec",  Multiplier<double, 1L>());
			calculator.suffix().insert(L"s",  Multiplier<double, 1L>());
			calculator.suffix().insert(L"ms", Divider<double, 1000L>());
			calculator.suffix().insert(L"us", Divider<double, 1000000L>());
			calculator.suffix().insert(L"ns", Divider<double, 1000000000L>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Aux::init();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
/// @brief ����������� ������� � ������
/**
		������� ���������� �����������, ������� ����� ���������
	������� � ������.

	������������ ��������� ��������� �������:
		- @b GHz ��������� ��������� (1e+9) � �����
		- @b MHz ��������� ��������� (1e+6) � �����
		- @b KHz, @b kHz ��������� ��������� (1e+3) � �����
		- @b Hz �������� ��� ����� ������� ���������

@return �����������
*/
const Calculator<double>& freq()
{
	// Aux
	struct Aux {
		static Calculator<double> init()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"GHz", Multiplier<double, 1000000000L>());
			calculator.suffix().insert(L"MHz", Multiplier<double, 1000000L>());
			calculator.suffix().insert(L"KHz", Multiplier<double, 1000L>());
			calculator.suffix().insert(L"kHz", Multiplier<double, 1000L>());
			calculator.suffix().insert(L"Hz",  Multiplier<double, 1L>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Aux::init();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
/// @brief ����������� ������� � ����� � ������.
/**
		������� ���������� �����������, ������� ����� ���������
	������� ��� � �����, ��� � � ������.

	������������ ��������� ��������� �������:
		- @b B, @b b �������� ��� ����� ������� ���������
		- @b KB, @b Kb ��������� �������� (1024) � ����
		- @b MB, @b Mb ��������� �������� (1024*1024) � ����
		- @b GB, @b Gb ��������� �������� (1024*1024*1024) � ����


@return �����������
*/
const Calculator<double>& bits()
{
	// Aux
	struct Aux {
		static Calculator<double> init()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"B", Multiplier<double, 1L>());
			calculator.suffix().insert(L"b", Multiplier<double, 1L>());
			calculator.suffix().insert(L"KB", Multiplier<double, 1024L>());
			calculator.suffix().insert(L"Kb", Multiplier<double, 1024L>());
			calculator.suffix().insert(L"MB", Multiplier<double, 1024L*1024>());
			calculator.suffix().insert(L"Mb", Multiplier<double, 1024L*1024>());
			calculator.suffix().insert(L"GB", Multiplier<double, 1024L*1024*1024>());
			calculator.suffix().insert(L"Gb", Multiplier<double, 1024L*1024*1024>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Aux::init();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
/// @brief ����������� ���������� � ������.
/**
		������� ���������� �����������, ������� ����� ���������
	���������� � ������.

	������������ ��������� ��������� �������:
		- @b nm, ��������� ��������� (0.000000001) � �����
		- @b um, ��������� ���������� (0.000001) � �����
		- @b mm, ��������� ��������� (0.001) � �����
		- @b cm, ��������� ���������� (0.01) � �����
		- @b m, �������� ��� ����� ������� ���������
		- @b km, @b Km ��������� ��������� (1000) � �����


@return �����������
*/
const Calculator<double>& dist()
{
	// Aux
	struct Aux {
		static Calculator<double> init()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"nm", Divider<double, 1000L*1000*1000>());
			calculator.suffix().insert(L"um", Divider<double, 1000L*1000>());
			calculator.suffix().insert(L"mm", Divider<double, 1000L>());
			calculator.suffix().insert(L"cm", Divider<double, 100L>());
			calculator.suffix().insert(L"m",  Multiplier<double, 1L>());
			calculator.suffix().insert(L"Km", Multiplier<double, 1000L>());
			calculator.suffix().insert(L"km", Multiplier<double, 1000L>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Aux::init();
	return g_CALC;
}


/// @cond details
namespace details
{

//////////////////////////////////////////////////////////////////////////
/// @brief ��������� ������������ ����� �������
/**
		��������� ������������ ����� ���������������� �������.
	��� ������ �������� �� ������ ����� � ���������� � �������.

@param[in] name ��� ���������������� �������
@return @b true ���� ��� ����������, ����� @b false
*/
	bool check_func_name(const std::wstring &name)
	{
		if (name.empty())
			return false;

		std::wistringstream is(name);
		if (get_func_name(is) != name)
			return false;

		return true;
	}

} // details namespace
/// @endcond

	} // calc namespace


	// exceptions...
	namespace calc
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Evaluation tools: exceptions.
/**
		This namespace contains exception classes
	related to the evaluation tools.
*/
		namespace err
		{

//////////////////////////////////////////////////////////////////////////
/// @brief ������� ����������
/**
@param[in] msg ��������� �� ������
*/
Failure::Failure(const std::string &msg)
	: inherited(msg)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief ������� ����������
/**
@param[in] msg ��������� �� ������
*/
Failure::Failure(const char *msg)
	: inherited(msg)
{}



//////////////////////////////////////////////////////////////////////////
/// @brief ������� ���������� � �����������
/**
@param[in] msg ��������� �� ������
*/
SyntaxError::SyntaxError(const std::string &msg)
	: inherited(msg)
{}

//////////////////////////////////////////////////////////////////////////
/// @brief ������� ���������� � �����������
/**
@param[in] msg ��������� �� ������
*/
SyntaxError::SyntaxError(const char *msg)
	: inherited(msg)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief ������� ���������� � �����������
/**
@param[in] func_name ��� �������
*/
UnknownFunctionCall::UnknownFunctionCall(const std::wstring &func_name)
	: inherited("unknown function call"), m_func_name(func_name)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief ��� �������������� �������
/**
		����� ���������� ��� �������, ������������� � ���������,
	�� �� ������������ � ������� ���������������� ������� ������������.

@return ��� �������������� �������
*/
const std::wstring& UnknownFunctionCall::funcName() const
{
	return m_func_name;
}


//////////////////////////////////////////////////////////////////////////
/// @brief ������� ���������� � �����������
/**
@param[in] msg ��������� �� ������
*/
CalculationError::CalculationError(const std::string &msg)
	: inherited(msg)
{}

//////////////////////////////////////////////////////////////////////////
/// @brief ������� ���������� � �����������
/**
@param[in] msg ��������� �� ������
*/
CalculationError::CalculationError(const char *msg)
	: inherited(msg)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief ����������� � �������� ���������
/**
*/
DivisionByZero::DivisionByZero()
	: inherited("division by zero")
{}


//////////////////////////////////////////////////////////////////////////
/// @brief ����������� � �������� ���������
/**
@param[in] func_name ��� �������
*/
InvalidFunctionName::InvalidFunctionName(const std::wstring &func_name)
	: inherited("invalid function name"), m_func_name(func_name)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief ����������� � �������� ���������
/**
@param[in] msg The error message.
@param[in] func_name The function name.
*/
InvalidFunctionName::InvalidFunctionName(const std::string &msg, const std::wstring &func_name)
	: inherited(msg), m_func_name(func_name)
{}

//////////////////////////////////////////////////////////////////////////
/// @brief ����������� � �������� ���������
/**
@param[in] msg The error message.
@param[in] func_name The function name.
*/
InvalidFunctionName::InvalidFunctionName(const char *msg, const std::wstring &func_name)
	: inherited(msg), m_func_name(func_name)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief ��� �������
/**
		����� ���������� ��� ������������ �������.

@return ��� ������������ �������
*/
const std::wstring& InvalidFunctionName::funcName() const
{
	return m_func_name;
}

		} // err namespace
	} // calc namespace


	// constants...
	namespace calc
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Evaluation tools: implementation.
/**
		This namespace contains implementation specific tools.

@warning Do not use this namespace.
*/
		namespace details
		{
			/// @cond details

// explicit <wchar_t> instantiation
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::FLOAT_CHARS[] = L".0123456789";
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::INT_CHARS[]   = L"0123456789";
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OCT_INDICATOR  = L'0';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::HEX_INDICATOR1 = L'X';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::HEX_INDICATOR2 = L'x';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::BRACE_OPEN  = L'(';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::BRACE_CLOSE = L')';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_ADD = L'+';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_SUB = L'-';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_MUL = L'*';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_DIV = L'/';

template<>
bool CharConst<wchar_t>::is_float_digit(Char cx)
{
	return wcschr(FLOAT_CHARS, cx) != 0;
}

template<>
bool CharConst<wchar_t>::is_int_digit(Char cx)
{
	return wcschr(INT_CHARS, cx) != 0;
}

template<>
wchar_t CharConst<wchar_t>::widen(Char cx, const std::ctype<Char>&)
{
	return cx;
}


// explicit <char> instantiation
template<> const CharConst<char>::Char CharConst<char>::FLOAT_CHARS[] = ".0123456789";
template<> const CharConst<char>::Char CharConst<char>::INT_CHARS[]   = "0123456789";
template<> const CharConst<char>::Char CharConst<char>::OCT_INDICATOR  = '0';
template<> const CharConst<char>::Char CharConst<char>::HEX_INDICATOR1 = 'X';
template<> const CharConst<char>::Char CharConst<char>::HEX_INDICATOR2 = 'x';
template<> const CharConst<char>::Char CharConst<char>::BRACE_OPEN  = '(';
template<> const CharConst<char>::Char CharConst<char>::BRACE_CLOSE = ')';
template<> const CharConst<char>::Char CharConst<char>::OP_ADD = '+';
template<> const CharConst<char>::Char CharConst<char>::OP_SUB = '-';
template<> const CharConst<char>::Char CharConst<char>::OP_MUL = '*';
template<> const CharConst<char>::Char CharConst<char>::OP_DIV = '/';

template<>
bool CharConst<char>::is_float_digit(Char cx)
{
	return strchr(FLOAT_CHARS, cx) != 0;
}

template<>
bool CharConst<char>::is_int_digit(Char cx)
{
	return strchr(INT_CHARS, cx) != 0;
}


template<>
wchar_t CharConst<char>::widen(Char cx, const std::ctype<Char> &fac)
{
	return fac.widen(cx);
}


			/// @endcond
		} // details namespace
	} // calc namespace

} // omni namespace


//////////////////////////////////////////////////////////////////////////
/** @page omni_calc Evaluation tools

		����������� ������������ ��� ���������� ��������� ���������.
	������������, ��������, ��� ������ ���������� �� ����� ������������
	��� ����� �����. ����������� ������������ �������� ��������������
	�������� � ����� ����������� ������������� ���������.

		����������� ������������ ��������� �������� ��������������
	�������� (� ������� �������� ����������):
			- ������, ������� ����, ������� �����, ����� �������
			- ���������, �������
			- ��������, ���������

			�� ��������� �������� ��� ������������: � ������������� � ������
	�������, �������������� omni::calc::evalf() � omni::calc::evali().
	��� ������� ������������ �������� ����������� ����������
	@a atof � @a atoi ��������������, �� ����� ��������� �������
	�������������� �������� (�� �������� ������� ����������).


@section omni_calc_func ������� ����������
		����������� ������������ ���������� ������������� ���������.
	���������������� ������� ������ ����� ��������� ��������:

		@code T user_func(T x) @endcode

		� ��������� ����� ���� ����������� ��� ���� �������: ����������
	� ����������. ���������� ������� � �������� ������������ ���
	������� ������ ��������� � ���������������� ���������������.
	��������, ���������

@code
"3.0 dB"
@endcode

		������������ ���������

@code
"dB2line(3.0)"
@endcode

		�������� ���������� ������� ������ ������ ������������� � �������!
	��������:

@code
"abs(-3.0)"
@endcode

		��� ���������� ������� ���������������� ������� ������������
	��������� ������ omni::calc::UserFuncTable::insert()
	� omni::calc::UserFuncTable::remove().


@section omni_calc_using ������������� ������������

		��� ��� ���� ������� ����, ����������� ����� ������������ ���
	�������������� ����������������� ����� (������ �� �����������������
	����� ��� ����� ����� �������) � �����.

		��������, ����� �������� ��� ���������: ����� ������������� � ��������
	� ������� ������� � ������. ��� �������� ������������ �� ����� ���
	��������� �������� ��� �������� ������� ����������:

@code
	simulation_time = "120 min"
	carrier_freq = "2.4 MHz"
@endcode

		���

@code
	simulation_time = "2*60 min"
	carrier_freq = "2400 KHz"
@endcode

		���

@code
	simulation_time = "2 hour"
	carrier_freq = "2400000 Hz"
@endcode

		��� ����� ������� ��� ������������: ���� ����� ��������� �����,
	� ������ �������. �����, �������, ������������ � ���� �����������,
	�� ����� ������������ �������� ����������� � ���� ����� ��� �������
	������ ������� (MHz, KHz, Hz) � ��������.

@code
	omni::calc::Calculator<double> calc_sec;
	omni::calc::Calculator<double> calc_Hz;
@endcode

		� ��������� ��������������� ������� ���������������

@code
	calc_sec.suffix().insert("hour", omni::calc::Multiplier<double, 60*60>());
	calc_sec.suffix().insert("min",  omni::calc::Multiplier<double, 60>());
	calc_sec.suffix().insert("sec",  omni::calc::Multiplier<double, 1>());

	calc_Hz.suffix().insert("MHz", omni::calc::Multiplier<double, 1000000>());
	calc_Hz.suffix().insert("KHz", omni::calc::Multiplier<double, 1000>());
	calc_Hz.suffix().insert("Hz",  omni::calc::Multiplier<double, 1>());
@endcode

	������, ����� ������������ ��� ������������
��� �������������� ����������������� �����:

@code
	std::string s_secs;
	std::cout << "simulation time [s]: ";
	std::cin >> s_secs;
	double x_secs = calc_sec(s_secs);

	std::string s_freq;
	std::cout << "carrier frequency [Hz]: ";
	std::cin >> s_freq;
	double x_freq = calc_Hz(s_freq);
@endcode

		��������� ���������� ������� �� �������� ��������� � ������
	������������� ��� �������� ������� ���������, ��������: @a Hz ��� @a sec.

		� ������� ��������� ��������� ��������� ������. � �������� ����������,
	��� ���������� ��������� ����� �������������� ����������, �������
	������� ���� �� �������������.

@section omni_calc_def ��������� ������������

	By default the following calculators are available:
		- sci() - the scientific calculator.
		- ratio() - the custom ratio.
		- power() - the power in watts.
		- time() - the time in seconds.
		- freq() - the frequency in Hertz.
		- bits() - the size in bits or bytes.
		- dist() - the distance in meters.
*/
