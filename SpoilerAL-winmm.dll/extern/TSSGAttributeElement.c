#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSGAttributeElement.h"
#include "bcb6_operator.h"

#pragma warning(disable:4733)

extern const DWORD F005D54CC;

__declspec(naked) TSSGAttributeElement * __cdecl new_TSSGAttributeElement()
{
	static const DWORD data1[] = {
		0x004D7D54,
		0x00002007, -4,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x00000000, -40,
		0x00050000, 0x00000000,
		0x00000000, 0x00050008,
		0x00000000,
		(DWORD)data1,
	};

	__asm
	{
		push    ebp
		mov     eax, offset data2
		mov     ebp, esp
		sub     esp, 40
		call    dword ptr [F005D54CC]
#if !OPTIMIZE_ALLOCATOR
		push    8
		call    dword ptr [operator_new]
		pop     ecx
#else
		mov     ecx, 8
		call    operator_new
#endif
		test    eax, eax
		jz      L1
		mov     dword ptr [eax], 006151C0H
		mov     dword ptr [eax + 4], 0
	L1:
		mov     ecx, dword ptr [ebp - 40]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
		ret
	}
}
