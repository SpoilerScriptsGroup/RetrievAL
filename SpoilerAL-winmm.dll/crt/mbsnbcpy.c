#include <windows.h>

#pragma function(memset)

#ifndef _M_IX86
unsigned char * __cdecl _mbsnbcpy(unsigned char *string1, const unsigned char *string2, size_t count)
{
	if (count)
	{
		unsigned char       *dest, c, lead;
		const unsigned char *src;

		dest = string1;
		src = string2;
		while (c = *(src++))
		{
			if (IsDBCSLeadByteEx(CP_THREAD_ACP, c))
			{
				if (!--count)
					return string1;
				lead = c;
				if (!(c = *(src++)))
					break;
				*(dest++) = lead;
			}
			*(dest++) = c;
			if (!--count)
				return string1;
		}
		memset(dest, 0, count);
	}
	return string1;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsnbcpy(unsigned char *string1, const unsigned char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebp, dword ptr [count + 16]
		mov     edi, dword ptr [string1 + 16]
		test    ebp, ebp
		jz      L3
		mov     esi, dword ptr [string2 + 16]
		dec     edi
		xor     ebx, ebx
		sub     esi, edi
		jmp     L1

		align   16
	L1:
		mov     bl, byte ptr [esi + edi]
		inc     edi
		test    bl, bl
		jz      L4
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		dec     ebp
		jz      L3
		mov     eax, ebx
		mov     bl, byte ptr [esi + edi]
		test    bl, bl
		jz      L4
		mov     byte ptr [edi], al
		inc     edi
	L2:
		mov     byte ptr [edi], bl
		xor     ebx, ebx
		dec     ebp
		jnz     L1
	L3:
		mov     eax, dword ptr [string1 + 16]
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	L4:
		push    ebp
		push    ebx
		push    edi
		call    memset
		mov     eax, dword ptr [string1 + 28]
		add     esp, 12
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
