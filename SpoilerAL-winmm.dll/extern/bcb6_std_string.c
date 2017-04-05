#include "bcb6_std_string.h"

void(__cdecl *bcb6_std_string_ctor)(bcb6_std_string *s) = (LPVOID)0x00402590;
void(__cdecl *bcb6_std_string_ctor_assign)(bcb6_std_string *dest, bcb6_std_string *src) = (LPVOID)0x004166F0;
void(__cdecl *bcb6_std_string_allocator)(bcb6_std_string *s, size_t n) = (LPVOID)0x004462DC;
bcb6_std_string *(__cdecl *bcb6_std_string_append)(bcb6_std_string *s, size_t n, char c) = (LPVOID)0x00462FFC;
