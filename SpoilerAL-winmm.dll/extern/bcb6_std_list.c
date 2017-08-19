#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_list.h"
#include "bcb6_std_allocator.h"

__declspec(naked) void __fastcall list_erase(list_iterator *it)
{
	__asm
	{
		mov     eax, dword ptr [ecx]
		test    eax, eax
		jz      L1
		mov     ecx, dword ptr [eax]
		mov     edx, dword ptr [eax + 4H]
		mov     dword ptr [edx], ecx
		mov     dword ptr [ecx + 4H], edx
		push    12
		push    eax
		call    dword ptr [node_alloc_deallocate]
		add     esp, 8
	L1:
		ret
	}
}
