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
		test    ebx, 0FFFF0000H
		jnz     L1
		test    bh, bh
		jnz     L3
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		mov     ecx, eax
		xor     eax, eax
		test    ecx, ecx
		jnz     L8
		dec     esi
		jmp     L2

		align   16
	L1:
		xor     eax, eax
		jmp     L8

		align   16
	L2:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bl
		je      L7
		test    al, al
		jz      L8
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L8
		inc     esi
		jmp     L2

		align   16
	L3:
		mov     eax, ebx
		mov     ecx, ebx
		and     eax, 0FFH
		jz      L8
		shr     ecx, 8
		dec     esi
		push    ecx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L8
		xor     eax, eax

		align   16
	L4:
		mov     al, byte ptr [esi + 1]
		inc     esi
		test    al, al
		jz      L8
		cmp     al, bh
		je      L5
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L4
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jnz     L6
		jmp     L8

		align   16
	L5:
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		cmp     cl, bl
		je      L7
		test    cl, cl
		jz      L8
	L6:
		inc     esi
		jmp     L4

		align   16
	L7:
		mov     eax, esi
	L8:
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
