#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
int __cdecl _tcsncmp(const TCHAR *string1, const TCHAR *string2, size_t count)
{
#ifdef _UNICODE
	unsigned short c1, c2;
#else
	unsigned char  c1, c2;
#endif

	if (count)
		do
			if ((c1 = *(string1++)) != (c2 = *(string2++)))
				return (int)c1 - (int)c2;
		while (c1 && --count);
	return 0;
}
#elif defined(_UNICODE)
__declspec(naked) int __cdecl wcsncmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     eax, dword ptr [count]
		mov     edx, dword ptr [string1]
		test    eax, eax
		jz      L3
		mov     ecx, dword ptr [string2]
		push    ebx
		push    esi
		lea     edx, [edx + eax * 2]
		lea     esi, [ecx + eax * 2]
		dec     eax
		xor     eax, -1

		align   16
	L1:
		mov     cx, word ptr [edx + eax * 2]
		mov     bx, word ptr [esi + eax * 2]
		cmp     cx, bx
		jne     L4
		inc     eax
		jz      L2
		test    cx, cx
		jnz     L1
		xor     eax, eax
	L2:
		pop     esi
		pop     ebx
	L3:
		ret

		align   16
	L4:
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
#else
__declspec(naked) int __cdecl strncmp(const char *string1, const char *string2, size_t count)
{
	#define PAGE_SIZE 4096

	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		mov     edx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		mov     eax, dword ptr [count + 8]
		sub     edx, esi
		test    eax, -1                         // alignment for L1
		jz      L4
		test    esi, 3
		jz      L2
	L1:
		mov     cl, byte ptr [esi + edx]
		mov     bl, byte ptr [esi]
		cmp     cl, bl
		jne     L5
		test    cl, cl
		jz      L3
		dec     eax
		jz      L4
		inc     esi
		test    esi, 3
		jnz     L1
	L2:
		lea     ecx, [esi + edx]
		and     ecx, PAGE_SIZE - 1
		cmp     ecx, PAGE_SIZE - 4
		ja      L1
		mov     ecx, dword ptr [esi + edx]
		mov     ebx, dword ptr [esi]
		cmp     ecx, ebx
		jne     L1
		sub     eax, 4
		jbe     L3
		lea     ebx, [ecx - 01010101H]
		xor     ecx, -1
		and     ebx, 80808080H
		add     esi, 4
		test    ecx, ebx
		jz      L2
	L3:
		xor     eax, eax
	L4:
		pop     esi
		pop     ebx
		ret

		align   16
	L5:
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}

	#undef PAGE_SIZE
}
#endif
