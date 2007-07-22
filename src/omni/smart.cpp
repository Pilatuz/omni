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

		This source file contains documentation only.

@author Sergey Polichnoy
*/
#include <omni/smart.hpp>

namespace omni
{
//////////////////////////////////////////////////////////////////////////
/** @brief Smart pointers.

		This namespace contains smart pointer with reference counting.

@see @ref omni_smart
*/
	namespace smart
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Construction.
/**
		This constructor initializes object without references
	(i.e. number of references is equal to zero).
*/
SharedObj::SharedObj()
	: m_N_refs(0)
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Destruction.
/**
		The destructor checks that this objects has no references
	(i.e. number of references is equal to zero).

		Objects created using @b new, should
	be deleted using detach() method.
*/
SharedObj::~SharedObj()
{
	assert(0 == m_N_refs
		&& "invalid object deletion");
}


//////////////////////////////////////////////////////////////////////////
/// @brief Number of references.
/**
		This method returns the current number of references.

@return Number of references.
*/
long SharedObj::N_refs() const
{
	return m_N_refs;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Add reference.
/**
		This method increases number of references.
*/
void SharedObj::attach()
{
	++m_N_refs;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Remove reference.
/**
		This method decreases number of references. If there is no more
	references, then object will be automatically deleted using @b delete
	operator.

@return @b true If object was destroyed, otherwise @b false.
*/
bool SharedObj::detach()
{
	if (0 == --m_N_refs)
	{
		delete this; // (!)
		return true;
	}

	return false;
}

	} // smart namespace
} // omni namespace


//////////////////////////////////////////////////////////////////////////
/** @page omni_smart Умные указатели

@section omni_smart_shared Указатель с подсчётом ссылок

		Указатель с подсчётом ссылок omni::smart::SharedPtr позволяет
	автоматически удалять объект, когда на него больше нет ссылок.
	Указатель является шаблонным классом. Параметр шаблона может
	быть классом, производным от omni::smart::SharedObj,
	который подсчитывает количество ссылок на себя.

@code
	class MyClass: public omni::smart::SharedObj {
		// ...
	};

	// shared pointer to MyClass
	typedef omni::smart::SharedPtr<MyClass> PMyClass;

	void f()
	{
		PMyClass p(new MyClass());
		p = new MyClass();

		*p; // access to MyClass

		// ...
	}
@endcode

		Поддерживаются два типа указателей с подсчетом ссылок: "тонкий" и "толстый".
	"Тонкий" указатель использует счетчик ссылок в самом объекте типа @a T. Для
	это тип @a T должен быть производным от omni::smart::SharedObj.

@image html SharedPtr1.png "Тонкий указатель с подсчетом ссылок"
	
		"Толстый" указатель для каждого объекта типа @a T создает свой счетчик
	в свободной памяти. В этом случае сам указатель является потенциально
	более медленным и большим, но и более универсальным. Т.к. может
	использоваться с типами не производными от omni::smart::SharedObj.

@image html SharedPtr2.png "Толстый указатель с подсчетом ссылок"

		При копировании автоматических указателей, объект не копируется, вместо
	этого у объекта увеличивается количество ссылок. При удалении указателя
	колличество ссылок уменьшается, и если количество ссылок становится
	равным нулю, то объект удаляется.

		Умные указатели не являются панацеей и их следует использовать ОЧЕНЬ
	осторожно. Они перестают работать в случае возникновения
	циклических зависимостей. Т.е. если объект @b A содержит умный
	указатель, который ссылается на объект @b B, который в свою очередь
	содержит умный указатель на тот же объект @b A, то эти два
	объекта автоматически никогда не удалятся. В цикле может участвовать
	два или более объектов. Поэтому при проектировании программы нужно
	осмысленно использовать автоматические указатели и разделять
	сильные и слабые зависимости.

		Использовать omni::smart::SharedPtr рекомендуется только там где,
	время жизни объекта может заканчиваться в разных местах программы
	в неопределённом порядке. Там же, где время жизни объекта чётко
	определено, можно использвать @a std::auto_ptr или простые указатели.

		Ещё один важный момент: если планируется использовать автоматические
	указатели, то объекты могут создаваться только в свободной памяти с
	помощью оператора @b new.
*/
