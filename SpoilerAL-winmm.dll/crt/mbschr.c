#include <windows.h>
#include <limits.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	unsigned char c2;

	if (c <= USHRT_MAX)
		if (c <= UCHAR_MAX) {
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c)) {
				string--;
				do
					if ((c2 = *(++string)) == (unsigned char)c)
						goto DONE;
				while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(++string)));
			}
		} else if ((unsigned char)c && IsDBCSLeadByteEx(CP_THREAD_ACP, c >> 8)) {
			string--;
			while (c2 = *(++string)) {
				if (c2 != (unsigned char)(c >> 8))
					if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
						continue;
					else if (!string[1])
						break;
				else if (!(c2 = string[1]))
					break;
				else if (c2 == (unsigned char)c)
					goto DONE;
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
		mov     ebx, dword ptr [c + 8]
		mov     esi, dword ptr [string + 8]
		test    ebx, 0FFFFFF00H
		jnz     L2
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		mov     ecx, eax
		xor     eax, eax
		test    ecx, ecx
		jnz     L7
		dec     esi

		align   16
	L1:
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
		jz      L1
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L7
		inc     esi
		jmp     L1

		align   16
	L2:
		mov     ecx, ebx
		xor     eax, eax
		test    ecx, 0FFFF0000H
		jnz     L7
		test    ecx, 0FFH
		jz      L7
		shr     ecx, 8
		dec     esi
		push    ecx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L7
		xor     eax, eax

		align   16
	L3:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bh
		je      L4
		test    al, al
		jz      L7
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L3
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jnz     L5
		jmp     L7

		align   16
	L4:
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		cmp     cl, bl
		je      L6
		test    cl, cl
		jz      L7
	L5:
		inc     esi
		jmp     L3

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
