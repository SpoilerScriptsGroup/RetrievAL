#include <stddef.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	string1 += count;
	string2 += count;
	count = ~count;
	while (++count)
	{
		unsigned char c1, c2;

		c1 = string1[count];
		c2 = string2[count];
		if (!(c1 -= c2))
			if (c2)
				continue;
			else
				break;
		if (c1 == 'A' - 'a')
		{
			if ((c2 - 'a') <= 'z' - 'a')
				continue;
		}
		else if (c1 == 'a' - 'A')
		{
			if ((c2 - 'a') <= 'Z' - 'A')
				continue;
		}
		c1 += c2;
		return (int)c1 - (int)c2;
	}
	return 0;
}
#else
__declspec(naked) int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 16]           // esi = string1
		mov     edi, dword ptr [string2 + 16]           // edi = string2
		mov     ecx, dword ptr [count + 16]             // ecx = count
		xor     eax, eax
		add     esi, ecx                                // esi = end of string1
		add     edi, ecx                                // edi = end of string2
		xor     ecx, -1                                 // ecx = -count - 1
		xor     edx, edx
		jmp     byte_loop_increment

		align   16
	byte_loop:
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jz      return_equal
	byte_loop_increment:
		inc     ecx
		jz      return_equal
		lea     ebx, [edi + ecx]
		and     ebx, 3
		jnz     byte_loop
		lea     ebp, [esi + ecx]

		align   16
	dword_loop:
		and     ebp, PAGE_SIZE - 1
		cmp     ebp, PAGE_SIZE - 4
		ja      byte_loop                               // cross pages
		mov     ebx, dword ptr [esi + ecx]
		mov     ebp, dword ptr [edi + ecx]
		cmp     ebx, ebp
		jne     byte_loop                               // not equal
		add     ecx, 4
		jc      return_equal
		lea     eax, [ebx - 01010101H]
		xor     ebx, -1
		and     eax, 80808080H
		lea     ebp, [esi + ecx]
		and     eax, ebx
		jz      dword_loop

	return_equal:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		jne     compare_borrow
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     byte_loop_increment
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_tolower

		align   16
	compare_borrow:
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     byte_loop_increment
		mov     eax, ebx
		lea     edx, [ebx + 'a' - 'A']
		jmp     secondary_tolower

		align   16
	return_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
	secondary_tolower:
		cmp     edx, 'Z' - 'A'
		ja      primary_tolower
		add     edx, 'a' - 'A'
	primary_tolower:
		cmp     eax, 'Z' - 'A'
		ja      difference
		add     eax, 'a' - 'A'
	difference:
		pop     edi
		sub     eax, edx
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
