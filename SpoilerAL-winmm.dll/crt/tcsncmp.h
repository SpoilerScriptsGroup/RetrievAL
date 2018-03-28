#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
int __cdecl _tcsncmp(const TCHAR *string1, const TCHAR *string2, size_t count)
{
#ifdef _UNICODE
	typedef wchar_t       uchar_t;
#else
	typedef unsigned char uchar_t;
#endif

	uchar_t c1, c2;

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
		mov     ecx, dword ptr [string1]
		test    eax, eax
		jz      L3
		mov     edx, dword ptr [string2]
		push    ebx
		push    esi
		lea     ebx, [ecx + eax * 2]
		lea     esi, [edx + eax * 2]
		dec     eax
		xor     eax, -1

		align   16
	L1:
		mov     cx, word ptr [ebx + eax * 2]
		mov     dx, word ptr [esi + eax * 2]
		cmp     cx, dx
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
		mov     ebx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		mov     ecx, dword ptr [count + 8]
		sub     ebx, esi
		test    ecx, -1 ; alignment for L1
		jz      L3
		test    esi, 3
		jz      L2
	L1:
		mov     al, byte ptr [esi + ebx]
		mov     dl, byte ptr [esi]
		cmp     al, dl
		jne     L4
		test    al, al
		jz      L3
		dec     ecx
		jz      L3
		inc     esi
		test    esi, 3
		jnz     L1
	L2:
		lea     eax, [esi + ebx]
		and     eax, PAGE_SIZE - 1
		cmp     eax, PAGE_SIZE - 4
		ja      L1
		mov     eax, dword ptr [esi + ebx]
		mov     edx, dword ptr [esi]
		cmp     eax, edx
		jne     L1
		sub     ecx, 4
		jbe     L3
		lea     edx, [eax - 01010101H]
		xor     eax, -1
		and     edx, 80808080H
		add     esi, 4
		test    eax, edx
		jz      L2
	L3:
		xor     eax, eax
		pop     esi
		pop     ebx
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

	#undef PAGE_SIZE
}
#endif
