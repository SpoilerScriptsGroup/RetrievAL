#include <wchar.h>

#ifndef _M_IX86
wchar_t * __cdecl _wcslwr(wchar_t *string)
{
	wchar_t *p, c;

	if (c = *(p = string))
		do
			if ((unsigned)(c - L'A') <= L'Z' - L'A')
				*p = c + (L'a' - L'A');
		while (c = *(++p));
	return string;
}

wchar_t * __cdecl _wcsupr(wchar_t *string)
{
	wchar_t *p, c;

	if (c = *(p = string))
		do
			if ((unsigned)(c - L'a') <= L'z' - L'a')
				*p = c - (L'a' - L'A');
		while (c = *(++p));
	return string;
}
#else
#include <emmintrin.h>

extern const wchar_t xmmconst_ahighW[8];
extern const wchar_t xmmconst_alowW[8];
extern const wchar_t xmmconst_azrangeW[8];
extern const wchar_t xmmconst_casebitW[8];
extern const char xmmconst_maskbit[32];
#define ahigh   xmmconst_ahighW
#define alow    xmmconst_alowW
#define azrange xmmconst_azrangeW
#define casebit xmmconst_casebitW
#define maskbit xmmconst_maskbit

static wchar_t * __cdecl wcslwrSSE2(wchar_t *string);
static wchar_t * __cdecl wcsuprSSE2(wchar_t *string);
static wchar_t * __cdecl wcslwruprSSE2(wchar_t *string);
static wchar_t * __cdecl wcslwr386(wchar_t *string);
static wchar_t * __cdecl wcsupr386(wchar_t *string);
static wchar_t * __cdecl wcslwrupr386(wchar_t *string);
static wchar_t * __cdecl wcslwrCPUDispatch(wchar_t *string);
static wchar_t * __cdecl wcsuprCPUDispatch(wchar_t *string);

static wchar_t *(__cdecl * wcslwrDispatch)(wchar_t *string) = wcslwrCPUDispatch;
static wchar_t *(__cdecl * wcsuprDispatch)(wchar_t *string) = wcsuprCPUDispatch;

__declspec(naked) wchar_t * __cdecl _wcslwr(wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcslwrDispatch]
	}
}

__declspec(naked) wchar_t * __cdecl _wcsupr(wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcsuprDispatch]
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwrSSE2(wchar_t *string)
{
	__asm
	{
		movdqa  xmm3, xmmword ptr [ahigh]
		jmp     wcslwruprSSE2
	}
}

__declspec(naked) static wchar_t * __cdecl wcsuprSSE2(wchar_t *string)
{
	__asm
	{
		movdqa  xmm3, xmmword ptr [alow]
		jmp     wcslwruprSSE2
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwruprSSE2(wchar_t *string)
{
	__asm
	{
		mov     eax, dword ptr [esp + 4]                // string
		push    edi
		mov     ecx, eax
		mov     edi, eax
		movdqa  xmm4, xmmword ptr [azrange]
		pxor    xmm5, xmm5                              // set to zero
		movdqa  xmm6, xmmword ptr [casebit]             // bit to change
		and     ecx, 15
		jz      aligned_loop_entry
		test    eax, 1
		jnz     unaligned
		and     edi, -16
		xor     ecx, 15
		movdqa  xmm0, xmmword ptr [edi]                 // load 16 byte
		movdqu  xmm1, xmmword ptr [maskbit + ecx + 1]
		movdqa  xmm2, xmm0                              // copy
		por     xmm0, xmm1                              // fill the non target bits to 1
		movdqa  xmm1, xmm0                              // copy
		psubw   xmm0, xmm3                              // all words less than 'A'
		psubusw xmm0, xmm4                              // and 'Z' will be reset
		pcmpeqw xmm0, xmm5                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm5                              // compare 8 words with zero
		pand    xmm0, xmm6                              // assign a mask for the appropriate words
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pmovmskb ecx, xmm1                              // get one bit for each byte result
		test    ecx, ecx
		jnz     store_last_xmmword

		align   16
	aligned_loop:
		movdqa  xmmword ptr [edi], xmm0
		add     edi, 16
	aligned_loop_entry:
		movdqa  xmm0, xmmword ptr [edi]                 // load 16 byte
		movdqa  xmm1, xmm0                              // copy
		movdqa  xmm2, xmm0                              //
		psubw   xmm0, xmm3                              // all words less than 'A'
		psubusw xmm0, xmm4                              // and 'Z' will be reset
		pcmpeqw xmm0, xmm5                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm5                              // compare 8 words with zero
		pand    xmm0, xmm6                              // assign a mask for the appropriate words
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pmovmskb ecx, xmm1                              // get one bit for each byte result
		test    ecx, ecx
		jz      aligned_loop
		jmp     store_last_xmmword

		align   16
	unaligned:
		xor     ecx, 15
		jz      unaligned_loop_entry
		and     edi, -16
		movdqu  xmm1, xmmword ptr [maskbit + ecx]
		movdqa  xmm0, xmmword ptr [edi]                 // load 16 byte
		movdqa  xmm2, xmm0                              // copy
		pslldq  xmm0, 1                                 // adjust xmm value for compare
		por     xmm0, xmm1                              // fill the non target bits to 1
		movdqa  xmm1, xmm0                              // copy
		psubw   xmm0, xmm3                              // all words less than 'A'
		psubusw xmm0, xmm4                              // and 'Z' will be reset
		pcmpeqw xmm0, xmm5                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm5                              // compare 8 words with zero
		pand    xmm0, xmm6                              // assign a mask for the appropriate words
		psrldq  xmm0, 1                                 // adjust mask value to 16 byte alignment
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pmovmskb ecx, xmm1                              // get one bit for each byte result
		shr     ecx, 1
		jnz     store_last_xmmword
		movdqa  xmmword ptr [edi], xmm0
		add     edi, 15
		jmp     unaligned_loop_entry

		align   16
	unaligned_loop:
		movdqu  xmmword ptr [edi], xmm0
		add     edi, 16
	unaligned_loop_entry:
		movdqu  xmm0, xmmword ptr [edi]                 // load 16 byte
		movdqa  xmm1, xmm0                              // copy
		movdqa  xmm2, xmm0                              //
		psubw   xmm0, xmm3                              // all words less than 'A'
		psubusw xmm0, xmm4                              // and 'Z' will be reset
		pcmpeqw xmm0, xmm5                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm5                              // compare 8 words with zero
		pand    xmm0, xmm6                              // assign a mask for the appropriate words
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pmovmskb ecx, xmm1                              // get one bit for each byte result
		test    ecx, ecx
		jz      unaligned_loop

		align   16
	store_last_xmmword:
		shr     ecx, 1
		jc      epilogue
		bsf     ecx, ecx
		xor     ecx, 15
		movdqu  xmm1, xmmword ptr [maskbit + ecx]
		maskmovdqu xmm0, xmm1
	epilogue:
		pop     edi
		ret
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwr386(wchar_t *string)
{
	__asm
	{
		mov     ecx, 'A'
		jmp     wcslwrupr386
	}
}

__declspec(naked) static wchar_t * __cdecl wcsupr386(wchar_t *string)
{
	__asm
	{
		mov     ecx, 'a'
		jmp     wcslwrupr386
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwrupr386(wchar_t *string)
{
	__asm
	{
		mov     edx, dword ptr [esp + 4]                // string
		push    ebx

		align   16
	A100:
		// loop
		mov     ax, word ptr [edx]
		add     edx, 2
		test    ax, ax
		jz      A900                                    // end of string

	A200:
		mov     bx, ax                                  // check case
		sub     ax, cx
		cmp     ax, 'Z' - 'A'
		ja      A100

		// convert case
		mov     ax, word ptr [edx]
		xor     bx, 'a' - 'A'
		mov     word ptr [edx - 2], bx
		add     edx, 2
		test    ax, ax
		jnz     A200                                    // loop to next character

	A900:
		mov     eax, dword ptr [esp + 8]                // string
		pop     ebx
		ret
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwrCPUDispatch(wchar_t *string)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcslwrDispatch], offset wcslwr386
		jmp     wcslwr386
	L1:
		mov     dword ptr [wcslwrDispatch], offset wcslwrSSE2
		jmp     wcslwrSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}

__declspec(naked) static wchar_t * __cdecl wcsuprCPUDispatch(wchar_t *string)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsuprDispatch], offset wcsupr386
		jmp     wcsupr386
	L1:
		mov     dword ptr [wcsuprDispatch], offset wcsuprSSE2
		jmp     wcsuprSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
