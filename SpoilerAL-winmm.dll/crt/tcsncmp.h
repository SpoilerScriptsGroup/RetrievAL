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

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]
		mov     edi, dword ptr [string2 + 8]
		mov     ecx, dword ptr [count + 8]
		xor     eax, eax
		xor     edx, edx
		lea     esi, [esi + ecx * 2]
		lea     edi, [edi + ecx * 2]
		xor     ecx, -1

		align   16
	L1:
		inc     ecx
		jz      L2
		mov     ax, word ptr [esi + ecx * 2]
		mov     dx, word ptr [edi + ecx * 2]
		sub     eax, edx
		jnz     L2
		test    edx, edx
		jnz     L1
	L2:
		pop     edi
		pop     esi
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
		push    edi
		mov     esi, dword ptr [string1 + 12]           // esi = string1
		mov     edi, dword ptr [string2 + 12]           // edi = string2
		mov     ecx, dword ptr [count + 12]             // ecx = count
		add     esi, ecx                                // esi = end of string1
		add     edi, ecx                                // edi = end of string2
		xor     ecx, -1                                 // ecx = -count - 1
		jmp     comp_head_loop_entry

		align   16
	comp_head_loop_begin:
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
		cmp     al, dl
		jne     return_not_equal
		test    al, al
		jz      return_equal
	comp_head_loop_entry:
		inc     ecx
		jz      return_equal
		lea     eax, [edi + ecx]
		lea     edx, [esi + ecx + 3]
		and     eax, 3
		jnz     comp_head_loop_begin

	dword_loop_begin:
		and     edx, PAGE_SIZE - 4
		jz      comp_head_loop_begin
		mov     eax, dword ptr [esi + ecx]
		mov     edx, dword ptr [edi + ecx]
		cmp     eax, edx
		jne     comp_head_loop_begin
		add     ecx, 4
		jc      return_equal
		lea     ebx, [eax - 01010101H]
		xor     eax, -1
		and     ebx, 80808080H
		lea     edx, [esi + ecx + 3]
		test    eax, ebx
		jz      dword_loop_begin

	return_equal:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	return_not_equal:
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
