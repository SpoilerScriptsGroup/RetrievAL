#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"
#include "bcb6_std_allocator.h"

void __fastcall vector_dtor(vector *v)
{
	allocator_deallocate(vector_begin(v), vector_byte_capacity(v));
}

void __fastcall vector_shrink_to_fit(vector *v)
{
	size_t capacity = vector_byte_capacity(v);
	size_t size = vector_byte_size(v);
	if (size != capacity)
		vector_end_of_storage(v) = vector_end(v) = (char *)(vector_begin(v) = allocator_reallocate(vector_begin(v), capacity, size)) + size;
}
