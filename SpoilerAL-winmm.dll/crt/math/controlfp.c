#include <float.h>
#include <immintrin.h>

#pragma warning(disable:4273)

#define __ISA_AVAILABLE_X86  0
#define __ISA_AVAILABLE_SSE2 1

extern unsigned int __isa_available;

static unsigned int __fastcall GetControlFlag87(unsigned int ControlWord);
static unsigned int __fastcall SetControlFlag87(unsigned int ControlFlag);
static unsigned int __fastcall GetControlFlagSIMD(unsigned int ControlWord);
static unsigned int __fastcall SetControlFlagSIMD(unsigned int ControlFlag, unsigned int ExceptionState);

__declspec(naked) unsigned int __cdecl _controlfp(unsigned int new, unsigned int mask)
{
	__asm
	{
		#define new  (esp + 4)
		#define mask (esp + 8)
		
		push    ebx
		push    0
		xor     ecx, ecx
		fstcw   word ptr [esp]
		mov     cx, word ptr [esp]
		call    GetControlFlag87
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		je      L1
		stmxcsr dword ptr [esp]
		mov     ecx, dword ptr [esp]
		mov     ebx, eax
		call    GetControlFlagSIMD
		or      eax, ebx
	L1:
		mov     ecx, dword ptr [new + 8]
		mov     edx, dword ptr [mask + 8]
		and     ecx, edx
		xor     edx, -1
		and     eax, edx
		or      ecx, eax
		mov     ebx, ecx
		call    SetControlFlag87
		mov     eax, dword ptr [__isa_available]
		mov     ecx, ebx
		test    eax, eax
		jz      L2
		mov     edx, dword ptr [esp]
		call    SetControlFlagSIMD
	L2:
		pop     ecx
		pop     ebx
		ret

		#undef new
		#undef mask
	}
}

__declspec(naked) static unsigned int __fastcall GetControlFlag87(unsigned int ControlWord)
{
	__asm
	{
		push    ebx                         __asm   mov     eax, ecx
		and     eax, EM_INEXACT    shl  5   __asm   mov     edx, ecx
		shr     eax,  5                     __asm   mov     ebx, ecx
		shr     ecx,  3                     __asm   and     edx, MCW_RC        shl  2
		shr     edx,  2                     __asm   and     ecx, EM_UNDERFLOW
		or      eax, ecx                    __asm   mov     ecx, ebx
		or      eax, edx                    __asm   mov     edx, ebx
		shr     ecx,  1                     __asm   and     edx, EM_ZERODIVIDE shr  1
		shl     edx,  1                     __asm   and     ecx, EM_OVERFLOW
		or      eax, ecx                    __asm   mov     ecx, ebx
		or      eax, edx                    __asm   mov     edx, ebx
		shl     ecx,  4                     __asm   and     edx, MCW_IC        shr  6
		shl     edx,  6                     __asm   and     ecx, EM_INVALID
		or      eax, ecx                    __asm   mov     ecx, ebx
		or      eax, edx                    __asm   mov     edx, ebx
		shl     ecx,  8                     __asm   and     edx, EM_DENORMAL   shr 18
		shl     edx, 18                     __asm   and     ebx, 8040H
		ror     ebx, 15
		or      eax, edx                    __asm   mov     edx, ebx
		shr     ebx, 22                     __asm   xor     ecx, -1
		or      edx, ebx                    __asm   and     ecx, MCW_PC
		neg     edx
		and     edx,  3                     __asm   or      eax, ecx
		shl     edx, 24
		or      eax, edx                    __asm   pop     ebx
		ret
	}
}

__declspec(naked) static unsigned int __fastcall SetControlFlag87(unsigned int ControlFlag)
{
	__asm
	{
		push    ebx                         __asm   mov     eax, ecx
		and     eax, EM_INEXACT             __asm   mov     edx, ecx
		shl     eax,  5                     __asm   mov     ebx, edx
		shl     ecx,  3                     __asm   and     edx, MCW_RC
		shl     edx,  2                     __asm   and     ecx, EM_UNDERFLOW  shl  3
		or      eax, ecx                    __asm   mov     ecx, ebx
		or      eax, edx                    __asm   mov     edx, ebx
		shl     ecx,  1                     __asm   and     edx, EM_ZERODIVIDE
		shr     edx,  1                     __asm   and     ecx, EM_OVERFLOW   shl  1
		or      eax, ecx                    __asm   mov     ecx, ebx
		or      eax, edx                    __asm   mov     edx, ebx
		shr     ecx,  4                     __asm   and     edx, MCW_IC
		shr     edx,  6                     __asm   and     ecx, EM_INVALID    shr  4
		or      eax, ecx                    __asm   mov     ecx, ebx
		or      eax, edx                    __asm   mov     edx, ebx
		shr     ecx,  8                     __asm   and     edx, EM_DENORMAL
		shr     edx, 18                     __asm   xor     ecx, -1
		shr     ebx, 24                     __asm   and     ecx, MCW_PC        shr  8
		xor     ebx, -1                     __asm   or      eax, ecx
		inc     ebx                         __asm   or      eax, edx
		mov     ecx, ebx                    __asm   and     ebx, 40H           shr  5
		shl     ebx,  5                     __asm   and     ecx, 8000H         shr 15
		shl     ecx, 15                     __asm   or      eax, ebx
		or      eax, ecx
		push    eax
		fldcw   word ptr [esp]
		pop     eax                         __asm   pop     ebx
		ret
	}
}

__declspec(naked) static unsigned int __fastcall GetControlFlagSIMD(unsigned int ControlWord)
{
	__asm
	{
		push    ebx                         __asm   mov     eax, ecx
		shr     eax,  7                     __asm   mov     ebx, ecx
		shr     ebx,  5                     __asm   and     ecx, 8040H
		ror     ecx, 15
		mov     edx, ecx                    __asm   and     eax, MCW_EM
		shr     edx, 22                     __asm   and     ebx, MCW_RC
		or      ecx, edx                    __asm   or      eax, ebx
		neg     ecx
		and     ecx,  3
		shl     ecx, 24
		or      eax, ecx                    __asm   pop     ebx
		ret
	}
}

__declspec(naked) static unsigned int __fastcall SetControlFlagSIMD(unsigned int ControlFlag, unsigned int ExceptionState)
{
	__asm
	{
		push    ebx                         __asm   mov     eax, ecx
		shl     eax,  7                     __asm   mov     ebx, ecx
		shl     ebx,  5                     __asm   and     ecx, _MCW_DN
		shr     ecx, 24                     __asm   and     eax, _MM_MASK_MASK
		xor     ecx, -1                     __asm   and     edx, _MM_EXCEPT_MASK
		inc     ecx                         __asm   or      eax, edx
		mov     edx, ecx                    __asm   and     ebx, _MM_ROUND_MASK
		shl     edx,  5                     __asm   and     ecx, 8000H shr 15
		shl     ecx, 15                     __asm   and     edx, 40H
		or      eax, ebx                    __asm   or      ecx, edx
		or      eax, ecx
		push    eax
		ldmxcsr dword ptr [esp]
		pop     eax                         __asm   pop     ebx
		ret
	}
}
