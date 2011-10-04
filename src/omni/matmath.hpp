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
	@brief The matrix arithmetic.

@author Sergey Polichnoy
*/
#ifndef __OMNI_MATMATH_HPP_
#define __OMNI_MATMATH_HPP_

#include <omni/matrix.hpp>

#include <stdexcept>
#include <complex>
#include <string>

#include <assert.h>
#include <float.h>
#include <math.h>

namespace omni
{
	namespace mx
	{
		// exceptions
		namespace err
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Matrix is singular.
class MatrixIsSingular: public std::runtime_error {
	typedef std::runtime_error inherited;
public:

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
	MatrixIsSingular()
		: inherited("matrix is singular")
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param msg The exception message.
*/
	explicit MatrixIsSingular(const std::string &msg)
		: inherited(msg)
	{}
};


//////////////////////////////////////////////////////////////////////////
/// @brief Matrix is not positive definite.
class MatrixNotPositiveDefinite: public std::runtime_error {
	typedef std::runtime_error inherited;
public:

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
	MatrixNotPositiveDefinite()
		: inherited("matrix is not positive definite")
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param msg The exception message.
*/
	explicit MatrixNotPositiveDefinite(const std::string &msg)
		: inherited(msg)
	{}
};

		} // exceptions
	} // mx namespace


	// ValueTraits
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// ValueTraits class
class ValueTraits {
public: // equal

	template<typename T>
	static bool equal(const std::complex<T> &x, const std::complex<T> &y)
	{
		return equal(x.real(), y.real())
			&& equal(x.imag(), y.imag());
	}

	template<typename T>
	static bool equal(const Scalar<T> &x, const Scalar<T> &y)
	{
		return equal(x.val(), y.val());
	}

	static bool equal(double x, double y)
	{
		return ::fabs(x-y) < DBL_EPSILON;
	}

	static bool equal(float x, float y)
	{
		return ::fabsf(x-y) < FLT_EPSILON;
	}

	static bool equal(long x, long y)
	{
		return x == y;
	}

	static bool equal(int x, int y)
	{
		return x == y;
	}

public: // norm

	template<typename T>
	static T norm(const std::complex<T> &x)
	{
		return std::norm(x);
	}

	template<typename T>
	static Scalar<T> norm(const Scalar<T> &x)
	{
		return Scalar<T>(norm(x.val()));
	}

	static double norm(double x)
	{
		return x*x;
	}

	static float norm(float x)
	{
		return x*x;
	}

	static long norm(long x)
	{
		return x*x;
	}

	static int norm(int x)
	{
		return x*x;
	}

public: // abs

	template<typename T>
	static T abs(const std::complex<T> &x)
	{
		return std::abs(x);
	}

	template<typename T>
	static Scalar<T> abs(const Scalar<T> &x)
	{
		return Scalar<T>(abs(x.val()));
	}

	static double abs(double x)
	{
		return ::fabs(x);
	}

	static float abs(float x)
	{
		return ::fabsf(x);
	}

	static long abs(long x)
	{
		return ::labs(x);
	}

	static int abs(int x)
	{
		return ::abs(x);
	}


public: // sqrt

	template<typename T>
	static std::complex<T> sqrt(const std::complex<T> &x)
	{
		return std::sqrt(x);
	}

	template<typename T>
	static Scalar<T> sqrt(const Scalar<T> &x)
	{
		return Scalar<T>(sqrt(x.val()));
	}

	static double sqrt(double x)
	{
		return ::sqrt(x);
	}

	static float sqrt(float x)
	{
		return ::sqrtf(x);
	}

public: // conj value

	template<typename T>
	static std::complex<T> conj(const std::complex<T> &x)
	{
		return std::conj(x);
	}

	template<typename T>
	static Scalar<T> conj(const Scalar<T> &x)
	{
		return Scalar<T>(conj(x.val()));
	}

	static double conj(double x)
	{
		return x;
	}

	static float conj(float x)
	{
		return x;
	}

	static long conj(long x)
	{
		return x;
	}

	static int conj(int x)
	{
		return x;
	}

public: // real part

	template<typename T>
	static T real(const std::complex<T> &x)
	{
		return std::real(x);
	}

	template<typename T>
	static Scalar<T> real(const Scalar<T> &x)
	{
		return Scalar<T>(real(x.val()));
	}

	static double real(double x)
	{
		return x;
	}

	static float real(float x)
	{
		return x;
	}

	static long real(long x)
	{
		return x;
	}

	static int real(int x)
	{
		return x;
	}

public: // imaginary part

	template<typename T>
	static T imag(const std::complex<T> &x)
	{
		return std::imag(x);
	}

	template<typename T>
	static Scalar<T> imag(const Scalar<T> &x)
	{
		return Scalar<T>(imag(x.val()));
	}

	static double imag(double)
	{
		return 0.0;
	}

	static float imag(float)
	{
		return 0.0f;
	}

	static long imag(long)
	{
		return 0L;
	}

	static int imag(int)
	{
		return 0;
	}

public: // one constant

	template<typename T>
	static std::complex<T> one(const std::complex<T>&)
	{
		return std::complex<T>(one(T()));
	}

	template<typename T>
	static Scalar<T> one(const Scalar<T>&)
	{
		return Scalar<T>(one(T()));
	}

	static double one(double)
	{
		return 1.0;
	}

	static float one(float)
	{
		return 1.0f;
	}

	static long one(long)
	{
		return 1L;
	}

	static int one(int)
	{
		return 1;
	}
};

		} // details
	} // ValueTraits


	// Scalar operators
	namespace mx
	{

// prefix increment
template<typename T> inline
Scalar<T>& operator++(Scalar<T> &x)
{
	++x.val();
	return x;
}

// prefix decrement
template<typename T> inline
Scalar<T>& operator--(Scalar<T> &x)
{
	--x.val();
	return x;
}

// postfix increment
template<typename T> inline
Scalar<T> operator++(Scalar<T> &x, int)
{
	Scalar<T> t(x);
	++x;
	return t;
}

// postfix decrement
template<typename T> inline
Scalar<T> operator--(Scalar<T> &x, int)
{
	Scalar<T> t(x);
	--x;
	return t;
}

// 
template<typename T> inline
Scalar<T> operator~(const Scalar<T> &x)
{
	return Scalar<T>(~x.val());
}

// negate
template<typename T> inline
Scalar<T> operator!(const Scalar<T> &x)
{
	return Scalar<T>(!x.val());
}

// 
template<typename T> inline
Scalar<T> operator-(const Scalar<T> &x)
{
	return Scalar<T>(-x.val());
}

// 
template<typename T> inline
Scalar<T> operator+(const Scalar<T> &x)
{
	return Scalar<T>(+x.val());
}


// multiplication
template<typename T> inline
Scalar<T> operator*(const Scalar<T> &x, const Scalar<T> &y)
{
	return Scalar<T>(x.val() * y.val());
}

// division
template<typename T> inline
Scalar<T> operator/(const Scalar<T> &x, const Scalar<T> &y)
{
	return Scalar<T>(x.val() / y.val());
}

// remainder
template<typename T> inline
Scalar<T> operator%(const Scalar<T> &x, const Scalar<T> &y)
{
	return Scalar<T>(x.val() % y.val());
}

// addition
template<typename T> inline
Scalar<T> operator+(const Scalar<T> &x, const Scalar<T> &y)
{
	return Scalar<T>(x.val() + y.val());
}

// subtraction
template<typename T> inline
Scalar<T> operator-(const Scalar<T> &x, const Scalar<T> &y)
{
	return Scalar<T>(x.val() - y.val());
}


// equals
template<typename T> inline
bool operator==(const Scalar<T> &x, const Scalar<T> &y)
{
	return x.val() == y.val();
}

// not equals
template<typename T> inline
bool operator!=(const Scalar<T> &x, const Scalar<T> &y)
{
	return x.val() != y.val();
}

// less
template<typename T> inline
bool operator<(const Scalar<T> &x, const Scalar<T> &y)
{
	return x.val() < y.val();
}

// less or equal
template<typename T> inline
bool operator<=(const Scalar<T> &x, const Scalar<T> &y)
{
	return x.val() <= y.val();
}

// great
template<typename T> inline
bool operator>(const Scalar<T> &x, const Scalar<T> &y)
{
	return x.val() > y.val();
}

// great or equal
template<typename T> inline
bool operator>=(const Scalar<T> &x, const Scalar<T> &y)
{
	return x.val() >= y.val();
}


// 
template<typename T> inline
Scalar<T>& operator*=(Scalar<T> &x, const Scalar<T> &y)
{
	x.val() *= y.val();
	return x;
}

// 
template<typename T> inline
Scalar<T>& operator/=(Scalar<T> &x, const Scalar<T> &y)
{
	x.val() /= y.val();
	return x;
}

// 
template<typename T> inline
Scalar<T>& operator%=(Scalar<T> &x, const Scalar<T> &y)
{
	x.val() %= y.val();
	return x;
}

// 
template<typename T> inline
Scalar<T>& operator+=(Scalar<T> &x, const Scalar<T> &y)
{
	x.val() += y.val();
	return x;
}

// 
template<typename T> inline
Scalar<T>& operator-=(Scalar<T> &x, const Scalar<T> &y)
{
	x.val() -= y.val();
	return x;
}


// write to output stream
template<typename T, typename Ch, typename ChTr> inline
std::basic_ostream<Ch,ChTr>& operator<<(std::basic_ostream<Ch,ChTr> &os, const Scalar<T> &x)
{
	return os << x.val();
}

// read from input stream
template<typename T, typename Ch, typename ChTr> inline
std::basic_istream<Ch,ChTr>& operator>>(std::basic_istream<Ch,ChTr> &is, Scalar<T> &x)
{
	return is >> x.val();
}

	} // Scalar operators


	// UnVExpr, BiVExpr, UnMExpr, BiMExpr
	namespace mx
	{
		// vector
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Unary vector expression.
template<typename OP, typename T1>
class UnVExpr: private omni::NonCopyable {
	typedef UnVExpr<OP,T1> this_type;

public:
	typedef OP Operator;  ///< @brief The operator type.
	typedef T1 Argument1; ///< @brief The first argument type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param a1 The first argument.
*/
	explicit UnVExpr(const Argument1 &a1)
		: m_a1(a1)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param other The other expression.
*/
	UnVExpr(const this_type &other)
		: m_a1(other.m_a1)
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get first argument.
	const Argument1& arg1() const
	{
		return m_a1;
	}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression on vector.
	template<typename TZ, typename AZ> inline
	Vector<TZ,AZ>& operator()(Vector<TZ,AZ> &z) const
	{
		return Operator::eval(z, m_a1);
	}

private:
	const Argument1 &m_a1;
};


//////////////////////////////////////////////////////////////////////////
/// @brief Binary vector expression.
template<typename OP, typename T1, typename T2>
class BiVExpr: private omni::NonCopyable {
	typedef BiVExpr<OP,T1,T2> this_type;

public:
	typedef OP Operator;  ///< @brief The operator type.
	typedef T1 Argument1; ///< @brief The first argument type.
	typedef T2 Argument2; ///< @brief The second argument type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param a1 The first argument.
@param a2 The second argument.
*/
	BiVExpr(const Argument1 &a1, const Argument2 &a2)
		: m_a1(a1),
		  m_a2(a2)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param other The other expression.
*/
	BiVExpr(const this_type &other)
		: m_a1(other.m_a1),
		  m_a2(other.m_a2)
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get first argument.
	const Argument1& arg1() const
	{
		return m_a1;
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Get second argument.
	const Argument2& arg2() const
	{
		return m_a2;
	}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression on vector.
	template<typename TZ, typename AZ> inline
	Vector<TZ,AZ>& operator()(Vector<TZ,AZ> &z) const
	{
		return Operator::eval(z, m_a1, m_a2);
	}

private:
	const Argument1 &m_a1;
	const Argument2 &m_a2;
};



//////////////////////////////////////////////////////////////////////////
// create UnVExpr
template<typename OP, typename T1> inline
	UnVExpr<OP,T1> make_Vexpr(const T1 &a1)
{
	return UnVExpr<OP,T1>(a1);
}

//////////////////////////////////////////////////////////////////////////
// create BiVExpr
template<typename OP, typename T1, typename T2> inline
	BiVExpr<OP,T1,T2> make_Vexpr(const T1 &a1, const T2 &a2)
{
	return BiVExpr<OP,T1,T2>(a1, a2);
}

		} // vector

		// matrix
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Unary matrix expression.
template<typename OP, typename T1>
class UnMExpr: private omni::NonCopyable {
	typedef UnMExpr<OP,T1> this_type;

public:
	typedef OP Operator;  ///< @brief The operator type.
	typedef T1 Argument1; ///< @brief The first argument type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param a1 The first argument.
*/
	explicit UnMExpr(const Argument1 &a1)
		: m_a1(a1)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param other The other expression.
*/
	UnMExpr(const this_type &other)
		: m_a1(other.m_a1)
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get first argument.
	const Argument1& arg1() const
	{
		return m_a1;
	}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression on matrix.
	template<typename TZ, typename AZ> inline
	Matrix<TZ,AZ>& operator()(Matrix<TZ,AZ> &z) const
	{
		return Operator::eval(z, m_a1);
	}

private:
	const Argument1 &m_a1;
};


//////////////////////////////////////////////////////////////////////////
/// @brief Binary matrix expression.
template<typename OP, typename T1, typename T2>
class BiMExpr: private omni::NonCopyable {
	typedef BiMExpr<OP,T1,T2> this_type;

public:
	typedef OP Operator;  ///< @brief The operator type.
	typedef T1 Argument1; ///< @brief The first argument type.
	typedef T2 Argument2; ///< @brief The second argument type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param a1 The first argument.
@param a2 The second argument.
*/
	BiMExpr(const Argument1 &a1, const Argument2 &a2)
		: m_a1(a1),
		  m_a2(a2)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param other The other expression.
*/
	BiMExpr(const this_type &other)
		: m_a1(other.m_a1),
		  m_a2(other.m_a2)
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get first argument.
	const Argument1& arg1() const
	{
		return m_a1;
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Get second argument.
	const Argument2& arg2() const
	{
		return m_a2;
	}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Evaluate expression on matrix.
	template<typename TZ, typename AZ> inline
	Matrix<TZ,AZ>& operator()(Matrix<TZ,AZ> &z) const
	{
		return Operator::eval(z, m_a1, m_a2);
	}

private:
	const Argument1 &m_a1;
	const Argument2 &m_a2;
};



//////////////////////////////////////////////////////////////////////////
// create UnMExpr
template<typename OP, typename T1> inline
	UnMExpr<OP,T1> make_Mexpr(const T1 &a1)
{
	return UnMExpr<OP,T1>(a1);
}

//////////////////////////////////////////////////////////////////////////
// create BiMExpr
template<typename OP, typename T1, typename T2> inline
	BiMExpr<OP,T1,T2> make_Mexpr(const T1 &a1, const T2 &a2)
{
	return BiMExpr<OP,T1,T2>(a1, a2);
}

		} // matrix
	} // UnVExpr, BiVExpr, UnMExpr, BiMExpr


	// "is" functions
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Is the matrix symmetric?
/**
		The symmetric matrix is equal to the transposed.

@param x The matrix.
@return @b true if the matrix is symmetric, otherwise @b false.
*/
template<typename Tr, typename T, typename A>
bool is_symmetric(const Matrix<T,A> &x, const Tr&)
{
	if (!is_square(x))
		return false;

	for (size_t i = 1; i < x.Nrows(); ++i)
		for (size_t j = 0; j < i; ++j)
	{
		if (!Tr::equal(x.at(i, j), x.at(j, i)))
			return false;
	}

	return true;
}

template<typename T, typename A> inline
bool is_symmetric(const Matrix<T,A> &x)
{
	return is_symmetric(x, details::ValueTraits());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is the matrix hermitian?
/**
		The hermitian matrix is equal to the transposed and conjugated.

@param x The matrix.
@return @b true if the matrix is hermitian, otherwise @b false.
*/
template<typename Tr, typename T, typename A>
bool is_hermitian(const Matrix<T,A> &x, const Tr&)
{
	if (!is_square(x))
		return false;

	for (size_t i = 0; i < x.Nrows(); ++i)
		for (size_t j = 0; j <= i; ++j) // (!) check diagonal elements
	{
		if (!Tr::equal(x.at(i, j), Tr::conj(x.at(j, i))))
			return false;
	}

	return true;
}

template<typename T, typename A> inline
bool is_hermitian(const Matrix<T,A> &x)
{
	return is_hermitian(x, details::ValueTraits());
}

	} // "is" functions


	// elementary operations
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Swap two rows.
/**
	This function swaps two rows in the matrix.

@param x The matrix.
@param row1 The first row index.
@param row2 The second row index.
@param start_col The start column index.
@return The @a x matrix reference.
*/
template<typename T, typename A>
Matrix<T,A>& row_swap(Matrix<T,A> &x, size_t row1, size_t row2, size_t start_col = 0)
{
	if (row1 != row2)
	{
		assert(start_col < x.Ncols() && "column index out of range");
		assert(row1 < x.Nrows() && "first row index out of range");
		assert(row2 < x.Nrows() && "second row index out of range");

		typename Matrix<T,A>::row_iterator xi = x.row_begin(row1);
		const typename Matrix<T,A>::row_iterator xe = x.row_end(row1);
		typename Matrix<T,A>::row_iterator yi = x.row_begin(row2);

		xi += start_col;
		yi += start_col;

		for (; xi != xe; ++xi, ++yi)
			std::iter_swap(xi, yi);
	}

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Swap two columns.
/**
		This function swaps two columns in the matrix.

@param x The matrix.
@param col1 The first column index.
@param col2 The second column index.
@param start_row The start row index.
@return The @a x matrix reference.
*/
template<typename T, typename A>
Matrix<T,A>& col_swap(Matrix<T,A> &x, size_t col1, size_t col2, size_t start_row = 0)
{
	if (col1 != col2)
	{
		assert(start_row < x.Nrows() && "row index out of range");
		assert(col1 < x.Ncols() && "first column index out of range");
		assert(col2 < x.Ncols() && "second column index out of range");

		typename Matrix<T,A>::col_iterator xi = x.col_begin(col1);
		const typename Matrix<T,A>::col_iterator xe = x.col_end(col1);
		typename Matrix<T,A>::col_iterator yi = x.col_begin(col2);

		xi += start_row;
		yi += start_row;

		for (; xi != xe; ++xi, ++yi)
			std::iter_swap(xi, yi);
	}

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Scale row by specified value.
/**
@param x The matrix.
@param row The row index.
@param val The scale factor.
@param start_col The start column index.
@return The @a x matrix reference.
*/
template<typename T, typename A, typename U>
Matrix<T,A>& row_mul(Matrix<T,A> &x, size_t row, const U &val, size_t start_col = 0)
{
	assert(start_col < x.Ncols() && "column index out of range");
	assert(row < x.Nrows() && "row index out of range");

	typename Matrix<T,A>::row_iterator xi = x.row_begin(row);
	const typename Matrix<T,A>::row_iterator xe = x.row_end(row);

	xi += start_col;

	for (; xi != xe; ++xi)
		(*xi) *= val;

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Scale column by specified value.
/**
@param x The matrix.
@param col The column index.
@param val The scale factor.
@param start_row The start row index.
@return The @a x matrix reference.
*/
template<typename T, typename A, typename U>
Matrix<T,A>& col_mul(Matrix<T,A> &x, size_t col, const U &val, size_t start_row = 0)
{
	assert(start_row < x.Nrows() && "row index out of range");
	assert(col < x.Ncols() && "column index out of range");

	typename Matrix<T,A>::col_iterator xi = x.col_begin(col);
	const typename Matrix<T,A>::col_iterator xe = x.col_end(col);

	xi += start_row;

	for (; xi != xe; ++xi)
		(*xi) *= val;

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Add the scaled row.
/**
@param x The matrix.
@param target_row The target row index.
@param source_row The source row index.
@param val The scale factor.
@param start_col The start column index.
@return The @a x matrix reference.
*/
template<typename T, typename A, typename U>
Matrix<T,A>& row_fadd(Matrix<T,A> &x, size_t target_row,
	size_t source_row, const U &val, size_t start_col = 0)
{
	assert(source_row < x.Nrows() && "source row index out of range");
	assert(target_row < x.Nrows() && "target row index out of range");
	assert(start_col < x.Ncols() && "column index out of range");

	typename Matrix<T,A>::row_iterator xi = x.row_begin(target_row);
	const typename Matrix<T,A>::row_iterator xe = x.row_end(target_row);
	typename Matrix<T,A>::row_iterator yi = x.row_begin(source_row);

	xi += start_col;
	yi += start_col;

	for (; xi != xe; ++xi, ++yi)
		(*xi) += (*yi) * val;

	return x;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Add the scaled column.
/**
@param x The matrix.
@param target_col The target column index.
@param source_col The source column index.
@param val The scale factor.
@param start_row The start row index.
@return The @a x matrix reference.
*/
template<typename T, typename A, typename U>
Matrix<T,A>& col_fadd(Matrix<T,A> &x, size_t target_col,
	size_t source_col, const U &val, size_t start_row = 0)
{
	assert(source_col < x.Ncols() && "source column index out of range");
	assert(target_col < x.Ncols() && "source column index out of range");
	assert(start_row < x.Nrows() && "row index out of range");

	typename Matrix<T,A>::col_iterator xi = x.col_begin(target_col);
	const typename Matrix<T,A>::col_iterator xe = x.col_end(target_col);
	typename Matrix<T,A>::col_iterator yi = x.col_begin(source_col);

	xi += start_row;
	yi += start_row;

	for (; xi != xe; ++xi, ++yi)
		(*xi) += (*yi) * val;

	return x;
}

	} // elementary operations


	// det()
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Calculate determinant of the matrix.
/**
@param x The matrix, should be square.
@return The matrix determinant.
*/
template<typename Tr, typename T, typename A>
T det(Matrix<T,A> x, const Tr&)
{
	assert(is_square(x) && "matrix must be square");

	typedef typename Matrix<T,A>::value_type value_type;
	typedef typename Matrix<T,A>::size_type size_type;

	const value_type ZERO = value_type();
	const value_type ONE = Tr::one(ZERO);

	value_type res = ONE;
	for (size_type k = 0; k < x.Nrows(); ++k)
	{
		{ // find pivoting element in column
			value_type max_x = ZERO;
			size_type max_i = k;

			typename Matrix<T,A>::col_const_iterator xi = x.col_begin(k) + k;
			const typename Matrix<T,A>::col_const_iterator xe = x.col_end(k);

			for (size_type i = k; xi != xe; ++xi, ++i)
				if (Tr::norm(max_x) < Tr::norm(*xi))
					{ max_x = *xi; max_i = i; }

			// rows swap
			if (max_i != k)
			{
				row_swap(x, max_i, k, k);
				res = -res;
			}
		}

		if (Tr::equal(x.at(k, k), ZERO))
			return ZERO; // matrix is singular

		res *= x.at(k, k);
		row_mul(x, k, ONE / x.at(k, k), k);

		for (size_type i = k+1; i < x.Nrows(); ++i)
			row_fadd(x, i, k, -x.at(i, k), k);
	}

	return res;
}

template<typename T, typename A> inline
T det(const Matrix<T,A> &x)
{
	return det(x, details::ValueTraits());
}

	} // det()

	// norme()
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Euclidean norm of the vector.
/**
@param z The vector.
@return The vector norm.
*/
template<typename Tr, typename T, typename A>
T norme(const std::vector<T,A> &z, const Tr&)
{
	typedef typename std::vector<T,A>::value_type value_type;

	typename std::vector<T,A>::const_iterator zi = z.begin();
	const typename std::vector<T,A>::const_iterator ze = z.end();

	value_type sum2 = value_type();
	for (; zi != ze; ++zi)
		sum2 += Tr::norm(*zi);

	return Tr::sqrt(sum2); // TODO: with/without sqrt
}

template<typename T, typename A> inline
	T norme(const std::vector<T,A> &z)
{
	return norme(z, details::ValueTraits());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Euclidean norm of the matrix.
/**
@param z The matrix.
@return The matrix norm.
*/
template<typename Tr, typename T, typename A>
T norme(const Matrix<T,A> &z, const Tr&)
{
	typedef typename Matrix<T,A>::value_type value_type;

	typename Matrix<T,A>::const_iterator zi = z.begin();
	const typename Matrix<T,A>::const_iterator ze = z.end();

	value_type sum2 = value_type();
	for (; zi != ze; ++zi)
		sum2 += Tr::norm(*zi);

	return Tr::sqrt(sum2); // TODO: with/without sqrt
}

template<typename T, typename A> inline
	T norme(const Matrix<T,A> &z)
{
	return norme(z, details::ValueTraits());
}

	} // norme()



	// ref_equal
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief Are references equal?
template<typename X, typename Y>
	bool ref_equal(const X &x, const Y &y)
{
	return static_cast<const void*>(&x)
		== static_cast<const void*>(&y);
}

		}
	} // ref_equal


	// forward declarations
	namespace mx
	{
		namespace details
		{
			template<typename Tr>
				class Eye;
			class Diag;
			class VCat;
			class HCat;
			class SubMat;
			class RepMat;

			class Negate;
			class Trans;
			template<typename Tr>
				class Conj;
			template<typename Tr>
				class Herm;
			template<typename Tr>
				class Inv;

			class XAdd;
			class XSub;
			class XMul;
			class XDiv;
			class Mul;

			template<typename Tr>
				class Solve;
			template<typename Tr>
				class Chol_L;
			template<typename Tr>
				class Chol_U;
			// class QRD;
		}
	} // forward declarations


	// Eye
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Eye class
template<typename Tr>
class Eye {
public:

//////////////////////////////////////////////////////////////////////////
// evaluate expression
	template<typename TZ, typename AZ>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, size_t size)
	{
		typedef typename Matrix<TZ,AZ>::value_type value_type;
		typedef typename Matrix<TZ,AZ>::size_type size_type;

		const value_type ZERO = value_type();
		const value_type ONE = Tr::one(ZERO);

		z.resize(size, size);

		typename Matrix<TZ,AZ>::iterator it = z.begin();

	#if OMNI_MATRIX_ROW_MAJOR
		for (size_type i = 0; i < z.Nrows(); ++i)
			for (size_type j = 0; j < z.Ncols(); ++j, ++it)
				(*it) = (i == j) ? ONE : ZERO;
	#else
		for (size_type j = 0; j < z.Ncols(); ++j)
			for (size_type i = 0; i < z.Nrows(); ++i, ++it)
				(*it) = (i == j) ? ONE : ZERO;
	#endif

		return z;
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename Tr> inline
details::UnMExpr<details::Eye<Tr>,size_t> eye(const size_t &size, const Tr&)
	{ return details::make_Mexpr< details::Eye<Tr> >(size); }

//////////////////////////////////////////////////////////////////////////
inline details::UnMExpr<details::Eye<details::ValueTraits>,size_t> eye(const size_t &size)
	{ return eye(size, details::ValueTraits()); }

	} // Eye


	// Diag
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Diag class
class Diag {
public: // matrix = diag(vector)

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{
		const size_t N = x.size();

		z.assign(N, N, TZ());
		for (size_t i = 0; i < N; ++i)
			z.at(i, i) = x[i];

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x)
	{
		return eval(z, Vector<TZ,AZ>(x));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Vector<TZ,AZ>(x));
	}

public: // vector = diag(matrix)

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		// assert(is_square(x) && "matrix must be square");
		const size_t N = (x.Nrows() < x.Ncols()) ? x.Nrows() : x.Ncols();

		z.resize(N);
		for (size_t i = 0; i < N; ++i)
			z[i] = x.at(i, i);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x));
	}
};
		} // details

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Diag,std::vector<TX,AX> > diag(const std::vector<TX,AX> &x)
	{ return details::make_Mexpr< details::Diag >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Diag,details::UnVExpr<X_OP,X_T1> > diag(const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Diag >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Diag,details::BiVExpr<X_OP,X_T1,X_T2> > diag(const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Diag >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnVExpr<details::Diag,Matrix<TX,AX> > diag(const Matrix<TX,AX> &x)
	{ return details::make_Vexpr< details::Diag >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnVExpr<details::Diag,details::UnMExpr<X_OP,X_T1> > diag(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Vexpr< details::Diag >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Diag,details::BiMExpr<X_OP,X_T1,X_T2> > diag(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Vexpr< details::Diag >(x); }

	} // Diag


	// VCat
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// VCat class
class VCat {
public:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		assert(x.Ncols() == y.Ncols() && "number of columns should be the same");

		if (ref_equal(z, x))
		{
			if (ref_equal(z, y))
			{
				const Matrix<TZ,AZ> t(z);
				return do_vcat(z, t, t);
			}
			else
				return do_vcat(z, Matrix<TX,AX>(x), y);
		}
		else if (ref_equal(z, y))
			return do_vcat(z, x, Matrix<TY,AY>(y));
		else
			return do_vcat(z, x, y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Matrix<TZ,AZ>& do_vcat(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		z.resize(x.Nrows()+y.Nrows(), x.Ncols());

		for (size_t j = 0; j < z.Ncols(); ++j)
		{
			for (size_t i = 0; i < x.Nrows(); ++i)
				z.at(i,j) = x.at(i,j);

			for (size_t i = 0; i < y.Nrows(); ++i)
				z.at(i+x.Nrows(),j) = y.at(i,j);
		}

		return z;
	}
};

		} // details

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiMExpr<details::VCat, Matrix<TX,AX>, Matrix<TY,AY> > vcat(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::VCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::VCat, Matrix<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > vcat(const Matrix<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::VCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::VCat, Matrix<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > vcat(const Matrix<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::VCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiMExpr<details::VCat, details::UnMExpr<X_OP,X_T1>, Matrix<TY,AY> > vcat(const details::UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::VCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::VCat, details::UnMExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > vcat(const details::UnMExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::VCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::VCat, details::UnMExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > vcat(const details::UnMExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::VCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiMExpr<details::VCat, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > vcat(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::VCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::VCat, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > vcat(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::VCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::VCat, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > vcat(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::VCat>(x, y); }

	} // VCat


	// HCat
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// HCat class
class HCat {
public:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		assert(x.Nrows() == y.Nrows() && "number of rows should be the same");

		if (ref_equal(z, x))
		{
			if (ref_equal(z, y))
			{
				const Matrix<TZ,AZ> t(z);
				return do_hcat(z, t, t);
			}
			else
				return do_hcat(z, Matrix<TX,AX>(x), y);
		}
		else if (ref_equal(z, y))
			return do_hcat(z, x, Matrix<TY,AY>(y));
		else
			return do_hcat(z, x, y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Matrix<TZ,AZ>& do_hcat(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		z.resize(x.Nrows(), x.Ncols()+y.Ncols());

		for (size_t i = 0; i < z.Nrows(); ++i)
		{
			for (size_t j = 0; j < x.Ncols(); ++j)
				z.at(i,j) = x.at(i,j);

			for (size_t j = 0; j < y.Ncols(); ++j)
				z.at(i,j+x.Ncols()) = y.at(i,j);
		}

		return z;
	}
};

		} // details

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiMExpr<details::HCat, Matrix<TX,AX>, Matrix<TY,AY> > hcat(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::HCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::HCat, Matrix<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > hcat(const Matrix<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::HCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::HCat, Matrix<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > hcat(const Matrix<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::HCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiMExpr<details::HCat, details::UnMExpr<X_OP,X_T1>, Matrix<TY,AY> > hcat(const details::UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::HCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::HCat, details::UnMExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > hcat(const details::UnMExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::HCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::HCat, details::UnMExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > hcat(const details::UnMExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::HCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiMExpr<details::HCat, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > hcat(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::HCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::HCat, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > hcat(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::HCat>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::HCat, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > hcat(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::HCat>(x, y); }

	} // HCat


	// SubMat
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// SubMat class
class SubMat {
public:

	// coordinates
	class Coord {
	public:
		Coord(size_t r, size_t c, size_t Nr, size_t Nc)
			: row(r), col(c), Nrows(Nr), Ncols(Nc)
		{}

	public:
		size_t row;
		size_t col;
		size_t Nrows;
		size_t Ncols;
	};

public:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Coord &y)
	{
		assert(y.row < x.Nrows() && "invalid start row index");
		assert(y.row+y.Nrows < x.Nrows() && "invalid number of rows");
		assert(y.col < x.Ncols() && "invalid start column index");
		assert(y.col+y.Ncols < x.Ncols() && "invalid number of columns");

		if (ref_equal(z, x))
			return do_submat(z, Matrix<TX,AX>(x));
		else
			return do_submat(z, x);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Coord &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Coord &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& do_submat(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Coord &y)
	{
		z.resize(y.Nrows, y.Ncols);

		for (size_t i = 0; i < z.Nrows(); ++i)
			for (size_t j = 0; j < z.Ncols(); ++j)
				z.at(i, j) = x.at(i+y.row, j+y.col);

		return z;
	}

};

		} // details

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::BiMExpr<details::SubMat, Matrix<TX,AX>, details::SubMat::Coord> submat(const Matrix<TX,AX> &x, size_t row, size_t col, size_t Nrows, size_t Ncols)
	{ return details::make_Mexpr<details::SubMat>(x, details::SubMat::Coord(row, col, Nrows, Ncols)); };

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::BiMExpr<details::SubMat, details::UnMExpr<X_OP,X_T1>, details::SubMat::Coord> submat(const details::UnMExpr<X_OP,X_T1> &x, size_t row, size_t col, size_t Nrows, size_t Ncols)
	{ return details::make_Mexpr<details::SubMat>(x, details::SubMat::Coord(row, col, Nrows, Ncols)); };

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::BiMExpr<details::SubMat, details::BiMExpr<X_OP,X_T1,X_T2>, details::SubMat::Coord> submat(const details::BiMExpr<X_OP,X_T1,X_T2> &x, size_t row, size_t col, size_t Nrows, size_t Ncols)
	{ return details::make_Mexpr<details::SubMat>(x, details::SubMat::Coord(row, col, Nrows, Ncols)); };

	} // SubMat


	// RepMat
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// RepMat class
class RepMat {
public:

	// coordinates
	class Coord {
	public:
		Coord(size_t Xr, size_t Xc)
			: Xrows(Xr), Xcols(Xc)
		{}

	public:
		size_t Xrows;
		size_t Xcols;
	};

public:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Coord &y)
	{
		if (ref_equal(z, x))
			return do_repmat(z, Matrix<TX,AX>(x));
		else
			return do_repmat(z, x);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Coord &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Coord &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& do_repmat(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Coord &y)
	{
		z.resize(x.Nrows()*y.Xrows, x.Ncols()*y.Xcols);

		for (size_t i = 0; i < x.Nrows(); ++i)
			for (size_t j = 0; j < x.Ncols(); ++j)
		{
			const TX &val = x.at(i, j);

			for (size_t xi = 0; xi < y.Xrows; ++xi)
				for (size_t xj = 0; xj < y.Ncols; ++xj)
					z.at(xi*x.Nrows() + i, xj*x.Ncols() + j) = val;
		}

		return z;
	}

};

		} // details

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::BiMExpr<details::RepMat, Matrix<TX,AX>, details::RepMat::Coord> repmat(const Matrix<TX,AX> &x, size_t row, size_t col, size_t Nrows, size_t Ncols)
	{ return details::make_Mexpr<details::RepMat>(x, details::RepMat::Coord(row, col, Nrows, Ncols)); };

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::BiMExpr<details::RepMat, details::UnMExpr<X_OP,X_T1>, details::RepMat::Coord> repmat(const details::UnMExpr<X_OP,X_T1> &x, size_t row, size_t col, size_t Nrows, size_t Ncols)
	{ return details::make_Mexpr<details::RepMat>(x, details::RepMat::Coord(row, col, Nrows, Ncols)); };

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::BiMExpr<details::RepMat, details::BiMExpr<X_OP,X_T1,X_T2>, details::RepMat::Coord> repmat(const details::BiMExpr<X_OP,X_T1,X_T2> &x, size_t row, size_t col, size_t Nrows, size_t Ncols)
	{ return details::make_Mexpr<details::RepMat>(x, details::RepMat::Coord(row, col, Nrows, Ncols)); };

	} // RepMat


	// Negate
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Negate class
class Negate {
public: // vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename Vector<TZ,AZ>::iterator zi = z.begin();
		const typename Vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = -(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<Negate,X_T1> &x)
	{
		return z = x.arg1();
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = -(*xi);

		return z;
	}


//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<Negate,X_T1> &x)
	{
		return z = x.arg1();
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

};

		} // details

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnVExpr<details::Negate, std::vector<TX,AX> > operator-(const std::vector<TX,AX> &x)
	{ return details::make_Vexpr<details::Negate>(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnVExpr<details::Negate, details::UnVExpr<X_OP,X_T1> > operator-(const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::make_Vexpr<details::Negate>(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Negate, details::BiVExpr<X_OP,X_T1,X_T2> > operator-(const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Vexpr<details::Negate>(x); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Negate, Matrix<TX,AX> > operator-(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr<details::Negate>(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Negate, details::UnMExpr<X_OP,X_T1> > operator-(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr<details::Negate>(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Negate, details::BiMExpr<X_OP,X_T1,X_T2> > operator-(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr<details::Negate>(x); }

	} // Negate

	// Trans
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Trans class
class Trans {
public: // vector
	// (?) transpose operation for vector

public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		if (x.Nrows()==1 || x.Ncols()==1)
		{
			z.resize(x.Ncols(), x.Nrows());
			if (!ref_equal(z, x))
			{
				std::copy(x.begin(),
					x.end(), z.begin());
			}

			return z;
		}
		else if (ref_equal(z, x))
		{
			return is_square(x) ? do_trans(z)
				: do_trans(z, Matrix<TX,AX>(x));
		}
		else
			return do_trans(z, x);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<Trans,X_T1> &x)
	{
		return z = x.arg1();
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

private:

	// transpose
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& do_trans(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		z.resize(x.Ncols(), x.Nrows());

		if (!is_empty(x))
		{
			for (size_t i = 0; i < x.Nrows(); ++i)
				for (size_t j = 0; j < x.Ncols(); ++j)
					z.at(j, i) = x.at(i, j);
		}

		return z;
	}

	// in-place transpose (square matrix)
	template<typename TZ, typename AZ>
	static Matrix<TZ,AZ>& do_trans(Matrix<TZ,AZ> &z)
	{
		for (size_t i = 1; i < z.Nrows(); ++i)
			for (size_t j = 0; j < i; ++j)
				std::swap(z.at(i, j), z.at(j, i));

		return z;
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Trans, Matrix<TX,AX> > trans(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr<details::Trans>(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Trans, details::UnMExpr<X_OP,X_T1> > trans(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr<details::Trans>(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Trans, details::BiMExpr<X_OP,X_T1,X_T2> > trans(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr<details::Trans>(x); }

	} // Trans


	// Conj
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Conj class
template<typename Tr>
class Conj {
public: // vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::conj(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<Conj<Tr>,X_T1> &x)
	{
		return z = x.arg1();
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::conj(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<Conj<Tr>,X_T1> &x)
	{
		return z = x.arg1();
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnVExpr<details::Conj<Tr>, std::vector<TX,AX> > conj(const std::vector<TX,AX> &x, const Tr&)
	{ return details::make_Vexpr< details::Conj<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnVExpr<details::Conj<details::ValueTraits>, std::vector<TX,AX> > conj(const std::vector<TX,AX> &x)
	{ return details::make_Vexpr< details::Conj<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnVExpr<details::Conj<Tr>, details::UnVExpr<X_OP,X_T1> > conj(const details::UnVExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Vexpr< details::Conj<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnVExpr<details::Conj<details::ValueTraits>, details::UnVExpr<X_OP,X_T1> > conj(const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::make_Vexpr< details::Conj<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Conj<Tr>, details::BiVExpr<X_OP,X_T1,X_T2> > conj(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Vexpr< details::Conj<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Conj<details::ValueTraits>, details::BiVExpr<X_OP,X_T1,X_T2> > conj(const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Vexpr< details::Conj<details::ValueTraits> >(x); }



//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Conj<Tr>, Matrix<TX,AX> > conj(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Conj<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Conj<details::ValueTraits>, Matrix<TX,AX> > conj(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Conj<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Conj<Tr>, details::UnMExpr<X_OP,X_T1> > conj(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Conj<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Conj<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > conj(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Conj<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Conj<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > conj(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Conj<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Conj<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > conj(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Conj<details::ValueTraits> >(x); }

	} // Conj


	// Herm
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Herm class
template<typename Tr>
class Herm {
public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		return Conj<Tr>::eval(z, Trans::eval(z, x));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<Herm<Tr>,X_T1> &x)
	{
		return z = x.arg1();
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Herm<Tr>, Matrix<TX,AX> > herm(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Herm<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Herm<details::ValueTraits>, Matrix<TX,AX> > herm(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Herm<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Herm<Tr>, details::UnMExpr<X_OP,X_T1> > herm(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Herm<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Herm<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > herm(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Herm<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Herm<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > herm(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Herm<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Herm<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > herm(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Herm<details::ValueTraits> >(x); }

	} // Herm


	// Norm
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Norm class
template<typename Tr>
class Norm {
public: // vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::norm(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::norm(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnVExpr<details::Norm<Tr>, std::vector<TX,AX> > norm(const std::vector<TX,AX> &x, const Tr&)
	{ return details::make_Vexpr< details::Norm<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnVExpr<details::Norm<details::ValueTraits>, std::vector<TX,AX> > norm(const std::vector<TX,AX> &x)
	{ return details::make_Vexpr< details::Norm<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnVExpr<details::Norm<Tr>, details::UnVExpr<X_OP,X_T1> > norm(const details::UnVExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Vexpr< details::Norm<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnVExpr<details::Norm<details::ValueTraits>, details::UnVExpr<X_OP,X_T1> > norm(const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::make_Vexpr< details::Norm<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Norm<Tr>, details::BiVExpr<X_OP,X_T1,X_T2> > norm(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Vexpr< details::Norm<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Norm<details::ValueTraits>, details::BiVExpr<X_OP,X_T1,X_T2> > norm(const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Vexpr< details::Norm<details::ValueTraits> >(x); }



//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Norm<Tr>, Matrix<TX,AX> > norm(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Norm<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Norm<details::ValueTraits>, Matrix<TX,AX> > norm(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Norm<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Norm<Tr>, details::UnMExpr<X_OP,X_T1> > norm(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Norm<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Norm<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > norm(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Norm<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Norm<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > norm(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Norm<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Norm<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > norm(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Norm<details::ValueTraits> >(x); }

	} // Norm


	// Abs
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Abs class
template<typename Tr>
class Abs {
public: // vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::abs(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::abs(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnVExpr<details::Abs<Tr>, std::vector<TX,AX> > abs(const std::vector<TX,AX> &x, const Tr&)
	{ return details::make_Vexpr< details::Abs<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnVExpr<details::Abs<details::ValueTraits>, std::vector<TX,AX> > abs(const std::vector<TX,AX> &x)
	{ return details::make_Vexpr< details::Abs<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnVExpr<details::Abs<Tr>, details::UnVExpr<X_OP,X_T1> > abs(const details::UnVExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Vexpr< details::Abs<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnVExpr<details::Abs<details::ValueTraits>, details::UnVExpr<X_OP,X_T1> > abs(const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::make_Vexpr< details::Abs<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Abs<Tr>, details::BiVExpr<X_OP,X_T1,X_T2> > abs(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Vexpr< details::Abs<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Abs<details::ValueTraits>, details::BiVExpr<X_OP,X_T1,X_T2> > abs(const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Vexpr< details::Abs<details::ValueTraits> >(x); }



//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Abs<Tr>, Matrix<TX,AX> > abs(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Abs<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Abs<details::ValueTraits>, Matrix<TX,AX> > abs(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Abs<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Abs<Tr>, details::UnMExpr<X_OP,X_T1> > abs(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Abs<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Abs<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > abs(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Abs<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Abs<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > abs(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Abs<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Abs<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > abs(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Abs<details::ValueTraits> >(x); }

	} // Abs


	// Sqrt
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Sqrt class
template<typename Tr>
class Sqrt {
public: // vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::sqrt(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::sqrt(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnVExpr<details::Sqrt<Tr>, std::vector<TX,AX> > sqrt(const std::vector<TX,AX> &x, const Tr&)
	{ return details::make_Vexpr< details::Sqrt<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnVExpr<details::Sqrt<details::ValueTraits>, std::vector<TX,AX> > sqrt(const std::vector<TX,AX> &x)
	{ return details::make_Vexpr< details::Sqrt<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnVExpr<details::Sqrt<Tr>, details::UnVExpr<X_OP,X_T1> > sqrt(const details::UnVExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Vexpr< details::Sqrt<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnVExpr<details::Sqrt<details::ValueTraits>, details::UnVExpr<X_OP,X_T1> > sqrt(const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::make_Vexpr< details::Sqrt<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Sqrt<Tr>, details::BiVExpr<X_OP,X_T1,X_T2> > sqrt(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Vexpr< details::Sqrt<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Sqrt<details::ValueTraits>, details::BiVExpr<X_OP,X_T1,X_T2> > sqrt(const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Vexpr< details::Sqrt<details::ValueTraits> >(x); }



//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Sqrt<Tr>, Matrix<TX,AX> > sqrt(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Sqrt<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Sqrt<details::ValueTraits>, Matrix<TX,AX> > sqrt(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Sqrt<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Sqrt<Tr>, details::UnMExpr<X_OP,X_T1> > sqrt(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Sqrt<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Sqrt<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > sqrt(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Sqrt<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Sqrt<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > sqrt(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Sqrt<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Sqrt<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > sqrt(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Sqrt<details::ValueTraits> >(x); }

	} // Sqrt


	// Real
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Real class
template<typename Tr>
class Real {
public: // vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::real(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::real(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnVExpr<details::Real<Tr>, std::vector<TX,AX> > real(const std::vector<TX,AX> &x, const Tr&)
	{ return details::make_Vexpr< details::Real<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnVExpr<details::Real<details::ValueTraits>, std::vector<TX,AX> > real(const std::vector<TX,AX> &x)
	{ return details::make_Vexpr< details::Real<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnVExpr<details::Real<Tr>, details::UnVExpr<X_OP,X_T1> > real(const details::UnVExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Vexpr< details::Real<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnVExpr<details::Real<details::ValueTraits>, details::UnVExpr<X_OP,X_T1> > real(const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::make_Vexpr< details::Real<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Real<Tr>, details::BiVExpr<X_OP,X_T1,X_T2> > real(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Vexpr< details::Real<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Real<details::ValueTraits>, details::BiVExpr<X_OP,X_T1,X_T2> > real(const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Vexpr< details::Real<details::ValueTraits> >(x); }



//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Real<Tr>, Matrix<TX,AX> > real(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Real<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Real<details::ValueTraits>, Matrix<TX,AX> > real(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Real<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Real<Tr>, details::UnMExpr<X_OP,X_T1> > real(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Real<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Real<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > real(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Real<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Real<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > real(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Real<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Real<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > real(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Real<details::ValueTraits> >(x); }

	} // Real


	// Imag
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Imag class
template<typename Tr>
class Imag {
public: // vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::imag(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Vector<TZ,AZ>(x)); // (!) may be optimized
	}

public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = Tr::imag(*xi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnVExpr<details::Imag<Tr>, std::vector<TX,AX> > imag(const std::vector<TX,AX> &x, const Tr&)
	{ return details::make_Vexpr< details::Imag<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnVExpr<details::Imag<details::ValueTraits>, std::vector<TX,AX> > imag(const std::vector<TX,AX> &x)
	{ return details::make_Vexpr< details::Imag<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnVExpr<details::Imag<Tr>, details::UnVExpr<X_OP,X_T1> > imag(const details::UnVExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Vexpr< details::Imag<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnVExpr<details::Imag<details::ValueTraits>, details::UnVExpr<X_OP,X_T1> > imag(const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::make_Vexpr< details::Imag<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Imag<Tr>, details::BiVExpr<X_OP,X_T1,X_T2> > imag(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Vexpr< details::Imag<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnVExpr<details::Imag<details::ValueTraits>, details::BiVExpr<X_OP,X_T1,X_T2> > imag(const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Vexpr< details::Imag<details::ValueTraits> >(x); }



//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Imag<Tr>, Matrix<TX,AX> > imag(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Imag<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Imag<details::ValueTraits>, Matrix<TX,AX> > imag(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Imag<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Imag<Tr>, details::UnMExpr<X_OP,X_T1> > imag(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Imag<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Imag<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > imag(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Imag<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Imag<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > imag(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Imag<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Imag<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > imag(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Imag<details::ValueTraits> >(x); }

	} // Imag


	// Solve
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Solve class (X*Z=Y) i.e. Z = (X^-1) * Y
template<typename Tr>
class Solve {
public: // vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Matrix<TX,AX> &x, const std::vector<TY,AY> &y)
	{
		assert(is_square(x) && "matrix must be square");
		assert(y.size()==x.Nrows() && "invalid vector size");

		if (ref_equal(z, y))
			return do_solve(z, x);
		else
			return do_solve(z=y, x);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
		static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
		static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
		static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
		static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

public: // matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		assert(is_square(x) && "matrix must be square");
		assert(y.Nrows()==x.Nrows() && "invalid number of rows");

		if (ref_equal(z, y))
			return do_solve(z, x);
		else
			return do_solve(z=y, x);
	}


//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
		static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
		static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
		static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
		static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Vector<TZ,AZ>& do_solve(Vector<TZ,AZ> &z, Matrix<TX,AX> x)
	{
		typedef typename std::vector<TZ,AZ>::value_type value_type;
		typedef typename std::vector<TZ,AZ>::size_type size_type;

		const value_type ZERO = value_type();
		const value_type ONE = Tr::one(ZERO);

		// forward
		for (size_type k = 0; k < x.Nrows(); ++k)
		{
			{ // find pivoting element in column
				value_type max_x = ZERO;
				size_type max_i = k;

				typename Matrix<TX,AX>::col_const_iterator xi = x.col_begin(k) + k;
				const typename Matrix<TX,AX>::col_const_iterator xe = x.col_end(k);

				for (size_type i = k; xi != xe; ++xi, ++i)
					if (Tr::norm(max_x) < Tr::norm(*xi))
						{ max_x = *xi; max_i = i; }

				// rows swap
				if (max_i != k)
				{
					row_swap(x, max_i, k, k);
					std::swap(z[max_i], z[k]);
				}
			}

			if (Tr::equal(x.at(k, k), ZERO))
				throw err::MatrixIsSingular();

			const value_type ff = ONE / x.at(k, k);
			row_mul(x, k, ff, k);
			z[k] *= ff;

			for (size_type i = k+1; i < x.Nrows(); ++i)
			{
				const value_type dd = x.at(i, k);
				row_fadd(x, i, k, -dd, k);
				z[i] -= z[k]*dd;
			}
		}

		// back trace
		for (ptrdiff_t j = x.Ncols()-1; 0 <= j; --j)
			for (ptrdiff_t i = j-1; 0 <= i; --i)
		{
			const value_type dd = x.at(i, j);
			row_fadd(x, i, j, -dd, x.Ncols()-i-1);
			z[i] -= z[j]*dd;
		}

		return z;
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& do_solve(Matrix<TZ,AZ> &z, Matrix<TX,AX> x)
	{
		typedef typename Matrix<TZ,AZ>::value_type value_type;
		typedef typename Matrix<TZ,AZ>::size_type size_type;

		const value_type ZERO = value_type();
		const value_type ONE = Tr::one(ZERO);

		// forward
		for (size_type k = 0; k < x.Nrows(); ++k)
		{
			{ // find pivoting element in column
				value_type max_x = ZERO;
				size_type max_i = k;

				typename Matrix<TX,AX>::col_const_iterator xi = x.col_begin(k) + k;
				const typename Matrix<TX,AX>::col_const_iterator xe = x.col_end(k);

				for (size_type i = k; xi != xe; ++xi, ++i)
					if (Tr::norm(max_x) < Tr::norm(*xi))
						{ max_x = *xi; max_i = i; }

				// rows swap
				if (max_i != k)
				{
					row_swap(x, max_i, k, k);
					row_swap(z, max_i, k);
				}
			}

			if (Tr::equal(x.at(k, k), ZERO))
				throw err::MatrixIsSingular();

			const value_type ff = ONE / x.at(k, k);
			row_mul(x, k, ff, k);
			row_mul(z, k, ff);

			for (size_type i = k+1; i < x.Nrows(); ++i)
			{
				const value_type dd = -x.at(i, k);
				row_fadd(x, i, k, dd, k);
				row_fadd(z, i, k, dd);
			}
		}

		// back trace
		for (ptrdiff_t j = x.Ncols()-1; 0 <= j; --j)
			for (ptrdiff_t i = j-1; 0 <= i; --i)
		{
			const value_type dd = -x.at(i, j);
			row_fadd(x, i, j, dd, x.Ncols()-i-1);
			row_fadd(z, i, j, dd);
		}

		return z;
	}
};

		} // details

//////////////////////////////////////////////////////////////////////////////
// computes the solution to the system of linear equations (X*Z=Y), i.e. Z = inv(X)*Y

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX, typename TY, typename AY> inline
details::BiVExpr<details::Solve<Tr>, Matrix<TX,AX>, std::vector<TY,AY> > solve(const Matrix<TX,AX> &x, const std::vector<TY,AY> &y, const Tr&)
	{ return details::make_Vexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiVExpr<details::Solve<details::ValueTraits>, Matrix<TX,AX>, std::vector<TY,AY> > solve(const Matrix<TX,AX> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Solve<Tr>, Matrix<TX,AX>, details::UnVExpr<Y_OP,Y_T1> > solve(const Matrix<TX,AX> &x, const details::UnVExpr<Y_OP,Y_T1> &y, const Tr&)
	{ return details::make_Vexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Solve<details::ValueTraits>, Matrix<TX,AX>, details::UnVExpr<Y_OP,Y_T1> > solve(const Matrix<TX,AX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Solve<Tr>, Matrix<TX,AX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > solve(const Matrix<TX,AX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y, const Tr&)
	{ return details::make_Vexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Solve<details::ValueTraits>, Matrix<TX,AX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > solve(const Matrix<TX,AX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiVExpr<details::Solve<Tr>, details::UnMExpr<X_OP,X_T1>, std::vector<TY,AY> > solve(const details::UnMExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y, const Tr&)
	{ return details::make_Vexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiVExpr<details::Solve<details::ValueTraits>, details::UnMExpr<X_OP,X_T1>, std::vector<TY,AY> > solve(const details::UnMExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Solve<Tr>, details::UnMExpr<X_OP,X_T1>, details::UnVExpr<Y_OP,Y_T1> > solve(const details::UnMExpr<X_OP,X_T1> &x, const details::UnVExpr<Y_OP,Y_T1> &y, const Tr&)
	{ return details::make_Vexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Solve<details::ValueTraits>, details::UnMExpr<X_OP,X_T1>, details::UnVExpr<Y_OP,Y_T1> > solve(const details::UnMExpr<X_OP,X_T1> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Solve<Tr>, details::UnMExpr<X_OP,X_T1>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > solve(const details::UnMExpr<X_OP,X_T1> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y, const Tr&)
	{ return details::make_Vexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Solve<details::ValueTraits>, details::UnMExpr<X_OP,X_T1>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > solve(const details::UnMExpr<X_OP,X_T1> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiVExpr<details::Solve<Tr>, details::BiMExpr<X_OP,X_T1,X_T2>, std::vector<TY,AY> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y, const Tr&)
	{ return details::make_Vexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiVExpr<details::Solve<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2>, std::vector<TY,AY> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Solve<Tr>, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnVExpr<Y_OP,Y_T1> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnVExpr<Y_OP,Y_T1> &y, const Tr&)
	{ return details::make_Vexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Solve<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnVExpr<Y_OP,Y_T1> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Solve<Tr>, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y, const Tr&)
	{ return details::make_Vexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Solve<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr< details::Solve<details::ValueTraits> >(x, y); }



//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX, typename TY, typename AY> inline
details::BiMExpr<details::Solve<Tr>, Matrix<TX,AX>, Matrix<TY,AY> > solve(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y, const Tr&)
	{ return details::make_Mexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiMExpr<details::Solve<details::ValueTraits>, Matrix<TX,AX>, Matrix<TY,AY> > solve(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::Solve<Tr>, Matrix<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > solve(const Matrix<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y, const Tr&)
	{ return details::make_Mexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::Solve<details::ValueTraits>, Matrix<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > solve(const Matrix<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::Solve<Tr>, Matrix<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > solve(const Matrix<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y, const Tr&)
	{ return details::make_Mexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::Solve<details::ValueTraits>, Matrix<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > solve(const Matrix<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiMExpr<details::Solve<Tr>, details::UnMExpr<X_OP,X_T1>, Matrix<TY,AY> > solve(const details::UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y, const Tr&)
	{ return details::make_Mexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiMExpr<details::Solve<details::ValueTraits>, details::UnMExpr<X_OP,X_T1>, Matrix<TY,AY> > solve(const details::UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::Solve<Tr>, details::UnMExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > solve(const details::UnMExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y, const Tr&)
	{ return details::make_Mexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::Solve<details::ValueTraits>, details::UnMExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > solve(const details::UnMExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::Solve<Tr>, details::UnMExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > solve(const details::UnMExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y, const Tr&)
	{ return details::make_Mexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::Solve<details::ValueTraits>, details::UnMExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > solve(const details::UnMExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiMExpr<details::Solve<Tr>, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y, const Tr&)
	{ return details::make_Mexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiMExpr<details::Solve<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::Solve<Tr>, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y, const Tr&)
	{ return details::make_Mexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::Solve<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::Solve<Tr>, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y, const Tr&)
	{ return details::make_Mexpr< details::Solve<Tr> >(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::Solve<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > solve(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr< details::Solve<details::ValueTraits> >(x, y); }

	} // Solve


	// Inv
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Inv class
template<typename Tr>
class Inv {
public:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		assert(is_square(x) && "matrix must be square");

		return ref_equal(z, x)
			? do_inv(z, Matrix<TX,AX>(x))
			: do_inv(z, x);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<Inv<Tr>,X_T1> &x)
	{
		return z = x.arg1();
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x));
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX> inline
	static Matrix<TZ,AZ>& do_inv(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		assert(is_square(x) && "matrix must be square");

		Eye<Tr>::eval(z, x.Nrows());
		return Solve<Tr>::eval(z, x, z);
	}
};

		} // details

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Inv<Tr>, Matrix<TX,AX> > inv(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Inv<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Inv<details::ValueTraits>, Matrix<TX,AX> > inv(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Inv<details::ValueTraits> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Inv<Tr>, details::UnMExpr<X_OP,X_T1> > inv(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Inv<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Inv<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > inv(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Inv<details::ValueTraits> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Inv<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > inv(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Inv<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Inv<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > inv(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Inv<details::ValueTraits> >(x); }

	} // Inv


	// Chol_L, Chol_U
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Cholesky (lower)
template<typename Tr>
class Chol_L {
public:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		assert(is_square(x) && "matrix must be square");
		z.resize(x.Nrows(), x.Ncols());

		// TODO: check ref_equal(z, x)?
		return do_chol(z, x);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& do_chol(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		typedef typename Matrix<TZ,AZ>::value_type value_type;
		typedef typename Matrix<TZ,AZ>::size_type size_type;
		const value_type ZERO = value_type();

		const size_type N = x.Nrows();
		for (size_type k = 0; k < N; ++k)
		{
			value_type d = x.at(k, k);
			for (size_type j = 0; j < k; ++j)
				d -= z.at(k, j) * Tr::conj(z.at(k, j));

			if (Tr::real(d) <= Tr::real(ZERO))
				throw err::MatrixNotPositiveDefinite();

			z.at(k, k) = Tr::sqrt(Tr::real(d));

			for (size_type i = k+1; i < N; ++i)
			{
				value_type s = x.at(i, k);
				for (size_type j = 0; j < k; ++j)
					s -= z.at(i, j) * Tr::conj(z.at(k, j));
				z.at(i, k) = s / Tr::real(z.at(k, k));
			}

			for (size_type j = k+1; j < N; ++j)
				z.at(k, j) = ZERO;
		}

		return z;
	}
};


//////////////////////////////////////////////////////////////////////////
// Cholesky (upper)
template<typename Tr>
class Chol_U {
public:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		assert(is_square(x) && "matrix must be square");
		z.resize(x.Nrows(), x.Ncols());

		// TODO: check ref_equal(z, x)?
		return do_chol(z, x);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x)
	{
		return eval(z, Matrix<TZ,AZ>(x)); // (!) may be optimized
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX>
	static Matrix<TZ,AZ>& do_chol(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{
		typedef typename Matrix<TZ,AZ>::value_type value_type;
		typedef typename Matrix<TZ,AZ>::size_type size_type;
		const value_type ZERO = value_type();

		const size_type N = x.Nrows();
		for (size_type k = 0; k < N; ++k)
		{
			value_type d = x.at(k, k);
			for (size_type i = 0; i < k; ++i)
				d -= z.at(i, k) * Tr::conj(z.at(i, k));

			if (Tr::real(d) <= Tr::real(ZERO))
				throw err::MatrixNotPositiveDefinite();

			z.at(k, k) = Tr::sqrt(Tr::real(d));

			for (size_type j = k+1; j < N; ++j)
			{
				value_type s = x.at(k, j);
				for (size_type i = 0; i < k; ++i)
					s -= z.at(i, j) * Tr::conj(z.at(i, k));
				z.at(k, j) = s / Tr::real(z.at(k, k));
			}

			for (size_type i = k+1; i < N; ++i)
				z.at(i, k) = ZERO;
		}

		return z;
	}
};

		} // details

// Cholesky lower: x = L * herm(L)

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Chol_L<Tr>, Matrix<TX,AX> > choll(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Chol_L<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Chol_L<details::ValueTraits>, Matrix<TX,AX> > choll(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Chol_L<details::ValueTraits> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Chol_L<Tr>, details::UnMExpr<X_OP,X_T1> > choll(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Chol_L<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Chol_L<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > choll(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Chol_L<details::ValueTraits> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Chol_L<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > choll(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Chol_L<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Chol_L<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > choll(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Chol_L<details::ValueTraits> >(x); }


// Cholesky upper: x = herm(U) * U

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX> inline
details::UnMExpr<details::Chol_U<Tr>, Matrix<TX,AX> > cholu(const Matrix<TX,AX> &x, const Tr&)
	{ return details::make_Mexpr< details::Chol_U<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX> inline
details::UnMExpr<details::Chol_U<details::ValueTraits>, Matrix<TX,AX> > cholu(const Matrix<TX,AX> &x)
	{ return details::make_Mexpr< details::Chol_U<details::ValueTraits> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1> inline
details::UnMExpr<details::Chol_U<Tr>, details::UnMExpr<X_OP,X_T1> > cholu(const details::UnMExpr<X_OP,X_T1> &x, const Tr&)
	{ return details::make_Mexpr< details::Chol_U<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1> inline
details::UnMExpr<details::Chol_U<details::ValueTraits>, details::UnMExpr<X_OP,X_T1> > cholu(const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::make_Mexpr< details::Chol_U<details::ValueTraits> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Chol_U<Tr>, details::BiMExpr<X_OP,X_T1,X_T2> > cholu(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Tr&)
	{ return details::make_Mexpr< details::Chol_U<Tr> >(x); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2> inline
details::UnMExpr<details::Chol_U<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2> > cholu(const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::make_Mexpr< details::Chol_U<details::ValueTraits> >(x); }

	} // Chol_L, Chol_U


	// QRD
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// QRD: X = Q*R
// herm(Q)*Q == I
// R is upper triangular matrix
template<typename Tr>
class QRD {
public:

//////////////////////////////////////////////////////////////////////////
	template<typename TQ, typename AQ, typename TX, typename AX, typename TR, typename AR>
	static Matrix<TQ,AQ>& eval(Matrix<TQ,AQ> &q, const Matrix<TX,AX> &x, const Matrix<TR,AR> &r)
	{
		assert(x.Ncols() <= x.Nrows() && "invalid matrix size!");
		assert(!ref_equal(q, r) && "Q and R matrices can't be the same");
		return do_qrd(q = x, const_cast<Matrix<TR,AR>&>(r));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TQ, typename AQ, typename X_OP, typename X_T1, typename TR, typename AR>
	static Matrix<TQ,AQ>& eval(Matrix<TQ,AQ> &q, const UnMExpr<X_OP,X_T1> &x, const Matrix<TR,AR> &r)
	{
		return eval(q, Matrix<TQ,AQ>(x), r); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TQ, typename AQ, typename X_OP, typename X_T1, typename X_T2, typename TR, typename AR>
	static Matrix<TQ,AQ>& eval(Matrix<TQ,AQ> &q, const BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TR,AR> &r)
	{
		return eval(q, Matrix<TQ,AQ>(x), r); // (!) may be optimized
	}

private:

//////////////////////////////////////////////////////////////////////////
	template<typename TQ, typename AQ, typename TR, typename AR>
	static Matrix<TQ,AQ>& do_qrd(Matrix<TQ,AQ> &Q, Matrix<TR,AR> &R)
	{
		typedef typename Matrix<TQ,AQ>::value_type value_type;
		const value_type ZERO = value_type();
		const value_type ONE = Tr::one(ZERO);

		R.assign(Q.Ncols(), Q.Ncols(), ZERO);

		for (size_t k = 0; k < Q.Ncols(); ++k)
		{
			value_type nrm = ZERO;
			for (size_t i = k; i < Q.Nrows(); ++i)
				nrm += Tr::norm(Q.at(i, k));
			nrm = Tr::sqrt(nrm);

			if (Tr::equal(nrm, ZERO))
				throw err::MatrixIsSingular();

			value_type &Qkk = Q.at(k,k);
			if (Tr::norm(Qkk+nrm) < Tr::norm(Qkk-nrm))
				nrm = -nrm;

			col_mul(Q, k, Tr::real(ONE/nrm), k);
			Qkk += ONE;

			for (size_t j = k+1; j < Q.Ncols(); ++j)
			{
				value_type s = ZERO;
				for (size_t i = k; i < Q.Nrows(); ++i)
					s += Tr::conj(Q.at(i, k)) * Q.at(i, j);
				s /= -Q.at(k, k);

				col_fadd(Q, j, k, s, k);
			}

			R.at(k, k) = -nrm;
		}

		// extract R matrix
		for (size_t j = 1; j < R.Ncols(); ++j)
			for (size_t i = 0; i < j; ++i)
				R.at(i, j) = Q.at(i, j);

		Matrix<TQ,AQ> QR(Q.Nrows(), Q.Ncols(), ZERO);
		Q.swap(QR); // (!)

		// extract Q matrix
		for (ptrdiff_t k = Q.Ncols()-1; 0 <= k; --k)
		{
			Q.at(k, k) = ONE;
			for (size_t j = k; j < Q.Ncols(); ++j)
			{
				value_type s = ZERO;
				for (size_t i = k; i < Q.Nrows(); ++i)
					s += Tr::conj(QR.at(i, k)) * Q.at(i, j);
				s /= -Tr::conj(QR.at(k, k));

				for (size_t i = k; i < Q.Nrows(); ++i)
					Q.at(i, j) += s * QR.at(i, k);
			}
		}

		return Q;
	}
};

		} // details

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename TX, typename AX, typename TR, typename AR>
details::BiMExpr<details::QRD<Tr>, Matrix<TX,AX>, Matrix<TR,AR> > qrd(const Matrix<TX,AX> &x, Matrix<TR,AR> &r, const Tr&)
{
	return details::make_Mexpr< details::QRD<Tr> >(x, r);
}

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TR, typename AR>
details::BiMExpr<details::QRD<details::ValueTraits>, Matrix<TX,AX>, Matrix<TR,AR> > qrd(const Matrix<TX,AX> &x, Matrix<TR,AR> &r)
{
	return details::make_Mexpr< details::QRD<details::ValueTraits> >(x, r);
}

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename TR, typename AR>
details::BiMExpr<details::QRD<Tr>, details::UnMExpr<X_OP,X_T1>, Matrix<TR,AR> > qrd(const details::UnMExpr<X_OP,X_T1> &x, Matrix<TR,AR> &r, const Tr&)
{
	return details::make_Mexpr< details::QRD<Tr> >(x, r);
}

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TR, typename AR>
details::BiMExpr<details::QRD<details::ValueTraits>, details::UnMExpr<X_OP,X_T1>, Matrix<TR,AR> > qrd(const details::UnMExpr<X_OP,X_T1> &x, Matrix<TR,AR> &r)
{
	return details::make_Mexpr< details::QRD<details::ValueTraits> >(x, r);
}

//////////////////////////////////////////////////////////////////////////
template<typename Tr, typename X_OP, typename X_T1, typename X_T2, typename TR, typename AR>
details::BiMExpr<details::QRD<Tr>, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TR,AR> > qrd(const details::BiMExpr<X_OP,X_T1,X_T2> &x, Matrix<TR,AR> &r, const Tr&)
{
	return details::make_Mexpr< details::QRD<Tr> >(x, r);
}

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TR, typename AR>
details::BiMExpr<details::QRD<details::ValueTraits>, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TR,AR> > qrd(const details::BiMExpr<X_OP,X_T1,X_T2> &x, Matrix<TR,AR> &r)
{
	return details::make_Mexpr< details::QRD<details::ValueTraits> >(x, r);
}

	} // QRD


	// XAdd
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// XAdd class
class XAdd {
public: // vector = scalar + vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename TY, typename AY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const std::vector<TY,AY> &y)
	{
		z.resize(y.size());

		typename std::vector<TY,AY>::const_iterator yi = y.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++yi)
			(*zi) = x.val() + (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

public: // vector = vector + scalar

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const Scalar<TY> &y)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename Vector<TZ,AZ>::iterator zi = z.begin();
		const typename Vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = (*xi) + y.val();

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

public: // vector = vector + vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const std::vector<TY,AY> &y)
	{
		assert(x.size()==y.size() && "invalid vector size");
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TY,AY>::const_iterator yi = y.begin();
		typename Vector<TZ,AZ>::iterator zi = z.begin();
		const typename Vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi, ++yi)
			(*zi) = (*xi) + (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}


public: // matrix = scalar + matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const Matrix<TY,AY> &y)
	{
		z.resize(y.Nrows(), y.Ncols());

		typename Matrix<TY,AY>::const_iterator yi = y.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++yi)
			(*zi) = x.val() + (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

public: // matrix = matrix + scalar

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Scalar<TY> &y)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = (*xi) + y.val();

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

public: // matrix = matrix + matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		assert(x.Nrows()==y.Nrows() && x.Ncols()==y.Ncols()
			&& "invalid matrix sizes");
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TY,AY>::const_iterator yi = y.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi, ++yi)
			(*zi) = (*xi) + (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename TY, typename AY> inline
details::BiVExpr<details::XAdd, Scalar<TX>, std::vector<TY,AY> > operator+(const Scalar<TX> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::XAdd, Scalar<TX>, details::UnVExpr<Y_OP,Y_T1> > operator+(const Scalar<TX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::XAdd, Scalar<TX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator+(const Scalar<TX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY> inline
details::BiVExpr<details::XAdd, std::vector<TX,AX>, Scalar<TY> > operator+(const std::vector<TX,AX> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY> inline
details::BiVExpr<details::XAdd, details::UnVExpr<X_OP,X_T1>, Scalar<TY> > operator+(const details::UnVExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY> inline
details::BiVExpr<details::XAdd, details::BiVExpr<X_OP,X_T1,X_T2>, Scalar<TY> > operator+(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiVExpr<details::XAdd, std::vector<TX,AX>, std::vector<TY,AY> > operator+(const std::vector<TX,AX> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::XAdd, std::vector<TX,AX>, details::UnVExpr<Y_OP,Y_T1> > operator+(const std::vector<TX,AX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::XAdd, std::vector<TX,AX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator+(const std::vector<TX,AX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiVExpr<details::XAdd, details::UnVExpr<X_OP,X_T1>, std::vector<TY,AY> > operator+(const details::UnVExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::XAdd, details::UnVExpr<X_OP,X_T1>, details::UnVExpr<Y_OP,Y_T1> > operator+(const details::UnVExpr<X_OP,X_T1> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::XAdd, details::UnVExpr<X_OP,X_T1>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator+(const details::UnVExpr<X_OP,X_T1> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiVExpr<details::XAdd, details::BiVExpr<X_OP,X_T1,X_T2>, std::vector<TY,AY> > operator+(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::XAdd, details::BiVExpr<X_OP,X_T1,X_T2>, details::UnVExpr<Y_OP,Y_T1> > operator+(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::XAdd, details::BiVExpr<X_OP,X_T1,X_T2>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator+(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::XAdd>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX> inline
Vector<TZ,AZ>& operator+=(Vector<TZ,AZ> &z, const Scalar<TX> &x)
	{ return details::XAdd::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX, typename AX> inline
Vector<TZ,AZ>& operator+=(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{ return details::XAdd::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
Vector<TZ,AZ>& operator+=(Vector<TZ,AZ> &z, const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::XAdd::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
Vector<TZ,AZ>& operator+=(Vector<TZ,AZ> &z, const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::XAdd::eval(z, z, x); }



//////////////////////////////////////////////////////////////////////////
template<typename TX, typename TY, typename AY> inline
details::BiMExpr<details::XAdd, Scalar<TX>, Matrix<TY,AY> > operator+(const Scalar<TX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::XAdd, Scalar<TX>, details::UnMExpr<Y_OP,Y_T1> > operator+(const Scalar<TX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::XAdd, Scalar<TX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator+(const Scalar<TX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY> inline
details::BiMExpr<details::XAdd, Matrix<TX,AX>, Scalar<TY> > operator+(const Matrix<TX,AX> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY> inline
details::BiMExpr<details::XAdd, details::UnMExpr<X_OP,X_T1>, Scalar<TY> > operator+(const details::UnMExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY> inline
details::BiMExpr<details::XAdd, details::BiMExpr<X_OP,X_T1,X_T2>, Scalar<TY> > operator+(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiMExpr<details::XAdd, Matrix<TX,AX>, Matrix<TY,AY> > operator+(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::XAdd, Matrix<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > operator+(const Matrix<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::XAdd, Matrix<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator+(const Matrix<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiMExpr<details::XAdd, details::UnMExpr<X_OP,X_T1>, Matrix<TY,AY> > operator+(const details::UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::XAdd, details::UnMExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > operator+(const details::UnMExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::XAdd, details::UnMExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator+(const details::UnMExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiMExpr<details::XAdd, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > operator+(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::XAdd, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > operator+(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::XAdd, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator+(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::XAdd>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX> inline
Matrix<TZ,AZ>& operator+=(Matrix<TZ,AZ> &z, const Scalar<TX> &x)
	{ return details::XAdd::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX, typename AX> inline
Matrix<TZ,AZ>& operator+=(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{ return details::XAdd::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
Matrix<TZ,AZ>& operator+=(Matrix<TZ,AZ> &z, const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::XAdd::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
Matrix<TZ,AZ>& operator+=(Matrix<TZ,AZ> &z, const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::XAdd::eval(z, z, x); }

	} // XAdd


	// XSub
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// XSub class
class XSub {
public: // vector = scalar - vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename TY, typename AY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const std::vector<TY,AY> &y)
	{
		z.resize(y.size());

		typename std::vector<TY,AY>::const_iterator yi = y.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++yi)
			(*zi) = x.val() - (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

public: // vector = vector - scalar

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const Scalar<TY> &y)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename Vector<TZ,AZ>::iterator zi = z.begin();
		const typename Vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = (*xi) - y.val();

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

public: // vector = vector - vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const std::vector<TY,AY> &y)
	{
		assert(x.size()==y.size() && "invalid vector size");
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TY,AY>::const_iterator yi = y.begin();
		typename Vector<TZ,AZ>::iterator zi = z.begin();
		const typename Vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi, ++yi)
			(*zi) = (*xi) - (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}


public: // matrix = scalar - matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const Matrix<TY,AY> &y)
	{
		z.resize(y.Nrows(), y.Ncols());

		typename Matrix<TY,AY>::const_iterator yi = y.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++yi)
			(*zi) = x.val() - (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

public: // matrix = matrix - scalar

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Scalar<TY> &y)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = (*xi) - y.val();

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

public: // matrix = matrix - matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		assert(x.Nrows()==y.Nrows() && x.Ncols()==y.Ncols()
			&& "invalid matrix sizes");
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TY,AY>::const_iterator yi = y.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi, ++yi)
			(*zi) = (*xi) - (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename TY, typename AY> inline
details::BiVExpr<details::XSub, Scalar<TX>, std::vector<TY,AY> > operator-(const Scalar<TX> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::XSub, Scalar<TX>, details::UnVExpr<Y_OP,Y_T1> > operator-(const Scalar<TX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::XSub, Scalar<TX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator-(const Scalar<TX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY> inline
details::BiVExpr<details::XSub, std::vector<TX,AX>, Scalar<TY> > operator-(const std::vector<TX,AX> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY> inline
details::BiVExpr<details::XSub, details::UnVExpr<X_OP,X_T1>, Scalar<TY> > operator-(const details::UnVExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY> inline
details::BiVExpr<details::XSub, details::BiVExpr<X_OP,X_T1,X_T2>, Scalar<TY> > operator-(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiVExpr<details::XSub, std::vector<TX,AX>, std::vector<TY,AY> > operator-(const std::vector<TX,AX> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::XSub, std::vector<TX,AX>, details::UnVExpr<Y_OP,Y_T1> > operator-(const std::vector<TX,AX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::XSub, std::vector<TX,AX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator-(const std::vector<TX,AX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiVExpr<details::XSub, details::UnVExpr<X_OP,X_T1>, std::vector<TY,AY> > operator-(const details::UnVExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::XSub, details::UnVExpr<X_OP,X_T1>, details::UnVExpr<Y_OP,Y_T1> > operator-(const details::UnVExpr<X_OP,X_T1> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::XSub, details::UnVExpr<X_OP,X_T1>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator-(const details::UnVExpr<X_OP,X_T1> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiVExpr<details::XSub, details::BiVExpr<X_OP,X_T1,X_T2>, std::vector<TY,AY> > operator-(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::XSub, details::BiVExpr<X_OP,X_T1,X_T2>, details::UnVExpr<Y_OP,Y_T1> > operator-(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::XSub, details::BiVExpr<X_OP,X_T1,X_T2>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator-(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::XSub>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX> inline
Vector<TZ,AZ>& operator-=(Vector<TZ,AZ> &z, const Scalar<TX> &x)
	{ return details::XSub::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX, typename AX> inline
Vector<TZ,AZ>& operator-=(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
	{ return details::XSub::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
Vector<TZ,AZ>& operator-=(Vector<TZ,AZ> &z, const details::UnVExpr<X_OP,X_T1> &x)
	{ return details::XSub::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
Vector<TZ,AZ>& operator-=(Vector<TZ,AZ> &z, const details::BiVExpr<X_OP,X_T1,X_T2> &x)
	{ return details::XSub::eval(z, z, x); }



//////////////////////////////////////////////////////////////////////////
template<typename TX, typename TY, typename AY> inline
details::BiMExpr<details::XSub, Scalar<TX>, Matrix<TY,AY> > operator-(const Scalar<TX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::XSub, Scalar<TX>, details::UnMExpr<Y_OP,Y_T1> > operator-(const Scalar<TX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::XSub, Scalar<TX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator-(const Scalar<TX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY> inline
details::BiMExpr<details::XSub, Matrix<TX,AX>, Scalar<TY> > operator-(const Matrix<TX,AX> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY> inline
details::BiMExpr<details::XSub, details::UnMExpr<X_OP,X_T1>, Scalar<TY> > operator-(const details::UnMExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY> inline
details::BiMExpr<details::XSub, details::BiMExpr<X_OP,X_T1,X_T2>, Scalar<TY> > operator-(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiMExpr<details::XSub, Matrix<TX,AX>, Matrix<TY,AY> > operator-(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::XSub, Matrix<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > operator-(const Matrix<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::XSub, Matrix<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator-(const Matrix<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiMExpr<details::XSub, details::UnMExpr<X_OP,X_T1>, Matrix<TY,AY> > operator-(const details::UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::XSub, details::UnMExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > operator-(const details::UnMExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::XSub, details::UnMExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator-(const details::UnMExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiMExpr<details::XSub, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > operator-(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::XSub, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > operator-(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::XSub, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator-(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::XSub>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX> inline
Matrix<TZ,AZ>& operator-=(Matrix<TZ,AZ> &z, const Scalar<TX> &x)
	{ return details::XSub::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX, typename AX> inline
Matrix<TZ,AZ>& operator-=(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{ return details::XSub::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
Matrix<TZ,AZ>& operator-=(Matrix<TZ,AZ> &z, const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::XSub::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
Matrix<TZ,AZ>& operator-=(Matrix<TZ,AZ> &z, const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::XSub::eval(z, z, x); }

	} // XSub


	// XMul
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// XMul class
class XMul {
public: // vector = scalar * vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename TY, typename AY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const std::vector<TY,AY> &y)
	{
		z.resize(y.size());

		typename std::vector<TY,AY>::const_iterator yi = y.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++yi)
			(*zi) = x.val() * (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

public: // vector = vector * scalar

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const Scalar<TY> &y)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename Vector<TZ,AZ>::iterator zi = z.begin();
		const typename Vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = (*xi) * y.val();

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

public: // vector = vector * vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const std::vector<TY,AY> &y)
	{
		assert(x.size()==y.size() && "invalid vector size");
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TY,AY>::const_iterator yi = y.begin();
		typename Vector<TZ,AZ>::iterator zi = z.begin();
		const typename Vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi, ++yi)
			(*zi) = (*xi) * (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}


public: // matrix = scalar * matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const Matrix<TY,AY> &y)
	{
		z.resize(y.Nrows(), y.Ncols());

		typename Matrix<TY,AY>::const_iterator yi = y.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++yi)
			(*zi) = x.val() * (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

public: // matrix = matrix * scalar

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Scalar<TY> &y)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = (*xi) * y.val();

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

public: // matrix = matrix * matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		assert(x.Nrows()==y.Nrows() && x.Ncols()==y.Ncols()
			&& "invalid matrix sizes");
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TY,AY>::const_iterator yi = y.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi, ++yi)
			(*zi) = (*xi) * (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename TY, typename AY> inline
details::BiVExpr<details::XMul, Scalar<TX>, std::vector<TY,AY> > operator*(const Scalar<TX> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::XMul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::XMul, Scalar<TX>, details::UnVExpr<Y_OP,Y_T1> > operator*(const Scalar<TX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::XMul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::XMul, Scalar<TX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator*(const Scalar<TX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::XMul>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY> inline
details::BiVExpr<details::XMul, std::vector<TX,AX>, Scalar<TY> > operator*(const std::vector<TX,AX> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XMul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY> inline
details::BiVExpr<details::XMul, details::UnVExpr<X_OP,X_T1>, Scalar<TY> > operator*(const details::UnVExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XMul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY> inline
details::BiVExpr<details::XMul, details::BiVExpr<X_OP,X_T1,X_T2>, Scalar<TY> > operator*(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XMul>(x, y); }


////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename TY, typename AY> inline
//details::BiVExpr<details::XMul, std::vector<TX,AX>, std::vector<TY,AY> > operator*(const std::vector<TX,AX> &x, const std::vector<TY,AY> &y)
//	{ return details::make_Vexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
//details::BiVExpr<details::XMul, std::vector<TX,AX>, details::UnVExpr<Y_OP,Y_T1> > operator*(const std::vector<TX,AX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Vexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiVExpr<details::XMul, std::vector<TX,AX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator*(const std::vector<TX,AX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Vexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename TY, typename AY> inline
//details::BiVExpr<details::XMul, details::UnVExpr<X_OP,X_T1>, std::vector<TY,AY> > operator*(const details::UnVExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
//	{ return details::make_Vexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
//details::BiVExpr<details::XMul, details::UnVExpr<X_OP,X_T1>, details::UnVExpr<Y_OP,Y_T1> > operator*(const details::UnVExpr<X_OP,X_T1> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Vexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiVExpr<details::XMul, details::UnVExpr<X_OP,X_T1>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::UnVExpr<X_OP,X_T1> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Vexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
//details::BiVExpr<details::XMul, details::BiVExpr<X_OP,X_T1,X_T2>, std::vector<TY,AY> > operator*(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
//	{ return details::make_Vexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
//details::BiVExpr<details::XMul, details::BiVExpr<X_OP,X_T1,X_T2>, details::UnVExpr<Y_OP,Y_T1> > operator*(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Vexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiVExpr<details::XMul, details::BiVExpr<X_OP,X_T1,X_T2>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Vexpr<details::XMul>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX> inline
Vector<TZ,AZ>& operator*=(Vector<TZ,AZ> &z, const Scalar<TX> &x)
	{ return details::XMul::eval(z, z, x); }

////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename TX, typename AX> inline
//Vector<TZ,AZ>& operator*=(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
//	{ return details::XMul::eval(z, z, x); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
//Vector<TZ,AZ>& operator*=(Vector<TZ,AZ> &z, const details::UnVExpr<X_OP,X_T1> &x)
//	{ return details::XMul::eval(z, z, x); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
//Vector<TZ,AZ>& operator*=(Vector<TZ,AZ> &z, const details::BiVExpr<X_OP,X_T1,X_T2> &x)
//	{ return details::XMul::eval(z, z, x); }



//////////////////////////////////////////////////////////////////////////
template<typename TX, typename TY, typename AY> inline
details::BiMExpr<details::XMul, Scalar<TX>, Matrix<TY,AY> > operator*(const Scalar<TX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::XMul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::XMul, Scalar<TX>, details::UnMExpr<Y_OP,Y_T1> > operator*(const Scalar<TX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::XMul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::XMul, Scalar<TX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const Scalar<TX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::XMul>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY> inline
details::BiMExpr<details::XMul, Matrix<TX,AX>, Scalar<TY> > operator*(const Matrix<TX,AX> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XMul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY> inline
details::BiMExpr<details::XMul, details::UnMExpr<X_OP,X_T1>, Scalar<TY> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XMul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY> inline
details::BiMExpr<details::XMul, details::BiMExpr<X_OP,X_T1,X_T2>, Scalar<TY> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XMul>(x, y); }


////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename TY, typename AY> inline
//details::BiMExpr<details::XMul, Matrix<TX,AX>, Matrix<TY,AY> > operator*(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
//	{ return details::make_Mexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
//details::BiMExpr<details::XMul, Matrix<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > operator*(const Matrix<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Mexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiMExpr<details::XMul, Matrix<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const Matrix<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Mexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename TY, typename AY> inline
//details::BiMExpr<details::XMul, details::UnMExpr<X_OP,X_T1>, Matrix<TY,AY> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
//	{ return details::make_Mexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
//details::BiMExpr<details::XMul, details::UnMExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Mexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiMExpr<details::XMul, details::UnMExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Mexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
//details::BiMExpr<details::XMul, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
//	{ return details::make_Mexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
//details::BiMExpr<details::XMul, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Mexpr<details::XMul>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiMExpr<details::XMul, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Mexpr<details::XMul>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX> inline
Matrix<TZ,AZ>& operator*=(Matrix<TZ,AZ> &z, const Scalar<TX> &x)
	{ return details::XMul::eval(z, z, x); }

////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename TX, typename AX> inline
//Matrix<TZ,AZ>& operator*=(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
//	{ return details::XMul::eval(z, z, x); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
//Matrix<TZ,AZ>& operator*=(Matrix<TZ,AZ> &z, const details::UnMExpr<X_OP,X_T1> &x)
//	{ return details::XMul::eval(z, z, x); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
//Matrix<TZ,AZ>& operator*=(Matrix<TZ,AZ> &z, const details::BiMExpr<X_OP,X_T1,X_T2> &x)
//	{ return details::XMul::eval(z, z, x); }

	} // XMul


	// XDiv
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// XDiv class
class XDiv {
public: // vector = scalar / vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename TY, typename AY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const std::vector<TY,AY> &y)
	{
		z.resize(y.size());

		typename std::vector<TY,AY>::const_iterator yi = y.begin();
		typename std::vector<TZ,AZ>::iterator zi = z.begin();
		const typename std::vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++yi)
			(*zi) = x.val() / (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Scalar<TX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

public: // vector = vector / scalar

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const Scalar<TY> &y)
	{
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename Vector<TZ,AZ>::iterator zi = z.begin();
		const typename Vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = (*xi) / y.val();

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

public: // vector = vector / vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const std::vector<TY,AY> &y)
	{
		assert(x.size()==y.size() && "invalid vector size");
		z.resize(x.size());

		typename std::vector<TX,AX>::const_iterator xi = x.begin();
		typename std::vector<TY,AY>::const_iterator yi = y.begin();
		typename Vector<TZ,AZ>::iterator zi = z.begin();
		const typename Vector<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi, ++yi)
			(*zi) = (*xi) / (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Vector<TZ,AZ>(y)); // (!) may be optimized
	}


public: // matrix = scalar / matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const Matrix<TY,AY> &y)
	{
		z.resize(y.Nrows(), y.Ncols());

		typename Matrix<TY,AY>::const_iterator yi = y.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++yi)
			(*zi) = x.val() / (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Scalar<TX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

public: // matrix = matrix / scalar

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Scalar<TY> &y)
	{
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = (*xi) / y.val();

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

public: // matrix = matrix / matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		assert(x.Nrows()==y.Nrows() && x.Ncols()==y.Ncols()
			&& "invalid matrix sizes");
		z.resize(x.Nrows(), x.Ncols());

		typename Matrix<TX,AX>::const_iterator xi = x.begin();
		typename Matrix<TY,AY>::const_iterator yi = y.begin();
		typename Matrix<TZ,AZ>::iterator zi = z.begin();
		const typename Matrix<TZ,AZ>::iterator ze = z.end();

		for (; zi != ze; ++zi, ++xi, ++yi)
			(*zi) = (*xi) / (*yi);

		return z;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}
};

		} // details


////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename TY, typename AY> inline
//details::BiVExpr<details::XDiv, Scalar<TX>, std::vector<TY,AY> > operator/(const Scalar<TX> &x, const std::vector<TY,AY> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename Y_OP, typename Y_T1> inline
//details::BiVExpr<details::XDiv, Scalar<TX>, details::UnVExpr<Y_OP,Y_T1> > operator/(const Scalar<TX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiVExpr<details::XDiv, Scalar<TX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator/(const Scalar<TX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY> inline
details::BiVExpr<details::XDiv, std::vector<TX,AX>, Scalar<TY> > operator/(const std::vector<TX,AX> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XDiv>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY> inline
details::BiVExpr<details::XDiv, details::UnVExpr<X_OP,X_T1>, Scalar<TY> > operator/(const details::UnVExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XDiv>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY> inline
details::BiVExpr<details::XDiv, details::BiVExpr<X_OP,X_T1,X_T2>, Scalar<TY> > operator/(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{ return details::make_Vexpr<details::XDiv>(x, y); }


////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename TY, typename AY> inline
//details::BiVExpr<details::XDiv, std::vector<TX,AX>, std::vector<TY,AY> > operator/(const std::vector<TX,AX> &x, const std::vector<TY,AY> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
//details::BiVExpr<details::XDiv, std::vector<TX,AX>, details::UnVExpr<Y_OP,Y_T1> > operator/(const std::vector<TX,AX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiVExpr<details::XDiv, std::vector<TX,AX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator/(const std::vector<TX,AX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename TY, typename AY> inline
//details::BiVExpr<details::XDiv, details::UnVExpr<X_OP,X_T1>, std::vector<TY,AY> > operator/(const details::UnVExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
//details::BiVExpr<details::XDiv, details::UnVExpr<X_OP,X_T1>, details::UnVExpr<Y_OP,Y_T1> > operator/(const details::UnVExpr<X_OP,X_T1> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiVExpr<details::XDiv, details::UnVExpr<X_OP,X_T1>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator/(const details::UnVExpr<X_OP,X_T1> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
//details::BiVExpr<details::XDiv, details::BiVExpr<X_OP,X_T1,X_T2>, std::vector<TY,AY> > operator/(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
//details::BiVExpr<details::XDiv, details::BiVExpr<X_OP,X_T1,X_T2>, details::UnVExpr<Y_OP,Y_T1> > operator/(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiVExpr<details::XDiv, details::BiVExpr<X_OP,X_T1,X_T2>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator/(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Vexpr<details::XDiv>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX> inline
Vector<TZ,AZ>& operator/=(Vector<TZ,AZ> &z, const Scalar<TX> &x)
	{ return details::XDiv::eval(z, z, x); }

////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename TX, typename AX> inline
//Vector<TZ,AZ>& operator/=(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x)
//	{ return details::XDiv::eval(z, z, x); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
//Vector<TZ,AZ>& operator/=(Vector<TZ,AZ> &z, const details::UnVExpr<X_OP,X_T1> &x)
//	{ return details::XDiv::eval(z, z, x); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
//Vector<TZ,AZ>& operator/=(Vector<TZ,AZ> &z, const details::BiVExpr<X_OP,X_T1,X_T2> &x)
//	{ return details::XDiv::eval(z, z, x); }



////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename TY, typename AY> inline
//details::BiMExpr<details::XDiv, Scalar<TX>, Matrix<TY,AY> > operator/(const Scalar<TX> &x, const Matrix<TY,AY> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename Y_OP, typename Y_T1> inline
//details::BiMExpr<details::XDiv, Scalar<TX>, details::UnMExpr<Y_OP,Y_T1> > operator/(const Scalar<TX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiMExpr<details::XDiv, Scalar<TX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator/(const Scalar<TX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY> inline
details::BiMExpr<details::XDiv, Matrix<TX,AX>, Scalar<TY> > operator/(const Matrix<TX,AX> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XDiv>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY> inline
details::BiMExpr<details::XDiv, details::UnMExpr<X_OP,X_T1>, Scalar<TY> > operator/(const details::UnMExpr<X_OP,X_T1> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XDiv>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY> inline
details::BiMExpr<details::XDiv, details::BiMExpr<X_OP,X_T1,X_T2>, Scalar<TY> > operator/(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Scalar<TY> &y)
	{ return details::make_Mexpr<details::XDiv>(x, y); }


////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename TY, typename AY> inline
//details::BiMExpr<details::XDiv, Matrix<TX,AX>, Matrix<TY,AY> > operator/(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
//details::BiMExpr<details::XDiv, Matrix<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > operator/(const Matrix<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiMExpr<details::XDiv, Matrix<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator/(const Matrix<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename TY, typename AY> inline
//details::BiMExpr<details::XDiv, details::UnMExpr<X_OP,X_T1>, Matrix<TY,AY> > operator/(const details::UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
//details::BiMExpr<details::XDiv, details::UnMExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > operator/(const details::UnMExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiMExpr<details::XDiv, details::UnMExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator/(const details::UnMExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
//details::BiMExpr<details::XDiv, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > operator/(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
//details::BiMExpr<details::XDiv, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > operator/(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
//details::BiMExpr<details::XDiv, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator/(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
//	{ return details::make_Mexpr<details::XDiv>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX> inline
Matrix<TZ,AZ>& operator/=(Matrix<TZ,AZ> &z, const Scalar<TX> &x)
	{ return details::XDiv::eval(z, z, x); }

////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename TX, typename AX> inline
//Matrix<TZ,AZ>& operator/=(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
//	{ return details::XDiv::eval(z, z, x); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
//Matrix<TZ,AZ>& operator/=(Matrix<TZ,AZ> &z, const details::UnMExpr<X_OP,X_T1> &x)
//	{ return details::XDiv::eval(z, z, x); }
//
////////////////////////////////////////////////////////////////////////////
//template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
//Matrix<TZ,AZ>& operator/=(Matrix<TZ,AZ> &z, const details::BiMExpr<X_OP,X_T1,X_T2> &x)
//	{ return details::XDiv::eval(z, z, x); }

	} // XDiv


	// Mul
	namespace mx
	{
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// Mul class
class Mul {
public: // matrix = matrix * matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		assert(x.Ncols()==y.Nrows() && "invalid matrix size");

		if (ref_equal(z, x))
		{
			if (ref_equal(z, y))                  // (z is x) && (z is y)
			{
				Matrix<TZ,AZ> t(z);
				return do_mulmm(z, t, t);
			}
			else                                  // (z is x)
			{
				Matrix<TX,AX> t(x);
				z.resize(x.Nrows(), y.Ncols());
				return do_mulmm(z, t, y);
			}
		}
		else
		{
			if (ref_equal(z, y))                  // (z is y)
			{
				Matrix<TY,AY> t(y);
				z.resize(x.Nrows(), y.Ncols());
				return do_mulmm(z, x, t);
			}
			else
			{
				z.resize(x.Nrows(), y.Ncols());
				return do_mulmm(z, x, y);
			}
		}
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Matrix<TZ,AZ>& eval(Matrix<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

public: // matrix = matrix * column vector

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Matrix<TX,AX> &x, const std::vector<TY, AY> &y)
	{
		assert(x.Ncols()==y.size() && "invalid vector size");

		if (ref_equal(z, y))                  // (z is y)
		{
			Vector<TY,AY> t(y);
			z.resize(x.Nrows());
			return do_mulmv(z, x, t);
		}
		else
		{
			z.resize(x.Nrows());
			return do_mulmv(z, x, y);
		}
	}


//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Matrix<TX,AX> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const Matrix<TX,AX> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Vector<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Vector<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnMExpr<X_OP,X_T1> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Vector<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const UnVExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Vector<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiMExpr<X_OP,X_T1,X_T2> &x, const BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Matrix<TZ,AZ>(x), Vector<TZ,AZ>(y));
	}

public: // matrix = row vector * matrix

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX, AX> &x, const Matrix<TY,AY> &y)
	{
		assert(x.size()==y.Nrows() && "invalid vector size");

		if (ref_equal(z, x))                  // (z is x)
		{
			Vector<TX,AX> t(x);
			z.resize(y.Ncols());
			return do_mulvm(z, t, y);
		}
		else
		{
			z.resize(y.Ncols());
			return do_mulvm(z, x, y);
		}
	}


//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, x, Matrix<TZ,AZ>(y)); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const UnVExpr<X_OP,X_T1> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), y); // (!) may be optimized
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const UnMExpr<Y_OP,Y_T1> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

//////////////////////////////////////////////////////////////////////////
	template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
	static Vector<TZ,AZ>& eval(Vector<TZ,AZ> &z, const BiVExpr<X_OP,X_T1,X_T2> &x, const BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{
		return eval(z, Vector<TZ,AZ>(x), Matrix<TZ,AZ>(y));
	}

private:

//////////////////////////////////////////////////////////////////////////
// multiplication (matrix * matrix)
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Matrix<TZ,AZ>& do_mulmm(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		typedef typename Matrix<TZ,AZ>::value_type value_type;
		typedef typename Matrix<TZ,AZ>::size_type size_type;

		for (size_type i = 0; i < z.Nrows(); ++i)
			for (size_type j = 0; j < z.Ncols(); ++j)
		{
			typename Matrix<TX,AX>::row_const_iterator xi = x.row_begin(i);
			const typename Matrix<TX,AX>::row_const_iterator xe = x.row_end(i);
			typename Matrix<TY,AY>::col_const_iterator yi = y.col_begin(j);

			value_type zz = value_type();
			for (; xi != xe; ++xi, ++yi)
				zz += (*xi) * (*yi);

			z.at(i, j) = zz;
		}

		return z;
	}

//////////////////////////////////////////////////////////////////////////
// multiplication (matrix * column vector)
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Vector<TZ,AZ>& do_mulmv(Vector<TZ,AZ> &z, const Matrix<TX,AX> &x, const std::vector<TY,AY> &y)
	{
		typedef typename Matrix<TZ,AZ>::value_type value_type;
		typedef typename Matrix<TZ,AZ>::size_type size_type;

		for (size_type i = 0; i < x.Nrows(); ++i)
		{
			typename Matrix<TX,AX>::row_const_iterator xi = x.row_begin(i);

			value_type zz = value_type();
			for (size_type j = 0; j < x.Ncols(); ++j, ++xi)
				zz += (*xi) * y[j];

			z[i] = zz;
		}

		return z;
	}

//////////////////////////////////////////////////////////////////////////
// multiplication (row vector * matrix)
	template<typename TZ, typename AZ, typename TX, typename AX, typename TY, typename AY>
	static Vector<TZ,AZ>& do_mulvm(Vector<TZ,AZ> &z, const std::vector<TX,AX> &x, const Matrix<TY,AY> &y)
	{
		typedef typename Matrix<TZ,AZ>::value_type value_type;
		typedef typename Matrix<TZ,AZ>::size_type size_type;

		for (size_type j = 0; j < y.Ncols(); ++j)
		{
			typename Matrix<TY,AY>::col_const_iterator yi = y.col_begin(j);

			value_type zz = value_type();
			for (size_type i = 0; i < y.Nrows(); ++i, ++yi)
				zz += x[i] * (*yi);

			z[j] = zz;
		}

		return z;
	}
};

		} // details


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiMExpr<details::Mul, Matrix<TX,AX>, Matrix<TY,AY> > operator*(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::Mul, Matrix<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > operator*(const Matrix<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::Mul, Matrix<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const Matrix<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiMExpr<details::Mul, details::UnMExpr<X_OP,X_T1>, Matrix<TY,AY> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::Mul, details::UnMExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::Mul, details::UnMExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiMExpr<details::Mul, details::BiMExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{ return details::make_Mexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiMExpr<details::Mul, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Mexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiMExpr<details::Mul, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Mexpr<details::Mul>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiVExpr<details::Mul, Matrix<TX,AX>, std::vector<TY,AY> > operator*(const Matrix<TX,AX> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Mul, Matrix<TX,AX>, details::UnVExpr<Y_OP,Y_T1> > operator*(const Matrix<TX,AX> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Mul, Matrix<TX,AX>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator*(const Matrix<TX,AX> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiVExpr<details::Mul, details::UnMExpr<X_OP,X_T1>, std::vector<TY,AY> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Mul, details::UnMExpr<X_OP,X_T1>, details::UnVExpr<Y_OP,Y_T1> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Mul, details::UnMExpr<X_OP,X_T1>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::UnMExpr<X_OP,X_T1> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiVExpr<details::Mul, details::BiMExpr<X_OP,X_T1,X_T2>, std::vector<TY,AY> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const std::vector<TY,AY> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Mul, details::BiMExpr<X_OP,X_T1,X_T2>, details::UnVExpr<Y_OP,Y_T1> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::UnVExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Mul, details::BiMExpr<X_OP,X_T1,X_T2>, details::BiVExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::BiMExpr<X_OP,X_T1,X_T2> &x, const details::BiVExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }



//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename TY, typename AY> inline
details::BiVExpr<details::Mul, std::vector<TX,AX>, Matrix<TY,AY> > operator*(const std::vector<TX,AX> &x, const Matrix<TY,AY> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Mul, std::vector<TX,AX>, details::UnMExpr<Y_OP,Y_T1> > operator*(const std::vector<TX,AX> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename TX, typename AX, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Mul, std::vector<TX,AX>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const std::vector<TX,AX> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename TY, typename AY> inline
details::BiVExpr<details::Mul, details::UnVExpr<X_OP,X_T1>, Matrix<TY,AY> > operator*(const details::UnVExpr<X_OP,X_T1> &x, const Matrix<TY,AY> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Mul, details::UnVExpr<X_OP,X_T1>, details::UnMExpr<Y_OP,Y_T1> > operator*(const details::UnVExpr<X_OP,X_T1> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Mul, details::UnVExpr<X_OP,X_T1>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::UnVExpr<X_OP,X_T1> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename TY, typename AY> inline
details::BiVExpr<details::Mul, details::BiVExpr<X_OP,X_T1,X_T2>, Matrix<TY,AY> > operator*(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const Matrix<TY,AY> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1> inline
details::BiVExpr<details::Mul, details::BiVExpr<X_OP,X_T1,X_T2>, details::UnMExpr<Y_OP,Y_T1> > operator*(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::UnMExpr<Y_OP,Y_T1> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }

//////////////////////////////////////////////////////////////////////////
template<typename X_OP, typename X_T1, typename X_T2, typename Y_OP, typename Y_T1, typename Y_T2> inline
details::BiVExpr<details::Mul, details::BiVExpr<X_OP,X_T1,X_T2>, details::BiMExpr<Y_OP,Y_T1,Y_T2> > operator*(const details::BiVExpr<X_OP,X_T1,X_T2> &x, const details::BiMExpr<Y_OP,Y_T1,Y_T2> &y)
	{ return details::make_Vexpr<details::Mul>(x, y); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX, typename AX> inline
Matrix<TZ,AZ>& operator*=(Matrix<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{ return details::Mul::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
Matrix<TZ,AZ>& operator*=(Matrix<TZ,AZ> &z, const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::Mul::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
Matrix<TZ,AZ>& operator*=(Matrix<TZ,AZ> &z, const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::Mul::eval(z, z, x); }


//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename TX, typename AX> inline
Vector<TZ,AZ>& operator*=(Vector<TZ,AZ> &z, const Matrix<TX,AX> &x)
	{ return details::Mul::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1> inline
Vector<TZ,AZ>& operator*=(Vector<TZ,AZ> &z, const details::UnMExpr<X_OP,X_T1> &x)
	{ return details::Mul::eval(z, z, x); }

//////////////////////////////////////////////////////////////////////////
template<typename TZ, typename AZ, typename X_OP, typename X_T1, typename X_T2> inline
Vector<TZ,AZ>& operator*=(Vector<TZ,AZ> &z, const details::BiMExpr<X_OP,X_T1,X_T2> &x)
	{ return details::Mul::eval(z, z, x); }

	} // Mul

} // omni namespace

#endif // __OMNI_MATMATH_HPP_
