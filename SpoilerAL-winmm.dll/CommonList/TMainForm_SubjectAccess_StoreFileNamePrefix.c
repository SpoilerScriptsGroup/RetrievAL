#include <windows.h>
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TMainForm_SubjectAccess_StoreFileNamePrefix()
{
	__asm
	{
		mov     eax, dword ptr [esp + 12]
		mov     edx, dword ptr [esp + 16]
		mov     cl, byte ptr [eax]
		cmp     cl, '+'
		je      L2
		cmp     cl, '*'
		jne     L1
		cmp     dword ptr [esp], 005020EFH
		je      L2
	L1:
		mov     byte ptr [esp + 64], cl
		jmp     dword ptr [TSSGCtrl_GetSSGDataFile]
		align   16
	L2:
		push    esi
		push    edi
		mov     edi, eax
		lea     esi, [eax + 1]
		mov     byte ptr [esp + 8 + 64], cl
		mov     ecx, edx
		dec     edx
		sub     ecx, eax
		rep movsb
		mov     dword ptr [esp + 8 + 16], edx
		pop     edi
		pop     esi
		jmp     dword ptr [TSSGCtrl_GetSSGDataFile]
	}
}
