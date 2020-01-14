#include <float.h>
#include <immintrin.h>

#pragma warning(disable:4273)

#define __ISA_AVAILABLE_X86  0
#define __ISA_AVAILABLE_SSE2 1

extern unsigned int __isa_available;

static unsigned int __fastcall ToControlFlag87(unsigned int ControlWord);
static unsigned int __fastcall ToControlWord87(unsigned int ControlFlag);
static unsigned int __fastcall ToControlFlagSIMD(unsigned int ControlWord);
static unsigned int __fastcall ToControlWordSIMD(unsigned int ControlFlag, unsigned int ExceptionState);

__declspec(naked) unsigned int __cdecl _controlfp(unsigned int new, unsigned int mask)
{
	__asm
	{
		#define new  (esp + 4)
		#define mask (esp + 8)

		push    esi
		push    edi
		push    0
		xor     ecx, ecx
		fstcw   word ptr [esp]
		mov     cx, word ptr [esp]
		call    ToControlFlag87
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		je      L1
		stmxcsr dword ptr [esp]
		mov     ecx, dword ptr [esp]
		mov     esi, eax
		mov     edi, ecx
		call    ToControlFlagSIMD
		or      eax, esi
	L1:
		mov     ecx, dword ptr [new + 12]
		mov     edx, dword ptr [mask + 12]
		and     ecx, edx
		xor     edx, -1
		and     eax, edx
		or      ecx, eax
		mov     esi, ecx
		call    ToControlWord87
		mov     dword ptr [esp], eax
		fldcw   word ptr [esp]
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		je      L2
		mov     ecx, esi
		mov     edx, edi
		call    ToControlWordSIMD
		mov     dword ptr [esp], eax
		ldmxcsr dword ptr [esp]
	L2:
		pop     ecx
		pop     edi
		pop     esi
		ret

		#undef new
		#undef mask
	}
}

#if EM_INEXACT    <<  5 != 0x0020 || \
    EM_UNDERFLOW  <<  3 != 0x0010 || \
    EM_OVERFLOW   <<  1 != 0x0008 || \
    EM_ZERODIVIDE >>  1 != 0x0004 || \
    EM_INVALID    >>  4 != 0x0001 || \
    EM_DENORMAL   >> 18 != 0x0002 || \
    RC_NEAR       <<  2 != 0x0000 || \
    RC_DOWN       <<  2 != 0x0400 || \
    RC_UP         <<  2 != 0x0800 || \
    RC_CHOP       <<  2 != 0x0C00 || \
    IC_AFFINE     >>  6 != 0x1000
#error Unmatch FPU control word definitions.
#endif

#if (PC_64 ^ MCW_PC) >> 8 != 0x0300 || \
    (PC_53 ^ MCW_PC) >> 8 != 0x0200 || \
    (PC_24 ^ MCW_PC) >> 8 != 0x0100
#error Unmatch FPU control word definitions.
#endif

#if (-_DN_SAVE                        >> 19) & 0x0040 != 0x0000 || \
    (-_DN_FLUSH                       >> 19) & 0x0040 != 0x0040 || \
    (-_DN_FLUSH_OPERANDS_SAVE_RESULTS >> 19) & 0x0040 != 0x0040 || \
    (-_DN_SAVE_OPERANDS_FLUSH_RESULTS >> 19) & 0x0040 != 0x0000 || \
    (-_DN_SAVE                        >>  9) & 0x8000 != 0x0000 || \
    (-_DN_FLUSH                       >>  9) & 0x8000 != 0x8000 || \
    (-_DN_FLUSH_OPERANDS_SAVE_RESULTS >>  9) & 0x8000 != 0x0000 || \
    (-_DN_SAVE_OPERANDS_FLUSH_RESULTS >>  9) & 0x8000 != 0x8000
#error Unmatch FPU control word definitions.
#endif

__declspec(naked) static unsigned int __fastcall ToControlFlag87(unsigned int ControlWord)
{
	__asm
	{
		push    esi                         __asm   mov     eax, ecx
		shr     eax,  5                     __asm   mov     edx, ecx
		shr     ecx,  3                     __asm   mov     esi, edx
		shr     edx,  2                     __asm   and     eax, EM_INEXACT
		and     ecx, EM_UNDERFLOW           __asm   and     edx, MCW_RC
		or      eax, ecx                    __asm   mov     ecx, esi
		or      eax, edx                    __asm   mov     edx, esi
		shr     ecx,  1                     __asm   and     edx, EM_ZERODIVIDE shr  1
		shl     edx,  1                     __asm   and     ecx, EM_OVERFLOW
		or      eax, ecx                    __asm   mov     ecx, esi
		or      eax, edx                    __asm   mov     edx, esi
		shl     ecx,  4                     __asm   and     edx, MCW_IC        shr  6
		shl     edx,  6                     __asm   and     ecx, EM_INVALID
		or      eax, ecx                    __asm   mov     ecx, esi
		or      eax, edx                    __asm   mov     edx, esi
		shl     edx, 18                     __asm   and     ecx, 8040H
		shl     ecx,  9                     __asm   and     edx, EM_DENORMAL
		or      eax, edx                    __asm   mov     edx, ecx
		shl     edx, 10                     __asm   and     esi, MCW_PC shr 8
		shl     esi,  8                     __asm   or      ecx, edx
		xor     esi, MCW_PC                 __asm   xor     ecx, -1
		or      eax, esi                    __asm   add     ecx, _DN_FLUSH
		and     ecx, _MCW_DN
		or      eax, ecx                    __asm   pop     esi
		ret
	}
}

__declspec(naked) static unsigned int __fastcall ToControlWord87(unsigned int ControlFlag)
{
	__asm
	{
		push    esi                         __asm   mov     eax, ecx
		and     eax, EM_INEXACT             __asm   mov     edx, ecx
		shl     eax,  5                     __asm   mov     esi, edx
		shl     ecx,  3                     __asm   and     edx, MCW_RC
		shl     edx,  2                     __asm   and     ecx, EM_UNDERFLOW  shl  3
		or      eax, ecx                    __asm   mov     ecx, esi
		or      eax, edx                    __asm   mov     edx, esi
		shl     ecx,  1                     __asm   and     edx, EM_ZERODIVIDE
		shr     edx,  1                     __asm   and     ecx, EM_OVERFLOW   shl  1
		or      eax, ecx                    __asm   mov     ecx, esi
		or      eax, edx                    __asm   mov     edx, esi
		shr     ecx,  4                     __asm   and     edx, MCW_IC
		shr     edx,  6                     __asm   and     ecx, EM_INVALID    shr  4
		or      eax, ecx                    __asm   mov     ecx, esi
		or      eax, edx                    __asm   mov     edx, esi
		shr     esi,  8                     __asm   xor     ecx, -1
		shr     edx, 18                     __asm   add     ecx, _DN_FLUSH
		shr     ecx,  9                     __asm   and     edx, EM_DENORMAL   shr 18
		or      eax, edx                    __asm   mov     edx, ecx
		shr     edx, 10                     __asm   xor     esi, -1
		and     ecx, 8000H                  __asm   and     esi, MCW_PC        shr  8
		and     edx, 40H                    __asm   or      eax, esi
		or      eax, ecx
		or      eax, edx                    __asm   pop     esi
		ret
	}
}

#if EM_INEXACT    << 12 != _MM_MASK_INEXACT      || \
    EM_UNDERFLOW  << 10 != _MM_MASK_UNDERFLOW    || \
    EM_OVERFLOW   <<  8 != _MM_MASK_OVERFLOW     || \
    EM_ZERODIVIDE <<  6 != _MM_MASK_DIV_ZERO     || \
    EM_INVALID    <<  3 != _MM_MASK_INVALID      || \
    EM_DENORMAL   >> 11 != _MM_MASK_DENORM       || \
    RC_NEAR       <<  5 != _MM_ROUND_NEAREST     || \
    RC_DOWN       <<  5 != _MM_ROUND_DOWN        || \
    RC_UP         <<  5 != _MM_ROUND_UP          || \
    RC_CHOP       <<  5 != _MM_ROUND_TOWARD_ZERO
#error Unmatch FPU control word definitions.
#endif

__declspec(naked) static unsigned int __fastcall ToControlFlagSIMD(unsigned int ControlWord)
{
	__asm
	{
		push    esi                         __asm   push    edi
		mov     eax, ecx                    __asm   mov     edx, ecx
		shr     eax, 12                     __asm   mov     esi, ecx
		shr     ecx, 10                     __asm   mov     edi, edx
		shr     edx,  8                     __asm   and     eax, EM_INEXACT
		shr     esi,  6                     __asm   and     ecx, EM_UNDERFLOW
		and     edx, EM_OVERFLOW            __asm   and     esi, EM_ZERODIVIDE
		or      eax, ecx                    __asm   mov     ecx, edi
		or      eax, edx                    __asm   mov     edx, edi
		or      eax, esi                    __asm   mov     esi, edi
		shr     edx,  3                     __asm   and     ecx, 8040H
		shl     ecx,  9                     __asm   and     edx, EM_INVALID
		or      eax, edx                    __asm   mov     edx, ecx
		shl     edx, 10                     __asm   and     esi, EM_DENORMAL   shr 11
		or      ecx, edx                    __asm   and     edi, MCW_RC        shl  5
		shl     esi, 11                     __asm   xor     ecx, -1
		shr     edi,  5                     __asm   add     ecx, _DN_FLUSH
		or      eax, esi                    __asm   and     ecx, _MCW_DN
		or      eax, edi                    __asm   pop     edi
		or      eax, ecx                    __asm   pop     esi
		ret
	}
}

__declspec(naked) static unsigned int __fastcall ToControlWordSIMD(unsigned int ControlFlag, unsigned int ExceptionState)
{
	__asm
	{
		push    esi                         __asm   push    edi
		mov     eax, ecx                    __asm   mov     edx, ecx
		mov     esi, ecx                    __asm   mov     edi, ecx
		and     eax, EM_INEXACT             __asm   and     ecx, EM_UNDERFLOW
		shl     eax, 12                     __asm   and     edx, EM_OVERFLOW
		shl     ecx, 10                     __asm   and     esi, EM_ZERODIVIDE
		shl     edx,  8                     __asm   or      eax, ecx
		shl     esi,  6                     __asm   or      eax, edx
		or      eax, esi                    __asm   mov     ecx, edi
		mov     edx, edi                    __asm   mov     esi, edi
		xor     ecx, -1                     __asm   and     edx, EM_INVALID
		shl     edx,  3                     __asm   add     ecx, _DN_FLUSH
		shr     ecx,  9                     __asm   or      eax, edx
		mov     edx, ecx                    __asm   and     esi, EM_DENORMAL
		shr     edx, 10                     __asm   and     edi, MCW_RC
		shr     esi, 11                     __asm   and     ecx, 8000H
		shl     edi,  5                     __asm   and     edx, 40H
		or      ecx, esi                    __asm   or      edx, edi
		or      eax, ecx                    __asm   pop     edi
		or      eax, edx                    __asm   pop     esi
		ret
	}
}
