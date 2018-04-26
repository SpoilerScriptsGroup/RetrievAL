#include <windows.h>

#ifndef _M_IX86
int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	unsigned char c1, c2;

	for (; ; )
	{
		BOOL isLead;

		if (!count--)
			return 0;
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
			if (!count--)
				return 0;
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
__declspec(naked) int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		mov     ebx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		push    edi
		dec     ebx
		mov     edi, dword ptr [count + 12]
		dec     esi
		inc     edi

		align   16
	L1:
		dec     edi
		jz      L5
		inc     ebx
		xor     eax, eax
		mov     al, byte ptr [ebx]
		inc     esi
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		mov     cl, byte ptr [ebx]
		mov     dl, byte ptr [esi]
		test    eax, eax
		jnz     L4
		sub     cl, dl
		jnz     L2
		test    dl, dl
		jnz     L1
		jmp     L6

		align   16
	L2:
		cmp     cl, 'a' - 'A'
		je      L3
		cmp     cl, 'A' - 'a'
		jne     L7
		cmp     dl, 'a'
		jl      L7
		cmp     dl, 'z'
		jbe     L1
		jmp     L7

		align   16
	L3:
		cmp     dl, 'A'
		jl      L7
		cmp     dl, 'Z'
		jbe     L1
		jmp     L7

		align   16
	L4:
		cmp     cl, dl
		jne     L8
		dec     edi
		jz      L5
		inc     ebx
		inc     esi
		mov     cl, byte ptr [ebx]
		mov     dl, byte ptr [esi]
		cmp     cl, dl
		jne     L8
		test    cl, cl
		jnz     L1
	L5:
		xor     eax, eax
	L6:
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	L7:
		add     cl, dl
	L8:
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
