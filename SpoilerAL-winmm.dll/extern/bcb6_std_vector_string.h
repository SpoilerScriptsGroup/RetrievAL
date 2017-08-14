#pragma once

#include <windows.h>
#include "bcb6_std_vector.h"
#include "bcb6_std_string.h"
#define typename string
#define type     bcb6_std_string
#include "bcb6_std_vector_template.h"

#ifdef USING_NAMESPACE_BCB6_STD
#define vector_string_dtor            bcb6_std_vector_string_dtor
#define vector_string_clear           bcb6_std_vector_string_clear
#define vector_string_deallocate      bcb6_std_vector_string_deallocate
#define vector_string_erase           bcb6_std_vector_string_erase
#define vector_string_resize          bcb6_std_vector_string_resize
#define vector_string_reserve         bcb6_std_vector_string_reserve
#define vector_string_push_back       bcb6_std_vector_string_push_back
#define vector_string_push_back_range bcb6_std_vector_string_push_back_range
#define vector_string_pop_back        bcb6_std_vector_string_pop_back
#endif

EXTERN_C void __fastcall bcb6_std_vector_string_dtor(bcb6_std_vector_string *v);
EXTERN_C void __fastcall bcb6_std_vector_string_clear(bcb6_std_vector_string *v);
EXTERN_C void __fastcall bcb6_std_vector_string_deallocate(bcb6_std_string *first, bcb6_std_string *last);
EXTERN_C void __fastcall bcb6_std_vector_string_erase(bcb6_std_vector_string *v, bcb6_std_string *first, bcb6_std_string *last);
EXTERN_C void __fastcall bcb6_std_vector_string_resize(bcb6_std_vector_string *v, size_t n);

EXTERN_C void(__cdecl *_bcb6_std_vector_string_deallocate)(bcb6_std_string *first, bcb6_std_string *last, ULONG64 *outReserved);
EXTERN_C void(__cdecl *_bcb6_std_vector_string_destroy)(bcb6_std_string *last, bcb6_std_string *vectorEnd, bcb6_std_string *first, ULONG64 *outReserved, DWORD zeroReserved);
EXTERN_C void(__cdecl *bcb6_std_vector_string_reserve)(bcb6_std_vector_string *v, size_t n);

__inline void bcb6_std_vector_string_push_back(bcb6_std_vector_string *v, bcb6_std_string *s)
{
	bcb6_std_vector_BYTE_reserve(v, bcb6_std_vector_bytes(v) + sizeof(bcb6_std_string));
	bcb6_std_string_ctor_assign(bcb6_std_vector_end(v)++, s);
}

__inline void bcb6_std_vector_string_push_back_range(bcb6_std_vector_string *v, const char *first, const char *last)
{
	bcb6_std_vector_BYTE_reserve(v, bcb6_std_vector_bytes(v) + sizeof(bcb6_std_string));
	bcb6_std_string_ctor_assign_range(bcb6_std_vector_end(v)++, first, last);
}

__inline void bcb6_std_vector_string_pop_back(bcb6_std_vector_string *v)
{
	if (!bcb6_std_vector_empty(v))
		bcb6_std_string_dtor(--(v)->_M_finish);
}
