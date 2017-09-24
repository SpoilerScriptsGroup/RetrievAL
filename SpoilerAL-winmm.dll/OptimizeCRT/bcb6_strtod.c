#include <stdlib.h>
#include "bcb6_errno.h"

__declspec(naked) double __cdecl bcb6_strtod(const char *nptr, char **endptr)
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
		push    edx
		push    ecx
		call    strtod
		mov     ecx, dword ptr [ebx]
		add     esp, 8
		mov     dword ptr [ebx], esi
		test    ecx, ecx
		jz      L1
		push    eax
		push    edx
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ERANGE
		pop     edx
		pop     eax
	L1:
		mov     ebx, dword ptr [esp + 4]
		mov     esi, dword ptr [esp + 8]
		ret
	}
}
