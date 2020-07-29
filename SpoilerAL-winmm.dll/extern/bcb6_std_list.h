#pragma once

#include <windows.h>
#include <intrin.h>

typedef struct _List_node
{
	struct _List_node *_M_next;
	struct _List_node *_M_prev;
	BYTE               _M_data[];
} *_List_iterator, bcb6_std_list_node, *pbcb6_std_list_node, *bcb6_std_list_iterator, **pbcb6_std_list_iterator;

typedef struct _List_base
{
	LPCVOID            _Node_allocator_type[2];
	struct _List_node *_M_node;
	LPCVOID            padding;
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
#define bcb6_std_list_end(list)   (list)->_M_node

#define bcb6_std_list_iterator_increment(it) ((it) = (it)->_M_next)
#define bcb6_std_list_iterator_decrement(it) ((it) = (it)->_M_prev)

EXTERN_C bcb6_std_list_iterator __fastcall bcb6_std_list_erase(bcb6_std_list_iterator it);

// create_node 4 bytes
__inline bcb6_std_list_iterator list_dword_push_back(bcb6_std_list *const self, const DWORD *const __x)
{
	pbcb6_std_list_node node = ((pbcb6_std_list_node(__cdecl *)(pbcb6_std_list, const void *))0x0050E758)(self, __x),
		edge = self->_M_node,
		tail = edge->_M_prev;
	node->_M_next = edge;
	node->_M_prev = tail;
	tail->_M_next = node;
	edge->_M_prev = node;
	return node;
}
