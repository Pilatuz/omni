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
@brief Smart pointers.

		This header file contains interface and implementation
	of the smart pointer with reference counting.

@author Sergey Polichnoy
*/
#ifndef __OMNI_SMART_HPP_
#define __OMNI_SMART_HPP_

#include <omni/defs.hpp>

#include <assert.h>

namespace omni
{
	namespace smart
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Reference counting.
/**
		This class implements reference counting and automatic
	self destruction if there is no more references.

		The objects of any derived class should be created using @b new
	operator. These objects must be destroyed using detach() method.

		It is recommended to use virtual derivation for derived classes.

		This class is used with SharedPtr template.

@code
	class MyObj: public virtual SharedObj {
		// ...
	};
@endcode

@see @ref omni_smart_shared
*/
class SharedObj: private omni::NonCopyable {
protected:
	SharedObj();
	virtual ~SharedObj();

public:
	long N_refs() const;

public:
	void attach();
	bool detach();

private:
	long m_N_refs;
};

		// forward declarations
		namespace details
		{
			template<typename T>
				class SharedPtr_Base;
		}


//////////////////////////////////////////////////////////////////////////
/// @brief Smart pointer with reference counting.
/**
		This class automatically manages object's number of references.
	Constructor increases the number of references. Destructor decreases
	number of references. The object should be dynamically created
	using @b new operator.

		This class behaves the same as a simple pointer to the object.
	It define dereference operators and compare operators (== и !=).
	The smart pointer can be "null".

		The type @a T can be derived from SharedObj class. In this case the
	SharedPtr class has no additional costs. Otherwise for each object
	the reference counter will be dynamically created.

@param T The object type.
@see @ref omni_smart_shared
*/
template<typename T>
class SharedPtr: private details::SharedPtr_Base<T>::base_type {
	typedef typename details::SharedPtr_Base<T>::base_type inherited;
	typedef SharedPtr<T> ThisType;

public:
	typedef typename inherited::reference reference;  ///< @brief Reference type.
	typedef typename inherited::pointer   pointer;    ///< @brief Pointer type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Create "null" pointer.
/**
		This constructor creates "null" pointer.

@code
	SharedPtr<MyClass> p;
	f(*p); // null pointer exception
@endcode
*/
	SharedPtr()
		: inherited(0)
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Attach simple pointer.
/**
		This constructor increases (@a *p) object's number of references.

@param[in] ptr Simple pointer.
*/
	explicit SharedPtr(pointer ptr)
		: inherited(ptr)
	{
		attach();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Copy construction.
/**
		This constructor increases (@a *x) object's number of references.

@param[in] other Smart pointer.
*/
	SharedPtr(const ThisType &other)
		: inherited(other)
	{
		attach();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Destruction.
/**
		The destructor decreases object's number of references.
	So if there is no more references, object will be destroyed.
*/
	~SharedPtr()
	{
		detach();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Assign smart pointer.
/**
		Если автоматический указатель ссылался на объект,
	то у старого объекта уменьшается количество ссылок.

		Запоминает указатель на новый объект &*x, и если он не является нулевым,
	то увеличивает у нового объекта количество ссылок.

@param[in] x Smart pointer.
@return Self reference.
*/
	ThisType& operator=(const ThisType &other)
	{
		ThisType t(other);
		swap(t);

		return *this;
	}


public:

//////////////////////////////////////////////////////////////////////////
/// @brief Ссылка на объект.
/**
		Метод разъименовывает умный указатель.
	Если указатель нулевой, произойдет обращение по нулевому адресу.

@return Ссылка на объект.
*/
	reference operator*() const
	{
		const pointer ptr = get();
		return *ptr;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Указатель на объект.
/**
		Метод разъименовывает умный указатель.

@return Простой указатель на объект.
*/
	pointer operator->() const
	{
		return get();
	}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Проверить на "нулевой" указатель.
/**
		Метод проверяет является ли умный указатель нулевым.

@return @b true Если умный указатель нулевой, иначе @b false.
*/
	pointer get() const
	{
		return inherited::get();
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить уникальность указателя.
/**
		Метод проверяет, является ли текущий автоматический указатель
	уникальным, т.е. на хранимый объект существует только одна ссылка.

@return @b true Если умный указатель уникальный, иначе @b false.
*/
	bool unique() const
	{
		return (get() != 0)
			&& (N_refs() == 1);
	}

public:
	/// @brief Unspecified bool type.
	typedef pointer (ThisType::*unspecified_bool_type)() const;

	/// @brief Check for non "NULL".
	operator unspecified_bool_type() const // never throws
	{
		return (get() == 0) ? 0 : &ThisType::get;
	}

	/// @brief Check for "NULL".
	bool operator!() const
	{
		return get() == 0;
	}

	/// @brief Swap two pointers
	void swap(ThisType &other)
	{
		inherited::swap(other);
	}
};



//////////////////////////////////////////////////////////////////////////
/// @brief Проверить два указателя на равенство.
/**
		Оператор проверяет два умных укзателя на равенство.
	Указатели считаются равными, если они указывают
	на один и тот же объект или оба являются нулевыми.

@param[in] x Первый умный указатель.
@param[in] y Второй умный указатель.
@return @b true Если указатели равны, иначе @b false.
*/
template<typename T> inline
	bool operator==(const SharedPtr<T> &x, const SharedPtr<T> &y)
{
	return x.get() == y.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить два указателя на равенство.
/**
		Оператор проверяет умный и простой укзатели на равенство.
	Указатели считаются равными, если они указывают
	на один и тот же объект или оба являются нулевыми.

@param[in] x Умный указатель.
@param[in] y Простой указатель.
@return @b true Если указатели равны, иначе @b false.
*/
template<typename T> inline
	bool operator==(const SharedPtr<T> &x, const T *y)
{
	return x.get() == y;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить два указателя на равенство.
/**
		Оператор проверяет простой и умный укзатели на равенство.
	Указатели считаются равными, если они указывают
	на один и тот же объект или оба являются нулевыми.

@param[in] x Простой указатель.
@param[in] y Умный указатель.
@return @b true Если указатели равны, иначе @b false.
*/
template<typename T> inline
	bool operator==(const T *x, const SharedPtr<T> &y)
{
	return x == y.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить два указателя на неравенство.
/**
		Оператор проверяет два умных укзателя на неравенство.

@param[in] x Первый умный указатель.
@param[in] y Второй умный указатель.
@return @b true Если указатели не равны, иначе @b false.
*/
template<typename T> inline
	bool operator!=(const SharedPtr<T> &x, const SharedPtr<T> &y)
{
	return x.get() != y.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить два указателя на неравенство.
/**
		Оператор проверяет умный и простой укзатели на неравенство.

@param[in] x Умный указатель.
@param[in] y Простой указатель.
@return @b true Если указатели не равны, иначе @b false.
*/
template<typename T> inline
	bool operator!=(const SharedPtr<T> &x, const T *y)
{
	return x.get() != y;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить два указателя на неравенство.
/**
		Оператор проверяет простой и умный укзатели на неравенство.

@param[in] x Простой указатель.
@param[in] y Умный указатель.
@return @b true Если указатели не равны, иначе @b false.
*/
template<typename T> inline
	bool operator!=(const T *x, const SharedPtr<T> &y)
{
	return x != y.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить два указателя на less.
/**
		Оператор проверяет два умных укзателя на less.

@param[in] x Первый умный указатель.
@param[in] y Второй умный указатель.
@return @b true Если указатели не равны, иначе @b false.
*/
template<typename T> inline
	bool operator<(const SharedPtr<T> &x, const SharedPtr<T> &y)
{
	return x.get() < y.get();
}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить два указателя на less.
/**
		Оператор проверяет умный и простой укзатели на less.

@param[in] x Умный указатель.
@param[in] y Простой указатель.
@return @b true Если указатели не равны, иначе @b false.
*/
template<typename T> inline
	bool operator<(const SharedPtr<T> &x, const T *y)
{
	return x.get() < y;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Проверить два указателя на less.
/**
		Оператор проверяет простой и умный укзатели на less.

@param[in] x Простой указатель.
@param[in] y Умный указатель.
@return @b true Если указатели не равны, иначе @b false.
*/
template<typename T> inline
	bool operator<(const T *x, const SharedPtr<T> &y)
{
	return x < y.get();
}



//////////////////////////////////////////////////////////////////////////
/// @brief Swap two smart pointers.
template<typename T> inline
	void swap(SharedPtr<T> &x, SharedPtr<T> &y)
{
	x.swap(y);
}

		// implementation...
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
// @brief Базовый класс без накладных расходов.
template<typename T>
class SharedPtr_ThinBase: private omni::NonCopyable {
	typedef SharedPtr_ThinBase<T> ThisType;

protected:
	typedef T& reference;
	typedef T* pointer;

protected: // constructors...
	explicit SharedPtr_ThinBase(pointer ptr)
		: m_ptr(ptr)
	{}
	SharedPtr_ThinBase(const ThisType &other)
		: m_ptr(other.m_ptr)
	{}

protected: // properties
	pointer get() const
	{
		return m_ptr;
	}
	long N_refs() const
	{
		return m_ptr->SharedObj::N_refs();
	}

protected: // reference counting
	void attach()
	{
		if (m_ptr)
			m_ptr->SharedObj::attach();
	}
	void detach()
	{
		if (m_ptr)
			m_ptr->SharedObj::detach();
	}

protected: // auxiliary
	void swap(ThisType &other)
	{
		pointer ptr = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = ptr;
	}

private:
	pointer m_ptr;
};


//////////////////////////////////////////////////////////////////////////
// @brief Базовый класс с накладными расходами.
template<typename T>
class SharedPtr_FatBase: private omni::NonCopyable {
	typedef SharedPtr_FatBase<T> ThisType;

protected:
	typedef T& reference;
	typedef T* pointer;

protected: // constructors
	explicit SharedPtr_FatBase(pointer ptr)
		: m_ptr(ptr), m_N(0)
	{}
	SharedPtr_FatBase(const ThisType &other)
		: m_ptr(other.m_ptr), m_N(other.m_N)
	{}

protected: // properties
	pointer get() const
	{
		return m_ptr;
	}
	long N_refs() const
	{
		return m_N->N_refs();
	}

protected: // reference counting
	void attach()
	{
		if (m_ptr && !m_N)
			m_N = new Counter();

		if (m_N)
			m_N->attach();
	}
	void detach()
	{
		if (m_N)
		{
			if (m_N->detach())
				delete m_ptr;
		}
	}

protected: // auxiliary

	void swap(ThisType &other)
	{
		pointer ptr = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = ptr;

		Counter *n = m_N;
		m_N = other.m_N;
		other.m_N = n;
	}

private:
	// TODO: pool::FastObj to base class?
	class Counter: public SharedObj {};

private:
	pointer m_ptr;
	Counter *m_N;
};

// SharedPtr_ThinBase selector
template<bool IsThin>
struct SharedPtr_BaseSelector {
	template<typename T>
	struct Base {
		typedef SharedPtr_ThinBase<T> base_type;
	};
};

// SharedPtr_FatBase selector
template<>
struct SharedPtr_BaseSelector<false> {
	template<typename T>
	struct Base {
		typedef SharedPtr_FatBase<T> base_type;
	};
};


// @brief Выбор базового класса в зависимости от типа T.
//
// если тип T производный от SharedObj, используется SharedPtr_ThinBase
// иначе, используется SharedPtr_FatBase
template<typename T>
class SharedPtr_Base {
private:
	struct YES { char dummy[666]; };
	struct  NO { char dummy[999]; };

	static YES check(const SharedObj*);
	static  NO check(...);

	enum { is_thin = sizeof(YES) == sizeof(check((const T*)0)) };

public:
	typedef typename SharedPtr_BaseSelector<is_thin>
		::template Base<T>::base_type base_type;
};

		} // implementation
	} // smart namespace
} // omni namespace

#endif // __OMNI_SMART_HPP_
