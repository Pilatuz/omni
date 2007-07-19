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
	@brief Матричная арифметика.

@author Сергей Поличной
*/
#ifndef __OMNI_MXMATH_H_
#define __OMNI_MXMATH_H_

#include <omni/matrix.h>

#include <stdexcept>
#include <complex>
#include <vector>

#include <math.h>

// MX -> matrix
// SR -> scalar
// VR -> std::vector
// UN -> UnaryExpr
// BI -> BinaryExpr

// template function header
#define __OMNI_UNOP_MX_TEMPLATE typename TX
#define __OMNI_UNOP_SR_TEMPLATE typename TX
#define __OMNI_UNOP_VR_TEMPLATE typename TX, typename TX_A
#define __OMNI_UNOP_UN_TEMPLATE typename X_OP, typename X_TX
#define __OMNI_UNOP_BI_TEMPLATE typename X_OP, typename X_TX, typename X_TY
#define __OMNI_BIOP_MX_MX_TEMPLATE typename TX, typename TY
#define __OMNI_BIOP_MX_SR_TEMPLATE typename TX, typename TY
#define __OMNI_BIOP_MX_VR_TEMPLATE typename TX, typename TY, typename TY_A
#define __OMNI_BIOP_MX_UN_TEMPLATE typename TX, typename Y_OP, typename Y_TX
#define __OMNI_BIOP_MX_BI_TEMPLATE typename TX, typename Y_OP, typename Y_TX, typename Y_TY
#define __OMNI_BIOP_SR_MX_TEMPLATE typename TX, typename TY
#define __OMNI_BIOP_SR_UN_TEMPLATE typename TX, typename Y_OP, typename Y_TX
#define __OMNI_BIOP_SR_BI_TEMPLATE typename TX, typename Y_OP, typename Y_TX, typename Y_TY
#define __OMNI_BIOP_VR_MX_TEMPLATE typename TX, typename TX_A, typename TY
#define __OMNI_BIOP_VR_UN_TEMPLATE typename TX, typename TX_A, typename Y_OP, typename Y_TX
#define __OMNI_BIOP_VR_BI_TEMPLATE typename TX, typename TX_A, typename Y_OP, typename Y_TX, typename Y_TY
#define __OMNI_BIOP_UN_MX_TEMPLATE typename X_OP, typename X_TX, typename TY
#define __OMNI_BIOP_UN_SR_TEMPLATE typename X_OP, typename X_TX, typename TY
#define __OMNI_BIOP_UN_VR_TEMPLATE typename X_OP, typename X_TX, typename TY, typename TY_A
#define __OMNI_BIOP_UN_UN_TEMPLATE typename X_OP, typename X_TX, typename Y_OP, typename Y_TX
#define __OMNI_BIOP_UN_BI_TEMPLATE typename X_OP, typename X_TX, typename Y_OP, typename Y_TX, typename Y_TY
#define __OMNI_BIOP_BI_MX_TEMPLATE typename X_OP, typename X_TX, typename X_TY, typename TY
#define __OMNI_BIOP_BI_SR_TEMPLATE typename X_OP, typename X_TX, typename X_TY, typename TY
#define __OMNI_BIOP_BI_VR_TEMPLATE typename X_OP, typename X_TX, typename X_TY, typename TY, typename TY_A
#define __OMNI_BIOP_BI_UN_TEMPLATE typename X_OP, typename X_TX, typename X_TY, typename Y_OP, typename Y_TX
#define __OMNI_BIOP_BI_BI_TEMPLATE typename X_OP, typename X_TX, typename X_TY, typename Y_OP, typename Y_TX, typename Y_TY

// function result type
#define __OMNI_UNOP_MX_RESULT(op) details::UnaryExpr< op, matrix<TX> >
#define __OMNI_UNOP_UN_RESULT(op) details::UnaryExpr< op, details::UnaryExpr<X_OP, X_TX> >
#define __OMNI_UNOP_BI_RESULT(op) details::UnaryExpr< op, details::BinaryExpr<X_OP, X_TX, X_TY> >
#define __OMNI_BIOP_MX_MX_RESULT(op) details::BinaryExpr< op, matrix<TX>, matrix<TY> >
#define __OMNI_BIOP_MX_SR_RESULT(op) details::BinaryExpr< op, matrix<TX>, scalar<TY> >
#define __OMNI_BIOP_MX_VR_RESULT(op) details::BinaryExpr< op, matrix<TX>, std::vector<TY, TY_A> >
#define __OMNI_BIOP_MX_UN_RESULT(op) details::BinaryExpr< op, matrix<TX>, details::UnaryExpr<Y_OP, Y_TX> >
#define __OMNI_BIOP_MX_BI_RESULT(op) details::BinaryExpr< op, matrix<TX>, details::BinaryExpr<Y_OP, Y_TX, Y_TY> >
#define __OMNI_BIOP_SR_MX_RESULT(op) details::BinaryExpr< op, scalar<TX>, matrix<TY> >
#define __OMNI_BIOP_SR_UN_RESULT(op) details::BinaryExpr< op, scalar<TX>, details::UnaryExpr<Y_OP, Y_TX> >
#define __OMNI_BIOP_SR_BI_RESULT(op) details::BinaryExpr< op, scalar<TX>, details::BinaryExpr<Y_OP, Y_TX, Y_TY> >
#define __OMNI_BIOP_VR_MX_RESULT(op) details::BinaryExpr< op, std::vector<TX, TX_A>, matrix<TY> >
#define __OMNI_BIOP_VR_UN_RESULT(op) details::BinaryExpr< op, std::vector<TX, TX_A>, details::UnaryExpr<Y_OP, Y_TX> >
#define __OMNI_BIOP_VR_BI_RESULT(op) details::BinaryExpr< op, std::vector<TX, TX_A>, details::BinaryExpr<Y_OP, Y_TX, Y_TY> >
#define __OMNI_BIOP_UN_MX_RESULT(op) details::BinaryExpr< op, details::UnaryExpr<X_OP, X_TX>, matrix<TY> >
#define __OMNI_BIOP_UN_SR_RESULT(op) details::BinaryExpr< op, details::UnaryExpr<X_OP, X_TX>, scalar<TY> >
#define __OMNI_BIOP_UN_VR_RESULT(op) details::BinaryExpr< op, details::UnaryExpr<X_OP, X_TX>, std::vector<TY, TY_A> >
#define __OMNI_BIOP_UN_UN_RESULT(op) details::BinaryExpr< op, details::UnaryExpr<X_OP, X_TX>, details::UnaryExpr<Y_OP, Y_TX> >
#define __OMNI_BIOP_UN_BI_RESULT(op) details::BinaryExpr< op, details::UnaryExpr<X_OP, X_TX>, details::BinaryExpr<Y_OP, Y_TX, Y_TY> >
#define __OMNI_BIOP_BI_MX_RESULT(op) details::BinaryExpr< op, details::BinaryExpr<X_OP, X_TX, X_TY>, matrix<TY> >
#define __OMNI_BIOP_BI_SR_RESULT(op) details::BinaryExpr< op, details::BinaryExpr<X_OP, X_TX, X_TY>, scalar<TY> >
#define __OMNI_BIOP_BI_VR_RESULT(op) details::BinaryExpr< op, details::BinaryExpr<X_OP, X_TX, X_TY>, std::vector<TY, TY_A> >
#define __OMNI_BIOP_BI_UN_RESULT(op) details::BinaryExpr< op, details::BinaryExpr<X_OP, X_TX, X_TY>, details::UnaryExpr<Y_OP, Y_TX> >
#define __OMNI_BIOP_BI_BI_RESULT(op) details::BinaryExpr< op, details::BinaryExpr<X_OP, X_TX, X_TY>, details::BinaryExpr<Y_OP, Y_TX, Y_TY> >

// function arguments list
#define __OMNI_UNOP_MX_ARGUMENTS const matrix<TX> &x
#define __OMNI_UNOP_SR_ARGUMENTS const scalar<TX> &x
#define __OMNI_UNOP_VR_ARGUMENTS const std::vector<TX, TX_A> &x
#define __OMNI_UNOP_UN_ARGUMENTS const details::UnaryExpr<X_OP, X_TX> &x
#define __OMNI_UNOP_BI_ARGUMENTS const details::BinaryExpr<X_OP, X_TX, X_TY> &x
#define __OMNI_BIOP_MX_MX_ARGUMENTS const matrix<TX> &x, const matrix<TY> &y
#define __OMNI_BIOP_MX_SR_ARGUMENTS const matrix<TX> &x, const scalar<TY> &y
#define __OMNI_BIOP_MX_VR_ARGUMENTS const matrix<TX> &x, const std::vector<TY, TY_A> &y
#define __OMNI_BIOP_MX_UN_ARGUMENTS const matrix<TX> &x, const details::UnaryExpr<Y_OP, Y_TX> &y
#define __OMNI_BIOP_MX_BI_ARGUMENTS const matrix<TX> &x, const details::BinaryExpr<Y_OP, Y_TX, Y_TY> &y
#define __OMNI_BIOP_SR_MX_ARGUMENTS const scalar<TX> &x, const matrix<TY> &y
#define __OMNI_BIOP_SR_UN_ARGUMENTS const scalar<TX> &x, const details::UnaryExpr<Y_OP, Y_TX> &y
#define __OMNI_BIOP_SR_BI_ARGUMENTS const scalar<TX> &x, const details::BinaryExpr<Y_OP, Y_TX, Y_TY> &y
#define __OMNI_BIOP_VR_MX_ARGUMENTS const std::vector<TX, TX_A> &x, const matrix<TY> &y
#define __OMNI_BIOP_VR_UN_ARGUMENTS const std::vector<TX, TX_A> &x, const details::UnaryExpr<Y_OP, Y_TX> &y
#define __OMNI_BIOP_VR_BI_ARGUMENTS const std::vector<TX, TX_A> &x, const details::BinaryExpr<Y_OP, Y_TX, Y_TY> &y
#define __OMNI_BIOP_UN_MX_ARGUMENTS const details::UnaryExpr<X_OP, X_TX> &x, const matrix<TY> &y
#define __OMNI_BIOP_UN_SR_ARGUMENTS const details::UnaryExpr<X_OP, X_TX> &x, const scalar<TY> &y
#define __OMNI_BIOP_UN_VR_ARGUMENTS const details::UnaryExpr<X_OP, X_TX> &x, const std::vector<TY, TY_A> &y
#define __OMNI_BIOP_UN_UN_ARGUMENTS const details::UnaryExpr<X_OP, X_TX> &x, const details::UnaryExpr<Y_OP, Y_TX> &y
#define __OMNI_BIOP_UN_BI_ARGUMENTS const details::UnaryExpr<X_OP, X_TX> &x, const details::BinaryExpr<Y_OP, Y_TX, Y_TY> &y
#define __OMNI_BIOP_BI_MX_ARGUMENTS const details::BinaryExpr<X_OP, X_TX, X_TY> &x, const matrix<TY> &y
#define __OMNI_BIOP_BI_SR_ARGUMENTS const details::BinaryExpr<X_OP, X_TX, X_TY> &x, const scalar<TY> &y
#define __OMNI_BIOP_BI_VR_ARGUMENTS const details::BinaryExpr<X_OP, X_TX, X_TY> &x, const std::vector<TY, TY_A> &y
#define __OMNI_BIOP_BI_UN_ARGUMENTS const details::BinaryExpr<X_OP, X_TX, X_TY> &x, const details::UnaryExpr<Y_OP, Y_TX> &y
#define __OMNI_BIOP_BI_BI_ARGUMENTS const details::BinaryExpr<X_OP, X_TX, X_TY> &x, const details::BinaryExpr<Y_OP, Y_TX, Y_TY> &y

namespace omni
{
	namespace mx
	{
		namespace details
		{
			// pre-declaration
			class ValueTraits;
		}

// Matrix is singular exception
class SingularError: public std::runtime_error {
	typedef std::runtime_error inherited;
public:
	SingularError()
		: inherited("matrix is singular")
	{}
};

// Is not positive definite matrix exception
class NotPositiveError: public std::runtime_error {
	typedef std::runtime_error inherited;
public:
	NotPositiveError()
		: inherited("Is not positive definite matrix")
	{}
};

	} // mx namespace


	// is functions
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Проверяет является ли матрица @a x квадратной.
/**
		Функция проверяет, является ли матрица @a x квадратной,
	т.е. её количество строк равно количеству столбцов.

@param x Ссылка на исходную матрицу.
@return @b true, если матрица квадратная и @b false - иначе.
*/
	template<typename T>
		bool is_square(const matrix<T> &x)
	{
		return x.N_rows() == x.N_cols();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверяет является ли матрица @a x симметричной.
/**
		Функция проверяет, является ли матрица @a x симметричной,
	т.е. равна ли матрица @a x своей транспонированной.

@param x Ссылка на исходную матрицу.
@return @b true, если матрица симметричная и @b false - иначе.
*/
	template<typename T>
		bool is_symmetric(const matrix<T> &x)
	{
		if (!is_square(x))
			return false;

		for (size_t i = 1; i < x.N_rows(); ++i)
			for (size_t j = 0; j < i; ++j)
				if (x(i, j) != x(j, i))
					return false;

		return true;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверяет является ли матрица @a x эрмитовой.
/**
		Функция проверяет, является ли матрица @a x эрмитовой,
	т.е. равна ли матрица @a x своей транспонированной и сопряжённой.

@param x Ссылка на исходную матрицу.
@return @b true, если матрица эрмитова и @b false - иначе.
*/
	template<typename Tr, typename T>
		bool is_hermitian(const matrix<T> &x, const Tr&)
	{
		if (!is_square(x))
			return false;

		for (size_t i = 0; i < x.N_rows(); ++i)
			for (size_t j = 0; j <= i; ++j) // (!) check diagonal elements
				if (x(i, j) != Tr::conj(x(j, i)))
					return false;

		return true;
	}

	template<typename T>
		bool is_hermitian(const matrix<T> &x)
	{
		return is_hermitian(x, details::ValueTraits());
	}

	} // mx namespace


	// make functions
	namespace mx
	{

// make new identity matrix
template<typename T, typename Tr>
	matrix<T> make_identity(size_t size, const Tr&) // (?) VC6 bug
{
	typedef matrix<T>::value_type value_type;
	typedef matrix<T>::size_type size_type;

	const value_type zero = value_type();
	const value_type one = Tr::one(zero);

	matrix<T> z(size, size, zero); // all zeros
	for (size_t i = 0; i < size; ++i)
		z(i, i) = one;

	return z;
}

template<typename T> inline
	matrix<T> make_identity(size_t size) // (?) VC6 bug
{
	return make_identity(size, details::ValueTraits());
}

// make identity matrix
template<typename T, typename Tr>
	matrix<T>& make_identity(matrix<T> &z, const Tr&)
{
	typedef matrix<T>::value_type value_type;
	typedef matrix<T>::size_type size_type;

	const value_type zero = value_type();
	const value_type one = Tr::one(zero);

	matrix<T>::RAW_iterator zi = z.RAW_begin();

#if OMNI_MATRIX_ROW_MAJOR
	for (size_type i = 0; i < z.N_rows(); ++i)
		for (size_type j = 0; j < z.N_cols(); ++j, ++zi)
			(*zi) = (i == j) ? one : zero;
#else
	for (size_type j = 0; j < z.N_cols(); ++j)
		for (size_type i = 0; i < z.N_rows(); ++i, ++zi)
			(*zi) = (i == j) ? one : zero;
#endif

	return z;
}
template<typename T> inline
	matrix<T>& make_identity(matrix<T> &z)
{
	return make_identity(z, details::ValueTraits());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Создаёт матрицу-строку из вектора @a x.
/**
		Возвращает матрицу-строку, элементы которой принимают
	значение соответствующих элементов вектора @a x.

@param x Исходный вектор.
@return Матрица-строка.
*/
template<typename T, typename A> inline
	matrix<T> make_row(const std::vector<T, A> &x)
{
	matrix<T> z;

	z.assign(1, x.size(),
		x.begin(), x.end());

	return z;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Создаёт матрицу-строку из последовательности.
/**
		Возвращает матрицу-строку, элементы которой принимают значения
	соответствующих элементов последовательности [first, last).

@param first Начало последовательности.
@param last Окончание последовательности.
@return Матрица-строка.
*/
template<typename T, typename In> inline
	matrix<T> make_row(In first, In last) // (?) VC6 bug
{
	matrix<T> z;

	const size_t N = std::distance(first, last);
	z.assign(1, N, first, last);

	return z;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Создаёт матрицу-столбец из вектора @a x.
/**
		Возвращает матрицу-столбец, элементы которой принимают
	значения соответствующих элементов вектора @a x.

@param x Исходный вектор.
@return Матрица-столбец.
*/
template<typename T, typename A> inline
	matrix<T> make_column(const std::vector<T, A> &x)
{
	matrix<T> z;

	z.assign(x.size(), 1,
		x.begin(), x.end());

	return z;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Создаёт матрицу-столбец из последовательности.
/**
		Возвращает матрицу-столбец, элементы которой принимают значения
	соответствующих элементов последовательности [first, last).

@param first Начало последовательности.
@param last Окончание последовательности.
@return Матрица-столбец.
*/
template<typename T, typename In> inline
	matrix<T> make_column(In first, In last) // (?) VC6 bug
{
	matrix<T> z;

	const size_t N = std::distance(first, last);
	z.assign(N, 1, first, last);

	return z;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Создаёт диагональную матрицу из вектора @a x.
/**
		Возвращает квадратную матрицу, диагональные элементы которой
	равны соответствующим элементам вектора @a x.
	Внедиагональные элементы матрицы принимают нулевые значения.

@param x Исходный вектор.
@return Диагональная матрица.
*/
template<typename T, typename A> inline
	matrix<T> make_diagonal(const std::vector<T, A>& x)
{
	const size_t N = x.size();
	matrix<T> z(N, N);

	for (size_t i = 0; i < N; ++i)
		z(i, i) = x[i];

	return z;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Создаёт диагональную матрицу из последовательности.
/**
		Возвращает квадратную матрицу, диагональные элементы которой
	равны соответствующим элементам последовательности [first, last).
	Внедиагональные элементы матрицы принимают нулевые значения.

@param first Начало последовательности
@param last Окончание последовательности
@return Диагональная матрица
*/
template<typename T, typename In> inline
	matrix<T> make_diagonal(In first, In last) // (?) VC6 bug
{
	const size_t N = std::distance(first, last);
	matrix<T> z(N, N); // zeros

	for (size_t i = 0; i < n; ++i, ++first)
		z(i, i) = *first;

	return z;
}

	} // mx namespace


	// det
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Вычисляет определитель матрицы @a x.
/**
		Функция вычисляет определитель матрицы @a x.
	Исходная матрица должна быть квадратной. Если матрица является
	сингулярной, то определитель равен нулю.
	Как и для инверсии матрицы, функция расчёта определителя не будет работать,
	если использовать целые матрицы. В этом случае следует или преобразовать
	её в вещественную. Или использовать определитель с перестановками.

@param x Исходная матрица.
@return Определитель.
*/
template<typename Tr, typename T>
	T det(matrix<T> x, const Tr&)
{
	assert(is_square(x) && "matrix must be square");

	typedef matrix<T>::value_type value_type;
	typedef matrix<T>::size_type size_type;
	typedef Tr traits_type;

	const value_type zero = value_type();
	const value_type one = traits_type::one(zero);

	value_type res = one;

	//#if OMNI_MATRIX_ROW_MAJOR
	for (size_type k = 0; k < x.N_rows(); ++k)
	{
		{ // find pivoting element in column
			value_type max_x = zero;
			size_type max_i = k;

			matrix<T>::const_col_iterator xi = x.col_begin(k) + k;
			matrix<T>::const_col_iterator xe = x.col_end(k);

			for (size_type i = k; xi != xe; ++xi, ++i)
				if (traits_type::abs(max_x) < traits_type::abs(*xi))
					{ max_x = *xi; max_i = i; }

			// rows swap
			if (max_i != k)
			{
				row_swap(x, max_i, k, k);
				res = -res;
			}
		}

		if (x(k, k) == zero)
			return zero;

		res *= x(k, k);
		row_mul(x, k, one / x(k, k), k);

		for (size_type i = k+1; i < x.N_rows(); ++i)
			row_fadd(x, i, k, -x(i, k), k);
	}
	//#else
	//for (size_type k = 0; k < x.N_cols(); ++k)
	//{
	//	{ // find pivoting element in row
	//		value_type max_x = zero;
	//		size_type max_j = k;

	//		matrix<T>::const_row_iterator xj = x.row_begin(k) + k;
	//		matrix<T>::const_row_iterator xe = x.row_end(k);

	//		for (size_type j = k; xi != xe; ++xj, ++j)
	//			if (traits_type::abs(max_x) < traits_type::abs(*xj))
	//				{ max_x = *xj; max_j = j; }

	//		// columns swap
	//		if (max_j != k)
	//		{
	//			col_swap(x, max_j, k, k);
	//			res = -res;
	//		}
	//	}

	//	if (x(k, k) == zero)
	//		return zero;

	//	res *= x(k, k);
	//	col_mul(x, k, one/x(k, k), k);

	//	for (size_type j = k+1; j < x.N_cols(); ++j)
	//		col_fadd(x, j, k, -x(k, j), k);
	//}
	//#endif

	return res;
}

template<typename T> inline
	T det(const matrix<T> &x)
{
	return det(x, details::ValueTraits());
}

	} // mx namespace

	// elementary operations
	namespace mx
	{

// row swap
template<typename T>
	matrix<T>& row_swap(matrix<T> &x, size_t row1, size_t row2, size_t start_col = 0)
{
	assert(start_col < x.N_cols() && "index out of range");
	assert(row1 < x.N_rows() && "index out of range");
	assert(row2 < x.N_rows() && "index out of range");

	matrix<T>::row_iterator yi = x.row_begin(row2) + start_col;
	matrix<T>::row_iterator xi = x.row_begin(row1) + start_col;
	matrix<T>::row_iterator xe = x.row_end(row1);

	for (; xi != xe; ++xi, ++yi)
		std::iter_swap(xi, yi);

	return x;
}

// col swap
template<typename T>
	matrix<T>& col_swap(matrix<T> &x, size_t col1, size_t col2, size_t start_row = 0)
{
	assert(start_row < x.N_rows() && "index out of range");
	assert(col1 < x.N_cols() && "index out of range");
	assert(col2 < x.N_cols() && "index out of range");

	matrix<T>::col_iterator yi = x.col_begin(col2) + start_row;
	matrix<T>::col_iterator xi = x.col_begin(col1) + start_row;
	matrix<T>::col_iterator xe = x.col_end(col1);

	for (; xi != xe; ++xi, ++yi)
		std::iter_swap(xi, yi);

	return x;
}

// row mul
template<typename T, typename U>
	matrix<T>& row_mul(matrix<T> &x, size_t row, const U &val, size_t start_col = 0)
{
	assert(start_col < x.N_cols() && "index out of range");
	assert(row < x.N_rows() && "index out of range");

	matrix<T>::row_iterator xi = x.row_begin(row) + start_col;
	matrix<T>::row_iterator xe = x.row_end(row);

	for (; xi != xe; ++xi)
		(*xi) *= val;

	return x;
}

// col mul
template<typename T, typename U>
	matrix<T>& col_mul(matrix<T> &x, size_t col, const U &val, size_t start_row = 0)
{
	assert(start_row < x.N_rows() && "index out of range");
	assert(col < x.N_cols() && "index out of range");

	matrix<T>::col_iterator xi = x.col_begin(col) + start_row;
	matrix<T>::col_iterator xe = x.col_end(col);

	for (; xi != xe; ++xi)
		(*xi) *= val;

	return x;
}

// row fadd
template<typename T, typename U>
	matrix<T>& row_fadd(matrix<T> &x,
		size_t target_row, size_t source_row,
		const U &val, size_t start_col = 0)
{
	assert(source_row < x.N_rows() && "index out of range");
	assert(target_row < x.N_rows() && "index out of range");
	assert(start_col < x.N_cols() && "index out of range");

	matrix<T>::row_iterator yi = x.row_begin(source_row) + start_col;
	matrix<T>::row_iterator xi = x.row_begin(target_row) + start_col;
	matrix<T>::row_iterator xe = x.row_end(target_row);

	for (; xi != xe; ++xi, ++yi)
		(*xi) += (*yi) * val;

	return x;
}

template<typename T, typename U>
	matrix<T>& col_fadd(matrix<T> &x,
		size_t target_col, size_t source_col,
		const U &val, size_t start_row = 0)
{
	assert(source_col < x.N_cols() && "index out of range");
	assert(target_col < x.N_cols() && "index out of range");
	assert(start_row < x.N_rows() && "index out of range");

	matrix<T>::col_iterator yi = x.col_begin(source_col) + start_row;
	matrix<T>::col_iterator xi = x.col_begin(target_col) + start_row;
	matrix<T>::col_iterator xe = x.col_end(target_col);

	for (; xi != xe; ++xi, ++yi)
		(*xi) += (*yi) * val;

	return x;
}

	} // mx namespace


	// norm function
	namespace mx
	{

// norm of matrix
template<typename Tr, typename T>
	T norm(const matrix<T> &z, const Tr&)
{
	typedef matrix<T>::value_type value_type;
	typedef Tr traits_type;

	matrix<T>::const_RAW_iterator zi = z.RAW_begin();
	matrix<T>::const_RAW_iterator ze = z.RAW_end();

	value_type sum2 = value_type();
	for (; zi != ze; ++zi)
		sum2 += traits_type::norm(*zi);

	return traits_type::sqrt(sum2); // TODO: with/without sqrt
}

// norm of matrix
template<typename T> inline
	T norm(const matrix<T> &z)
{
	return norm(z, details::ValueTraits());
}

	} // mx namespace


	namespace mx
	{
		namespace details
		{

// unary expression
template<typename OP, typename TX>
	class UnaryExpr: private omni::NonCopyable {
		typedef UnaryExpr<OP, TX> this_type;

	public:
		typedef OP operator_type;
		typedef TX argument1_type;

	public:
		UnaryExpr(const argument1_type &x)
			: m_x(x) {}
		UnaryExpr(const this_type &x)
			: m_x(x.m_x) {}

	public:
		template<typename TZ>
			matrix<TZ>& operator()(matrix<TZ> &z) const
		{
			return Evaluator<operator_type>::eval(z, m_x);
		}

	private:
		const argument1_type &m_x;
	};

// create unary expression
template<typename OP, typename TX>
	UnaryExpr<OP, TX> make_unexpr(const TX &x)
{
	return UnaryExpr<OP, TX>(x);
}


// binary expression
template<typename OP, typename TX, typename TY>
	class BinaryExpr: private omni::NonCopyable {
		typedef BinaryExpr<OP, TX, TY> this_type;

	public:
		typedef OP operator_type;
		typedef TX argument1_type;
		typedef TY argument2_type;

	public:
		BinaryExpr(const argument1_type &x, const argument2_type &y)
			: m_x(x), m_y(y) {}
		BinaryExpr(const this_type &x)
			: m_x(x.m_x), m_y(x.m_y) {}

	public:
		template<typename TZ>
			matrix<TZ>& operator()(matrix<TZ> &z) const
		{
			return Evaluator<operator_type>::eval(z, m_x, m_y);
		}

	private:
		const TX &m_x;
		const TY &m_y;
	};

// create binary matrix expression
template<typename OP, typename TX, typename TY>
	BinaryExpr<OP, TX, TY> make_biexpr(const TX &x, const TY &y)
{
	return BinaryExpr<OP, TX, TY>(x, y);
}


// Evaluator
template<typename OP>
	class Evaluator {
		typedef OP operator_type;

	public: // unary expressions
		template<typename TZ, __OMNI_UNOP_MX_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_UNOP_MX_ARGUMENTS)
				{ return operator_type::eval(z, x); }
		template<typename TZ, __OMNI_UNOP_UN_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_UNOP_UN_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x)); }
		template<typename TZ, __OMNI_UNOP_BI_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_UNOP_BI_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x)); }

	public: // binary expressions: MX
		template<typename TZ, __OMNI_BIOP_MX_MX_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_MX_MX_ARGUMENTS)
				{ return operator_type::eval(z, x, y); }
		template<typename TZ, __OMNI_BIOP_MX_SR_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_MX_SR_ARGUMENTS)
				{ return operator_type::eval(z, x, y); }
		template<typename TZ, __OMNI_BIOP_MX_VR_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_MX_VR_ARGUMENTS)
				{ return operator_type::eval(z, x, y); }
		template<typename TZ, __OMNI_BIOP_MX_UN_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_MX_UN_ARGUMENTS)
				{ return operator_type::eval(z, x, matrix<TZ>(y)); }
		template<typename TZ, __OMNI_BIOP_MX_BI_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_MX_BI_ARGUMENTS)
				{ return operator_type::eval(z, x, matrix<TZ>(y)); }

	public: // binary expressions: SR
		template<typename TZ, __OMNI_BIOP_SR_MX_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_SR_MX_ARGUMENTS)
				{ return operator_type::eval(z, x, y); }
		template<typename TZ, __OMNI_BIOP_SR_UN_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_SR_UN_ARGUMENTS)
				{ return operator_type::eval(z, x, matrix<TZ>(y)); }
		template<typename TZ, __OMNI_BIOP_SR_BI_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_SR_BI_ARGUMENTS)
				{ return operator_type::eval(z, x, matrix<TZ>(y)); }

	public: // binary expressions: VR
		template<typename TZ, __OMNI_BIOP_VR_MX_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_VR_MX_ARGUMENTS)
				{ return operator_type::eval(z, x, y); }
		template<typename TZ, __OMNI_BIOP_VR_UN_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_VR_UN_ARGUMENTS)
				{ return operator_type::eval(z, x, matrix<TZ>(y)); }
		template<typename TZ, __OMNI_BIOP_VR_BI_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_VR_BI_ARGUMENTS)
				{ return operator_type::eval(z, x, matrix<TZ>(y)); }

	public: // binary expressions: UN
		template<typename TZ, __OMNI_BIOP_UN_MX_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_UN_MX_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), y); }
		template<typename TZ, __OMNI_BIOP_UN_SR_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_UN_SR_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), y); }
		template<typename TZ, __OMNI_BIOP_UN_VR_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_UN_VR_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), y); }
		template<typename TZ, __OMNI_BIOP_UN_UN_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_UN_UN_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), matrix<TZ>(y)); }
		template<typename TZ, __OMNI_BIOP_UN_BI_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_UN_BI_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), matrix<TZ>(y)); }

	public: // binary expressions: BI
		template<typename TZ, __OMNI_BIOP_BI_MX_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_BI_MX_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), y); }
		template<typename TZ, __OMNI_BIOP_BI_SR_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_BI_SR_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), y); }
		template<typename TZ, __OMNI_BIOP_BI_VR_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_BI_VR_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), y); }
		template<typename TZ, __OMNI_BIOP_BI_UN_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_BI_UN_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), matrix<TZ>(y)); }
		template<typename TZ, __OMNI_BIOP_BI_BI_TEMPLATE> inline
			static matrix<TZ>& eval(matrix<TZ> &z, __OMNI_BIOP_BI_BI_ARGUMENTS)
				{ return operator_type::eval(z, matrix<TZ>(x), matrix<TZ>(y)); }
	};


// reference equal
template<typename X, typename Y>
	bool ref_equal(const X &x, const Y &y)
{
	return static_cast<const void*>(&x)
		== static_cast<const void*>(&y);
}


// Negate class
class Negate {
public:
	template<typename TZ, typename TX> inline
		static matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x)
	{
		z.resize(x.N_rows(), x.N_cols());

		matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
		matrix<TZ>::RAW_iterator zi = z.RAW_begin();
		matrix<TZ>::RAW_iterator ze = z.RAW_end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = -(*xi);

		return z;
	}
};


// Trans class
class Trans {
public:
	template<typename TZ, typename TX> inline
		static matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x)
	{
		if (ref_equal(z, x))
		{
			return is_square(x) ? __sq_trans(z)
				: __trans(z, matrix<TX>(x));
		}
		else
			return __trans(z, x);
	}

private:
	template<typename TZ, typename TX>
		static matrix<TZ>& __trans(matrix<TZ> &z, const matrix<TX> &x)
	{
		z.resize(x.N_cols(), x.N_rows());
		for (size_t j = 0; j < x.N_cols(); ++j)
			std::copy(x.col_begin(j), x.col_end(j),
				z.row_begin(j));
		return z;
	}

	template<typename TZ>
		static matrix<TZ>& __sq_trans(matrix<TZ> &z)
	{
		for (size_t i = 1; i < z.N_rows(); ++i)
			for (size_t j = 0; j < i; ++j)
				std::swap(z(i, j), z(j, i));
		return z;
	}
};


// Conj class
template<typename Tr>
class Conj {
	typedef Tr traits_type;

public:
	template<typename TZ, typename TX> inline static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x)
	{
		z.resize(x.N_rows(), x.N_cols());

		matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
		matrix<TZ>::RAW_iterator zi = z.RAW_begin();
		matrix<TZ>::RAW_iterator ze = z.RAW_end();

		for (; zi != ze; ++zi, ++xi)
			(*zi) = traits_type::conj(*xi);

		return z;
	}
};


// Herm class
template<typename Tr>
class Herm {
	typedef Tr traits_type;

public:
	template<typename TZ, typename TX> inline static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x)
	{
		return Conj<traits_type>::eval(z, Trans::eval(z, x));
	}
}; // class __matrix_herm


// Solve class (A*X=B)
template<typename Tr>
class Solve {
	typedef Tr traits_type;

public:
	template<typename TX, typename TA, typename TB> inline static
		matrix<TX>& eval(matrix<TX> &x, const matrix<TA> &a, const matrix<TB> &b)
	{
		assert(is_square(a) && "matrix must be square");
		assert(b.N_rows()==a.N_rows() && "invalid number of rows");

		if (details::ref_equal(x, b))
			return __solve(x, a);
		else
			return __solve(x=b, a);
	}

	template<typename TX, typename TA, typename TB> inline static
		matrix<TX>& eval(matrix<TX> &x, const matrix<TA> &a, const std::vector<TB> &b)
	{
		assert(is_square(a) && "matrix must be square");
		assert(b.size()==a.N_rows() && "invalid vector size");

		// make column matrix
		x.assign(b.size(), 1,
			b.begin(), b.end());

		return __solve(x, a);
	}

private:
	template<typename TX, typename TA> static
		matrix<TX>& __solve(matrix<TX> &x, matrix<TA> a)
	{
		typedef matrix<TX>::value_type value_type;
		typedef matrix<TX>::size_type size_type;

		const value_type zero = value_type();
		const value_type one = traits_type::one(zero);

		//#if OMNI_MATRIX_ROW_MAJOR
		// forward
		for (size_type k = 0; k < a.N_rows(); ++k)
		{
			{ // find pivoting element in column
				value_type max_a = zero;
				size_type max_i = k;

				matrix<TA>::const_col_iterator ai = a.col_begin(k) + k;
				matrix<TA>::const_col_iterator ae = a.col_end(k);

				for (size_type i = k; ai != ae; ++ai, ++i)
					if (traits_type::abs(max_a) < traits_type::abs(*ai))
						{ max_a = *ai; max_i = i; }

				// rows swap
				if (max_i != k)
				{
					row_swap(a, max_i, k, k);
					row_swap(x, max_i, k);
				}
			}

			if (a(k, k) == zero)
				throw SingularError();

			value_type ff = one / a(k, k);
			row_mul(a, k, ff, k);
			row_mul(x, k, ff);

			for (size_type i = k+1; i < a.N_rows(); ++i)
			{
				value_type dd = -a(i, k);
				row_fadd(a, i, k, dd, k);
				row_fadd(x, i, k, dd);
			}
		}

		// back trace
		for (ptrdiff_t j = a.N_cols()-1; 0 <= j; --j)
			for (ptrdiff_t i = j-1; 0 <= i; --i)
		{
			value_type dd = -a(i, j);
			row_fadd(a, i, j, dd, a.N_cols()-i-1);
			row_fadd(x, i, j, dd);
		}
		//#else
		//// forward
		//for (size_type k = 0; k < a.N_cols(); ++k)
		//{
		//	{ // find pivoting element in row
		//		value_type max_a = zero;
		//		size_type max_j = k;

		//		matrix<TA>::const_col_iterator aj = a.col_begin(k) + k;
		//		matrix<TA>::const_col_iterator ae = a.col_end(k);

		//		for (size_type j = k; aj != ae; ++aj, ++j)
		//			if (traits_type::abs(max_a) < traits_type::abs(*aj))
		//				{ max_a = *ai; max_j = j; }

		//		// columns swap
		//		if (max_j != k)
		//		{
		//			col_swap(a, max_j, k, k);
		//			col_swap(x, max_j, k);
		//		}
		//	}

		//	if (a(k, k) == zero)
		//		throw SingularError();

		//	value_type ff = one/a(k, k);
		//	col_mul(a, k, ff, k);
		//	col_mul(x, k, ff);

		//	for (size_type j = k+1; j < a.N_cols(); ++j)
		//	{
		//		value_type dd = -a(k, j);
		//		col_fadd(a, j, k, dd, k);
		//		col_fadd(x, j, k, dd);
		//	}
		//}

		//// back trace ???
		//for (int j = a.N_cols()-1; 0 <= j; --j)
		//	for (int i = j-1; 0 <= i; --i)
		//{
		//	value_type dd = -a(i, j);
		//	row_fadd(a, i, j, dd, a.N_cols()-i-1);
		//	row_fadd(x, i, j, dd);
		//}
		//#endif

		return x;
	}
};


// Inv class
template<typename Tr>
class Inv {
	typedef Tr traits_type;

public:
	template<typename TZ, typename TX> inline static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x)
	{
		assert(is_square(x) && "matrix must be square");

		z.resize(x.N_rows(), x.N_cols());
		make_identity(z, traits_type());
		return Solve<traits_type>::eval(z, x, z);
	}
};


// Cholesky (lower)
template<typename Tr>
class Chol_L {
	typedef Tr traits_type;

public:
	template<typename TZ, typename TX> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x)
	{
		typedef matrix<TZ>::value_type value_type;
		typedef matrix<TZ>::size_type size_type;
		const value_type zero = value_type();

		assert(is_square(x));

		z.resize(x.N_rows(), x.N_cols());
		const size_type N = x.N_rows();

		for (size_type k = 0; k < N; ++k)
		{
			value_type d = x(k, k);
			for (size_type j = 0; j < k; ++j)
				d -= z(k, j) * traits_type::conj(z(k, j));

			if (traits_type::real(d) <= traits_type::real(zero))
				throw NotPositiveError();

			z(k, k) = traits_type::sqrt(traits_type::real(d));

			for (size_type i = k+1; i < N; ++i)
			{
				value_type s = x(i, k);
				for (size_type j = 0; j < k; ++j)
					s -= z(i, j) * traits_type::conj(z(k, j));
				z(i, k) = s / traits_type::real(z(k, k));
			}

			for (size_type j = k+1; j < N; ++j)
				z(k, j) = zero;
		}
	}
};


// Cholesky (upper)
template<typename Tr>
class Chol_U {
	typedef Tr traits_type;

public:
	template<typename TZ, typename TX> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x)
	{
		typedef matrix<TZ>::value_type value_type;
		typedef matrix<TZ>::size_type size_type;
		const value_type zero = value_type();

		assert(is_square(x));

		z.resize(x.N_rows(), x.N_cols());
		const size_type N = x.N_rows();

		for (size_type k = 0; k < N; ++k)
		{
			value_type d = x(k, k);
			for (size_type i = 0; i < k; ++i)
				d -= z(i, k) * traits_type::conj(z(i, k));

			if (traits_type::real(d) <= traits_type::real(zero))
				throw NotPositiveError();

			z(k, k) = traits_type::sqrt(traits_type::real(d));

			for (size_type j = k+1; j < N; ++j)
			{
				value_type s = x(k, j);
				for (size_type i = 0; i < k; ++i)
					s -= z(i, j) * traits_type::conj(z(i, k));
				z(k, j) = s / traits_type::real(z(k, k));
			}

			for (size_type i = k+1; i < N; ++i)
				z(i, k) = zero;
		}
	}
};


// XAdd class
class XAdd {
public:
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const matrix<TY> &y)
		{
			assert(x.N_rows()==y.N_rows() && x.N_cols()==y.N_cols()
				&& "invalid matrix size");
			z.resize(x.N_rows(), x.N_cols());

			matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
			matrix<TY>::const_RAW_iterator yi = y.RAW_begin();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++xi, ++yi)
				(*zi) = (*xi) + (*yi);

			return z;
		}
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const scalar<TY> &y)
		{
			z.resize(x.N_rows(), x.N_cols());

			matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
			scalar<TY>::const_reference    yi = y.val();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++xi)
				(*zi) = (*xi) + yi;

			return z;
		}
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const scalar<TX> &x, const matrix<TY> &y)
		{
			z.resize(y.N_rows(), y.N_cols());

			scalar<TX>::const_reference    xi = x.val();
			matrix<TY>::const_RAW_iterator yi = y.RAW_begin();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++yi)
				(*zi) = xi + (*yi);

			return z;
		}
};


// XSub class
class XSub {
public:
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const matrix<TY> &y)
		{
			assert(x.N_rows()==y.N_rows() && x.N_cols()==y.N_cols()
				&& "invalid matrix size");
			z.resize(x.N_rows(), x.N_cols());

			matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
			matrix<TY>::const_RAW_iterator yi = y.RAW_begin();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++xi, ++yi)
				(*zi) = (*xi) - (*yi);

			return z;
		}
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const scalar<TY> &y)
		{
			z.resize(x.N_rows(), x.N_cols());

			matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
			scalar<TY>::const_reference    yi = y.val();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++xi)
				(*zi) = (*xi) - yi;

			return z;
		}
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const scalar<TX> &x, const matrix<TY> &y)
		{
			z.resize(y.N_rows(), y.N_cols());

			scalar<TX>::const_reference    xi = x.val();
			matrix<TY>::const_RAW_iterator yi = y.RAW_begin();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++yi)
				(*zi) = xi - (*yi);

			return z;
		}
};


// XMul class
class XMul {
public:
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const matrix<TY> &y)
		{
			assert(x.N_rows()==y.N_rows() && x.N_cols()==y.N_cols()
				&& "invalid matrix size");
			z.resize(x.N_rows(), x.N_cols());

			matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
			matrix<TY>::const_RAW_iterator yi = y.RAW_begin();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++xi, ++yi)
				(*zi) = (*xi) * (*yi);

			return z;
		}
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const scalar<TY> &y)
		{
			z.resize(x.N_rows(), x.N_cols());

			matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
			scalar<TY>::const_reference    yi = y.val();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++xi)
				(*zi) = (*xi) * yi;

			return z;
		}
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const scalar<TX> &x, const matrix<TY> &y)
		{
			z.resize(y.N_rows(), y.N_cols());

			scalar<TX>::const_reference    xi = x.val();
			matrix<TY>::const_RAW_iterator yi = y.RAW_begin();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++yi)
				(*zi) = xi * (*yi);

			return z;
		}
};


// XDiv class
class XDiv {
public:
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const matrix<TY> &y)
		{
			assert(x.N_rows()==y.N_rows() && x.N_cols()==y.N_cols()
				&& "invalid matrix size");
			z.resize(x.N_rows(), x.N_cols());

			matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
			matrix<TY>::const_RAW_iterator yi = y.RAW_begin();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++xi, ++yi)
				(*zi) = (*xi) / (*yi);

			return z;
		}
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const scalar<TY> &y)
		{
			z.resize(x.N_rows(), x.N_cols());

			matrix<TX>::const_RAW_iterator xi = x.RAW_begin();
			scalar<TY>::const_reference    yi = y.val();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++xi)
				(*zi) = (*xi) / yi;

			return z;
		}
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& eval(matrix<TZ> &z, const scalar<TX> &x, const matrix<TY> &y)
		{
			z.resize(y.N_rows(), y.N_cols());

			scalar<TX>::const_reference    xi = x.val();
			matrix<TY>::const_RAW_iterator yi = y.RAW_begin();
			matrix<TZ>::RAW_iterator zi = z.RAW_begin();
			matrix<TZ>::RAW_iterator ze = z.RAW_end();

			for (; zi != ze; ++zi, ++yi)
				(*zi) = xi / (*yi);

			return z;
		}
};


// Mul class
class Mul {
public:
	// matrix = matrix * matrix
	template<typename TZ, typename TX, typename TY> inline static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const matrix<TY> &y)
		{
			assert(x.N_cols()==y.N_rows() && "invalid matrix size");

			if (ref_equal(z, x))
			{
				if (ref_equal(z, y))                  // (z is x) && (z is y)
				{
					matrix<TZ> t(z);
					return __mul(z, t, t);
				}
				else                                  // (z is x)
				{
					matrix<TX> t(x);
					z.resize(x.N_rows(), y.N_cols());
					return __mul(z, t, y);
				}
			}
			else
			{
				if (ref_equal(z, y))                  // (z is y)
				{
					matrix<TY> t(y);
					z.resize(x.N_rows(), y.N_cols());
					return __mul(z, x, t);
				}
				else
				{
					z.resize(x.N_rows(), y.N_cols());
					return __mul(z, x, y);
				}
			}
		}

	// matrix = matrix * vector
	template<typename TZ, typename TX, typename TY, typename TY_A> inline static
		matrix<TZ>& eval(matrix<TZ> &z, const matrix<TX> &x, const std::vector<TY, TY_A> &y)
		{
			assert(x.N_cols()==y.size() && "invalid vector size");

			if (ref_equal(z, x))                  // (z is x)
			{
				matrix<TX> t(x);
				z.resize(x.N_rows(), 1);
				return __mulmv(z, t, y);
			}
			else
			{
				z.resize(x.N_rows(), 1);
				return __mulmv(z, x, y);
			}
		}

	// matrix = vector * matrix
	template<typename TZ, typename TX, typename TX_A, typename TY> inline static
		matrix<TZ>& eval(matrix<TZ> &z, const std::vector<TX, TX_A> &x, const matrix<TY> &y)
		{
			assert(x.size()==y.N_rows() && "invalid vector size");

			if (ref_equal(z, y))                  // (z is y)
			{
				matrix<TY> t(y);
				z.resize(1, y.N_cols());
				return __mulvm(z, x, t);
			}
			else
			{
				z.resize(1, y.N_cols());
				return __mulvm(z, x, y);
			}
		}

private:

	// multiplication (matrix * matrix)
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& __mul(matrix<TZ> &z, const matrix<TX> &x, const matrix<TY> &y)
		{
			typedef matrix<TZ>::value_type value_type;
			typedef matrix<TZ>::size_type size_type;

			for (size_type i = 0; i < z.N_rows(); ++i)
				for (size_type j = 0; j < z.N_cols(); ++j)
				{
					matrix<TX>::const_row_iterator xi = x.row_begin(i);
					matrix<TX>::const_row_iterator xe = x.row_end(i);
					matrix<TY>::const_col_iterator yi = y.col_begin(j);

					value_type zz = value_type();
					for (; xi != xe; ++xi, ++yi)
						zz += (*xi) * (*yi);

					z(i, j) = zz;
				}

			return z;
		}

	// multiplication (matrix * vector) (vector: [])
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& __mulmv(matrix<TZ> &z, const matrix<TX> &x, const TY &y)
		{
			typedef matrix<TZ>::value_type value_type;
			typedef matrix<TZ>::size_type size_type;

			for (size_type i = 0; i < x.N_rows(); ++i)
			{
				matrix<TX>::const_row_iterator xi = x.row_begin(i);

				value_type zz = value_type();
				for (size_type j = 0; j < x.N_cols(); ++j, ++xi)
					zz += (*xi) * y[j];

				z(i, 0) = zz;
			}

			return z;
		}

	// multiplication (vector * matrix) (vector: [])
	template<typename TZ, typename TX, typename TY> static
		matrix<TZ>& __mulvm(matrix<TZ> &z, const TX &x, const matrix<TY> &y)
		{
			typedef matrix<TZ>::value_type value_type;
			typedef matrix<TZ>::size_type size_type;

			for (size_type j = 0; j < y.N_cols(); ++j)
			{
				matrix<TY>::const_col_iterator yi = y.col_begin(j);

				value_type zz = value_type();
				for (size_type i = 0; i < y.N_rows(); ++i, ++yi)
					zz += x[i] * (*yi);

				z(0, j) = zz;
			}

			return z;
		}
};

// ValueTraits type
class ValueTraits {
public: // norm
	template<typename T> inline static
		T norm(const std::complex<T> &x)
			{ return std::norm(x); }
	template<typename T> inline static
		scalar<T> norm(const scalar<T> &x)
			{ return scalar<T>(norm(x.val())); }
	inline static double norm(double x) { return x*x; }
	inline static float norm(float x) { return x*x; }
	inline static long norm(long x) { return x*x; }
	inline static int norm(int x) { return x*x; }

public: // abs
	template<typename T> inline static
		T abs(const std::complex<T> &x)
			{ return std::abs(x); }
	template<typename T> inline static
		scalar<T> abs(const scalar<T> &x)
			{ return scalar<T>(abs(x.val())); }
	inline static double abs(double x) { return ::fabs(x); }
	inline static float abs(float x) { return ::fabsf(x); }
	inline static long abs(long x) { return ::labs(x); }
	inline static int abs(int x) { return ::abs(x); }

public: // sqrt
	template<typename T> inline static
		std::complex<T> sqrt(const std::complex<T> &x)
			{ return std::sqrt(x); }
	template<typename T> inline static
		scalar<T> sqrt(const scalar<T> &x)
			{ return scalar<T>(sqrt(x.val())); }
	inline static double sqrt(double x) { return ::sqrt(x); }
	inline static float sqrt(float x) { return ::sqrtf(x); }

public: // conj value
	template<typename T> inline static
		std::complex<T> conj(const std::complex<T> &x)
			{ return std::conj(x); }
	template<typename T> inline static
		scalar<T> conj(const scalar<T> &x)
			{ return scalar<T>(conj(x.val())); }
	inline static double conj(double x) { return x; }
	inline static float conj(float x) { return x; }
	inline static long conj(long x) { return x; }
	inline static int conj(int x) { return x; }

public: // real part
	template<typename T> inline static
		T real(const std::complex<T> &x)
			{ return std::real(x); }
	template<typename T> inline static
		scalar<T> real(const scalar<T> &x)
			{ return scalar<T>(real(x.val())); }
	inline static double real(double x) { return x; }
	inline static float real(float x) { return x; }
	inline static long real(long x) { return x; }
	inline static int real(int x) { return x; }

public: // imaginary part
	template<typename T> inline static
		T imag(const std::complex<T> &x)
			{ return std::imag(x); }
	template<typename T> inline static
		scalar<T> imag(const scalar<T> &x)
			{ return scalar<T>(imag(x.val())); }
	inline static double imag(double) { return 0.0; }
	inline static float imag(float) { return 0.0f; }
	inline static long imag(long) { return 0L; }
	inline static int imag(int) { return 0; }

public: // one constant
	template<typename T> inline static
		std::complex<T> one(const std::complex<T>&)
			{ return std::complex<T>(T(1)); }
	template<typename T> inline static
		scalar<T> one(const scalar<T>&)
			{ return scalar<T>(one(T())); }
	inline static double one(double) { return 1.0; }
	inline static float one(float) { return 1.0f; }
	inline static long one(long) { return 1L; }
	inline static int one(int) { return 1; }
};

		} // details namespace


//////////////////////////////////////////////////////////////////////////
// operator- (unary)
template<__OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Negate) operator-(__OMNI_UNOP_MX_ARGUMENTS)
		{ return details::make_unexpr<details::Negate>(x); }
template<__OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Negate) operator-(__OMNI_UNOP_UN_ARGUMENTS)
		{ return details::make_unexpr<details::Negate>(x); }
template<__OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Negate) operator-(__OMNI_UNOP_BI_ARGUMENTS)
		{ return details::make_unexpr<details::Negate>(x); }


//////////////////////////////////////////////////////////////////////////
// trans (unary)
template<__OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Trans) trans(__OMNI_UNOP_MX_ARGUMENTS)
		{ return details::make_unexpr<details::Trans>(x); }
template<__OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Trans) trans(__OMNI_UNOP_UN_ARGUMENTS)
		{ return details::make_unexpr<details::Trans>(x); }
template<__OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Trans) trans(__OMNI_UNOP_BI_ARGUMENTS)
		{ return details::make_unexpr<details::Trans>(x); }


//////////////////////////////////////////////////////////////////////////
// conj (unary)
template<typename Tr, __OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Conj<Tr>) conj(__OMNI_UNOP_MX_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Conj<Tr> >(x); }
template<__OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Conj<details::ValueTraits>) conj(__OMNI_UNOP_MX_ARGUMENTS)
		{ return details::make_unexpr< details::Conj<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Conj<Tr>) conj(__OMNI_UNOP_UN_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Conj<Tr> >(x); }
template<__OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Conj<details::ValueTraits>) conj(__OMNI_UNOP_UN_ARGUMENTS)
		{ return details::make_unexpr< details::Conj<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Conj<Tr>) conj(__OMNI_UNOP_BI_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Conj<Tr> >(x); }
template<__OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Conj<details::ValueTraits>) conj(__OMNI_UNOP_BI_ARGUMENTS)
		{ return details::make_unexpr< details::Conj<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
// herm (unary)
template<typename Tr, __OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Herm<Tr>) herm(__OMNI_UNOP_MX_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Herm<Tr> >(x); }
template<__OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Herm<details::ValueTraits>) herm(__OMNI_UNOP_MX_ARGUMENTS)
		{ return details::make_unexpr< details::Herm<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Herm<Tr>) herm(__OMNI_UNOP_UN_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Herm<Tr> >(x); }
template<__OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Herm<details::ValueTraits>) herm(__OMNI_UNOP_UN_ARGUMENTS)
		{ return details::make_unexpr< details::Herm<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Herm<Tr>) herm(__OMNI_UNOP_BI_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Herm<Tr> >(x); }
template<__OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Herm<details::ValueTraits>) herm(__OMNI_UNOP_BI_ARGUMENTS)
		{ return details::make_unexpr< details::Herm<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////////
// computes the solution to the system of linear equations (a*x=b)
template<typename Tr, __OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_MX_MX_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_MX_MX_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }
template<typename Tr, __OMNI_BIOP_MX_VR_TEMPLATE> inline
	__OMNI_BIOP_MX_VR_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_MX_VR_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_MX_VR_TEMPLATE> inline
	__OMNI_BIOP_MX_VR_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_MX_VR_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }
template<typename Tr, __OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_MX_UN_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_MX_UN_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }
template<typename Tr, __OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_MX_BI_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_MX_BI_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }

template<typename Tr, __OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_UN_MX_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_UN_MX_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }
template<typename Tr, __OMNI_BIOP_UN_VR_TEMPLATE> inline
	__OMNI_BIOP_UN_VR_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_UN_VR_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_UN_VR_TEMPLATE> inline
	__OMNI_BIOP_UN_VR_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_UN_VR_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }
template<typename Tr, __OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_UN_UN_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_UN_UN_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }
template<typename Tr, __OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_UN_BI_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_UN_BI_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }

template<typename Tr, __OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_BI_MX_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_BI_MX_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }
template<typename Tr, __OMNI_BIOP_BI_VR_TEMPLATE> inline
	__OMNI_BIOP_BI_VR_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_BI_VR_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_BI_VR_TEMPLATE> inline
	__OMNI_BIOP_BI_VR_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_BI_VR_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }
template<typename Tr, __OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_BI_UN_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_BI_UN_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }
template<typename Tr, __OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::Solve<Tr>) solve(__OMNI_BIOP_BI_BI_ARGUMENTS, const Tr&)
		{ return details::make_biexpr< details::Solve<Tr> >(x, y); }
template<__OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::Solve<details::ValueTraits>) solve(__OMNI_BIOP_BI_BI_ARGUMENTS)
		{ return details::make_biexpr< details::Solve<details::ValueTraits> >(x, y); }


//////////////////////////////////////////////////////////////////////////
// inv (unary)
template<typename Tr, __OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Inv<Tr>) inv(__OMNI_UNOP_MX_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Inv<Tr> >(x); }
template<__OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Inv<details::ValueTraits>) inv(__OMNI_UNOP_MX_ARGUMENTS)
		{ return details::make_unexpr< details::Inv<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Inv<Tr>) inv(__OMNI_UNOP_UN_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Inv<Tr> >(x); }
template<__OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Inv<details::ValueTraits>) inv(__OMNI_UNOP_UN_ARGUMENTS)
		{ return details::make_unexpr< details::Inv<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Inv<Tr>) inv(__OMNI_UNOP_BI_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Inv<Tr> >(x); }
template<__OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Inv<details::ValueTraits>) inv(__OMNI_UNOP_BI_ARGUMENTS)
		{ return details::make_unexpr< details::Inv<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
// chol lower (unary) x = L * herm(L)
template<typename Tr, __OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Inv<Tr>) choll(__OMNI_UNOP_MX_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Chol_L<Tr> >(x); }
template<__OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Inv<details::ValueTraits>) choll(__OMNI_UNOP_MX_ARGUMENTS)
		{ return details::make_unexpr< details::Chol_L<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Inv<Tr>) choll(__OMNI_UNOP_UN_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Chol_L<Tr> >(x); }
template<__OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Inv<details::ValueTraits>) choll(__OMNI_UNOP_UN_ARGUMENTS)
		{ return details::make_unexpr< details::Chol_L<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Inv<Tr>) choll(__OMNI_UNOP_BI_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Chol_L<Tr> >(x); }
template<__OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Inv<details::ValueTraits>) choll(__OMNI_UNOP_BI_ARGUMENTS)
		{ return details::make_unexpr< details::Chol_L<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
// chol upper (unary) x = herm(U) * U
template<typename Tr, __OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Inv<Tr>) cholu(__OMNI_UNOP_MX_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Chol_U<Tr> >(x); }
template<__OMNI_UNOP_MX_TEMPLATE> inline
	__OMNI_UNOP_MX_RESULT(details::Inv<details::ValueTraits>) cholu(__OMNI_UNOP_MX_ARGUMENTS)
		{ return details::make_unexpr< details::Chol_U<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Inv<Tr>) cholu(__OMNI_UNOP_UN_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Chol_U<Tr> >(x); }
template<__OMNI_UNOP_UN_TEMPLATE> inline
	__OMNI_UNOP_UN_RESULT(details::Inv<details::ValueTraits>) cholu(__OMNI_UNOP_UN_ARGUMENTS)
		{ return details::make_unexpr< details::Chol_U<details::ValueTraits> >(x); }
template<typename Tr, __OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Inv<Tr>) cholu(__OMNI_UNOP_BI_ARGUMENTS, const Tr&)
		{ return details::make_unexpr< details::Chol_U<Tr> >(x); }
template<__OMNI_UNOP_BI_TEMPLATE> inline
	__OMNI_UNOP_BI_RESULT(details::Inv<details::ValueTraits>) cholu(__OMNI_UNOP_BI_ARGUMENTS)
		{ return details::make_unexpr< details::Chol_U<details::ValueTraits> >(x); }


//////////////////////////////////////////////////////////////////////////
// operator+, xadd (binary)
template<__OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::XAdd) operator+(__OMNI_BIOP_MX_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::XAdd) operator+(__OMNI_BIOP_MX_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::XAdd) operator+(__OMNI_BIOP_MX_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }

template<__OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::XAdd) operator+(__OMNI_BIOP_UN_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::XAdd) operator+(__OMNI_BIOP_UN_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::XAdd) operator+(__OMNI_BIOP_UN_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }

template<__OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::XAdd) operator+(__OMNI_BIOP_BI_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::XAdd) operator+(__OMNI_BIOP_BI_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::XAdd) operator+(__OMNI_BIOP_BI_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }

template<typename TZ, __OMNI_UNOP_MX_TEMPLATE> inline
	matrix<TZ>& operator+=(matrix<TZ> &z, __OMNI_UNOP_MX_ARGUMENTS)
		{ return details::Evaluator<details::XAdd>::eval(z, z, x); }
template<typename TZ, __OMNI_UNOP_UN_TEMPLATE> inline
	matrix<TZ>& operator+=(matrix<TZ> &z, __OMNI_UNOP_UN_ARGUMENTS)
		{ return details::Evaluator<details::XAdd>::eval(z, z, x); }
template<typename TZ, __OMNI_UNOP_BI_TEMPLATE> inline
	matrix<TZ>& operator+=(matrix<TZ> &z, __OMNI_UNOP_BI_ARGUMENTS)
		{ return details::Evaluator<details::XAdd>::eval(z, z, x); }

template<__OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::XAdd) xadd(__OMNI_BIOP_MX_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_MX_SR_TEMPLATE> inline
	__OMNI_BIOP_MX_SR_RESULT(details::XAdd) xadd(__OMNI_BIOP_MX_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::XAdd) xadd(__OMNI_BIOP_MX_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::XAdd) xadd(__OMNI_BIOP_MX_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }

template<__OMNI_BIOP_SR_MX_TEMPLATE> inline
	__OMNI_BIOP_SR_MX_RESULT(details::XAdd) xadd(__OMNI_BIOP_SR_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_SR_UN_TEMPLATE> inline
	__OMNI_BIOP_SR_UN_RESULT(details::XAdd) xadd(__OMNI_BIOP_SR_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_SR_BI_TEMPLATE> inline
	__OMNI_BIOP_SR_BI_RESULT(details::XAdd) xadd(__OMNI_BIOP_SR_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }

template<__OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::XAdd) xadd(__OMNI_BIOP_UN_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_UN_SR_TEMPLATE> inline
	__OMNI_BIOP_UN_SR_RESULT(details::XAdd) xadd(__OMNI_BIOP_UN_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::XAdd) xadd(__OMNI_BIOP_UN_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::XAdd) xadd(__OMNI_BIOP_UN_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }

template<__OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::XAdd) xadd(__OMNI_BIOP_BI_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_BI_SR_TEMPLATE> inline
	__OMNI_BIOP_BI_SR_RESULT(details::XAdd) xadd(__OMNI_BIOP_BI_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::XAdd) xadd(__OMNI_BIOP_BI_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }
template<__OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::XAdd) xadd(__OMNI_BIOP_BI_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XAdd>(x, y); }


//////////////////////////////////////////////////////////////////////////
// operator-, xsub (binary)
template<__OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::XSub) operator-(__OMNI_BIOP_MX_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::XSub) operator-(__OMNI_BIOP_MX_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::XSub) operator-(__OMNI_BIOP_MX_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }

template<__OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::XSub) operator-(__OMNI_BIOP_UN_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::XSub) operator-(__OMNI_BIOP_UN_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::XSub) operator-(__OMNI_BIOP_UN_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }

template<__OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::XSub) operator-(__OMNI_BIOP_BI_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::XSub) operator-(__OMNI_BIOP_BI_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::XSub) operator-(__OMNI_BIOP_BI_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }

template<typename TZ, __OMNI_UNOP_MX_TEMPLATE> inline
	matrix<TZ>& operator-=(matrix<TZ> &z, __OMNI_UNOP_MX_ARGUMENTS)
		{ return details::Evaluator<details::XSub>::eval(z, z, x); }
template<typename TZ, __OMNI_UNOP_UN_TEMPLATE> inline
	matrix<TZ>& operator-=(matrix<TZ> &z, __OMNI_UNOP_UN_ARGUMENTS)
		{ return details::Evaluator<details::XSub>::eval(z, z, x); }
template<typename TZ, __OMNI_UNOP_BI_TEMPLATE> inline
	matrix<TZ>& operator-=(matrix<TZ> &z, __OMNI_UNOP_BI_ARGUMENTS)
		{ return details::Evaluator<details::XSub>::eval(z, z, x); }

template<__OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::XSub) xsub(__OMNI_BIOP_MX_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_MX_SR_TEMPLATE> inline
	__OMNI_BIOP_MX_SR_RESULT(details::XSub) xsub(__OMNI_BIOP_MX_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::XSub) xsub(__OMNI_BIOP_MX_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::XSub) xsub(__OMNI_BIOP_MX_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }

template<__OMNI_BIOP_SR_MX_TEMPLATE> inline
	__OMNI_BIOP_SR_MX_RESULT(details::XSub) xsub(__OMNI_BIOP_SR_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_SR_UN_TEMPLATE> inline
	__OMNI_BIOP_SR_UN_RESULT(details::XSub) xsub(__OMNI_BIOP_SR_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_SR_BI_TEMPLATE> inline
	__OMNI_BIOP_SR_BI_RESULT(details::XSub) xsub(__OMNI_BIOP_SR_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }

template<__OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::XSub) xsub(__OMNI_BIOP_UN_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_UN_SR_TEMPLATE> inline
	__OMNI_BIOP_UN_SR_RESULT(details::XSub) xsub(__OMNI_BIOP_UN_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::XSub) xsub(__OMNI_BIOP_UN_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::XSub) xsub(__OMNI_BIOP_UN_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }

template<__OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::XSub) xsub(__OMNI_BIOP_BI_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_BI_SR_TEMPLATE> inline
	__OMNI_BIOP_BI_SR_RESULT(details::XSub) xsub(__OMNI_BIOP_BI_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::XSub) xsub(__OMNI_BIOP_BI_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }
template<__OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::XSub) xsub(__OMNI_BIOP_BI_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XSub>(x, y); }


//////////////////////////////////////////////////////////////////////////
// xmul (binary)
template<__OMNI_BIOP_MX_SR_TEMPLATE> inline
	__OMNI_BIOP_MX_SR_RESULT(details::XMul) operator*(__OMNI_BIOP_MX_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_UN_SR_TEMPLATE> inline
	__OMNI_BIOP_UN_SR_RESULT(details::XMul) operator*(__OMNI_BIOP_UN_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_BI_SR_TEMPLATE> inline
	__OMNI_BIOP_BI_SR_RESULT(details::XMul) operator*(__OMNI_BIOP_BI_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_SR_MX_TEMPLATE> inline
	__OMNI_BIOP_SR_MX_RESULT(details::XMul) operator*(__OMNI_BIOP_SR_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_SR_UN_TEMPLATE> inline
	__OMNI_BIOP_SR_UN_RESULT(details::XMul) operator*(__OMNI_BIOP_SR_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_SR_BI_TEMPLATE> inline
	__OMNI_BIOP_SR_BI_RESULT(details::XMul) operator*(__OMNI_BIOP_SR_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }

template<typename TZ, __OMNI_UNOP_SR_TEMPLATE> inline
	matrix<TZ>& operator*=(matrix<TZ> &z, __OMNI_UNOP_SR_ARGUMENTS)
		{ return details::Evaluator<details::XMul>::eval(z, z, x); }

template<__OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::XMul) xmul(__OMNI_BIOP_MX_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_MX_SR_TEMPLATE> inline
	__OMNI_BIOP_MX_SR_RESULT(details::XMul) xmul(__OMNI_BIOP_MX_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::XMul) xmul(__OMNI_BIOP_MX_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::XMul) xmul(__OMNI_BIOP_MX_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }

template<__OMNI_BIOP_SR_MX_TEMPLATE> inline
	__OMNI_BIOP_SR_MX_RESULT(details::XMul) xmul(__OMNI_BIOP_SR_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_SR_UN_TEMPLATE> inline
	__OMNI_BIOP_SR_UN_RESULT(details::XMul) xmul(__OMNI_BIOP_SR_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_SR_BI_TEMPLATE> inline
	__OMNI_BIOP_SR_BI_RESULT(details::XMul) xmul(__OMNI_BIOP_SR_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }

template<__OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::XMul) xmul(__OMNI_BIOP_UN_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_UN_SR_TEMPLATE> inline
	__OMNI_BIOP_UN_SR_RESULT(details::XMul) xmul(__OMNI_BIOP_UN_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::XMul) xmul(__OMNI_BIOP_UN_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::XMul) xmul(__OMNI_BIOP_UN_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }

template<__OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::XMul) xmul(__OMNI_BIOP_BI_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_BI_SR_TEMPLATE> inline
	__OMNI_BIOP_BI_SR_RESULT(details::XMul) xmul(__OMNI_BIOP_BI_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::XMul) xmul(__OMNI_BIOP_BI_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }
template<__OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::XMul) xmul(__OMNI_BIOP_BI_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XMul>(x, y); }


//////////////////////////////////////////////////////////////////////////
// xdiv (binary)
template<__OMNI_BIOP_MX_SR_TEMPLATE> inline
	__OMNI_BIOP_MX_SR_RESULT(details::XDiv) operator/(__OMNI_BIOP_MX_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_UN_SR_TEMPLATE> inline
	__OMNI_BIOP_UN_SR_RESULT(details::XDiv) operator/(__OMNI_BIOP_UN_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_BI_SR_TEMPLATE> inline
	__OMNI_BIOP_BI_SR_RESULT(details::XDiv) operator/(__OMNI_BIOP_BI_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }

template<typename TZ, __OMNI_UNOP_SR_TEMPLATE> inline
	matrix<TZ>& operator/=(matrix<TZ> &z, __OMNI_UNOP_SR_ARGUMENTS)
		{ return details::Evaluator<details::XDiv>::eval(z, z, x); }

template<__OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::XDiv) xdiv(__OMNI_BIOP_MX_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_MX_SR_TEMPLATE> inline
	__OMNI_BIOP_MX_SR_RESULT(details::XDiv) xdiv(__OMNI_BIOP_MX_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::XDiv) xdiv(__OMNI_BIOP_MX_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::XDiv) xdiv(__OMNI_BIOP_MX_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }

template<__OMNI_BIOP_SR_MX_TEMPLATE> inline
	__OMNI_BIOP_SR_MX_RESULT(details::XDiv) xdiv(__OMNI_BIOP_SR_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_SR_UN_TEMPLATE> inline
	__OMNI_BIOP_SR_UN_RESULT(details::XDiv) xdiv(__OMNI_BIOP_SR_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_SR_BI_TEMPLATE> inline
	__OMNI_BIOP_SR_BI_RESULT(details::XDiv) xdiv(__OMNI_BIOP_SR_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }

template<__OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::XDiv) xdiv(__OMNI_BIOP_UN_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_UN_SR_TEMPLATE> inline
	__OMNI_BIOP_UN_SR_RESULT(details::XDiv) xdiv(__OMNI_BIOP_UN_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::XDiv) xdiv(__OMNI_BIOP_UN_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::XDiv) xdiv(__OMNI_BIOP_UN_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }

template<__OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::XDiv) xdiv(__OMNI_BIOP_BI_MX_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_BI_SR_TEMPLATE> inline
	__OMNI_BIOP_BI_SR_RESULT(details::XDiv) xdiv(__OMNI_BIOP_BI_SR_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::XDiv) xdiv(__OMNI_BIOP_BI_UN_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }
template<__OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::XDiv) xdiv(__OMNI_BIOP_BI_BI_ARGUMENTS)
		{ return details::make_biexpr<details::XDiv>(x, y); }


//////////////////////////////////////////////////////////////////////////
// mul (binary)
template<__OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::Mul) operator*(__OMNI_BIOP_MX_MX_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_MX_VR_TEMPLATE> inline
	__OMNI_BIOP_MX_VR_RESULT(details::Mul) operator*(__OMNI_BIOP_MX_VR_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::Mul) operator*(__OMNI_BIOP_MX_UN_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::Mul) operator*(__OMNI_BIOP_MX_BI_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }

template<__OMNI_BIOP_VR_MX_TEMPLATE> inline
	__OMNI_BIOP_VR_MX_RESULT(details::Mul) operator*(__OMNI_BIOP_VR_MX_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_VR_UN_TEMPLATE> inline
	__OMNI_BIOP_VR_UN_RESULT(details::Mul) operator*(__OMNI_BIOP_VR_UN_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_VR_BI_TEMPLATE> inline
	__OMNI_BIOP_VR_BI_RESULT(details::Mul) operator*(__OMNI_BIOP_VR_BI_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }

template<__OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::Mul) operator*(__OMNI_BIOP_UN_MX_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_UN_VR_TEMPLATE> inline
	__OMNI_BIOP_UN_VR_RESULT(details::Mul) operator*(__OMNI_BIOP_UN_VR_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::Mul) operator*(__OMNI_BIOP_UN_UN_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::Mul) operator*(__OMNI_BIOP_UN_BI_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }

template<__OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::Mul) operator*(__OMNI_BIOP_BI_MX_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_BI_VR_TEMPLATE> inline
	__OMNI_BIOP_BI_VR_RESULT(details::Mul) operator*(__OMNI_BIOP_BI_VR_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::Mul) operator*(__OMNI_BIOP_BI_UN_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::Mul) operator*(__OMNI_BIOP_BI_BI_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }

template<typename TZ, __OMNI_UNOP_MX_TEMPLATE> inline
	matrix<TZ>& operator*=(matrix<TZ> &z, __OMNI_UNOP_MX_ARGUMENTS)
		{ return details::Evaluator<details::Mul>::eval(z, z, x); }
template<typename TZ, __OMNI_UNOP_VR_TEMPLATE> inline
	matrix<TZ>& operator*=(matrix<TZ> &z, __OMNI_UNOP_VR_ARGUMENTS)
		{ return details::Evaluator<details::Mul>::eval(z, z, x); }
template<typename TZ, __OMNI_UNOP_UN_TEMPLATE> inline
	matrix<TZ>& operator*=(matrix<TZ> &z, __OMNI_UNOP_UN_ARGUMENTS)
		{ return details::Evaluator<details::Mul>::eval(z, z, x); }
template<typename TZ, __OMNI_UNOP_BI_TEMPLATE> inline
	matrix<TZ>& operator*=(matrix<TZ> &z, __OMNI_UNOP_BI_ARGUMENTS)
		{ return details::Evaluator<details::Mul>::eval(z, z, x); }

template<__OMNI_BIOP_MX_MX_TEMPLATE> inline
	__OMNI_BIOP_MX_MX_RESULT(details::Mul) mul(__OMNI_BIOP_MX_MX_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_MX_VR_TEMPLATE> inline
	__OMNI_BIOP_MX_VR_RESULT(details::Mul) mul(__OMNI_BIOP_MX_VR_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_MX_UN_TEMPLATE> inline
	__OMNI_BIOP_MX_UN_RESULT(details::Mul) mul(__OMNI_BIOP_MX_UN_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_MX_BI_TEMPLATE> inline
	__OMNI_BIOP_MX_BI_RESULT(details::Mul) mul(__OMNI_BIOP_MX_BI_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }

template<__OMNI_BIOP_VR_MX_TEMPLATE> inline
	__OMNI_BIOP_VR_MX_RESULT(details::Mul) mul(__OMNI_BIOP_VR_MX_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_VR_UN_TEMPLATE> inline
	__OMNI_BIOP_VR_UN_RESULT(details::Mul) mul(__OMNI_BIOP_VR_UN_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_VR_BI_TEMPLATE> inline
	__OMNI_BIOP_VR_BI_RESULT(details::Mul) mul(__OMNI_BIOP_VR_BI_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }

template<__OMNI_BIOP_UN_MX_TEMPLATE> inline
	__OMNI_BIOP_UN_MX_RESULT(details::Mul) mul(__OMNI_BIOP_UN_MX_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_UN_VR_TEMPLATE> inline
	__OMNI_BIOP_UN_VR_RESULT(details::Mul) mul(__OMNI_BIOP_UN_VR_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_UN_UN_TEMPLATE> inline
	__OMNI_BIOP_UN_UN_RESULT(details::Mul) mul(__OMNI_BIOP_UN_UN_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_UN_BI_TEMPLATE> inline
	__OMNI_BIOP_UN_BI_RESULT(details::Mul) mul(__OMNI_BIOP_UN_BI_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }

template<__OMNI_BIOP_BI_MX_TEMPLATE> inline
	__OMNI_BIOP_BI_MX_RESULT(details::Mul) mul(__OMNI_BIOP_BI_MX_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_BI_VR_TEMPLATE> inline
	__OMNI_BIOP_BI_VR_RESULT(details::Mul) mul(__OMNI_BIOP_BI_VR_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_BI_UN_TEMPLATE> inline
	__OMNI_BIOP_BI_UN_RESULT(details::Mul) mul(__OMNI_BIOP_BI_UN_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }
template<__OMNI_BIOP_BI_BI_TEMPLATE> inline
	__OMNI_BIOP_BI_BI_RESULT(details::Mul) mul(__OMNI_BIOP_BI_BI_ARGUMENTS)
		{ return details::make_biexpr<details::Mul>(x, y); }

	} // mx namespace


	// scalar operators
	namespace mx
	{

// prefix increment
template<typename T> inline
	scalar<T>& operator++(scalar<T> &x)
{
	++x.val();
	return x;
}

// prefix decrement
template<typename T> inline
	scalar<T>& operator--(scalar<T> &x)
{
	--x.val();
	return x;
}

// postfix increment
template<typename T> inline
	scalar<T> operator++(scalar<T> &x, int)
{
	scalar<T> t(x);
	++x;
	return t;
}

// postfix decrement
template<typename T> inline
	scalar<T> operator--(scalar<T> &x, int)
{
	scalar<T> t(x);
	--x;
	return t;
}

// 
template<typename T> inline
	scalar<T> operator~(const scalar<T> &x)
{
	return scalar<T>(~x.val());
}

// negate
template<typename T> inline
	scalar<T> operator!(const scalar<T> &x)
{
	return scalar<T>(!x.val());
}

// 
template<typename T> inline
	scalar<T> operator-(const scalar<T> &x)
{
	return scalar<T>(-x.val());
}

// 
template<typename T> inline
	scalar<T> operator+(const scalar<T> &x)
{
	return scalar<T>(+x.val());
}


// multiplication
template<typename T> inline
	scalar<T> operator*(const scalar<T> &x, const scalar<T> &y)
{
	return scalar<T>(x.val() * y.val());
}

// division
template<typename T> inline
	scalar<T> operator/(const scalar<T> &x, const scalar<T> &y)
{
	return scalar<T>(x.val() / y.val());
}

// remainder
template<typename T> inline
	scalar<T> operator%(const scalar<T> &x, const scalar<T> &y)
{
	return scalar<T>(x.val() % y.val());
}

// addition
template<typename T> inline
	scalar<T> operator+(const scalar<T> &x, const scalar<T> &y)
{
	return scalar<T>(x.val() + y.val());
}

// subtraction
template<typename T> inline
	scalar<T> operator-(const scalar<T> &x, const scalar<T> &y)
{
	return scalar<T>(x.val() - y.val());
}


// equals
template<typename T> inline
	bool operator==(const scalar<T> &x, const scalar<T> &y)
{
	return x.val() == y.val();
}

// not equals
template<typename T> inline
	bool operator!=(const scalar<T> &x, const scalar<T> &y)
{
	return x.val() != y.val();
}

// less
template<typename T> inline
	bool operator<(const scalar<T> &x, const scalar<T> &y)
{
	return x.val() < y.val();
}

// less or equal
template<typename T> inline
	bool operator<=(const scalar<T> &x, const scalar<T> &y)
{
	return x.val() <= y.val();
}

// great
template<typename T> inline
	bool operator>(const scalar<T> &x, const scalar<T> &y)
{
	return x.val() > y.val();
}

// great or equal
template<typename T> inline
	bool operator>=(const scalar<T> &x, const scalar<T> &y)
{
	return x.val() >= y.val();
}


// 
template<typename T> inline
	scalar<T>& operator*=(scalar<T> &x, const scalar<T> &y)
{
	x.val() *= y.val();
	return x;
}

// 
template<typename T> inline
	scalar<T>& operator/=(scalar<T> &x, const scalar<T> &y)
{
	x.val() /= y.val();
	return x;
}

// 
template<typename T> inline
	scalar<T>& operator%=(scalar<T> &x, const scalar<T> &y)
{
	x.val() %= y.val();
	return x;
}

// 
template<typename T> inline
	scalar<T>& operator+=(scalar<T> &x, const scalar<T> &y)
{
	x.val() += y.val();
	return x;
}

// 
template<typename T> inline
	scalar<T>& operator-=(scalar<T> &x, const scalar<T> &y)
{
	x.val() -= y.val();
	return x;
}


// write to output stream
template<typename T, typename E, typename Tr> inline
	std::basic_ostream<E, Tr>& operator<<(
		std::basic_ostream<E, Tr> &s_out, const scalar<T> &x)
{
	return s_out << x.val();
}

// read from input stream
template<typename T, typename E, typename Tr> inline
	std::basic_istream<E, Tr>& operator>>(
		std::basic_istream<E, Tr> &s_in, scalar<T> &x)
{
	return s_in >> x.val();
}

	} // mx namespace
} // omni namespace


#undef __OMNI_UNOP_MX_TEMPLATE
#undef __OMNI_UNOP_SR_TEMPLATE
#undef __OMNI_UNOP_VR_TEMPLATE
#undef __OMNI_UNOP_UN_TEMPLATE
#undef __OMNI_UNOP_BI_TEMPLATE
#undef __OMNI_BIOP_MX_MX_TEMPLATE
#undef __OMNI_BIOP_MX_SR_TEMPLATE
#undef __OMNI_BIOP_MX_VR_TEMPLATE
#undef __OMNI_BIOP_MX_UN_TEMPLATE
#undef __OMNI_BIOP_MX_BI_TEMPLATE
#undef __OMNI_BIOP_SR_MX_TEMPLATE
#undef __OMNI_BIOP_SR_UN_TEMPLATE
#undef __OMNI_BIOP_SR_BI_TEMPLATE
#undef __OMNI_BIOP_VR_MX_TEMPLATE
#undef __OMNI_BIOP_VR_UN_TEMPLATE
#undef __OMNI_BIOP_VR_BI_TEMPLATE
#undef __OMNI_BIOP_UN_MX_TEMPLATE
#undef __OMNI_BIOP_UN_SR_TEMPLATE
#undef __OMNI_BIOP_UN_VR_TEMPLATE
#undef __OMNI_BIOP_UN_UN_TEMPLATE
#undef __OMNI_BIOP_UN_BI_TEMPLATE
#undef __OMNI_BIOP_BI_MX_TEMPLATE
#undef __OMNI_BIOP_BI_SR_TEMPLATE
#undef __OMNI_BIOP_BI_VR_TEMPLATE
#undef __OMNI_BIOP_BI_UN_TEMPLATE
#undef __OMNI_BIOP_BI_BI_TEMPLATE

#undef __OMNI_UNOP_MX_RESULT
#undef __OMNI_UNOP_UN_RESULT
#undef __OMNI_UNOP_BI_RESULT
#undef __OMNI_BIOP_MX_MX_RESULT
#undef __OMNI_BIOP_MX_SR_RESULT
#undef __OMNI_BIOP_MX_VR_RESULT
#undef __OMNI_BIOP_MX_UN_RESULT
#undef __OMNI_BIOP_MX_BI_RESULT
#undef __OMNI_BIOP_SR_MX_RESULT
#undef __OMNI_BIOP_SR_UN_RESULT
#undef __OMNI_BIOP_SR_BI_RESULT
#undef __OMNI_BIOP_VR_MX_RESULT
#undef __OMNI_BIOP_VR_UN_RESULT
#undef __OMNI_BIOP_VR_BI_RESULT
#undef __OMNI_BIOP_UN_MX_RESULT
#undef __OMNI_BIOP_UN_SR_RESULT
#undef __OMNI_BIOP_UN_VR_RESULT
#undef __OMNI_BIOP_UN_UN_RESULT
#undef __OMNI_BIOP_UN_BI_RESULT
#undef __OMNI_BIOP_BI_MX_RESULT
#undef __OMNI_BIOP_BI_SR_RESULT
#undef __OMNI_BIOP_BI_VR_RESULT
#undef __OMNI_BIOP_BI_UN_RESULT
#undef __OMNI_BIOP_BI_BI_RESULT

#undef __OMNI_UNOP_MX_ARGUMENTS
#undef __OMNI_UNOP_SR_ARGUMENTS
#undef __OMNI_UNOP_VR_ARGUMENTS
#undef __OMNI_UNOP_UN_ARGUMENTS
#undef __OMNI_UNOP_BI_ARGUMENTS
#undef __OMNI_BIOP_MX_MX_ARGUMENTS
#undef __OMNI_BIOP_MX_SR_ARGUMENTS
#undef __OMNI_BIOP_MX_VR_ARGUMENTS
#undef __OMNI_BIOP_MX_UN_ARGUMENTS
#undef __OMNI_BIOP_MX_BI_ARGUMENTS
#undef __OMNI_BIOP_VR_MX_ARGUMENTS
#undef __OMNI_BIOP_VR_UN_ARGUMENTS
#undef __OMNI_BIOP_VR_BI_ARGUMENTS
#undef __OMNI_BIOP_VR_MX_ARGUMENTS
#undef __OMNI_BIOP_VR_UN_ARGUMENTS
#undef __OMNI_BIOP_VR_BI_ARGUMENTS
#undef __OMNI_BIOP_UN_MX_ARGUMENTS
#undef __OMNI_BIOP_UN_SR_ARGUMENTS
#undef __OMNI_BIOP_UN_VR_ARGUMENTS
#undef __OMNI_BIOP_UN_UN_ARGUMENTS
#undef __OMNI_BIOP_UN_BI_ARGUMENTS
#undef __OMNI_BIOP_BI_MX_ARGUMENTS
#undef __OMNI_BIOP_BI_SR_ARGUMENTS
#undef __OMNI_BIOP_BI_VR_ARGUMENTS
#undef __OMNI_BIOP_BI_UN_ARGUMENTS
#undef __OMNI_BIOP_BI_BI_ARGUMENTS


#endif // __OMNI_MXMATH_H_
