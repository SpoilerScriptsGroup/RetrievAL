#ifndef _M_IX86
char * __cdecl _strlwr(char *string)
{
	char *p, c;

	if (c = *(p = string))
		do
			if ((unsigned char)(c - 'A') < 'Z' - 'A' + 1)
				*p = c + ('a' - 'A');
		while (c = *(++p));
	return string;
}

char * __cdecl _strupr(char *string)
{
	char *p, c;

	if (c = *(p = string))
		do
			if ((unsigned char)(c - 'a') < 'z' - 'a' + 1)
				*p = c - ('a' - 'A');
		while (c = *(++p));
	return string;
}
#else
extern const char xmmconst_upperA[16];
extern const char xmmconst_lowerA[16];
extern const char xmmconst_azrangeA[16];
extern const char xmmconst_casebitA[16];
extern const char xmmconst_maskbit[32];
#define upper   xmmconst_upperA
#define lower   xmmconst_lowerA
#define azrange xmmconst_azrangeA
#define casebit xmmconst_casebitA
#define maskbit xmmconst_maskbit

__declspec(align(16)) static const char azhigh[16] = {      // define range for upper case
	'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z', 'A', 'Z'
};
__declspec(align(16)) static const char azlow[16] = {       // define range for lower case
	'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z', 'a', 'z'
};

static char * __cdecl strlwrSSE42(char *string);
static char * __cdecl struprSSE42(char *string);
static char * __cdecl strlwruprSSE42(char *string);
static char * __cdecl strlwrSSE2(char *string);
static char * __cdecl struprSSE2(char *string);
static char * __cdecl strlwruprSSE2(char *string);
static char * __cdecl strlwrGeneric(char *string);
static char * __cdecl struprGeneric(char *string);
static char * __cdecl strlwruprGeneric(char *string);
static char * __cdecl strlwrCPUDispatch(char *string);
static char * __cdecl struprCPUDispatch(char *string);

// Pointer to appropriate version. Initially points to dispatcher
static char *(__cdecl *strlwrDispatch)(char *string) = strlwrCPUDispatch;
static char *(__cdecl *struprDispatch)(char *string) = struprCPUDispatch;

// _strlwr function
__declspec(naked) char * __cdecl _strlwr(char *string)
{
	__asm
	{
		jmp     dword ptr [strlwrDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// _strupr function
__declspec(naked) char * __cdecl _strupr(char *string)
{
	__asm
	{
		jmp     dword ptr [struprDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl strlwrSSE42(char *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [azhigh]                  // define range A-Z
		jmp     strlwruprSSE42
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl struprSSE42(char *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [azlow]                   // define range a-z
		jmp     strlwruprSSE42
	}
}

__declspec(naked) static char * __cdecl strlwruprSSE42(char *string)
{
	__asm
	{
		// common code for strupr and strlwr
		mov     ecx, dword ptr [esp + 4]                    // string
		movdqa  xmm3, xmmword ptr [casebit]                 // bit to change
		mov     eax, ecx
		mov     edx, ecx
		and     ecx, 15
		jz      loop_entry
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm4, xmmword ptr [maskbit + ecx + 1]
		movdqa  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		por     xmm4, xmm1                                  // fill the non target bits to 1
		pcmpistrm xmm2, xmm4, 01000100B                     // find bytes in range A-Z or a-z, return mask in xmm0
		jz      last                                        // string ends in this paragraph

		align   16
	loop_begin:
		// loop
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
		add     edx, 16
	loop_entry:
		movdqa  xmm1, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm2, xmm1, 01000100B                     // find bytes in range A-Z or a-z, return mask in xmm0
		jnz     loop_begin                                  // next 16 bytes

	last:
		// Write last 0-15 bytes
		// While we can read past the end of the string if precautions are made, we cannot write
		// past the end of the string, even if the value is unchanged, because the value may have
		// been changed in the meantime by another thread
		jnc     finish                                      // nothing changed, no need to write
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm0, xmm1                                  // change case bit in masked bytes of string
		movdqa  xmmword ptr [edx], xmm0                     // write changed value
	finish:
		ret
	}
}

// SSE2 version
__declspec(naked) static char * __cdecl strlwrSSE2(char *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [upper]
		jmp     strlwruprSSE2
	}
}

// SSE2 version
__declspec(naked) static char * __cdecl struprSSE2(char *string)
{
	__asm
	{
		movdqa  xmm2, xmmword ptr [lower]
		jmp     strlwruprSSE2
	}
}

__declspec(naked) static char * __cdecl strlwruprSSE2(char *string)
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
		jz      loop_entry1
		xor     ecx, 15
		and     edx, -16
		movdqu  xmm0, xmmword ptr [maskbit + ecx + 1]       // load the non target bits mask
		movdqa  xmm1, xmmword ptr [edx]                     // load 16 bytes
		por     xmm0, xmm1                                  // fill the non target bits to 1
		jmp     loop_entry2

		align   16
	loop_begin:
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
		add     edx, 16
	loop_entry1:
		movdqa  xmm0, xmmword ptr [edx]                     // load 16 bytes
		movdqa  xmm1, xmm0                                  // copy
	loop_entry2:
		pcmpeqb xmm3, xmm0                                  // compare 16 bytes with zero
		paddb   xmm0, xmm2                                  // all bytes greater than 'Z' if negative
		pmovmskb ecx, xmm3                                  // get one bit for each byte result
		pcmpgtb xmm0, xmm4                                  // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pand    xmm0, xmm5                                  // assign a mask for the appropriate bytes
		test    ecx, ecx
		jz      loop_begin                                  // next 16 bytes

		shr     ecx, 1
		jc      epilogue
		bsf     ecx, ecx
		xor     ecx, 15
		movdqu  xmm2, xmmword ptr [maskbit + ecx]           // load the target bits mask
		pand    xmm0, xmm2                                  // assign a mask for casebit
		pxor    xmm0, xmm1                                  // negation of the 5th bit - lowercase letters
		movdqa  xmmword ptr [edx], xmm0                     // store 16 bytes
	epilogue:
		ret
	}
}

// 386 version
__declspec(naked) static char * __cdecl strlwrGeneric(char *string)
{
	__asm
	{
		mov     ecx, 'A'
		jmp     strlwruprGeneric
	}
}

// 386 version
__declspec(naked) static char * __cdecl struprGeneric(char *string)
{
	__asm
	{
		mov     ecx, 'a'
		jmp     strlwruprGeneric
	}
}

__declspec(naked) static char * __cdecl strlwruprGeneric(char *string)
{
	__asm
	{
		push    ebx
		mov     edx, dword ptr [esp + 8]                    // string

		// loop
		align   16
	loop_begin:
		mov     al, byte ptr [edx]
		inc     edx
		test    al, al
		jz      return_string                               // end of string

	check_case:
		mov     bl, al                                      // check case
		sub     al, cl
		cmp     al, 'Z' - 'A' + 1
		jae     loop_begin

		// convert case
		mov     al, byte ptr [edx]
		xor     bl, 'a' - 'A'
		mov     byte ptr [edx - 1], bl
		inc     edx
		test    al, al
		jnz     check_case                                  // loop to next character

	return_string:
		mov     eax, dword ptr [esp + 8]                    // string
		pop     ebx
		ret
	}
}

// CPU dispatching for _strlwr. This is executed only once
__declspec(naked) static char * __cdecl strlwrCPUDispatch(char *string)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [strlwrDispatch], offset strlwrSSE42
		jmp     strlwrSSE42

	L1:
		mov     dword ptr [strlwrDispatch], offset strlwrSSE2
		jb      L2
		jmp     strlwrSSE2

	L2:
		mov     dword ptr [strlwrDispatch], offset strlwrGeneric
		jmp     strlwrGeneric
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}

// CPU dispatching for _strupr. This is executed only once
__declspec(naked) static char * __cdecl struprCPUDispatch(char *string)
{
	#define __ISA_AVAILABLE_X86   0
	#define __ISA_AVAILABLE_SSE2  1
	#define __ISA_AVAILABLE_SSE42 2

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_SSE2
		jbe     L1
		mov     dword ptr [struprDispatch], offset struprSSE42
		jmp     struprSSE42

	L1:
		mov     dword ptr [struprDispatch], offset struprSSE2
		jb      L2
		jmp     struprSSE2

	L2:
		mov     dword ptr [struprDispatch], offset struprGeneric
		jmp     struprGeneric
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
	#undef __ISA_AVAILABLE_SSE42
}
#endif
