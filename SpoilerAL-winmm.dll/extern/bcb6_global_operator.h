#pragma once

#ifdef USING_NAMESPACE_BCB6_GLOBAL
#define operator_new    bcb6_global_operator_new
#define operator_delete bcb6_global_operator_delete
#endif

#include <windows.h>

// ::operator new(size_t n);
EXTERN_C void *(__cdecl *bcb6_global_operator_new)(size_t n);
// ::delete(void *p);
EXTERN_C void (__cdecl *bcb6_global_operator_delete)(void *p);
