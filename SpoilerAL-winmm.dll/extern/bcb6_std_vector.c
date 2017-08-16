#include "bcb6_std_vector.h"
#include "bcb6_std_allocator.h"

void(__cdecl *bcb6_std_vector_BYTE_reserve)(void *v, size_t n) = (LPVOID)0x0050F128;

void __fastcall bcb6_std_vector_shrink_to_fit(bcb6_std_vector *v)
{
	size_t capacity = (size_t)v->_M_end_of_storage - (size_t)v->_M_start;
	size_t size = (size_t)v->_M_finish - (size_t)v->_M_start;
	if (size != capacity)
		v->_M_end_of_storage = v->_M_finish = (char *)(v->_M_start = bcb6_std_allocator_reallocate(v->_M_start, capacity, size)) + size;
}
