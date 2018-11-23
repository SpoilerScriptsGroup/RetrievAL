#pragma once

#include <windows.h>

typedef struct
{
	LPVOID unknown[16];
} bcb6_std_stack, *pbcb6_std_stack;

typedef bcb6_std_stack  bcb6_std_stack_dword;
typedef pbcb6_std_stack pbcb6_std_stack_dword;

#ifdef USING_NAMESPACE_BCB6_STD
typedef bcb6_std_stack        stack;
typedef pbcb6_std_stack       pstack;
typedef bcb6_std_stack_dword  stack_dword;
typedef pbcb6_std_stack_dword pstack_dword;
#define stack_dword_push bcb6_std_stack_dword_push
#define stack_dword_pop  bcb6_std_stack_dword_pop
#define stack_dword_size bcb6_std_stack_dword_size
#define stack_dword_top  bcb6_std_stack_dword_top
#endif

void __fastcall bcb6_std_stack_dword_push(stack_dword *this, DWORD value);
void __fastcall bcb6_std_stack_dword_pop(stack_dword *this);

__inline size_t bcb6_std_stack_dword_size(bcb6_std_stack *this)
{
	return (
		((size_t *)this)[7] - ((size_t *)this)[3] +
		((size_t *)this)[4] - ((size_t *)this)[5] +
		((size_t *)this)[2] - ((size_t *)this)[0]) / sizeof(DWORD) - 1;
}

__inline DWORD bcb6_std_stack_dword_top(bcb6_std_stack *this)
{
	return
		((DWORD **)this)[4] != ((DWORD **)this)[5] ? \
			((DWORD **)this)[4][-1] : \
			((DWORD ***)this)[7][-1][0];
}
