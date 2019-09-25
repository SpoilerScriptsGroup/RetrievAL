#include <stddef.h>
#include <ctype.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count)
{
	int ret;

	(char *)buffer1 += count;
	(char *)buffer2 += count;
	count ^= -1;
	ret = 0;
	while (++count)
		if (ret = tolower(((char *)buffer1)[count]) - tolower(((char *)buffer2)[count]))
			break;
	return ret;
}
#else
__declspec(naked) int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     esi, dword ptr [buffer1 + 16]           // esi = buffer1
		mov     edi, dword ptr [buffer2 + 16]           // edi = buffer2
		mov     ecx, dword ptr [count + 16]             // ecx = count
		xor     eax, eax
		add     esi, ecx                                // esi = end of buffer1
		add     edi, ecx                                // edi = end of buffer2
		xor     ecx, -1                                 // ecx = -count - 1
		xor     edx, edx
		jmp     byte_loop_increment

		align   16
	byte_loop:
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
		sub     eax, edx
		jnz     compare_insensitive
	byte_loop_increment:
		inc     ecx
		jz      return_equal
		lea     ebx, [edi + ecx]
		lea     ebp, [esi + ecx]
		and     ebx, 3
		jnz     byte_loop

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
		lea     ebp, [esi + ecx]
		jmp     dword_loop

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
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
	compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     byte_loop_increment
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_to_lower

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
		sub     eax, edx
	return_equal:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef buffer1
		#undef buffer2
	}
}
#endif
