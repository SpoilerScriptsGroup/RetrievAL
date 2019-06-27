#include <windows.h>

#ifndef _M_IX86
size_t __cdecl _mbsspn(const unsigned char *string, const unsigned char *control)
{
	const unsigned char *p1, *p2;
	unsigned char       c1, c2, trail;

	p1 = string - 1;
	while (c1 = *(++p1))
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c1))
			for (p2 = control; (c2 = *p2++) != c1; )
				if (!c2 || IsDBCSLeadByteEx(CP_THREAD_ACP, c2) && !(*p2++))
					goto DONE;
		else {
			if (!(trail = p1[1]))
				break;
			for (p2 = control; ; )
				if ((c2 = *p2++) != c1) {
					if (!c2 || IsDBCSLeadByteEx(CP_THREAD_ACP, c2) && !(*p2++))
						goto DONE;
				} else if ((c2 = *p2++) == trail)
					break;
				else if (!c2)
					goto DONE;
			p1++;
		}
DONE:
	return p1 - string;
}
#else
__declspec(naked) size_t __cdecl _mbsspn(const unsigned char *string, const unsigned char *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     esi, dword ptr [string + 16]
		mov     ebp, dword ptr [control + 16]
		dec     esi
		xor     eax, eax

		align   16
	L1:
		mov     al, byte ptr [esi + 1]
		inc     esi
		test    al, al
		jz      L8
		mov     ebx, eax
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jnz     L3
		mov     edi, ebp

		align   16
	L2:
		mov     al, byte ptr [edi]
		inc     edi
		cmp     al, bl
		je      L1
		test    al, al
		jz      L8
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     cl, byte ptr [edi]
		xor     eax, eax
		test    cl, cl
		jz      L8
		inc     edi
		jmp     L2

		align   16
	L3:
		mov     bh, byte ptr [esi + 1]
		xor     eax, eax
		test    bh, bh
		jz      L8
		mov     edi, ebp

		align   16
	L4:
		mov     al, byte ptr [edi]
		inc     edi
		cmp     al, bl
		je      L5
		test    al, al
		jz      L8
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L4
		inc     edi
		xor     eax, eax
		mov     al, byte ptr [edi - 1]
		jmp     L6

		align   16
	L5:
		mov     al, byte ptr [edi]
		inc     edi
		cmp     al, bh
		je      L7
	L6:
		test    al, al
		jnz     L4
		jmp     L8

		align   16
	L7:
		inc     esi
		jmp     L1

		align   16
	L8:
		mov     eax, esi
		mov     ecx, dword ptr [string + 16]
		sub     eax, ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string
		#undef control
	}
}
#endif
