#include <windows.h>

#ifndef _M_IX86
size_t __cdecl _mbscspn(const unsigned char *string1, const unsigned char *string2)
{
	const unsigned char *p1, *p2;
	unsigned char       c1, c2, trail;

	for (p1 = string1; c1 = *p1++; )
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c1)) {
			for (p2 = string2; ; )
				if (!(c2 = *p2++))
					break;
				else if (c2 == c1)
					goto DONE;
				else if (IsDBCSLeadByteEx(CP_THREAD_ACP, c2) && !(*p2++))
					break;
		} else {
			if (!(trail = *p1++))
				break;
			for (p2 = string2; ; )
				if (!(c2 = *p2++))
					break;
				else if (c2 != c1) {
					if (IsDBCSLeadByteEx(CP_THREAD_ACP, c2) && !(*p2++))
						break;
				} else if (!(c2 = *p2++))
					break;
				else if (c2 == trail)
					goto DONE;
		}
DONE:
	return (p1 - 1 - string1);
}
#else
__declspec(naked) size_t __cdecl _mbscspn(const unsigned char *string1, const unsigned char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		push    esi
		push    edi
		mov     edi, dword ptr [string1 + 12]

		align   16
	L1:
		mov     bl, byte ptr [edi]
		inc     edi
		and     ebx, 0FFH
		jz      L6
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jnz     L3
		mov     esi, dword ptr [string2 + 12]

		align   16
	L2:
		mov     al, byte ptr [esi]
		inc     esi
		and     eax, 0FFH
		jz      L1
		cmp     al, bl
		je      L6
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jnz     L2
		jmp     L1

		align   16
	L3:
		mov     bh, byte ptr [edi]
		inc     edi
		test    bh, bh
		jz      L6
		mov     esi, dword ptr [string2 + 12]

		align   16
	L4:
		mov     al, byte ptr [esi]
		inc     esi
		and     eax, 0FFH
		jz      L1
		cmp     al, bl
		je      L5
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L4
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jnz     L4
		jmp     L1

		align   16
	L5:
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L1
		cmp     al, bh
		jne     L4
	L6:
		mov     ecx, dword ptr [string1 + 12]
		lea     eax, [edi - 1]
		sub     eax, ecx
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
