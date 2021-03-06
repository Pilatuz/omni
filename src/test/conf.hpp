///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
/** @file
@brief The unit-test of <omni/conf.hpp>.
@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include <omni/conf.hpp>
#include <test/test.hpp>

#include <iterator>
#include <sstream>

//#include <tchar.h>
#if OMNI_UNICODE
#	define _T(A) L##A
#else
#	define _T(A) A
#endif

template class omni::conf::ItemT<std::wstring>;
template class omni::conf::ItemT<std::string>;
template class omni::conf::io::ParserT<std::wstring>;
template class omni::conf::io::ParserT<std::string>;
template class omni::conf::io::WriterT<std::wstring>;
template class omni::conf::io::WriterT<std::string>;


// unit test
namespace
{

///////////////////////////////////////////////////////////////////////////////
/// @brief The omni::conf unit test.
class ConfTest:
	public omni::test::UnitTest
{
private:

	// test title
	virtual char const* title() const
	{
		return "omni::conf";
	}

private:

	// test function
	virtual bool do_test(std::ostream &os) const
	{
#define TEST(expr) if (expr) {} else { os << "expression failed: \"" \
	<< #expr << "\" at line " << __LINE__ << "\n"; return false; }

		using namespace omni;
		typedef Config::String String;

		os << "sizeof(omni::Config): " << sizeof(Config) << " bytes\n";

		{ // basic test: typedefs, create, destroy, member access
			String str1 = _T("str1");
			String str2 = _T("str1");

			Config e1;
			Config e2(_T("e2"));
			Config e3(String(_T("e3")));
			Config e4(e3);
			Config e5; e5 = e4;
			e5 = String(_T("e5"));
			e5 = _T("e6");

			TEST(e2 != e3);
			TEST(e3 == e4);

			const Config e6(_T("e6"));
			TEST(e6.name() == _T("e6"));
			TEST(e6.val() == _T(""));

			Config e7;
			e7.name() = e6.name();
			e7.val() = e6.val();

			TEST(e6.name() == e6.fullName());
			TEST(e7.name() == e7.fullName());
		}

		{ // basic operations
			Config s1;

			TEST(s1.empty());

			// push_back
			s1.push_back(_T("key1")) = _T("val1");
			Config::ConstIterator it = s1.begin();
			it->fullName();
			TEST(++it == s1.end());

			// push_back
			s1.push_back(_T("key2")) = _T("val2");
			size_t d = std::distance(s1.begin(), s1.end());
			TEST(d == s1.size() && 2U==d);

			// ???
			s1.push_back(s1).name() = _T("sx1");
			s1.push_back(s1).name() = _T("sx2");
			TEST(4 == s1.size());

			s1.get(_T("key3"), true);
			s1[_T("key4")] = _T("val4");

			const Config &s2 = s1;
			s2[_T("key3")].val();

			TEST(s2.exists(_T("key3")));
			s1.remove(_T("key3"));
			TEST(!s2.exists(_T("key3")));

			Config s3(s2);
			swap(s3, s1);
		}

#if OMNI_UNICODE
		typedef std::wostringstream OSStream;
		typedef std::wistringstream ISStream;
		typedef std::wstring String;
#else
		typedef std::ostringstream OSStream;
		typedef std::istringstream ISStream;
		typedef std::string String;
#endif

		{ // print and parse
			Config s1;
			OSStream os1; os1 << s1;
			ISStream is1(os1.str());
			Config s1_; is1 >> s1_;
			TEST(s1 == s1_);

			Config s2;
			s2[_T("key1")] = _T("val1/");
			s2[_T("key2")] = _T(">val2");
			OSStream os2; os2 << s2;
			ISStream is2(os2.str());
			Config s2_; is2 >> s2_;
			TEST(s2 == s2_);

			Config s3;
			s3[_T("s1")] = _T("val1");
			s3[_T("s4")][_T("s4s1")] = _T("val2");
			OSStream os3; os3 << s3;
			ISStream is3(os3.str());
			Config s3_; is3 >> s3_;
			TEST(s3 == s3_);
		}

		{ // parse testing
			const String str1 = _T("");
			ISStream istr1(str1);
			Config s1; istr1 >> s1;
			Config s1_;
			TEST(s1 == s1_);

			Config s2; s2.parse(_T(" #prefix\n<s1> \n </s1>#suffix"));
			Config s2_;
			s2_[_T("s1")];
			TEST(s2 == s2_);

			Config s3; s3.parse(_T(" #prefix\n<s1 \t= \nval1  \n />#suffix"));
			Config s3_;
			s3_[_T("s1")] = _T("val1");
			TEST(s3 == s3_);

			Config s4; s4.parse(_T("<s1 key1=val1 />"));
			Config s4_;
			s4_[_T("s1")][_T("key1")] = _T("val1");
			TEST(s4 == s4_);

			Config s5; s5.parse(_T("#prefix\nkey1=val1 key2=val2#suffix"));
			Config s5_;
			s5_[_T("key1")] = _T("val1");
			s5_[_T("key2")] = _T("val2");
			TEST(s5 == s5_);

			Config s6; s6.parse(_T("key1 key2\n#prefix\nkey3#suffix"));
			Config s6_;
			s6_[_T("key1")];
			s6_[_T("key2")];//.val() = _T("val2");
			s6_[_T("key3")];
			TEST(s6 == s6_);

			Config s7; s7.parse(_T("<? key1=val1 ?>"));
			Config s7_;
			s7_[_T("?")][_T("key1")] = _T("val1");
			TEST(s7 == s7_);
		}

		{ // merge
			Config s1; s1.parse(_T("e1=A1 e2=A2 <s1=AA e1=AA1 e2=AA2/>"));
			Config s2; s2.parse(_T("e2=B2 e3=B3 <s1=BB e3=BB3/> <z2=CC/>"));
			Config s3; s3.parse(_T("e1=A1 e2=B2 <s1=BB e1=AA1 e2=AA2 e3=BB3/> e3=B3 z2=CC"));
			s1.merge(s2);
			TEST(s1 == s3);
		}

		{ // format
			Config s1;
			s1.format(_T("%07X%08X"), 1, 2);
			TEST(s1.val() == _T("000000100000002"));
			s1.format(_T("%07X%08X%08X%08X"), 1, 2, 3, 4);
			TEST(s1.val() == _T("0000001000000020000000300000004"));
		}

		{ // exceptions
			const Config s1;

			try { s1[_T("s")]; return false; }
			catch (const Config::ChildNotFound&) { /*ok*/ }

			try { s1[_T("e")]; return false; }
			catch (const Config::ChildNotFound&) { /*ok*/ }

			Config s2;
			s2.push_back(_T("key")) = _T("val1");
			s2.push_back(_T("key")) = _T("val2");

			try { s2[_T("key")]; return false; }
			catch (const Config::NameIsAmbiguous&) { /*ok*/ }
		}

#undef TEST
		return true;
	}
};

	// global instance
	ConfTest g_ConfTest;

} // unit test
