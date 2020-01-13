#include <float.h>
#include <immintrin.h>

#pragma warning(disable:4273)

#define __ISA_AVAILABLE_X86  0
#define __ISA_AVAILABLE_SSE2 1

extern unsigned int __isa_available;

static unsigned int __fastcall GetStatusFlag(unsigned int StatusWord);

__declspec(naked) unsigned int __cdecl _statusfp()
{
	__asm
	{
		xor     ecx, ecx
		fstsw   ax
		mov     cx, ax
		call    GetStatusFlag
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		je      L1
		push    eax
		push    0
		stmxcsr dword ptr [esp]
		pop     ecx
		call    GetStatusFlag
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
		call    GetStatusFlag
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		je      L1
		push    eax
		push    0
		stmxcsr dword ptr [esp]
		pop     ecx
		and     ecx, not _MM_EXCEPT_MASK
		push    ecx
		ldmxcsr dword ptr [esp]
		call    GetStatusFlag
		pop     edx
		pop     ecx
		or      eax, ecx
	L1:
		ret
	}
}

__declspec(naked) static unsigned int __fastcall GetStatusFlag(unsigned int StatusWord)
{
	#if (SW_INEXACT    <<  5) != _MM_EXCEPT_INEXACT   || \
	    (SW_UNDERFLOW  <<  3) != _MM_EXCEPT_UNDERFLOW || \
	    (SW_OVERFLOW   <<  1) != _MM_EXCEPT_OVERFLOW  || \
	    (SW_ZERODIVIDE >>  1) != _MM_EXCEPT_DIV_ZERO  || \
	    (SW_INVALID    >>  4) != _MM_EXCEPT_INVALID   || \
	    (SW_DENORMAL   >> 18) != _MM_EXCEPT_DENORM
	#error Unmatch FPU status word definitions.
	#endif

	__asm
	{
		push    ebx                         __asm   mov     eax, ecx
		and     eax, SW_INEXACT    shl  5   __asm   mov     ebx, ecx
		shr     eax,  5                     __asm   mov     edx, ebx
		shr     ecx,  3                     __asm   and     edx, SW_OVERFLOW   shl  1
		shr     edx,  1                     __asm   and     ecx, SW_UNDERFLOW
		or      eax, ecx                    __asm   mov     ecx, ebx
		or      eax, edx                    __asm   mov     edx, ebx
		shl     ecx,  1                     __asm   and     edx, SW_INVALID    shr  4
		shl     edx,  4                     __asm   and     ecx, SW_ZERODIVIDE
		and     ebx, SW_DENORMAL   shr 18   __asm   or      eax, ecx
		shl     ebx, 18                     __asm   or      eax, edx
		or      eax, ebx                    __asm   pop     ebx
		ret
	}
}
