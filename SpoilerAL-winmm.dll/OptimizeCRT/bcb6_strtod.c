#include <stdlib.h>
#include "bcb6_errno.h"

__declspec(naked) double __cdecl bcb6_strtod(const char *nptr, char **endptr)
{
	__asm
	{
		mov     edx, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 4]
		push    edx
		push    ecx
		call    strtod
		mov     ecx, edx
		add     esp, 8
		and     ecx, 7FF00000H
		cmp     ecx, 7FF00000H
		jne     L1
		push    eax
		push    edx
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ERANGE
		pop     edx
		pop     eax
	L1:
		ret
	}
}
