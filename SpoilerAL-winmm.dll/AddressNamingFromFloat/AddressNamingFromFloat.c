#include "TStringDivision.h"

__declspec(naked) void __cdecl AddressNamingFromFloat()
{
	__asm
	{
		mov     eax, dword ptr [ebp - 01ECH]
		mov     ecx, dword ptr [ebp - 01E8H]
		cmp     eax, 8
		jae     L2
		cmp     eax, 4
		jae     L1
		mov     ecx, 0051A768H
	L1:
		fld     dword ptr [ecx]
		jmp     L3
	L2:
		fld     qword ptr [ecx]
	L3:
		fstp    qword ptr [ebp - 0420H]
		mov     eax, dword ptr [esi]
		mov     word ptr [ebx + 10H], 1340
		mov     eax, dword ptr [eax + 78H]
		mov     ecx, dword ptr [ebp - 041CH]
		mov     edx, dword ptr [ebp - 0420H]
		push    eax
		push    ecx
		lea     eax, [ebp - 0144H]
		push    edx
		push    eax
		call    TStringDivision_ToStringDouble
		mov     eax, 005062A0H
		add     esp, 16
		jmp     eax
	}
}
