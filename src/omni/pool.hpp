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
@brief Fast memory management.
@author Sergey Polichnoy <pilatuz@gmail.com>
@see @ref omni_pool
*/
#ifndef __OMNI_POOL_HPP_
#define __OMNI_POOL_HPP_

#include <omni/defs.hpp>

#include <assert.h>

#include <memory>
#include <new>

#include <Windows.h>

namespace omni
{
	/// @brief Fast memory manager.
	/**
	@see @ref omni_pool
	*/
	namespace pool
	{
		/// @brief Memory manager: implementation.
		namespace details
		{

///////////////////////////////////////////////////////////////////////////////
/// @brief Constants.
enum
{
	/// @brief Default chunk size. @hideinitializer
	DEF_CHUNK_SIZE = 64*1024 // 64KB
};


///////////////////////////////////////////////////////////////////////////////
/// @brief Round-up to the next integer power of 2.
/**
		This function is compile-time equivalent of the utils::clp2() function.

@tparam X The input, should be in range [0..2^32).
*/
template<size_t X>
class CLP2
{
private: // see utils::clp2() function

	enum
	{
		A1 = X - 1,
		A2 = A1 | (A1>>1),
		A3 = A2 | (A2>>2),
		A4 = A3 | (A3>>4),
		A5 = A4 | (A4>>8),
		Y  = A5 | (A5>>16)
	};

public: // result

	enum
	{
		/// @brief The result. @hideinitializer
		RESULT = Y+1
	};
};


// zero specialization
template<>
class CLP2<0>
{
public:
	enum
	{
		RESULT = 1
	};
};

		} // details namespace
	} // pool namespace


	// ObjPool
	namespace pool
	{

///////////////////////////////////////////////////////////////////////////////
/// @brief The pool of fixed-size memory blocks.
/**
		The class manages the fixed-size memory blocks. These memory blocks
	are organized as a single-linked list (list of unused blocks or pool).
	The get() method returns the first memory block from the pool.
	The put() method puts the memory block back into the pool.
	If there are no unused memory blocks (i.e. the pool is empty),
	then you should call grow() method. The grow() method allocates
	memory chunk (several adjacent memory blocks) and puts these
	blocks into the list of unused blocks.

		For chunk allocation/deallocation global
	@b new / @b delete operators are used.

		The template parameter @a A is an alignment of memory blocks. It should
	be integer power of two: 1, 2, 4, 8, 16, ...

		The class does not contain the memory block size. So you should provide
	the correct block size each time the grow() method is called.

		As an example of using ObjPool class see implementation of FastObjT class.

@see @ref omni_pool
*/
template<size_t A> // A - alignment (for example: 1, 4, 16)
class ObjPool:
	private omni::NonCopyable
{
public:
	typedef size_t size_type; ///< @brief Size type.
	typedef void* pointer;    ///< @brief Pointer type.

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief The constants.
	enum Const
	{
		///< @brief The base alignemnt. @hideinitializer
		BASE_ALIGNMENT = details::CLP2<A>::RESULT,

		/// @brief Alignment of memory blocks. @hideinitializer
		ALIGNMENT = BASE_ALIGNMENT < MEMORY_ALLOCATION_ALIGNMENT
			? MEMORY_ALLOCATION_ALIGNMENT : BASE_ALIGNMENT
	};

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		The constructor initializes an empty pool. The empty pool has no unused
	memory blocks. So, before getting the memory block you should call
	grow() method.

@see grow()
*/
	ObjPool()
#if OMNI_DEBUG
		: m_obj_size(0),
		  m_N_used(0)
#endif
	{
		::InitializeSListHead(&m_chunks);
		::InitializeSListHead(&m_unused);
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
/**
		The destructor releases all (used and unused) memory blocks.

	@warning Make sure that all memory blocks are returned to the pool.

		In debug version there is a memory leak checking.
*/
	~ObjPool()
	{
#if OMNI_DEBUG
		assert(0 == m_N_used
			&& "memory leak");
#endif

		while (pointer p = ::InterlockedPopEntrySList(&m_chunks))
			release_chunk(p);
	}

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief Grow the pool.
/**
		One memory chunk is several adjacent memory blocks. This method
	allocates one memory chunk and puts these blocks into the list
	of unused blocks.

		Allocated memory chunk contains at least one memory block.

		The ObjPool class does not contain memory block size, so, you should
	provide the correct memory block size each time grow() method is called. You should
	specify the same memory block size @a obj_size each time grow() is called.
	Otherwise your program will have undefined behavior.

@param[in] obj_size The memory block size in bytes.
@param[in] chunk_size Approximate memory chunk size in bytes.
*/
	void grow(size_type obj_size, size_type chunk_size = details::DEF_CHUNK_SIZE)
	{
		const size_type ptr_size = sizeof(SLIST_ENTRY);
		const size_type aux_size = ptr_size + ALIGNMENT-1;
		obj_size = align(obj_size ? obj_size : 1);

#if OMNI_DEBUG
		if (!m_obj_size)
			m_obj_size = obj_size;
		assert(m_obj_size == obj_size
			&& "invalid block size");
#endif

		// number of blocks
		size_type No = (chunk_size - aux_size) / obj_size;
		if (!No) No = 1; // (!) one block minimum

		char *chunk = static_cast<char*>(alloc_chunk(aux_size + No*obj_size));

		// single-linked list of chunks
		::InterlockedPushEntrySList(&m_chunks,
			reinterpret_cast<PSLIST_ENTRY>(chunk));

		// update list of unused elements
		chunk = static_cast<char*>(align(chunk + ptr_size)); // "useful" memory
		for (size_type i = 0; i < No; ++i)
			put(chunk + (No-1-i)*obj_size); // (!) locality
		OMNI_DEBUG_CODE(::InterlockedExchangeAdd(&m_N_used, LONG(No)));  // (!) put() makes (--m_N_used);
	}

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief Get the memory block from the pool.
/**
		This method returns the first unused memory block.
	If the pool is empty the null pointer will be return.
	In this case call the grow() method and then get() again.

@return Pointer to the memory block or null.

@see grow()
*/
	pointer get()
	{
		// pop from the list
		pointer pObj = ::InterlockedPopEntrySList(&m_unused);

#if OMNI_DEBUG
		if (pObj)
		{
			::InterlockedIncrement(&m_N_used);
		}
#endif

		return pObj;
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Put the memory block back into the pool.
/**
		This method puts the memory block @a pObj back into the pool.
	The memory block is inserted into the beginning of the unused blocks list.
	So, next call of the get() method will return memory block @a pObj again.

@param[in] pObj Pointer to the memory block.
*/
	void put(pointer pObj)
	{
		assert(pObj == align(pObj)
			&& "invalid block alignemnt");

		// push to the list
		::InterlockedPushEntrySList(&m_unused,
			reinterpret_cast<PSLIST_ENTRY>(pObj));

#if OMNI_DEBUG
		::InterlockedDecrement(&m_N_used);
#endif
	}

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief Align block size.
/**
		This method aligns the memory block size @a obj_size.

@param[in] obj_size The memory block size.
@return The aligned memory block size.
*/
	static size_type align(size_type obj_size)
	{
		return (obj_size + ALIGNMENT-1) & ~size_type(ALIGNMENT-1);
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Align block pointer.
/**
		This method aligns the memory block pointer @a ptr.

@param[in] ptr The memory block pointer.
@return The aligned memory block pointer.
*/
	static pointer align(pointer ptr)
	{
		const size_type x = reinterpret_cast<size_type>(ptr);
		return reinterpret_cast<pointer>(align(x));
	}

private:

///////////////////////////////////////////////////////////////////////////////
/// @brief Allocate memory chunk.
/**
		This method allocates @a chunk_size bytes memory chunk.

@param[in] chunk_size The chunk size in bytes.
@return Pointer to allocated chunk.
*/
	static pointer alloc_chunk(size_type chunk_size)
	{
		return _aligned_malloc(chunk_size, ALIGNMENT);
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Release memory chunk.
/**
		This method releases memory chunk @a pChunk.

@param[in] pChunk Pointer to the chunk.
*/
	static void release_chunk(pointer pChunk)
	{
		_aligned_free(pChunk);
	}

private:
	SLIST_HEADER m_unused; ///< @brief The list of unused memory blocks.
	SLIST_HEADER m_chunks; ///< @brief The list of memory chunks.

#if OMNI_DEBUG
	size_t m_obj_size; ///< @brief The object size.
	LONG m_N_used; ///< @brief The total number of memory blocks used.
#endif // OMNI_DEBUG
};

	} // ObjPool


	// FastObjT
	namespace pool
	{

///////////////////////////////////////////////////////////////////////////////
/// @brief Fast object with individual pool.
/**
		The FastObjT class contains individual pool object.
	The @b new / @b delete operators of this class use this individual pool object.

		To use fast memory management your class should be derived
	from the FastObjT<> class. See the example below.

		Since the pool object can manage only one memory block size,
	the FastObjT class can't be used with polymorphic classes.

@tparam T Object type. This object type is used to determine memory
	block size. The memory block size is equal to @b sizeof(T).

@tparam A Alignment of pointers. Should be integer power of two.
@tparam CS Approximate memory chunk size in bytes.

	Example of using the FastObjT class:

@code
	class Packet:
		public omni::pool::FastObjT<Packet>
	{
	public:
		char data[188]; // 188 bytes data payload
		int a, b;       // custom parameters
	};

	void f()
	{
		Packet *p1 = new Packet(); // ObjPool::get() used
		// ...
		delete p1;                 // ObjPool::put() used
		// ...
		Packet *p2 = new Packet(); // ObjPool::get() used
		// ...
		delete p2;                 // ObjPool::put() used
	}
@endcode

		Note: The Packet class is derived from FastObjT<Packet>.

@see @ref omni_pool
*/
template<typename T, size_t A = sizeof(void*),
	size_t CS = details::DEFAULT_CHUNK_SIZE>
class FastObjT
{
public:
	typedef ObjPool<A> pool_type; ///< @brief The pool type.

///////////////////////////////////////////////////////////////////////////////
/// @brief Constants.
	enum
	{
		ALIGNMENT = pool_type::ALIGNMENT, ///< @brief The objects alignment. @hideinitializer
		CHUNK_SIZE = CS ///< @brief Approximate chunk size. @hideinitializer
	};

protected:

///////////////////////////////////////////////////////////////////////////////
/// @brief Trivial constructor.
	FastObjT()
	{}

///////////////////////////////////////////////////////////////////////////////
/// @brief Trivial destructor.
	~FastObjT()
	{}

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief The memory allocation.
/**
		This operator allocates the @a buf_size bytes memory block.
	Argument @a buf_size should be less than or equal to the @b sizeof(T)!

@throw std::bad_alloc If there's no available memory.
@param[in] buf_size The memory block size.
@return The memory block.
*/
	static void* operator new(size_t buf_size) // throw(std::bad_alloc);
	{
		assert(buf_size <= sizeof(T)
			&& "invalid object size");
		buf_size; // argument not used

		return alloc();
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief The memory allocation.
/**
		This operator allocates the @a buf_size bytes memory block.
	Argument @a buf_size should be less than or equal to the @b sizeof(T)!

		If there is no available memory, then this operator
	will return null pointer.

@param[in] buf_size The memory block size.
@return The memory block or null.
*/
	static void* operator new(size_t buf_size, std::nothrow_t const&) // throw();
	{
		assert(buf_size <= sizeof(T)
			&& "invalid object size");
		buf_size; // argument not used

		try
		{
			return alloc();
		}
		catch (std::bad_alloc const&)
		{}

		return 0;
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Placement new operator.
/**
		This operator is used only for correct overloading of
	other @b new operators. It uses global placement @b new operator.

@param[in] buf_size The memory block size.
@param[in] p The memory block.
@return The memory block.
*/
	static void* operator new(size_t buf_size, void *p) // throw();
	{
		return ::operator new(buf_size, p);
	}

public:

///////////////////////////////////////////////////////////////////////////////
///@brief The memory deallocation.
/**
		This operator deallocates the @a buf memory block.

@param[in] buf The memory block.
*/
	static void operator delete(void *buf)
	{
		obj_pool().put(buf);
	}

///////////////////////////////////////////////////////////////////////////////
///@brief The memory deallocation.
/**
		This operator deallocates the @a buf memory block.

@param[in] buf The memory block.
*/
	static void operator delete(void *buf, std::nothrow_t const&) // throw();
	{
		obj_pool().put(buf);
	}

///////////////////////////////////////////////////////////////////////////////
/// @brief Placement delete operator?
/**
		This operator is used only for correct overloading of
	other @b delete operators. It uses global placement @b delete operator.

@param[in] buf The memory block?
@param[in] p The memory block?
*/
	static void operator delete(void *buf, void *p) // throw();
	{
		::operator delete(buf, p);
	}

private:

///////////////////////////////////////////////////////////////////////////////
/// @brief The individual pool object.
/**
		This static method returns the ObjPool object.
	This pool object is used for memory management operations.

@return The static ObjPool object.
*/
	static pool_type& obj_pool()
	{
		static pool_type G;
		return G;
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Get memory block.
/**
		This static method returns the memory block from the individual pool.
	The size of allocated memory block is equal to @b sizeof(T).

@return The memory block.
*/
	static void* alloc()
	{
		pool_type &x = obj_pool();

		void *p = x.get();
		while (!p) // unlikely
		{
			x.grow(sizeof(T),
				CHUNK_SIZE);
			p = x.get();
		}

		return p;
	}
};

	} // FastObjT


	// Manager
	namespace pool
	{

///////////////////////////////////////////////////////////////////////////////
/// @brief The pool manager.
/**
		The Manager class contains and manages several pool objects.
	The number of managed pool objects is equal to POOL_SIZE.

		The granularity argument is the difference between block sizes
	of the two adjacent pool objects. For example, if granularity is 2,
	then pools are 2, 4, 6, 8, ... bytes. If granularity is 4, then
	pools are 4, 8, 12, 16, ... bytes. It is recommended to set granularity
	to the alignment.

@param A Alignment of pointers. Should be integer power of two.
@param G Granularity of memory block sizes. Recommended as an alignment.
@param PS Total number of managed pool objects.
@param CS Approximate chunk size in bytes.

@see @ref omni_pool
*/
template<size_t A, size_t G, size_t PS = 1024, size_t CS = details::DEFAULT_CHUNK_SIZE> // A - alignment
class Manager:
	private omni::NonCopyable
{
public:

///////////////////////////////////////////////////////////////////////////////
/// @brief Constants.
	enum
	{
		MAX_SIZE = G*PS, ///< @brief Maximum available block size. @hideinitializer
		GRANULARITY = G, ///< @brief Block size granularity. @hideinitializer
		CHUNK_SIZE = CS, ///< @brief Approximate chunk size. @hideinitializer
		POOL_SIZE = PS,  ///< @brief Total number of pools. @hideinitializer
		ALIGNMENT = ObjPool<A>::ALIGNMENT  ///< @brief Alignment of pointers. @hideinitializer
	};

public:
	typedef ObjPool<A> pool_type;            ///< @brief The pool type.
	typedef typename pool_type::size_type size_type; ///< @brief Size type.
	typedef typename pool_type::pointer   pointer;   ///< @brief Pointer type.

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief The default constructor.
/**
		Initializes all managed pools.
*/
	Manager()
	{}


///////////////////////////////////////////////////////////////////////////////
/// @brief The destructor.
/**
		Releases all managed pools.
*/
	~Manager()
	{}

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief Get the memory block.
/**
		This method gets the memory block from
	the corresponding managed pool object.

		The memory block size @a obj_size
	should be less than or equal to MAX_SIZE.

@param[in] obj_size The memory block size in bytes.
@return The memory block.

@see ObjPool::get()
*/
	pointer get(size_type obj_size)
	{
		assert(obj_size <= MAX_SIZE
			&& "object size too big");

		pool_type &obj_pool = find(obj_size); // (!) obj_size changed due to granularity!

		pointer pObj = obj_pool.get();
		while (!pObj) // unlikely
		{
			obj_pool.grow(obj_size, CHUNK_SIZE);
			pObj = obj_pool.get();
		}

		return pObj;
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Put the memory block.
/**
		This method puts the memory block @a obj back
	into the corresponding managed pool object.

		The memory block size @a obj_size
	should be less than or equal to MAX_SIZE.

@param[in] obj The memory block.
@param[in] obj_size The memory block size in bytes.

@see ObjPool::put()
*/
	void put(pointer obj, size_type obj_size)
	{
		assert(obj_size <= MAX_SIZE
			&& "object size too big");

		pool_type &obj_pool = find(obj_size);
		obj_pool.put(obj);
	}

private:

///////////////////////////////////////////////////////////////////////////////
/// @brief Find pool with specified block size.
/**
		This method finds the managed pool object
	by memory block size @a obj_size.

@param[in,out] obj_size The memory block size in bytes.
@return The pool object.
*/
	pool_type& find(size_type &obj_size)
	{
		const size_type x = !obj_size ? 0
			: (obj_size - 1) / GRANULARITY;
		obj_size = (x+1)*GRANULARITY;

		assert(x < POOL_SIZE
			&& "object size too big");
		return m_pools[x];
	}

private:
	pool_type m_pools[POOL_SIZE]; ///< @brief Managed pool objects.
};

	} // Manager


	// global pool manager
	namespace pool
	{

		void* mem_get(size_t buf_size);            ///< @brief Allocate the memory block.
		void mem_put(void *buf, size_t buf_size);  ///< @brief Release the memory block.

		void* mem_get_sized(size_t buf_size);      ///< @brief Allocate the memory block.
		void mem_put_sized(void *buf);             ///< @brief Release the memory block.

	} // global pool manager


	// FastObj
	namespace pool
	{

///////////////////////////////////////////////////////////////////////////////
/// @brief Fast object with global pool.
/**
		The FastObj class uses global pool. So, the FastObj class may
	be used with polymorphic classes in contrast to FastObjT class.
	Any allocation/deallocation operations use mem_get_sized()
	and mem_put_sized() functions.

		To use fast memory management your class should be derived
	from the FastObj class. See the example below.

@code
	class MyClass:
		public omni::pool::FastObj
	{
		// ...
	};

	class Test:
		public MyClass
	{
		// ...
	};

	void f()
	{
		MyClass *p1 = new MyClass(); // mem_get_sized() used
		// ...
		delete p1; // mem_put_sized() used

		Test *p2 = new Test(); // mem_get_sized() used
		// ...
		delete p2; // mem_put_sized() used
	}
@endcode

@see @ref omni_pool
*/
class FastObj
{
protected:
	FastObj();
	virtual ~FastObj();

public:
	static void* operator new(size_t buf_size); // throw(std::bad_alloc);
	static void* operator new(size_t buf_size, std::nothrow_t const&); // throw();
	static void* operator new(size_t buf_size, void *p); // throw();

	static void operator delete(void *buf);
	static void operator delete(void *buf, std::nothrow_t const&); // throw();
	static void operator delete(void *buf, void *p); // throw();
};

	} // FastObj


	// Allocator
	namespace pool
	{

///////////////////////////////////////////////////////////////////////////////
/// @brief Fast allocator for STL containers.
/**
		This class uses global pool and can be used with many STL containers.
	Any allocation/deallocation methods use mem_get_sized()
	and mem_put_sized() functions.

	For example:

@code
	std::vector<double, omni::pool::Allocator<double> > a;
	std::list<int, omni::pool::Allocator<int> > b;
@endcode
*/
template<typename T>
class Allocator
{
public:
	typedef T value_type; ///< @brief Value type.

	typedef T const& const_reference; ///< @brief Constant reference type.
	typedef       T&       reference; ///< @brief Reference type.
	typedef T const* const_pointer;   ///< @brief Constant pointer type.
	typedef       T*       pointer;   ///< @brief Pointer type.

	typedef ptrdiff_t difference_type;    ///< @brief Difference type.
	typedef size_t size_type; ///< @brief Size type.

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief Auxiliary structure.
/**
		This structure is used to change current allocator's type.
*/
	template<typename U>
	struct rebind
	{
		typedef Allocator<U> other; ///< @brief New allocator's type.
	};

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief Get address by constant reference.
/**
@param[in] x The constant reference.
@return The constant pointer.
*/
	const_pointer address(const_reference x) const
	{
		return &x;
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Get address by reference.
/**
@param[in] x The reference.
@return The pointer.
*/
	pointer address(reference x) const
	{
		return &x;
	}

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
	Allocator()
	{}


///////////////////////////////////////////////////////////////////////////////
/// @brief Auxiliary copy-constructor.
	template<typename U>
	Allocator(Allocator<U> const&)
	{}


///////////////////////////////////////////////////////////////////////////////
/// @brief Auxiliary assignment operator.
	template<typename U>
	Allocator<T>& operator=(Allocator<U> const&)
	{
		return (*this);
	}

public:

///////////////////////////////////////////////////////////////////////////////
/// @brief The memory allocation.
/**
		This method allocates memory block for @a n objects.

@param[in] n The number of adjacent objects.
@return The memory block.
*/
	pointer allocate(size_type n)
	{
		return static_cast<pointer>(mem_get_sized(n*sizeof(value_type)));
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief The memory allocation.
/**
		This method allocates memory block for @a n objects.
	Second argument (allocation hint) is not used.

@param[in] n The number of adjacent objects.
@return The memory block.
*/
	pointer allocate(size_type n, void const*)
	{
		return allocate(n);
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief The memory allocation.
/**
		This method allocates memory block of @a n bytes.
	It is used by STL containers in Visual C++ 6.0!

@param[in] n The memory block size in bytes.
@return The memory block.
*/
	char* _Charalloc(size_type n)
	{
		return static_cast<char*>(mem_get_sized(n));
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief The memory deallocation.
/**
		This method deallocates memory block @a p.
	the second argument (memory block size) is not used.

@param[in] p The memory block.
*/
	void deallocate(void *p, size_type)
	{
		mem_put_sized(p);
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Object construction.
/**
		This method constructs the object at address @a p
	using placement @b new operator.

@param[in] p The address of object.
@param[in] x Construction prototype.
*/
	void construct(pointer p, const_reference x)
	{
		new (static_cast<void*>(p)) value_type(x);
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Object destruction.
/**
		This method destroys object @a p.

@param[in] p The address of object.
*/
	void destroy(pointer p)
	{
		p->~T(); p;
	}


///////////////////////////////////////////////////////////////////////////////
/// @brief Maximum number of objects.
/**
		This method returns the maximum number of objects,
	which can be allocated using this allocator.

@return The maximum number of objects.
*/
	size_type max_size() const
	{
		return size_type(-1) / sizeof(value_type); // (?)
	}
};


///////////////////////////////////////////////////////////////////////////////
/// @brief Are two allocators equal?
/**
	The two allocators are always equal.

@return @b true.
*/
template<typename T, typename U> inline
bool operator==(Allocator<T> const&, Allocator<U> const&)
{
	return true;
}


///////////////////////////////////////////////////////////////////////////////
/// @brief Are two allocators non-equal?
/**
	The two allocators are always equal.

@return @b false.
*/
template<typename T, typename U> inline
bool operator!=(Allocator<T> const&, Allocator<U> const&)
{
	return false;
}

	} // Allocator

} // omni namespace


///////////////////////////////////////////////////////////////////////////////
/** @page omni_pool Fast memory management.

		The pool is useful if your program contains many number of small objects.
	These objects (for example, data packets) are created and destroyed dynamically.
	In this case the standard memory manager is not effective enough,
	because it is designed for various memory block sizes. The pool (also known
	as "node allocator") in this case is more effective, because it doesn't use
	real memory allocation/deallocation so often.

		The omni::pool::ObjPool class is a pool for one memory block size.
	The omni::pool::Manager class contains several pool objects, and
	therefore can manage memory blocks of various sizes (within a specific range).
	The omni::pool::FastObjT class overrides @b new / @b delete operators
	and contains a pool object. So if your class is derived from
	omni::pool::FastObjT, then fast memory management will be used.

		There are one global pool. The omni::pool::mem_get() and
	omni::pool::mem_put() functions use this global pool to allocate
	and deallocate memory blocks. The omni::pool::FastObj class overrides
	@b new / @b delete operators and uses the global pool.
	The omni::pool::Allocator can be used with STL containers, so these
	containers will use the global pool.
*/

#endif // __OMNI_POOL_HPP_
