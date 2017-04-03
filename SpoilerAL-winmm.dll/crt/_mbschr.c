#include <windows.h>
#include <limits.h>

#ifndef _M_IX86
unsigned char *_mbschr(const unsigned char *string, unsigned int c)
{
	char c2;

	if (c > UCHAR_MAX || IsDBCSLeadByte(c))
		goto FAILED;
	do
	{
		c2 = *(string++);
		if (c2 == (unsigned char)c)
			goto SUCCESS;
	} while (c2 && (!IsDBCSLeadByte(c2) || *(string++)));
FAILED:
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
		ja      L1
		push    ebx
		call    IsDBCSLeadByte
		test    eax, eax
		jz      L2
	L1:
		xor     eax, eax
		jmp     L3
		align   16
	L2:
		mov     al, byte ptr [esi]
		inc     esi
		cmp     al, bl
		je      L4
		test    al, al
		jz      L3
		push    eax
		call    IsDBCSLeadByte
		test    eax, eax
		jz      L2
		mov     al, byte ptr [esi]
		inc     esi
		and     eax, 0FFH
		jnz     L2
	L3:
		pop     esi
		pop     ebx
		ret
		align   16
	L4:
		mov     eax, esi
		pop     esi
		dec     eax
		pop     ebx
		ret
	}
}
#endif
