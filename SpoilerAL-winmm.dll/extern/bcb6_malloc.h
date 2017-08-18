#pragma once

#include <windows.h>

EXTERN_C void * __cdecl bcb6_calloc(size_t num, size_t size);
EXTERN_C void * __cdecl bcb6_malloc(size_t size);
EXTERN_C void __cdecl bcb6_free(void *memblock);
EXTERN_C void * __cdecl bcb6_realloc(void *memblock, size_t size);
