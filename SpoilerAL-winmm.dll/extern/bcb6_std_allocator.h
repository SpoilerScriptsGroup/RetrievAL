#pragma once

#include <windows.h>
#include "bcb6_global_operator.h"

EXTERN_C void *(__cdecl *bcb6_std_allocator_allocate)(size_t n);
EXTERN_C void (__cdecl *bcb6_std_allocator_deallocate)(void *p, size_t n);

#define bcb6_stl_new    bcb6_global_operator_new
#define bcb6_stl_delete bcb6_global_operator_delete

#define bcb6_std_node_alloc_MAX_BYTES 128

__inline void * bcb6_std_node_alloc_allocate(size_t n)
{
	return (n > bcb6_std_node_alloc_MAX_BYTES) ?
		bcb6_stl_new(n) :
		bcb6_std_allocator_allocate(n);
}

__inline void * bcb6_std_node_alloc_deallocate(void *p, size_t n)
{
	if (n > bcb6_std_node_alloc_MAX_BYTES)
		bcb6_stl_delete(p);
	else
		bcb6_std_allocator_deallocate(p, n);
}

