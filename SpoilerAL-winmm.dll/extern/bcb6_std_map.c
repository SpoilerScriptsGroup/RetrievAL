#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_map.h"
#include "bcb6_std_allocator.h"

#pragma warning(disable:4733)

map_iterator(__cdecl *map_find)(map *map, LPVOID key) = (LPVOID)0x004D4008;
map_iterator(__cdecl *map_iterator_sub_one)(map_iterator it) = (LPVOID)0x005F5360;

extern const DWORD F005D54CC;
extern const DWORD F005F5078;
extern const DWORD F004D3BAC;

__declspec(naked) void __stdcall map_erase(map *map, map_iterator it)
{
	static const DWORD data1[] = {
		0x004D3B20,
		0x0000400F, -4,
		0x00000000,
	};
	static const DWORD data2[] = {
		0x00000000, -40,
		0x00050000, 0x00000000,
		0x00000000, 0x00050008,
		0x00000000,
		(DWORD)data1,
		0x00050014, 0x00000000,
		0x00000000, 0x00000000,
	};

	__asm
	{
		push    ebp
		mov     eax, offset data2
		mov     ebp, esp
		sub     esp, 152
		push    ebx
		mov     ebx, dword ptr [ebp + 8H]
		call    dword ptr [F005D54CC]
		mov     eax, dword ptr [ebx + 8H]
		mov     edx, dword ptr [ebp + 0CH]
		add     eax, 12
		mov     dword ptr [ebp - 30H], edx
		push    eax
		sub     eax, 4
		push    eax
		sub     eax, 4
		push    eax
		mov     dword ptr [ebp - 2CH], edx
		push    edx
		call    dword ptr [F005F5078]
		add     esp, 16
		lea     edx, [eax + 10H]
		test    edx, edx
		mov     dword ptr [ebp - 4H], edx
		jz      L1
		push    eax
		mov     eax, dword ptr [ebp - 0CH]
		push    0
		sub     eax, 8
		add     edx, 8
		mov     dword ptr [ebp - 0CH], eax
		mov     eax, dword ptr [edx]
		mov     ecx, dword ptr [edx + 4H]
		mov     dword ptr [ebp - 98H], eax
		mov     dword ptr [ebp - 94H], ecx
		mov     dword ptr [ebp - 78H], eax
		mov     dword ptr [ebp - 74H], ecx
		mov     dword ptr [ebp - 58H], eax
		mov     dword ptr [ebp - 54H], ecx
		mov     eax, dword ptr [edx + 8H]
		mov     ecx, dword ptr [edx + 0CH]
		mov     dword ptr [ebp - 90H], eax
		mov     dword ptr [ebp - 8CH], ecx
		mov     dword ptr [ebp - 70H], eax
		mov     dword ptr [ebp - 6CH], ecx
		mov     dword ptr [ebp - 50H], eax
		mov     dword ptr [ebp - 4CH], ecx
		mov     eax, dword ptr [edx + 10H]
		mov     ecx, dword ptr [edx + 14H]
		mov     dword ptr [ebp - 88H], eax
		mov     dword ptr [ebp - 84H], ecx
		mov     dword ptr [ebp - 68H], eax
		mov     dword ptr [ebp - 64H], ecx
		mov     dword ptr [ebp - 48H], eax
		mov     dword ptr [ebp - 44H], ecx
		mov     eax, dword ptr [edx + 18H]
		mov     ecx, dword ptr [edx + 1CH]
		mov     dword ptr [ebp - 80H], eax
		mov     dword ptr [ebp - 7CH], ecx
		mov     dword ptr [ebp - 60H], eax
		mov     dword ptr [ebp - 5CH], ecx
		mov     dword ptr [ebp - 40H], eax
		mov     dword ptr [ebp - 3CH], ecx
		mov     dword ptr [ebp - 38H], edx
		mov     dword ptr [ebp - 34H], edx
		push    edx
		call    dword ptr [F004D3BAC]
		mov     eax, dword ptr [esp + 8]
		add     esp, 8 + 4
	L1:
		test    eax, eax
		jz      L2
		push    88
		push    eax
		call    dword ptr [node_alloc_deallocate]
		add     esp, 8
	L2:
		mov     eax, dword ptr [ebx + 10H]
		mov     ecx, dword ptr [ebp - 40]
		dec     eax
		mov     dword ptr fs:[0], ecx
		mov     dword ptr [ebx + 10H], eax
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret     8
	}
}
