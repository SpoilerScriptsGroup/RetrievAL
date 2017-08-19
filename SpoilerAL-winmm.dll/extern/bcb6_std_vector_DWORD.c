#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"
#include "bcb6_std_allocator.h"

void __fastcall vector_DWORD_push_back(vector_DWORD *v, DWORD value)
{
	if (vector_end(v) + 1 > v->_M_end_of_storage)
	{
		size_t size = vector_BYTE_size(v);
		size_t capacity = size + max(size, sizeof(DWORD));
		BYTE *p = (BYTE *)allocator_reallocate(vector_begin(v), vector_BYTE_capacity(v), capacity);
		vector_begin(v) = (DWORD *)p;
		vector_end(v) = (DWORD *)(p + size);
		v->_M_end_of_storage = (DWORD *)(p + capacity);
	}
	*(vector_end(v)++) = value;
}
