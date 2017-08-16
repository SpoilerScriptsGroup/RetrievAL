#include "bcb6_malloc.h"

void *(__cdecl *bcb6_calloc)(size_t num, size_t size) = (LPVOID)0x005D4458;
void *(__cdecl *bcb6_malloc)(size_t size) = (LPVOID)0x005D4B3C;
void(__cdecl *bcb6_free)(void *memblock) = (LPVOID)0x005D4B4C;
void *(__cdecl *bcb6_realloc)(void *memblock, size_t size) = (LPVOID)0x005D4B5C;
