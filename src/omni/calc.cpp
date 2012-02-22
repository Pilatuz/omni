///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
/** @file
@brief Implementation of the calculator tools.
@author Sergey Polichnoy <pilatuz@gmail.com>
@see @ref omni_calc
*/
#include <omni/calc.hpp>
#include <omni/util.hpp>

#include <string.h>
#include <math.h>

namespace omni
{
	namespace calc
	{

///////////////////////////////////////////////////////////////////////////////
Calculator<double> const& sci()
{
	// factory
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


///////////////////////////////////////////////////////////////////////////////
Calculator<double> const& ratio()
{
	// factory
	struct Init
	{
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


///////////////////////////////////////////////////////////////////////////////
Calculator<double> const& power()
{
	// factory
	struct Init
	{
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"MW", MulFunc<double, 1000000L>());
			calculator.suffix().insert(L"KW", MulFunc<double, 1000L>());
			calculator.suffix().insert(L"W",  MulFunc<double, 1L>());
			calculator.suffix().insert(L"mW", DivFunc<double, 1000L>());
			calculator.suffix().insert(L"uW", DivFunc<double, 1000000L>());
			calculator.suffix().insert(L"dBm", UserFunc<double>(omni::util::dBm2watt));
			calculator.suffix().insert(L"dBW", UserFunc<double>(omni::util::dB2line));
			calculator.suffix().insert(L"dB", UserFunc<double>(omni::util::dB2line));

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


///////////////////////////////////////////////////////////////////////////////
Calculator<double> const& time()
{
	// factory
	struct Init
	{
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"day", MulFunc<double, 24*60*60L>());
			calculator.suffix().insert(L"days", MulFunc<double, 24*60*60L>());
			calculator.suffix().insert(L"hour", MulFunc<double, 60*60L>());
			calculator.suffix().insert(L"hours", MulFunc<double, 60*60L>());
			calculator.suffix().insert(L"min", MulFunc<double, 60L>());
			calculator.suffix().insert(L"sec",  MulFunc<double, 1L>());
			calculator.suffix().insert(L"s",  MulFunc<double, 1L>());
			calculator.suffix().insert(L"ms", DivFunc<double, 1000L>());
			calculator.suffix().insert(L"us", DivFunc<double, 1000000L>());
			calculator.suffix().insert(L"ns", DivFunc<double, 1000000000L>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


///////////////////////////////////////////////////////////////////////////////
Calculator<double> const& freq()
{
	// factory
	struct Init
	{
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"GHz", MulFunc<double, 1000000000L>());
			calculator.suffix().insert(L"MHz", MulFunc<double, 1000000L>());
			calculator.suffix().insert(L"KHz", MulFunc<double, 1000L>());
			calculator.suffix().insert(L"kHz", MulFunc<double, 1000L>());
			calculator.suffix().insert(L"Hz",  MulFunc<double, 1L>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


///////////////////////////////////////////////////////////////////////////////
Calculator<double> const& volt()
{
	// factory
	struct Init
	{
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"GV", MulFunc<double, 1000000000L>());
			calculator.suffix().insert(L"MV", MulFunc<double, 1000000L>());
			calculator.suffix().insert(L"KV", MulFunc<double, 1000L>());
			calculator.suffix().insert(L"kV", MulFunc<double, 1000L>());
			calculator.suffix().insert(L"V",  MulFunc<double, 1L>());
			calculator.suffix().insert(L"mV", DivFunc<double, 1000L>());
			calculator.suffix().insert(L"uV", DivFunc<double, 1000000L>());
			calculator.suffix().insert(L"nV", DivFunc<double, 1000000000L>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


///////////////////////////////////////////////////////////////////////////////
Calculator<double> const& bits()
{
	// factory
	struct Init
	{
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"GB", MulFunc<double, 1024L*1024*1024>());
			calculator.suffix().insert(L"Gb", MulFunc<double, 1024L*1024*1024>());
			calculator.suffix().insert(L"MB", MulFunc<double, 1024L*1024>());
			calculator.suffix().insert(L"Mb", MulFunc<double, 1024L*1024>());
			calculator.suffix().insert(L"KB", MulFunc<double, 1024L>());
			calculator.suffix().insert(L"Kb", MulFunc<double, 1024L>());
			calculator.suffix().insert(L"B", MulFunc<double, 1L>());
			calculator.suffix().insert(L"b", MulFunc<double, 1L>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


///////////////////////////////////////////////////////////////////////////////
Calculator<double> const& dist()
{
	// factory
	struct Init
	{
		static Calculator<double> get()
		{
			Calculator<double> calculator;

			calculator.suffix().insert(L"Km", MulFunc<double, 1000L>());
			calculator.suffix().insert(L"km", MulFunc<double, 1000L>());
			calculator.suffix().insert(L"m",  MulFunc<double, 1L>());
			calculator.suffix().insert(L"cm", DivFunc<double, 100L>());
			calculator.suffix().insert(L"mm", DivFunc<double, 1000L>());
			calculator.suffix().insert(L"um", DivFunc<double, 1000L*1000>());
			calculator.suffix().insert(L"nm", DivFunc<double, 1000L*1000*1000>());

			return calculator;
		}
	};

	static Calculator<double> g_CALC = Init::get();
	return g_CALC;
}


namespace details
{

///////////////////////////////////////////////////////////////////////////////
/// @brief Check the function name.
/**
		This function checks the custom function name. The function
	name can't be empty, can't start from digit and can't contain
	any white spaces.

@param[in] name The function name.
@return @b true if the function name is correct, otherwise @b false.
*/
bool check_func_name(std::wstring const& name)
{
	if (name.empty())
		return false;

	std::wistringstream is(name);
	if (get_func_name(is) != name)
		return false;

	return true;
}

} // details namespace

	} // calc namespace


	// constants...
	namespace calc
	{
		namespace details
		{

///////////////////////////////////////////////////////////////////////////////
// explicit <wchar_t> constant instantiation
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::FLOAT_CHARS[] = L".0123456789";
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::INT_CHARS[]   = L"0123456789";
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OCT_INDICATOR  = L'0';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::HEX_INDICATOR_UP = L'X';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::HEX_INDICATOR_LO = L'x';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::LEFT_BRACKET  = L'(';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::RIGHT_BRACKET = L')';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_ADD = L'+';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_SUB = L'-';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_MUL = L'*';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::OP_DIV = L'/';

///////////////////////////////////////////////////////////////////////////////
// is floating-point digit?
template<>
bool CharConst<wchar_t>::is_float_digit(Char cx)
{
	return wcschr(FLOAT_CHARS, cx) != 0;
}

///////////////////////////////////////////////////////////////////////////////
// is integer digit?
template<>
bool CharConst<wchar_t>::is_int_digit(Char cx)
{
	return wcschr(INT_CHARS, cx) != 0;
}

///////////////////////////////////////////////////////////////////////////////
// widen char
template<>
wchar_t CharConst<wchar_t>::widen(Char cx, std::ctype<Char> const&)
{
	return cx;
}


///////////////////////////////////////////////////////////////////////////////
// explicit <char> contants instantiation
template<> const CharConst<char>::Char CharConst<char>::FLOAT_CHARS[] = ".0123456789";
template<> const CharConst<char>::Char CharConst<char>::INT_CHARS[]   = "0123456789";
template<> const CharConst<char>::Char CharConst<char>::OCT_INDICATOR  = '0';
template<> const CharConst<char>::Char CharConst<char>::HEX_INDICATOR_UP = 'X';
template<> const CharConst<char>::Char CharConst<char>::HEX_INDICATOR_LO = 'x';
template<> const CharConst<char>::Char CharConst<char>::LEFT_BRACKET  = '(';
template<> const CharConst<char>::Char CharConst<char>::RIGHT_BRACKET = ')';
template<> const CharConst<char>::Char CharConst<char>::OP_ADD = '+';
template<> const CharConst<char>::Char CharConst<char>::OP_SUB = '-';
template<> const CharConst<char>::Char CharConst<char>::OP_MUL = '*';
template<> const CharConst<char>::Char CharConst<char>::OP_DIV = '/';


///////////////////////////////////////////////////////////////////////////////
// is floating-point digit?
template<>
bool CharConst<char>::is_float_digit(Char cx)
{
	return strchr(FLOAT_CHARS, cx) != 0;
}


///////////////////////////////////////////////////////////////////////////////
// is integer digit?
template<>
bool CharConst<char>::is_int_digit(Char cx)
{
	return strchr(INT_CHARS, cx) != 0;
}


///////////////////////////////////////////////////////////////////////////////
// widen char
template<>
wchar_t CharConst<char>::widen(Char cx, std::ctype<Char> const& facet)
{
	return facet.widen(cx);
}

		} // details namespace
	} // calc namespace
} // omni namespace
