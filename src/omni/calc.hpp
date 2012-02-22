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
@brief Interface of the calculator tools.
@author Sergey Polichnoy <pilatuz@gmail.com>
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
	/** @brief Evaluation tools.

			The calc namespace contains tools for expression
		evaluation from the input stream or from the string.

	@see @ref omni_calc
	*/
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
			class MulFunc;
		template<typename T, long>
			class DivFunc;


		/// @brief Evaluation tools: exceptions.
		namespace err
		{
			class Failure;
				class SyntaxError;
					class UnknownFunctionCall;
				class CalculationError;
					class DivisionByZero;
				class InvalidFunctionName;
		}

		/// @brief Evaluation tools: implementation.
		/** @warning Do not use this namespace. */
		namespace details
		{
			template<typename T, typename Ch, typename Tr>
				T eval(std::basic_istream<Ch,Tr>&, Calculator<T> const&);
			bool check_func_name(std::wstring const&);
		}
	}


	// Calculator
	namespace calc
	{

///////////////////////////////////////////////////////////////////////////////
/// @brief The calculator tool.
/**
		This class performs evaluation of the string expressions.

		There are two function tables: prefix() and suffix().

@param T The type of the evaluation result.
@see @ref omni_calc
*/
template<typename T>
class Calculator
{
public:
	typedef FuncTable<T> Table;  ///< @brief The function table.
	typedef           T  Result; ///< @brief The result type.


///////////////////////////////////////////////////////////////////////////////
/// @name Evaluation
/// @{
public:

	/// @brief Evaluate first expression.
	/**
			This method evaluates the first expression from the input stream.

		If input stream has no valid expression the exception will be thrown.

	@param[in,out] expression The input stream.
	@return The evaluation result.
	@throw omni::calc::err::SyntaxError If expression is invalid.
	@throw omni::calc::err::CalculationError If expression has any errors.
	*/
	template<typename Ch, typename Tr>
	Result operator()(std::basic_istream<Ch,Tr> & expression) const
	{
		return details::eval(expression, *this);
	}


	/// @brief Evaluate expression from string.
	/**
			This method evaluates the whole expression from the string.

			If input string has no valid expression the exception will be thrown.

	@param[in] expression The input string.
	@return The evaluation result.
	@throw omni::calc::err::SyntaxError If expression is invalid.
	@throw omni::calc::err::CalculationError If expression has any errors.
	*/
	template<typename Ch, typename Tr, typename Ax>
	Result operator()(std::basic_string<Ch,Tr,Ax> const& expression) const
	{
		std::basic_istringstream<Ch,Tr,Ax> is(expression);
		const Result ret = details::eval(is, *this);

		if (!(is >> std::ws).eof())
			throw err::SyntaxError("expression not fully parsed");

		return ret;
	}


	/// @brief Evaluate expression from C-string.
	/**
			This method evaluates the whole expression from the C-string.

			If input C-string has no valid expression the exception will be thrown.

	@param[in] expression The input C-string.
	@return The evaluation result.
	@throw omni::calc::err::SyntaxError If expression is invalid.
	@throw omni::calc::err::CalculationError If expression has any errors.
	*/
	template<typename Ch>
	Result operator()(Ch const* expression) const
	{
		return (*this)(std::basic_string<Ch>(expression));
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Function tables
/// @{
public:

	/// @brief The prefix function table (read-only).
	Table const& prefix() const
	{
		return m_prefix;
	}


	/// @brief The prefix function table.
	Table& prefix()
	{
		return m_prefix;
	}


	/// @brief The suffix function table (read-only).
	Table const& suffix() const
	{
		return m_suffix;
	}


	/// @brief The suffix function table.
	Table& suffix()
	{
		return m_suffix;
	}
/// @}


///////////////////////////////////////////////////////////////////////////////
/// @name Properties
/// @{
public:

	/// @brief Is integer arithmetic?
	/**
	@return @b true if calculator is for integer numbers,
		or @b false if calculator is for float numbers.
	*/
	static bool is_integer()
	{
		assert(std::numeric_limits<Result>::is_specialized); // (?) static check
		return std::numeric_limits<Result>::is_integer;
	}
/// @}

private:
	Table m_prefix; ///< @brief The prefix function table.
	Table m_suffix; ///< @brief The suffix function table.
};

	} // Calculator


	// auxiliary functions
	namespace calc
	{

///////////////////////////////////////////////////////////////////////////////
/// @brief Evaluate the floating-point expression from the string.
/**
		This function evaluates the floating-point expression from the string.

		The following operators are supported: "+", "-", "*", "/" and brackets.
	There is no any additional prefix or suffix functions.

		If input string has no valid expression the exception will be thrown.

@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see @ref omni_calc
*/
template<typename Ch, typename Tr, typename Ax> inline
double atof(std::basic_string<Ch,Tr,Ax> const& expression)
{
	Calculator<double> x;
	return x(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Evaluate the floating-point expression from the C-string.
/**
		This function evaluates the floating-point expression from the C-string.

		The following operators are supported: "+", "-", "*", "/" and brackets.
	There is no any additional prefix or suffix functions.

		If input C-string has no valid expression the exception will be thrown.

@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see @ref omni_calc
*/
template<typename Ch> inline
double atof(Ch const* expression)
{
	Calculator<double> x;
	return x(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Evaluate the integer expression from the string.
/**
		This function evaluates the integer expression from the string.

		The following operators are supported: "+", "-", "*", "/" and brackets.
	There is no any additional prefix or suffix functions.

		If input string has no valid expression the exception will be thrown.

@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see @ref omni_calc
*/
template<typename Ch, typename Tr, typename Ax> inline
long atoi(std::basic_string<Ch,Tr,Ax> const& expression)
{
	Calculator<long> x;
	return x(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Evaluate the integer expression from the C-string.
/**
		This function evaluates the integer expression from the C-string.

		The following operators are supported: "+", "-", "*", "/" and brackets.
	There is no any additional prefix or suffix functions.

		If input C-string has no valid expression the exception will be thrown.

@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see @ref omni_calc
*/
template<typename Ch> inline
long atoi(Ch const* expression)
{
	Calculator<long> x;
	return x(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The scientific calculator.
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
Calculator<double> const& sci();


///////////////////////////////////////////////////////////////////////////////
/// @brief The scientific calculator: evaluate expression from the string.
/**
@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see sci()
*/
template<typename Ch, typename Tr, typename Ax> inline
double sci(std::basic_string<Ch,Tr,Ax> const& expression)
{
	return sci()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The scientific calculator: evaluate expression from the C-string.
/**
@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see sci()
*/
template<typename Ch> inline
double sci(Ch const* expression)
{
	return sci()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The ratio calculator.
/**
		This function returns ratio calculator reference.
	This calculator can evaluate any non-dimensional expressions.

		The following operators are supported: "+", "-", "*", "/" and brackets.

	The following suffix functions are supported:
		- @b dB - Conversion from dB to linear scale (by power).

@return The ratio calculator reference.
@see omni::util::dB2line()
*/
Calculator<double> const& ratio();


///////////////////////////////////////////////////////////////////////////////
/// @brief The ratio calculator: evaluate expression from the string.
/**
@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see ratio()
*/
template<typename Ch, typename Tr, typename Ax> inline
double ratio(std::basic_string<Ch,Tr,Ax> const& expression)
{
	return ratio()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The ratio calculator: evaluate expression from the C-string.
/**
@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see ratio()
*/
template<typename Ch> inline
double ratio(Ch const* expression)
{
	return ratio()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The power calculator.
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
Calculator<double> const& power();


///////////////////////////////////////////////////////////////////////////////
/// @brief The power calculator: evaluate expression from the string.
/**
@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see power()
*/
template<typename Ch, typename Tr, typename Ax> inline
double power(std::basic_string<Ch,Tr,Ax> const& expression)
{
	return power()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The power calculator: evaluate expression from the C-string.
/**
@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see power()
*/
template<typename Ch> inline
double power(Ch const* expression)
{
	return power()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The time calculator.
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
Calculator<double> const& time();


///////////////////////////////////////////////////////////////////////////////
/// @brief The time calculator: evaluate expression from the string.
/**
@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see time()
*/
template<typename Ch, typename Tr, typename Ax> inline
double time(std::basic_string<Ch,Tr,Ax> const& expression)
{
	return time()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The time calculator: evaluate expression from the C-string.
/**
@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see time()
*/
template<typename Ch> inline
double time(Ch const* expression)
{
	return time()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The frequency calculator.
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
Calculator<double> const& freq();


///////////////////////////////////////////////////////////////////////////////
/// @brief The frequency calculator: evaluate expression from the string.
/**
@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see freq()
*/
template<typename Ch, typename Tr, typename Ax> inline
double freq(std::basic_string<Ch,Tr,Ax> const& expression)
{
	return freq()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The frequency calculator: evaluate expression from the C-string.
/**
@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see freq()
*/
template<typename Ch> inline
double freq(Ch const* expression)
{
	return freq()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The voltage calculator.
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
Calculator<double> const& volt();


///////////////////////////////////////////////////////////////////////////////
/// @brief The voltage calculator: evaluate expression from the string.
/**
@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see volt()
*/
template<typename Ch, typename Tr, typename Ax> inline
double volt(std::basic_string<Ch,Tr,Ax> const& expression)
{
	return volt()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The voltage calculator: evaluate expression from the C-string.
/**
@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see volt()
*/
template<typename Ch> inline
double volt(Ch const* expression)
{
	return volt()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The bits calculator.
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
Calculator<double> const& bits();


///////////////////////////////////////////////////////////////////////////////
/// @brief The bits calculator: evaluate expression from the string.
/**
@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see bits()
*/
template<typename Ch, typename Tr, typename Ax> inline
double bits(std::basic_string<Ch,Tr,Ax> const& expression)
{
	return bits()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The bits calculator: evaluate expression from the C-string.
/**
@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see bits()
*/
template<typename Ch> inline
double bits(Ch const* expression)
{
	return bits()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The distance calculator.
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
Calculator<double> const& dist();


///////////////////////////////////////////////////////////////////////////////
/// @brief The distance calculator: evaluate expression from the string.
/**
@param[in] expression The input string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see dist()
*/
template<typename Ch, typename Tr, typename Ax> inline
double dist(std::basic_string<Ch,Tr,Ax> const& expression)
{
	return dist()(expression);
}


///////////////////////////////////////////////////////////////////////////////
/// @brief The distance calculator: evaluate expression from the C-string.
/**
@param[in] expression The input C-string.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
@see dist()
*/
template<typename Ch> inline
double dist(Ch const* expression)
{
	return dist()(expression);
}

	} // auxiliary functions



	// user functions...
	namespace calc
	{

///////////////////////////////////////////////////////////////////////////////
/// @brief The user function.
/**
		This class is used as wrapper class for user functions:
@code
	T (*f)(T)
@endcode

		The user function can be prefix or suffix.

@see Calculator<T>
*/
template<typename T>
class UserFunc
{
public:
	typedef T (*Function)(T);  ///< @brief The user function pointer.
	typedef T Argument;        ///< @brief The argument type.
	typedef T Result;          ///< @brief The result type.

	friend class FuncTable<T>;

private:

	/// @brief Default constructor.
	/**
		No function.
	*/
	UserFunc()
		: m_func(0)
	{}


public:

	/// @brief The main constructor.
	/**
			This constructor holds the user function @a f.

	@param[in] f The user function pointer. Should't be null.
	*/
	explicit UserFunc(Function f)
		: m_func(f)
	{}


public:

	/// @brief Execute function.
	/**
			This method executes user function.

	@param[in] x The function argument.
	@return The function result.
	*/
	Result operator()(Argument x) const
	{
		return m_func(x);
	}

private:
	Function m_func; ///< @brief The user function pointer.
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The user function table.
/**
		This class is a function table.
	It allows insert, remove, and execute custom function by name.
*/
template<typename T>
class FuncTable
{
public:
	typedef UserFunc<T> Function;   ///< @brief The user function.

public:

	/// @brief Insert function to the table.
	/**
			This method inserts the user function @a f to the table.

			If function name @a name is invalid or the same function name
		already exists the exception will be thrown.

	@param[in] name The function name.
	@param[in] f The user function.
	@throw omni::calc::err::InvalidFunctionName if function name is invalid
		or the same function name already exists.
	*/
	void insert(std::wstring const& name, Function const& f)
	{
		if (!details::check_func_name(name)) // invalid name
			throw err::InvalidFunctionName("invalid function name", name);

		if (m_table.find(name) != m_table.end()) // already exists
			throw err::InvalidFunctionName("function already exists", name);

		m_table.insert(std::make_pair(name, f));
	}


	/// @brief Remove function from the table.
	/**
			This method removes the user function from the table.

			If the function name is not exists the exception will be thrown.

	@param[in] name The function name.
	@throw omni::calc::err::InvalidFunctionName is function name is not exists.
	*/
	void remove(std::wstring const& name)
	{
		typename Table::iterator found = m_table.find(name);
		if (found == m_table.end()) // not found
			throw err::InvalidFunctionName("function doesn't exists", name);

		m_table.erase(found);
	}


	/// @brief Is function exists?
	/**
			This method checks the function with name @a name.

	@param[in] name The function name.
	@return @b true if function already exists, otherwise @b false.
	*/
	bool exists(std::wstring const& name) const
	{
		return (m_table.find(name) != m_table.end());
	}


	/// @brief Execute user function.
	/**
			This method executes the user function with name @a name.

			If the function name not exists the exception will be thrown.

	@param[in] name The function name.
	@param[in] arg The function argument.
	@return The function result.
	@throw omni::calc::err::UnknownFunctionCall is function name is not exists.
	*/
	T operator()(std::wstring const& name, T arg) const
	{
		typename Table::const_iterator found = m_table.find(name);
		if (found == m_table.end())
			throw err::UnknownFunctionCall(name);

		return (*found).second(arg);
	}

private:
	typedef std::map<std::wstring, Function> Table; ///< @brief Table type.
	Table m_table; ///< @brief The function table.
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The multiplier user function.
/**
		This class is used as user function.
	It multiplies argument by @a SCALE value.

		For example, the following code adds "Mb" and "Kb" user functions:

@code
template<typename T>
	void f(Calculator<T> &x)
	{
		x.suffix().insert(L"Mb", MulFunc<T, 1024*1024>());
		x.suffix().insert(L"Kb", MulFunc<T, 1024>());
	}
@endcode

@param T The function argument and result type.
@param SCALE The multiplier.
*/
template<typename T, long SCALE>
class MulFunc:
	public UserFunc<T>
{
	typedef UserFunc<T> base_type; ///< @brief The base type.

public:
	typedef typename base_type::Argument Argument; ///< @brief The argument type.
	typedef typename base_type::Result Result;   ///< @brief The result type.

	// enum { scale = SCALE };

public:

	/// @brief Default constructor.
	MulFunc()
		: base_type(func)
	{}

private:

	/// @brief The scale function.
	/**
			This static method multiplies argument @a x by @a SCALE.

	@param[in] x The function argument.
	@return The function result.
	*/
	static Result func(Argument x)
	{
		return x * SCALE;
	}
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The divider user function.
/**
		This class is used as user function.
	It divides argument by @a SCALE value.

		For example, the following code adds "ms" and "us" user functions:

@code
template<typename T>
	void f(Calculator<T> &x)
	{
		x.suffix().insert(L"ms", DivFunc<T, 1000>());
		x.suffix().insert(L"us", DivFunc<T, 1000*1000>());
	}
@endcode

@param T The function argument and result type.
@param SCALE The divider.
*/
template<typename T, long SCALE>
class DivFunc:
	public UserFunc<T>
{
	typedef UserFunc<T> base_type; ///< @brief The base type.

public:
	typedef typename base_type::Argument Argument; ///< @brief The argument type.
	typedef typename base_type::Result Result;   ///< @brief The result type.

	// enum { scale = SCALE };

public:

	/// @brief The default constructor.
	DivFunc()
		: base_type(func)
	{}

private:

	/// @brief The scale function.
	/**
			This static method divides argument @a x by @a SCALE.

	@param[in] x The function argument.
	@return The function result.
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

///////////////////////////////////////////////////////////////////////////////
/// @brief The basic exception class.
/**
		This class is a basic exception class.
*/
class Failure:
	public std::runtime_error
{
	typedef std::runtime_error base_type; ///< @brief The base type.
public:

	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	*/
	explicit Failure(std::string const& msg)
		: base_type(msg)
	{}


	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	*/
	explicit Failure(char const* msg)
		: base_type(msg)
	{}


	/// @brief The trivial destructor.
	virtual ~Failure() OMNI_THROW0()
	{}
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The syntax error.
/**
		This exception is thrown if expression has any syntax error:
			- expected expression is omitted.
			- invalid brackets.
*/
class SyntaxError:
	public Failure
{
	typedef Failure base_type; ///< @brief The base type.
public:

	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	*/
	explicit SyntaxError(std::string const& msg)
		: base_type(msg)
	{}


	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	*/
	explicit SyntaxError(char const* msg)
		: base_type(msg)
	{}


	/// @brief The trivial destructor.
	virtual ~SyntaxError() OMNI_THROW0()
	{}
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The unknown function call.
/**
		This exception is thrown if the unknown function call found.

	The function name returned by name() method.
*/
class UnknownFunctionCall:
	public SyntaxError
{
	typedef SyntaxError base_type; ///< @brief The base type.
public:

	/// @brief The main constructor.
	/**
	@param[in] func_name The function name.
	*/
	explicit UnknownFunctionCall(std::wstring const& func_name)
		: base_type("unknown function call"), m_name(func_name)
	{}


	/// @brief The trivial destructor.
	virtual ~UnknownFunctionCall() OMNI_THROW0()
	{}

public:

	/// @brief Get the function name.
	/**
	@return The function name.
	*/
	std::wstring const& name() const
	{
		return m_name;
	}

private:
	std::wstring m_name; ///< @brief The function name.
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The calculation error.
/**
		This is the basic exception for calculation errors.
	The calculation errors may be thrown during the expression evaluation.
*/
class CalculationError:
	public Failure
{
	typedef Failure base_type; ///< @brief The base type.
public:

	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	*/
	explicit CalculationError(std::string const& msg)
		: base_type(msg)
	{}


	/// @brief The main constructor.
	/**
	@param[in] msg The error message.
	*/
	explicit CalculationError(char const* msg)
		: base_type(msg)
	{}


	/// @brief The trivial destructor.
	virtual ~CalculationError() OMNI_THROW0()
	{}
};


///////////////////////////////////////////////////////////////////////////////
/// @brief Division by zero.
/**
		This exception is thrown if division by zero found during expression
	evaluation.

@note This exception is thrown only for integer expressions.
@see Calculator<T>::is_integer()
*/
class DivisionByZero:
	public CalculationError
{
	typedef CalculationError base_type; ///< @brief The base type.
public:

	/// @brief The default constructor.
	DivisionByZero()
		: base_type("division by zero")
	{}


	/// @brief The trivial destructor.
	virtual ~DivisionByZero() OMNI_THROW0()
	{}
};


///////////////////////////////////////////////////////////////////////////////
/// @brief The function name is invalid.
/**
		This is exception may be thrown during the function table manipulation.

@see FuncTable<T>::insert()
@see FuncTable<T>::remove()
*/
class InvalidFunctionName:
	public Failure
{
	typedef Failure base_type; ///< @brief The base type.
public:

	/// @brief The main constructor.
	/**
	@param[in] func_name The function name.
	*/
	explicit InvalidFunctionName(std::wstring const& func_name)
		: base_type("invalid function name"), m_name(func_name)
	{}


	/// @brief The extended constructor.
	/**
	@param[in] msg The error message.
	@param[in] func_name The function name.
	*/
	InvalidFunctionName(std::string const& msg, std::wstring const& func_name)
		: base_type(msg), m_name(func_name)
	{}


	/// @brief The extended constructor.
	/**
	@param[in] msg The error message.
	@param[in] func_name The function name.
	*/
	InvalidFunctionName(char const* msg, std::wstring const& func_name)
		: base_type(msg), m_name(func_name)
	{}


	/// @brief The trivial destructor.
	virtual ~InvalidFunctionName() OMNI_THROW0()
	{}

public:

	/// @brief Get the function name.
	/**
	@return The function name.
	*/
	std::wstring const& name() const
	{
		return m_name;
	}

private:
	std::wstring m_name; ///< @brief The function name.
};

		} // err namespace
	} // calc namespace


	// constants...
	namespace calc
	{
		namespace details
		{

///////////////////////////////////////////////////////////////////////////////
// @brief Char constants.
/*
		This class contains constants for single (@b char)
	and wide (@b wchar_t) characters.

		The constants FLOAT_CHARS and INT_CHARS defines symbols, from which
	the floating-point and integer numbers may be started corresponding.

		The integer numbers may be in octal, decimal or hexadecimal forms

		The LEFT_BRACKET and RIGHT_BRACKET defines left and right brackets.

		The following constants defines arithmetic operations:
			- OP_ADD - addition.
			- OP_SUB - subtraction.
			- OP_MUL - multiplication.
			- OP_DIV - division.

@param Ch The char type.
*/
template<typename Ch>
class CharConst {
public:
	typedef Ch Char; ///< @brief The char type.

public:
	static const Char FLOAT_CHARS[]; ///< @brief The floating-point digits.
	static const Char INT_CHARS[];   ///< @brief The integer digits.

public:
	static const Char OCT_INDICATOR;  ///< @brief The octal form indicator.
	static const Char HEX_INDICATOR_UP; ///< @brief The hexadecimal indicator (upper case).
	static const Char HEX_INDICATOR_LO; ///< @brief The hexadecimal indicator (lower case).

	static const Char LEFT_BRACKET;  ///< @brief The left bracket.
	static const Char RIGHT_BRACKET; ///< @brief The right bracket.

	static const Char OP_ADD;  ///< @brief The unary or binary plus.
	static const Char OP_SUB;  ///< @brief The unary or binary minus.
	static const Char OP_MUL;  ///< @brief The multiplication.
	static const Char OP_DIV;  ///< @brief The division.

public:

	/// @brief Is floating-point digit?
	static bool is_float_digit(Char cx);

	/// @brief Is integer digit?
	static bool is_int_digit(Char cx);

	/// @brief Widen the symbol.
	static wchar_t widen(Char cx,
		std::ctype<Char> const& facet);
};

		} // details namespace
	} // calc namespace


	// Evaluating implementation...
	namespace calc
	{
		namespace details
		{

///////////////////////////////////////////////////////////////////////////////
/// @brief Get function name.
/**
		This function parses the function name from the input stream.

		The function name should be started from alpha char
	and should't contains any white spaces.

@param[in,out] is The input stream.
@return The function name or empty string.
*/
template<typename Ch, typename Tr>
std::wstring get_func_name(std::basic_istream<Ch,Tr> &is)
{
	typedef std::basic_istream<Ch,Tr> istream_type;
	typedef std::basic_string<Ch,Tr> string_type;
	typedef typename istream_type::traits_type traits_type;
	typedef CharConst<Ch> calc_traits;
	typedef std::ctype<Ch> facet_type;

	std::ios::iostate state = std::ios::goodbit;
	const typename istream_type::sentry ok(is); // (!) skip white space

	std::wstring func_name;
	//func_name.reserve(32);

	if (ok)
	{
		facet_type const& fac = OMNI_USE_FACET(is.getloc(), facet_type);

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


///////////////////////////////////////////////////////////////////////////////
/// @brief Get number.
/**
		This function is trying to parse the number from the input stream.

	If the number has been parsed it is returned in @a x.

@param[in] is The input stream.
@param[out] x The parsed number.
@param[in] is_integer Is calculator integer?
@return @b true If the number is parsed, otherwise @b false.
*/
template<typename T, typename Ch, typename Tr>
bool get_num(std::basic_istream<Ch,Tr> &is, T &x, bool is_integer)
{
	typedef std::basic_istream<Ch, Tr> istream_type;
	typedef std::basic_string<Ch, Tr> string_type;
	typedef typename istream_type::traits_type traits_type;
	typedef CharConst<Ch> calc_traits;

	typename traits_type::int_type meta = is.peek();
	if (!traits_type::eq_int_type(meta, traits_type::eof()))
	{
		typename traits_type::char_type cx = traits_type::to_char_type(meta);

		if (is_integer) // integer value
		{
			if (calc_traits::is_int_digit(cx))
			{
				if (traits_type::eq(cx, calc_traits::OCT_INDICATOR)) // check for "oct" or "hex"
				{
					meta = is.ignore().peek();

					if (!traits_type::eq_int_type(meta, traits_type::eof()))
					{
						cx = traits_type::to_char_type(meta);

						if (traits_type::eq(cx, calc_traits::HEX_INDICATOR_UP)
							|| traits_type::eq(cx, calc_traits::HEX_INDICATOR_LO))
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
		else // float value
		{
			if (calc_traits::is_float_digit(cx))
			{
				if (is >> x)
					return true;
			}
		}
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Level 4 expression parsing.
/**
		This function trying to parse the number, unary plus or minus,
	brackets or prefix function call.

		If expression is invalid the exception will be thrown.

		The @a calculator reference is used to access the prefix function table.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If syntax error is found.
@throw omni::calc::err::UnknownFunctionCall If function name is not found.
*/
template<typename T, typename Ch, typename Tr>
T level_4(std::basic_istream<Ch,Tr> &is, Calculator<T> const& calculator)
{
	T x = T();

	if (!get_num(is >> std::ws, x, calculator.is_integer())) // try to parse number
	{
		typedef std::basic_istream<Ch, Tr> istream_type;
		typedef typename istream_type::traits_type traits_type;
		typedef CharConst<Ch> calc_traits;

		typename traits_type::int_type meta = is.peek();
		if (!traits_type::eq_int_type(meta, traits_type::eof()))
		{
			typename traits_type::char_type cx = traits_type::to_char_type(meta);

			if (traits_type::eq(cx, calc_traits::OP_ADD)) // unary "+"
				x = +level_4(is.ignore(), calculator);
			else if (traits_type::eq(cx, calc_traits::OP_SUB)) // unary "-"
				x = -level_4(is.ignore(), calculator);
			else if (traits_type::eq(cx, calc_traits::LEFT_BRACKET)) // brackets
			{
				x = level_1(is.ignore(), calculator);

				// check syntax
				meta = (is >> std::ws).peek();
				cx = traits_type::to_char_type(meta);
				if (traits_type::eq_int_type(meta, traits_type::eof())
					|| !traits_type::eq(cx, calc_traits::RIGHT_BRACKET))
						throw err::SyntaxError("expected \")\" char");
				else
					is.ignore();
			}
			else // prefix function call
			{
				std::wstring func_name = get_func_name(is);
				if (!calculator.prefix().exists(func_name))
					throw err::UnknownFunctionCall(func_name);

				if (!func_name.empty())
				{
					// find open bracket
					meta = (is >> std::ws).peek();
					cx = traits_type::to_char_type(meta);
					if (traits_type::eq_int_type(meta, traits_type::eof())
						|| !traits_type::eq(cx, calc_traits::LEFT_BRACKET))
							throw err::SyntaxError("expected \"(\" char");
					else
						is.ignore();

					const T z = level_1(is, calculator);

					// check close bracket
					meta = (is >> std::ws).peek();
					cx = traits_type::to_char_type(meta);
					if (traits_type::eq_int_type(meta, traits_type::eof())
						|| !traits_type::eq(cx, calc_traits::RIGHT_BRACKET))
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


///////////////////////////////////////////////////////////////////////////////
/// @brief Level 3 expression parsing.
/**
		This function trying to perform the multiplication and division.

	The exception will be thrown in case of integer division by zero.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return The evaluation result.
@throw omni::calc::err::DivisionByZero In case of integer division by zero.
*/
template<typename T, typename Ch, typename Tr>
T level_3(std::basic_istream<Ch,Tr> &is, Calculator<T> const& calculator)
{
	T x = level_4(is, calculator);

	while ((is >> std::ws) && !is.eof())
	{
		typedef std::basic_istream<Ch, Tr> istream_type;
		typedef typename istream_type::traits_type traits_type;
		typedef CharConst<Ch> calc_traits;

		typename traits_type::int_type meta = is.peek();
		typename traits_type::char_type cx = traits_type::to_char_type(meta);
		if (traits_type::eq(cx, calc_traits::OP_MUL))
		{
			x *= level_4(is.ignore(1), calculator);
		}
		else if (traits_type::eq(cx, calc_traits::OP_DIV))
		{
			const T z = level_4(is.ignore(1), calculator);
			if (calculator.is_integer() && (T() == z))
				throw err::DivisionByZero();
			x /= z;
		}
		else
			break;
	}

	return x;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Level 2 expression parsing.
/**
		This function is trying to perform the binary addition and subtraction.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return The evaluation result.
*/
template<typename T, typename Ch, typename Tr>
T level_2(std::basic_istream<Ch,Tr> &is, Calculator<T> const& calculator)
{
	T x = level_3(is, calculator);

	while ((is >> std::ws) && !is.eof())
	{
		typedef std::basic_istream<Ch, Tr> istream_type;
		typedef typename istream_type::traits_type traits_type;
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


///////////////////////////////////////////////////////////////////////////////
/// @brief Level 1 expression parsing.
/**
		This function is trying to perform suffix function calls.

	The @a calculator reference is used to access the suffix function table.

@param[in,out] is The input stream.
@param[in] calculator The calculator.
@return The evaluation result.
*/
template<typename T, typename Ch, typename Tr>
T level_1(std::basic_istream<Ch,Tr> &is, Calculator<T> const& calculator)
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


///////////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression from the input stream.
/**
		This function evaluates the custom expression from the input stream.

@param[in,out] expression The input stream.
@param[in] calculator The calculator.
@return The evaluation result.
@throw omni::calc::err::SyntaxError If expression is invalid.
@throw omni::calc::err::CalculationError If expression has any errors.
*/
template<typename T, typename Ch, typename Tr>
T eval(std::basic_istream<Ch,Tr> &expression, Calculator<T> const& calculator)
{
	return level_1(expression, calculator);
}

		} // details namespace
	} // calc namespace
} // omni namespace


///////////////////////////////////////////////////////////////////////////////
/** @page omni_calc Evaluation tools

		The calculator is used for expression evaluation from strings.
	It is used, for example, during configuration parameters parsing.
	The calculator supports the main arithmetic operations and round brackets
	and may be enhanced by custom user prefix and/or suffix functions.

		The calculator supports the following operations
	(from high priority to low priority):
			- numbers, unary plus and minus, round brackets, prefix function call.
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

		As noted above calculator may be used to convert user input to the values.
	For example, we get two parameters from configuration file: simulation time
	in seconds and the carrier frequency in Hertz. The following variants are
	possible:

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

	You can convert these parameters:

@code
	std::string s_secs;
	std::cout << "simulation time [s]: ";
	std::cin >> s_secs;
	double x_secs = omni::calc::time(s_secs);

	std::string s_freq;
	std::cout << "carrier frequency [Hz]: ";
	std::cin >> s_freq;
	double x_freq = omni::calc::freq(s_freq);
@endcode

		Some suffix functions doesn't change output and just indicate the units.
	For example @a Hz or @a sec.


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

#endif // __OMNI_CALC_HPP_
