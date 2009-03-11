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
	@brief Configuration tools.

		This source file contains a char constants,
	that helps to work with configurations.

@author Sergey Polichnoy
*/
#include <omni/conf.hpp>

namespace omni
{

//////////////////////////////////////////////////////////////////////////
/** @namespace omni::conf
	@brief Configuration tools.

		This namespace contains configuration tools.

@code
	#include <omni/conf.hpp>
@endcode

@see @ref omni_config
*/
	namespace conf
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Configuration tools: implementation.
/**
		This namespace contains implementation specific tools.

@warning Do not use this namespace.
*/
		namespace details
		{}


//////////////////////////////////////////////////////////////////////////
/// @brief Configuration tools: exceptions.
/**
		This namespace contains exception classes
	related to the configuration tools.
*/
		namespace err
		{}


//////////////////////////////////////////////////////////////////////////
/// @brief Configuration tools: input/output.
/**
		This namespace contains input/output tools.
*/
		namespace io
		{}


		// CharConst
		/// @cond details
		namespace details
		{

// explicit CharConst<wchar_t> instantiation...
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::SEPARATOR[]  = L":";
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::ENDLINE  = L'\n';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::SPACE    = L' ';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::COMMENT  = L'#';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::EQUAL    = L'=';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::BEGIN    = L'<';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::CLOSE    = L'/';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::END      = L'>';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::SQUOTE   = L'\'';
template<> const CharConst<wchar_t>::Char CharConst<wchar_t>::DQUOTE   = L'\"';

template<>
bool CharConst<wchar_t>::is_delim(Char ch)
{
	return iswspace(ch)
		|| (COMMENT == ch)
		|| (EQUAL == ch)
		|| (BEGIN == ch)
		|| (CLOSE == ch)
		|| (END == ch)
		|| (SQUOTE == ch)
		|| (DQUOTE == ch);
}

// explicit CharConst<char> instantiation...
template<> const CharConst<char>::Char CharConst<char>::SEPARATOR[]  = ":";
template<> const CharConst<char>::Char CharConst<char>::ENDLINE  = '\n';
template<> const CharConst<char>::Char CharConst<char>::SPACE    = ' ';
template<> const CharConst<char>::Char CharConst<char>::COMMENT  = '#';
template<> const CharConst<char>::Char CharConst<char>::EQUAL    = '=';
template<> const CharConst<char>::Char CharConst<char>::BEGIN    = '<';
template<> const CharConst<char>::Char CharConst<char>::CLOSE    = '/';
template<> const CharConst<char>::Char CharConst<char>::END      = '>';
template<> const CharConst<char>::Char CharConst<char>::SQUOTE   = '\'';
template<> const CharConst<char>::Char CharConst<char>::DQUOTE   = '\"';

template<>
bool CharConst<char>::is_delim(Char ch)
{
	return isspace(ch)
		|| (COMMENT == ch)
		|| (EQUAL == ch)
		|| (BEGIN == ch)
		|| (CLOSE == ch)
		|| (END == ch)
		|| (SQUOTE == ch)
		|| (DQUOTE == ch);
}


		} // CharConst
		/// @endcond

	} // conf namespace
} // omni namespace


//////////////////////////////////////////////////////////////////////////
/** @page omni_config Configuration tools

		omni::config::SectionT class ����������� ����� ���������� �������������
	��������� ������. ������������ � �������� �������������� �������� ��������
	����������, ��������, ��� ������� ������������ �����������, ���
	���������� �����������, ��� �������� ������������ ����������.
	������ ������������ ����� ��������� ��������� ������ � ���������.
	������ �������� omni::config::ElementT ������������ ����� ���� "����=��������".
	����� ����� �������� �������� � �������� �������� - ��� ������ ��������
	�����������, � �������� ������.

		������ � ��������� ����� ��������� ����������, �.�. ���������, �������
	� �������� �������� ������ ��� ���������. �������� ����� ������������
	��������� ����� ��� ������� ������������. ��������:

@code
	<data>
		"time" = 1000 # [seconds]
	</data>
@endcode

		���������� �� ������: ��� ����� ������������� � ��������. ���� ���
	������ ��� ��������� �������� ������� ��� ������ ��������� �������,
	�� ��� ������ ���� ��������� � �������. ����������� �������������
	������� (") ��� ��������� (') �������.

@section omni_config_section ������
		������ ������������� ��� ������������� ������� �� ������ ����������.
	����� ����, ������ ����� ��������� �������� ������, ������� �����������
	���������.

@code
	<A>
		a=b
		<B>
			b=c
		</B>
	</A>
@endcode

		��� ����������� ������ � ��������� ������ ������������ ������� ������.

@code
	# ������ ������
	< "section name" >
		# ...
	</ "section name" >  # ��������� ������
@endcode

		������ ������ ������ ���� ������� ��� �� ������! �����������
	������������� ��������� �������:

@code
	# ������ ������
	< "section name"
		# ...
	/>                 # ��������� ������
@endcode

@see omni::config::SectionT

@section omni_config_element ���������
		��������� �������� ������� ����� ���� ������: �� ������
	��������� ����� ������� "=" ����������� �������� ���������.
	@code a = 123 @endcode ���� ������� "=" ���, �� ���������, ��� ��������
	����� ������ ��������. ��������, @code a b @endcode ���������� ���
	��������� � ������� ����������. ������ ����� ����� ��������� ��������,
	��������:

@code
	<"section name" = "section value"
		"element name" = "element value" />
@endcode

@see omni::config::ElementT

@section omni_config_comment �����������
		�� ��� ����������� �� �������� "#" ��������� ������������.
	����������� ������������� ��� ����� ������������:
		- �������: ��������� ������
		- ����������: ����������� ����������� � ���������� ��������� ��� ������
		- ����������: ���������� ����������� � ����������� ��������� ��� ������

	@code
	# simple comment

	# section's prefix comment
	# section's prefix continue
	<A>
		# element's prefix comment
		a = b # element's suffix comment
	</A>    # section's suffix comment
	        # section's suffix continue
	@endcode
*/
