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
	@brief The unit-test of "conf.hpp".

@author Sergey Polichnoy
*/
#include <omni/conf.hpp>
#include <test/test.hpp>

#include <iterator>
#include <sstream>

#include <tchar.h>

template class omni::conf::ItemT<std::wstring>;
template class omni::conf::ItemT<std::string>;
template class omni::conf::io::ParserT<std::wstring>;
template class omni::conf::io::ParserT<std::string>;
template class omni::conf::io::WriterT<std::wstring>;
template class omni::conf::io::WriterT<std::string>;


// test function
bool test_conf(std::ostream &os)
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
		Config::const_iterator it = s1.begin();
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

		const String str2 = _T(" #prefix\n<s1> \n </s1>#suffix");
		ISStream istr2(str2);
		Config s2; istr2 >> s2;
		Config s2_;
		s2_[_T("s1")];
		TEST(s2 == s2_);

		const String str3 = _T(" #prefix\n<s1 \t= \nval1  \n />#suffix");
		ISStream istr3(str3);
		Config s3; istr3 >> s3;
		Config s3_;
		s3_[_T("s1")] = _T("val1");
		TEST(s3 == s3_);

		const String str4 = _T("<s1 key1=val1 />");
		ISStream istr4(str4);
		Config s4; istr4 >> s4;
		Config s4_;
		s4_[_T("s1")][_T("key1")] = _T("val1");
		TEST(s4 == s4_);

		const String str5 = _T("#prefix\nkey1=val1 key2=val2#suffix");
		ISStream istr5(str5);
		Config s5; istr5 >> s5;
		Config s5_;
		s5_[_T("key1")] = _T("val1");
		s5_[_T("key2")] = _T("val2");
		TEST(s5 == s5_);

		const String str6 = _T("key1 key2\n#prefix\nkey3#suffix");
		ISStream istr6(str6);
		Config s6; istr6 >> s6;
		Config s6_;
		s6_[_T("key1")];
		s6_[_T("key2")];//.val() = _T("val2");
		s6_[_T("key3")];
		TEST(s6 == s6_);
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

	return true;
#undef TEST
}

namespace {

	// Test1 class
	class Test1: public omni::test::UnitTest {
		// test title
		virtual const char* title() const
		{
			return "omni::conf";
		}

		// test function
		virtual bool do_test(std::ostream &os) const
		{
			return test_conf(os);
		}
	} test1;

} // namespace
