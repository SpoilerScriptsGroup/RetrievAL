#include <windows.h>
#include <limits.h>

#ifndef _M_IX86
unsigned char *_mbschr(const unsigned char *string, unsigned int c)
{
	if (c <= UCHAR_MAX && !IsDBCSLeadByteEx(CP_THREAD_ACP, c))
	{
		unsigned char c2;

		do
			if ((c2 = *(string++)) == (unsigned char)c)
				goto SUCCESS;
		while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(string++)));
	}
	return NULL;
SUCCESS:
	return (unsigned char *)string - 1;
}
#else
__declspec(naked) unsigned char *_mbschr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		push    ebx
		push    esi
		mov     ebx, dword ptr [esp + 16]
		mov     esi, dword ptr [esp + 12]
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
		je      L3
		test    al, al
		jz      L2
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
		pop     esi
		pop     ebx
		ret

		align   16
	L3:
		mov     eax, esi
		pop     esi
		dec     eax
		pop     ebx
		ret
	}
}
#endif
