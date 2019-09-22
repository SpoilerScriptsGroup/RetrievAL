#include <wchar.h>

#ifndef _M_IX86
int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	for (; ; )
	{
		wchar_t c1, c2;

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
__declspec(naked) int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		xor     eax, eax
		mov     ecx, dword ptr [string1 + 4]
		mov     ebx, dword ptr [string2 + 4]
		sub     ebx, ecx

		align   16
	loop_head:
		xor     edx, edx
		mov     ax, word ptr [ecx]
		mov     dx, word ptr [ecx + ebx]
		add     ecx, 2
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_head
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		jne     compare_borrow
		sub     edx, 'A'
		xor     eax, eax
		cmp     edx, 'Z' - 'A'
		jbe     loop_head
		lea     eax, [edx + 'a' - 'A']
		jmp     primary_tolower

		align   16
	compare_borrow:
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		sub     edx, 'a'
		xor     eax, eax
		cmp     edx, 'z' - 'a'
		jbe     loop_head
		mov     eax, edx
		add     edx, 'a' - 'A'
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
		sub     eax, edx
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
