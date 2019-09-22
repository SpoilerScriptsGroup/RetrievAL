#include <wchar.h>

#ifndef _M_IX86
int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	string1 += count;
	string2 += count;
	count = ~count;
	while (++count)
	{
		unsigned int i1, i2;

		i1 = (unsigned short)string1[count];
		i2 = (unsigned short)string2[count];
		if (!(i1 -= i2))
			if (i2)
				continue;
			else
				break;
		if (i1 == 'A' - 'a')
		{
			if ((i2 - 'a') <= 'z' - 'a')
				continue;
		}
		else if (i1 == 'a' - 'A')
		{
			if ((i2 - 'A') <= 'Z' - 'A')
				continue;
		}
		i1 += (i2 -= 'A');
		if (i1 <= 'Z' - 'A')
			i1 += 'a' - 'A';
		if (i2 <= 'Z' - 'A')
			i2 += 'a' - 'A';
		return i1 - i2;
	}
	return 0;
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
		push    edi
		xor     eax, eax
		mov     esi, dword ptr [string1 + 12]
		mov     edi, dword ptr [string2 + 12]
		mov     ecx, dword ptr [count + 12]
		xor     edx, edx
		lea     esi, [esi + ecx * 2]
		lea     edi, [edi + ecx * 2]
		xor     ecx, -1

		align   16
	loop_begin:
		inc     ecx
		jz      return_equal
		mov     ax, word ptr [esi + ecx * 2]
		mov     dx, word ptr [edi + ecx * 2]
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_begin
	return_equal:
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		jne     compare_borrow
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     loop_begin
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	compare_borrow:
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     loop_begin
		sub     edx, 'A'
		mov     eax, ebx
		jmp     secondary_to_lower

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
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
