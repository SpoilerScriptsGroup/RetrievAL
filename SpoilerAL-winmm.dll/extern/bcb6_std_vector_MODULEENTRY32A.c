#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_MODULEENTRY32A.h"
#include "bcb6_std_allocator.h"

void __fastcall vector_MODULEENTRY32A_push_back(vector_MODULEENTRY32A *v, MODULEENTRY32A *value)
{
	if (vector_end(v) + 1 > vector_end_of_storage(v))
	{
		size_t size = vector_byte_size(v);
		size_t capacity = size + max(size, sizeof(MODULEENTRY32A));
		char *p = (char *)allocator_reallocate(vector_begin(v), vector_byte_capacity(v), capacity);
		vector_begin(v) = (MODULEENTRY32A *)p;
		vector_end(v) = (MODULEENTRY32A *)(p + size);
		vector_end_of_storage(v) = (MODULEENTRY32A *)(p + capacity);
	}
	*(vector_end(v)++) = *value;
}
