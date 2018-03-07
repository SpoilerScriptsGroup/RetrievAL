#include <stddef.h>

#ifndef _M_IX86
int __cdecl strncmp(const char *string1, const char *string2, size_t count)
{
	if (count)
	{
		unsigned char c1, c2;

		do
			if ((c1 = *(string1++)) != (c2 = *(string2++)))
				return (int)c1 - (int)c2;
		while (c1 && --count);
	}
	return 0;
}
#else
__declspec(naked) int __cdecl strncmp(const char *string1, const char *string2, size_t count)
{
	#define PAGE_SIZE 4096

	__asm
	{
		push    ebx
		push    esi
		mov     ebx, dword ptr [esp + 12]
		mov     esi, dword ptr [esp + 16]
		mov     ecx, dword ptr [esp + 20]
		sub     ebx, esi
		test    ecx, -1 ; alignment for L1
		jz      L3
		test    esi, 3
		jz      L2
	L1:
		mov     al, byte ptr [esi + ebx]
		mov     dl, byte ptr [esi]
		cmp     al, dl
		jne     L4
		test    al, al
		jz      L3
		sub     ecx, 1
		jbe     L3
		inc     esi
		test    esi, 3
		jnz     L1
	L2:
		lea     eax, [esi + ebx]
		and     eax, PAGE_SIZE - 1
		cmp     eax, PAGE_SIZE - 4
		ja      L1
		mov     eax, dword ptr [esi + ebx]
		mov     edx, dword ptr [esi]
		cmp     eax, edx
		jne     L1
		sub     ecx, 4
		jbe     L3
		lea     edx, [eax - 01010101H]
		add     esi, 4
		xor     eax, edx
		test    eax, 80808080H
		jz      L2
	L3:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	L4:
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret
	}

	#undef PAGE_SIZE
}
#endif
