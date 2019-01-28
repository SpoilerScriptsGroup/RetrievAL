#include <windows.h>
#include <limits.h>

#ifndef _M_IX86
#include <intrin.h>
#pragma intrinsic(_byteswap_ushort)

unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c)
{
	if (c <= USHRT_MAX)
		if (c <= UCHAR_MAX) {
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c)) {
				unsigned char c2;

				do
					if ((c2 = *(string++)) == (unsigned char)c)
						return (unsigned char *)string - 1;
				while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(string++)));
			}
		} else
			if ((unsigned char)c && IsDBCSLeadByteEx(CP_THREAD_ACP, (c = _byteswap_ushort(c)) & 0xFF)) {
				unsigned int c2;

				while (c2 = *(string++)) {
					if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
						continue;
					if (!(((unsigned char *)&c2)[1] = *(string++)))
						break;
					if (c2 != c)
						continue;
					return (unsigned char *)string - 2;
				}
			}
	return NULL;
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
		cmp     ebx, 0FFFFH
		ja      L2
		cmp     bh, bh
		jnz     L5
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
		push    edi
		xor     eax, eax
		test    bl, bl
		jz      L8
		mov     al, bh
		push    eax
		mov     ah, bl
		push    CP_THREAD_ACP
		mov     edi, eax
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L8

		align   16
	L6:
		mov     bl, byte ptr [esi]
		inc     esi
		and     ebx, 0FFH
		jz      L7
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L6
		mov     bh, byte ptr [esi]
		inc     esi
		test    bh, bh
		jz      L7
		cmp     ebx, edi
		jne     L6
		pop     edi
		lea     eax, [esi - 2]
		pop     esi
		pop     ebx
		ret

		align   16
	L7:
		xor     eax, eax
	L8:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
