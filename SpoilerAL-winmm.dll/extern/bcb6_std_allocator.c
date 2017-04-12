#include "bcb6_std_allocator.h"

void *(__cdecl *bcb6_std_allocator_allocate)(size_t n) = (LPVOID)0x005F43F0;
void (__cdecl *bcb6_std_allocator_deallocate)(void *p, size_t n) = (LPVOID)0x005F47A0;

