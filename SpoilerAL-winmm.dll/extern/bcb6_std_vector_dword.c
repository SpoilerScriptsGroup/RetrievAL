#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"
#include "bcb6_std_allocator.h"

__declspec(naked) void __fastcall vector_dword_reserve(vector_dword *s, size_t n)
{
	__asm
	{
		shl     edx, 2
		jmp     vector_byte_reserve
	}
}

void __fastcall vector_dword_push_back(vector_dword *v, DWORD value)
{
	if (vector_end(v) + 1 > vector_end_of_storage(v))
	{
		size_t size = vector_byte_size(v);
		size_t capacity = size + max(size, sizeof(DWORD));
		char *p = (char *)allocator_reallocate(vector_begin(v), vector_byte_capacity(v), capacity);
		vector_begin(v) = (DWORD *)p;
		vector_end(v) = (DWORD *)(p + size);
		vector_end_of_storage(v) = (DWORD *)(p + capacity);
	}
	*(vector_end(v)++) = value;
}
