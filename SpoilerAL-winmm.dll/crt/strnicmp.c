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
		unsigned int i1, i2;

		i1 = (unsigned char)string1[count];
		i2 = (unsigned char)string2[count];
		if (!(i1 -= i2))
			if (i2)
				continue;
			else
				break;
		if (i1 == 'A' - 'a')
		{
			if ((i2 - 'a') <= 'z' - 'a')
				continue;
		}
		else if (i1 == 'a' - 'A')
		{
			if ((i2 - 'A') <= 'Z' - 'A')
				continue;
		}
		i1 += (i2 -= 'A');
		if (i1 <= 'Z' - 'A')
			i1 += 'a' - 'A';
		if (i2 <= 'Z' - 'A')
			i2 += 'a' - 'A';
		return i1 - i2;
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
		and     ebp, PAGE_SIZE - 1

		align   16
	dword_loop:
		cmp     ebp, PAGE_SIZE - 4
		ja      byte_loop                               // cross pages
		mov     ebx, dword ptr [esi + ecx]
		mov     ebp, dword ptr [edi + ecx]
		cmp     ebx, ebp
		jne     byte_loop                               // not equal
		add     ecx, 4
		jc      return_equal
		mov     eax, ebx
		xor     ebx, -1
		sub     eax, 01010101H
		lea     ebp, [esi + ecx]
		and     eax, 80808080H
		and     ebp, PAGE_SIZE - 1
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
		jmp     primary_to_lower

		align   16
	compare_borrow:
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     byte_loop_increment
		sub     edx, 'A'
		mov     eax, ebx
		jmp     secondary_to_lower

		align   16
	return_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      primary_to_lower
		add     edx, 'a' - 'A'
	primary_to_lower:
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
