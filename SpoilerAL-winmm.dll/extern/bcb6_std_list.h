#pragma once

#include <windows.h>
#include <intrin.h>

typedef struct _List_node
{
	struct _List_node *_M_next;
	struct _List_node *_M_prev;
	BYTE               _M_data[];
} bcb6_std_list_node, *pbcb6_std_list_node, *bcb6_std_list_iterator, **pbcb6_std_list_iterator;

typedef struct _List_base
{
	LPVOID             Rebound[2];
	struct _List_node *_M_node;
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

#define bcb6_std_list_begin(list) (list)->_M_node->_M_next
#define bcb6_std_list_end(list) (list)->_M_node

#define bcb6_std_list_iterator_increment(it) ((it) = it->_M_next)
#define bcb6_std_list_iterator_decrement(it) ((it) = it->_M_prev)

EXTERN_C void __fastcall bcb6_std_list_erase(bcb6_std_list_iterator *it);
