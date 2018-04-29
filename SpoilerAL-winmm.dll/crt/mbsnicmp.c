#include <windows.h>

int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	unsigned char c1, c2;

	for (; ; )
	{
		if (!count--)
			return 0;
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
__declspec(naked) int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		mov     ebx, dword ptr [count + 8]
		push    edi
		test    ebx, ebx
		jz      L2
		mov     esi, dword ptr [string1 + 12]
		mov     edi, dword ptr [string2 + 12]
		sub     edi, esi

		align   16
	L1:
		mov     cl, byte ptr [esi]
		xor     eax, eax
		mov     al, byte ptr [esi + edi]
		inc     esi
		sub     cl, al
		jnz     L4
		test    eax, eax
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [esi]
		xor     eax, eax
		mov     al, byte ptr [esi + edi]
		inc     esi
		cmp     cl, al
		jne     L7
		test    eax, eax
		jz      L3
		dec     ebx
		jnz     L1
	L2:
		xor     eax, eax
	L3:
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	L4:
		cmp     cl, 'a' - 'A'
		je      L5
		cmp     cl, 'A' - 'a'
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
		add     cl, al
	L7:
		pop     edi
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
