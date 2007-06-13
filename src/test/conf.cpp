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

template class omni::conf::ElementT<std::wstring>;
template class omni::conf::ElementT<std::string>;
template class omni::conf::SectionT<std::wstring>;
template class omni::conf::SectionT<std::string>;
template class omni::conf::ElementListT<std::wstring>;
template class omni::conf::ElementListT<std::string>;
template class omni::conf::SectionListT<std::wstring>;
template class omni::conf::SectionListT<std::string>;
template class omni::conf::io::ParserT<std::wstring>;
template class omni::conf::io::ParserT<std::string>;
template class omni::conf::io::WriterT<std::wstring>;
template class omni::conf::io::WriterT<std::string>;


// test function
bool test_conf(std::ostream &os)
{
#define TEST(expr) if (!(expr)) { os << "expression failed: \"" \
	<< #expr << "\" at line " << __LINE__ << "\n"; return false; }

	using namespace omni::conf;

	//os << "sizeof(CfgElement): " << sizeof(CfgElement) << " bytes\n";
	//os << "sizeof(CfgSection): " << sizeof(CfgSection) << " bytes\n";

	{ // basic test: typedefs, create, destroy, member access
		String es1 = _T("es1");
		String ss1 = _T("es1");

		Element e1;
		Element e2(_T("e2"));
		Element e3; e3 = e2;
		Element e4 = e3;
		Element e5(e4);

		Section s1;
		Section s2(_T("s2"));
		Section s3; s3 = s2;
		Section s4 = s3;
		Section s5(s4);

		TEST(e1 != e2); TEST(s1 != s2);
		TEST(e2 == e3); TEST(s2 == s3);
		TEST(e3 == e4); TEST(s3 == s4);
		TEST(e4 == e5); TEST(s4 == s5);

		e1.name() = _T("key");
		e2.val() = _T("val");
		e3.prefix() = _T("prefix");
		e4.suffix() = _T("suffix");

		s1.name() = _T("key");
		s1.val() = _T("val");
		s1.prefix() = _T("prefix");
		s1.suffix() = _T("suffix");

		// read and write
		e1.name()  = const_cast<const Section&>(s1).name();
		e2.val()   = const_cast<const Section&>(s2).val();
		e3.prefix() = const_cast<const Section&>(s3).prefix();
		e4.suffix() = const_cast<const Section&>(s4).suffix();

		// implicit conversion to string_type
		const String es2 = _T("test");
		e2 = es2; TEST(e2.val() == es2);
		String es3 = e2.val();
		TEST(es3 == es2);

		TEST(e2.name() == e2.fullName());
		TEST(s2.name() == s2.fullName());
	}

	{ // basic operations
		Section s1;

		TEST(s1.sections.empty());
		TEST(s1.elements.empty());

		//TEST(s1.sections.begin() == static_cast<const CfgSection&>(s1).sections.end());
		//TEST(s1.elements.begin() == static_cast<const CfgSection&>(s1).elements.end());

		SectionList::const_iterator si1 = s1.sections.begin();
		ElementList::iterator si2 = s1.elements.begin();
		// ++si1; --si1; si1+=2; si1-=2;
		TEST(si1 == s1.sections.begin());
		// ++si2; --si2; si2+=2; si2-=2;
		TEST(si2 == s1.elements.begin());

		// push_front
		s1.elements.push_back(_T("key1")) = _T("val1");
		ElementList::const_iterator it = s1.elements.begin();
		TEST(++it == s1.elements.end());

		// push_back
		s1.elements.push_back( _T("key2")).val() = _T("val2");
		size_t d = std::distance(s1.elements.begin(), s1.elements.end());
		TEST(d == s1.elements.size() && d==2U);

		// ???
		s1.sections.push_back(s1);
		s1.sections.push_back(s1);
		TEST(2 == s1.sections.size());

		s1.elements.get(_T("key3"), true);
		s1.elements[_T("key3")].val() = _T("val3");

		const Section &s3 = s1;
		s3.elements[_T("key3")].val();

		TEST(s3.elements.exists(_T("key3")));
		s1.elements.remove(s1.elements[_T("key3")]);
		TEST(!s3.elements.exists(_T("key3")));
	}

#if OMNI_UNICODE
	typedef std::wostringstream ostringstream_type;
	typedef std::wistringstream istringstream_type;
	typedef std::wstring string_type;
#else
	typedef std::ostringstream ostringstream_type;
	typedef std::istringstream istringstream_type;
	typedef std::string string_type;
#endif

	{ // print and parse
		Section s1;
		ostringstream_type os1; os1 << s1;
		istringstream_type is1(os1.str());
		Section s1_; is1 >> s1_;
		TEST(s1 == s1_);

		Section s2;
		s2.elements[_T("key1?")].val() = _T("val1");
		s2.elements[_T("key2")].val() = _T(">val2");
		ostringstream_type os2; os2 << s2;
		istringstream_type is2(os2.str());
		Section s2_; is2 >> s2_;
		TEST(s2 == s2_);

		Section s3;
		s3.sections[_T("s1")].val() = _T("val1");
		s3.sections[_T("s2")].prefix() = _T("prefix");
		s3.sections[_T("s3")].suffix() = _T("suffix");
		s3.sections[_T("s4")].sections[_T("s4s1")].val() = _T("val2");
		ostringstream_type os3; os3 << s3;
		istringstream_type is3(os3.str());
		Section s3_; is3 >> s3_;
		TEST(s3 == s3_);
	}

	{ // parse testing
		const string_type str1 = _T("");
		istringstream_type istr1(str1);
		Section s1; istr1 >> s1;
		Section s1_;
		TEST(s1 == s1_);

		const string_type str2 = _T(" #prefix\n<s1> \n </s1>#suffix");
		istringstream_type istr2(str2);
		Section s2; istr2 >> s2;
		Section s2_;
		s2_.sections[_T("s1")].prefix() = _T("prefix");
		s2_.sections[_T("s1")].suffix() = _T("suffix");
		TEST(s2 == s2_);

		const string_type str3 = _T(" #prefix\n<s1 \t= \nval1  \n />#suffix");
		istringstream_type istr3(str3);
		Section s3; istr3 >> s3;
		Section s3_;
		s3_.sections[_T("s1")].val() = _T("val1");
		s3_.sections[_T("s1")].prefix() = _T("prefix");
		s3_.sections[_T("s1")].suffix() = _T("suffix");
		TEST(s3 == s3_);

		const string_type str4 = _T("<s1 key1=val1 />");
		istringstream_type istr4(str4);
		Section s4; istr4 >> s4;
		Section s4_;
		s4_.sections[_T("s1")].elements[_T("key1")].val() = _T("val1");
		TEST(s4 == s4_);

		const string_type str5 = _T("#prefix\nkey1=val1 key2=val2#suffix");
		istringstream_type istr5(str5);
		Section s5; istr5 >> s5;
		Section s5_;
		s5_.elements[_T("key1")].val() = _T("val1");
		s5_.elements[_T("key2")].val() = _T("val2");
		s5_.elements[_T("key1")].prefix() = _T("prefix");
		s5_.elements[_T("key2")].suffix() = _T("suffix");
		TEST(s5 == s5_);

		const string_type str6 = _T("key1 key2\n#prefix\nkey3#suffix");
		istringstream_type istr6(str6);
		Section s6; istr6 >> s6;
		Section s6_;
		s6_.elements[_T("key1")];
		s6_.elements[_T("key2")];//.val() = _T("val2");
		s6_.elements[_T("key3")].prefix() = _T("prefix");
		s6_.elements[_T("key3")].suffix() = _T("suffix");
		TEST(s6 == s6_);
	}

	{ // exceptions
		const Section s1;

		try { s1.sections[_T("s")]; return false; }
		catch (const err::ElementNotFound&) { /*ok*/ }

		try { s1.elements[_T("e")]; return false; }
		catch (const err::ElementNotFound&) { /*ok*/ }

		Section s2;
		s2.elements.push_back(_T("key")).val() = _T("val1");
		s2.elements.push_back(_T("key")).val() = _T("val2");

		try { s2.elements[_T("key")]; return false; }
		catch (const err::NameIsAmbiguous&) { /*ok*/ }
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
