#include <windows.h>

extern const BOOL EnableParserFix;
extern const DWORD F005D54CC;

__declspec(naked) void __cdecl Caller_StrToProcessAccessElementVecWithoutRel()
{
	__asm
	{
		cmp     dword ptr [EnableParserFix], 0
		je      L2
		mov     ecx, dword ptr [esp +  4]
		mov     edx, dword ptr [esp     ]
		push    ecx
		push    offset L1
		mov     ecx, dword ptr [esp + 16]
		push    ebp
		mov     ebp, esp
		sub     esp, 4092
		push    eax
		mov     eax, dword ptr [ebp + 24]
		mov     dword ptr [ebp + 12], ecx
		mov     dword ptr [ebp + 16], eax
		mov     ecx, dword ptr [ebp + 28]
		mov     eax, dword ptr [ebp + 32]
		mov     dword ptr [ebp + 20], ecx
		mov     dword ptr [ebp + 24], eax
		mov     ecx, dword ptr [ebp + 36]
		mov     eax, dword ptr [ebp + 40]
		mov     dword ptr [ebp + 28], ecx
		mov     dword ptr [ebp + 32], eax
		mov     ecx, dword ptr [ebp + 44]
		mov     eax, dword ptr [ebp + 48]
		mov     dword ptr [ebp + 36], ecx
		mov     dword ptr [ebp + 40], eax
		mov     dword ptr [ebp + 44], 0
		mov     dword ptr [ebp + 48], edx
		sub     esp, 760
		mov     eax, 0063B614H
		push    ebx
		lea     ebx, [ebp - 5D4H]
		push    esi
		push    edi
		push    00506F9BH
		jmp     dword ptr [F005D54CC]

		align   16
	L1:
		mov     eax, dword ptr [esp + 40]
		add     esp, 8
		jmp     eax

		align   16
	L2:
		push    ebp
		mov     eax, 00506F81H
		mov     ebp, esp
		sub     esp, 4092
		jmp     eax
	}
}
