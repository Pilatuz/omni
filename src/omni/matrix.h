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
	@brief Матрицы.

		Файл содержит реализацию класса omni::mx::matrix.

@author Сергей Поличной
*/
#ifndef __OMNI_MATRIX_H_
#define __OMNI_MATRIX_H_

#include <omni/defs.hpp>
#include <omni/pool.hpp>

#include <algorithm>
#include <iterator>
#include <memory>

#include <assert.h>

namespace omni
{
	namespace mx
	{

// row-major or column-major
#if !defined(OMNI_MATRIX_ROW_MAJOR)
#	define OMNI_MATRIX_ROW_MAJOR 0 // default column-major
#endif

		namespace details
		{
			// pre-declarations
			template<typename T>
				class XBuffer;

			template<typename T>
				class matrix_base;

			template<typename MxRef, typename Ref>
				class RowWrap;

			template<typename It, typename Val,
				typename Ref, typename Ptr>
					class Iterator1;
			template<typename It, typename Val,
				typename Ref, typename Ptr>
					class IteratorN;

			template<typename OP, typename TX>
				class UnaryExpr;
			template<typename OP, typename TX, typename TY>
				class BinaryExpr;
			template<typename OP>
				class Evaluator;
		}

		// pre-declarations
		template<typename T>
			class scalar;
		template<typename T>
			class matrix;


//////////////////////////////////////////////////////////////////////////
/// @brief Матрица.
	template<typename T>
		class matrix: private details::matrix_base<T> {
			typedef details::matrix_base<T> inherited;
			typedef matrix<T> this_type;

			using inherited::xbuf;

		public: // typedefs
			typedef typename inherited::value_type value_type; ///< @brief Matrix value type.
			typedef typename inherited::size_type size_type;   ///< @brief Size and index type.

			typedef typename inherited::const_reference const_reference; ///< @brief Constant value reference.
			typedef typename inherited::      reference       reference; ///< @brief Value reference.
			typedef typename inherited::const_pointer const_pointer;
			typedef typename inherited::      pointer       pointer;

		public: /// @name Конструкторы
			matrix() {}
			matrix(size_type Nrows, size_type Ncols)
				: inherited(Nrows, Ncols)
			{
				xbuf().construct_fill_n(Nrows*Ncols, value_type());
			}
			template<typename U>
				matrix(size_type Nrows, size_type Ncols, const U &val)
					: inherited(Nrows, Ncols)
			{
				xbuf().construct_fill_n(Nrows*Ncols, val);
			}
			template<typename U> // auxiliary copy constructor
				matrix(const matrix<U> &x)
					: inherited(x.N_rows(), x.N_cols())
			{
				xbuf().construct_copy(x.RAW_begin(),
					x.RAW_end(), RAW_begin());
			}
			template<typename OP, typename TX>
				matrix(const details::UnaryExpr<OP, TX> &x)
			{
				x(*this);
			}
			template<typename OP, typename TX, typename TY>
				matrix(const details::BinaryExpr<OP, TX, TY> &x)
			{
				x(*this);
			}

			template<typename U> // auxiliary assignment operator
				this_type& operator=(const matrix<U> &x)
			{
				assign(x.N_rows(), x.N_cols(),
					x.RAW_begin(), x.RAW_end());
				return *this;
			}
			template<typename OP, typename TX>
				this_type& operator=(const details::UnaryExpr<OP, TX> &x)
			{
				x(*this);
				return *this;
			}
			template<typename OP, typename TX, typename TY>
				this_type& operator=(const details::BinaryExpr<OP, TX, TY> &x)
			{
				x(*this);
				return *this;
			}

		public: // size, resize and assign
			size_type N_rows() const { return inherited::N_rows(); }
			size_type N_cols() const { return inherited::N_cols(); }

			void resize(size_type Nrows, size_type Ncols)
			{
				inherited::resize(Nrows, Ncols, value_type());
			}

			template<typename In>
				void assign(size_type Nrows, size_type Ncols, In first, In last)
			{
				assert(Nrows*Ncols == size_type(std::distance(first, last))
					&& "invalid input sequence size");
				inherited::assign(Nrows, Ncols, first, last);
			}

			template<typename U>
				void assign(size_type Nrows, size_type Ncols, const U &val)
			{
				inherited::assign(Nrows, Ncols, val);
			}

			template<typename In>
				void assign(In first, In last)
			{
				assert(xbuf().size() == size_type(std::distance(first, last))
					&& "invalid input sequence size");
				std::copy(first, last, RAW_begin());
			}

			template<typename U>
				void assign(const U &val)
			{
				std::fill(RAW_begin(),
					RAW_end(), val);
			}

		public: // matrix access
			typedef details::RowWrap<const this_type&, const_reference> const_row_wrap; ///< @brief Constant row wrap
			typedef details::RowWrap<      this_type&,       reference>       row_wrap; ///< @brief Row wrap

			// get element: at
			const_reference at(size_type row, size_type col) const
			{
				assert(row < N_rows() && "index out of range");
				assert(col < N_cols() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return RAW_begin()[row*N_cols() + col];
			#else
				return RAW_begin()[col*N_rows() + row];
			#endif
			}

			// get element: at
			reference at(size_type row, size_type col)
			{
				assert(row < N_rows() && "index out of range");
				assert(col < N_cols() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return RAW_begin()[row*N_cols() + col];
			#else
				return RAW_begin()[col*N_rows() + row];
			#endif
			}

			// get element: ()
			const_reference operator()(size_type row, size_type col) const { return at(row, col); }
			      reference operator()(size_type row, size_type col)       { return at(row, col); }

			// get element: [][] style
			const_row_wrap operator[](size_type row) const
			{
				// assert(row < N_rows() && "index out of range");
				return const_row_wrap(*this, row);
			}

			// get element: [][] style
			row_wrap operator[](size_type row)
			{
				// assert(row < N_rows() && "index out of range");
				return row_wrap(*this, row);
			}

		public: // RAW storage
			typedef typename inherited::const_pointer const_RAW_iterator;
			typedef typename inherited::      pointer       RAW_iterator;

			const_RAW_iterator RAW_begin() const { return inherited::RAW_begin(); }
			      RAW_iterator RAW_begin()       { return inherited::RAW_begin(); }
			const_RAW_iterator RAW_end() const { return inherited::RAW_end(); }
			      RAW_iterator RAW_end()       { return inherited::RAW_end(); }

		public: // row iterators
		#if OMNI_MATRIX_ROW_MAJOR
			typedef details::Iterator1<const_pointer, value_type,
				const_reference, const_pointer> const_row_iterator;
			typedef details::Iterator1<pointer, value_type,
				reference, pointer> row_iterator;
		#else
			typedef details::IteratorN<const_pointer, value_type,
				const_reference, const_pointer> const_row_iterator;
			typedef details::IteratorN<pointer, value_type,
				reference, pointer> row_iterator;
		#endif

			const_row_iterator row_begin(size_type row) const
			{
				assert(row < N_rows() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return const_row_iterator(RAW_begin() + row*N_cols());
			#else
				return const_row_iterator(RAW_begin() + row, N_rows());
			#endif
			}

			row_iterator row_begin(size_type row)
			{
				assert(row < N_rows() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return row_iterator(RAW_begin() + row*N_cols());
			#else
				return row_iterator(RAW_begin() + row, N_rows());
			#endif
			}

			const_row_iterator row_end(size_type row) const
			{
				assert(row < N_rows() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return const_row_iterator(RAW_begin() + (row+1)*N_cols());
			#else
				return const_row_iterator(RAW_end() + row, N_rows());
			#endif
			}

			row_iterator row_end(size_type row)
			{
				assert(row < N_rows() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return row_iterator(RAW_begin() + (row+1)*N_cols());
			#else
				return row_iterator(RAW_end() + row, N_rows());
			#endif
			}

		public: // column iterators
		#if OMNI_MATRIX_ROW_MAJOR
			typedef details::IteratorN<const_pointer, value_type,
				const_reference, const_pointer> const_col_iterator;
			typedef details::IteratorN<pointer, value_type,
				reference, pointer> col_iterator;
		#else
			typedef details::Iterator1<const_pointer, value_type,
				const_reference, const_pointer> const_col_iterator;
			typedef details::Iterator1<pointer, value_type,
				reference, pointer> col_iterator;
		#endif

			const_col_iterator col_begin(size_type col) const
			{
				assert(col < N_cols() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return const_col_iterator(RAW_begin() + col, N_cols());
			#else
				return const_col_iterator(RAW_begin() + col*N_rows());
			#endif
			}

			col_iterator col_begin(size_type col)
			{
				assert(col < N_cols() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return col_iterator(RAW_begin() + col, N_cols());
			#else
				return col_iterator(RAW_begin() + col*N_rows());
			#endif
			}

			const_col_iterator col_end(size_type col) const
			{
				assert(col < N_cols() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return const_col_iterator(RAW_end() + col, N_cols());
			#else
				return const_col_iterator(RAW_begin() + (col+1)*N_rows());
			#endif
			}

			col_iterator col_end(size_type col)
			{
				assert(col < N_cols() && "index out of range");

			#if OMNI_MATRIX_ROW_MAJOR
				return col_iterator(RAW_end() + col, N_cols());
			#else
				return col_iterator(RAW_begin() + (col+1)*N_rows());
			#endif
			}

		public: // swap method
			void swap(this_type &x)
			{
				inherited::swap(x);
			}
		};

	// swap two matrices
	template<typename T> inline
		void swap(matrix<T> &x, matrix<T> &y)
	{
		x.swap(y);
	}

	// equal operator
	template<typename TX, typename TY>
		bool operator==(const matrix<TX> &x, const matrix<TY> &y)
	{
		if (x.N_rows() != y.N_rows())
			return false;
		if (x.N_cols() != y.N_cols())
			return false;

		return std::equal(x.RAW_begin(),
			x.RAW_end(), y.RAW_begin());
	}

	// non-equal operator
	template<typename TX, typename TY> inline
		bool operator!=(const matrix<TX> &x, const matrix<TY> &y)
	{
		return !(x == y);
	}


	//// save to output stream
	//template<typename T, typename Ch, typename ChTr>
	//	std::basic_ostream<Ch, ChTr>& operator<<(std::basic_ostream<Ch, ChTr> &os, const matrix<T> &x)
	//{
	//	os << x.N_rows(); os.putc(' ');
	//	os << x.N_cols(); os.putc(' ');

	//	matrix<T>::const_RAW_iterator xi = x.RAW_begin();
	//	matrix<T>::const_RAW_iterator xe = x.RAW_end();
	//	for (; xi != xe; ++xi)
	//	{
	//		os << (*xi);
	//		os.putc(' ');
	//	}

	//	return os;
	//}

	//// load from input stream
	//template<typename T, typename Ch, typename ChTr>
	//	std::basic_istream<Ch, ChTr>& operator>>(std::basic_istream<Ch, ChTr> &is, matrix<T> &x)
	//{
	//	size_t Nrows = 0, Ncols = 0;
	//	is >> Nrows >> Ncols;

	//	x.resize(Nrows, Ncols);

	//	matrix<T>::RAW_iterator xi = x.RAW_begin();
	//	matrix<T>::RAW_iterator xe = x.RAW_end();
	//	for (; xi != xe; ++xi)
	//		is >> (*xi);

	//	return is;
	//}


//////////////////////////////////////////////////////////////////////////
// scalar class
	template<typename T>
		class scalar {
			typedef scalar<T> this_type;

		public:
			typedef T value_type;

			typedef const T& const_reference;
			typedef       T&       reference;

		public:
			scalar(): m_val() {}
			explicit scalar(const_reference x): m_val(x) {}
			this_type& operator=(const_reference x)
			{
				m_val = x;
				return *this;
			}

		public:
			const_reference val() const { return m_val; }
			      reference val()       { return m_val; }
			// operator const_reference() { return val(); }

		private:
			T m_val;
		};

	// create custom scalar
	template<typename T> inline
		scalar<T> make_scalar(const T &x)
	{
		return scalar<T>(x);
	}



namespace details
{

//////////////////////////////////////////////////////////////////////////
// @brief Контейнер для хранения элементов матрицы - аналог vector
template<typename T>
	class XBuffer: private omni::NonCopyable {
		typedef XBuffer<T> this_type;

	public:
		typedef size_t size_type;
		typedef T value_type;
		typedef T* pointer;

	public:
		XBuffer(): m_first(0),
			m_last(0), m_end(0)
		{}

		explicit XBuffer(size_type n)
		{
			if (0 < n)
			{
				m_first = static_cast<pointer>(omni::pool::mem_get(n*sizeof(value_type)));
				m_last = m_first;    // memory will be initialized later
				m_end = m_first + n; // reserved memory
			}
			else
			{
				m_first = 0;
				m_last = 0;
				m_end = 0;
			}
		}

		~XBuffer()
		{
			if (m_first)
			{
				omni::pool::mem_put(m_first,
					capacity()*sizeof(value_type));
			}
		}

	public:
		size_type capacity() const { return m_end - m_first; }
		size_type size() const { return m_last - m_first; }

		pointer begin() const { return m_first; }
		pointer end() const { return m_last; }

	public: // utility
		template<typename U> // @brief Создать копии элемента
			void construct_fill_n(size_type n, const U &val)
			{
				std::uninitialized_fill_n(begin(), n, val);
				m_last = m_first + n;
			}

		template<typename U> // @brief Создать копии элемента
			void construct_fill(pointer last, const U &val)
			{
				std::uninitialized_fill(end(), last, val);
				m_last = last;
			}

		template<typename In> // @brief Создать копии элементов
			void construct_copy(In first, In last, pointer where)
			{
				if (first != last) // avoid VC++8.0 checked iterators
					m_last = std::uninitialized_copy(first, last, where);
			}

		void destroy(pointer first) // @brief Удалить часть элементов
		{
			pointer last = m_last;
			m_last = first;

			for (; first != last; ++first)
				(*first).~T(); // destructor call
		}

		// @brief Обменять два контейнера местами
		void swap(this_type &x)
		{
			std::swap(m_first, x.m_first);
			std::swap(m_last,  x.m_last);
			std::swap(m_end,   x.m_end);
		}

	private:
		pointer m_first;
		pointer m_last;
		pointer m_end;
	};


//////////////////////////////////////////////////////////////////////////
// template class matrix_base
template<typename T>
	class matrix_base {
		typedef XBuffer<T> xbuffer_type;
		typedef matrix_base<T> this_type;

	protected:
		typedef typename xbuffer_type::size_type size_type;
		typedef typename xbuffer_type::value_type value_type;

		typedef const T& const_reference;
		typedef       T&       reference;
		typedef const T* const_pointer;
		typedef       T*       pointer;

	protected: // constructors and destructor
		matrix_base(): m_N_rows(0), m_N_cols(0) {}
		matrix_base(size_type Nrows, size_type Ncols)
			: m_xbuf(Nrows*Ncols),
			  m_N_rows(Nrows),
			  m_N_cols(Ncols)
		{} // (!) not initialized
		matrix_base(const this_type &x)
			: m_xbuf(x.xbuf().size()),
			  m_N_rows(x.N_rows()),
			  m_N_cols(x.N_cols())
		{
			xbuf().construct_copy(x.RAW_begin(),
				x.RAW_end(), RAW_begin());
		}
		~matrix_base()
		{
			xbuf().destroy(RAW_begin());
		}

		// assignment operator
		this_type& operator=(const this_type &x)
		{
			assign(x.N_rows(), x.N_cols(),
				x.RAW_begin(), x.RAW_end());
			return *this;
		}

	protected:
		// RAW storage buffer
		const xbuffer_type& xbuf() const { return m_xbuf; }
		      xbuffer_type& xbuf()       { return m_xbuf; }

		// RAW storage
		const_pointer RAW_begin() const { return xbuf().begin(); }
		      pointer RAW_begin()       { return xbuf().begin(); }
		const_pointer RAW_end() const { return xbuf().end(); }
		      pointer RAW_end()       { return xbuf().end(); }

		// number of rows and number of columns
		size_type N_rows() const { return m_N_rows; }
		size_type N_cols() const { return m_N_cols; }

	protected:
		// assign new matrix size (content undefined)
		template<typename U>
		void resize(size_type Nrows, size_type Ncols, const U &val)
		{
			const size_type xbuf_size = Nrows*Ncols;

			if (xbuf_size != xbuf().size())
			{
				if (xbuf_size < xbuf().size())
					xbuf().destroy(RAW_begin() + xbuf_size);
				else if (xbuf_size <= xbuf().capacity())
					xbuf().construct_fill(RAW_begin() + xbuf_size, val);
				else
				{
					xbuffer_type new_xbuf(xbuf_size);
					new_xbuf.construct_fill_n(xbuf_size, val);
					xbuf().swap(new_xbuf);
				}
			}

			m_N_rows = Nrows;
			m_N_cols = Ncols;
		}

		// assign new matrix size and initialize content
		template<typename In>
		void assign(size_type Nrows, size_type Ncols, In first, In last)
		{
			const size_type xbuf_size = Nrows*Ncols;

			if (xbuf_size < xbuf().size())
				xbuf().destroy(std::copy(first,
					last, RAW_begin()));
			else if (xbuf_size == xbuf().size())
				std::copy(first, last, RAW_begin());
			else if (xbuf_size <= xbuf().capacity())
			{
				In xtmp = first; std::advance(xtmp, xbuf().size());
				xbuf().construct_copy(xtmp, last,
					std::copy(first, xtmp,
						RAW_begin()));
			}
			else
			{
				xbuffer_type new_xbuf(xbuf_size);
				new_xbuf.construct_copy(first,
					last, new_xbuf.begin());
				xbuf().destroy(RAW_begin());
				xbuf().swap(new_xbuf);
			}

			m_N_rows = Nrows;
			m_N_cols = Ncols;
		}

		// assign new matrix size and initialize content
		template<typename U>
		void assign(size_type Nrows, size_type Ncols, const U &val)
		{
			const size_type xbuf_size = Nrows*Ncols;

			if (xbuf_size <= xbuf().size())
			{
				pointer tmp = RAW_begin() + xbuf_size;
				std::fill(RAW_begin(), tmp, val);
				xbuf().destroy(tmp);
			}
			else if (xbuf_size <= xbuf().capacity())
			{
				pointer tmp = RAW_begin() + xbuf_size;
				std::fill(RAW_begin(), RAW_end(), val);
				xbuf().construct_fill(tmp, val);
			}
			else
			{
				xbuffer_type new_xbuf(xbuf_size);
				new_xbuf.construct_fill_n(xbuf_size, val);
				xbuf().swap(new_xbuf);
			}

			m_N_rows = Nrows;
			m_N_cols = Ncols;
		}

		// swap matrix
		void swap(this_type &x)
		{
			std::swap(m_N_rows, x.m_N_rows);
			std::swap(m_N_cols, x.m_N_cols);
			xbuf().swap(x.xbuf());
		}

	private:
		xbuffer_type m_xbuf;
		size_type m_N_rows;
		size_type m_N_cols;
	};


//////////////////////////////////////////////////////////////////////////
// matrix ROW wrap template class
template<typename MxRef, typename Ref>
	class RowWrap: private omni::NonCopyable {
		typedef RowWrap<MxRef, Ref> this_type;

	public:
		typedef size_t size_type;
		typedef MxRef matrix_ref;
		typedef Ref reference;

	public:
		RowWrap(matrix_ref x, size_type row)
			: m_mx(x), m_i(row)
		{}

		RowWrap(const this_type &x)
			: m_mx(x.m_mx), m_i(x.m_i)
		{}

	public:
		reference operator[](size_type col) const
			{ return m_mx.at(m_i, col); }

	private:
		matrix_ref m_mx; // matrix reference
		size_type  m_i;  // row index
	};

//////////////////////////////////////////////////////////////////////////
// matrix iterator (1 step)
template<typename It, typename Val, typename Ref, typename Ptr>
	class Iterator1: public std::iterator<std::random_access_iterator_tag, Val> {
		typedef std::iterator<std::random_access_iterator_tag, Val> inherited;
		typedef Iterator1<It, Val, Ref, Ptr> this_type;
		typedef It base_type;

	public:
		typedef typename inherited::iterator_category iterator_category;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Val value_type;
		typedef Ref reference;
		typedef Ptr pointer;

	public:
		Iterator1() {}
		explicit Iterator1(const base_type &x)
			: m_base(x) {}

		template<typename It2, typename Val2, typename Ref2, typename Ptr2>
			Iterator1(const Iterator1<It2, Val2, Ref2, Ptr2> &x)
				: m_base(x.base()) {}
		template<typename It2, typename Val2, typename Ref2, typename Ptr2>
			this_type& operator=(const Iterator1<It2, Val2, Ref2, Ptr2> &x)
				{ base() = x.base(); return *this; }

	public:
		reference operator[](difference_type row) const { return m_base[row]; }
		reference operator*() const { return (*base()); }
		pointer operator->() const { return &(*base()); }

		this_type& operator++() { m_base += 1; return *this; }
		this_type& operator--() { m_base -= 1; return *this; }
		this_type operator++(int) { this_type t(*this); m_base += 1; return t; }
		this_type operator--(int) { this_type t(*this); m_base -= 1; return t; }

		this_type& operator+=(difference_type d) { m_base += d; return *this; }
		this_type& operator-=(difference_type d) { m_base -= d; return *this; }
		this_type operator+(difference_type d) const { this_type t(*this); return (t += d); }
		this_type operator-(difference_type d) const { this_type t(*this); return (t -= d); }
		difference_type operator-(const this_type &x) const { return (base() - x.base()); }

		bool operator==(const this_type &x) const { return base() == x.base(); }
		bool operator!=(const this_type &x) const { return base() != x.base(); }
		bool operator< (const this_type &x) const { return base() <  x.base(); }
		bool operator> (const this_type &x) const { return base() >  x.base(); }
		bool operator<=(const this_type &x) const { return base() <= x.base(); }
		bool operator>=(const this_type &x) const { return base() >= x.base(); }

	public:
		const base_type& base() const { return m_base; }

	private:
		base_type m_base;
	};

//////////////////////////////////////////////////////////////////////////
// matrix iterator (N step)
template<typename It, typename Val, typename Ref, typename Ptr>
	class IteratorN: public std::iterator<std::random_access_iterator_tag, Val> {
		typedef std::iterator<std::random_access_iterator_tag, Val> inherited;
		typedef IteratorN<It, Val, Ref, Ptr> this_type;
		typedef It base_type;

	public:
		typedef typename inherited::iterator_category iterator_category;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Val value_type;
		typedef Ref reference;
		typedef Ptr pointer;

	public:
		IteratorN(): m_step(0) {}
		IteratorN(const base_type &x, size_type Ncols)
			: m_base(x), m_step(Ncols) {}

		template<typename It2, typename Val2, typename Ref2, typename Ptr2>
			IteratorN(const IteratorN<It2, Val2, Ref2, Ptr2> &x)
				: m_base(x.base()), m_step(x.step()) {}
		template<typename It2, typename Val2, typename Ref2, typename Ptr2>
			this_type& operator=(const IteratorN<It2, Val2, Ref2, Ptr2> &x)
				{ base() = x.base(); m_step = x.step(); return *this; }

	public:
		reference operator[](difference_type row) const { return m_base[row*step()]; }
		reference operator*() const { return (*base()); }
		pointer operator->() const { return &(*base()); }

		this_type& operator++() { m_base += step(); return *this; }
		this_type& operator--() { m_base -= step(); return *this; }
		this_type operator++(int) { this_type t(*this); m_base += step(); return t; }
		this_type operator--(int) { this_type t(*this); m_base -= step(); return t; }

		this_type& operator+=(difference_type d) { m_base += d*step(); return *this; }
		this_type& operator-=(difference_type d) { m_base -= d*step(); return *this; }
		this_type operator+(difference_type d) const { this_type t(*this); return (t += d); }
		this_type operator-(difference_type d) const { this_type t(*this); return (t -= d); }
		difference_type operator-(const this_type &x) const { return (base() - x.base())/step(); }

		bool operator==(const this_type &x) const { return base() == x.base(); }
		bool operator!=(const this_type &x) const { return base() != x.base(); }
		bool operator< (const this_type &x) const { return base() <  x.base(); }
		bool operator> (const this_type &x) const { return base() >  x.base(); }
		bool operator<=(const this_type &x) const { return base() <= x.base(); }
		bool operator>=(const this_type &x) const { return base() >= x.base(); }

	public:
		const base_type& base() const { return m_base; }
		size_type step() const { return m_step; }

	private:
		base_type m_base;
		size_type m_step;
	};

} // details namespace

	} // mx namespace
} // omni namespace

#endif // __OMNI_MATRIX_H_
