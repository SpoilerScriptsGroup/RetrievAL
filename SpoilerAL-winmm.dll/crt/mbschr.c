#include <windows.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	unsigned char c2;

	if (!(c & ~0xFF)) {
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c)) {
			string--;
			do
				if ((c2 = *(++string)) == (unsigned char)c)
					goto DONE;
			while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(++string)));
		}
	} else if (!(c & ~0xFFFF) && (c & 0xFF) && IsDBCSLeadByteEx(CP_THREAD_ACP, c >> 8)) {
		for (string--; ; ) {
			if ((c2 = *(++string)) != (unsigned char)(c >> 8))
				if (!c2)
					break;
				else if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
					continue;
				else if (!string[1])
					break;
			else if ((c2 = string[1]) == (unsigned char)c)
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
__declspec(naked) unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		push    esi
		mov     eax, dword ptr [c + 8]
		mov     esi, dword ptr [string + 8]
		mov     ebx, eax
		test    eax, not 0FFH
		jnz     L2
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jnz     L5
		dec     esi

		align   16
	L1:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bl
		je      L7
		test    al, al
		jz      L6
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L6
		inc     esi
		jmp     L1

		align   16
	L2:
		test    eax, not 0FFFFH
		jnz     L5
		test    al, al
		jz      L5
		shr     eax, 8
		dec     esi
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L6
		xor     eax, eax

		align   16
	L3:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bh
		je      L4
		test    al, al
		jz      L6
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L3
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L6
		inc     esi
		jmp     L3

		align   16
	L4:
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		cmp     cl, bl
		je      L7
		test    cl, cl
		jz      L6
		inc     esi
		jmp     L3

		align   16
	L5:
		xor     eax, eax
	L6:
		pop     esi
		pop     ebx
		ret

		align   16
	L7:
		mov     eax, esi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
