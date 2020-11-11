#include <windows.h>

#pragma function(strlen)

#ifndef _M_IX86
unsigned char * __cdecl _mbsnbcat(unsigned char *string1, const unsigned char *string2, size_t count)
{
	if (count)
	{
		unsigned char       *dest, c, lead;
		const unsigned char *src;

		dest = string1;
		src = string2;
		while (c = *dest)
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
				dest++;
			else if (!dest[1])
				break;
			else
				dest += 2;
		while (c = *(src++))
		{
			if (IsDBCSLeadByteEx(CP_THREAD_ACP, c))
			{
				if (!--count)
					break;
				lead = c;
				if (!(c = *(src++)))
					break;
				*(dest++) = lead;
			}
			*(dest++) = c;
			if (!--count)
				break;
		}
		*dest = '\0';
	}
	return string1;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsnbcat(unsigned char *string1, const unsigned char *string2, size_t count)
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
		jz      L7
		mov     esi, dword ptr [string2 + 16]
		xor     eax, eax
		jmp     L1

		align   16
	L1:
		mov     al, byte ptr [edi]
		inc     edi
		test    al, al
		jz      L3
	L2:
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     cl, byte ptr [edi]
		xor     eax, eax
		test    cl, cl
		jz      L3
		mov     al, byte ptr [edi + 1]
		add     edi, 2
		test    al, al
		jnz     L2
	L3:
		sub     edi, 2
		xor     ebx, ebx
		sub     esi, edi

		align   16
	L4:
		mov     bl, byte ptr [esi + edi]
		inc     edi
		test    bl, bl
		jz      L6
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L5
		dec     ebp
		jz      L6
		mov     eax, ebx
		mov     bl, byte ptr [esi + edi]
		test    bl, bl
		jz      L6
		mov     byte ptr [edi], al
		inc     edi
	L5:
		mov     byte ptr [edi], bl
		xor     ebx, ebx
		dec     ebp
		jnz     L4
		inc     edi
	L6:
		mov     byte ptr [edi], '\0'
	L7:
		mov     eax, dword ptr [string1 + 16]
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
