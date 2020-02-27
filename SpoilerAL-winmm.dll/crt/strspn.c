#include <stddef.h>

#define INVALID_PAGE 1
#if !INVALID_PAGE
extern int __cdecl InstructionSet();

static size_t __cdecl strspnSSE42(const char *string, const char *control);
static size_t __cdecl strspnGeneric(const char *string, const char *control);
static size_t __cdecl strspnCPUDispatch(const char *string, const char *control);

static size_t(__cdecl * strspnDispatch)(const char *string, const char *control) = strspnCPUDispatch;

// function dispatching
__declspec(naked) size_t __cdecl strspn(const char *string, const char *control)
{
	__asm
	{
		jmp     dword ptr [strspnDispatch]                  // Go to appropriate version, depending on instruction set
	}
}

// SSE4.2 version
__declspec(naked) static size_t __cdecl strspnSSE42(const char *string, const char *control)
{
#error Contains a bug that reads invalid page. The end of string may be on a page boundary.
	__asm
	{
		push    esi
		push    edi
		mov     esi, dword ptr [esp + 12]                   // str
		mov     edi, dword ptr [esp + 16]                   // set
		xor     ecx, ecx                                    // span counter

	str_next:
		movdqu  xmm2, xmmword ptr [esi]                     // str
		movdqu  xmm1, xmmword ptr [edi]                     // set
		pcmpistrm xmm1, xmm2, 00000000B                     // find in set, return bit mask in xmm0
		movd    eax, xmm0
		jns     set_extends

	set_finished:
		cmp     ax, -1
		jne     str_finished
		// first 16 characters matched, continue with next 16 characters (a terminating zero would never match)
		add     esi, 16                                     // next 16 bytes of str
		add     ecx, 16                                     // count span
		jmp     str_next

	str_finished:
		not     eax
		bsf     eax, eax
		add     eax, ecx
		pop     edi
		pop     esi
		ret

	set_loop:
		or      eax, edx                                    // accumulate matches

	set_extends:
		// the set is more than 16 bytes
		add     edi, 16
		movdqu  xmm1, xmmword ptr [edi]                     // next part of set
		pcmpistrm xmm1, xmm2, 00000000B                     // find in set, return bit mask in xmm0
		movd    edx, xmm0
		jns     set_loop
		mov     edi, dword ptr [esp + 16]                   // restore set pointer
		or      eax, edx                                    // accumulate matches
		jmp     set_finished
	}
}
#endif

// Generic version
#if !INVALID_PAGE
__declspec(naked) static size_t __cdecl strspnGeneric(const char *string, const char *control)
#else
__declspec(naked) size_t __cdecl strspn(const char *string, const char *control)
#endif
{
	__asm
	{
		push    esi
		push    edi
		mov     esi, dword ptr [esp + 12]                   // str pointer
	str_next10:
		mov     edi, dword ptr [esp + 16]                   // set pointer
		mov     al, dword ptr [esi]                         // read one byte from str
		test    al, al
		jz      str_finished10                              // str finished
	set_next10:
		mov     dl, dword ptr [edi]
		test    dl, dl
		jz      set_finished10
		inc     edi
		cmp     al, dl
		jne     set_next10
		// character match found, goto next character
		inc     esi
		jmp     str_next10

	str_finished10:
		// end of str, all match
	set_finished10:
		// end of set, mismatch found
		sub     esi, dword ptr [esp + 12]                   // calculate position
		mov     eax, esi
		pop     edi
		pop     esi
		ret
	}
}

#if !INVALID_PAGE
// CPU dispatching for strspn. This is executed only once
__declspec(naked) static size_t __cdecl strspnCPUDispatch(const char *string, const char *control)
{
	__asm
	{
		// get supported instruction set
		call    InstructionSet

		// Point to generic version of strspn
		mov     ecx, offset strspnGeneric

		cmp     eax, 10                                     // check SSE4.2
		jb      Q100

		// SSE4.2 supported
		// Point to SSE4.2 version of strspn
		mov     ecx, offset strspnSSE42

	Q100:
		mov     dword ptr [strspnDispatch], ecx

		// Continue in appropriate version
		jmp     ecx
	}
}
#endif
