#include <stdlib.h>
#include "bcb6_errno.h"

#ifdef _DEBUG
errno_t * __cdecl _errno();
#else
EXTERN_C errno_t _terrno;
#endif

__declspec(naked) double __cdecl bcb6_strtod(const char *nptr, char **endptr)
{
	__asm
	{
		mov     eax, dword ptr [esp + 4]
		mov     ecx, dword ptr [esp + 8]
#ifdef _DEBUG
		mov     dword ptr [esp + 4], ebx
		mov     dword ptr [esp + 8], esi
		push    ecx
		push    eax
		call    _errno
		mov     esi, dword ptr [eax]
		mov     ebx, eax
		mov     dword ptr [eax], 0
		call    strtod
		mov     eax, dword ptr [ebx]
		add     esp, 8
		mov     dword ptr [ebx], esi
		mov     ebx, dword ptr [esp + 4]
		mov     esi, dword ptr [esp + 8]
#else
		mov     dword ptr [esp + 4], ebx
		mov     ebx, dword ptr [_terrno]
		push    ecx
		push    eax
		mov     dword ptr [_terrno], 0
		call    strtod
		mov     eax, dword ptr [_terrno]
		add     esp, 8
		mov     dword ptr [_terrno], ebx
		mov     ebx, dword ptr [esp + 4]
#endif
		test    eax, eax
		jz      L1
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ERANGE
	L1:
		ret
	}
}
