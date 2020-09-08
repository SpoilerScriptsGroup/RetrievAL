#include <float.h>
#include <immintrin.h>

#pragma warning(disable:4273)

#define __ISA_AVAILABLE_X86  0
#define __ISA_AVAILABLE_SSE2 1

extern unsigned int __isa_available;

static unsigned int __fastcall ToStatusFlag(unsigned int StatusWord);

__declspec(naked) unsigned int __cdecl _statusfp()
{
	__asm
	{
		xor     ecx, ecx
		fstsw   ax
		mov     cx, ax
		call    ToStatusFlag
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		je      L1
		push    eax
		push    0
		stmxcsr dword ptr [esp]
		pop     ecx
		call    ToStatusFlag
		pop     ecx
		or      eax, ecx
	L1:
		ret
	}
}

__declspec(naked) unsigned int __cdecl _clearfp()
{
	__asm
	{
		xor     ecx, ecx
		fnstsw  ax
		fnclex
		mov     cx, ax
		call    ToStatusFlag
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		je      L1
		push    eax
		push    0
		stmxcsr dword ptr [esp]
		pop     ecx
		and     ecx, not _MM_EXCEPT_MASK
		push    ecx
		ldmxcsr dword ptr [esp]
		call    ToStatusFlag
		pop     edx
		pop     ecx
		or      eax, ecx
	L1:
		ret
	}
}

#if SW_INEXACT    <<  5 != _MM_EXCEPT_INEXACT   || \
    SW_UNDERFLOW  <<  3 != _MM_EXCEPT_UNDERFLOW || \
    SW_OVERFLOW   <<  1 != _MM_EXCEPT_OVERFLOW  || \
    SW_ZERODIVIDE >>  1 != _MM_EXCEPT_DIV_ZERO  || \
    SW_INVALID    >>  4 != _MM_EXCEPT_INVALID   || \
    SW_DENORMAL   >> 18 != _MM_EXCEPT_DENORM
#error "Unmatch FPU status word definitions."
#endif

__declspec(naked) static unsigned int __fastcall ToStatusFlag(unsigned int StatusWord)
{
	__asm
	{
		push    esi                 __asm   mov     eax, ecx
		shr     eax,  5             __asm   mov     edx, ecx
		shr     ecx,  3             __asm   mov     esi, edx
		shr     edx,  1             __asm   and     eax, SW_INEXACT
		and     ecx, SW_UNDERFLOW   __asm   and     edx, SW_OVERFLOW
		or      eax, ecx            __asm   mov     ecx, esi
		or      eax, edx            __asm   mov     edx, esi
		shl     ecx,  1             __asm   and     esi, SW_DENORMAL   shr 18
		shl     edx,  4             __asm   and     ecx, SW_ZERODIVIDE
		shl     esi, 18             __asm   and     edx, SW_INVALID
		or      eax, ecx            __asm   or      edx, esi
		or      eax, edx            __asm   pop     esi
		ret
	}
}
