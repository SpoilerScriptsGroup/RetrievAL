#include <ctype.h>
#include "PageSize.h"

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _stricmp(const char *string1, const char *string2)
{
	int ret, c;

	do
		if (ret = tolower(*(((unsigned char *)string1)++)) - (c = tolower(*(((unsigned char *)string2)++))))
			break;
	while (c);
	return ret;
}
#else
static int __cdecl stricmpSSE2(const char *string1, const char *string2);
static int __cdecl stricmp386(const char *string1, const char *string2);
static int __cdecl stricmpCPUDispatch(const char *string1, const char *string2);

static int(__cdecl * stricmpDispatch)(const char *string1, const char *string2) = stricmpCPUDispatch;

extern const char xmmconst_upperA[16];
extern const char xmmconst_azrangeA[16];
extern const char xmmconst_casebitA[16];
#define upper   xmmconst_upperA
#define azrange xmmconst_azrangeA
#define casebit xmmconst_casebitA

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

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]            // esi = string1
		mov     edi, dword ptr [string2 + 8]            // edi = string2
		mov     ecx, edi                                // ecx = string2
		sub     edi, esi                                // edi = string2 - string1
		movdqa  xmm4, xmmword ptr [upper]
		movdqa  xmm5, xmmword ptr [azrange]
		movdqa  xmm6, xmmword ptr [casebit]             // bit to change
		pxor    xmm7, xmm7                              // set to zero
		jmp     byte_loop_entry

		align   16
	byte_loop:
		movzx   eax, byte ptr [esi]
		movzx   edx, byte ptr [esi + edi]
		sub     eax, 'A'
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		lea     ecx, [eax + 'a' - 'A']
		cmovb   eax, ecx
		cmp     edx, 'Z' - 'A' + 1
		lea     ecx, [edx + 'a' - 'A']
		cmovb   edx, ecx
		sub     eax, edx
		jnz     epilogue
		cmp     edx, -'A'
		je      epilogue
		lea     ecx, [esi + edi + 1]
		inc     esi
	byte_loop_entry:
		and     ecx, 15
		jnz     byte_loop
		mov     ecx, esi
		and     ecx, PAGE_SIZE - 1

		align   16
	xmmword_loop:
		cmp     ecx, PAGE_SIZE - 16
		ja      byte_loop                               // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi]                 // load 16 byte
		movdqa  xmm1, xmmword ptr [esi + edi]           //
		movdqa  xmm2, xmm0                              // copy
		movdqa  xmm3, xmm1                              //
		paddb   xmm0, xmm4                              // all bytes greater than 'Z' if negative
		paddb   xmm1, xmm4                              //
		pcmpgtb xmm0, xmm5                              // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pcmpgtb xmm1, xmm5                              //
		pand    xmm0, xmm6                              // assign a mask for the appropriate bytes
		pand    xmm1, xmm6                              //
		por     xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		por     xmm1, xmm3                              //
		pcmpeqb xmm0, xmm1                              // compare
		pcmpeqb xmm2, xmm7                              //
		pmovmskb eax, xmm0                              // get one bit for each byte result
		pmovmskb ecx, xmm2                              //
		xor     eax, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilogue
		lea     ecx, [esi + 16]
		add     esi, 16
		and     ecx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     edx, 0FFFFH
		xor     ecx, 15
		shr     edx, cl
		and     eax, edx
		jz      epilogue
	xmmword_has_not_null:
		bsf     eax, eax
		add     esi, eax
		movzx   eax, byte ptr [esi]
		movzx   edx, byte ptr [esi + edi]
		sub     eax, 'A'
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		lea     ecx, [eax + 'a' - 'A']
		cmovb   eax, ecx
		cmp     edx, 'Z' - 'A' + 1
		lea     ecx, [edx + 'a' - 'A']
		cmovb   edx, ecx
		sub     eax, edx
	epilogue:
		pop     edi
		pop     esi
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
		xor     eax, eax                                // eax = 0
		mov     ecx, dword ptr [string1 + 4]            // ecx = string1
		mov     ebx, dword ptr [string2 + 4]            // ebx = string2
		sub     ebx, ecx                                // ebx = string2 - string1

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
		jne     not_equal
		sub     edx, 'a'
		xor     eax, eax
		cmp     edx, 'z' - 'a'
		jbe     loop_begin
		dec     eax
		pop     ebx
		ret

		align   16
	compare_above:
		sub     edx, 'A'
		xor     eax, eax
		cmp     edx, 'Z' - 'A'
		jbe     loop_begin
		inc     eax
		pop     ebx
		ret

		align   16
	not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A'
		ja      secondary_to_lower
		add     eax, 'a' - 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      difference
		add     edx, 'a' - 'A'
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
