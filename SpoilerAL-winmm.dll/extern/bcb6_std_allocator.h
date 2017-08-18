#pragma once

#include <windows.h>
#include "bcb6_global_operator.h"
#include "OptimizeAllocator.h"

#ifdef USING_NAMESPACE_BCB6_STD
#define node_alloc_allocate         bcb6_std_node_alloc_allocate
#define node_alloc_deallocate       bcb6_std_node_alloc_deallocate
#define allocator_MAX_BYTES         bcb6_std_allocator_MAX_BYTES
#define inline_allocator_allocate   inline_bcb6_std_allocator_allocate
#define inline_allocator_deallocate inline_bcb6_std_allocator_deallocate
#define allocator_allocate          bcb6_std_allocator_allocate
#define allocator_deallocate        bcb6_std_allocator_deallocate
#define allocator_reallocate        bcb6_std_allocator_reallocate
#endif

EXTERN_C void *(__cdecl *bcb6_std_node_alloc_allocate)(size_t n);
EXTERN_C void (__cdecl *bcb6_std_node_alloc_deallocate)(void *p, size_t n);

#define bcb6_stl_new    bcb6_global_operator_new
#define bcb6_stl_delete bcb6_global_operator_delete

EXTERN_C void * __fastcall bcb6_std_allocator_allocate(size_t n);
EXTERN_C void __fastcall bcb6_std_allocator_deallocate(void *p, size_t n);
#if !OPTIMIZE_ALLOCATOR
EXTERN_C void * __fastcall bcb6_std_allocator_reallocate(void *p, size_t from, size_t to);
#else
EXTERN_C void * __fastcall _bcb6_std_allocator_reallocate(void *p, size_t n);
#define bcb6_std_allocator_reallocate(p, from, to) _bcb6_std_allocator_reallocate(p, to)
#endif
EXTERN_C void __cdecl bad_alloc();
