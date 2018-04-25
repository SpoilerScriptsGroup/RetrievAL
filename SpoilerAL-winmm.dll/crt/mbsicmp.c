#include <windows.h>

#ifndef _M_IX86
int __cdecl _mbsicmp(const unsigned char *string1, const unsigned char *string2)
{
	unsigned char c1, c2;

	for (; ; )
	{
		BOOL isLead;

		isLead = IsDBCSLeadByteEx(CP_THREAD_ACP, *string1);
		c1 = *(string1++);
		c2 = *(string2++);
		if (!isLead)
		{
			if (!(c1 -= c2))
				if (c2)
					continue;
				else
					return 0;
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
		else
		{
			if (c1 != c2)
				break;
			c1 = *(string1++);
			c2 = *(string2++);
			if (c1 != c2)
				break;
			if (!c1)
				return 0;
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
		dec     ebx
		dec     esi

		align   16
	L1:
		inc     ebx
		xor     eax, eax
		mov     al, byte ptr [ebx]
		inc     esi
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		mov     al, byte ptr [ebx]
		mov     cl, byte ptr [esi]
		jnz     L4
		sub     al, cl
		jnz     L2
		test    cl, cl
		jnz     L1
		jmp     L5

		align   16
	L2:
		cmp     al, 'a' - 'A'
		je      L3
		cmp     al, 'A' - 'a'
		jne     L6
		cmp     cl, 'a'
		jl      L6
		cmp     cl, 'z'
		jbe     L1
		jmp     L6

		align   16
	L3:
		cmp     cl, 'A'
		jl      L6
		cmp     cl, 'Z'
		jbe     L1
		jmp     L6

		align   16
	L4:
		cmp     al, cl
		jne     L7
		inc     ebx
		inc     esi
		mov     al, byte ptr [ebx]
		mov     cl, byte ptr [esi]
		cmp     al, cl
		jne     L7
		test    al, al
		jnz     L1
	L5:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	L6:
		add     al, cl
	L7:
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
