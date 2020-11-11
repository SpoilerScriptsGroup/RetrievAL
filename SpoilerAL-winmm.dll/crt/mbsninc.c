#include <windows.h>

#ifndef _M_IX86
unsigned char * __cdecl _mbsninc(const unsigned char *string, size_t count)
{
	unsigned char c;

	if (string && count)
		do
			if (!(c = *string)))
				break;
			else if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				string++;
			else if (!string[1])
				break;
			else
				string += 2;
		while (--count);
	return (unsigned char *)string;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsninc(const unsigned char *string, size_t count)
{
	__asm
	{
		#define string (esp + 4)
		#define count  (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string + 8]
		mov     ecx, dword ptr [count + 8]
		xor     eax, eax
		mov     edi, ecx
		or      ecx, esi
		jnz     L2
		jmp     L6

		align   16
	L1:
		dec     edi
		jz      L5
	L2:
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L5
	L3:
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
	L4:
		mov     cl, byte ptr [esi]
		xor     eax, eax
		test    cl, cl
		jz      L5
		dec     edi
		jz      L5
		mov     al, byte ptr [esi + 1]
		add     esi, 2
		test    al, al
		jnz     L3
	L5:
		dec     esi
	L6:
		pop     edi
		mov     eax, esi
		pop     esi
		ret

		#undef string
		#undef count
	}
}
#endif
