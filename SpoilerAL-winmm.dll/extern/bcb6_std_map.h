#pragma once

#include <windows.h>

typedef struct _bcb6_std_map_iterator
{
	DWORD                         padding1[2];
	struct _bcb6_std_map_iterator *iterator;
	DWORD                         padding2;
	BYTE                          first[];
} *bcb6_std_map_iterator, **pbcb6_std_map_iterator;

typedef struct
{
	DWORD                 padding1[2];
	bcb6_std_map_iterator iterator;
	DWORD                 padding2[5];
} bcb6_std_map, *pbcb6_std_map;

typedef bcb6_std_map_iterator (__cdecl *LPFN_STD_MAP_FIND)(bcb6_std_map *map, LPVOID key);
#define bcb6_std_map_find ((LPFN_STD_MAP_FIND)0x004D4008)

#define bcb6_std_map_begin(map) (map)->iterator->iterator
#define bcb6_std_map_end(map) (map)->iterator

#define bcb6_std_pair_first(it) (LPVOID)(it)->first
#define bcb6_std_pair_second(it, typeOfFirst) (LPVOID)((it)->first + ((sizeof(typeOfFirst) + (8 - 1)) & ~(size_t)(8 - 1)))

typedef bcb6_std_map_iterator (__cdecl *LPFN_STD_MAP_ITERATOR_SUB_ONE)(bcb6_std_map_iterator it);
#define bcb6_std_map_iterator_sub_one ((LPFN_STD_MAP_ITERATOR_SUB_ONE)0x005F5360)
#define bcb6_std_map_iterator_decrement(it) ((it) = bcb6_std_map_iterator_sub_one(it))

EXTERN_C void __stdcall bcb6_std_map_erase(bcb6_std_map *map, bcb6_std_map_iterator it);
