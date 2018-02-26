#include <windows.h>

#ifndef _M_IX86
int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	for (; ; )
	{
		wchar_t c1, c2;

		c1 = *(string1++);
		c2 = *(string2++);
		if (!(c1 -= c2))
		{
			if (!c2)
				break;
		}
		else
		{
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
	return 0;
}
#else
__declspec(naked) int __cdecl _wcsicmp(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		push    ebx
		xor     eax, eax
		mov     ebx, dword ptr [esp +  8]
		mov     edx, dword ptr [esp + 12]
		xor     ecx, ecx
		jmp     L2

		align   16
	L1:
		add     ebx, 2
		add     edx, 2
	L2:
		mov     ax, word ptr [ebx]
		mov     cx, word ptr [edx]
		sub     ax, cx
		jnz     L3
		test    cx, cx
		jnz     L1
		jmp     L6

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
		sub     eax, ecx
	L6:
		pop     ebx
		ret
	}
}
#endif
