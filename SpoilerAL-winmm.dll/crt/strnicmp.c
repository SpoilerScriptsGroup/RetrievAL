#include <stddef.h>
#include <ctype.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	int ret, c;

	string1 += count;
	string2 += count;
	count ^= -1;
	ret = 0;
	while (++count)
		if (ret = tolower(string1[count]) - (c = tolower(string2[count])))
			break;
		else if (!c)
			break;
	return ret;
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
		mov     ebp, dword ptr [count + 16]             // ebp = count
		xor     eax, eax
		add     esi, ebp                                // esi = end of string1
		add     edi, ebp                                // edi = end of string2
		xor     ebp, -1                                 // ebp = -count - 1
		xor     edx, edx
		jmp     byte_loop_increment

		align   16
	byte_loop:
		mov     al, byte ptr [esi + ebp]
		mov     dl, byte ptr [edi + ebp]
		sub     eax, edx
		jnz     byte_compare_insensitive
		test    edx, edx
		jz      return_equal
	byte_loop_increment:
		inc     ebp
		jz      return_equal
		lea     ebx, [edi + ebp]
		lea     ecx, [esi + ebp]
		and     ebx, 3
		jnz     byte_loop
		shl     ecx, 32 - BSF_PAGE_SIZE
		jmp     dword_loop

		align   16
	byte_compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      byte_compare_above
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
	byte_compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     byte_loop_increment
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	dword_loop:
		cmp     ecx, (PAGE_SIZE - 4) shl (32 - BSF_PAGE_SIZE)
		ja      byte_loop                               // cross pages
		mov     ecx, dword ptr [esi + ebp]
		mov     ebx, dword ptr [edi + ebp]
		sub     ecx, ebx
		jnz     dword_compare_insensitive
		add     ebp, 4
		jc      return_equal
		mov     edx, ebx
		lea     ecx, [esi + ebp]
		sub     ebx, 01010101H
		xor     edx, -1
		shl     ecx, 32 - BSF_PAGE_SIZE
		and     ebx, 80808080H
		and     edx, ebx
		jz      dword_loop
	return_equal:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	dword_compare_insensitive:
		test    cl, cl
		jz      dword_compare_byte_1
		cmp     cl, 'A' - 'a'
		je      dword_compare_borrow_byte_0
		cmp     cl, 'a' - 'A'
		jne     dword_unmatch_byte_0
		sub     ebx, 'A'
		cmp     bl, 'Z' - 'A'
		ja      dword_unmatch_above_byte_even
		add     ebx, 'A'
		jmp     dword_compare_byte_1

	dword_compare_borrow_byte_0:
		sub     ebx, 'a'
		cmp     bl, 'z' - 'a'
		ja      dword_unmatch_borrow_byte_even
		add     ecx, 0100H
		add     ebx, 'a'

	dword_compare_byte_1:
		test    ch, ch
		jnz     dword_compare_above_byte_1
		shr     ecx, 16
		shr     ebx, 16
		jmp     dword_compare_byte_2

	dword_compare_above_byte_1:
		cmp     ch, 'A' - 'a'
		je      dword_compare_borrow_byte_1
		cmp     ch, 'a' - 'A'
		jne     dword_unmatch_byte_1
		sub     ebx, 'A' << 8
		cmp     bh, 'Z' - 'A'
		ja      dword_unmatch_above_byte_odd
		shr     ecx, 16
		add     ebx, 'A' << 8
		shr     ebx, 16
		jmp     dword_compare_byte_2

	dword_compare_borrow_byte_1:
		sub     ebx, 'a' << 8
		cmp     bh, 'z' - 'a'
		ja      dword_unmatch_borrow_byte_odd
		shr     ecx, 16
		add     ebx, 'a' << 8
		shr     ebx, 16
		inc     ecx

	dword_compare_byte_2:
		test    cl, cl
		jz      dword_compare_byte_3
		cmp     cl, 'A' - 'a'
		je      dword_compare_borrow_byte_2
		cmp     cl, 'a' - 'A'
		jne     dword_unmatch_byte_2
		sub     ebx, 'A'
		cmp     bl, 'Z' - 'A'
		ja      dword_unmatch_above_byte_even
		add     ebx, 'A'
		jmp     dword_compare_byte_3

	dword_compare_borrow_byte_2:
		sub     ebx, 'a'
		cmp     bl, 'z' - 'a'
		ja      dword_unmatch_borrow_byte_even
		add     ecx, 0100H
		add     ebx, 'a'

	dword_compare_byte_3:
		test    ch, ch
		jz      dword_loop_increment
		cmp     ch, 'A' - 'a'
		je      dword_compare_borrow_byte_3
		cmp     ch, 'a' - 'A'
		jne     dword_unmatch_byte_3
		sub     ebx, 'A' << 8
		cmp     bh, 'Z' - 'A'
		jbe     dword_loop_increment
		jmp     dword_unmatch_above_byte_odd

	dword_compare_borrow_byte_3:
		sub     ebx, 'a' << 8
		cmp     bh, 'z' - 'a'
		ja      dword_unmatch_borrow_byte_odd
	dword_loop_increment:
		add     ebp, 4
		jc      return_equal
		lea     ecx, [esi + ebp]
		shl     ecx, 32 - BSF_PAGE_SIZE
		jmp     dword_loop

	dword_unmatch_above_byte_odd:
		shr     ebx, 8
	dword_unmatch_above_byte_even:
		lea     eax, [ebx + 'a']
		lea     edx, [ebx + 'A']
		and     eax, 0FFH
		and     edx, 0FFH
		sub     eax, 'A'
		sub     edx, 'A'
		jmp     primary_to_lower

	dword_unmatch_byte_2:
		add     ebp, 2
		jc      return_equal
	dword_unmatch_byte_0:
		mov     edx, ebx
		lea     eax, [ecx + ebx]
		jmp     dword_unmatch

	dword_unmatch_byte_1:
		inc     ebp
		jnz     dword_unmatch_byte_odd
		jmp     return_equal

	dword_unmatch_byte_3:
		add     ebp, 3
		jc      return_equal
	dword_unmatch_byte_odd:
		mov     edx, ebx
		lea     eax, [ecx + ebx]
		shr     eax, 8
		shr     edx, 8
		jmp     dword_unmatch

	dword_unmatch_borrow_byte_odd:
		shr     ebx, 8
	dword_unmatch_borrow_byte_even:
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
		pop     esi
		pop     ebp
		pop     ebx
		sub     eax, edx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
