#include <windows.h>

#ifndef _M_IX86
int __cdecl _mbsicmp(const unsigned char *string1, const unsigned char *string2)
{
	unsigned char c1, c2;

	for (; ; )
	{
		c1 = *(string1++);
		c2 = *(string2++);
		if (!(c1 -= c2))
		{
			if (!c2)
				return 0;
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
				continue;
			c1 = *(string1++);
			c2 = *(string2++);
			if (c1 != c2)
				break;
			if (!c2)
				return 0;
		}
		else
		{
			if (c1 == (unsigned char)('A' - 'a'))
			{
				if ((char)c2 >= 'a' && c2 <= (unsigned char)'z')
					continue;
			}
			else if (c1 == (unsigned char)('a' - 'A'))
			{
				if ((char)c2 >= 'A' && c2 <= (unsigned char)'Z')
					continue;
			}
			c1 += c2;
			break;
		}
	}
	return (int)c1 - (int)c2;
}
#else
__declspec(naked) int __cdecl _mbsicmp(const unsigned char *string1, const unsigned char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		sub     esi, ebx

		align   16
	L1:
		mov     cl, byte ptr [ebx]
		xor     eax, eax
		mov     al, byte ptr [ebx + esi]
		inc     ebx
		sub     cl, al
		jnz     L3
		test    eax, eax
		jz      L2
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [ebx]
		xor     eax, eax
		mov     al, byte ptr [ebx + esi]
		inc     ebx
		cmp     cl, al
		jne     L6
		test    eax, eax
		jnz     L1
	L2:
		pop     esi
		pop     ebx
		ret

		align   16
	L3:
		cmp     cl, 'a' - 'A'
		je      L4
		cmp     cl, 'A' - 'a'
		jne     L5
		cmp     al, 'a'
		jl      L5
		cmp     al, 'z'
		jbe     L1
		jmp     L5

		align   16
	L4:
		cmp     al, 'A'
		jl      L5
		cmp     al, 'Z'
		jbe     L1
	L5:
		add     cl, al
	L6:
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
