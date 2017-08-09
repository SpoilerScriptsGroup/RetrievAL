#include "bcb6_stdio.h"

static const DWORD F005D8280 = 0x005D8280;

__declspec(naked) int __cdecl bcb6_snprintf(char *buffer, size_t count, const char *format, ...)
{
	__asm
	{
		mov     eax, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 4]
		test    eax, eax
		jz      L1
		mov     byte ptr [ecx], 0
	L1:
		lea     ecx, [esp + 16]
		push    ecx
		push    eax
		mov     ecx, dword ptr [esp + 8 + 12]
		lea     eax, [esp + 8 + 4]
		push    1
		push    ecx
		push    eax
		push    005D7E58H
		call    dword ptr [F005D8280]
		add     esp, 24
		ret
	}
}

__declspec(naked) int __cdecl bcb6_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
{
	__asm
	{
		mov     eax, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 4]
		test    eax, eax
		jz      L1
		mov     byte ptr [ecx], 0
	L1:
		mov     ecx, dword ptr [esp + 16]
		push    ecx
		push    eax
		mov     ecx, dword ptr [esp + 8 + 12]
		lea     eax, [esp + 8 + 4]
		push    1
		push    ecx
		push    eax
		push    005D7E58H
		call    dword ptr [F005D8280]
		add     esp, 24
		ret
	}
}

