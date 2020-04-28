#include <wchar.h>

#ifndef _M_IX86
wchar_t * __cdecl _wcslwr(wchar_t *string)
{
	wchar_t *p, c;

	if (c = *(p = string))
		do
			if ((wchar_t)(c - L'A') < L'Z' - L'A' + 1)
				*p = c + (L'a' - L'A');
		while (c = *(++p));
	return string;
}

wchar_t * __cdecl _wcsupr(wchar_t *string)
{
	wchar_t *p, c;

	if (c = *(p = string))
		do
			if ((wchar_t)(c - L'a') < L'z' - L'a' + 1)
				*p = c - (L'a' - L'A');
		while (c = *(++p));
	return string;
}
#else
extern const wchar_t xmmconst_upperW[8];
extern const wchar_t xmmconst_lowerW[8];
extern const wchar_t xmmconst_azrangeW[8];
extern const wchar_t xmmconst_casebitW[8];
extern const char xmmconst_maskbit[32];
#define upper   xmmconst_upperW
#define lower   xmmconst_lowerW
#define azrange xmmconst_azrangeW
#define casebit xmmconst_casebitW
#define maskbit xmmconst_maskbit

__declspec(align(16)) static const wchar_t azhigh[8] = {    // define range for upper case
	L'A', L'Z', L'A', L'Z', L'A', L'Z', L'A', L'Z'
};
__declspec(align(16)) static const wchar_t azlow[8] = {     // define range for lower case
	L'a', L'z', L'a', L'z', L'a', L'z', L'a', L'z'
};

static wchar_t * __cdecl wcslwrSSE42(wchar_t *string);
static wchar_t * __cdecl wcsuprSSE42(wchar_t *string);
static wchar_t * __cdecl wcslwruprSSE42(wchar_t *string);
static wchar_t * __cdecl wcslwrSSE2(wchar_t *string);
static wchar_t * __cdecl wcsuprSSE2(wchar_t *string);
static wchar_t * __cdecl wcslwruprSSE2(wchar_t *string);
static wchar_t * __cdecl wcslwrGeneric(wchar_t *string);
static wchar_t * __cdecl wcsuprGeneric(wchar_t *string);
static wchar_t * __cdecl wcslwruprGeneric(wchar_t *string);
static wchar_t * __cdecl wcslwrCPUDispatch(wchar_t *string);
static wchar_t * __cdecl wcsuprCPUDispatch(wchar_t *string);

// Pointer to appropriate version. Initially points to dispatcher
static wchar_t *(__cdecl *wcslwrDispatch)(wchar_t *string) = wcslwrCPUDispatch;
static wchar_t *(__cdecl *wcsuprDispatch)(wchar_t *string) = wcsuprCPUDispatch;

// _wcslwr function
__declspec(naked) wchar_t * __cdecl _wcslwr(wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcslwrDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// _wcsupr function
__declspec(naked) wchar_t * __cdecl _wcsupr(wchar_t *string)
{
	__asm
	{
		jmp     dword ptr [wcsuprDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// SSE4.2 version
__declspec(naked) static wchar_t * __cdecl wcslwrSSE42(wchar_t *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [azhigh]                  // define range A-Z
		jmp     wcslwruprSSE42
	}
}

// SSE4.2 version
__declspec(naked) static wchar_t * __cdecl wcsuprSSE42(wchar_t *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [azlow]                   // define range a-z
		jmp     wcslwruprSSE42
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwruprSSE42(wchar_t *string)
{
	__asm
	{
		// common code for wcsupr and wcslwr
		mov     ecx, dword ptr [esp + 4]                    // string
		movdqa  xmm3, xmmword ptr [casebit]                 // bit to change
		mov     eax, ecx
		mov     edx, ecx
		and     ecx, 15
		jz      aligned_loop_entry
		test    eax, 1
		jnz     unaligned
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm4, xmmword ptr [maskbit + ecx + 1]
		movdqa  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		por     xmm4, xmm1                                  // fill the non target bits to 1
		pcmpistrm xmm2, xmm4, 01000101B                     // find words in range A-Z or a-z, return mask in xmm0
		jz      aligned_last                                // string ends in this paragraph

		align   16
	aligned_loop:
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
		add     edx, 16
	aligned_loop_entry:
		movdqa  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm2, xmm1, 01000101B                     // find words in range A-Z or a-z, return mask in xmm0
		jnz      aligned_loop                               // next 16 bytes

	aligned_last:
		// Write last 0-15 bytes
		// While we can read past the end of the string if precautions are made, we cannot write
		// past the end of the string, even if the value is unchanged, because the value may have
		// been changed in the meantime by another thread
		jnc     aligned_finish                              // nothing changed, no need to write
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
	aligned_finish:
		ret

		align   16
	unaligned:
		xor     ecx, 15
		jz      unaligned_loop_entry
		and     edx, -16
		movdqa  xmm0, xmmword ptr [edx]                     // read 16 bytes from string
		movdqu  xmm4, xmmword ptr [maskbit + ecx]           // load the non target bits mask
		movdqa  xmm1, xmm0                                  // copy
		pslldq  xmm0, 1                                     // adjust xmm value for compare
		por     xmm4, xmm0                                  // fill the non target bits to 1
		pcmpistrm xmm2, xmm4, 01000101B                     // find words in range A-Z or a-z, return mask in xmm0
		jz      unaligned_first
		pand    xmm0, xmm3                                  // mask AND case bit
		psrldq  xmm0, 1                                     // adjust mask value to 16 bytes alignment
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
		add     edx, 15
		jmp     unaligned_loop_entry

		align   16
	unaligned_first:
		jnc     unaligned_first_finish                      // nothing changed, no need to write
		pand    xmm0, xmm3                                  // mask AND case bit
		psrldq  xmm0, 1                                     // adjust mask value to 16 bytes alignment
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
	unaligned_first_finish:
		ret

		align   16
		// loop
	unaligned_loop:
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqu  xmmword ptr [edx], xmm0                     // write changed value
		add     edx, 16
	unaligned_loop_entry:
		movdqu  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm2, xmm1, 01000101B                     // find words in range A-Z or a-z, return mask in xmm0
		jnz     unaligned_loop                              // next 16 bytes

		// Write last 0-15 bytes
		// While we can read past the end of the string if precautions are made, we cannot write
		// past the end of the string, even if the value is unchanged, because the value may have
		// been changed in the meantime by another thread
		jnc     unaligned_finish                            // nothing changed, no need to write
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqu  xmmword ptr [edx], xmm0                     // write changed value
	unaligned_finish:
		ret
	}
}

// SSE2 version
__declspec(naked) static wchar_t * __cdecl wcslwrSSE2(wchar_t *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [upper]
		jmp     wcslwruprSSE2
	}
}

// SSE2 version
__declspec(naked) static wchar_t * __cdecl wcsuprSSE2(wchar_t *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [lower]
		jmp     wcslwruprSSE2
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwruprSSE2(wchar_t *string)
{
	__asm
	{
		mov     ecx, dword ptr [esp + 4]                    // string
		pxor    xmm3, xmm3                                  // set to zero
		movdqa  xmm4, xmmword ptr [azrange]
		movdqa  xmm5, xmmword ptr [casebit]                 // bit to change
		mov     eax, ecx
		mov     edx, ecx
		and     ecx, 15
		jz      aligned_loop_entry1
		test    eax, 1
		jnz     unaligned
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm0, xmmword ptr [maskbit + ecx + 1]       // load the non target bits mask
		movdqa  xmm1, xmmword ptr [edx]                     // load 16 bytes
		por     xmm0, xmm1                                  // fill the non target bits to 1
		jmp     aligned_loop_entry2

		align   16
	aligned_loop:
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
		add     edx, 16
	aligned_loop_entry1:
		movdqa  xmm0, xmmword ptr [edx]                     // load 16 bytes
		movdqa  xmm1, xmm0                                  // copy
	aligned_loop_entry2:
		pcmpeqw xmm3, xmm0                                  // compare 8 words with zero
		paddw   xmm0, xmm2                                  // all words greater than 'Z' if negative
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pcmpgtw xmm0, xmm4                                  // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pand    xmm0, xmm5                                  // assign a mask for the appropriate words
		test    ecx, ecx
		jz      aligned_loop                                // next 16 bytes

	aligned_last:
		shr     ecx, 1
		jc      aligned_epilogue
		bsf     ecx, ecx
		xor     ecx, 15
		movdqu  xmm2, xmmword ptr [maskbit + ecx]           // load the target bits mask
		pand    xmm0, xmm2                                  // assign a mask for casebit
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
	aligned_epilogue:
		ret

		align   16
	unaligned:
		xor     ecx, 15
		jz      unaligned_loop_entry
		and     edx, -16
		movdqa  xmm6, xmmword ptr [edx]                     // load 16 bytes
		movdqu  xmm0, xmmword ptr [maskbit + ecx]           // load the non target bits mask
		movdqa  xmm1, xmm6                                  // copy
		pslldq  xmm6, 1                                     // adjust xmm value for compare
		por     xmm0, xmm6                                  // fill the non target bits to 1
		pcmpeqw xmm3, xmm0                                  // compare 8 words with zero
		paddw   xmm0, xmm2                                  // all words greater than 'Z' if negative
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pcmpgtw xmm0, xmm4                                  // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pand    xmm0, xmm5                                  // assign a mask for the appropriate words
		psrldq  xmm0, 1                                     // adjust mask value to 16 bytes alignment
		shr     ecx, 1
		jnz     aligned_last
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
		add     edx, 15
		jmp     unaligned_loop_entry

		align   16
	unaligned_loop:
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqu  xmmword ptr [edx], xmm0                     // store 16 bytes
		add     edx, 16
	unaligned_loop_entry:
		movdqu  xmm0, xmmword ptr [edx]                     // load 16 bytes
		movdqa  xmm1, xmm0                                  // copy
		pcmpeqw xmm3, xmm0                                  // compare 8 words with zero
		paddw   xmm0, xmm2                                  // all words greater than 'Z' if negative
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pcmpgtw xmm0, xmm4                                  // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pand    xmm0, xmm5                                  // assign a mask for the appropriate words
		test    ecx, ecx
		jz      unaligned_loop                              // next 16 bytes

		shr     ecx, 1
		jc      unaligned_epilogue
		bsf     ecx, ecx
		xor     ecx, 15
		movdqu  xmm2, xmmword ptr [maskbit + ecx]           // load the target bits mask
		pand    xmm0, xmm2                                  // assign a mask for casebit
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqu  xmmword ptr [edx], xmm0                     // store 16 bytes
	unaligned_epilogue:
		ret
	}
}

// 386 version
__declspec(naked) static wchar_t * __cdecl wcslwrGeneric(wchar_t *string)
{
	__asm
	{
		mov     ecx, 'A'
		jmp     wcslwruprGeneric
	}
}

// 386 version
__declspec(naked) static wchar_t * __cdecl wcsuprGeneric(wchar_t *string)
{
	__asm
	{
		mov     ecx, 'a'
		jmp     wcslwruprGeneric
	}
}

__declspec(naked) static wchar_t * __cdecl wcslwruprGeneric(wchar_t *string)
{
	__asm
	{
		mov     edx, dword ptr [esp + 4]                    // string
		push    ebx

		align   16
	loop_begin:
		// loop
		mov     ax, word ptr [edx]
		add     edx, 2
		test    ax, ax
		jz      return_string                               // end of string

	check_case:
		mov     bx, ax                                      // check case
		sub     ax, cx
		cmp     ax, 'Z' - 'A' + 1
		jae     loop_begin

		// convert case
		mov     ax, word ptr [edx]
		xor     bx, 'a' - 'A'
		mov     word ptr [edx - 2], bx
		add     edx, 2
		test    ax, ax
		jnz     check_case                                  // loop to next character

	return_string:
		mov     eax, dword ptr [esp + 8]                    // string
		pop     ebx
		ret
	}
}

// CPU dispatching for _wcslwr. This is executed only once
__declspec(naked) static wchar_t * __cdecl wcslwrCPUDispatch(wchar_t *string)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [wcslwrDispatch], offset wcslwrSSE42
		jmp     wcslwrSSE42

	L1:
		mov     dword ptr [wcslwrDispatch], offset wcslwrSSE2
		jb      L2
		jmp     wcslwrSSE2

	L2:
		mov     dword ptr [wcslwrDispatch], offset wcslwrGeneric
		jmp     wcslwrGeneric
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}

// CPU dispatching for _wcsupr. This is executed only once
__declspec(naked) static wchar_t * __cdecl wcsuprCPUDispatch(wchar_t *string)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [wcsuprDispatch], offset wcsuprSSE42
		jmp     wcsuprSSE42

	L1:
		mov     dword ptr [wcsuprDispatch], offset wcsuprSSE2
		jb      L2
		jmp     wcsuprSSE2

	L2:
		mov     dword ptr [wcsuprDispatch], offset wcsuprGeneric
		jmp     wcsuprGeneric
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
