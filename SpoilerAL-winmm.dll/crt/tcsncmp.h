#include <string.h>
#include <tchar.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _tcsncmp(const TCHAR *string1, const TCHAR *string2, size_t count)
{
#ifdef _UNICODE
	unsigned short c1, c2;
#else
	unsigned char  c1, c2;
#endif

	string1 += count;
	string2 += count;
	count = ~count;
	do
		if (!++count)
			break;
		else if ((c1 = string1[count]) != (c2 = string2[count]))
			return (int)c1 - (int)c2;
	while (c1);
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

		push    ebx
		push    esi
		mov     edx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		mov     ecx, dword ptr [count + 8]
		xor     eax, eax
		lea     edx, [edx + ecx * 2]
		lea     esi, [esi + ecx * 2]
		xor     ecx, -1

		align   16
	L1:
		inc     ecx
		jz      L2
		mov     ax, word ptr [edx + ecx * 2]
		mov     bx, word ptr [esi + ecx * 2]
		cmp     ax, bx
		jne     L3
		test    ax, ax
		jnz     L1
	L2:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	L3:
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
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		mov     esi, dword ptr [string1 + 8]
		push    edi
		mov     edi, dword ptr [string2 + 12]
		mov     ecx, dword ptr [count + 12]
		add     esi, ecx
		add     edi, ecx
		xor     ecx, -1
		jmp     L2

		align   16
	L1:
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
		cmp     al, dl
		jne     L5
		test    al, al
		jz      L4
	L2:
		inc     ecx
		jz      L4
		lea     eax, [edi + ecx]
		lea     edx, [esi + ecx]
		test    eax, 3
		jnz     L1
	L3:
		and     edx, PAGE_SIZE - 1
		cmp     edx, PAGE_SIZE - 4
		ja      L1
		mov     eax, dword ptr [esi + ecx]
		mov     edx, dword ptr [edi + ecx]
		cmp     eax, edx
		jne     L1
		add     ecx, 4
		jc      L4
		lea     ebx, [eax - 01010101H]
		xor     eax, -1
		and     ebx, 80808080H
		lea     edx, [esi + ecx]
		test    eax, ebx
		jz      L3
	L4:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	L5:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
