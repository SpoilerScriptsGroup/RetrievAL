#include <windows.h>

#ifndef _M_IX86
int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	wchar_t c1, c2;

	for (; ; )
	{
		if (!count--)
			return 0;
		c1 = *(string1++);
		c2 = *(string2++);
		if (!(c1 -= c2))
			if (c2)
				continue;
			else
				return 0;
		if (c1 == (wchar_t)(L'A' - L'a'))
		{
			if ((short)c2 >= (short)L'a' && c2 <= L'z')
				continue;
		}
		else if (c1 == (wchar_t)(L'a' - L'A'))
		{
			if ((short)c2 >= (short)L'A' && c2 <= L'Z')
				continue;
		}
		c1 += c2;
		return (int)c1 - (int)c2;
	}
}
#else
__declspec(naked) int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		mov     edx, dword ptr [count + 8]
		mov     ebx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		inc     edx

		align   16
	L1:
		dec     edx
		jz      L2
		mov     ax, word ptr [ebx]
		mov     cx, word ptr [esi]
		add     ebx, 2
		add     esi, 2
		sub     ax, cx
		jnz     L3
		test    cx, cx
		jnz     L1
	L2:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	L3:
		cmp     ax, 'A' - 'a'
		je      L4
		cmp     ax, 'a' - 'A'
		jne     L5
		cmp     cx, 'A'
		jl      L5
		cmp     cx, 'Z'
		jbe     L1
		jmp     L5

		align   16
	L4:
		cmp     cx, 'a'
		jl      L5
		cmp     cx, 'z'
		jbe     L1
	L5:
		add     ax, cx
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
