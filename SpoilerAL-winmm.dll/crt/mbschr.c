#include <windows.h>
#include <limits.h>

#ifndef _M_IX86
unsigned char *_mbschr(const unsigned char *string, unsigned int c)
{
	unsigned char c2;

	if (c <= USHORT_MAX)
		if (c <= UCHAR_MAX) {
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				do
					if ((c2 = *(string++)) == (unsigned char)c)
						return (unsigned char *)string - 1;
				while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(string++)));
		} else
			if ((c & 0xFF) && IsDBCSLeadByteEx(CP_THREAD_ACP, c >> 8))
				for (; ; ) {
					if ((c2 = *(string++)) == (unsigned char)(c >> 8)) {
						if ((c2 = *(string++)) == (unsigned char)(c & 0xFF))
							return (unsigned char *)string - 2;
					} else
						if (!c2)
							break;
						else if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
							continue;
						else
							c2 = *(string++);
					if (!c2)
						break;
				}
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
		cmp     ebx, 0FFFFH
		ja      L2
		cmp     ebx, 0FFH
		ja      L5
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

		align   16
	L5:
		test    ebx, 0FFH
		jz      L10
		movzx   eax, bh
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L10

		align   16
	L6:
		mov     al, byte ptr [esi]
		inc     esi
		cmp     al, bl
		jne     L9
		mov     al, byte ptr [esi]
		inc     esi
		cmp     al, bl
		je      L11
		jmp     L9
	L7:
		and     al, 0FFH
		jz      L10
	L8:
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L6
		mov     al, byte ptr [esi]
		inc     esi
	L9:
		test    al, al
		jnz     L6
	L10:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	L11:
		lea     eax, [esi - 2]
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
