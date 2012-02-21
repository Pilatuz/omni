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
@brief The unit-test of <omni/misc.hpp>.
@author Sergey Polichnoy <pilatuz@gmail.com>
*/
#include <omni/misc.hpp>
#include <test/test.hpp>

#include <iostream>

#include <time.h>

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
	omni::misc::EventsImpl<MyEvents> m_events;

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

} // locals


// unit test
namespace
{

///////////////////////////////////////////////////////////////////////////////
/// @brief The omni::misc unit test.
class MiscTest:
	public omni::test::UnitTest
{
private:

	// test title
	virtual const char* title() const
	{
		return "omni::misc";
	}

private:

	// test function
	virtual bool do_test(std::ostream &os) const
	{
		{ // events
			os << " Events test...";

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

			if (hh.m_hits != 6666)
				return false;
			os << "done\n";
		}

		{ // timer test
			os << " Timer test...";
			const int T = 100;

			omni::misc::Timer t(T);
			if (t.interval() != T)
				return false;
			const clock_t t0 = clock();
			while (!t.elapsed());
			const clock_t t1 = clock();

			const int TT = (t1-t0)/(CLOCKS_PER_SEC/1000);
			const int dt = T - TT;
			if (dt < -T/2 || T/2 < dt)
				return false;
			os << "done\n";
		}

		return true;
	}
};

	// global instance
	MiscTest g_MiscTest;

} // unit test
