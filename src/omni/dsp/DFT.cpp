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
	@brief Discrete Fourie Transform
	@author Sergey Polichnoy
*/
#include <omni/dsp/DFT.h>
#include <omni/util.hpp>

#include <functional>
#include <algorithm>

#if defined(OMNI_USE_MKL)
#include <mkl_dfti.h>
#endif

#if OMNI_MT && !defined(OMNI_USE_MKL)
#include <omni/sync.hpp>
using namespace omni::sync;
#endif

namespace omni
{
	namespace dsp
	{

#if !defined(OMNI_USE_MKL)
namespace details
{

//////////////////////////////////////////////////////////////////////////
// @brief DFT Table
template<typename T>
class DFT_Table {
public:
	typedef typename DFT<T>::scalar_type scalar_type;
	typedef typename DFT<T>::value_type value_type;
	typedef typename DFT<T>::size_type size_type;

public:
	// table creation
	explicit DFT_Table(size_type N)
		: m_table(N)
	{
		for (size_type i = 0; i < N; ++i)
			m_table[i] = std::polar(scalar_type(1),
				scalar_type(i*2*omni::util::PI / N));
	}

public:
	// get table item
	const value_type& operator[](size_type i) const
	{
		assert(i<size() && "index out of range");
		return m_table[i];
	}

	// get table size
	size_type size() const
	{
		return m_table.size();
	}

private:
	std::vector<value_type> m_table;
};


//////////////////////////////////////////////////////////////////////////
// @brief DFT Table manager
template<typename T>
class DFT_Manager: private omni::NonCopyable {
public:
	typedef DFT_Table<T> table_type;
	typedef typename table_type::size_type size_type;

public:
	~DFT_Manager()
	{
		// release all tables
		for (size_type i = 0; i < m_tables.size(); ++i)
		{
			assert(0==m_tables[i].N_refs && "table not used");
			delete m_tables[i].table;
		}
	}

	// release unused tables
	void clear()
	{
		for (size_type i = 0; i < m_tables.size(); ++i)
			if (0 == m_tables[i].N_refs)
		{
			delete m_tables[i].table;
			m_tables.erase(m_tables.begin()+i);
			--i; // (!)
		}
	}

	// get table
	table_type* get(size_type N)
	{
		Item &item = find(N);
		item.N_refs += 1;

		return item.table;
	}

	// release table
	void put(table_type *t)
	{
		Item &item = find(t->size());
		assert(0<item.N_refs && "table is used");
		item.N_refs -= 1;
	}

private:
	struct Item
	{
		table_type *table;
		size_type N_refs; // number of references

		explicit Item(table_type *t = 0)
			: table(t), N_refs(0) {}
	};

	struct FindCmp: public std::binary_function<Item, size_type, bool>
	{
		bool operator()(const Item &t, size_type N) const
		{
			return t.table->size() < N;
		}

		// VC8 "debug" check
		bool operator()(const Item &t1, const Item &t2) const
			{ return t1.table->size() < t2.table->size(); }
		bool operator()(size_type N, const Item &t) const
			{ return !(*this)(t, N); }
	};

private:
	// find table (create if not exists)
	Item& find(size_type N)
	{
		typename std::vector<Item>::iterator found = std::lower_bound(
			m_tables.begin(), m_tables.end(), N, FindCmp());

		if (found == m_tables.end() || (*found).table->size() != N)
			found = m_tables.insert(found, Item(new table_type(N)));

		return *found;
	}

private:
	std::vector<Item> m_tables;
};


//////////////////////////////////////////////////////////////////////////
// global synchronization
#if OMNI_MT
	CriticalSection& dft_lock()
	{
		static CriticalSection LOCK;
		return LOCK;
	}
#endif

//////////////////////////////////////////////////////////////////////////
// global table manager
template<typename T>
	DFT_Manager<T>& dft_tables(const T* = 0) // avoid VC6 bug
{
	static DFT_Manager<T> MANAGER;
	return MANAGER;
}


// @brief Danielson-Lanczos algorithm
template<bool is_fwd, typename T>
void fft_algorithm(std::complex<T> *data, size_t N_log2, const DFT_Table<T> &phase)
{
	const size_t MOD = (1<<N_log2) - 1;
	size_t NL = (1<<N_log2) / 2;
	size_t NR = 1;

	for (size_t i = 0; i < N_log2; ++i)
	{
		for (size_t L = 0; L < NL; ++L)
			for (size_t R = 0; R < NR; ++R)
			{
				size_t p = R + 2*L*NR;
				size_t q = p + NR;

				std::complex<T> tmp = is_fwd
					? std::conj(phase[(R*NL) & MOD])
					: phase[(R*NL) & MOD];

				tmp *= data[q];
				data[q] = data[p] - tmp;
				data[p] += tmp;
			}

		NL /= 2;
		NR *= 2;
	}
}

// @brief bit-reversal reordering
template<typename T>
void fft_reordering(std::complex<T> *data, size_t N)
{
	for (size_t i = 0, L = 0; i < N-1; ++i)
	{
		if (i < L) std::swap(data[L], data[i]);

		size_t R = N/2;
		while (R <= L)
		{
			L -= R;
			R /= 2;
		}
		L += R;
	}
}

// @brief DFT transformation
template<bool is_fwd, typename T>
void dft_algorithm(std::complex<T> *data, size_t N, const DFT_Table<T> &phase)
{
	std::vector< std::complex<T> > tmp(data, data+N);

	for (size_t i = 0; i < N; ++i)
	{
		std::complex<T> sum = T();
		for (size_t k = 0; k < N; ++k)
			sum += tmp[k] * (is_fwd
				? std::conj(phase[(i*k) % N])
				: phase[(i*k) % N]);

		data[i] = sum;
	}
}

} // details namespace
#endif // !OMNI_USE_MKL


#if defined(OMNI_USE_MKL)

//////////////////////////////////////////////////////////////////////////
// DFT initialization
template<typename T>
void DFT<T>::init()
{
	// create descriptor
	long ret = DftiCreateDescriptor((DFTI_DESCRIPTOR_HANDLE*)&m_impl,
		sizeof(T) < sizeof(double) ? DFTI_SINGLE:DFTI_DOUBLE,
			DFTI_COMPLEX, 1, m_size);
	assert(DFTI_NO_ERROR==ret && "DFTI descriptor creation");

	// set forward scale
	ret = DftiSetValue((DFTI_DESCRIPTOR_HANDLE)m_impl, DFTI_FORWARD_SCALE, m_fwd_scale);
	assert(DFTI_NO_ERROR==ret && "set DFTI forward scale");

	// set inverse scale
	ret = DftiSetValue((DFTI_DESCRIPTOR_HANDLE)m_impl, DFTI_BACKWARD_SCALE, m_inv_scale);
	assert(DFTI_NO_ERROR==ret && "set DFTI backward scale");

	// commit descriptor
	ret = DftiCommitDescriptor((DFTI_DESCRIPTOR_HANDLE)m_impl);
	assert(DFTI_NO_ERROR==ret && "DFTI descriptor commit");
}


//////////////////////////////////////////////////////////////////////////
// DFT destruction
template<typename T>
DFT<T>::~DFT()
{
	// release descriptor
	long ret = DftiFreeDescriptor((DFTI_DESCRIPTOR_HANDLE*)&m_impl);
	ret; assert(DFTI_NO_ERROR==ret && "DFTI descriptor destruction");
}


//////////////////////////////////////////////////////////////////////////
// forward transform
template<typename T>
void DFT<T>::forward(value_type *data)
{
	long ret = DftiComputeForward((DFTI_DESCRIPTOR_HANDLE)m_impl, data);
	ret; assert(DFTI_NO_ERROR==ret && "DFTI forward transform");
}


//////////////////////////////////////////////////////////////////////////
// inverse transform
template<typename T>
void DFT<T>::inverse(value_type *data)
{
	long ret = DftiComputeBackward((DFTI_DESCRIPTOR_HANDLE)m_impl, data);
	ret; assert(DFTI_NO_ERROR==ret && "DFTI backward transform");
}

#else // OMNI_USE_MKL

//////////////////////////////////////////////////////////////////////////
// DFT initialization
template<typename T>
void DFT<T>::init()
{
	// get table
	OMNI_MT_CODE(AutoLock guard(details::dft_lock()));
	m_impl = details::dft_tables((T*)0).get(m_size);
}


//////////////////////////////////////////////////////////////////////////
// DFT destruction
template<typename T>
DFT<T>::~DFT()
{
	// release table
	OMNI_MT_CODE(AutoLock guard(details::dft_lock()));
	details::dft_tables((T*)0).put((details::DFT_Table<T>*)m_impl);
}

//////////////////////////////////////////////////////////////////////////
// forward transform
template<typename T>
void DFT<T>::forward(value_type *data)
{
	typedef details::DFT_Table<T> table_type;

	if (m_log2)
	{
		details::fft_reordering(data, m_size);
		details::fft_algorithm<true>(data,
			m_log2, *(table_type*)m_impl);
	}
	else
		details::dft_algorithm<true>(data,
			m_size, *(table_type*)m_impl);

	// normalizing
	if (m_fwd_scale != scalar_type(1))
	for (size_t i = 0; i < m_size; ++i)
		data[i] *= m_fwd_scale;
}


//////////////////////////////////////////////////////////////////////////
// inverse transform
template<typename T>
void DFT<T>::inverse(value_type *data)
{
	typedef details::DFT_Table<T> table_type;

	if (m_log2)
	{
		details::fft_reordering(data, m_size);
		details::fft_algorithm<false>(data,
			m_log2, *(table_type*)m_impl);
	}
	else
		details::dft_algorithm<false>(data,
			m_size, *(table_type*)m_impl);

	// normalizing
	if (m_inv_scale != scalar_type(1))
	for (size_t i = 0; i < m_size; ++i)
		data[i] *= m_inv_scale;
}

#endif // OMNI_USE_MKL


//////////////////////////////////////////////////////////////////////////
// DFT construction
template<typename T>
DFT<T>::DFT(size_type DFT_size)
	: m_fwd_scale(scalar_type(1)/DFT_size), m_inv_scale(1), m_size(DFT_size)
{
	using namespace omni::util;

	m_log2 = (is_ipow2(DFT_size))
		? log2(DFT_size) : 0;

	init();
}


//////////////////////////////////////////////////////////////////////////
// DFT construction
template<typename T>
DFT<T>::DFT(size_type DFT_size, scalar_type fwd_scale, scalar_type inv_scale)
	: m_fwd_scale(fwd_scale), m_inv_scale(inv_scale), m_size(DFT_size)
{
	using namespace omni::util;

	m_log2 = (is_ipow2(DFT_size))
		? log2(DFT_size) : 0;

	init();
}


//////////////////////////////////////////////////////////////////////////
// DFT copy construction
template<typename T>
DFT<T>::DFT(const this_type &x)
	: m_fwd_scale(x.m_fwd_scale), m_inv_scale(x.m_inv_scale),
	  m_size(x.m_size), m_log2(x.m_log2)
{
	init();
}


//////////////////////////////////////////////////////////////////////////
// DFT assignment
template<typename T>
typename DFT<T>::this_type& DFT<T>::operator=(const this_type &x)
{
	this_type t(x);
	swap(t);

	return *this;
}


//////////////////////////////////////////////////////////////////////////
// swap two DFT
template<typename T>
void DFT<T>::swap(this_type &x)
{
	std::swap(m_fwd_scale, x.m_fwd_scale);
	std::swap(m_inv_scale, x.m_inv_scale);

	std::swap(m_size, x.m_size);
	std::swap(m_log2, x.m_log2);
	std::swap(m_impl, x.m_impl);
}


//////////////////////////////////////////////////////////////////////////
// explicit instantiation
template class DFT<double>;
template class DFT<float>;

	} // dsp namespace
} // omni namespace
