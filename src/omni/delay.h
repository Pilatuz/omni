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
#ifndef __OMNI_DELAY_H_
#define __OMNI_DELAY_H_

#include <omni/defs.hpp>

#include <iterator>
#include <vector>

#include <assert.h>

namespace omni
{
	namespace dsp
	{
		namespace details
		{
			// DelayLine iterator
			template<typename T, typename Ref, typename Ptr>
				class DelayLine_Iterator;
		}

//////////////////////////////////////////////////////////////////////////
/// @brief Линия задержки
template<typename T>
	class DelayLine {
		typedef std::vector<T> basic_type;

	public: // @name Определения псевдонимов
		typedef typename basic_type::const_reference const_reference;    ///< @brief Константная ссылка на элемент
		typedef typename basic_type::reference             reference;    ///< @brief Ссылка на элемент
		typedef typename basic_type::value_type value_type;              ///< @brief Тип элемента
		typedef typename basic_type::size_type   size_type;              ///< @brief Тип индексов и размера

	public: /// @name Конструкторы
		DelayLine();                                 ///< @brief Создать линию задержки нулевого размера
		explicit DelayLine(size_type N);             ///< @brief Создать линию задержки заданного размера
		DelayLine(size_type N, const_reference x);   ///< @brief Создать линию задержки заданного размера и заполнить

	public: /// @name Размер и содержимое
		size_type size() const;                      ///< @brief Текущий размер линии задержки

		void resize(size_type N);                    ///< @brief Изменить размер линии задержки
		void resize(size_type N, const_reference x); ///< @brief Изменить размер линии задержки и заполнить

		void reset();                                ///< @brief Обнулить линию задержки
		void reset(const_reference x);               ///< @brief Заполнить линию задержки заданным значением

	public: // ()
		const_reference operator()(const_reference x); ///< @brief Поместить новый элемент в линию задержки

	public: /// @name Доступ к содержимому
		const_reference at(size_type i) const;       ///< @brief Константная ссылка на i-ый элемент
		      reference at(size_type i);             ///< @brief Ссылка на i-ый элемент

		const_reference operator[](size_type i) const; ///< @brief Константная ссылка на i-ый элемент
		      reference operator[](size_type i);       ///< @brief Ссылка на i-ый элемент

		const_reference out() const;                 ///< @brief Константная ссылка на последний "вытолкнутый" элемент
		      reference out();                       ///< @brief Ссылка на последний "вытолкнутый" элемент

		const_reference front() const;               ///< @brief Константная ссылка на первый элемент
		      reference front();                     ///< @brief Ссылка на первый элемент

		const_reference back() const;                ///< @brief Константная ссылка на последний элемент
		      reference back();                      ///< @brief Ссылка на последний элемент

		template<typename Out>
			void copy(Out first) const                 /// @brief Копировать содержимое линии задержки
		{
			const size_type N = m_xbuf.size();

			for (size_type i = m_wpos+1; i < N; ++i)
				{ *first = m_xbuf[i]; ++first; }
			for (size_type i = 0; i < m_wpos; ++i)
				{ *first = m_xbuf[i]; ++first; }
		}

	public: /// @name Итераторы
		typedef details::DelayLine_Iterator<const DelayLine,
			const_reference, const value_type*> const_iterator;  ///< @brief Константный итератор
		typedef details::DelayLine_Iterator<DelayLine,
			reference, value_type*> iterator;                    ///< @brief Итератор

		const_iterator begin() const;                ///< @brief Константный итератор на начало линии задержки
		      iterator begin();                      ///< @brief Итератор на начало линии задержки

		const_iterator end() const;                  ///< @brief Константный итератор за окончание линии задержки
		      iterator end();                        ///< @brief Итератор за окончание линии задержки

	public:

	private:
		basic_type m_xbuf;       ///< @brief Циркулярный буфер элементов
		size_type  m_wpos;       ///< @brief Позиция для записи
	};


//////////////////////////////////////////////////////////////////////////
// Iterator implementation...
		namespace details
		{

// DelayLine iterator
template<typename Containter, typename Ref, typename Ptr>
	class DelayLine_Iterator: public std::iterator<std::random_access_iterator_tag, typename Containter::value_type> {
		typedef std::iterator<std::random_access_iterator_tag, typename Containter::value_type> inherited;
		typedef DelayLine_Iterator<Containter, Ref, Ptr> this_type;
		typedef Containter container_type;

	public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Ref reference;
		typedef Ptr pointer;

	public:
		DelayLine_Iterator() {}
		DelayLine_Iterator(container_type *c, difference_type i)
			: m_buf(c), m_pos(i) {}

		template<typename C2, typename Ref2, typename Ptr2>
			DelayLine_Iterator(const DelayLine_Iterator<C2, Ref2, Ptr2> &x)
				: m_buf(x.__buf()), m_pos(x.__pos()) {}
		template<typename C2, typename Ref2, typename Ptr2>
			this_type& operator=(const DelayLine_Iterator<C2, Ref2, Ptr2> &x)
				{ m_buf = x.__buf(); m_pos = x.__pos(); return *this; }

	public:
			reference operator[](difference_type i) const { return (*m_buf)[m_pos + i]; }
			reference operator*() const { return (*m_buf)[m_pos]; }
			pointer operator->() const { return &(**this); }

	public:
		this_type& operator++() { ++m_pos; return *this; }
		this_type& operator--() { --m_pos; return *this; }
		this_type operator++(int) { this_type t(*this); ++m_pos; return t; }
		this_type operator--(int) { this_type t(*this); --m_pos; return t; }

		this_type& operator+=(difference_type d) { m_pos += d; return *this; }
		this_type& operator-=(difference_type d) { m_pos -= d; return *this; }
		this_type operator+(difference_type d) const { this_type t(*this); return (t += d); }
		this_type operator-(difference_type d) const { this_type t(*this); return (t -= d); }
		difference_type operator-(const this_type &x) const { return (m_pos - x.m_pos); }

		bool operator==(const this_type &x) const { return m_pos == x.m_pos; }
		bool operator!=(const this_type &x) const { return m_pos != x.m_pos; }
		bool operator< (const this_type &x) const { return m_pos <  x.m_pos; }
		bool operator> (const this_type &x) const { return m_pos >  x.m_pos; }
		bool operator<=(const this_type &x) const { return m_pos <= x.m_pos; }
		bool operator>=(const this_type &x) const { return m_pos >= x.m_pos; }

	public:
		container_type* __buf() const { return m_buf; }
		difference_type __pos() const { return m_pos; }

	private:
		container_type *m_buf;
		difference_type m_pos;
	};

		} // details namespace


//////////////////////////////////////////////////////////////////////////
// DelayLine implementation...

// default constructor
template<typename T>
	DelayLine<T>::DelayLine()
		: m_xbuf(1), m_wpos(0)
{}

// set size and fill by default value
template<typename T>
	DelayLine<T>::DelayLine(size_type N)
		: m_xbuf(N+1), m_wpos(N)
{}

// set size and fill by specified value
template<typename T>
	DelayLine<T>::DelayLine(size_type N, const_reference x)
		: m_xbuf(N+1, x), m_wpos(N)
{}

// get current size
template<typename T> inline
	typename DelayLine<T>::size_type DelayLine<T>::size() const
{
	return m_xbuf.size() - 1;
}

// resize and fill by default value
template<typename T> inline
	void DelayLine<T>::resize(size_type N)
{
	resize(N, value_type());
}

// resize and fill by specified value
template<typename T>
	void DelayLine<T>::resize(size_type N, const_reference x)
{
	m_xbuf.assign(N+1, x);
	m_wpos = N;
}

// fill by default value
template<typename T> inline
	void DelayLine<T>::reset()
{
	reset(value_type());
}

// fill by specified value
template<typename T>
	void DelayLine<T>::reset(const_reference x)
{
	const size_type N = m_xbuf.size();
	for (size_type i = 0; i < N; ++i)
		m_xbuf[i] = x;
	m_wpos = size();
}

// put specified value
template<typename T> inline
	typename DelayLine<T>::const_reference DelayLine<T>::operator()(const_reference x)
{
	m_xbuf[m_wpos] = x;

	if (0 == m_wpos)
		m_wpos = size();
	else
		--m_wpos;

	return m_xbuf[m_wpos];
}

// get element's const reference by index
template<typename T> inline
	typename DelayLine<T>::const_reference DelayLine<T>::at(size_type i) const
{
	assert(i<size() && "index out of range");

	size_type N = m_xbuf.size();
	size_type x = m_wpos + i+1;
	if (N <= x) x -= N;

	return m_xbuf[x];
}

// get element's reference by index
template<typename T> inline
	typename DelayLine<T>::reference DelayLine<T>::at(size_type i)
{
	assert(i<size() && "index out of range");

	size_type N = m_xbuf.size();
	size_type x = m_wpos + i+1;
	if (N <= x) x -= N;

	return m_xbuf[x];
}

// get element's const reference by index
template<typename T> inline
	typename DelayLine<T>::const_reference DelayLine<T>::operator[](size_type i) const
{
	return at(i);
}

// get element's reference by index
template<typename T> inline
	typename DelayLine<T>::reference DelayLine<T>::operator[](size_type i)
{
	return at(i);
}

// get last output element's const reference
template<typename T> inline
	typename DelayLine<T>::const_reference DelayLine<T>::out() const
{
	return m_xbuf[m_wpos];
}

// get last output element's reference
template<typename T> inline
	typename DelayLine<T>::reference DelayLine<T>::out()
{
	return m_xbuf[m_wpos];
}

// get first element's const reference
template<typename T> inline
	typename DelayLine<T>::const_reference DelayLine<T>::front() const
{
	return at(0);
}

// get first element's reference
template<typename T> inline
	typename DelayLine<T>::reference DelayLine<T>::front()
{
	return at(0);
}

// get last element's const reference
template<typename T> inline
	typename DelayLine<T>::const_reference DelayLine<T>::back() const
{
	return at(size()-1);
}

// get last element's reference
template<typename T> inline
	typename DelayLine<T>::reference DelayLine<T>::back()
{
	return at(size()-1);
}

// get first element's const iterator
template<typename T> inline
	typename DelayLine<T>::const_iterator DelayLine<T>::begin() const
{
	return const_iterator(this, 0);
}

// get first element's iterator
template<typename T> inline
	typename DelayLine<T>::iterator DelayLine<T>::begin()
{
	return iterator(this, 0);
}

// get end element's const iterator
template<typename T> inline
	typename DelayLine<T>::const_iterator DelayLine<T>::end() const
{
	return const_iterator(this, size());
}

// get end element's iterator
template<typename T> inline
	typename DelayLine<T>::iterator DelayLine<T>::end()
{
	return iterator(this, size());
}

	} // dsp namespace
} // omni namespace

#endif // __OMNI_DELAY_H_

//////////////////////////////////////////////////////////////////////////
/** @file
@brief Линия задержки

		Файл содержит интерфейс и реализацию шаблонного
	класса omni::dsp::DelayLine.

@author Сергей Поличной
*/
