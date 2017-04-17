#include "bcb6_std_string.h"

void(__cdecl *bcb6_std_string_ctor)(bcb6_std_string *s) = (LPVOID)0x00402590;
void(__cdecl *_bcb6_std_string_assign_range)(bcb6_std_string *dest, LPCSTR first, LPCSTR last, ULONG64 *outReserved) = (LPVOID)0x004159D4;
void(__cdecl *_bcb6_std_string_ctor_assign_cstr)(bcb6_std_string *dest, bcb6_std_string *src, ULONG64 *outReserved) = (LPVOID)0x004165E8;
void(__cdecl *bcb6_std_string_ctor_assign)(bcb6_std_string *dest, bcb6_std_string *src) = (LPVOID)0x004166F0;
void(__cdecl *_bcb6_std_string_append_range)(bcb6_std_string *s, LPCSTR first, LPCSTR last, ULONG64 *outReserved) = (LPVOID)0x00418610;
void(__cdecl *bcb6_std_string_allocate)(bcb6_std_string *s, size_t n) = (LPVOID)0x004462DC;
bcb6_std_string *(__cdecl *bcb6_std_string_append)(bcb6_std_string *s, size_t n, char c) = (LPVOID)0x00462FFC;

