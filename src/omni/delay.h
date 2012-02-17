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
	@brief The delay line tool.

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#ifndef __OMNI_DELAY_HPP_
#define __OMNI_DELAY_HPP_

#include <omni/defs.hpp>

#include <assert.h>
#include <memory>
#include <vector>

namespace omni
{
	namespace dsp
	{
		namespace details
		{
			// DelayLine iterator
			template<typename ContRef, typename Base>
				class DelayLine_Iterator;
		}

//////////////////////////////////////////////////////////////////////////
/// @brief The delay line class.
/**
		Линия задержки представляет собой структуру данных, которая содержит
	некоторое количество элементов, определяемое размером линии задержки.
	Новый элемент, поступающий на вход, "сдвигает" уже имеющиеся элементы.
	При этом последний элемент "выталкивается" из линии задержки. Таким
	образом, новые элементы поступающие на вход линии задержки, оказываются
	на ее выходе через несколько тактов работы, равное размеру.

@image html DelayLine.png Логическая схема линии задержки

		Например, следующий код демонстрирует работу линии задержки. После
	добавления нового элемента на экран выводится новое содержимое линии
	задержки и "вытолкнутый" элемент.

@code
DelayLine<int> d(5); // размер - 5 элементов
for (int k = 0; k < 10; ++k)
{
	int x = k+1;  // новый элемент
	int y = d(x); // добавить новый элемент, вернет "вытолкнутый" элемент

	std::cout << "input: " << x << ", ";
	std::cout << "content: [ ";
	std::copy(d.begin(), d.end(),
		std::ostream_iterator<int>(std::cout, " "));
	std::cout << "], output: " << y << "\n";
}
@endcode

		После запуска программы на экране появится:

@code
input: 1, content: [ 1 0 0 0 0 ], output: 0
input: 2, content: [ 2 1 0 0 0 ], output: 0
input: 3, content: [ 3 2 1 0 0 ], output: 0
input: 4, content: [ 4 3 2 1 0 ], output: 0
input: 5, content: [ 5 4 3 2 1 ], output: 0
input: 6, content: [ 6 5 4 3 2 ], output: 1
input: 7, content: [ 7 6 5 4 3 ], output: 2
input: 8, content: [ 8 7 6 5 4 ], output: 3
input: 9, content: [ 9 8 7 6 5 ], output: 4
@endcode

		Параметр шаблона @a T определяет тип элементов линии задержки.

@section omni_DelayLine_access Доступ к элементам

		Класс содержит методы доступа к содержимому линии задержки по индексу
	at() и через квадратные скобки. Также можно скопировать все содержимое
	во внешний буфер с помощью метода copy().

		Также класс содержит несколько вспомогательных методов для доступа
	к первому, последнему, а также, "вытолкнотому" элементам.
	Соответственно: front(), back(), out().

		Поддерживаются итераторы с произвольным доступом и соответственно
	методы begin() и end().


@section omni_DelayLine_example Примеры использования

		Следующие фрагменты кода демонстрируют примеры использования
	линии задержки.


@subsection omni_DelayLine_example1 Медианный фильтр
		Медианный фильтр - скользящее окно, используется в качестве защиты
	от случайных выбросов сигнала. Содержимое скользящего окна
	упорядочивается и возвращается элемент из середины.

@code
template<typename T>
	class MedianFilter {
	public:
		// создать фильтр размера N
		MedianFilter(size_t N, const T &x)
			: m_delay(N, x), m_buffer(N) {}

		// фильтровать отсчет
		T operator()(const T &x)
		{
			m_delay(x); // добавляем в линию задержки

			// копируем во временный буфер
			m_delay.copy(m_buffer.begin());

			// упорядочиваем
			std::sort(m_buffer.begin(),
				m_buffer.end());

			// возвращаем элемент из середины
			return m_buffer[m_buffer.size()/2];
		}

	private:
		DelayLine<T> m_delay;
		std::vector<T> m_buffer;
	};
@endcode


@subsection omni_DelayLine_example2 Многолучевый канал
		Выходом многолучевого канала является взвешенная сумма задержанного
	входного сигнала. Первый луч без задержки, второй задержан на 5 отсчетов,
	а третий - на 9.

@code
double channel_run(double x)
{
	m_delay(x);

	return 1.0*m_delay[0]
		+ 0.5*m_delay[5]
		+ 0.1*m_delay[9];
}
@endcode
*/
template<typename T, typename A = std::allocator<T> >
class DelayLine
{
	typedef DelayLine<T, A> ThisType;
	typedef std::vector<T, A> BufType;

//////////////////////////////////////////////////////////////////////////
/// @name Main typedefs
/// @{
public:
	typedef typename BufType::const_reference const_reference;    ///< @brief The const reference type.
	typedef typename BufType::      reference       reference;    ///< @brief The non-const reference type.
	typedef typename BufType::value_type value_type;              ///< @brief The value type.
	typedef typename BufType::size_type   size_type;              ///< @brief The size type.

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Конструкторы
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		Конструктор создает линию задержки нулевого размера. Элементы
	попадающие на вход линии задержки, сразу же оказываются на ее выходе.
	Эквивалентный код работы:

@code
const_reference operator()(const_reference x)
{
	return x;
}
@endcode
*/
	DelayLine()
		: m_xbuf(1),
		  m_wpos(0)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
		Конструктор создает линию задержки размера @a N. Элементы попадающие
	на вход линии задержки, оказываются на ее выходе через @a N тактов.
	Линия задержки инициализируется значениями @a init_val.

@param N The delay line size.
@param init_val The initial value.
*/
	explicit DelayLine(size_type N, const_reference init_val = value_type())
		: m_xbuf(N+1, init_val), m_wpos(N)
	{}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @{ Manipulators
public: /// @name Размер и содержимое

//////////////////////////////////////////////////////////////////////////
/// @brief Get size.
/**
@return The current delay line size.
*/
	size_type size() const
	{
		return m_xbuf.size() - 1;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Resize delay line.
/**
		The current content of the delay line is not dropped!
	The initial value @a init_val is actual if the delay line size is grown.

@param N The new delay line size.
@param init_val The initial value.
*/
	void resize(size_type N, const_reference init_val = value_type())
	{
		m_xbuf.resize(N+1, init_val);
		m_wpos = N;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Reset the delay line content.
/**
		The size of the delay line not changed!

@param init_val The initial value.
*/
	void reset(const_reference init_val = value_type())
	{
		const size_type N = m_xbuf.size();
		m_xbuf.assign(N, init_val);
		m_wpos = N-1;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Put value to the delay line.
/**
		Метод помещяет элемент @a x в линию задержки
	и возвращает "вытолкнутый" элемент.

	The equivalent code is:

@code
const_reference operator()(const_reference x)
{
	m_buf.push_front(x);
	m_out = m_buf.back();
	m_buf.pop_back();
	return m_out;
}
@endcode

		Where @a m_buf - buffer of elements (for example @a deque),
	@a m_out - "poped" element.

@param x The element "pushed" to the delay line.
@return The element "poped" from the delay line.
*/
	const_reference operator()(const_reference x)
	{
		m_xbuf[m_wpos] = x;

		if (0 == m_wpos)
			m_wpos = size();
		else
			--m_wpos;

		return m_xbuf[m_wpos];
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Access
/// @{
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get element at specified index.
	const_reference at(size_type i) const
	{
		assert(i<size() && "index out of range");

		size_type N = m_xbuf.size();
		size_type x = m_wpos + i+1;
		if (N <= x) x -= N;

		return m_xbuf[x];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element at specified index.
	reference at(size_type i)
	{
		assert(i<size() && "index out of range");

		size_type N = m_xbuf.size();
		size_type x = m_wpos + i+1;
		if (N <= x) x -= N;

		return m_xbuf[x];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element at specified index.
	const_reference operator[](size_type i) const
	{
		return at(i);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get element at specified index.
	reference operator[](size_type i)
	{
		return at(i);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get last "popped" element.
	const_reference out() const
	{
		return m_xbuf[m_wpos];
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Get last "popped" element.
	reference out()
	{
		return m_xbuf[m_wpos];
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the first element.
	const_reference front() const
	{
		return at(0);
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Get the first element.
	reference front()
	{
		return at(0);
	}

//////////////////////////////////////////////////////////////////////////
/// @brief Get the last element.
	const_reference back() const
	{
		return at(size()-1);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Get the last element.
	reference back()
	{
		return at(size()-1);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Copy the delay line content.
	template<typename Out>
		Out copy(Out first) const
	{
		const size_type N = m_xbuf.size();

		for (size_type i = m_wpos+1; i < N; ++i)
			{ *first = m_xbuf[i]; ++first; }
		for (size_type i = 0; i < m_wpos; ++i)
			{ *first = m_xbuf[i]; ++first; }

		return first;
	}

/// @}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @name Iterators
/// @{
public:

	typedef details::DelayLine_Iterator<const ThisType*,
		typename BufType::const_iterator> const_iterator;  ///< @brief The const iterator type.
	typedef details::DelayLine_Iterator<ThisType*,
		typename BufType::iterator> iterator; ///< @brief The non-const iterator type.

//////////////////////////////////////////////////////////////////////////
/// @brief The begin of the delay line.
	const_iterator begin() const
	{
		return const_iterator(this, 0);
	}

//////////////////////////////////////////////////////////////////////////
/// @brief The begin of the delay line.
	iterator begin()
	{
		return iterator(this, 0);
	}

//////////////////////////////////////////////////////////////////////////
/// @brief The end of the delay line.
	const_iterator end() const
	{
		return const_iterator(this, size());
	}

//////////////////////////////////////////////////////////////////////////
/// @brief The end of the delay line.
	iterator end()
	{
		return iterator(this, size());
	}

private:
	BufType m_xbuf;       ///< @brief The circular buffer.
	size_type m_wpos;     ///< @brief The current write position.
};


//////////////////////////////////////////////////////////////////////////
// Iterator implementation...
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// DelayLine iterator
template<typename ContPtr, typename Base>
class DelayLine_Iterator
{
	typedef DelayLine_Iterator<ContPtr, Base> ThisType;
	typedef Base BaseType;

public:
	typedef typename std::iterator_traits<BaseType>::iterator_category iterator_category;
	typedef typename std::iterator_traits<BaseType>::difference_type difference_type;
	typedef typename std::iterator_traits<BaseType>::value_type value_type;
	typedef typename std::iterator_traits<BaseType>::reference reference;
	typedef typename std::iterator_traits<BaseType>::pointer pointer;

public:

//////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
	DelayLine_Iterator()
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The main constructor.
/**
@param pcont The container pointer.
@param elem_pos The element index.
*/
	DelayLine_Iterator(ContPtr pcont, difference_type elem_pos)
		: m_pcont(pcont), m_pos(elem_pos)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The auxiliary constructor.
	template<typename ContPtr2, typename Base2>
	DelayLine_Iterator(const DelayLine_Iterator<ContPtr2, Base2> &other)
		: m_pcont(other.cont()), m_pos(other.pos())
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief The auxiliary assignment operator.
	template<typename ContPtr2, typename Base2>
	ThisType& operator=(const DelayLine_Iterator<ContPtr2, Base2> &other)
	{
		m_pcont = other.cont();
		m_pos = other.pos();
		return *this;
	}

public:
	ContPtr cont() const { return m_pcont; }
	difference_type pos() const { return m_pos; }

public:
	reference operator[](difference_type i) const
	{
		return m_pcont->at(m_pos + i);
	}

	reference operator*() const
	{
		return m_pcont->at(m_pos);
	}

	pointer operator->() const
	{
		return &(**this);
	}

public:
	ThisType& operator++() { ++m_pos; return *this; }
	ThisType& operator--() { --m_pos; return *this; }
	ThisType operator++(int) { ThisType t(*this); ++m_pos; return t; }
	ThisType operator--(int) { ThisType t(*this); --m_pos; return t; }

	ThisType& operator+=(difference_type d) { m_pos += d; return *this; }
	ThisType& operator-=(difference_type d) { m_pos -= d; return *this; }
	ThisType operator+(difference_type d) const { ThisType t(*this); return (t += d); }
	ThisType operator-(difference_type d) const { ThisType t(*this); return (t -= d); }
	difference_type operator-(const ThisType &x) const { return (m_pos - x.m_pos); }

	bool operator==(const ThisType &x) const { return m_pos == x.m_pos; }
	bool operator!=(const ThisType &x) const { return m_pos != x.m_pos; }
	bool operator< (const ThisType &x) const { return m_pos <  x.m_pos; }
	bool operator> (const ThisType &x) const { return m_pos >  x.m_pos; }
	bool operator<=(const ThisType &x) const { return m_pos <= x.m_pos; }
	bool operator>=(const ThisType &x) const { return m_pos >= x.m_pos; }

private:
	ContPtr m_pcont;
	difference_type m_pos;
};

		} // details namespace
	} // dsp namespace
} // omni namespace

#endif // __OMNI_DELAY_HPP_
