#include "bcb6_std_allocator.h"

void *(__cdecl *bcb6_std_allocator_allocate)(size_t n) = (LPVOID)0x005F43F0;
void (__cdecl *bcb6_std_allocator_deallocate)(void *p, size_t n) = (LPVOID)0x005F47A0;

__declspec(naked) void * __fastcall bcb6_std_node_alloc_allocate(size_t n)
{
	__asm
	{
		cmp     ecx, 128
		push    ecx
		mov     eax, ecx
		jbe     L1
		call    dword ptr [bcb6_global_operator_new]
		pop     ecx
		ret
		align   16
	L1:
		test    ecx, ecx
		jz      L2
		call    dword ptr [bcb6_std_allocator_allocate]
	L2:
		pop     ecx
		ret
	}
}

__declspec(naked) void * __fastcall bcb6_std_node_alloc_deallocate(void *p, size_t n)
{
	__asm
	{
		test    ecx, ecx
		jz      L1
		cmp     edx, 128
		jbe     L2
		push    ecx
		call    dword ptr [bcb6_global_operator_delete]
		pop     ecx
	L1:
		ret
		align   16
	L2:
		push    edx
		push    ecx
		call    dword ptr [bcb6_std_allocator_deallocate]
		add     esp, 8
		ret
	}
}
