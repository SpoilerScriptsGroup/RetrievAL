#include <stdlib.h>

__declspec(naked) unsigned long __cdecl strtol(const char *nptr, char **endptr, int base)
{
	__asm
	{
		call    _errno
		push    ebx
		mov     ebx, eax
		mov     edx, dword ptr [eax]
		mov     eax, dword ptr [esp + 4 + 4]
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
		push    edx
		push    ecx
		mov     edx, dword ptr [esp + 12 + 12]
		mov     ecx, dword ptr [esp + 12 + 8]
		push    edx
		push    ecx
		push    eax
		call    strtoul
		mov     ecx, dword ptr [ebx]
		add     esp, 12
		test    ecx, ecx
		jnz     L5
		pop     ecx
		pop     edx
		cmp     cl, '-'
		je      L3
		test    eax, eax
		jns     L4
		jmp     L6
	L3:
		test    eax, eax
		jns     L7
	L4:
		mov     dword ptr [ebx], edx
		pop     ebx
		ret

		align   16
	L5:
		cmp     ecx, ERANGE
		pop     ecx
		pop     edx
		jne     L8
		cmp     cl, '-'
		je      L7

		align   16
	L6:
		mov     dword ptr [ebx], ERANGE
		mov     eax, LONG_MAX
		pop     ebx
		ret

		align   16
	L7:
		mov     dword ptr [ebx], ERANGE
		mov     eax, LONG_MIN
		pop     ebx
		ret

		align   16
	L8:
		mov     dword ptr [ebx], EINVAL
		xor     eax, eax
		pop     ebx
		ret
	}
}
