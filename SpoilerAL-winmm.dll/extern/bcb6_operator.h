#pragma once

#include <windows.h>
#include "bcb6_std_allocator.h"
#include "OptimizeAllocator.h"

#ifdef USING_NAMESPACE_BCB6
#define operator_new    bcb6_operator_new
#define operator_delete bcb6_operator_delete
#endif

#if !OPTIMIZE_ALLOCATOR
// ::operator new(size_t n);
EXTERN_C void *(__cdecl * const bcb6_operator_new)(size_t n);
// ::operator delete(void *p);
EXTERN_C void (__cdecl * const bcb6_operator_delete)(void *p);
#else
#define bcb6_operator_new    _bcb6_std_allocator_allocate
#define bcb6_operator_delete _bcb6_std_allocator_deallocate
#endif
