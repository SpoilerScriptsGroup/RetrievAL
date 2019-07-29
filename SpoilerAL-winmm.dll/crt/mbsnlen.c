#include <windows.h>
#include <mbstring.h>

#ifndef _M_IX86
size_t __cdecl _mbsnlen(const unsigned char *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		unsigned char c;

		while ((c = *(p++)) && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c) || *(p++)) && --length);
		length = maxlen - length;
	}
	return length;
}
#else
__declspec(naked) size_t __cdecl _mbsnlen(const unsigned char *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     ecx, dword ptr [string]
		test    eax, eax
		jz      L4
		push    ebx
		push    esi
		mov     ebx, eax
		xor     eax, eax
		mov     esi, ecx

		align   16
	L1:
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		xor     eax, eax
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L3
	L2:
		dec     ebx
		jnz     L1
	L3:
		mov     eax, dword ptr [maxlen + 8]
		pop     esi
		sub     eax, ebx
		pop     ebx
	L4:
		ret

		#undef string
		#undef maxlen
	}
}
#endif
