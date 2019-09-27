#include <ctype.h>
#include "PageSize.h"

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _stricmp(const char *string1, const char *string2)
{
	int ret, c;

	do
		if (ret = tolower(*(string1++)) - (c = tolower(*(string2++))))
			return ret;
	while (c);
	return 0;
}
#else
static int __cdecl stricmpSSE2(const char *string1, const char *string2);
static int __cdecl stricmp386(const char *string1, const char *string2);
static int __cdecl stricmpCPUDispatch(const char *string1, const char *string2);

static int(__cdecl * stricmpDispatch)(const char *string1, const char *string2) = stricmpCPUDispatch;

extern const char xmm_ahighA[16];
extern const char xmm_azrangeA[16];
extern const char xmm_casebitA[16];
#define ahigh   xmm_ahighA
#define azrange xmm_azrangeA
#define casebit xmm_casebitA

__declspec(naked) int __cdecl _stricmp(const char *string1, const char *string2)
{
	__asm
	{
		jmp     dword ptr [stricmpDispatch]
	}
}

__declspec(naked) static int __cdecl stricmpSSE2(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		push    esi
		mov     esi, dword ptr [string1 + 8]            // esi = string1
		mov     eax, dword ptr [string2 + 8]            // eax = string2
		mov     ebx, esi
		sub     esi, eax
		xor     edx, edx
		xor     ecx, ecx
		movdqa  xmm4, xmmword ptr [ahigh]
		movdqa  xmm5, xmmword ptr [azrange]
		pxor    xmm6, xmm6                              // set to zero
		movdqa  xmm7, xmmword ptr [casebit]             // bit to change
		jmp     byte_loop_entry

		align   16
	byte_loop:
		mov     dl, byte ptr [eax + esi]
		mov     cl, byte ptr [eax]
		sub     edx, ecx
		jnz     byte_compare_insensitive
		test    ecx, ecx
		jz      return_equal
	byte_loop_increment:
		lea     ebx, [eax + esi + 1]
		inc     eax
	byte_loop_entry:
		test    eax, 15                                  // use only eax for 'test reg, imm'
		jnz     byte_loop
		and     ebx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	byte_compare_insensitive:
		cmp     edx, 'a' - 'A'
		je      byte_compare_above
		cmp     edx, 'A' - 'a'
		jne     byte_not_equal
		xor     edx, edx
		lea     ebx, [ecx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     byte_loop_increment
		sub     ecx, 'A'
		mov     eax, ebx
		jmp     secondary_to_lower

		align   16
	byte_compare_above:
		xor     edx, edx
		lea     ebx, [ecx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     byte_loop_increment
		mov     ecx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	byte_not_equal:
		lea     eax, [edx + ecx - 'A']
		sub     ecx, 'A'
		jmp     secondary_to_lower

		align   16
	return_equal:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	xmmword_loop:
		cmp     ebx, PAGE_SIZE - 15
		ja      byte_loop                                // jump if cross pages
		movdqu  xmm3, xmmword ptr [eax + esi]            // enter 16 byte
		movdqa  xmm1, xmmword ptr [eax]                  //
		movdqa  xmm0, xmm3                               // copy
		pcmpeqb xmm3, xmm6                               // compare 16 bytes with zero
		movdqa  xmm2, xmm0                               // copy
		pmovmskb ecx, xmm3                               // get one bit for each byte result
		movdqa  xmm3, xmm1                               // copy
		psubb   xmm0, xmm4                               // all bytes less than 'A'
		psubb   xmm1, xmm4                               //
		psubusb xmm0, xmm5                               // and 'Z' will be reset
		psubusb xmm1, xmm5                               //
		pcmpeqb xmm0, xmm6                               // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pcmpeqb xmm1, xmm6                               //
		pand    xmm0, xmm7                               // assign a mask for the appropriate bytes
		pand    xmm1, xmm7                               //
		pxor    xmm0, xmm2                               // negation of the 5th bit - lowercase letters
		pxor    xmm1, xmm3                               //
		pcmpeqb xmm0, xmm1                               // compare
		pmovmskb edx, xmm0                               // get one bit for each byte result
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     return_equal
		lea     ebx, [eax + esi + 16]
		add     eax, 16
		and     ebx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     ebx, 0FFFFH
		xor     ecx, 15
		shr     ebx, cl
		and     edx, ebx
		jz      return_equal
		xor     ecx, ecx
	xmmword_has_not_null:
		bsf     edx, edx
		add     edx, eax
		xor     eax, eax
		mov     cl, byte ptr [edx]
		mov     al, byte ptr [edx + esi]
		sub     ecx, 'A'
		sub     eax, 'A'
	secondary_to_lower:
		cmp     ecx, 'Z' - 'A'
		ja      primary_to_lower
		add     ecx, 'a' - 'A'
	primary_to_lower:
		cmp     eax, 'Z' - 'A'
		ja      return_difference
		add     eax, 'a' - 'A'
	return_difference:
		sub     eax, ecx
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}

__declspec(naked) static int __cdecl stricmp386(const char *string1, const char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		xor     eax, eax
		mov     ecx, dword ptr [string1 + 4]
		mov     ebx, dword ptr [string2 + 4]
		sub     ebx, ecx

		align   16
	loop_begin:
		xor     edx, edx
		mov     al, byte ptr [ecx]
		mov     dl, byte ptr [ecx + ebx]
		inc     ecx
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_begin
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		sub     edx, 'a'
		xor     eax, eax
		cmp     edx, 'z' - 'a'
		jbe     loop_begin
		mov     eax, edx
		add     edx, 'a' - 'A'
		jmp     secondary_to_lower

		align   16
	compare_above:
		sub     edx, 'A'
		xor     eax, eax
		cmp     edx, 'Z' - 'A'
		jbe     loop_begin
		lea     eax, [edx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	return_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      primary_to_lower
		add     edx, 'a' - 'A'
	primary_to_lower:
		cmp     eax, 'Z' - 'A'
		ja      difference
		add     eax, 'a' - 'A'
	difference:
		sub     eax, edx
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}

__declspec(naked) static int __cdecl stricmpCPUDispatch(const char *string1, const char *string2)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [stricmpDispatch], offset stricmp386
		jmp     stricmp386
	L1:
		mov     dword ptr [stricmpDispatch], offset stricmpSSE2
		jmp     stricmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
