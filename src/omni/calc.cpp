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
		Пространство имен содержит средства для вычисления выражений
	из потока ввода или строки.

@see @ref omni_calc
*/
	namespace calc
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Научный калькулятор
/**
		Функция возвращает калькулятор, который может вычислять
	безразмерные величины.

	Поддерживает следующие префиксные функции:
		- @b sin синус
		- @b cos косинус
		- @b tan тангенс
		- @b sinh гиперболический синус
		- @b cosh гиперболический косинус
		- @b tanh гиперболический тангенс
		- @b asin арксинус
		- @b acos арккосинус
		- @b atan арктангенс
		- @b sqrt квадратный корень
		- @b exp экспонента
		- @b ln натуральный логарифм
		- @b lg десятичный логарифм
		- @b floor округление вниз
		- @b ceil округление вверх
		- @b abs модуль числа

	и следующие суфиксные функции:
		- @b degr, @b deg конвертировать из градусов в радианы

@return Калькулятор
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
/// @brief Калькулятор безразмерной величины (отношение)
/**
		Функция возвращает калькулятор, который может вычислять
	безразмерные величины.

	Поддерживает следующие суфиксные функции:
		- @b dB перевод из децибелов по мощности в разы

@return Калькулятор
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
/// @brief Калькулятор мощности в ваттах
/**
		Функция возвращает калькулятор, который может вычислять
	мощность в ваттах.

	Поддерживает следующие суфиксные функции:
		- @b MW перевод из мегаватт (1e+6) в ватты
		- @b KW перевод из киловатт (1e+3) в ватты
		- @b W пустышка для ввода единицы измерения
		- @b mW перевод из миливатт (1e-3) в ватты
		- @b uW перевод из микроватт (1e-6) в ватты
		- @b dBm перевод из dBm в ватты
		- @b dBW перевод из dBW в ватты

@return Калькулятор
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
/// @brief Калькулятор времени в секундах
/**
		Функция возвращает калькулятор, который может вычислять
	время в секундах.

	Поддерживает следующие суфиксные функции:
		- @b day, @b days перевод из дней в секунды
		- @b hour, @b hours перевод из часов в секунды
		- @b min перевод из минут в секунды
		- @b sec, @b s пустышка для ввода единицы измерения
		- @b ms перевод милисекунд (1e-3) в секунды
		- @b us перевод микросекунд (1e-6) в секунды
		- @b ns перевод наносекунд (1e-9) в секунды

@return Калькулятор
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
/// @brief Калькулятор частоты в Герцах
/**
		Функция возвращает калькулятор, который может вычислять
	частоту в Герцах.

	Поддерживает следующие суфиксные функции:
		- @b GHz переводит гигагерцы (1e+9) в герцы
		- @b MHz переводит мегагерцы (1e+6) в герцы
		- @b KHz, @b kHz переводит килогерцы (1e+3) в герцы
		- @b Hz пустышка для ввода единицы измерения

@return Калькулятор
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
/// @brief Калькулятор размера в битах и байтах.
/**
		Функция возвращает калькулятор, который может вычислять
	размеры как в битах, так и в байтах.

	Поддерживает следующие суфиксные функции:
		- @b B, @b b пустышка для ввода единицы измерения
		- @b KB, @b Kb переводит килобиты (1024) в биты
		- @b MB, @b Mb переводит мегабиты (1024*1024) в биты
		- @b GB, @b Gb переводит гигабиты (1024*1024*1024) в биты


@return Калькулятор
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
/// @brief Калькулятор расстояния в метрах.
/**
		Функция возвращает калькулятор, который может вычислять
	расстояния в метрах.

	Поддерживает следующие суфиксные функции:
		- @b nm, переводит нанометры (0.000000001) в метры
		- @b um, переводит микрометры (0.000001) в метры
		- @b mm, переводит милиметры (0.001) в метры
		- @b cm, переводит сантиметры (0.01) в метры
		- @b m, пустышка для ввода единицы измерения
		- @b km, @b Km переводит километры (1000) в метры


@return Калькулятор
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
/// @brief Проверить корректность имени функции
/**
		Проверяет корректность имени пользовательской функции.
	Имя должно состоять из одного слова и начинаться с символа.

@param[in] name Имя пользовательской фукнции
@return @b true если имя корректное, иначе @b false
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
/// @brief Создать исключение
/**
@param[in] msg Сообщение об ошибке
*/
Failure::Failure(const std::string &msg)
	: inherited(msg)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение
/**
@param[in] msg Сообщение об ошибке
*/
Failure::Failure(const char *msg)
	: inherited(msg)
{}



//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
*/
SyntaxError::SyntaxError(const std::string &msg)
	: inherited(msg)
{}

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
*/
SyntaxError::SyntaxError(const char *msg)
	: inherited(msg)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] func_name Имя функции
*/
UnknownFunctionCall::UnknownFunctionCall(const std::wstring &func_name)
	: inherited("unknown function call"), m_func_name(func_name)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Имя несуществующей функции
/**
		Метод возвращает имя функции, встретившейся в выражении,
	но не существующей в таблице пользовательских функций калькулятора.

@return Имя несуществующей функции
*/
const std::wstring& UnknownFunctionCall::funcName() const
{
	return m_func_name;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
*/
CalculationError::CalculationError(const std::string &msg)
	: inherited(msg)
{}

//////////////////////////////////////////////////////////////////////////
/// @brief Создать исключение с параметрами
/**
@param[in] msg Сообщение об ошибке
*/
CalculationError::CalculationError(const char *msg)
	: inherited(msg)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Конструктор с заданием сообщения
/**
*/
DivisionByZero::DivisionByZero()
	: inherited("division by zero")
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Конструктор с заданием сообщения
/**
@param[in] func_name Имя функции
*/
InvalidFunctionName::InvalidFunctionName(const std::wstring &func_name)
	: inherited("invalid function name"), m_func_name(func_name)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Конструктор с заданием сообщения
/**
@param[in] msg The error message.
@param[in] func_name The function name.
*/
InvalidFunctionName::InvalidFunctionName(const std::string &msg, const std::wstring &func_name)
	: inherited(msg), m_func_name(func_name)
{}

//////////////////////////////////////////////////////////////////////////
/// @brief Конструктор с заданием сообщения
/**
@param[in] msg The error message.
@param[in] func_name The function name.
*/
InvalidFunctionName::InvalidFunctionName(const char *msg, const std::wstring &func_name)
	: inherited(msg), m_func_name(func_name)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Имя функции
/**
		Метод возвращает имя некорректной функции.

@return Имя некорректной функции
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

		Калькулятор используется для вычисления строковых выражений.
	Используется, например, при чтении параметров из файла конфигурации
	или полей ввода. Калькулятор поддерживает основные арифметические
	операции и может расширяться произвольными функциями.

		Калькулятор поддерживает следующие основные арифметические
	операции (в порядке убывания приоритета):
			- скобки, унарный плюс, унарный минус, вызов функции
			- умножение, деление
			- сложение, вычитание

			По умолчанию доступны два калькулятора: с вещественными и целыми
	числами, соответственно omni::calc::evalf() и omni::calc::evali().
	Эти функции эквивалентны функциям стандартной библиотеки
	@a atof и @a atoi соответственно, но умеют выполнять простые
	арифметические операции (не содержат функций расширения).


@section omni_calc_func Функции расширения
		Калькулятор поддерживает расширение произвольными функциями.
	Пользовательская функция должна иметь следующий прототип:

		@code T user_func(T x) @endcode

		В выражении могут быть использваны два типа функций: префиксные
	и суффиксные. Суффиксные функции в основном используются для
	задания единиц измерения и соответствующего масштабирования.
	Например, выражение

@code
"3.0 dB"
@endcode

		эквивалентно выражению

@code
"dB2line(3.0)"
@endcode

		Аргумент префиксной функции всегда должен располагаться в скобках!
	Например:

@code
"abs(-3.0)"
@endcode

		Для управления набором пользовательских функций используются
	следующие методы omni::calc::UserFuncTable::insert()
	и omni::calc::UserFuncTable::remove().


@section omni_calc_using Использование калькулятора

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

		или

@code
	simulation_time = "2*60 min"
	carrier_freq = "2400 KHz"
@endcode

		или

@code
	simulation_time = "2 hour"
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
	calc_sec.suffix().insert("hour", omni::calc::Multiplier<double, 60*60>());
	calc_sec.suffix().insert("min",  omni::calc::Multiplier<double, 60>());
	calc_sec.suffix().insert("sec",  omni::calc::Multiplier<double, 1>());

	calc_Hz.suffix().insert("MHz", omni::calc::Multiplier<double, 1000000>());
	calc_Hz.suffix().insert("KHz", omni::calc::Multiplier<double, 1000>());
	calc_Hz.suffix().insert("Hz",  omni::calc::Multiplier<double, 1>());
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

@section omni_calc_def Доступные калькуляторы

	By default the following calculators are available:
		- sci() - the scientific calculator.
		- ratio() - the custom ratio.
		- power() - the power in watts.
		- time() - the time in seconds.
		- freq() - the frequency in Hertz.
		- bits() - the size in bits or bytes.
		- dist() - the distance in meters.
*/
