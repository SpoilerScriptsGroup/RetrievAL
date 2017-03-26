#pragma once

#include <windows.h>

typedef struct _bcb6_std_set_iterator
{
	DWORD                         padding1[2];
	struct _bcb6_std_set_iterator *iterator;
	DWORD                         padding2;
	BYTE                          element[];
} *bcb6_std_set_iterator, **pbcb6_std_set_iterator;

typedef struct
{
	DWORD                 padding1[2];
	bcb6_std_set_iterator iterator;
	DWORD                 padding2[5];
} bcb6_std_set, *pbcb6_std_set;

#define bcb6_std_set_begin(map) (map)->iterator->iterator
#define bcb6_std_set_end(map) (map)->iterator

typedef bcb6_std_set_iterator (__cdecl *LPFN_STD_SET_ITERATOR_SUB_ONE)(bcb6_std_set_iterator it);
#define bcb6_std_set_iterator_sub_one ((LPFN_STD_SET_ITERATOR_SUB_ONE)0x005F5360)
#define bcb6_std_set_iterator_decrement(it) ((it) = bcb6_std_set_iterator_sub_one(it))
