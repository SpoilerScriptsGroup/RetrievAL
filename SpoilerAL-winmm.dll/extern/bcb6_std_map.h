#pragma once

#include <windows.h>

typedef struct _Rb_tree_node
{
	BOOLEAN               _M_color;
	struct _Rb_tree_node *_M_parent;
	struct _Rb_tree_node *_M_left;
	struct _Rb_tree_node *_M_right;
	BYTE                  _M_value_field[];
} bcb6_std_map_node, *bcb6_std_map_iterator, **pbcb6_std_map_iterator;

typedef struct _Rb_tree
{
	DWORD                 padding1[2];
	bcb6_std_map_iterator _M_data;
	DWORD                 padding2;
	size_t                _M_node_count;
	DWORD                 padding3;
	void *                _M_key_compare;
	DWORD                 padding4;
} bcb6_std_map, *pbcb6_std_map;

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_map_iterator  map_iterator;
typedef pbcb6_std_map_iterator pmap_iterator;
typedef bcb6_std_map           map;
typedef pbcb6_std_map          pmap;
#define map_find               bcb6_std_map_find
#define map_lower_bound        bcb6_std_map_lower_bound
#define map_insert             bcb6_std_map_insert
#define map_begin              bcb6_std_map_begin
#define map_end                bcb6_std_map_end
#define pair_first             bcb6_std_pair_first
#define pair_second            bcb6_std_pair_second
#define pair_second_aligned    bcb6_std_pair_second_aligned
#define map_iterator_increment bcb6_std_map_iterator_increment
#define map_iterator_sub_one   bcb6_std_map_iterator_sub_one
#define map_iterator_decrement bcb6_std_map_iterator_decrement
#define map_erase              bcb6_std_map_erase
#endif

EXTERN_C bcb6_std_map_iterator(__cdecl * const bcb6_std_map_find)(bcb6_std_map *map, LPVOID key);
EXTERN_C bcb6_std_map_iterator(__cdecl * const bcb6_std_map_lower_bound)(bcb6_std_map *, void *key);
EXTERN_C void(__cdecl * const bcb6_std_map_insert)(bcb6_std_map_iterator *, bcb6_std_map *, bcb6_std_map_iterator pos, void *pair);

#define bcb6_std_map_begin(map) (map)->_M_data->_M_left
#define bcb6_std_map_end(map) (map)->_M_data

#define bcb6_std_pair_first(it) (LPVOID)(it)->_M_value_field
#define bcb6_std_pair_second(it, typeOfFirst) (LPVOID)((it)->_M_value_field + sizeof(typeOfFirst))
#define bcb6_std_pair_second_aligned(it, typeOfFirst) (LPVOID)((it)->_M_value_field + ((sizeof(typeOfFirst) + (8 - 1)) & (ptrdiff_t)-8))

EXTERN_C bcb6_std_map_iterator(__cdecl * const bcb6_std_map_iterator_increment)(bcb6_std_map_iterator it);
EXTERN_C bcb6_std_map_iterator(__cdecl * const bcb6_std_map_iterator_sub_one)(bcb6_std_map_iterator it);
#define bcb6_std_map_iterator_decrement(it) ((it) = bcb6_std_map_iterator_sub_one(it))

EXTERN_C void __stdcall bcb6_std_map_erase(bcb6_std_map *map, bcb6_std_map_iterator it);
