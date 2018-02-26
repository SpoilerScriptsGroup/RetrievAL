#include <windows.h>

#ifndef _M_IX86
int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	unsigned char c1, c2;

	for (; ; )
	{
		BOOL isLead;

		if (!count--)
			goto SUCCESS;
		isLead = IsDBCSLeadByteEx(CP_THREAD_ACP, *string1);
		c1 = *(string1++);
		c2 = *(string2++);
		if (!isLead)
		{
			if (!(c1 -= c2))
			{
				if (!c2)
					goto SUCCESS;
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
		else
		{
			if (c1 != c2)
				break;
			if (!count--)
				goto SUCCESS;
			c1 = *(string1++);
			c2 = *(string2++);
			if (c1 != c2)
				break;
			if (!c1)
				goto SUCCESS;
		}
	}
	return (int)c1 - (int)c2;
SUCCESS:
	return 0;
}
#else
__declspec(naked) int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	__asm
	{
		push    ebx
		push    esi
		mov     ebx, dword ptr [esp + 12]
		mov     esi, dword ptr [esp + 16]
		push    edi
		dec     ebx
		mov     edi, dword ptr [esp + 24]
		dec     esi
		inc     edi

		align   16
	L1:
		dec     edi
		jz      L8
		mov     al, byte ptr [ebx + 1]
		inc     ebx
		inc     esi
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		mov     al, byte ptr [ebx]
		mov     cl, byte ptr [esi]
		jnz     L5
		sub     cl, al
		jnz     L2
		test    al, al
		jnz     L1
		jmp     L7

		align   16
	L2:
		cmp     cl, 'a' - 'A'
		je      L3
		cmp     cl, 'A' - 'a'
		jne     L4
		cmp     al, 'a'
		jl      L4
		cmp     al, 'z'
		jbe     L1
		jmp     L4

		align   16
	L3:
		cmp     al, 'A'
		jl      L4
		cmp     al, 'Z'
		jbe     L1
	L4:
		add     cl, al
		jmp     L6

		align   16
	L5:
		cmp     al, cl
		jne     L6
		dec     edi
		jz      L8
		inc     ebx
		inc     esi
		mov     al, byte ptr [ebx]
		mov     cl, byte ptr [esi]
		cmp     al, cl
		jne     L6
		and     eax, 0FFH
		jnz     L1
		jmp     L7

		align   16
	L6:
		and     eax, 0FFH
		and     ecx, 0FFH
		sub     eax, ecx
	L7:
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	L8:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret
	}
}
#endif
