#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_THeapListData.h"
#include "bcb6_std_allocator.h"

void __fastcall vector_THeapListData_push_back(vector_THeapListData *v, THeapListData *value)
{
	if (vector_end(v) + 1 > vector_end_of_storage(v))
	{
		size_t size = vector_BYTE_size(v);
		size_t capacity = size + max(size, sizeof(THeapListData));
		char *p = (char *)allocator_reallocate(vector_begin(v), vector_BYTE_capacity(v), capacity);
		vector_begin(v) = (THeapListData *)p;
		vector_end(v) = (THeapListData *)(p + size);
		vector_end_of_storage(v) = (THeapListData *)(p + capacity);
	}
	*(vector_end(v)++) = *value;
}
