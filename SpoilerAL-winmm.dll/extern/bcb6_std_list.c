#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_list.h"
#include "bcb6_std_allocator.h"

__declspec(naked) list_iterator __fastcall list_erase(list_iterator it)
{
	__asm
	{
		test    ecx, ecx
		jz      L1
		mov     eax, [ecx]list_node._M_next
		mov     edx, [ecx]list_node._M_prev
		mov     [edx]list_node._M_next, eax
		mov     [eax]list_node._M_prev, edx
		push    eax
#if !OPTIMIZE_ALLOCATOR
		push    size list_node + size ULONG// vary with type
		push    ecx
		call    node_alloc_deallocate
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
