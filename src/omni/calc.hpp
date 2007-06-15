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
	@brief Interface of the calculator tools.

@author Sergey Polichnoy
@see @ref omni_calc
*/
#ifndef __OMNI_CALC_HPP_
#define __OMNI_CALC_HPP_

#include <omni/defs.hpp>

#include <stdexcept>
#include <iomanip>
#include <istream>
#include <sstream>
#include <limits>
#include <string>
#include <map>

#include <assert.h>

namespace omni
{
	// forward declarations
	namespace calc
	{

		// main classes
		template<typename T>
			class Calculator;
		template<typename T>
			class FuncTable;

		template<typename T>
			class UserFunc;
		template<typename T, long>
			class Multiplier;
		template<typename T, long>
			class Divider;


		// exceptions
		namespace err
		{
			class Failure;
				class SyntaxError;
					class UnknownFunctionCall;
				class CalculationError;
					class DivisionByZero;
				class InvalidFunctionName;
		}

		// implementation
		/// @cond details
		namespace details
		{
			template<typename T, typename Ch, typename Tr>
				T eval(std::basic_istream<Ch, Tr>&, const Calculator<T>&);
			template<typename T, typename Ch, typename Tr, typename Ax>
				T eval(const std::basic_string<Ch, Tr, Ax>&, const Calculator<T>&);
			template<typename T, typename Ch>
				T eval(const Ch*, const Calculator<T>&);
			bool check_func_name(const std::wstring &name);
		}
		/// @endcond

	} // forward declarations


	// Calculator
	namespace calc
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The calculator tool.
/**
		This class performs evaluation of the string expressions.

		There are two function tables: prefix() and suffix().

@param T The type of the evaluation result.
@see @ref omni_calc
*/
template<typename T>
class Calculator {
public:
	typedef FuncTable<T> TableType;  ///< @brief The function table.
	typedef           T  ValueType;  ///< @brief The value type.


//////////////////////////////////////////////////////////////////////////
/// @name Evaluation
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate first expression.
/**
		This method evaluates the first expression from the input stream.

	If input stream has no valid expression the exception will be thrown.

@param[in,out] expression The input stream.
@return The evaluation result.
@throw omni::calc::ex::SyntaxError If expression is invalid.
@throw omni::calc::ex::CalculationError If expression has any errors.
*/
	template<typename Ch, typename Tr>
		ValueType operator()(std::basic_istream<Ch, Tr> &expression) const
	{
		return details::eval(expression, *this);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression from string.
/**
		Метод вычисляет выражение из строки @a expression.

		Если выражение некорректно или строка содержит более
	одного выражения будет сгенерировано исключение.

@param[in] expression Строковое выражение.
@return Значение выражения.
@throw omni::calc::ex::SyntaxError Если выражение некорректно.
@throw omni::calc::ex::CalculationError Если выражение содержит вычислительные ошибки.
*/
	template<typename Ch, typename Tr, typename Ax>
		ValueType operator()(const std::basic_string<Ch, Tr, Ax> &expression) const
	{
		return details::eval(expression, *this);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression from C-string.
/**
		Метод вычисляет выражение из C-строки @a expression.

		Если выражение некорректно или строка содержит более
	одного выражения будет сгенерировано исключение.

@param[in] expression Строковое выражение.
@return Значение выражения.
@throw omni::calc::ex::SyntaxError Если выражение некорректно.
@throw omni::calc::ex::CalculationError Если выражение содержит вычислительные ошибки.
*/
	template<typename Ch>
		ValueType operator()(const Ch *expression) const
	{
		return details::eval(expression, *this);
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Function tables.
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief The prefix function table.
	const TableType& prefix() const
	{
		return m_prefix;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief The prefix function table.
	TableType& prefix()
	{
		return m_prefix;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief The suffix function table.
	const TableType& suffix() const
	{
		return m_suffix;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief The suffix function table.
	TableType& suffix()
	{
		return m_suffix;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Properties
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Is integer arithmetic?
/**
@return @b true if calculator is for integer numbers,
	or @b false if calculator is for float numbers.
*/
	bool is_integer() const
	{
		assert(std::numeric_limits<ValueType>::is_specialized); // (?) static check
		return std::numeric_limits<ValueType>::is_integer;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

private:
	TableType m_prefix; ///< @brief The prefix function table.
	TableType m_suffix; ///< @brief The suffix function table.
};

	} // Calculator


	// auxiliary functions
	namespace calc
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Вычислить вещественное выражение из строки
/**
		Функция вычисляет вещественное выражение из строки @a expression.

		Если выражение некорректно или строка содержит более
	одного выражения будет сгенерировано исключение.

@param[in] expression Строковое выражение
@return Значение выражения
@throw omni::calc::SyntaxError Если выражение некорректно
@throw omni::calc::CalculationError Если выражение содержит вычислительные ошибки
@see @ref omni_calc
*/
template<typename Ch, typename Tr, typename Ax> inline
	double evalf(const std::basic_string<Ch, Tr, Ax> &expression)
{
	Calculator<double> x;
	return x(expression);
}

//////////////////////////////////////////////////////////////////////////
/// @brief Вычислить вещественное выражение из C-строки
/**
		Функция вычисляет вещественное выражение из C-строки @a expression.

		Если выражение некорректно или строка содержит более
	одного выражения будет сгенерировано исключение.

@param[in] expression Строковое выражение
@return Значение выражения
@throw omni::calc::SyntaxError Если выражение некорректно
@throw omni::calc::CalculationError Если выражение содержит вычислительные ошибки
@see @ref omni_calc
*/
template<typename Ch> inline
	double evalf(const Ch *expression)
{
	Calculator<double> x;
	return x(expression);
}

//////////////////////////////////////////////////////////////////////////
/// @brief Вычислить целое выражение из строки
/**
		Функция вычисляет целое выражение из строки @a expression.

		Если выражение некорректно или строка содержит более
	одного выражения будет сгенерировано исключение.

@param[in] expression Строковое выражение
@return Значение выражения
@throw omni::calc::SyntaxError Если выражение некорректно
@throw omni::calc::CalculationError Если выражение содержит вычислительные ошибки
@see @ref omni_calc
*/
template<typename Ch, typename Tr, typename Ax> inline
	long evali(const std::basic_string<Ch, Tr, Ax> &expression)
{
	Calculator<long> x;
	return x(expression);
}

//////////////////////////////////////////////////////////////////////////
/// @brief Вычислить целое выражение из C-строки
/**
		Функция вычисляет целое выражение из C-строки @a expression.

		Если выражение некорректно или строка содержит более
	одного выражения будет сгенерировано исключение.

@param[in] expression Строковое выражение
@return Значение выражения
@throw omni::calc::SyntaxError Если выражение некорректно
@throw omni::calc::CalculationError Если выражение содержит вычислительные ошибки
@see @ref omni_calc
*/
template<typename Ch> inline
	long evali(const Ch *expression)
{
	Calculator<long> x;
	return x(expression);
}

// common calculator: sci
const Calculator<double>& sci();

// common calculator: sci
template<typename Ch, typename Tr, typename Ax> inline
	double sci(const std::basic_string<Ch, Tr, Ax> &expression)
{
	return sci()(expression);
}

// common calculator: sci
template<typename Ch> inline
	double sci(const Ch *expression)
{
	return sci()(expression);
}


// common calculator: ratio
const Calculator<double>& ratio();
// common calculator: ratio
template<typename Ch, typename Tr, typename Ax> inline
	double ratio(const std::basic_string<Ch, Tr, Ax> &expression)
{
	return ratio()(expression);
}

// common calculator: ratio
template<typename Ch> inline
	double ratio(const Ch *expression)
{
	return ratio()(expression);
}


// common calculator: power
const Calculator<double>& power();
// common calculator: power
template<typename Ch, typename Tr, typename Ax> inline
	double power(const std::basic_string<Ch, Tr, Ax> &expression)
{
	return power()(expression);
}

// common calculator: power
template<typename Ch> inline
	double power(const Ch *expression)
{
	return power()(expression);
}


// common calculator: time
const Calculator<double>& time();
// common calculator: time
template<typename Ch, typename Tr, typename Ax> inline
	double time(const std::basic_string<Ch, Tr, Ax> &expression)
{
	return time()(expression);
}

// common calculator: time
template<typename Ch> inline
	double time(const Ch *expression)
{
	return time()(expression);
}

// common calculator: freq
const Calculator<double>& freq();
// common calculator: freq
template<typename Ch, typename Tr, typename Ax> inline
	double freq(const std::basic_string<Ch, Tr, Ax> &expression)
{
	return freq()(expression);
}

// common calculator: freq
template<typename Ch> inline
	double freq(const Ch *expression)
{
	return freq()(expression);
}


// common calculator: bits
const Calculator<double>& bits();
// common calculator: bits
template<typename Ch, typename Tr, typename Ax> inline
	double bits(const std::basic_string<Ch, Tr, Ax> &expression)
{
	return bits()(expression);
}

// common calculator: bits
template<typename Ch> inline
	double bits(const Ch *expression)
{
	return bits()(expression);
}


// common calculator: distance
const Calculator<double>& dist();
// common calculator: distance
template<typename Ch, typename Tr, typename Ax> inline
	double dist(const std::basic_string<Ch, Tr, Ax> &expression)
{
	return dist()(expression);
}

// common calculator: distance
template<typename Ch> inline
	double dist(const Ch *expression)
{
	return dist()(expression);
}

	} // auxiliary functions



	// user functions...
	namespace calc
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Обёртка для пользовательской функции
/**
		Класс служит обёрткой для функций вида
@code
	T (*f)(T)
@endcode

		Используется для расширения функциональности калькулятора
	префиксными или суфиксными функциями.
*/
template<typename T>
class UserFunc {
public:
	typedef T (*Function)(T);  ///< @brief Пользовательская функция
	typedef T Argument;        ///< @brief Тип аргумента пользовательской функции
	typedef T Result;          ///< @brief Тип результата пользовательской функции

	friend class FuncTable<T>;

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
	UserFunc()
		: m_func(0)
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Конструктор с заданием пользовательской функции
/**
		Конструктор сохраняет пользовательскую функцию @a f.

@param[in] f Пользовательская функция
*/
explicit UserFunc(Function f)
	: m_func(f)
{}


public:

//////////////////////////////////////////////////////////////////////////
/// @brief Выполнить функцию
/**
		Метод вызывает пользовательскую функцию с аргументом @a x.

@param[in] x Аргумент функции
@return Результат функции
*/
	Result operator()(Argument x) const
	{
		return m_func(x);
	}

private:
	Function m_func;
};


//////////////////////////////////////////////////////////////////////////
/// @brief Таблица пользовательских функций
/**
		Класс представляет собой таблицу пользовательских функций.
	Позволяет добавлять, удалять и выполнять функцию по строковому имени.
*/
template<typename T>
class FuncTable {
public:
	typedef UserFunc<T> Function;   ///< @brief Пользовательская функция

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Добавить пользовательскую функцию
/**
		Метод добавляет в таблицу новую пользовательскую функцию @a f
	с именем @a name. Если имя функции некорректно или функция с таким
	именем уже существует, то будет сгенерировано исключение.

@param[in] name Имя пользовательской функции
@param[in] f Пользовательская функция
@throw omni::calc::InvalidFunctionName
	Если имя функции не является корректным
	или функция с таким именем уже существует
*/
	void insert(const std::wstring &name, const Function &f)
	{
		if (!details::check_func_name(name))
			throw err::InvalidFunctionName("invalid function name", name); // invalid name

		if (m_table.find(name) != m_table.end())
			throw err::InvalidFunctionName("function already exists", name); // already exists

		m_table.insert(std::make_pair(name, f));
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Удалить пользовательскую функцию
/**
		Метод удаляет из таблицы пользовательскую функцию с именем @a name.

@param[in] name Имя пользовательской функции
@throw omni::calc::InvalidFunctionName
	Если функции с таким именем не существует
*/
	void remove(const std::wstring &name)
	{
		typename Table::iterator found = m_table.find(name);
		if (found == m_table.end())
			throw err::InvalidFunctionName("function not exists", name); // not exists

		m_table.erase(found);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить задана ли пользовательская функция
/**
		Метод проверяет наличие в таблице функции с именем @a name.

@param[in] name Имя пользовательской функции
@return @b true Если функция существует, иначе @b false
*/
	bool exists(const std::wstring &name) const
	{
		return (m_table.find(name) != m_table.end());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Выполнить пользовательскую функцию
/**
		Метод выполняет пользовательскую функцию с именем @a name. Если функции
	с заданным именем нет в таблице, будет сгенерировано исключение.

@param[in] name Имя пользовательской функции
@param[in] arg Аргумент пользовательской функции
@return Результат выполнения пользовательской функции
@throw omni::calc::UnknownFunctionCall
	Если функции с заданным именем нет в таблице
*/
	T operator()(const std::wstring &name, T arg) const
	{
		typename Table::const_iterator found = m_table.find(name);
		if (found == m_table.end())
			throw err::UnknownFunctionCall(name);

		return (*found).second(arg);
	}

private:
	typedef std::map<std::wstring, Function> Table;
	Table m_table;
};


//////////////////////////////////////////////////////////////////////////
/// @brief Функция масштабирования (умножение)
/**
	Используется в качестве пользовательской функции масштабирования.

		Например, следующий код добавляет в калькулятор функции "Mb" и "Kb":

@code
template<typename T>
	void f(Calculator<T> &x)
	{
		x.suffix().insert(L"Mb", Multiplier<T, 1024*1024>());
		x.suffix().insert(L"Kb", Multiplier<T, 1024>());
	}
@endcode
*/
template<typename T, long SCALE>
class Multiplier: public UserFunc<T> {
	typedef UserFunc<T> inherited;

public:
	typedef typename inherited::Argument Argument; ///< @brief The argument type.
	typedef typename inherited::Result Result;   ///< @brief The result type.

	enum { scale = SCALE };

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
	Multiplier()
		: inherited(func)
	{}

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Функция масштабирования
/**
		Метод выполняет увеличение аргумента @a x в @a SCALE раз.

@param[in] x Аргумент функции
@return Результат масштабирования
*/
	static Result func(Argument x)
	{
		return x * SCALE;
	}
};


//////////////////////////////////////////////////////////////////////////
/// @brief Функция масштабирования (деление)
/**
	Используется в качестве пользовательской функции масштабирования.

		Например, следующий код добавляет в калькулятор функции "ms" и "us":

@code
template<typename T>
	void f(Calculator<T> &x)
	{
		x.suffix().insert(L"ms", Divider<T, 1000>());
		x.suffix().insert(L"us", Divider<T, 1000*1000>());
	}
@endcode
*/
template<typename T, long SCALE>
class Divider: public UserFunc<T> {
	typedef UserFunc<T> inherited;

public:
	typedef typename inherited::Argument Argument; ///< @brief The argument type.
	typedef typename inherited::Result Result;   ///< @brief The result type.

	enum { scale = SCALE };

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
	Divider()
		: inherited(func)
	{}

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Функция масштабирования
/**
		Метод выполняет уменьшение аргумента @a x в @a SCALE раз.

@param[in] x Аргумент функции
@return Результат масштабирования
*/
	static Result func(Argument x)
	{
		return x / SCALE;
	}
};

	} // calc namespace



	// exceptions...
	namespace calc
	{
		namespace err
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Базовое исключение
/**
		Класс является базовым для всех исключений калькулятора.
*/
class Failure: public std::runtime_error {
	typedef std::runtime_error inherited;

protected:
	explicit Failure(const std::string &msg);
	explicit Failure(const char *msg);
	virtual ~Failure() OMNI_THROW0() {}
};


//////////////////////////////////////////////////////////////////////////
/// @brief Синтаксическая ошибка
/**
		Исключение генерируется, если в выражении встретилась синтаксическая
	ошибка. Это может быть:

		- отсутствие ожидаемого выражения
		- неверно расставленные скобки
*/
class SyntaxError: public Failure {
	typedef Failure inherited;

public:
	explicit SyntaxError(const std::string &msg);
	explicit SyntaxError(const char *msg);
	virtual ~SyntaxError() OMNI_THROW0() {}
};


//////////////////////////////////////////////////////////////////////////
/// @brief Вызов несуществующей функции
/**
		Исключение генерируется, если в выражении встретился вызов
	несуществующей функции. Имя функции возвращается методом funcName().
*/
class UnknownFunctionCall: public SyntaxError {
	typedef SyntaxError inherited;

public:
	explicit UnknownFunctionCall(const std::wstring &func_name);
	virtual ~UnknownFunctionCall() OMNI_THROW0() {}

public:
	const std::wstring& funcName() const;

private:
	std::wstring m_func_name;
};


//////////////////////////////////////////////////////////////////////////
/// @brief Ошибка вычислений
/**
		Исключения является базовым для ошибок вычисления.
*/
class CalculationError: public Failure {
	typedef Failure inherited;

protected:
	explicit CalculationError(const std::string &msg);
	explicit CalculationError(const char *msg);
	virtual ~CalculationError() OMNI_THROW0() {}
};


//////////////////////////////////////////////////////////////////////////
/// @brief Деление на ноль
/**
		Исключение генерируется если в выражении встречается
	целочисленное деление на ноль.
*/
class DivisionByZero: public CalculationError {
	typedef CalculationError inherited;
public:
	DivisionByZero();
	virtual ~DivisionByZero() OMNI_THROW0() {}
};


//////////////////////////////////////////////////////////////////////////
/// @brief Некорректное имя функций
/**
		Исключение используется при операциях с таблицами функций.
	При добавлении функции генерируется, если имя функции некорректно или
	функция с таким именем уже существует. При удалении функции генерируется,
	если функции с таким именем нет.
*/
class InvalidFunctionName: public Failure {
	typedef Failure inherited;

public:
	explicit InvalidFunctionName(const std::wstring &func_name);
	InvalidFunctionName(const std::string &msg, const std::wstring &func_name);
	InvalidFunctionName(const char *msg, const std::wstring &func_name);
	virtual ~InvalidFunctionName() OMNI_THROW0() {}

public:
	const std::wstring& funcName() const;

private:
	std::wstring m_func_name;
};

		} // err namespace
	} // calc namespace


	// constants...
	namespace calc
	{
		/// @cond details
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// @brief Константы
/*
		Класс содержит константы для обычного (@b char)
	и расширенного (@b wchar_t) символьных наборов.

		Константы FLOAT_CHARS и INT_CHARS определяют символы, с которых
	могут начинаться вещественные и целые числа соответственно.

		Если целое число начинается с OCT_INDICATOR, то число будет
	интерпретироваться в восьмеричной системе. Если сразу за OCT_INDICATOR
	расположен символ HEX_INDICATOR1 или HEX_INDICATOR2, то число будет
	интерпретироваться в шестнадцатеричной системе.

		Константы BRACE_OPEN и BRACE_CLOSE обозначают открывающуюся
	и закрывающуюся скобки соответственно.

		Следующие константы обозначают основные арифметические операции:
			- OP_ADD - сложение
			- OP_SUB - вычитание
			- OP_MUL - умножение
			- OP_DIV - деление

@param Ch Тип символа
*/
template<typename Ch>
class CharConst {
public:
	typedef Ch Char; ///< @brief The char type.

public:
	static const Char FLOAT_CHARS[]; ///< @brief Цифры вещественного числа
	static const Char INT_CHARS[];   ///< @brief Цифры целого числа

public:
	static const Char OCT_INDICATOR;  ///< @brief Индикатор восмеричной или шестнадцатеричной системы счисления
	static const Char HEX_INDICATOR1; ///< @brief Первый индикатор шестандцатеричной системы счисления
	static const Char HEX_INDICATOR2; ///< @brief Второй индикатор шестандцатеричной системы счисления

	static const Char BRACE_OPEN;  ///< @brief Открывающая скобка
	static const Char BRACE_CLOSE; ///< @brief Закрывающая скобка
	static const Char OP_ADD; ///< @brief Унарный или бинарный плюс
	static const Char OP_SUB; ///< @brief Унарный или бинарный минус
	static const Char OP_MUL; ///< @brief Бинарное умножение
	static const Char OP_DIV; ///< @brief Бинарное деление

public:
	static bool is_float_digit(Char cx);
	static bool is_int_digit(Char cx);
	static wchar_t widen(Char cx,
		const std::ctype<Char> &fac);
};

		} // details namespace
		/// @endcond
	} // calc namespace


	// Evaluating implementation...
	namespace calc
	{
		/// @cond details
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Прочитать имя функции из потока ввода
/**
		Возвращает имя функции из потока ввода @a is.
	Имя функции не должно начинаться с цифры, не должно содержать пробелов.
	Может вернуть пустую строку. Для расширенных символов выполняет
	преобразование @a char в @a wchar_t.

@param[in,out] is Поток ввода
@return Имя функции
*/
template<typename Ch, typename Tr>
	std::wstring get_func_name(std::basic_istream<Ch, Tr> &is)
{
	typedef std::basic_istream<Ch, Tr> istream_type;
	typedef std::basic_string<Ch, Tr> string_type;
	typedef istream_type::traits_type traits_type;
	typedef CharConst<Ch> calc_traits;
	typedef std::ctype<Ch> facet_type;

	std::ios::iostate state = std::ios::goodbit;
	const istream_type::sentry ok(is); // (!) skip white space

	std::wstring func_name;
	// func_name.reserve(32);

	if (ok)
	{
		const facet_type &fac = OMNI_USE_FACET(is.getloc(), facet_type);

		for (bool first_char = true;; first_char = false)
		{
			typename traits_type::int_type meta = is.peek();

			if (traits_type::eq_int_type(meta, traits_type::eof()))
			{
				state |= std::ios::eofbit;
				break;
			}

			typename traits_type::char_type cx = traits_type::to_char_type(meta);
			if (first_char) // first char must be "alpha"
			{
				if (!fac.is(facet_type::alpha, cx))
					break;

				if (calc_traits::is_float_digit(cx)
					|| calc_traits::is_int_digit(cx))
						break;
			}

			if (!fac.is(facet_type::alnum, cx))
				break;

			func_name += calc_traits::widen(cx, fac);
			is.ignore();
		}
	}

	is.setstate(state);
	return func_name;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Прочитать число из потока ввода
/**
		Функция пытается прочитать число из потока ввода @a is.
	Если число прочитано, то оно возвращается в переменной @a x.

@param[in] is Поток ввода
@param[out] x Возврашает число
@param[in] is_integer Is integer calculator?
@return @b true если число успешно прочитано, иначе @b false
*/
template<typename T, typename Ch, typename Tr>
	bool get_num(std::basic_istream<Ch, Tr> &is, T &x, bool is_integer)
{
	typedef std::basic_istream<Ch, Tr> istream_type;
	typedef std::basic_string<Ch, Tr> string_type;
	typedef istream_type::traits_type traits_type;
	typedef CharConst<Ch> calc_traits;

	typename traits_type::int_type meta = is.peek();
	if (!traits_type::eq_int_type(meta, traits_type::eof()))
	{
		typename traits_type::char_type cx = traits_type::to_char_type(meta);

		if (is_integer)
		{ // integer value
			if (calc_traits::is_int_digit(cx))
			{
				if (traits_type::eq(cx, calc_traits::OCT_INDICATOR)) // check for "oct" or "hex"
				{
					meta = is.ignore().peek();

					if (!traits_type::eq_int_type(meta, traits_type::eof()))
					{
						cx = traits_type::to_char_type(meta);

						if (traits_type::eq(cx, calc_traits::HEX_INDICATOR1)
							|| traits_type::eq(cx, calc_traits::HEX_INDICATOR2))
						{
							if (is.ignore() >> std::hex >> x)
								return true;
						}
						else if (calc_traits::is_int_digit(cx))
						{
							if (is >> std::oct >> x)
								return true;
						}
						else // (!) simple zero
						{
							x = T();
							return true;
						}
					}
					else
					{
						is.clear(); // clear "eof" bit
						is.putback(cx);
					}
				}

				// parse "dec" integer
				if (is >> std::dec >> x)
					return true;
			}
		}
		else
		{ // float value
			if (calc_traits::is_float_digit(cx))
			{
				if (is >> x)
					return true;
			}
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Level 4 expression parsing.
/**
		Функция пробует прочитать число, унарный плюс или минус, скобки или
	вызов префиксной функции.

		Генерирует исключение если нет закрывающей скобки, неправильный
	вызов префиксной функции или встретился неправильный символ.

		Параметр @a calculator используется для доступа к таблицам функций.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return Результат
@throw omni::calc::SyntaxError В случае синтаксической ошибки
*/
template<typename T, typename Ch, typename Tr>
	T level_4(std::basic_istream<Ch, Tr> &is, const Calculator<T> &calculator)
{
	T x = T();

	if (!get_num(is >> std::ws, x, calculator.is_integer())) // try to parse number
	{
		typedef std::basic_istream<Ch, Tr> istream_type;
		typedef istream_type::traits_type traits_type;
		typedef CharConst<Ch> calc_traits;

		typename traits_type::int_type meta = is.peek();
		if (!traits_type::eq_int_type(meta, traits_type::eof()))
		{
			typename traits_type::char_type cx = traits_type::to_char_type(meta);

			if (traits_type::eq(cx, calc_traits::OP_ADD)) // unary "+"
				x = +level_4(is.ignore(), calculator);
			else if (traits_type::eq(cx, calc_traits::OP_SUB)) // unary "-"
				x = -level_4(is.ignore(), calculator);
			else if (traits_type::eq(cx, calc_traits::BRACE_OPEN)) // braces
			{
				x = level_1(is.ignore(), calculator);

				// check syntax
				meta = (is >> std::ws).peek();
				cx = traits_type::to_char_type(meta);
				if (traits_type::eq_int_type(meta, traits_type::eof())
					|| !traits_type::eq(cx, calc_traits::BRACE_CLOSE))
						throw err::SyntaxError("expected \")\" char");
				else
					is.ignore();
			}
			else // prefix function call
			{
				std::wstring func_name = get_func_name(is);
				if (!func_name.empty())
				{
					// find open brace
					meta = (is >> std::ws).peek();
					cx = traits_type::to_char_type(meta);
					if (traits_type::eq_int_type(meta, traits_type::eof())
						|| !traits_type::eq(cx, calc_traits::BRACE_OPEN))
							throw err::SyntaxError("expected \"(\" char");
					else
						is.ignore();

					T z = level_1(is, calculator);

					// check close brace
					meta = (is >> std::ws).peek();
					cx = traits_type::to_char_type(meta);
					if (traits_type::eq_int_type(meta, traits_type::eof())
						|| !traits_type::eq(cx, calc_traits::BRACE_CLOSE))
							throw err::SyntaxError("expected \")\" char");
					else
						is.ignore();

					x = calculator.prefix()(func_name, z);
				}
				else
					throw err::SyntaxError("expected value or function call");
			}
		}
		else
			throw err::SyntaxError("unexpected end of input stream");
	}

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Level 3 expression parsing.
/**
		Функция выполняет бинарное умножение и деление.

		Генерирует исключение если встретилось целочисленное деление на ноль.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return The evaluation result.
@throw omni::calc::DivisionByZero В случае целочисленного деления на ноль
*/
template<typename T, typename Ch, typename Tr>
	T level_3(std::basic_istream<Ch, Tr> &is, const Calculator<T> &calculator)
{
	T x = level_4(is, calculator);

	while ((is >> std::ws) && !is.eof())
	{
		typedef std::basic_istream<Ch, Tr> istream_type;
		typedef istream_type::traits_type traits_type;
		typedef CharConst<Ch> calc_traits;

		typename traits_type::int_type meta = is.peek();
		typename traits_type::char_type cx = traits_type::to_char_type(meta);
		if (traits_type::eq(cx, calc_traits::OP_MUL))
		{
			x *= level_4(is.ignore(1), calculator);
		}
		else if (traits_type::eq(cx, calc_traits::OP_DIV))
		{
			T z = level_4(is.ignore(1), calculator);
			if (calculator.is_integer() && (T() == z))
				throw err::DivisionByZero();
			x /= z;
		}
		else
			break;
	}

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Level 2 expression parsing.
/**
		Функция выполняет бинарное сложение и вычитание.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return The evaluation result.
*/
template<typename T, typename Ch, typename Tr>
	T level_2(std::basic_istream<Ch, Tr> &is, const Calculator<T> &calculator)
{
	T x = level_3(is, calculator);

	while ((is >> std::ws) && !is.eof())
	{
		typedef std::basic_istream<Ch, Tr> istream_type;
		typedef istream_type::traits_type traits_type;
		typedef CharConst<Ch> calc_traits;

		typename traits_type::int_type meta = is.peek();
		typename traits_type::char_type cx = traits_type::to_char_type(meta);
		if (traits_type::eq(cx, calc_traits::OP_ADD))
			x += level_3(is.ignore(1), calculator);
		else if (traits_type::eq(cx, calc_traits::OP_SUB))
			x -= level_3(is.ignore(1), calculator);
		else
			break;
	}

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Level 1 expression parsing.
/**
		Performs suffix function calculation.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return The evaluation result.
*/
template<typename T, typename Ch, typename Tr>
	T level_1(std::basic_istream<Ch, Tr> &is, const Calculator<T> &calculator)
{
	T x = level_2(is, calculator);

	while ((is >> std::ws) && !is.eof())
	{
		std::wstring func_name = get_func_name(is);
		if (!func_name.empty())
			x = calculator.suffix()(func_name, x);
		else
			break;
	}

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression.
/**
		Функция вычисляет выражение из потока ввода @a is пока не будет достигнуто
	окончание выражения (которое может не совпадать с окончанием потока ввода).

		Если поток ввода не содержит выражения или выражение некорректно,
	будет сгенерировано исключение.

@param[in,out] expression Поток ввода
@param[in] calculator Калькулятор
@return Значение выражения
@throw omni::calc::SyntaxError Если выражение некорректно
@throw omni::calc::CalculationError Если выражение содержит вычислительные ошибки
*/
template<typename T, typename Ch, typename Tr>
	T eval(std::basic_istream<Ch, Tr> &expression, const Calculator<T> &calculator)
{
	return level_1(expression, calculator);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression.
/**
		Функция вычисляет выражение из строки @a expression.

		Если выражение некорректно или строка содержит более
	одного выражения будет сгенерировано исключение.

@param[in] expression Строковое выражение
@param[in] calculator Калькулятор
@return Значение выражения
@throw omni::calc::SyntaxError Если выражение некорректно
@throw omni::calc::CalculationError Если выражение содержит вычислительные ошибки
*/
template<typename T, typename Ch, typename Tr, typename Ax>
	T eval(const std::basic_string<Ch, Tr, Ax> &expression, const Calculator<T> &calculator)
{
	std::basic_istringstream<Ch, Tr> is(expression);
	T x = eval(is, calculator);

	if (!(is >> std::ws).eof())
		throw err::SyntaxError("expression not fully parsed");

	return x;
}

//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression.
/**
		Функция вычисляет выражение из C-строки @a expression.

		Если выражение некорректно или строка содержит более
	одного выражения будет сгенерировано исключение.

@param[in] expression Строковое выражение
@param[in] calculator Калькулятор
@return Значение выражения
@throw omni::calc::SyntaxError Если выражение некорректно
@throw omni::calc::CalculationError Если выражение содержит вычислительные ошибки
*/
template<typename T, typename Ch>
	T eval(const Ch *expression, const Calculator<T> &calculator)
{
	return eval(std::basic_string<Ch>(expression), calculator);
}

		} // details namespace
		/// @endcond
	} // calc namespace


} // omni namespace

#endif // __OMNI_CALC_HPP_
