#include <emmintrin.h>

extern const char xmmconst_ahighA[16];
extern const char xmmconst_alowA[16];
extern const char xmmconst_azrangeA[16];
extern const char xmmconst_casebitA[16];
extern const char xmmconst_maskbit[32];
#define ahigh   xmmconst_ahighA
#define alow    xmmconst_alowA
#define azrange xmmconst_azrangeA
#define casebit xmmconst_casebitA
#define maskbit xmmconst_maskbit

extern int __cdecl InstructionSet();

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
static char *(__cdecl * strlwrDispatch)(char *string) = strlwrCPUDispatch;
static char *(__cdecl * struprDispatch)(char *string) = struprCPUDispatch;

__declspec(naked) char * __cdecl _strlwr(char *string)
{
	__asm
	{
		jmp     dword ptr [strlwrDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

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
		movdqa  xmm1, xmmword ptr [azhigh]                  // define range A-Z
		movdqa  xmm3, xmmword ptr [casebit]                 // bit to change
		jmp     strlwruprSSE42
	}
}

// SSE4.2 version
__declspec(naked) static char * __cdecl struprSSE42(char *string)
{
	__asm
	{
		movdqa  xmm1, xmmword ptr [azlow]                   // define range a-z
		movdqa  xmm3, xmmword ptr [casebit]                 // bit to change
		jmp     strlwruprSSE42
	}
}

__declspec(naked) static char * __cdecl strlwruprSSE42(char *string)
{
	__asm
	{
		// common code for strupr and strlwr
		mov     edx, dword ptr [esp + 4]                    // string

	next:
		// loop
		movdqu  xmm2, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm1, xmm2, 01000100b                     // find bytes in range A-Z or a-z, return mask in xmm0
		jz      last                                        // string ends in this paragraph
		pand    xmm0, xmm3                                  // mask AND case bit
		pxor    xmm2, xmm0                                  // change case bit in masked bytes of string
		movdqu  xmmword ptr [edx], xmm2                     // write changed value
		add     edx, 16
		jmp     next                                        // next 16 bytes

	last:
		// Write last 0-15 bytes
		// While we can read past the end of the string if precautions are made, we cannot write
		// past the end of the string, even if the value is unchanged, because the value may have
		// been changed in the meantime by another thread
		jnc     finish                                      // nothing changed, no need to write
		pand    xmm3, xmm0                                  // mask and case bit
		pxor    xmm2, xmm3                                  // change case bit

		// less elegant alternative, but probably faster if data needed again soon
		// write 8-4-2-1 bytes, if necessary
		pmovmskb eax, xmm0                                  // create bit mask
		cmp     eax, 10000000b
		jb      L10

		// there are at least 8 bytes to write
		movq    qword ptr [edx], xmm2
		psrldq  xmm2, 8
		add     edx, 8
		shr     eax, 8

	L10:
		cmp     eax, 1000b
		jb      L20

		// there are at least 4 bytes to write
		movd    dword ptr [edx], xmm2
		psrldq  xmm2, 4
		add     edx, 4
		shr     eax, 4

	L20:
		movd    ecx, xmm2                                   // use ecx for last 3 bytes
		cmp     eax, 10b
		jb      L30

		// there are at least 2 bytes to write
		mov     word ptr [edx], cx
		shr     ecx, 16
		add     edx, 2
		shr     eax, 2

	L30:
		cmp     eax, 1
		jb      finish

		// there is one more byte to write
		mov     byte ptr [edx], cl

	finish:
		mov     eax, dword ptr [esp + 4]                    // string
		ret
	}
}

// SSE2 version
__declspec(naked) static char * __cdecl strlwrSSE2(char *string)
{
	__asm
	{
		movdqa  xmm3, xmmword ptr [ahigh]
		jmp     strlwruprSSE2
	}
}

// SSE2 version
__declspec(naked) static char * __cdecl struprSSE2(char *string)
{
	__asm
	{
		movdqa  xmm3, xmmword ptr [alow]
		jmp     strlwruprSSE2
	}
}

__declspec(naked) static char * __cdecl strlwruprSSE2(char *string)
{
	__asm
	{
		mov     eax, dword ptr [esp + 4]                    // string
		push    edi
		mov     ecx, eax
		mov     edi, eax
		movdqa  xmm4, xmmword ptr [azrange]
		pxor    xmm5, xmm5                                  // set to zero
		movdqa  xmm6, xmmword ptr [casebit]                 // bit to change
		and     ecx, 15
		jz      L2
		and     edi, -16
		xor     ecx, 15
		movdqa  xmm0, xmmword ptr [edi]                     // load 16 byte
		movdqu  xmm1, xmmword ptr [maskbit + ecx + 1]
		movdqa  xmm2, xmm0                                  // copy
		por     xmm0, xmm1                                  // fill the non target bits to 1
		movdqa  xmm1, xmm0                                  // copy
		psubb   xmm0, xmm3                                  // all bytes less than 'A'
		psubusb xmm0, xmm4                                  // and 'Z' will be reset
		pcmpeqb xmm0, xmm5                                  // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pcmpeqb xmm1, xmm5                                  // compare 16 bytes with zero
		pand    xmm0, xmm6                                  // assign a mask for the appropriate bytes
		pxor    xmm0, xmm2                                  // negation of the 5th bit - lowercase letters
		pmovmskb ecx, xmm1                                  // get one bit for each byte result
		test    ecx, ecx
		jnz     L3

		align   16
	L1:
		movdqa  xmmword ptr [edi], xmm0
		add     edi, 16
	L2:
		movdqa  xmm0, xmmword ptr [edi]                     // load 16 byte
		movdqa  xmm1, xmm0                                  // copy
		movdqa  xmm2, xmm0                                  //
		psubb   xmm0, xmm3                                  // all bytes less than 'A'
		psubusb xmm0, xmm4                                  // and 'Z' will be reset
		pcmpeqb xmm0, xmm5                                  // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pcmpeqb xmm1, xmm5                                  // compare 16 bytes with zero
		pand    xmm0, xmm6                                  // assign a mask for the appropriate bytes
		pxor    xmm0, xmm2                                  // negation of the 5th bit - lowercase letters
		pmovmskb ecx, xmm1                                  // get one bit for each byte result
		test    ecx, ecx
		jz      L1

		align   16
	L3:
		shr     ecx, 1
		jc      L4
		bsf     ecx, ecx
		xor     ecx, 15
		movdqu  xmm1, xmmword ptr [maskbit + ecx]
		maskmovdqu xmm0, xmm1
	L4:
		pop     edi
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
#if 0
	__asm
	{
		mov     edx, dword ptr [esp + 4]                    // string

	A100:
		// loop
		mov     al, byte ptr [edx]
		test    al, al
		jz      A900                                        // end of string
		sub     al, 'A'
		cmp     al, 'Z' - 'A'
		jbe     A200                                        // is upper case
		inc     edx
		jmp     A100                                        // loop to next character

	A200:
		// convert to lower case
		add     al, 'a'
		mov     byte ptr [edx], al
		inc     edx
		jmp     A100

	A900:
		ret
	}
#else
	__asm
	{
		push    ebx
		mov     edx, dword ptr [esp + 8]                    // string

		align   16
	A100:
		// loop
		mov     al, byte ptr [edx]
		inc     edx
		test    al, al
		jz      A900                                        // end of string

	A200:
		mov     bl, al                                      // check case
		sub     al, cl
		cmp     al, 'Z' - 'A'
		ja      A100

		// convert case
		mov     al, byte ptr [edx]
		xor     bl, 'a' - 'A'
		mov     byte ptr [edx - 1], bl
		inc     edx
		test    al, al
		jnz     A200                                        // loop to next character

	A900:
		mov     eax, dword ptr [esp + 8]                    // string
		pop     ebx
		ret
	}
#endif
}

// CPU dispatching for strlwr. This is executed only once
__declspec(naked) static char * __cdecl strlwrCPUDispatch(char *string)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version
		mov     ecx, offset strlwrGeneric
		cmp     eax, 4                                      // check SSE2
		jb      Q100

		// SSE2 supported
		// Point to SSE2 version
		mov     ecx, offset strlwrSSE2
		cmp     eax, 10                                     // check SSE4.2
		jb      Q100

		// SSE4.2 supported
		// Point to SSE4.2 version
		mov     ecx, offset strlwrSSE42

	Q100:
		mov     dword ptr [strlwrDispatch], ecx

		// Continue in appropriate version
		jmp     ecx
	}
}

// CPU dispatching for strupr. This is executed only once
__declspec(naked) static char * __cdecl struprCPUDispatch(char *string)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version
		mov     ecx, offset struprGeneric
		cmp     eax, 4                                      // check SSE2
		jb      Q200

		// SSE2 supported
		// Point to SSE2 version
		mov     ecx, offset struprSSE2
		cmp     eax, 10                                     // check SSE4.2
		jb      Q200

		// SSE4.2 supported
		// Point to SSE4.2 version
		mov     ecx, offset struprSSE42

	Q200:
		mov     dword ptr [struprDispatch], ecx

		// Continue in appropriate version
		jmp     ecx
	}
}
