#include <stddef.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl strncmp(const char *string1, const char *string2, size_t count)
{
	unsigned char  c1, c2;

	string1 += count;
	string2 += count;
	count = ~count;
	do
		if (!++count)
			break;
		else if ((c1 = string1[count]) != (c2 = string2[count]))
			return (int)c1 - (int)c2;
	while (c1);
	return 0;
}
#else
__declspec(naked) int __cdecl strncmp(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 12]           // esi = string1
		mov     edi, dword ptr [string2 + 12]           // edi = string2
		mov     eax, dword ptr [count + 12]             // eax = count
		add     esi, eax                                // esi = end of string1
		add     edi, eax                                // edi = end of string2
		xor     eax, -1                                 // eax = -count - 1
		jmp     byte_loop_increment

		align   16
	byte_loop:
		mov     cl, byte ptr [esi + eax]
		mov     dl, byte ptr [edi + eax]
		cmp     cl, dl
		jne     return_not_equal
		test    cl, cl
		jz      return_equal
	byte_loop_increment:
		inc     eax
		jz      return_equal
		lea     ecx, [edi + eax]
		and     ecx, 3
		jnz     byte_loop
		lea     ebx, [esi + eax]
		and     ebx, PAGE_SIZE - 1

		align   16
	dword_loop:
		cmp     ebx, PAGE_SIZE - 4
		ja      byte_loop                               // cross pages
		mov     ecx, dword ptr [esi + eax]
		mov     edx, dword ptr [edi + eax]
		cmp     ecx, edx
		jne     byte_loop                               // not equal
		add     eax, 4
		jc      return_equal
		mov     edx, ecx
		xor     ecx, -1
		sub     edx, 01010101H
		lea     ebx, [esi + eax]
		and     edx, 80808080H
		and     ebx, PAGE_SIZE - 1
		and     edx, ecx
		jz      dword_loop

	return_equal:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
