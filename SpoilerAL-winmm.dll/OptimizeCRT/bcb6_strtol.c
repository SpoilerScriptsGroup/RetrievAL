#include <stdlib.h>
#include "bcb6_errno.h"

__declspec(naked) long __cdecl bcb6_strtol(const char *nptr, char **endptr, int base)
{
	__asm
	{
		#define nptr   (esp + 4)
		#define endptr (esp + 8)
		#define base   (esp + 12)

		call    _errno
		push    ebx
		mov     ebx, eax
		mov     edx, dword ptr [eax]
		mov     eax, dword ptr [nptr + 4]
		mov     dword ptr [ebx], 0
		dec     eax
		align   16
	L1:
		mov     cl, byte ptr [eax + 1]
		inc     eax
		cmp     cl, ' '
		je      L1
		cmp     cl, 0DH
		ja      L2
		cmp     cl, 09H
		jae     L1
	L2:
		push    ecx
		push    edx
		mov     edx, dword ptr [base + 12]
		mov     ecx, dword ptr [endptr + 12]
		mov     eax, dword ptr [nptr + 12]
		push    edx
		push    ecx
		push    eax
		call    strtoul
		add     esp, 12
		mov     edx, dword ptr [ebx]
		pop     ecx
		test    edx, edx
		mov     dword ptr [ebx], ecx
		pop     ecx
		jnz     L5
		cmp     cl, '-'
		je      L3
		test    eax, eax
		jns     L4
		jmp     L6
	L3:
		test    eax, eax
		jg      L7
	L4:
		pop     ebx
		ret

		align   16
	L5:
		cmp     edx, ERANGE
		jne     L8
		cmp     cl, '-'
		je      L7

		align   16
	L6:
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ERANGE
		mov     eax, LONG_MAX
		pop     ebx
		ret

		align   16
	L7:
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_ERANGE
		mov     eax, LONG_MIN
		pop     ebx
		ret

		align   16
	L8:
		call    dword ptr [_bcb6_errno]
		mov     dword ptr [eax], BCB6_EINVAL
		xor     eax, eax
		pop     ebx
		ret

		#undef nptr
		#undef endptr
		#undef base
	}
}
