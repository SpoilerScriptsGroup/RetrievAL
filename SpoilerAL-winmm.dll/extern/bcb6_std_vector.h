#pragma once

#include <windows.h>
#include "bcb6_std_string.h"

typedef struct
{
	LPVOID _M_start;
	LPVOID _M_finish;
	LPVOID padding1;
	LPVOID padding2;
	LPVOID _M_end_of_storage;
	LPVOID padding3;
} bcb6_std_vector, *pbcb6_std_vector;

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_vector  vector;
typedef pbcb6_std_vector pvector;
#define vector_ctor            bcb6_std_vector_ctor
#define vector_size            bcb6_std_vector_size
#define vector_size_by_type    bcb6_std_vector_size_by_type
#define vector_bytes           bcb6_std_vector_bytes
#define vector_begin           bcb6_std_vector_begin
#define vector_end             bcb6_std_vector_end
#define vector_empty           bcb6_std_vector_empty
#define vector_at              bcb6_std_vector_at
#define vector_type_at         bcb6_std_vector_type_at
#define vector_offset          bcb6_std_vector_offset
#define vector_BYTE_reserve    bcb6_std_vector_BYTE_reserve
#define vector_dtor            bcb6_std_vector_dtor
#define vector_BYTE_resize     bcb6_std_vector_BYTE_resize
#define vector_BYTE_push_back  bcb6_std_vector_BYTE_push_back
#define vector_DWORD_push_back bcb6_std_vector_DWORD_push_back
#endif

#define bcb6_std_vector_ctor(v)                 ((v)->_M_end_of_storage = (v)->_M_finish = (v)->_M_start = NULL)
#define bcb6_std_vector_size(v)                 (size_t)((v)->_M_finish - (v)->_M_start)
#define bcb6_std_vector_size_by_type(v, type)   (size_t)((type *)(v)->_M_finish - (type *)(v)->_M_start)
#define bcb6_std_vector_bytes(v)                bcb6_std_vector_size_by_type(v, BYTE)
#define bcb6_std_vector_begin(v)                (v)->_M_start
#define bcb6_std_vector_end(v)                  (v)->_M_finish
#define bcb6_std_vector_empty(v)                ((v)->_M_finish == (v)->_M_start)
#define bcb6_std_vector_at(v, index)            (v)->_M_start[index]
#define bcb6_std_vector_type_at(v, type, index) ((type *)(v)->_M_start)[index]
#define bcb6_std_vector_offset(v, index)        (void *)&bcb6_std_vector_type_at(v, BYTE, index)

EXTERN_C void(__cdecl *bcb6_std_vector_BYTE_reserve)(void *v, size_t n);

EXTERN_C void __fastcall bcb6_std_vector_dtor(void *v);
EXTERN_C void __fastcall bcb6_std_vector_BYTE_resize(void *v, size_t n);
EXTERN_C void __fastcall bcb6_std_vector_BYTE_push_back(void *v, BYTE value);
EXTERN_C void __fastcall bcb6_std_vector_DWORD_push_back(void *v, DWORD value);

