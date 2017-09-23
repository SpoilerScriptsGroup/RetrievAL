#include <stdlib.h>
#include "bcb6_errno.h"

__declspec(naked) unsigned long __cdecl bcb6_strtoul(const char *nptr, char **endptr, int base)
{
	__asm
	{
		call    _errno
		mov     ecx, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		mov     dword ptr [esp + 4], ebx
		mov     dword ptr [esp + 8], esi
		mov     ebx, eax
		mov     esi, dword ptr [eax]
		mov     dword ptr [eax], 0
		mov     eax, dword ptr [esp + 12]
		push    eax
		push    edx
		push    ecx
		call    strtoul
		mov     ecx, dword ptr [ebx]
		add     esp, 12
		mov     dword ptr [ebx], esi
		test    ecx, ecx
		jz      L2
		cmp     ecx, EINVAL
		jne     L1
		sub     ecx, EINVAL - BCB6_EINVAL
	L1:
		push    eax
		mov     ebx, ecx
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], ebx
		pop     eax
	L2:
		mov     ebx, dword ptr [esp + 4]
		mov     esi, dword ptr [esp + 8]
		ret
	}
}
