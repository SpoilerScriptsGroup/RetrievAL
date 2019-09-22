#include <wchar.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl wcsncmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	unsigned short c1, c2;

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
#else
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
	loop_head:
		inc     ecx
		jz      epilogue
		mov     ax, word ptr [esi + ecx * 2]
		mov     dx, word ptr [edi + ecx * 2]
		sub     eax, edx
		jnz     epilogue
		test    edx, edx
		jnz     loop_head
	epilogue:
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
