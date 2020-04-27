#include <string.h>

#define INVALID_PAGE 0

#if INVALID_PAGE
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
static char * __cdecl strlwrGeneric(char *string);
static char * __cdecl struprGeneric(char *string);
static char * __cdecl strlwrCPUDispatch(char *string);
static char * __cdecl struprCPUDispatch(char *string);

// Pointer to appropriate version. Initially points to dispatcher
static char *(__cdecl *strlwrDispatch)(char *string) = strlwrCPUDispatch;
static char *(__cdecl *struprDispatch)(char *string) = struprCPUDispatch;

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
#error Contains a bug that reads invalid page. The end of string may be on a page boundary.
	__asm
	{
		// common code for strupr and strlwr
		mov     edx, dword ptr [esp + 4]                    // string

	next:
		// loop
		movdqu  xmm2, xmmword ptr [edx]                     // read 16 bytes from string
		pcmpistrm xmm1, xmm2, 01000100B                     // find bytes in range A-Z or a-z, return mask in xmm0
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
		cmp     eax, 10000000B
		jb      L10

		// there are at least 8 bytes to write
		movq    qword ptr [edx], xmm2
		psrldq  xmm2, 8
		add     edx, 8
		shr     eax, 8

	L10:
		cmp     eax, 1000B
		jb      L20

		// there are at least 4 bytes to write
		movd    dword ptr [edx], xmm2
		psrldq  xmm2, 4
		add     edx, 4
		shr     eax, 4

	L20:
		movd    ecx, xmm2                                   // use ecx for last 3 bytes
		cmp     eax, 10B
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
#endif

// 386 version
#if INVALID_PAGE
__declspec(naked) static char * __cdecl strlwrGeneric(char *string)
#else
__declspec(naked) char * __cdecl _strlwr(char *string)
#endif
{
	__asm
	{
		mov     edx, dword ptr [esp + 4]                    // string

		// loop
	A100:
		mov     al, byte ptr [edx]
		test    al, al
		jz      A900                                        // end of string
		sub     al, 'A'
		cmp     al, 'Z' - 'A' + 1
		jb      A200                                        // is upper case
		inc     edx
		jmp     A100                                        // loop to next character

		// convert to lower case
	A200:
		add     al, 'a'
		mov     byte ptr [edx], al
		inc     edx
		jmp     A100

	A900:
		mov     eax, dword ptr [esp + 4]                    // string
		ret
	}
}

// 386 version
#if INVALID_PAGE
__declspec(naked) static char * __cdecl struprGeneric(char *string)
#else
__declspec(naked) char * __cdecl _strupr(char *string)
#endif
{
	__asm
	{
		mov     edx, dword ptr [esp + 4]                    // string

		// loop
	B100:
		mov     al, byte ptr [edx]
		test    al, al
		jz      B900                                        // end of string
		sub     al, 'a'
		cmp     al, 'z' - 'a' + 1
		jb      B200                                        // is lower case
		inc     edx
		jmp     B100                                        // loop to next character

		// convert to upper case
	B200:
		add     al, 'A'
		mov     byte ptr [edx], al
		inc     edx
		jmp     B100

	B900:
		mov     eax, dword ptr [esp + 4]                    // string
		ret
	}
}

#if INVALID_PAGE
// CPU dispatching for strlwr. This is executed only once
__declspec(naked) static char * __cdecl strlwrCPUDispatch(char *string)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version
		mov     ecx, offset strlwrGeneric
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
#endif
