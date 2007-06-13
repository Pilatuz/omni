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
	@brief Fast memory management.

@author Sergey Polichnoy
@see @ref omni_pool
*/
#ifndef __OMNI_POOL_HPP_
#define __OMNI_POOL_HPP_

#include <omni/defs.hpp>

#include <assert.h>
#include <memory.h>

#include <new>

namespace omni
{
	namespace pool
	{
		/// @brief Implementation specific.
		namespace details
		{

//////////////////////////////////////////////////////////////////////////
/// @brief %RAW memory operations.
/**
		This class makes %RAW memory read/write operations easy.
	To read the value at custom address, you should use read() method.
	To write the value at custom address, you should use write() method.

		For example:

@code
	void f(void *ptr)
	{
		int a = RAW<int>::read(ptr); // read integer
		RAW<int>::write(ptr, a + 1); // write integer
	}
@endcode

		Template argument @a T should be of POD type.
*/
template<typename T>
class RAW {
public:
	typedef void* pointer; ///< @brief Pointer type.
	typedef T value_type;  ///< @brief Value type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Write the value.
/**
	This method writes the value @a x at the custom address @a p.

@param[in] p The custom address.
@param[in] x The value.
*/
	static void write(pointer p, value_type x)
	{
		*static_cast<value_type*>(p) = x;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Read the value.
/**
	This method reads the value at the custom address @a p.

@param[in] p The custom address.
@return The value.
*/
	static value_type read(pointer p)
	{
		return *static_cast<value_type*>(p);
	}
};


//////////////////////////////////////////////////////////////////////////
/// @brief Constants.
enum {
	/// @brief Default chunk size. @hideinitializer
	DEFAULT_CHUNK_SIZE = 16*4096 // = 64Kb
};

		} // details namespace
	} // pool namespace


	// ObjPool
	namespace pool
	{

//////////////////////////////////////////////////////////////////////////
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
class ObjPool: private omni::NonCopyable {
public:
	typedef size_t size_type; ///< @brief Size type.
	typedef void* pointer;    ///< @brief Pointer type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Constants.
	enum {
		ALIGNMENT = A ///< @brief Alignment of memory blocks. @hideinitializer
	};

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Construction.
/**
		The constructor initializes an empty pool. The empty pool has no unused
	memory blocks. So, before getting the memory block you should call
	grow() method.

@see empty()
@see grow()
*/
	ObjPool()
		: m_unused(0),
		  m_chunks(0)
#if OMNI_DEBUG
		, m_N_used(0)
#endif
	{
		assert(0!=ALIGNMENT && 0==(ALIGNMENT&(ALIGNMENT-1)) // see utils::is_ipow2()
			&& "alignment must be integer power of two");
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Destruction.
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

		while (m_chunks)
		{
			typedef details::RAW<pointer> xRAW;
			pointer next = xRAW::read(m_chunks);
			release_chunk(m_chunks);
			m_chunks = next;
		}
	}

public:

//////////////////////////////////////////////////////////////////////////
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

@see empty()
*/
	void grow(size_type obj_size, size_type chunk_size = details::DEFAULT_CHUNK_SIZE)
	{
		const size_type ptr_size = sizeof(pointer);
		const size_type aux_size = ptr_size + ALIGNMENT-1;
		obj_size = align(obj_size ? obj_size : 1);

		// number of blocks
		size_type No = (chunk_size - aux_size) / obj_size;
		if (!No) No = 1; // (?) minimum one block

		char *chunk = static_cast<char*>(alloc_chunk(No*obj_size + aux_size));

		// single-linked list of chunks
		typedef details::RAW<pointer> xRAW;
		xRAW::write(chunk, m_chunks);
		m_chunks = chunk;

		// update list of unused elements
		chunk = static_cast<char*>(align(chunk + ptr_size)); // "useful" memory
		for (size_type i = 0; i < No; ++i)
			put(chunk + (No-1-i)*obj_size); // (!) locality
		OMNI_DEBUG_CODE(m_N_used += No);  // (!) put() make (--m_N_used);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Is the pool empty?
/**
		This method checks the list of unused blocks. If the pool is empty,
	then you should call grow() method before using get().

		For example:

@code
	ObjPool<4> m_pool;
	size_t m_block_size;

	// ...

	void* my_alloc()
	{
		if (m_pool.empty())
			m_pool.grow(m_block_size);

		return m_pool.get();
	}
@endcode

@return @b true if the pool is empty, otherwise @b false.
*/
	bool empty() const
	{
		return (0 == m_unused);
	}

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get the memory block from the pool.
/**
		This method returns the first unused memory block.
	To ensure that the pool has unused memory block
	call empty() and grow() if needed.

@return Pointer to the memory block.

@see empty()
@see grow()
*/
	pointer get()
	{
		assert(0 != m_unused
			&& "pool is empty");

		pointer obj = m_unused;
		typedef details::RAW<pointer> xRAW;
		m_unused = xRAW::read(obj);

		OMNI_DEBUG_CODE(++m_N_used);
		return obj;
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Put the memory block back into the pool.
/**
		This method puts the memory block @a obj back into the pool.
	The memory block is inserted into the beginning of unused blocks list.
	So, next call of the get() method will return memory block @a obj again.

@param[in] obj Pointer to the memory block.
*/
	void put(pointer obj)
	{
		OMNI_DEBUG_CODE(--m_N_used);

		typedef details::RAW<pointer> xRAW;
		xRAW::write(obj, m_unused);
		m_unused = obj;
	}

public:

//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
/// @brief Allocate memory chunk.
/**
		This method allocates @a chunk_size bytes memory chunk using global
	@b new operator.

@param[in] chunk_size The chunk size in bytes.
@return Pointer to allocated chunk.
*/
	static pointer alloc_chunk(size_type chunk_size)
	{
		return ::operator new(chunk_size);
	}


//////////////////////////////////////////////////////////////////////////
/// @brief Release memory chunk.
/**
		This method releases memory chunk @a chunk using global
	@b delete operator.

@param[in] chunk Pointer to chunk.
*/
	static void release_chunk(pointer chunk)
	{
		::operator delete(chunk);
	}

private:
	pointer m_unused; ///< @brief List of unused memory blocks.
	pointer m_chunks; ///< @brief List of memory chunks.

#if OMNI_DEBUG
	size_type m_N_used; ///< @brief Total number of memory blocks used.
#endif // OMNI_DEBUG
};

	} // ObjPool


	// FastObjT
	namespace pool
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Fast object with individual pool.
/**
		The FastObjT class contains individual pool object.
	The @b new / @b delete operators of this class use this individual pool object.

		To use fast memory management your class should be derived
	from the FastObjT<> class. See the example below.

		Since the pool object can manage only one memory block size,
	the FastObjT class can't be used with polymorphic classes.

@param T Object type. This object type is used to determine memory
	block size. The memory block size is equal to @b sizeof(T).

@param A Alignment of pointers. Should be integer power of two.
@param CS Approximate memory chunk size in bytes.

	Example of using the FastObjT class:

@code
	class Packet: public omni::pool::FastObjT<Packet> {
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
	The same technique is widely used in WTL library.

@see @ref omni_pool
*/
template<typename T, size_t A = sizeof(void*),
	size_t CS = details::DEFAULT_CHUNK_SIZE>
class FastObjT {
public:
	typedef ObjPool<A> pool_type; ///< @brief The pool type.

//////////////////////////////////////////////////////////////////////////
/// @brief Constants.
	enum {
		CHUNK_SIZE = CS, ///< @brief Approximate chunk size. @hideinitializer
		ALIGNMENT = A    ///< @brief Alignment of objects. @hideinitializer
	};

protected:

//////////////////////////////////////////////////////////////////////////
/// @brief Trivial constructor.
	FastObjT()
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Trivial destructor.
	~FastObjT()
	{}

public:

//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
/// @brief The memory allocation.
/**
		This operator allocates the @a buf_size bytes memory block.
	Argument @a buf_size should be less than or equal to the @b sizeof(T)!

		If there is no available memory, then this operator
	will return null pointer.

@param[in] buf_size The memory block size.
@return The memory block or null.
*/
	static void* operator new(size_t buf_size, const std::nothrow_t&) // throw();
	{
		assert(buf_size <= sizeof(T)
			&& "invalid object size");
		buf_size; // argument not used

		try { return alloc(); }
		catch (const std::bad_alloc&) {}

		return 0;
	}


//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
///@brief The memory deallocation.
/**
		This operator deallocates the @a buf memory block.

@param[in] buf The memory block.
*/
	static void operator delete(void *buf)
	{
		obj_pool().put(buf);
	}

//////////////////////////////////////////////////////////////////////////
///@brief The memory deallocation.
/**
		This operator deallocates the @a buf memory block.

@param[in] buf The memory block.
*/
	static void operator delete(void *buf, const std::nothrow_t&) // throw();
	{
		obj_pool().put(buf);
	}

//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
/// @brief Get memory block.
/**
		This static method returns the memory block from the individual pool.
	The size of allocated memory block is equal to @b sizeof(T).

@return The memory block.
*/
	static void* alloc()
	{
		pool_type &x = obj_pool();

		if (x.empty())
			x.grow(sizeof(T),
				CHUNK_SIZE);

		return x.get();
	}
};

	} // FastObjT


	// Manager
	namespace pool
	{

//////////////////////////////////////////////////////////////////////////
/// @brief The pool manager.
/**
		The Manager class contains and manages several pool objects.
	The number of managed pool objects is equal to POOL_SIZE.

		The granularity argument is the difference between block sizes
	of the two adjacent pool objects. For example, if granularity is 2,
	then pools are 2, 4, 6, 8, ... bytes. If granularity is 4, then
	pools are 4, 8, 12, 16, ... bytes.

@param A Alignment of pointers. Should be integer power of two.
@param G Granularity of memory block sizes. Recommended 4 or 16.
@param PS Total number of managed pool objects.
@param CS Approximate chunk size in bytes.

@see @ref omni_pool
*/
template<size_t A, size_t G, size_t PS = 1024, size_t CS = details::DEFAULT_CHUNK_SIZE> // A - alignment
class Manager: private omni::NonCopyable {
public:

//////////////////////////////////////////////////////////////////////////
/// @brief Constants.
	enum {
		MAX_SIZE = G*PS, ///< @brief Maximum available block size. @hideinitializer
		GRANULARITY = G, ///< @brief Block size granularity. @hideinitializer
		CHUNK_SIZE = CS, ///< @brief Approximate chunk size. @hideinitializer
		POOL_SIZE = PS,  ///< @brief Total number of pools. @hideinitializer
		ALIGNMENT = A    ///< @brief Alignment of pointers. @hideinitializer
	};

public:
	typedef ObjPool<ALIGNMENT> pool_type;            ///< @brief The pool type.
	typedef typename pool_type::size_type size_type; ///< @brief Size type.
	typedef typename pool_type::pointer   pointer;   ///< @brief Pointer type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Construction.
/**
		The constructor initializes all managed pools.
*/
	Manager()
	{}


//////////////////////////////////////////////////////////////////////////
/// @brief Destruction.
/**
		The destructor destroys all managed pools.
*/
	~Manager()
	{}

public:

//////////////////////////////////////////////////////////////////////////
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

		pool_type &obj_pool = find(obj_size);

		if (obj_pool.empty())
			obj_pool.grow(obj_size,
				CHUNK_SIZE);

		return obj_pool.get();
	}


//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
/// @brief Find pool with specified block size.
/**
		This method finds the managed pool object
	by memory block size @a obj_size.

@param[in] obj_size The memory block size in bytes.
@return The pool object.
*/
	pool_type& find(size_type obj_size)
	{
		const size_type x = !obj_size ? 0
			: (obj_size - 1) / GRANULARITY;

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

//////////////////////////////////////////////////////////////////////////
/// @brief Fast object with global pool.
/**
		The FastObj class uses global pool. So, the FastObj class may
	be used with polymorphic classes in contrast to FastObjT class.
	Any allocation/deallocation operations use mem_get_sized()
	and mem_put_sized() functions.

		To use fast memory management your class should be derived
	from the FastObj class. See the example below.

	@code
		class MyClass: public omni::pool::FastObj {
			// ...
		};

		class Test: public MyClass {
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
class FastObj {
protected:
	FastObj();
	virtual ~FastObj();

public:
	static void* operator new(size_t buf_size); // throw(std::bad_alloc);
	static void* operator new(size_t buf_size, const std::nothrow_t&); // throw();
	static void* operator new(size_t buf_size, void *p); // throw();

	static void operator delete(void *buf);
	static void operator delete(void *buf, const std::nothrow_t&); // throw();
	static void operator delete(void *buf, void *p); // throw();
};

	} // FastObj


	// Allocator
	namespace pool
	{

//////////////////////////////////////////////////////////////////////////
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
class Allocator {
public:
	typedef T value_type; ///< @brief Value type.

	typedef const T& const_reference; ///< @brief Constant reference type.
	typedef       T&       reference; ///< @brief Reference type.
	typedef const T* const_pointer;   ///< @brief Constant pointer type.
	typedef       T*       pointer;   ///< @brief Pointer type.

	typedef ptrdiff_t difference_type;    ///< @brief Difference type.
	typedef size_t size_type; ///< @brief Size type.

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Auxiliary structure.
/**
		This structure is used to change current allocator's type.
*/
	template<typename U>
		struct rebind {
			typedef Allocator<U> other; ///< @brief New allocator's type.
		};

public:

//////////////////////////////////////////////////////////////////////////
/// @brief Get address by constant reference.
/**
@param[in] x The constant reference.
@return The constant pointer.
*/
	const_pointer address(const_reference x) const
	{
		return &x;
	}


//////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////
/// @brief Default constructor.
	Allocator()
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Auxiliary copy-constructor.
	template<typename U>
		Allocator(const Allocator<U>&)
	{}

//////////////////////////////////////////////////////////////////////////
/// @brief Auxiliary assignment operator.
	template<typename U>
		Allocator<T>& operator=(const Allocator<U>&)
	{
		return (*this);
	}

public:

//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
/// @brief The memory allocation.
/**
		This method allocates memory block for @a n objects.
	Second argument (allocation hint) is not used.

@param[in] n The number of adjacent objects.
@return The memory block.
*/
	pointer allocate(size_type n, const void*)
	{
		return allocate(n);
	}


//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
/// @brief Object destruction.
/**
		This method destroys object @a p.

@param[in] p The address of object.
*/
	void destroy(pointer p)
	{
		p->~T(); p;
	}


//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
/// @brief Are two allocators equal?
/**
	The two allocators are always equal.

@return @b true.
*/
template<typename T, typename U> inline
	bool operator==(const Allocator<T>&, const Allocator<U>&)
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
/// @brief Are two allocators non-equal?
/**
	The two allocators are always equal.

@return @b false.
*/
template<typename T, typename U> inline
	bool operator!=(const Allocator<T>&, const Allocator<U>&)
{
	return false;
}

	} // Allocator

} // omni namespace

#endif // __OMNI_POOL_HPP_
