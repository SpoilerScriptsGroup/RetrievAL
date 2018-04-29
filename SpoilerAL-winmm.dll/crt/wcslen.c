#include <string.h>

#ifndef _M_IX86
size_t __cdecl wcslen(const wchar_t *string)
{
	size_t length;

	length = 0;
	while (string[length++]);
	return --length;
}
#else
__declspec(naked) size_t __cdecl wcslen(const wchar_t *string)
{
	__asm
	{
		#define string (esp + 4)

		mov     edx, dword ptr [string]
		xor     ecx, ecx

		align   16
	L1:
		mov     ax, word ptr [edx + ecx * 2]
		inc     ecx
		test    ax, ax
		jnz     L1
		lea     eax, [ecx - 1]
		ret

		#undef string
	}
}
#endif
