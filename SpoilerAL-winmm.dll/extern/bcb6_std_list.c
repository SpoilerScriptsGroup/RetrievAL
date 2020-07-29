#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_list.h"
#include "bcb6_std_allocator.h"

__declspec(naked) list_iterator __fastcall list_erase(list_iterator it)
{
	__asm
	{
		test    ecx, ecx
		jz      L1
		mov     eax, dword ptr [ecx + list_node._M_next]
		mov     edx, dword ptr [ecx + list_node._M_prev]
		mov     dword ptr [edx + list_node._M_next], eax
		mov     dword ptr [eax + list_node._M_prev], edx
		push    eax
#if !OPTIMIZE_ALLOCATOR
		push    size list_node + size ULONG// vary with type
		push    ecx
		call    dword ptr [node_alloc_deallocate]
		add     esp, 8
#else
		call    node_alloc_deallocate
#endif
		pop     ecx
	L1:
		mov     eax, ecx
		ret
	}
}
