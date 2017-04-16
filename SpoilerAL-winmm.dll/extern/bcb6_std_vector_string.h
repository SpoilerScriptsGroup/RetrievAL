#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"

EXTERN_C void __fastcall bcb6_std_vector_string_clear(bcb6_std_vector *v);
EXTERN_C void __fastcall bcb6_std_vector_string_deallocate(bcb6_std_string *first, bcb6_std_string *last);
EXTERN_C void __fastcall bcb6_std_vector_string_erase(bcb6_std_vector *v, bcb6_std_string *first, bcb6_std_string *last);
EXTERN_C void __fastcall bcb6_std_vector_string_resize(bcb6_std_vector *v, size_t n);

EXTERN_C void(__cdecl *_bcb6_std_vector_string_deallocate)(bcb6_std_string *first, bcb6_std_string *last, ULONG64 *reserved);
EXTERN_C void(__cdecl *_bcb6_std_vector_string_destroy)(bcb6_std_string *last, bcb6_std_string *vectorEnd, bcb6_std_string *first, ULONG64 *reserved1, DWORD reserved2);
EXTERN_C void(__cdecl *bcb6_std_vector_string_reserve)(bcb6_std_vector *s, size_t n);

