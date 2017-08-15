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
#define vector_ctor             bcb6_std_vector_ctor
#define vector_size             bcb6_std_vector_size
#define vector_size_by_type     bcb6_std_vector_size_by_type
#define vector_BYTE_size        bcb6_std_vector_BYTE_size
#define vector_capacity         bcb6_std_vector_capacity
#define vector_capacity_by_type bcb6_std_vector_capacity_by_type
#define vector_BYTE_capacity    bcb6_std_vector_BYTE_capacity
#define vector_begin            bcb6_std_vector_begin
#define vector_end              bcb6_std_vector_end
#define vector_empty            bcb6_std_vector_empty
#define vector_at               bcb6_std_vector_at
#define vector_type_at          bcb6_std_vector_type_at
#define vector_offset           bcb6_std_vector_offset
#define vector_BYTE_reserve     bcb6_std_vector_BYTE_reserve
#define vector_dtor             bcb6_std_vector_dtor
#define vector_BYTE_grow        bcb6_std_vector_BYTE_grow
#define vector_BYTE_resize      bcb6_std_vector_BYTE_resize
#define vector_BYTE_push_back   bcb6_std_vector_BYTE_push_back
#define vector_DWORD_push_back  bcb6_std_vector_DWORD_push_back
#define vector_push_back        bcb6_std_vector_push_back
#define vector_pop_back         bcb6_std_vector_pop_back
#define vector_insert           bcb6_std_vector_insert
#define vector_insert_by_type   bcb6_std_vector_insert_by_type
#endif

#define bcb6_std_vector_ctor(v)                   ((v)->_M_end_of_storage = (v)->_M_finish = (v)->_M_start = NULL)
#define bcb6_std_vector_size(v)                   (size_t)((v)->_M_finish - (v)->_M_start)
#define bcb6_std_vector_size_by_type(v, type)     (size_t)((type *)(v)->_M_finish - (type *)(v)->_M_start)
#define bcb6_std_vector_BYTE_size(v)              bcb6_std_vector_size_by_type(v, char)
#define bcb6_std_vector_capacity(v)               (size_t)((v)->_M_end_of_storage - (v)->_M_start)
#define bcb6_std_vector_capacity_by_type(v, type) (size_t)((type *)(v)->_M_end_of_storage - (type *)(v)->_M_start)
#define bcb6_std_vector_BYTE_capacity(v)          bcb6_std_vector_capacity_by_type(v, char)
#define bcb6_std_vector_begin(v)                  (v)->_M_start
#define bcb6_std_vector_end(v)                    (v)->_M_finish
#define bcb6_std_vector_empty(v)                  ((v)->_M_finish == (v)->_M_start)
#define bcb6_std_vector_at(v, index)              (v)->_M_start[index]
#define bcb6_std_vector_type_at(v, type, index)   ((type *)(v)->_M_start)[index]
#define bcb6_std_vector_offset(v, index)          (void *)&bcb6_std_vector_type_at(v, char, index)

EXTERN_C void(__cdecl *bcb6_std_vector_BYTE_reserve)(void *v, size_t n);

EXTERN_C void __fastcall bcb6_std_vector_dtor(void *v);
EXTERN_C void __fastcall bcb6_std_vector_BYTE_resize(void *v, size_t n);
EXTERN_C void __fastcall bcb6_std_vector_BYTE_push_back(void *v, BYTE value);
EXTERN_C void __fastcall bcb6_std_vector_DWORD_push_back(void *v, DWORD value);

#define bcb6_std_vector_push_back(v, x)                                                    \
do                                                                                         \
{                                                                                          \
    bcb6_std_vector_BYTE_resize(v, bcb6_std_vector_BYTE_size(v) + sizeof(*(v)->_M_start)); \
    *((v)->_M_finish - 1) = x;                                                             \
} while (0)

#define bcb6_std_vector_pop_back(v)                            \
do                                                             \
{                                                              \
    bcb6_std_vector * __restrict __v = (bcb6_std_vector *)(v); \
    if (!bcb6_std_vector_empty(__v))                           \
        (char *)(__v)->_M_finish -= sizeof(*(v)->_M_start);    \
} while (0)

#define bcb6_std_vector_insert(v, position, x)                                             \
do                                                                                         \
{                                                                                          \
    size_t __index = (position) - (v)->_M_start;                                           \
    size_t __backward = (char *)(v)->_M_finish - (char *)(position);                       \
    bcb6_std_vector_BYTE_resize(v, bcb6_std_vector_BYTE_size(v) + sizeof(*(v)->_M_start)); \
    memmove((v)->_M_start + __index + 1, (v)->_M_start + __index, __backward);             \
    (v)->_M_start[__index] = x;                                                            \
} while (0)

#define bcb6_std_vector_insert_by_type(v, type, position, x)                     \
do                                                                               \
{                                                                                \
    bcb6_std_vector * __restrict __v = (bcb6_std_vector *)(v);                   \
    void * __restrict __position = position;                                     \
    size_t __forward = (char *)__position - (char *)__v->_M_start;               \
    size_t __backward = (char *)__v->_M_finish - (char *)__position;             \
    bcb6_std_vector_BYTE_resize(v, bcb6_std_vector_BYTE_size__v + sizeof(type)); \
    type *__element = (type *)((char *)__v->_M_start + __forward);               \
    memmove(__element + 1, __element, __backward);                               \
    *__element = x;                                                              \
} while (0)
