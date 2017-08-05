#include <stdlib.h>
#include "bcb6_errno.h"

__declspec(naked) unsigned long __cdecl bcb6_strtoul(const char *nptr, char **endptr, int base)
{
	__asm
	{
		call    _errno
		mov     ecx, dword ptr [eax]
		push    ebx
		mov     dword ptr [eax], 0
		push    ecx
		mov     ebx, eax
		mov     edx, dword ptr [esp + 8 + 12]
		mov     ecx, dword ptr [esp + 8 + 8]
		mov     eax, dword ptr [esp + 8 + 4]
		push    edx
		push    ecx
		push    eax
		call    strtoul
		add     esp, 12
		mov     ecx, dword ptr [ebx]
		pop     edx
		test    ecx, ecx
		mov     dword ptr [ebx], edx
		jz      L2
		cmp     ecx, EINVAL
		jne     L1
		sub     ecx, EINVAL - BCB6_EINVAL
	L1:
		push    eax
		mov     ebx, ecx
		call    dword ptr [bcb6___errno]
		mov     dword ptr [eax], ebx
		pop     eax
	L2:
		pop     ebx
		ret
	}
}
