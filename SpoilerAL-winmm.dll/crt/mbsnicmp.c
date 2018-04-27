#include <windows.h>

#ifndef _M_IX86
int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	unsigned char c1, c2;

	string1 += count;
	string2 += count;
	count = ~count;
	for (; ; )
	{
		if (!++count)
			return 0;
		c1 = string1[count];
		c2 = string2[count];
		if (!(c1 -= c2))
		{
			if (!c2)
				return 0;
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
				continue;
			if (!++count)
				return 0;
			c1 = string1[count];
			c2 = string2[count];
			if (c1 != c2)
				break;
			if (!c1)
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
__declspec(naked) int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
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
		mov     esi, dword ptr [string1 + 16]
		mov     edi, dword ptr [string2 + 16]
		mov     ebp, dword ptr [count + 16]
		xor     eax, eax
		add     edi, ebp
		add     esi, ebp
		xor     ebp, -1

		align   16
	L1:
		inc     ebp
		jz      L2
		mov     bl, byte ptr [esi + ebp]
		mov     al, byte ptr [edi + ebp]
		sub     bl, al
		jnz     L4
		and     eax, 0FFH
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		inc     ebp
		jz      L2
		mov     bl, byte ptr [esi + ebp]
		mov     al, byte ptr [edi + ebp]
		cmp     bl, al
		jne     L7
		test    al, al
		jnz     L1
	L2:
		mov     eax, 0
	L3:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	L4:
		cmp     bl, 'a' - 'A'
		je      L5
		cmp     bl, 'A' - 'a'
		jne     L6
		cmp     al, 'a'
		jl      L6
		cmp     al, 'z'
		jbe     L1
		jmp     L6

		align   16
	L5:
		cmp     al, 'A'
		jl      L6
		cmp     al, 'Z'
		jbe     L1
	L6:
		add     bl, al
		mov     eax, esp
	L7:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
