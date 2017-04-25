#include "bcb6_stdio.h"

int(__cdecl *bcb6__snprintf)(char *buffer, size_t count, const char *format, ...) = (LPVOID)0x005D7EE8;

__declspec(naked) int __cdecl bcb6__vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
{
	__asm
	{
		mov     eax, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 4]
		test    eax, eax
		jz      L1
		mov     byte ptr [ecx], 0
	L1:
		mov     edx, dword ptr [esp + 16]
		lea     ecx, [esp + 4]
		push    edx
		push    eax
		mov     eax, dword ptr [esp + 8 + 12]
		push    1
		push    eax
		mov     eax, 005D8280H
		push    ecx
		push    005D7E58H
		call    eax
		add     esp, 24
		ret
	}
}

