#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_list.h"
#include "bcb6_std_allocator.h"

__declspec(naked) void __fastcall list_erase(list_iterator *it)
{
	__asm
	{
		mov     ecx, dword ptr [ecx]
		test    ecx, ecx
		jz      L1
		mov     eax, dword ptr [ecx]
		mov     edx, dword ptr [ecx + 4H]
		mov     dword ptr [edx], eax
		mov     dword ptr [eax + 4H], edx
#if !OPTIMIZE_ALLOCATOR
		push    12
		push    ecx
		call    dword ptr [node_alloc_deallocate]
		add     esp, 8
#else
		call    node_alloc_deallocate
#endif
	L1:
		ret
	}
}
