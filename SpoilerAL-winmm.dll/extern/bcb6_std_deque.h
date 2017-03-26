#pragma once

#include <windows.h>

typedef struct
{
	LPVOID *_M_cur;
	LPVOID *_M_first;
	LPVOID *_M_last;
	LPVOID **_M_node;
} bcb6_std_deque_iterator, *pbcb6_std_deque_iterator;

typedef struct
{
	bcb6_std_deque_iterator _M_start;
	bcb6_std_deque_iterator _M_finish;
} bcb6_std_deque, *pbcb6_std_deque;

#define bcb6_std_deque_empty(deque) (deque->_M_finish._M_cur == deque->_M_start._M_cur)
#define bcb6_std_deque_begin(deque) &deque->_M_start
#define bcb6_std_deque_end(deque) &deque->_M_finish

#define bcb6_std_deque_iterator_decrement(it) \
	((it)->_M_cur = ((it)->_M_cur != (it)->_M_first) ? (it)->_M_cur - 1 : ((it)->_M_last = ((it)->_M_first = *--(it)->_M_node) + 1) - 1)

#define bcb6_std_deque_iterator_greater_or_equal(it1, it2) \
	(((it1)->_M_node == (it2)->_M_node && (it1)->_M_cur >= (it2)->_M_cur) || (it1)->_M_node >= (it2)->_M_node)

EXTERN_C void __stdcall bcb6_std_deque_erase_element_size_4(bcb6_std_deque *deque, bcb6_std_deque_iterator *it);
