#include <windows.h>

#ifndef _M_IX86
size_t __cdecl _mbslen(const unsigned char *string)
{
	size_t        length;
	unsigned char c;

	length = 0;
	while (c = string[length++])
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
			continue;
		else if (!(string++)[length])
			break;
	return --length;
}
#else
__declspec(naked) size_t __cdecl _mbslen(const unsigned char *string)
{
	__asm
	{
		#define string (esp + 4)

		push    esi
		push    edi
		xor     edi, edi
		mov     esi, dword ptr [string + 8]
		xor     eax, eax
		jmp     L1

		align   16
	L1:
		mov     al, byte ptr [esi + edi]
		inc     edi
		test    al, al
		jz      L2
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [esi + edi]
		inc     esi
		test    cl, cl
		jz      L2
		xor     eax, eax
		jmp     L1

		align   16
	L2:
		mov     eax, edi
		pop     edi
		dec     eax
		pop     esi
		ret

		#undef string
	}
}
#endif
