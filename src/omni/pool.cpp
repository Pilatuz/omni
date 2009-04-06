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
#include <omni/pool.hpp>

namespace omni
{

//////////////////////////////////////////////////////////////////////////
/// @brief Fast memory manager.
/**
		This namespace contains fast memory manager tools.

@see @ref omni_pool
*/
	namespace pool
	{
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


/// @brief Global pool manager type.
typedef omni::pool::Manager<sizeof(void*),
	sizeof(void*), 1024, DEFAULT_CHUNK_SIZE> GManager;


//////////////////////////////////////////////////////////////////////////
/// @brief Get the global pool manager.
/**
		This function returns reference to the global pool manager.

@return The global pool manager.
*/
GManager& g_pool()
{
	static GManager G;
	return G;
}

		} // details namespace
	} // pool namespace
} // omni namespace


namespace omni
{
	// global functions
	namespace pool
	{
		using details::g_pool;


//////////////////////////////////////////////////////////////////////////
/**
		This function allocates the memory block of @a buf_size bytes.
	If the memory block size is less than or equal to 4 Kbytes, then global
	pool manager will be used, otherwise @b new operator will be used.

		To release allocated memory block you should call mem_put()
	function and provide the same memory block size!

		In debug version the whole memory block is filled by @a 0xAA value.

@param[in] buf_size The memory block size in bytes.
@return The allocated memory block.

@see mem_put()
@see mem_get_sized()
*/
void* mem_get(size_t buf_size)
{
	void *buf;

	if (buf_size <= details::GManager::MAX_SIZE)
		buf = g_pool().get(buf_size);
	else
		buf = ::operator new(buf_size);

	OMNI_DEBUG_CODE(memset(buf, 0xAA, buf_size));

	return buf;
}


//////////////////////////////////////////////////////////////////////////
/**
		This function releases the memory block @a buf of @a buf_size bytes.
	If the memory block size is less than or equal to 4 Kbytes, then global
	pool manager will be used, otherwise @b delete operator will be used.

		The memory block size @a buf_size should be the same as when calling mem_get()!

		In debug version the whole memory block is filled by @a 0xAA value.

@param[in] buf The memory block.
@param[in] buf_size The memory block size in bytes.

@see mem_get()
*/
void mem_put(void *buf, size_t buf_size)
{
	OMNI_DEBUG_CODE(memset(buf, 0xAA, buf_size));

	if (buf_size <= details::GManager::MAX_SIZE)
		g_pool().put(buf, buf_size);
	else
		::operator delete(buf);
}


//////////////////////////////////////////////////////////////////////////
/**
		This function allocates the memory block of @a buf_size bytes.
	If the memory block size is less than or equal to 4 Kbytes, then global
	pool manager will be used, otherwise @b new operator will be used.

		To release allocated memory block you should
	call mem_put_sized() function!

		In debug version the whole memory block is filled by @a 0xAA value.

@param[in] buf_size The memory block size in bytes.
@return The allocated memory block.

@see mem_put_sized()
@see mem_get()
*/
void* mem_get_sized(size_t buf_size)
{
	typedef details::GManager::pool_type pool_type;
	size_t aux_size = pool_type::align(sizeof(size_t));

	void* buf = mem_get(buf_size + aux_size);
	typedef details::RAW<size_t> xRAW;
	xRAW::write(buf, buf_size);

	return static_cast<char*>(buf) + aux_size;
}


//////////////////////////////////////////////////////////////////////////
/**
		This function releases the memory block @a buf.
	If the memory block size is less than or equal to 4 Kbytes, then global
	pool manager will be used, otherwise @b delete operator will be used.

		In debug version the whole memory block is filled by @a 0xAA value.

@param[in] buf The memory block.

@see mem_get_sized()
*/
void mem_put_sized(void *buf)
{
	typedef details::GManager::pool_type pool_type;
	size_t aux_size = pool_type::align(sizeof(size_t));

	buf = static_cast<char*>(buf) - aux_size;
	typedef details::RAW<size_t> xRAW;
	size_t buf_size = xRAW::read(buf);

	mem_put(buf, buf_size + aux_size);
}

	} // global functions


	// FastObj
	namespace pool
	{

//////////////////////////////////////////////////////////////////////////
/// @brief Trivial constructor.
FastObj::FastObj()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief Trivial virtual destructor.
FastObj::~FastObj()
{}


//////////////////////////////////////////////////////////////////////////
/// @brief The memory allocation.
/**
		This operator allocates the @a buf_size bytes memory block
	using mem_get_sized() function.

	@throw std::bad_alloc If no memory is available.
	@param[in] buf_size The memory block size.
	@return The memory block.
*/
void* FastObj::operator new(size_t buf_size) // throw(std::bad_alloc)
{
	return mem_get_sized(buf_size);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The memory allocation.
/**
		This operator allocates the @a buf_size bytes memory block
	using mem_get_sized() function.

		If there is no available memory, then this operator
	will return null pointer.

	@param[in] buf_size The memory block size.
	@return The memory block or null.
*/
void* FastObj::operator new(size_t buf_size, std::nothrow_t const&) // throw()
{
	try
	{
		return mem_get_sized(buf_size);
	}
	catch (std::bad_alloc const&)
	{}

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
void* FastObj::operator new(size_t buf_size, void *p) // throw()
{
	return ::operator new(buf_size, p);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The memory deallocation.
/**
		This operator deallocates the @a buf memory block
	using mem_put_sized() function.

@param[in] buf The memory block.
*/
void FastObj::operator delete(void *buf)
{
	mem_put_sized(buf);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The memory deallocation.
/**
		This operator deallocates the @a buf memory block
	using mem_put_sized() function.

@param[in] buf The memory block.
*/
void FastObj::operator delete(void *buf, std::nothrow_t const&) // throw()
{
	mem_put_sized(buf);
}


//////////////////////////////////////////////////////////////////////////
/// @brief Placement delete operator?
/**
		This operator is used only for correct overloading of
	other @b delete operators. It uses global placement @b delete operator.

@param[in] buf The memory block?
@param[in] p The memory block?
*/
void FastObj::operator delete(void *buf, void *p) // throw()
{
	::operator delete(buf, p);
}

	} // FastObj

} // omni namespace



//////////////////////////////////////////////////////////////////////////
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
