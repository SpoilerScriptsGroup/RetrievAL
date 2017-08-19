#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_THeapListData.h"
#include "bcb6_std_allocator.h"

void __fastcall vector_THeapListData_push_back(vector_THeapListData *v, THeapListData *value)
{
	if (vector_end(v) + 1 > v->_M_end_of_storage)
	{
		(char *)vector_end(v) -= (size_t)vector_begin(v);
		size_t capacity = vector_BYTE_capacity(v);
		size_t allocate = capacity ? capacity * 2 : sizeof(*value);
		vector_begin(v) = allocator_reallocate(vector_begin(v), capacity, allocate);
		v->_M_end_of_storage = (THeapListData *)((char *)vector_begin(v) + allocate);
		(char *)vector_end(v) += (size_t)vector_begin(v);
	}
	*(vector_end(v)++) = *value;
}
