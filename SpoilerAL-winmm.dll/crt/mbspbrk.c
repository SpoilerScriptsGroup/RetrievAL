#include <windows.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbspbrk(const unsigned char *string, const unsigned char *control)
{
	const unsigned char *p1, *p2;
	unsigned char       c1, c2, trail;

	p1 = string;
	for (; ; ) {
		if (!(c1 = *p1++))
			goto NOT_FOUND;
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c1)) {
			for (p2 = control; ; )
				if (!(c2 = *p2++))
					goto DONE;
				else if (c2 == c1)
					break;
				else if (IsDBCSLeadByteEx(CP_THREAD_ACP, c2) && !(*p2++))
					goto DONE;
		} else {
			if (!(trail = *p1++))
				goto NOT_FOUND;
			for (p2 = control; ; )
				if (!(c2 = *p2++))
					goto DONE;
				else if (c2 != c1) {
					if (IsDBCSLeadByteEx(CP_THREAD_ACP, c2) && !(*p2++))
						goto DONE;
				} else if (!(c2 = *p2++))
					goto DONE;
				else if (c2 == trail)
					break;
		}
	}
DONE:
	return (unsigned char *)(p1 - 1);

NOT_FOUND:
	return NULL;
}
#else
__declspec(naked) unsigned char * __cdecl _mbspbrk(const unsigned char *string, const unsigned char *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		push    ebx
		push    esi
		push    edi
		mov     esi, dword ptr [string + 12]

		align   16
	L1:
		mov     bl, byte ptr [esi]
		inc     esi
		and     ebx, 0FFH
		jz      L7
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jnz     L3
		mov     edi, dword ptr [control + 12]

		align   16
	L2:
		mov     al, byte ptr [edi]
		inc     edi
		and     eax, 0FFH
		jz      L6
		cmp     al, bl
		je      L1
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     al, byte ptr [edi]
		inc     edi
		test    al, al
		jnz     L2
		jmp     L6

		align   16
	L3:
		mov     bh, byte ptr [esi]
		inc     esi
		test    bh, bh
		jz      L7
		mov     edi, dword ptr [control + 12]

		align   16
	L4:
		mov     al, byte ptr [edi]
		inc     edi
		and     eax, 0FFH
		jz      L6
		cmp     al, bl
		je      L5
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L4
		mov     al, byte ptr [edi]
		inc     edi
		test    al, al
		jnz     L4
		jmp     L6

		align   16
	L5:
		mov     al, byte ptr [edi]
		inc     edi
		test    al, al
		jz      L1
		cmp     al, bh
		jne     L4
		jmp     L1

		align   16
	L6:
		pop     edi
		lea     eax, [esi - 1]
		pop     esi
		pop     ebx
		ret

		align   16
	L7:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef control
	}
}
#endif
