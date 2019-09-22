#include "PageSize.h"

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _stricmp(const char *string1, const char *string2)
{
	for (; ; )
	{
		unsigned int i1, i2;

		i1 = (unsigned char)*(string1++);
		i2 = (unsigned char)*(string2++);
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
		sub     esi, eax
		xor     edx, edx
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [eax + esi]
		mov     dl, byte ptr [eax]
		sub     ecx, edx
		jnz     compare_insensitive
		test    edx, edx
		jz      return_equal
	byte_loop_increment:
		inc     eax
	byte_loop_entry:
		test    eax, 3                                  // use only eax for 'test reg, imm'
		jnz     byte_loop
		lea     ebp, [eax + esi]
		and     ebp, PAGE_SIZE - 1

		align   16
	dword_loop:
		cmp     ebp, PAGE_SIZE - 4
		ja      byte_loop                               // cross pages
		mov     ebx, dword ptr [eax + esi]
		mov     ebp, dword ptr [eax]
		cmp     ebx, ebp
		jne     byte_loop                               // not equal
		add     eax, 4
		lea     ecx, [ebx - 01010101H]
		xor     ebx, -1
		lea     ebp, [eax + esi]
		and     ecx, 80808080H
		and     ebp, PAGE_SIZE - 1
		and     ecx, ebx
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
		jmp     primary_to_lower

		align   16
	compare_borrow:
		cmp     ecx, 'A' - 'a'
		jne     return_not_equal
		xor     ecx, ecx
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     byte_loop_increment
		sub     edx, 'A'
		mov     eax, ebx
		jmp     secondary_to_lower

		align   16
	return_not_equal:
		lea     eax, [ecx + edx - 'A']
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
