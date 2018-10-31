#pragma once

#include <windows.h>
#include "bcb6_operator.h"
#include "OptimizeAllocator.h"

#ifdef USING_NAMESPACE_BCB6_STD
#define node_alloc_allocate   bcb6_std_node_alloc_allocate
#define node_alloc_deallocate bcb6_std_node_alloc_deallocate
#define allocator_allocate    bcb6_std_allocator_allocate
#define allocator_deallocate  bcb6_std_allocator_deallocate
#define allocator_reallocate  bcb6_std_allocator_reallocate
#define internal_allocate     bcb6_std_internal_allocate
#define internal_deallocate   bcb6_std_internal_deallocate
#define internal_reallocate   bcb6_std_internal_reallocate
#endif

#if !OPTIMIZE_ALLOCATOR
EXTERN_C void *(__cdecl * const bcb6_std_node_alloc_allocate)(size_t n);
EXTERN_C void (__cdecl * const bcb6_std_node_alloc_deallocate)(void *p, size_t n);
#else
#define bcb6_std_node_alloc_allocate   bcb6_std_internal_allocate
#define bcb6_std_node_alloc_deallocate bcb6_std_internal_deallocate
#endif

EXTERN_C void * __fastcall bcb6_std_internal_allocate(size_t n);
#define bcb6_std_allocator_allocate bcb6_std_internal_allocate
#if !OPTIMIZE_ALLOCATOR
EXTERN_C void __fastcall bcb6_std_internal_deallocate(void *p, size_t n);
EXTERN_C void * __fastcall bcb6_std_internal_reallocate(void *p, size_t from, size_t to);
#define bcb6_std_allocator_deallocate bcb6_std_internal_deallocate
#define bcb6_std_allocator_reallocate bcb6_std_internal_reallocate
#else
EXTERN_C void __fastcall bcb6_std_internal_deallocate(void *p);
EXTERN_C void * __fastcall bcb6_std_internal_reallocate(void *p, size_t n);
#define bcb6_std_allocator_deallocate(p, n)        bcb6_std_internal_deallocate(p)
#define bcb6_std_allocator_reallocate(p, from, to) bcb6_std_internal_reallocate(p, to)
#endif

EXTERN_C void __cdecl bad_alloc();
