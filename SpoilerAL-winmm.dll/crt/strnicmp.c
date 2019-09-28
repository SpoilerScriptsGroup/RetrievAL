#include <stddef.h>
#include <ctype.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	int ret, c;

	string1 += count;
	string2 += count;
	count ^= -1;
	while (++count)
		if (ret = tolower(string1[count]) - (c = tolower(string2[count])))
			return ret;
		else if (!c)
			break;
	return 0;
}
#else
static int __cdecl strnicmpSSE2(const char *string1, const char *string2, size_t count);
static int __cdecl strnicmp386(const char *string1, const char *string2, size_t count);
static int __cdecl strnicmpCPUDispatch(const char *string1, const char *string2, size_t count);

static int(__cdecl * strnicmpDispatch)(const char *string1, const char *string2, size_t count) = strnicmpCPUDispatch;

extern const char xmm_ahighA[16];
extern const char xmm_azrangeA[16];
extern const char xmm_casebitA[16];
#define ahigh   xmm_ahighA
#define azrange xmm_azrangeA
#define casebit xmm_casebitA

__declspec(naked) int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		jmp     dword ptr [strnicmpDispatch]
	}
}

__declspec(naked) static int __cdecl strnicmpSSE2(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax                                // eax = NULL
		mov     esi, dword ptr [string1 + 12]           // esi = string1
		mov     edi, dword ptr [string2 + 12]           // edi = string2
		mov     ebx, dword ptr [count + 12]             // ebx = count
		lea     esi, [esi + ebx]                        // esi = end of string1
		lea     edi, [edi + ebx]                        // edi = end of string2
		xor     ebx, -1                                 // ebx = -count - 1
		movdqa  xmm4, xmmword ptr [ahigh]
		movdqa  xmm5, xmmword ptr [azrange]
		pxor    xmm6, xmm6                              // set to zero
		movdqa  xmm7, xmmword ptr [casebit]             // bit to change
		jmp     byte_loop_increment

		align   16
	byte_loop:
		movzx   eax, byte ptr [esi + ebx]
		movzx   edx, byte ptr [edi + ebx]
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
	byte_loop_increment:
		inc     ebx
		jz      epilogue
		lea     edx, [edi + ebx]
		lea     ecx, [esi + ebx]
		and     edx, 15
		jnz     byte_loop
		and     ecx, PAGE_SIZE - 1

		align   16
	xmmword_loop:
		cmp     ecx, PAGE_SIZE - 15
		ja      byte_loop                               // jump if cross pages
		movdqu  xmm3, xmmword ptr [esi + ebx]           // load 16 byte
		movdqa  xmm1, xmmword ptr [edi + ebx]           //
		movdqa  xmm0, xmm3                              // copy
		pcmpeqb xmm3, xmm6                              // compare 16 bytes with zero
		movdqa  xmm2, xmm0                              // copy
		pmovmskb ecx, xmm3                              // get one bit for each byte result
		movdqa  xmm3, xmm1                              // copy
		psubb   xmm0, xmm4                              // all bytes less than 'A'
		psubb   xmm1, xmm4                              //
		psubusb xmm0, xmm5                              // and 'Z' will be reset
		psubusb xmm1, xmm5                              //
		pcmpeqb xmm0, xmm6                              // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pcmpeqb xmm1, xmm6                              //
		pand    xmm0, xmm7                              // assign a mask for the appropriate bytes
		pand    xmm1, xmm7                              //
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pxor    xmm1, xmm3                              //
		pcmpeqb xmm0, xmm1                              // compare
		pmovmskb edx, xmm0                              // get one bit for each byte result
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilogue
		add     ebx, 16
		jc      epilogue
		lea     ecx, [esi + ebx]
		and     ecx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     eax, 0FFFFH
		xor     ecx, 15
		shr     eax, cl
		and     eax, edx
		jz      epilogue
		mov     edx, eax
		xor     eax, eax
	xmmword_has_not_null:
		bsf     edx, edx
		add     ebx, edx
		jc      epilogue
		movzx   eax, byte ptr [esi + ebx]
		movzx   edx, byte ptr [edi + ebx]
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
		pop     esi
		pop     edi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl strnicmp386(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax                                // eax = NULL
		mov     esi, dword ptr [string1 + 12]           // esi = string1
		mov     edi, dword ptr [string2 + 12]           // edi = string2
		mov     ecx, dword ptr [count + 12]             // ecx = count
		xor     edx, edx
		lea     esi, [esi + ecx]                        // esi = end of string1
		lea     edi, [edi + ecx]                        // edi = end of string2
		xor     ecx, -1                                 // ecx = -count - 1

		align   16
	loop_begin:
		inc     ecx
		jz      return_equal
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_begin
	return_equal:
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     loop_begin
		dec     eax
		jmp     epilogue

		align   16
	compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     loop_begin
		inc     eax
		jmp     epilogue

		align   16
	return_not_equal:
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
	epilogue:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl strnicmpCPUDispatch(const char *string1, const char *string2, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strnicmpDispatch], offset strnicmp386
		jmp     strnicmp386
	L1:
		mov     dword ptr [strnicmpDispatch], offset strnicmpSSE2
		jmp     strnicmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
