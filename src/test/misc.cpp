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
	@brief The unit-test of "misc.hpp".

@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include <omni/misc.hpp>
#include <test/test.hpp>

#include <iostream>

namespace
{

/// @brief The document.
class Document
{
public:

	/// @brief The events set.
	struct MyEvents
	{
		virtual void onChanging(Document *pDoc) = 0;
		virtual void onChanged(Document *pDoc) = 0;
		virtual void onNewTitle(Document *pDoc, const char *title) = 0;
		virtual void onNewAuthor(Document *pDoc, const char *author, int flags) = 0;
	};

	omni::misc::Events<MyEvents>& events()
	{
		return m_events;
	}

private:
	omni::misc::EventsDriver<MyEvents> m_events;

public:
	void doChaning()
	{
		m_events.emit(&MyEvents::onChanging, this);
	}

	void doChanged()
	{
		m_events.emit(&MyEvents::onChanged, this);
	}

	void doNewTitle(const char *title)
	{
		m_events.emit(&MyEvents::onNewTitle, this, title);
	}

	void doNewAuthor(const char *author, int flags)
	{
		m_events.emit(&MyEvents::onNewAuthor, this, author, flags);
	}
};

/// @brief Events handler.
class Handler:
	public Document::MyEvents
{
public:
	Handler()
		: m_hits(0)
	{}

public:

	virtual void onChanging(Document *pDoc) { pDoc; m_hits += 1000; }
	virtual void onChanged(Document *pDoc)  { pDoc; m_hits += 100; }
	virtual void onNewTitle(Document *pDoc, const char *title) { pDoc; title; m_hits += 10; }
	virtual void onNewAuthor(Document *pDoc, const char *author, int flags) { pDoc; author; flags; m_hits += 1; }

public:
	int m_hits;
};

} // local namespace


// test function
bool test_misc(std::ostream &os)
{
	os << " Events Test...";

	Document doc;
	Handler hh;
	doc.events().insert(&hh);
	doc.events().insert(&hh);
	doc.events().insert(&hh);

	for (int i = 0; i < 4; ++i)
	{
		doc.doChaning();
		doc.doChanged();
		doc.doNewTitle("title");
		doc.doNewAuthor("author", 0);
		doc.events().remove(&hh);
	}

	os << "done\n";

	return hh.m_hits == 6666;
}


//////////////////////////////////////////////////////////////////////////
// unit test
namespace
{
	// Test1 class
	class Test1:
		public omni::test::UnitTest
	{
		// test title
		virtual const char* title() const
		{
			return "omni::misc";
		}

		// test function
		virtual bool do_test(std::ostream &os) const
		{
			return test_misc(os);
		}
	} test1;
} // namespace
