#include <windows.h>
#include <limits.h>

#ifndef _M_IX86
unsigned char *_mbschr(const unsigned char *string, unsigned int c)
{
	unsigned char c2;

	if (c <= UCHAR_MAX && !IsDBCSLeadByteEx(CP_THREAD_ACP, c))
		do
			if ((c2 = *(string++)) == (unsigned char)c)
				return (unsigned char *)string - 1;
		while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(string++)));
	return NULL;
}
#else
__declspec(naked) unsigned char *_mbschr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [c + 8]
		mov     esi, dword ptr [string + 8]
		cmp     ebx, 0FFH
		ja      L2
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jnz     L2

		align   16
	L1:
		mov     al, byte ptr [esi]
		inc     esi
		cmp     al, bl
		je      L4
		and     eax, 0FFH
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jnz     L1
	L2:
		xor     eax, eax
	L3:
		pop     esi
		pop     ebx
		ret

		align   16
	L4:
		lea     eax, [esi - 1]
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
