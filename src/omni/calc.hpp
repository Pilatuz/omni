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
		����� ��������� ��������� �� ������ @a expression.

		���� ��������� ����������� ��� ������ �������� �����
	������ ��������� ����� ������������� ����������.

@param[in] expression ��������� ���������.
@return �������� ���������.
@throw omni::calc::ex::SyntaxError ���� ��������� �����������.
@throw omni::calc::ex::CalculationError ���� ��������� �������� �������������� ������.
*/
	template<typename Ch, typename Tr, typename Ax>
		ValueType operator()(const std::basic_string<Ch, Tr, Ax> &expression) const
	{
		return details::eval(expression, *this);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression from C-string.
/**
		����� ��������� ��������� �� C-������ @a expression.

		���� ��������� ����������� ��� ������ �������� �����
	������ ��������� ����� ������������� ����������.

@param[in] expression ��������� ���������.
@return �������� ���������.
@throw omni::calc::ex::SyntaxError ���� ��������� �����������.
@throw omni::calc::ex::CalculationError ���� ��������� �������� �������������� ������.
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
/// @brief ��������� ������������ ��������� �� ������
/**
		������� ��������� ������������ ��������� �� ������ @a expression.

		���� ��������� ����������� ��� ������ �������� �����
	������ ��������� ����� ������������� ����������.

@param[in] expression ��������� ���������
@return �������� ���������
@throw omni::calc::SyntaxError ���� ��������� �����������
@throw omni::calc::CalculationError ���� ��������� �������� �������������� ������
@see @ref omni_calc
*/
template<typename Ch, typename Tr, typename Ax> inline
	double evalf(const std::basic_string<Ch, Tr, Ax> &expression)
{
	Calculator<double> x;
	return x(expression);
}

//////////////////////////////////////////////////////////////////////////
/// @brief ��������� ������������ ��������� �� C-������
/**
		������� ��������� ������������ ��������� �� C-������ @a expression.

		���� ��������� ����������� ��� ������ �������� �����
	������ ��������� ����� ������������� ����������.

@param[in] expression ��������� ���������
@return �������� ���������
@throw omni::calc::SyntaxError ���� ��������� �����������
@throw omni::calc::CalculationError ���� ��������� �������� �������������� ������
@see @ref omni_calc
*/
template<typename Ch> inline
	double evalf(const Ch *expression)
{
	Calculator<double> x;
	return x(expression);
}

//////////////////////////////////////////////////////////////////////////
/// @brief ��������� ����� ��������� �� ������
/**
		������� ��������� ����� ��������� �� ������ @a expression.

		���� ��������� ����������� ��� ������ �������� �����
	������ ��������� ����� ������������� ����������.

@param[in] expression ��������� ���������
@return �������� ���������
@throw omni::calc::SyntaxError ���� ��������� �����������
@throw omni::calc::CalculationError ���� ��������� �������� �������������� ������
@see @ref omni_calc
*/
template<typename Ch, typename Tr, typename Ax> inline
	long evali(const std::basic_string<Ch, Tr, Ax> &expression)
{
	Calculator<long> x;
	return x(expression);
}

//////////////////////////////////////////////////////////////////////////
/// @brief ��������� ����� ��������� �� C-������
/**
		������� ��������� ����� ��������� �� C-������ @a expression.

		���� ��������� ����������� ��� ������ �������� �����
	������ ��������� ����� ������������� ����������.

@param[in] expression ��������� ���������
@return �������� ���������
@throw omni::calc::SyntaxError ���� ��������� �����������
@throw omni::calc::CalculationError ���� ��������� �������� �������������� ������
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
/// @brief ������ ��� ���������������� �������
/**
		����� ������ ������� ��� ������� ����
@code
	T (*f)(T)
@endcode

		������������ ��� ���������� ���������������� ������������
	����������� ��� ���������� ���������.
*/
template<typename T>
class UserFunc {
public:
	typedef T (*Function)(T);  ///< @brief ���������������� �������
	typedef T Argument;        ///< @brief ��� ��������� ���������������� �������
	typedef T Result;          ///< @brief ��� ���������� ���������������� �������

	friend class FuncTable<T>;

private:

//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
	UserFunc()
		: m_func(0)
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief ����������� � �������� ���������������� �������
/**
		����������� ��������� ���������������� ������� @a f.

@param[in] f ���������������� �������
*/
explicit UserFunc(Function f)
	: m_func(f)
{}


public:

//////////////////////////////////////////////////////////////////////////
/// @brief ��������� �������
/**
		����� �������� ���������������� ������� � ���������� @a x.

@param[in] x �������� �������
@return ��������� �������
*/
	Result operator()(Argument x) const
	{
		return m_func(x);
	}

private:
	Function m_func;
};


//////////////////////////////////////////////////////////////////////////
/// @brief ������� ���������������� �������
/**
		����� ������������ ����� ������� ���������������� �������.
	��������� ���������, ������� � ��������� ������� �� ���������� �����.
*/
template<typename T>
class FuncTable {
public:
	typedef UserFunc<T> Function;   ///< @brief ���������������� �������

public:

//////////////////////////////////////////////////////////////////////////
/// @brief �������� ���������������� �������
/**
		����� ��������� � ������� ����� ���������������� ������� @a f
	� ������ @a name. ���� ��� ������� ����������� ��� ������� � �����
	������ ��� ����������, �� ����� ������������� ����������.

@param[in] name ��� ���������������� �������
@param[in] f ���������������� �������
@throw omni::calc::InvalidFunctionName
	���� ��� ������� �� �������� ����������
	��� ������� � ����� ������ ��� ����������
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
/// @brief ������� ���������������� �������
/**
		����� ������� �� ������� ���������������� ������� � ������ @a name.

@param[in] name ��� ���������������� �������
@throw omni::calc::InvalidFunctionName
	���� ������� � ����� ������ �� ����������
*/
	void remove(const std::wstring &name)
	{
		typename Table::iterator found = m_table.find(name);
		if (found == m_table.end())
			throw err::InvalidFunctionName("function not exists", name); // not exists

		m_table.erase(found);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief ��������� ������ �� ���������������� �������
/**
		����� ��������� ������� � ������� ������� � ������ @a name.

@param[in] name ��� ���������������� �������
@return @b true ���� ������� ����������, ����� @b false
*/
	bool exists(const std::wstring &name) const
	{
		return (m_table.find(name) != m_table.end());
	}


//////////////////////////////////////////////////////////////////////////
/// @brief ��������� ���������������� �������
/**
		����� ��������� ���������������� ������� � ������ @a name. ���� �������
	� �������� ������ ��� � �������, ����� ������������� ����������.

@param[in] name ��� ���������������� �������
@param[in] arg �������� ���������������� �������
@return ��������� ���������� ���������������� �������
@throw omni::calc::UnknownFunctionCall
	���� ������� � �������� ������ ��� � �������
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
/// @brief ������� ��������������� (���������)
/**
	������������ � �������� ���������������� ������� ���������������.

		��������, ��������� ��� ��������� � ����������� ������� "Mb" � "Kb":

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
/// @brief ������� ���������������
/**
		����� ��������� ���������� ��������� @a x � @a SCALE ���.

@param[in] x �������� �������
@return ��������� ���������������
*/
	static Result func(Argument x)
	{
		return x * SCALE;
	}
};


//////////////////////////////////////////////////////////////////////////
/// @brief ������� ��������������� (�������)
/**
	������������ � �������� ���������������� ������� ���������������.

		��������, ��������� ��� ��������� � ����������� ������� "ms" � "us":

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
/// @brief ������� ���������������
/**
		����� ��������� ���������� ��������� @a x � @a SCALE ���.

@param[in] x �������� �������
@return ��������� ���������������
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
/// @brief ������� ����������
/**
		����� �������� ������� ��� ���� ���������� ������������.
*/
class Failure: public std::runtime_error {
	typedef std::runtime_error inherited;

protected:
	explicit Failure(const std::string &msg);
	explicit Failure(const char *msg);
	virtual ~Failure() OMNI_THROW0() {}
};


//////////////////////////////////////////////////////////////////////////
/// @brief �������������� ������
/**
		���������� ������������, ���� � ��������� ����������� ��������������
	������. ��� ����� ����:

		- ���������� ���������� ���������
		- ������� ������������� ������
*/
class SyntaxError: public Failure {
	typedef Failure inherited;

public:
	explicit SyntaxError(const std::string &msg);
	explicit SyntaxError(const char *msg);
	virtual ~SyntaxError() OMNI_THROW0() {}
};


//////////////////////////////////////////////////////////////////////////
/// @brief ����� �������������� �������
/**
		���������� ������������, ���� � ��������� ���������� �����
	�������������� �������. ��� ������� ������������ ������� funcName().
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
/// @brief ������ ����������
/**
		���������� �������� ������� ��� ������ ����������.
*/
class CalculationError: public Failure {
	typedef Failure inherited;

protected:
	explicit CalculationError(const std::string &msg);
	explicit CalculationError(const char *msg);
	virtual ~CalculationError() OMNI_THROW0() {}
};


//////////////////////////////////////////////////////////////////////////
/// @brief ������� �� ����
/**
		���������� ������������ ���� � ��������� �����������
	������������� ������� �� ����.
*/
class DivisionByZero: public CalculationError {
	typedef CalculationError inherited;
public:
	DivisionByZero();
	virtual ~DivisionByZero() OMNI_THROW0() {}
};


//////////////////////////////////////////////////////////////////////////
/// @brief ������������ ��� �������
/**
		���������� ������������ ��� ��������� � ��������� �������.
	��� ���������� ������� ������������, ���� ��� ������� ����������� ���
	������� � ����� ������ ��� ����������. ��� �������� ������� ������������,
	���� ������� � ����� ������ ���.
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
// @brief ���������
/*
		����� �������� ��������� ��� �������� (@b char)
	� ������������ (@b wchar_t) ���������� �������.

		��������� FLOAT_CHARS � INT_CHARS ���������� �������, � �������
	����� ���������� ������������ � ����� ����� ��������������.

		���� ����� ����� ���������� � OCT_INDICATOR, �� ����� �����
	������������������ � ������������ �������. ���� ����� �� OCT_INDICATOR
	���������� ������ HEX_INDICATOR1 ��� HEX_INDICATOR2, �� ����� �����
	������������������ � ����������������� �������.

		��������� BRACE_OPEN � BRACE_CLOSE ���������� �������������
	� ������������� ������ ��������������.

		��������� ��������� ���������� �������� �������������� ��������:
			- OP_ADD - ��������
			- OP_SUB - ���������
			- OP_MUL - ���������
			- OP_DIV - �������

@param Ch ��� �������
*/
template<typename Ch>
class CharConst {
public:
	typedef Ch Char; ///< @brief The char type.

public:
	static const Char FLOAT_CHARS[]; ///< @brief ����� ������������� �����
	static const Char INT_CHARS[];   ///< @brief ����� ������ �����

public:
	static const Char OCT_INDICATOR;  ///< @brief ��������� ����������� ��� ����������������� ������� ���������
	static const Char HEX_INDICATOR1; ///< @brief ������ ��������� ����������������� ������� ���������
	static const Char HEX_INDICATOR2; ///< @brief ������ ��������� ����������������� ������� ���������

	static const Char BRACE_OPEN;  ///< @brief ����������� ������
	static const Char BRACE_CLOSE; ///< @brief ����������� ������
	static const Char OP_ADD; ///< @brief ������� ��� �������� ����
	static const Char OP_SUB; ///< @brief ������� ��� �������� �����
	static const Char OP_MUL; ///< @brief �������� ���������
	static const Char OP_DIV; ///< @brief �������� �������

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
/// @brief ��������� ��� ������� �� ������ �����
/**
		���������� ��� ������� �� ������ ����� @a is.
	��� ������� �� ������ ���������� � �����, �� ������ ��������� ��������.
	����� ������� ������ ������. ��� ����������� �������� ���������
	�������������� @a char � @a wchar_t.

@param[in,out] is ����� �����
@return ��� �������
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
/// @brief ��������� ����� �� ������ �����
/**
		������� �������� ��������� ����� �� ������ ����� @a is.
	���� ����� ���������, �� ��� ������������ � ���������� @a x.

@param[in] is ����� �����
@param[out] x ���������� �����
@param[in] is_integer Is integer calculator?
@return @b true ���� ����� ������� ���������, ����� @b false
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
		������� ������� ��������� �����, ������� ���� ��� �����, ������ ���
	����� ���������� �������.

		���������� ���������� ���� ��� ����������� ������, ������������
	����� ���������� ������� ��� ���������� ������������ ������.

		�������� @a calculator ������������ ��� ������� � �������� �������.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return ���������
@throw omni::calc::SyntaxError � ������ �������������� ������
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
		������� ��������� �������� ��������� � �������.

		���������� ���������� ���� ����������� ������������� ������� �� ����.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return The evaluation result.
@throw omni::calc::DivisionByZero � ������ �������������� ������� �� ����
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
		������� ��������� �������� �������� � ���������.

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
		������� ��������� ��������� �� ������ ����� @a is ���� �� ����� ����������
	��������� ��������� (������� ����� �� ��������� � ���������� ������ �����).

		���� ����� ����� �� �������� ��������� ��� ��������� �����������,
	����� ������������� ����������.

@param[in,out] expression ����� �����
@param[in] calculator �����������
@return �������� ���������
@throw omni::calc::SyntaxError ���� ��������� �����������
@throw omni::calc::CalculationError ���� ��������� �������� �������������� ������
*/
template<typename T, typename Ch, typename Tr>
	T eval(std::basic_istream<Ch, Tr> &expression, const Calculator<T> &calculator)
{
	return level_1(expression, calculator);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression.
/**
		������� ��������� ��������� �� ������ @a expression.

		���� ��������� ����������� ��� ������ �������� �����
	������ ��������� ����� ������������� ����������.

@param[in] expression ��������� ���������
@param[in] calculator �����������
@return �������� ���������
@throw omni::calc::SyntaxError ���� ��������� �����������
@throw omni::calc::CalculationError ���� ��������� �������� �������������� ������
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
		������� ��������� ��������� �� C-������ @a expression.

		���� ��������� ����������� ��� ������ �������� �����
	������ ��������� ����� ������������� ����������.

@param[in] expression ��������� ���������
@param[in] calculator �����������
@return �������� ���������
@throw omni::calc::SyntaxError ���� ��������� �����������
@throw omni::calc::CalculationError ���� ��������� �������� �������������� ������
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
