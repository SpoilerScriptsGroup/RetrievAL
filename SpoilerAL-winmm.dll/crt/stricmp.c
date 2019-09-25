#include <ctype.h>
#include "PageSize.h"

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _stricmp(const char *string1, const char *string2)
{
	int ret, c;

	do
		if (ret = tolower(*(string1++)) - (c = tolower(*(string2++))))
			break;
	while (c);
	return ret;
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
		push    edi
		mov     esi, dword ptr [string1 + 16]           // esi = string1
		mov     edi, dword ptr [string2 + 16]           // edi = string2
		sub     esi, edi
		xor     ecx, ecx
		xor     edx, edx
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     cl, byte ptr [edi + esi]
		mov     dl, byte ptr [edi]
		sub     ecx, edx
		jnz     byte_compare_insensitive
		test    edx, edx
		jz      return_equal
	byte_loop_increment:
		lea     eax, [edi + 1]
		inc     edi
	byte_loop_entry:
		test    eax, 3                                  // use only eax for 'test reg, imm'
		jnz     byte_loop
		add     eax, esi
		and     eax, PAGE_SIZE - 1
		jmp     dword_loop

		align   16
	byte_compare_insensitive:
		cmp     ecx, 'a' - 'A'
		je      byte_compare_above
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
	byte_compare_above:
		xor     ecx, ecx
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     byte_loop_increment
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	dword_loop:
		cmp     eax, PAGE_SIZE - 4
		ja      byte_loop                               // cross pages
		mov     eax, dword ptr [edi + esi]
		mov     ebx, dword ptr [edi]
		sub     eax, ebx
		jnz     dword_compare_insensitive
		add     edi, 4
		lea     ecx, [ebx - 01010101H]
		xor     ebx, -1
		lea     eax, [edi + esi]
		and     ecx, 80808080H
		and     eax, PAGE_SIZE - 1
		and     ecx, ebx
		jz      dword_loop
	return_equal:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	dword_compare_insensitive:
		test    al, al
		jz      dword_compare_byte_1
		cmp     al, 'A' - 'a'
		je      dword_compare_byte_0_borrow
		cmp     al, 'a' - 'A'
		jne     dword_unmatch_byte_even
		sub     ebx, 'A'
		cmp     bl, 'Z' - 'A'
		ja      dword_unmatch_byte_even_above
		add     ebx, 'A'
		jmp     dword_compare_byte_1

	dword_compare_byte_0_borrow:
		sub     ebx, 'a'
		cmp     bl, 'z' - 'a'
		ja      dword_unmatch_byte_even_borrow
		add     eax, 0100H
		add     ebx, 'a'

	dword_compare_byte_1:
		test    ah, ah
		jnz     dword_compare_byte_1_above
		shr     eax, 16
		shr     ebx, 16
		jmp     dword_compare_byte_2

	dword_compare_byte_1_above:
		cmp     ah, 'A' - 'a'
		je      dword_compare_byte_1_borrow
		cmp     ah, 'a' - 'A'
		jne     dword_unmatch_byte_odd
		sub     ebx, 'A' shl 8
		cmp     bh, 'Z' - 'A'
		ja      dword_unmatch_byte_odd_above
		shr     eax, 16
		add     ebx, 'A' shl 8
		shr     ebx, 16
		jmp     dword_compare_byte_2

	dword_compare_byte_1_borrow:
		sub     ebx, 'a' shl 8
		cmp     bh, 'z' - 'a'
		ja      dword_unmatch_byte_odd_borrow
		shr     eax, 16
		add     ebx, 'a' shl 8
		shr     ebx, 16
		inc     eax

	dword_compare_byte_2:
		test    al, al
		jz      dword_compare_byte_3
		cmp     al, 'A' - 'a'
		je      dword_compare_byte_2_borrow
		cmp     al, 'a' - 'A'
		jne     dword_unmatch_byte_even
		sub     ebx, 'A'
		cmp     bl, 'Z' - 'A'
		ja      dword_unmatch_byte_even_above
		add     ebx, 'A'
		jmp     dword_compare_byte_3

	dword_compare_byte_2_borrow:
		sub     ebx, 'a'
		cmp     bl, 'z' - 'a'
		ja      dword_unmatch_byte_even_borrow
		add     eax, 0100H
		add     ebx, 'a'

	dword_compare_byte_3:
		test    ah, ah
		jz      dword_loop_increment
		cmp     ah, 'A' - 'a'
		je      dword_compare_byte_3_borrow
		cmp     ah, 'a' - 'A'
		jne     dword_unmatch_byte_odd
		sub     ebx, 'A' shl 8
		cmp     bh, 'Z' - 'A'
		jbe     dword_loop_increment
		jmp     dword_unmatch_byte_odd_above

	dword_compare_byte_3_borrow:
		sub     ebx, 'a' shl 8
		cmp     bh, 'z' - 'a'
		ja      dword_unmatch_byte_odd_borrow

	dword_loop_increment:
		lea     eax, [edi + esi + 4]
		add     edi, 4
		and     eax, PAGE_SIZE - 1
		jmp     dword_loop

	dword_unmatch_byte_odd_above:
		shr     ebx, 8
	dword_unmatch_byte_even_above:
		lea     eax, [ebx + 'a']
		lea     edx, [ebx + 'A']
		and     eax, 0FFH
		and     edx, 0FFH
		sub     eax, 'A'
		sub     edx, 'A'
		jmp     primary_to_lower

	dword_unmatch_byte_even:
		add     eax, ebx
		mov     edx, ebx
		jmp     dword_unmatch

	dword_unmatch_byte_odd:
		add     eax, ebx
		mov     edx, ebx
		shr     eax, 8
		shr     edx, 8
		jmp     dword_unmatch

	dword_unmatch_byte_odd_borrow:
		shr     ebx, 8
	dword_unmatch_byte_even_borrow:
		lea     eax, [ebx + 'A']
		lea     edx, [ebx + 'a']
	dword_unmatch:
		and     eax, 0FFH
		and     edx, 0FFH
		sub     eax, 'A'
		sub     edx, 'A'
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
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
