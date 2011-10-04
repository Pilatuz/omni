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
	@brief The unit-test of "pool.hpp".

@author Sergey Polichnoy
*/
#include <omni/pool.hpp>
#include <test/test.hpp>

#include <ostream>
#include <vector>
#include <list>
#include <map>

#include <stdlib.h>

template class std::vector<double, omni::pool::Allocator<double> >;
template class std::vector<int, omni::pool::Allocator<int> >;
template class std::list<double, omni::pool::Allocator<double> >;
template class std::list<int, omni::pool::Allocator<int> >;

#if (_MSC_VER <= 1200)
	template class std::map<int, double, std::less<int>,
		omni::pool::Allocator<double> >;
	template class std::map<int, int, std::less<int>,
		omni::pool::Allocator<int> >;
#else
	template class std::map<int, double, std::less<int>,
		omni::pool::Allocator< std::pair<const int, double> > >;
	template class std::map<int, int, std::less<int>,
		omni::pool::Allocator< std::pair<const int, int> > >;
#endif

namespace
{
	// FastObjT test
	class FTest:
		public omni::pool::FastObjT<FTest>
	{
	public:
		int v[1024];
	};
}


// test function
bool test_pool(std::ostream &os)
{
	using namespace omni::pool;

	{ // first test
		ObjPool<4> op;

		op.grow(0, 1024);
		op.put(op.get());
	}

	{ // first test
		ObjPool<64> op;

		op.grow(0, 1024);
		op.put(op.get());
	}

	{ // FastObjT
		FTest *p1 = new FTest();
		FTest *p2 = new FTest();

		delete p1;
		delete p2;

		p1 = new FTest();
		p2 = new FTest();
		delete p1;
		delete p2;
	}

	typedef std::vector<void*> BufList;
	typedef std::map<size_t, BufList> BufMap;

	for (size_t i = 0; i <= 4096; ++i)
		mem_put(mem_get(i), i);
	//if (N_used() != 0)
	//	return false;
	os;

	BufList sized_bufs;
	BufMap bufs;
	for (size_t i = 0; i < 1000; ++i)
	{
		size_t bufSize = rand()%(4096+1);
		void *buf;

		buf = mem_get(bufSize);
		bufs[bufSize].push_back(buf);

		buf = mem_get_sized(bufSize);
		sized_bufs.push_back(buf);
	}

	for (size_t i = 0; i < sized_bufs.size(); ++i)
		mem_put_sized(sized_bufs[i]);

	for (BufMap::iterator it = bufs.begin(); it != bufs.end(); ++it)
	{
		size_t bufSize = it->first;
		BufList buf_list = it->second;

		for (size_t i = 0; i < buf_list.size(); ++i)
			mem_put(buf_list[i], bufSize);
	}
	//if (0 < N_used())
	//	return false;

	{ // TestObj
		struct TestObj:
			public FastObj
		{
			TestObj(): value(0) {}
			TestObj(int v): value(v) {}
			int value;
		};

		TestObj *p1 = new TestObj();
		TestObj *p2 = new TestObj(777);
		p1->value = 777;

		if (p1->value != p2->value)
			return false;

		delete p1;
		delete p2;
	}

	// omni::ObjPool::statistics(os);
	return true;
}


namespace
{
	// Test1 class
	class Test1:
		public omni::test::UnitTest
	{
		// test title
		virtual char const* title() const
		{
			return "omni::pool";
		}

		// test function
		virtual bool do_test(std::ostream &os) const
		{
			return test_pool(os);
		}
	} test1;

} // namespace
