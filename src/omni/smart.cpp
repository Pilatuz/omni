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
/** @page omni_smart ����� ���������

@section omni_smart_shared ��������� � ��������� ������

		��������� � ��������� ������ omni::smart::SharedPtr ���������
	������������� ������� ������, ����� �� ���� ������ ��� ������.
	��������� �������� ��������� �������. �������� ������� �����
	���� �������, ����������� �� omni::smart::SharedObj,
	������� ������������ ���������� ������ �� ����.

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

		�������������� ��� ���� ���������� � ��������� ������: "������" � "�������".
	"������" ��������� ���������� ������� ������ � ����� ������� ���� @a T. ���
	��� ��� @a T ������ ���� ����������� �� omni::smart::SharedObj.

@image html SharedPtr1.png "������ ��������� � ��������� ������"
	
		"�������" ��������� ��� ������� ������� ���� @a T ������� ���� �������
	� ��������� ������. � ���� ������ ��� ��������� �������� ������������
	����� ��������� � �������, �� � ����� �������������. �.�. �����
	�������������� � ������ �� ������������ �� omni::smart::SharedObj.

@image html SharedPtr2.png "������� ��������� � ��������� ������"

		��� ����������� �������������� ����������, ������ �� ����������, ������
	����� � ������� ������������� ���������� ������. ��� �������� ���������
	����������� ������ �����������, � ���� ���������� ������ ����������
	������ ����, �� ������ ���������.

		����� ��������� �� �������� �������� � �� ������� ������������ �����
	���������. ��� ��������� �������� � ������ �������������
	����������� ������������. �.�. ���� ������ @b A �������� �����
	���������, ������� ��������� �� ������ @b B, ������� � ���� �������
	�������� ����� ��������� �� ��� �� ������ @b A, �� ��� ���
	������� ������������� ������� �� ��������. � ����� ����� �����������
	��� ��� ����� ��������. ������� ��� �������������� ��������� �����
	���������� ������������ �������������� ��������� � ���������
	������� � ������ �����������.

		������������ omni::smart::SharedPtr ������������� ������ ��� ���,
	����� ����� ������� ����� ������������� � ������ ������ ���������
	� ������������� �������. ��� ��, ��� ����� ����� ������� �����
	����������, ����� ����������� @a std::auto_ptr ��� ������� ���������.

		��� ���� ������ ������: ���� ����������� ������������ ��������������
	���������, �� ������� ����� ����������� ������ � ��������� ������ �
	������� ��������� @b new.
*/
