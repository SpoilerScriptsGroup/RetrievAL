#pragma once

#include <windows.h>
#include "bcb6_std_deque.h"

typedef struct _Deque_base
bcb6_std_stack, *pbcb6_std_stack;

typedef bcb6_std_stack
bcb6_std_stack_dword, *pbcb6_std_stack_dword;

#define bcb6_std_stack_Get_c(this) ((bcb6_std_deque *)(this))
#define bcb6_std_stack_size        bcb6_std_deque_size
#define bcb6_std_stack_top         bcb6_std_deque_back

#define bcb6_std_stack_dword_pop   bcb6_std_deque_ptr_pop_back
#define bcb6_std_stack_dword_push  bcb6_std_deque_ptr_push_back

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_stack        stack;
typedef pbcb6_std_stack       pstack;
typedef bcb6_std_stack_dword  stack_ptr;
typedef pbcb6_std_stack_dword pstack_ptr;
#define stack_Get_c      bcb6_std_stack_Get_c
#define stack_size       bcb6_std_stack_size
#define stack_top        bcb6_std_stack_top
#define stack_ptr_push   bcb6_std_stack_dword_push
#define stack_ptr_pop    bcb6_std_stack_dword_pop
#endif
