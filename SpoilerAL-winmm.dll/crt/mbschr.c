#include <windows.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	unsigned char c2;

	if (c < 0x100) {
		if (!c)
			return (unsigned char *)string + strlen((char *)string);
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c)) {
			string--;
			do
				if ((c2 = *(++string)) == (unsigned char)c)
					goto DONE;
			while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(++string)));
		}
	} else if (c < 0x10000 && (c & 0xFF) && IsDBCSLeadByteEx(CP_THREAD_ACP, c >> 8)) {
		for (string--; ; ) {
			if ((c2 = *(++string)) != (unsigned char)(c >> 8)) {
				if (!c2)
					break;
				if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
					continue;
				if (!string[1])
					break;
			} else if ((c2 = string[1]) == (unsigned char)c)
				goto DONE;
			else if (!c2)
				break;
			string++;
		}
	}
	string = NULL;
DONE:
	return (unsigned char *)string;
}
#else
#pragma function(strlen)

__declspec(naked) unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [c + 8]
		mov     esi, dword ptr [string + 8]
		cmp     ebx, 100H
		jae     L3
		test    ebx, ebx
		jnz     L1
		push    esi
		call    strlen
		pop     ecx
		add     eax, esi
		jmp     L7

		align   16
	L1:
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		mov     ecx, eax
		xor     eax, eax
		test    ecx, ecx
		jnz     L7
		dec     esi

		align   16
	L2:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bl
		je      L6
		test    al, al
		jz      L7
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L7
		inc     esi
		jmp     L2

		align   16
	L3:
		xor     eax, eax
		cmp     ebx, 10000H
		jae     L7
		test    bl, bl
		jz      L7
		mov     al, bh
		dec     esi
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L7
		xor     eax, eax

		align   16
	L4:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bh
		je      L5
		test    al, al
		jz      L7
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L4
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L7
		inc     esi
		jmp     L4

		align   16
	L5:
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		cmp     cl, bl
		je      L6
		test    cl, cl
		jz      L7
		inc     esi
		jmp     L4

		align   16
	L6:
		mov     eax, esi
	L7:
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
