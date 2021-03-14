#define USING_NAMESPACE_BCB6_STD
#include "TWinControl.h"

__declspec(naked) HWND __fastcall TWinControl_GetHandle(LPCVOID WinControl)
{
	__asm
	{
		mov     edx, 0058750CH
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) int __fastcall TWinControl_GetTextWidth(LPCVOID WinControl, const struct bcb6_std_string *s)
{
	extern const DWORD _TWinControl_GetHandle;

	__asm
	{
		push    esi
		mov     eax, ecx
		mov     esi, edx
		sub     esp, 12
		test    eax, eax
		jz      epilog
		call    dword ptr [_TWinControl_GetHandle]
		test    eax, eax
		jz      epilog
		mov     dword ptr [esp + 8], eax
		push    eax
		call    GetDC
		test    eax, eax
		jz      epilog
		mov     ecx, dword ptr [esi]
		mov     edx, dword ptr [esi + 4]
		sub     edx, ecx
		mov     esi, eax
		push    esp
		push    edx
		push    ecx
		push    eax
		call    GetTextExtentPoint32A
		test    eax, eax
		jz      release
		mov     eax, dword ptr [esp]
	release:
		mov     ecx, dword ptr [esp + 8]
		mov     edx, esi
		mov     esi, eax
		push    edx
		push    ecx
		call    ReleaseDC
		mov     eax, esi
	epilog:
		add     esp, 12
		pop     esi
		ret
	}
}
