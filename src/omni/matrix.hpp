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
//		https://bitbucket.org/pilatuz/omni
//////////////////////////////////////////////////////////////////////////
/** @file
	@brief The basic matrix tools.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_MATRIX_HPP_
#define __OMNI_MATRIX_HPP_

// TODO: avoid Visual Studio 8 checked iterators for row_iterator and col_iterator.
// TODO: promote<> concept

#include <omni/defs.hpp>

#include <algorithm>
#include <iterator>
#include <vector>

#include <assert.h>

// row-major or column-major
#if !defined(OMNI_MATRIX_ROW_MAJOR)
#	define OMNI_MATRIX_ROW_MAJOR 0 // (!) the matrix is column-major by default
#endif

// default allocator
#if !defined(OMNI_MATRIX_ALLOCATOR)
#	if defined(OMNI_MATRIC_STD_ALLOCATOR)
#		define OMNI_MATRIX_ALLOCATOR(T)  std::allocator<T>
#	else
#		define OMNI_MATRIX_ALLOCATOR(T)  omni::pool::Allocator<T>
#		include <omni/pool.hpp>
#	endif // OMNI_MATRIX_STD_ALLOCATOR
#endif

namespace omni
{
	// forward declarations
	namespace mx
	{
		template<typename T>
			class Scalar;
		template<typename T, typename A>
			class Vector;
		template<typename T, typename A>
			class Matrix;

		// implementation
		namespace details
		{
			template<typename T, typename A>
				class NConstRow;
			template<typename T, typename A>
				class ConstRow;

			template<typename Base>
				class Iterator1;
			template<typename Base>
				class IteratorN;

			template<typename OP, typename T1>
				class UnVExpr;
			template<typename OP, typename T1, typename T2>
				class BiVExpr;

			template<typename OP, typename T1>
				class UnMExpr;
			template<typename OP, typename T1, typename T2>
				class BiMExpr;
		}
	} // forward declarations


	// Scalar
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The scalar wrapper class.
/**
		This class represents custom scalar value.
*/
template<typename T>
class Scalar {
	typedef Scalar<T> this_type;

public:
	typedef T value_type; ///< @brief The value type.

	typedef const T& const_reference;  ///< @brief The constant reference type.
	typedef       T&       reference;  ///< @brief The reference type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		The scalar value initialized by default value T().
*/
	Scalar()
		: m_val()
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Main constructor.
/**
		The scalar value initialized by @a x.

@param x The scalar value.
*/
	explicit Scalar(const_reference x)
		: m_val(x)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Assignment operator.
/**
@param x The scalar value.
*/
	this_type& operator=(const_reference x)
	{
		m_val = x;
		return *this;
	}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get value.
/**
@return The scalar constant reference.
*/
	const_reference val() const
	{
		return m_val;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get/Set value.
/**
@return The scalar non-constant reference.
*/
	reference val()
	{
		return m_val;
	}

private:
	value_type m_val; ///< @brief The scalar value.
};


//////////////////////////////////////////////////////////////////////////
/// @brief Create custom scalar.
/** @relates Scalar
@param x The scalar value.
@return The scalar.
*/
template<typename T> inline
	Scalar<T> make_scalar(const T &x)
{
	return Scalar<T>(x);
}

	} // Scalar


	// Vector
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The vector class.
/**
		This class represents a custom column (!) vector.
*/
template<typename T, typename A = OMNI_MATRIX_ALLOCATOR(T) >
class Vector: public std::vector<T,A>
{
	typedef std::vector<T,A> inherited;
	typedef Vector<T,A> this_type;

public:
	typedef typename inherited::allocator_type allocator_type;
	typedef typename inherited::difference_type difference_type;
	typedef typename inherited::size_type size_type;

	typedef typename inherited::value_type value_type;
	typedef typename inherited::const_reference const_reference;
	typedef typename inherited::reference reference;
	typedef typename inherited::const_pointer const_pointer;
	typedef typename inherited::pointer pointer;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
	Vector()
	{}


//////////////////////////////////////////////////////////////////////////
	explicit Vector(size_type n)
		: inherited(n)
	{}

//////////////////////////////////////////////////////////////////////////
	Vector(size_type n, const_reference val)
		: inherited(n, val)
	{}

//////////////////////////////////////////////////////////////////////////
	Vector(const inherited &other)
		: inherited(other)
	{}

//////////////////////////////////////////////////////////////////////////
	Vector(const this_type &other)
		: inherited(other)
	{}

//////////////////////////////////////////////////////////////////////////
	template<typename T2, typename A2>
	Vector(const std::vector<T2,A2> &other)
		: inherited(other.begin(), other.end())
	{}

//////////////////////////////////////////////////////////////////////////
	template<typename T2, typename A2>
	Vector(const Vector<T2,A2> &other)
		: inherited(other.begin(), other.end())
	{}

//////////////////////////////////////////////////////////////////////////
	template<typename T2, typename A2>
	Vector(const Matrix<T2,A2> &other)
		: inherited(other.begin(), other.end())
	{
		assert(other.Nrows()==0 || other.Ncols()==0
			|| other.Nrows()==1 || other.Ncols()==1);
	}

//////////////////////////////////////////////////////////////////////////
	template<class In>
	Vector(In first, In last)
		: inherited(first, last)
	{}

//////////////////////////////////////////////////////////////////////////
	~Vector()
	{}

public:

//////////////////////////////////////////////////////////////////////////
	template<typename OP, typename T1>
	/*explicit */Vector(const details::UnVExpr<OP,T1> &expr)
	{
		expr(*this);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename OP, typename T1, typename T2>
	/*explicit */Vector(const details::BiVExpr<OP,T1,T2> &expr)
	{
		expr(*this);
	}

public:

//////////////////////////////////////////////////////////////////////////
	this_type& operator=(const inherited &other)
	{
		if (this != &other)
			inherited::operator=(other);

		return *this;
	}

//////////////////////////////////////////////////////////////////////////
	this_type& operator=(const this_type &other)
	{
		if (this != &other)
			inherited::operator=(other);

		return *this;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename T2, typename A2>
	this_type& operator=(const Vector<T2,A2> &other)
	{
		inherited::assign(other.begin(), other.end());
		return *this;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename T2, typename A2>
	this_type& operator=(const Matrix<T2,A2> &other)
	{
		assert(other.Nrows()==0 || other.Ncols()==0
			|| other.Nrows()==1 || other.Ncols()==1);
		inherited::assign(other.begin(), other.end());
		return *this;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename OP, typename T1>
	this_type& operator=(const details::UnVExpr<OP,T1> &expr)
	{
		expr(*this);
		return *this;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename OP, typename T1, typename T2>
	this_type& operator=(const details::BiVExpr<OP,T1,T2> &expr)
	{
		expr(*this);
		return *this;
	}
};

	} // Vector


	// Matrix
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The matrix class.
template<typename T, typename A = OMNI_MATRIX_ALLOCATOR(T) >
class Matrix
{
	typedef Matrix<T,A> this_type;
	typedef std::vector<T,A> base_type;

public: // typedefs
	typedef typename base_type::allocator_type allocator_type;
	typedef typename base_type::value_type value_type; ///< @brief The value type.
	typedef typename base_type::size_type size_type;   ///< @brief The size and index type.

	typedef typename base_type::const_reference const_reference; ///< @brief The constant reference type.
	typedef typename base_type::reference reference; ///< @brief The reference type.
	typedef typename base_type::const_pointer const_pointer; ///< @brief The constant pointer type.
	typedef typename base_type::pointer pointer; ///< @brief The pointer type.

public: /// @name Constructors

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
	Matrix()
		: m_N_rows(0),
		  m_N_cols(0)
	{}

//////////////////////////////////////////////////////////////////////////
	Matrix(size_type N_rows, size_type N_cols)
		: m_buf(N_rows*N_cols),
		  m_N_rows(N_rows),
		  m_N_cols(N_cols)
	{}

//////////////////////////////////////////////////////////////////////////
	Matrix(size_type N_rows, size_type N_cols, const_reference val)
		: m_buf(N_rows*N_cols, val),
		  m_N_rows(N_rows),
		  m_N_cols(N_cols)
	{}

//////////////////////////////////////////////////////////////////////////
	Matrix(const this_type &other)
		: m_buf(other.m_buf),
		  m_N_rows(other.m_N_rows),
		  m_N_cols(other.m_N_cols)
	{}

//////////////////////////////////////////////////////////////////////////
	template<typename T2, typename A2>
	Matrix(const Matrix<T2,A2> &other)
		: m_buf(other.begin(), other.end()),
		  m_N_rows(other.Nrows()),
		  m_N_cols(other.Ncols())
	{}

//////////////////////////////////////////////////////////////////////////
	template<typename T2, typename A2>
	explicit Matrix(const std::vector<T2,A2> &other, bool is_column = true)
		: m_buf(other.begin(), other.end()),
		  m_N_rows(is_column ? other.size() : 1),
		  m_N_cols(is_column ? 1 : other.size())
	{}

public:

//////////////////////////////////////////////////////////////////////////
	template<typename OP, typename T1>
	/*explicit */Matrix(const details::UnMExpr<OP,T1> &expr)
	{
		expr(*this);
	}

//////////////////////////////////////////////////////////////////////////
	template<typename OP, typename T1, typename T2>
	/*explicit */Matrix(const details::BiMExpr<OP,T1,T2> &expr)
	{
		expr(*this);
	}

public:

//////////////////////////////////////////////////////////////////////////
	this_type& operator=(const this_type &other)
	{
		if (this != &other)
		{
			m_buf = other.m_buf;
			m_N_rows = other.m_N_rows;
			m_N_cols = other.m_N_cols;
		}

		return *this;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename T2, typename A2> // auxiliary assignment operator
	this_type& operator=(const Matrix<T2,A2> &other)
	{
		m_buf.assign(other.begin(), other.end());
		m_N_rows = other.Nrows();
		m_N_cols = other.Ncols();

		return *this;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename T2, typename A2>
	this_type& operator=(const std::vector<T2,A2> &other)
	{
		m_buf.assign(other.begin(), other.end());
		m_N_rows = other.size();
		m_N_cols = 1;

		return *this;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename OP, typename T1>
	this_type& operator=(const details::UnMExpr<OP,T1> &expr)
	{
		expr(*this);
		return *this;
	}

//////////////////////////////////////////////////////////////////////////
	template<typename OP, typename T1, typename T2>
	this_type& operator=(const details::BiMExpr<OP,T1,T2> &expr)
	{
		expr(*this);
		return *this;
	}

public: // size, resize and assign

//////////////////////////////////////////////////////////////////////////
/// @brief Get the number of rows.
	size_type Nrows() const
	{
		return m_N_rows;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the number of columns.
	size_type Ncols() const
	{
		return m_N_cols;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new matrix size.
/**
@warning The matrix content after operation is undefined!

@param N_rows The number of rows.
@param N_cols The number of columns.
*/
	void resize(size_type N_rows, size_type N_cols)
	{
		m_buf.resize(N_rows*N_cols);
		m_N_rows = N_rows;
		m_N_cols = N_cols;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Set the new matrix size.
/**
		This method saves the previous matrix content.

@param N_rows The number of rows.
@param N_cols The number of columns.
*/
	void safe_resize(size_type N_rows, size_type N_cols)
	{
		if (Nrows()!=N_rows || Ncols()!=N_cols)
		{
			this_type t(N_rows, N_cols);

			// minimum size
			if (Nrows() < N_rows)
				N_rows = Nrows();
			if (Ncols() < N_cols)
				N_cols = Ncols();

			// copy old content
			for (size_t i = 0; i < N_rows; ++i)
			#if defined(_MSC_VER) && (1400 <= _MSC_VER) && (1600 != _MSC_VER)
				// avoid VS8.0, VS9.0 checked iterators
				// VS10.0 doesn't support unchecked_copy() yet
				stdext::unchecked_copy(row_begin(i),
					row_begin(i)+N_cols,
						t.row_begin(i));
			#else
				std::copy(row_begin(i),
					row_begin(i)+N_cols,
						t.row_begin(i));
			#endif // (1400 <= _MSC_VER)

			swap(t);
		}
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign the new matrix size and content.
/**
@param N_rows The number of rows.
@param N_cols The number of columns.
@param first The begin of the new content.
@param last The end of the new content.
*/
	template<typename In>
	void assign(size_type N_rows, size_type N_cols, In first, In last)
	{
		assert(N_rows*N_cols == size_type(std::distance(first, last))
			&& "invalid input sequence size");
		m_buf.assign(first, last);
		m_N_rows = N_rows;
		m_N_cols = N_cols;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign the new matrix size and fill the content.
/**
		This method resizes the matrix and fills the matrix content by @a val value.

@param N_rows The number of rows.
@param N_cols The number of columns.
@param val The new content value.
*/
	void assign(size_type N_rows, size_type N_cols, const_reference val)
	{
		m_buf.assign(N_rows*N_cols, val);
		m_N_rows = N_rows;
		m_N_cols = N_cols;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign the new matrix content.
/**
@param first The begin of the new content.
@param last The end of the new content.
*/
	template<typename In>
	void assign(In first, In last)
	{
		assert(m_buf.size() == size_type(std::distance(first, last))
			&& "invalid input sequence size");
		m_buf.assign(first, last);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Fill the matrix content.
/**
		This method fills the matrix content by @a val value.

@param val The new content value.
*/
	void assign(const_reference val)
	{
		m_buf.assign(m_buf.size(), val);
	}

public: // matrix access
	typedef details::ConstRow<T,A>   ConstRow; ///< @brief The constant row wrapper type.
	typedef details::NConstRow<T,A> NConstRow; ///< @brief The non-contains row wrapper type.


//////////////////////////////////////////////////////////////////////////
/// @brief Get element at specified position.
/**
@param row The row index.
@param col The column index.
@return The element constant reference.
*/
	const_reference at(size_type row, size_type col) const
	{
		assert(row < m_N_rows && "row index out of range");
		assert(col < m_N_cols && "column index out of range");

		#if OMNI_MATRIX_ROW_MAJOR
			return m_buf[row*m_N_cols + col];
		#else
			return m_buf[col*m_N_rows + row];
		#endif
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get/Set element at specified position.
/**
@param row The row index.
@param col The column index.
@return The element non-constant reference.
*/
	reference at(size_type row, size_type col)
	{
		assert(row < m_N_rows && "row index out of range");
		assert(col < m_N_cols && "column index out of range");

		#if OMNI_MATRIX_ROW_MAJOR
			return m_buf[row*m_N_cols + col];
		#else
			return m_buf[col*m_N_rows + row];
		#endif
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element at specified position (linear indexing).
/**
@param pos The element position in linear array.
@return The element constant reference.
*/
	const_reference at(size_type pos) const
	{
		assert(pos < m_buf.size() && "index out of range");

		return m_buf[pos];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get/set element at specified position (linear indexing).
/**
@param pos The element position in linear array.
@return The element non-constant reference.
*/
	reference at(size_type pos)
	{
		assert(pos < m_buf.size() && "index out of range");

		return m_buf[pos];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element at specified position ([][] style).
/**
@param row The row index.
*/
	ConstRow operator[](size_type row) const
	{
		// assert(row < N_rows() && "row index out of range");
		return ConstRow(*this, row);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get/set element at specified position ([][] style).
/**
@param row The row index.
*/
	NConstRow operator[](size_type row)
	{
		// assert(row < N_rows() && "row index out of range");
		return NConstRow(*this, row);
	}

public: // linear storage
	typedef typename base_type::const_iterator const_iterator; ///< @brief The constant linear iterator type.
	typedef typename base_type::      iterator       iterator; ///< @brief The non-constant linear iterator type.

//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the storage.
	const_iterator begin() const
	{
		return m_buf.begin();
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the storage.
	iterator begin()
	{
		return m_buf.begin();
	}

//////////////////////////////////////////////////////////////////////////
/// @brief End of the storage.
	const_iterator end() const
	{
		return m_buf.end();
	}

//////////////////////////////////////////////////////////////////////////
/// @brief End of the storage.
	iterator end()
	{
		return m_buf.end();
	}

public: // row iterators
	#if OMNI_MATRIX_ROW_MAJOR
		typedef details::Iterator1<const_iterator> row_const_iterator; ///< @brief The constant row iterator type.
		typedef details::Iterator1<iterator> row_iterator; ///< @brief The non-constant row iterator type.
	#else
		typedef details::IteratorN<const_iterator> row_const_iterator; ///< @brief The constant row iterator type.
		typedef details::IteratorN<iterator> row_iterator; ///< @brief The constant row iterator type.
	#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the specified row.
/**
@param row The row index.
@return The constant row iterator.
*/
	row_const_iterator row_begin(size_type row) const
	{
		assert(row < m_N_rows && "row index out of range");

	#if OMNI_MATRIX_ROW_MAJOR
		return row_const_iterator(m_buf.begin() + row*m_N_cols);
	#else
		return row_const_iterator(&m_buf[0] + row, m_N_rows);
	#endif
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the specified row.
/**
@param row The row index.
@return The non-constant row iterator.
*/
	row_iterator row_begin(size_type row)
	{
		assert(row < m_N_rows && "row index out of range");

	#if OMNI_MATRIX_ROW_MAJOR
		return row_iterator(m_buf.begin() + row*m_N_cols);
	#else
		return row_iterator(&m_buf[0] + row, m_N_rows);
	#endif
	}


//////////////////////////////////////////////////////////////////////////
/// @brief End of the specified row.
/**
@param row The row index.
@return The constant row iterator.
*/
	row_const_iterator row_end(size_type row) const
	{
		assert(row < m_N_rows && "row index out of range");

	#if OMNI_MATRIX_ROW_MAJOR
		return row_const_iterator(m_buf.begin() + (row+1)*m_N_cols);
	#else
		return row_const_iterator(&m_buf[0] + m_buf.size() + row, m_N_rows);
	#endif
	}


//////////////////////////////////////////////////////////////////////////
/// @brief End of the specified row.
/**
@param row The row index.
@return The non-constant row iterator.
*/
	row_iterator row_end(size_type row)
	{
		assert(row < m_N_rows && "row index out of range");

	#if OMNI_MATRIX_ROW_MAJOR
		return row_iterator(m_buf.begin() + (row+1)*m_N_cols);
	#else
		return row_iterator(&m_buf[0] + m_buf.size() + row, m_N_rows);
	#endif
	}

public: // column iterators
#if OMNI_MATRIX_ROW_MAJOR
	typedef details::IteratorN<const_iterator> col_const_iterator; ///< @brief The constant column iterator type.
	typedef details::IteratorN<iterator> col_iterator; ///< @brief The non-constant column iterator type.
#else
	typedef details::Iterator1<const_iterator> col_const_iterator; ///< @brief The constant column iterator type.
	typedef details::Iterator1<iterator> col_iterator; ///< @brief The non-constant column iterator type.
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the specified column.
/**
@param col The column index.
@return The constant column iterator.
*/
	col_const_iterator col_begin(size_type col) const
	{
		assert(col < m_N_cols && "column index out of range");

	#if OMNI_MATRIX_ROW_MAJOR
		return col_const_iterator(&m_buf[0] + col, m_N_cols);
	#else
		return col_const_iterator(m_buf.begin() + col*m_N_rows);
	#endif
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Begin of the specified column.
/**
@param col The column index.
@return The non-constant column iterator.
*/
	col_iterator col_begin(size_type col)
	{
		assert(col < m_N_cols && "column index out of range");

	#if OMNI_MATRIX_ROW_MAJOR
		return col_iterator(&m_buf[0] + col, m_N_cols);
	#else
		return col_iterator(m_buf.begin() + col*m_N_rows);
	#endif
	}


//////////////////////////////////////////////////////////////////////////
/// @brief End of the specified column.
/**
@param col The column index.
@return The constant column iterator.
*/
	col_const_iterator col_end(size_type col) const
	{
		assert(col < m_N_cols && "column index out of range");

	#if OMNI_MATRIX_ROW_MAJOR
		return col_const_iterator(&m_buf[0] + m_buf.size() + col, m_N_cols);
	#else
		return col_const_iterator(m_buf.begin() + (col+1)*m_N_rows);
	#endif
	}


//////////////////////////////////////////////////////////////////////////
/// @brief End of the specified column.
/**
@param col The column index.
@return The non-constant column iterator.
*/
	col_iterator col_end(size_type col)
	{
		assert(col < m_N_cols && "column index out of range");

	#if OMNI_MATRIX_ROW_MAJOR
		return col_iterator(&m_buf[0] + m_buf.size() + col, m_N_cols);
	#else
		return col_iterator(m_buf.begin() + (col+1)*m_N_rows);
	#endif
	}

public: // swap method

//////////////////////////////////////////////////////////////////////////
/// @brief Swap the matrix content.
/**
@param other The other matrix reference.
*/
	void swap(this_type &other)
	{
		m_buf.swap(other.m_buf);
		std::swap(m_N_rows, other.m_N_rows);
		std::swap(m_N_cols, other.m_N_cols);
	}

private:
	base_type m_buf;    ///< @brief The matrix content.
	size_type m_N_rows; ///< @brief The number of rows.
	size_type m_N_cols; ///< @brief The number of columns.
};

	} // Matrix


	// auxiliary functions
	namespace mx
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Swap two matrices.
/** @relates Matrix
@param x The first matrix.
@param y The second matrix.
*/
template<typename T, typename A> inline
void swap(Matrix<T,A> &x, Matrix<T,A> &y)
{
	x.swap(y);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Are two matrices equal?
/** @relates Matrix
@param x The first matrix.
@param y The second matrix.
@return @b true if the first matrix is equal to the second matrix, otherwise @b false.
*/
template<typename TX, typename AX, typename TY, typename AY>
bool operator==(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
{
	if (x.Nrows() != y.Nrows())
		return false;
	if (x.Ncols() != y.Ncols())
		return false;

	return std::equal(x.begin(),
		x.end(), y.begin());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Are two matrices non-equal?
/** @relates Matrix
@param x The first matrix.
@param y The second matrix.
@return @b true if the first matrix is differ from the second matrix, otherwise @b false.
*/
template<typename TX, typename AX, typename TY, typename AY> inline
bool operator!=(const Matrix<TX,AX> &x, const Matrix<TY,AY> &y)
{
	return !(x == y);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is the matrix empty?
/** @relates Matrix

		The empty matrix has no rows or has no columns,
	i.e. number of rows is equal to zero or number
	of columns is equal to zero.

@param x The matrix.
@return @b true if the matrix is empty, otherwise @b false.
*/
template<typename T, typename A> inline
bool is_empty(const Matrix<T,A> &x)
{
	return !x.Nrows() || !x.Ncols();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is the matrix square?
/** @relates Matrix

		For the square matrix the number of rows
	is the same as the number of columns.

@param x The matrix.
@return @b true if the matrix is square, otherwise @b false.
*/
template<typename T, typename A> inline
bool is_square(const Matrix<T,A> &x)
{
	return x.Nrows() == x.Ncols();
}

	} // auxiliary functions


	namespace mx
	{
		// ConstRow, NConstRow
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief The matrix constant row wrapper.
template<typename T, typename A>
class ConstRow: private omni::NonCopyable {
	typedef ConstRow<T,A> this_type;

public:
	typedef Matrix<T,A> owner_type; ///< @brief The matrix type.
	typedef typename owner_type::const_reference reference; ///< @brief The element reference type.
	typedef typename owner_type::size_type size_type; ///< @brief The size type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param ref The matrix reference.
@param row The row index.
*/
	ConstRow(const owner_type &ref, size_type row)
		: m_ref(ref), m_row(row)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param other The other matrix row wrapper.
*/
	ConstRow(const this_type &other)
		: m_ref(other.m_ref),
		  m_row(other.m_row)
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get the row element.
/**
@param col The column index.
@return The element reference.
*/
	reference operator[](size_type col) const
	{
		return m_ref.at(m_row, col);
	}

private:
	const owner_type &m_ref; ///< @brief The matrix reference.
	size_type         m_row; ///< @brief The row index.
};


//////////////////////////////////////////////////////////////////////////
/// @brief The matrix non-constant row wrapper.
template<typename T, typename A>
class NConstRow: private omni::NonCopyable {
	typedef NConstRow<T,A> this_type;

public:
	typedef Matrix<T,A> owner_type; ///< @brief The matrix type.
	typedef typename owner_type::reference reference; ///< @brief The element reference type.
	typedef typename owner_type::size_type size_type; ///< @brief The size type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param ref The matrix reference.
@param row The row index.
*/
	NConstRow(owner_type &ref, size_type row)
		: m_ref(ref), m_row(row)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The copy constructor.
/**
@param other The other matrix row wrapper.
*/
	NConstRow(const this_type &other)
		: m_ref(other.m_ref),
		  m_row(other.m_row)
	{}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get the row element.
/**
@param col The column index.
@return The element reference.
*/
	reference operator[](size_type col) const
	{
		return m_ref.at(m_row, col);
	}

private:
	owner_type &m_ref; ///< @brief The matrix reference.
	size_type   m_row; ///< @brief The row index.
};

		} // Row


		// Iterator1
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief The matrix step-1 iterator.
/**
		This iterator is the same as the Base iterator.
*/
template<typename Base>
class Iterator1
{
	typedef Iterator1<Base> this_type;
	typedef Base base_type;

public: // typedefs
	typedef typename std::iterator_traits<base_type>::iterator_category iterator_category; ///< @brief The iterator category.
	typedef typename std::iterator_traits<base_type>::difference_type difference_type; ///< @brief The difference type.
	typedef typename std::iterator_traits<base_type>::value_type value_type;  ///< @brief The value type.
	typedef typename std::iterator_traits<base_type>::reference reference;    ///< @brief The reference type.
	typedef typename std::iterator_traits<base_type>::pointer pointer;        ///< @brief The pointer type.

public: // constructors

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
	Iterator1()
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param it_base The base iterator.
*/
	explicit Iterator1(const base_type &it_base)
		: m_base(it_base)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The auxiliary copy constructor.
/**
@param other The other iterator.
*/
	template<typename Base2>
	Iterator1(const Iterator1<Base2> &other)
		: m_base(other.base())
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The auxiliary assignment operator.
/**
@param other The other iterator.
@return The self reference.
*/
	template<typename Base2>
	this_type& operator=(const Iterator1<Base2> &other)
	{
		m_base = other.base();
		return *this;
	}

public: // access

//////////////////////////////////////////////////////////////////////////
/// @brief Get element at specified index.
/**
@param i The element index.
@return The element reference.
*/
	reference operator[](difference_type i) const
	{
		return m_base[i];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Dereference.
/**
@return The element reference.
*/
	reference operator*() const
	{
		return (*m_base);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Dereference.
/**
@return The element pointer.
*/
	pointer operator->() const
	{
		return &(*m_base);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the base iterator.
/**
@return The base iterator.
*/
	const base_type& base() const
	{
		return m_base;
	}


public: // Increment and decrement

//////////////////////////////////////////////////////////////////////////
/// @brief Prefix increment.
/**
@return The self reference.
*/
	this_type& operator++()
	{
		++m_base;
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Prefix decrement.
/**
@return The self reference.
*/
	this_type& operator--()
	{
		--m_base;
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Postfix increment.
/**
@return The incremented iterator.
*/
	this_type operator++(int)
	{
		return this_type(m_base++);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Postfix decrement.
/**
@return The decremented iterator.
*/
	this_type operator--(int)
	{
		return this_type(m_base--);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Increment.
/**
@param d The increment distance.
@return The self reference.
*/
	this_type& operator+=(difference_type d)
	{
		std::advance(m_base, +d);
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrement.
/**
@param d The decrement distance.
@return The self reference.
*/
	this_type& operator-=(difference_type d)
	{
		std::advance(m_base, -d);
		return *this;
	}

private:
	base_type m_base; ///< @brief The base iterator.
};


//////////////////////////////////////////////////////////////////////////
/// @brief Are two iterators equal?
/** @relates Iterator1
@param x The first iterator.
@param y The second iterator.
@return @b true If two iterators are equal, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator==(const Iterator1<Base1> &x, const Iterator1<Base2> &y)
{
	return x.base() == y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Are two iterators non-equal?
/** @relates Iterator1
@param x The first iterator.
@param y The second iterator.
@return @b true If two iterators are non-equal, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator!=(const Iterator1<Base1> &x, const Iterator1<Base2> &y)
{
	return x.base() != y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator less than second?
/** @relates Iterator1
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is less than second iterator, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator<(const Iterator1<Base1> &x, const Iterator1<Base2> &y)
{
	return x.base() < y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator greater than second?
/** @relates Iterator1
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is greater than second iterator, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator>(const Iterator1<Base1> &x, const Iterator1<Base2> &y)
{
	return x.base() > y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator less than or equal to second?
/** @relates Iterator1
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is less than or equal to second iterator, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator<=(const Iterator1<Base1> &x, const Iterator1<Base2> &y)
{
	return x.base() <= y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator greater than or equal to second?
/** @relates Iterator1
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is greater than or equal to second iterator, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator>=(const Iterator1<Base1> &x, const Iterator1<Base2> &y)
{
	return x.base() >= y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increment.
/** @relates Iterator1
@param x The iterator.
@param d The increment distance.
@return The incremented iterator.
*/
template<typename Base> inline
const Iterator1<Base> operator+(const Iterator1<Base> &x, typename Iterator1<Base>::difference_type d)
{
	return Iterator1<Base>(x.base() + d);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increment.
/** @relates Iterator1
@param d The increment distance.
@param x The iterator.
@return The incremented iterator.
*/
template<typename Base> inline
const Iterator1<Base> operator+(typename Iterator1<Base>::difference_type d, const Iterator1<Base> &x)
{
	return Iterator1<Base>(d + x.base());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrement.
/** @relates Iterator1
@param x The iterator.
@param d The decrement distance.
@return The decremented iterator.
*/
template<typename Base> inline
const Iterator1<Base> operator-(const Iterator1<Base> &x, typename Iterator1<Base>::difference_type d)
{
	return Iterator1<Base>(x.base() - d);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get distance between two iterators.
/** @relates Iterator1
@param x The first iterator.
@param y The second iterator.
@return The distance between two iterators.
*/
template<typename Base> inline
typename Iterator1<Base>::difference_type operator-(const Iterator1<Base> &x, const Iterator1<Base> &y)
{
	return x.base() - y.base();
}

		} // Iterator1


		// IteratorN
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief The matrix step-N iterator.
/**
		This iterator uses the step as increment value.
*/
template<typename Base>
class IteratorN
{
	typedef IteratorN<Base> this_type;
	typedef Base base_type;

public: // typedefs
	typedef typename std::iterator_traits<base_type>::iterator_category iterator_category; ///< @brief The iterator category.
	typedef typename std::iterator_traits<base_type>::difference_type difference_type; ///< @brief The difference type.
	typedef typename std::iterator_traits<base_type>::value_type value_type;  ///< @brief The value type.
	typedef typename std::iterator_traits<base_type>::reference reference;    ///< @brief The reference type.
	typedef typename std::iterator_traits<base_type>::pointer pointer;        ///< @brief The pointer type.

	typedef pointer base_type_; ///< @brief The real base type.

public: // constructors

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
	IteratorN()
		: m_base(0),
		  m_step(0)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param it_base The base iterator.
@param is_step The iterator step.
@param offset The initial iterator offset.
*/
	//IteratorN(const base_type &it_base, difference_type it_step, difference_type offset)
	//	: m_base(&*it_base + offset), m_step(it_step)
	//{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param it_base The base iterator.
@param is_step The iterator step.
*/
	IteratorN(const base_type_ &it_base, difference_type it_step)
		: m_base(it_base), m_step(it_step)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The auxiliary copy constructor.
/**
@param other The other iterator.
*/
	template<typename Base2>
	IteratorN(const IteratorN<Base2> &other)
		: m_base(other.base()), m_step(other.step())
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The auxiliary assignment operator.
/**
@param other The other iterator.
@return The self reference.
*/
	template<typename Base2>
	this_type& operator=(const IteratorN<Base2> &other)
	{
		m_base = other.base();
		m_step = other.step();
		return *this;
	}

public: // access

//////////////////////////////////////////////////////////////////////////
/// @brief Get element at specified index.
/**
@param i The element index.
@return The element reference.
*/
	reference operator[](difference_type i) const
	{
		return m_base[i*m_step];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Dereference.
/**
@return The element reference.
*/
	reference operator*() const
	{
		return (*m_base);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Dereference.
/**
@return The element pointer.
*/
	pointer operator->() const
	{
		return &(*m_base);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the base iterator.
/**
@return The base iterator.
*/
	const base_type_& base() const
	{
		return m_base;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the iterator step.
/**
@return The iterator step.
*/
	difference_type step() const
	{
		return m_step;
	}


public: // Increment and decrement

//////////////////////////////////////////////////////////////////////////
/// @brief Prefix increment.
/**
@return The self reference.
*/
	this_type& operator++()
	{
		std::advance(m_base, +m_step);
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Prefix decrement.
/**
@return The self reference.
*/
	this_type& operator--()
	{
		std::advance(m_base, -m_step);
		return *this;
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Postfix increment.
/**
@return The incremented iterator.
*/
	this_type operator++(int)
	{
		this_type t(*this);
		++(*this);
		return t;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Postfix decrement.
/**
@return The decremented iterator.
*/
	this_type operator--(int)
	{
		this_type t(*this);
		--(*this);
		return t;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Increment.
/**
@param d The increment distance.
@return The self reference.
*/
	this_type& operator+=(difference_type d)
	{
		std::advance(m_base, +d*m_step);
		return *this;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrement.
/**
@param d The decrement distance.
@return The self reference.
*/
	this_type& operator-=(difference_type d)
	{
		std::advance(m_base, -d*m_step);
		return *this;
	}

private:
	base_type_      m_base; ///< @brief The base iterator.
	difference_type m_step; ///< @brief The iterator step.
};


//////////////////////////////////////////////////////////////////////////
/// @brief Are two iterators equal?
/** @relates IteratorN
@param x The first iterator.
@param y The second iterator.
@return @b true If two iterators are equal, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator==(const IteratorN<Base1> &x, const IteratorN<Base2> &y)
{
	assert(x.step() == y.step());
	return x.base() == y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Are two iterators non-equal?
/** @relates IteratorN
@param x The first iterator.
@param y The second iterator.
@return @b true If two iterators are non-equal, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator!=(const IteratorN<Base1> &x, const IteratorN<Base2> &y)
{
	assert(x.step() == y.step());
	return x.base() != y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator less than second?
/** @relates IteratorN
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is less than second iterator, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator<(const IteratorN<Base1> &x, const IteratorN<Base2> &y)
{
	assert(x.step() == y.step());
	return x.base() < y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator greater than second?
/** @relates IteratorN
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is greater than second iterator, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator>(const IteratorN<Base1> &x, const IteratorN<Base2> &y)
{
	assert(x.step() == y.step());
	return x.base() > y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator less than or equal to second?
/** @relates IteratorN
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is less than or equal to second iterator, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator<=(const IteratorN<Base1> &x, const IteratorN<Base2> &y)
{
	assert(x.step() == y.step());
	return x.base() <= y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Is first iterator greater than or equal to second?
/** @relates IteratorN
@param x The first iterator.
@param y The second iterator.
@return @b true If first iterator is greater than or equal to second iterator, otherwise @b false.
*/
template<typename Base1, typename Base2> inline
bool operator>=(const IteratorN<Base1> &x, const IteratorN<Base2> &y)
{
	assert(x.step() == y.step());
	return x.base() >= y.base();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increment.
/** @relates IteratorN
@param x The iterator.
@param d The increment distance.
@return The incremented iterator.
*/
template<typename Base> inline
const IteratorN<Base> operator+(const IteratorN<Base> &x, typename IteratorN<Base>::difference_type d)
{
	return IteratorN<Base>(x.base() + d*x.step(), x.step());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Increment.
/** @relates IteratorN
@param d The increment distance.
@param x The iterator.
@return The incremented iterator.
*/
template<typename Base> inline
const IteratorN<Base> operator+(typename IteratorN<Base>::difference_type d, const IteratorN<Base> &x)
{
	return IteratorN<Base>(d*x.step() + x.base(), x.step());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Decrement.
/** @relates IteratorN
@param x The iterator.
@param d The decrement distance.
@return The decremented iterator.
*/
template<typename Base> inline
const IteratorN<Base> operator-(const IteratorN<Base> &x, typename IteratorN<Base>::difference_type d)
{
	return IteratorN<Base>(x.base() - d*x.step(), x.step());
}


//////////////////////////////////////////////////////////////////////////
/// @brief Get distance between two iterators.
/** @relates IteratorN
@param x The first iterator.
@param y The second iterator.
@return The distance between two iterators.
*/
template<typename Base> inline
typename IteratorN<Base>::difference_type operator-(const IteratorN<Base> &x, const IteratorN<Base> &y)
{
	assert(x.step() == y.step());
	return (x.base() - y.base()) / x.step();
}

		} // IteratorN
	} // mx namespace
} // omni namespace

#endif // __OMNI_MATRIX_HPP_
