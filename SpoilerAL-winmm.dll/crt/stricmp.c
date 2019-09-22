#include "PageSize.h"

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _stricmp(const char *string1, const char *string2)
{
	for (; ; )
	{
		unsigned char c1, c2;

		c1 = *(string1++);
		c2 = *(string2++);
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
			if ((c2 - 'A') <= 'Z' - 'A')
				continue;
		}
		c1 += c2;
		return (int)c1 - (int)c2;
	}
	return 0;
}
#else
__declspec(naked) int __cdecl _stricmp(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		push    ebp
		push    esi
		xor     ecx, ecx
		mov     esi, dword ptr [string1 + 12]           // esi = string1
		mov     eax, dword ptr [string2 + 12]           // eax = string2
		xor     edx, edx
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [esi]
		mov     dl, byte ptr [eax]
		sub     ecx, edx
		jnz     compare_insensitive
		test    edx, edx
		jz      return_equal
	byte_loop_increment:
		inc     eax
		inc     esi
	byte_loop_entry:
		test    eax, 3                                  // use only eax for 'test reg, imm'
		jnz     byte_loop
		mov     ebx, esi

		align   16
	dword_loop:
		and     ebx, PAGE_SIZE - 1
		cmp     ebx, PAGE_SIZE - 4
		ja      byte_loop                               // cross pages
		mov     ebx, dword ptr [esi]
		mov     ebp, dword ptr [eax]
		cmp     ebx, ebp
		jne     byte_loop                               // not equal
		add     esi, 4
		lea     ecx, [ebx - 01010101H]
		xor     ebx, -1
		and     ecx, 80808080H
		add     eax, 4
		and     ecx, ebx
		mov     ebx, esi
		jz      dword_loop

	return_equal:
		xor     eax, eax
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     ecx, 'a' - 'A'
		jne     compare_borrow
		xor     ecx, ecx
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     byte_loop_increment
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_tolower

		align   16
	compare_borrow:
		cmp     ecx, 'A' - 'a'
		jne     return_not_equal
		xor     ecx, ecx
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     byte_loop_increment
		mov     eax, ebx
		lea     edx, [ebx + 'a' - 'A']
		jmp     secondary_tolower

		align   16
	return_not_equal:
		lea     eax, [ecx + edx - 'A']
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
