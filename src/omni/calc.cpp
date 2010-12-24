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

		The calc namespace contains tools for expression
	evaluation from the input stream or from the string.

@see @ref omni_calc
*/
	namespace calc
	{

//////////////////////////////////////////////////////////////////////////
// The scientific calculator.
/**
		This function returns scientific calculator reference.
	This calculator can evaluate any non-dimensional expressions.

		The following operators are supported: "+", "-", "*", "/" and brackets.

	The following prefix functions are supported:
		- @b sin - Sine.
		- @b cos - Cosine.
		- @b tan - Tangent.
		- @b sinh - Hyperbolic sine.
		- @b cosh - Hyperbolic cosine.
		- @b tanh - Hyperbolic tangent.
		- @b asin - Inverse sine.
		- @b acos - Inverse cosine.
		- @b atan - Inverse tangent.
		- @b sqrt - Square root.
		- @b exp - Exponent.
		- @b ln - Natural logarithm.
		- @b lg - Decimal logarithm.
		- @b floor - Round down.
		- @b ceil - Round up.
		- @b abs - Absolute value.

	The following suffix functions are supported:
		- @b degr, @b deg - Conversion from degrees to radians.

@return The scientific calculator reference.
*/
const Calculator<double>& sci()
{
	// Init
	struct Init
	{
		static Calculator<double> get()
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

			calculator.suffix().insert(L"degr", UserFunc<double>(omni::util::deg2rad));
			calculator.suffix().insert(L"deg", UserFunc<double>(omni::util::deg2rad));

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
// The ratio calculator.
/**
		This function returns ratio calculator reference.
	This calculator can evaluate any non-dimensional expressions.

		The following operators are supported: "+", "-", "*", "/" and brackets.

	The following suffix functions are supported:
		- @b dB - Conversion from dB to linear scale (by power).

@return The ratio calculator reference.
@see omni::util::dB2line()
*/
const Calculator<double>& ratio()
{
	// Init
	struct Init {
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"dB", UserFunc<double>(omni::util::dB2line));

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
// The power calculator.
/**
		This function returns power (in watt) calculator reference.

		The following operators are supported: "+", "-", "*", "/" and brackets.

	The following suffix functions are supported:
		- @b MW - Conversion from megawatts (1.0e+6) to watts.
		- @b KW - Conversion from kilowatts (1.0e+3) to watts.
		- @b W - Actual, no conversion.
		- @b mW - Conversion from milliwatts (1.0e-3) to watts.
		- @b uW - Conversion from microwatts (1.0e-6) to watts.
		- @b dBm - Conversion from dBm to watts.
		- @b dBW - Conversion from dBW to watts.
		- @b dB - Conversion from dBW to watts.

@return The power calculator reference.
@see omni::util::dBm2watt()
@see omni::util::dB2line()
*/
const Calculator<double>& power()
{
	// Init
	struct Init {
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"MW", Multiplier<double, 1000000L>());
			calculator.suffix().insert(L"KW", Multiplier<double, 1000L>());
			calculator.suffix().insert(L"W",  Multiplier<double, 1L>());
			calculator.suffix().insert(L"mW", Divider<double, 1000L>());
			calculator.suffix().insert(L"uW", Divider<double, 1000000L>());
			calculator.suffix().insert(L"dBm", UserFunc<double>(omni::util::dBm2watt));
			calculator.suffix().insert(L"dBW", UserFunc<double>(omni::util::dB2line));
			calculator.suffix().insert(L"dB", UserFunc<double>(omni::util::dB2line));

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
// The time calculator.
/**
		This function returns time (in seconds) calculator reference.

		The following operators are supported: "+", "-", "*", "/" and brackets.

	The following suffix functions are supported:
		- @b day, @b days - Conversion from days (24*60*60) to seconds.
		- @b hour, @b hours - Conversion from hours (3600) to seconds.
		- @b min - Conversion from minutes (60) to seconds.
		- @b sec, @b s - Actually, no conversion.
		- @b ms - Conversion from milliseconds (1.0e-3) to seconds.
		- @b us - Conversion from microseconds (1.0e-6) to seconds.
		- @b ns - Conversion from nanoseconds (1.0e-9) to seconds.

@return The time calculator reference.
*/
const Calculator<double>& time()
{
	// Init
	struct Init {
		static Calculator<double> get()
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

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
// The frequency calculator.
/**
		This function returns frequency (in hertz) calculator reference.

		The following operators are supported: "+", "-", "*", "/" and brackets.

	The following suffix functions are supported:
		- @b GHz - Conversion from gigahertz (1.0e+9) to hertz.
		- @b MHz - Conversion from megahertz (1.0e+6) to hertz.
		- @b KHz, @b kHz - Conversion from kilohertz (1.0e+3) to hertz.
		- @b Hz - Actually, no conversion.

@return The frequency calculator reference.
*/
const Calculator<double>& freq()
{
	// Init
	struct Init {
		static Calculator<double> get()
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

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
// The voltage calculator.
/**
		This function returns voltage (in Volt) calculator reference.

		The following operators are supported: "+", "-", "*", "/" and brackets.

	The following suffix functions are supported:
		- @b GV - Conversion from gigavolt (1.0e+9) to volt.
		- @b MV - Conversion from megavolt (1.0e+6) to volt.
		- @b KV, @b kV - Conversion from kilovolt (1.0e+3) to volt.
		- @b V - Actually, no conversion.
		- @b mV - Conversion from millivolt (1.0e-3) to volt.
		- @b uV - Conversion from microvolt (1.0e-6) to volt.
		- @b nV - Conversion from nanovolt (1.0e-9) to volt.

@return The voltage calculator reference.
*/
const Calculator<double>& volt()
{
	// Init
	struct Init {
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"GV", Multiplier<double, 1000000000L>());
			calculator.suffix().insert(L"MV", Multiplier<double, 1000000L>());
			calculator.suffix().insert(L"KV", Multiplier<double, 1000L>());
			calculator.suffix().insert(L"kV", Multiplier<double, 1000L>());
			calculator.suffix().insert(L"V",  Multiplier<double, 1L>());
			calculator.suffix().insert(L"mV", Divider<double, 1000L>());
			calculator.suffix().insert(L"uV", Divider<double, 1000000L>());
			calculator.suffix().insert(L"nV", Divider<double, 1000000000L>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
// The bits calculator.
/**
		This function returns size (in bits or in bytes) calculator reference.

		The following operators are supported: "+", "-", "*", "/" and brackets.

	The following suffix functions are supported:
		- @b GB, @b Gb - Conversion from gigabits (1024*1024*1024) to bits.
		- @b MB, @b Mb - Conversion from megabits (1024*1024) to bits.
		- @b KB, @b Kb - Conversion from kilobits (1024) to bits.
		- @b B, @b b - Actually, no conversion.

@return The bits calculator reference.
*/
const Calculator<double>& bits()
{
	// Init
	struct Init {
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"GB", Multiplier<double, 1024L*1024*1024>());
			calculator.suffix().insert(L"Gb", Multiplier<double, 1024L*1024*1024>());
			calculator.suffix().insert(L"MB", Multiplier<double, 1024L*1024>());
			calculator.suffix().insert(L"Mb", Multiplier<double, 1024L*1024>());
			calculator.suffix().insert(L"KB", Multiplier<double, 1024L>());
			calculator.suffix().insert(L"Kb", Multiplier<double, 1024L>());
			calculator.suffix().insert(L"B", Multiplier<double, 1L>());
			calculator.suffix().insert(L"b", Multiplier<double, 1L>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


//////////////////////////////////////////////////////////////////////////
// The distance calculator.
/**
		This function returns distance (in meters) calculator reference.

		The following operators are supported: "+", "-", "*", "/" and brackets.

	The following suffix functions are supported:
		- @b km, @b Km - Conversion from kilometers (1.0e+3) to meters.
		- @b m - Actually, no conversion.
		- @b cm - Conversion from centimeters (1.0e-2) to meters.
		- @b mm - Conversion from millimeters (1.0e-3) to meters.
		- @b um - Conversion from micrometers (1.0e-6) to meters.
		- @b nm - Conversion from nanometers (1.0e-9) to meters.

@return The distance calculator reference.
*/
const Calculator<double>& dist()
{
	// Init
	struct Init {
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"Km", Multiplier<double, 1000L>());
			calculator.suffix().insert(L"km", Multiplier<double, 1000L>());
			calculator.suffix().insert(L"m",  Multiplier<double, 1L>());
			calculator.suffix().insert(L"cm", Divider<double, 100L>());
			calculator.suffix().insert(L"mm", Divider<double, 1000L>());
			calculator.suffix().insert(L"um", Divider<double, 1000L*1000>());
			calculator.suffix().insert(L"nm", Divider<double, 1000L*1000*1000>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


/// @cond details
namespace details
{

//////////////////////////////////////////////////////////////////////////
/// @brief Check the function name.
/**
		This function checks the custom function name. The function
	name can't be empty, can't start from digit and can't contain
	any white spaces.

@param[in] name The function name.
@return @b true if the function name is correct, otherwise @b false.
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
/// @brief The main constructor.
/**
@param[in] msg The error message.
*/
Failure::Failure(const std::string &msg)
	: inherited(msg)
{}



//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] msg The error message.
*/
SyntaxError::SyntaxError(const std::string &msg)
	: inherited(msg)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] func_name The function name.
*/
UnknownFunctionCall::UnknownFunctionCall(const std::wstring &func_name)
	: inherited("unknown function call"), m_name(func_name)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the function name.
/**
@return The function name.
*/
const std::wstring& UnknownFunctionCall::name() const
{
	return m_name;
}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] msg The error message.
*/
CalculationError::CalculationError(const std::string &msg)
	: inherited(msg)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
*/
DivisionByZero::DivisionByZero()
	: inherited("division by zero")
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param[in] func_name The function name.
*/
InvalidFunctionName::InvalidFunctionName(const std::wstring &func_name)
	: inherited("invalid function name"), m_name(func_name)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The extended constructor.
/**
@param[in] msg The error message.
@param[in] func_name The function name.
*/
InvalidFunctionName::InvalidFunctionName(const std::string &msg, const std::wstring &func_name)
	: inherited(msg), m_name(func_name)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the function name.
/**
@return The function name.
*/
const std::wstring& InvalidFunctionName::name() const
{
	return m_name;
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
		/// @cond details
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// explicit <wchar_t> constant instantiation
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::FLOAT_CHARS[] = L".0123456789";
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::INT_CHARS[]   = L"0123456789";
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OCT_INDICATOR  = L'0';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::HEX_INDICATOR1 = L'X';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::HEX_INDICATOR2 = L'x';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::LEFT_BRACKET  = L'(';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::RIGHT_BRACKET = L')';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_ADD = L'+';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_SUB = L'-';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_MUL = L'*';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_DIV = L'/';

//////////////////////////////////////////////////////////////////////////
// is floating-point digit?
template<>
bool CharConst<wchar_t>::is_float_digit(Char cx)
{
	return wcschr(FLOAT_CHARS, cx) != 0;
}

//////////////////////////////////////////////////////////////////////////
// is integer digit?
template<>
bool CharConst<wchar_t>::is_int_digit(Char cx)
{
	return wcschr(INT_CHARS, cx) != 0;
}

//////////////////////////////////////////////////////////////////////////
// widen char
template<>
wchar_t CharConst<wchar_t>::widen(Char cx, const std::ctype<Char>&)
{
	return cx;
}


//////////////////////////////////////////////////////////////////////////
// explicit <char> contants instantiation
template<> const CharConst<char>::Char CharConst<char>::FLOAT_CHARS[] = ".0123456789";
template<> const CharConst<char>::Char CharConst<char>::INT_CHARS[]   = "0123456789";
template<> const CharConst<char>::Char CharConst<char>::OCT_INDICATOR  = '0';
template<> const CharConst<char>::Char CharConst<char>::HEX_INDICATOR1 = 'X';
template<> const CharConst<char>::Char CharConst<char>::HEX_INDICATOR2 = 'x';
template<> const CharConst<char>::Char CharConst<char>::LEFT_BRACKET  = '(';
template<> const CharConst<char>::Char CharConst<char>::RIGHT_BRACKET = ')';
template<> const CharConst<char>::Char CharConst<char>::OP_ADD = '+';
template<> const CharConst<char>::Char CharConst<char>::OP_SUB = '-';
template<> const CharConst<char>::Char CharConst<char>::OP_MUL = '*';
template<> const CharConst<char>::Char CharConst<char>::OP_DIV = '/';


//////////////////////////////////////////////////////////////////////////
// is floating-point digit?
template<>
bool CharConst<char>::is_float_digit(Char cx)
{
	return strchr(FLOAT_CHARS, cx) != 0;
}


//////////////////////////////////////////////////////////////////////////
// is integer digit?
template<>
bool CharConst<char>::is_int_digit(Char cx)
{
	return strchr(INT_CHARS, cx) != 0;
}


//////////////////////////////////////////////////////////////////////////
// widen char
template<>
wchar_t CharConst<char>::widen(Char cx, const std::ctype<Char> &fac)
{
	return fac.widen(cx);
}

		} // details namespace
		/// @endcond
	} // calc namespace

} // omni namespace



//////////////////////////////////////////////////////////////////////////
/** @page omni_calc Evaluation tools

		The calculator is used for expression evaluation from strings.
	It is used, for example, during configuration parameters parsing.
	The calculator supports the main arithmetic operations and brackets
	and may be enhanced by custom user prefix and/or suffix functions.

		The calculator supports the following operations
	(from high priority to low priority):
			- numbers, unary plus and minus, brackets, prefix function call.
			- multiplication and division.
			- binary addition and subtraction.
			- suffix function call.

		First of all, the functions omni::calc::atof() and omni::calc::atoi()
	are enhanced equivalent of standard @a atof() and @a atoi() functions.


@section omni_calc_func The user functions

		The calculator supports the custom user prefix and suffix functions.
	The user function should have the following prototype:

@code
	T user_func(T x)
@endcode

		The suffix functions are used to setup the units of the parameter.
	For example, the following expression

@code
	"-3.0 dB"
@endcode

		is equivalent to the following code:

@code
	omni::util::dB2line(-3.0);
@endcode

		The prefix function argument should be in brackets!

@code
	"abs(-3.0 - 5.0)"
@endcode

		To manipulate of set of prefix and suffix functions use the following
	methods: omni::calc::FuncTable::insert() and omni::calc::FuncTable::remove().


@section omni_calc_using Using calculator.

	TODO: translate to English

		Как уже было сказано выше, калькулятор можно использовать для
	преобразования пользовательского ввода (строки из конфигурационного
	файла или полей ввода диалога) в числа.

		Например, нужно получить два параметра: время моделирования в секундах
	и частоту несущей в герцах. Для удобства пользователя мы можем ему
	позволить задавать эти значения разными вариантами:

@code
	simulation_time = "120 min"
	carrier_freq = "2.4 MHz"
@endcode

		or

@code
	simulation_time = "2*60 min"
	carrier_freq = "2400 kHz"
@endcode

		or

@code
	simulation_time = "2 hours"
	carrier_freq = "2400000 Hz"
@endcode

		Для этого создаем два калькулятора: один будет вычислять время,
	а другой частоту. Можно, конечно, использовать и один калькулятор,
	но тогда пользователь получает возможность в поле ввода для времени
	ввести частоту (MHz, KHz, Hz) и наоборот.

@code
	omni::calc::Calculator<double> calc_sec;
	omni::calc::Calculator<double> calc_Hz;
@endcode

		и добавляем соответствующие функции масштабирования

@code
	calc_sec.suffix().insert(L"hour", omni::calc::Multiplier<double, 60*60>());
	calc_sec.suffix().insert(L"min",  omni::calc::Multiplier<double, 60>());
	calc_sec.suffix().insert(L"sec",  omni::calc::Multiplier<double, 1>());

	calc_Hz.suffix().insert(L"MHz", omni::calc::Multiplier<double, 1000000>());
	calc_Hz.suffix().insert(L"KHz", omni::calc::Multiplier<double, 1000>());
	calc_Hz.suffix().insert(L"Hz",  omni::calc::Multiplier<double, 1>());
@endcode

	готово, можно использовать эти калькуляторы
для преобразования пользовательского ввода:

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

		Некоторые суффиксные функции не изменяют аргумента и служат
	исключительно для введения единицы измерения, например: @a Hz или @a sec.

		В примере полностью пропущена обработка ошибок. В реальных программах,
	при вычислении выражения могут генерироваться исключения, которые
	неплохо было бы перехватывать.

@section omni_calc_def Available calculators

	By default the following calculators are available:
		- sci() - the scientific calculator.
		- ratio() - the custom ratio (dB).
		- power() - the power in watts.
		- time() - the time in seconds.
		- freq() - the frequency in Hertz.
		- bits() - the size in bits or bytes.
		- dist() - the distance in meters.
*/
