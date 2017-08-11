#pragma once

#include <windows.h>
#include <intrin.h>

typedef struct _bcb6_std_list_node
{
	struct _bcb6_std_list_node *_M_next;
	struct _bcb6_std_list_node *_M_prev;
	struct _bcb6_std_list_node *_M_data;
} bcb6_std_list_node, *pbcb6_std_list_node;

typedef struct
{
	bcb6_std_list_node *_M_node;
} bcb6_std_list_iterator, *pbcb6_std_list_iterator;

typedef struct
{
	bcb6_std_list_node _M_node;
	LPVOID             padding;
} bcb6_std_list, *pbcb6_std_list;

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_list_node      list_node;
typedef pbcb6_std_list_node     plist_node;
typedef bcb6_std_list_iterator  list_iterator;
typedef pbcb6_std_list_iterator plist_iterator;
typedef bcb6_std_list           list;
typedef pbcb6_std_list          plist;
#define list_begin              bcb6_std_list_begin
#define list_end                bcb6_std_list_end
#define list_iterator_increment bcb6_std_list_iterator_increment
#define list_iterator_decrement bcb6_std_list_iterator_decrement
#define list_erase              bcb6_std_list_erase
#endif

#define bcb6_std_list_begin(list) (bcb6_std_list_iterator *)(list)->_M_node._M_data->_M_next
#define bcb6_std_list_end(list) (bcb6_std_list_iterator *)(list)->_M_node._M_data

#define bcb6_std_list_iterator_increment(it) ((it) = (bcb6_std_list_iterator *)((bcb6_std_list_node *)(it))->_M_next)
#define bcb6_std_list_iterator_decrement(it) ((it) = (bcb6_std_list_iterator *)((bcb6_std_list_node *)(it))->_M_prev)

EXTERN_C void __fastcall bcb6_std_list_erase(bcb6_std_list_iterator *it);
