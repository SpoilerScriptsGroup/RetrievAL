#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#define typename string
#define type     bcb6_std_string
#include "bcb6_std_vector_template.h"

#ifdef USING_NAMESPACE_BCB6_STD
#define vector_string_dtor       bcb6_std_vector_string_dtor
#define vector_string_clear      bcb6_std_vector_string_clear
#define vector_string_deallocate bcb6_std_vector_string_deallocate
#define vector_string_erase      bcb6_std_vector_string_erase
#define vector_string_resize     bcb6_std_vector_string_resize
#define vector_string_reserve    bcb6_std_vector_string_reserve
#endif

EXTERN_C void __fastcall bcb6_std_vector_string_dtor(bcb6_std_vector_string *v);
EXTERN_C void __fastcall bcb6_std_vector_string_clear(bcb6_std_vector_string *v);
EXTERN_C void __fastcall bcb6_std_vector_string_deallocate(bcb6_std_string *first, bcb6_std_string *last);
EXTERN_C void __fastcall bcb6_std_vector_string_erase(bcb6_std_vector_string *v, bcb6_std_string *first, bcb6_std_string *last);
EXTERN_C void __fastcall bcb6_std_vector_string_resize(bcb6_std_vector_string *v, size_t n);

EXTERN_C void(__cdecl *_bcb6_std_vector_string_deallocate)(bcb6_std_string *first, bcb6_std_string *last, ULONG64 *outReserved);
EXTERN_C void(__cdecl *_bcb6_std_vector_string_destroy)(bcb6_std_string *last, bcb6_std_string *vectorEnd, bcb6_std_string *first, ULONG64 *outReserved, DWORD zeroReserved);
EXTERN_C void(__cdecl *bcb6_std_vector_string_reserve)(bcb6_std_vector_string *v, size_t n);

