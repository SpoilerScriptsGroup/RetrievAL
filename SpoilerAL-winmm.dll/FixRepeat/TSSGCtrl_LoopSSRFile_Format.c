#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>

void __cdecl Caller_ParsingWithVal();
void __cdecl TSSGCtrl_LoopSSRFile_LineListLoopContinue();

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_Format()
{
	__asm
	{
		#define RT_FORMAT 20H
		#define this      (ebp + 8H)
		#define LoopVal   (ebp + 14H)
		#define VIt       (ebp - 180H)
		#define tmpS      (ebp - 78H)
		#define SSGS      (ebp - 60H)
		#define _Type     edi

		cmp     _Type, RT_FORMAT
		jne     L3
		sub     esp, 256 + 8

		#define buffer      esp
		#define outReserved (esp + 256)

		mov     eax, dword ptr [LoopVal]
		mov     edx, dword ptr [VIt]
		push    eax
		add     edx, 16
		lea     ecx, [SSGS]
		mov     eax, dword ptr [this]
		push    edx
		push    ecx
		push    eax
		call    Caller_ParsingWithVal
		mov     ecx, dword ptr [VIt]
		add     esp, 16
		mov     ecx, dword ptr [ecx]
		mov     edx, buffer
		mov     dword ptr [VIt], ecx
		mov     ecx, dword ptr [ecx + 16]
		push    eax
		push    ecx
		push    256
		push    edx
		call    _snprintf
		add     esp, 16
		cmp     eax, 256
		mov     edx, buffer
		jb      L2
		test    eax, eax
		js      L1
		mov     eax, 256 - 1
		jmp     L2

	L1:
		xor     eax, eax
	L2:
		add     eax, buffer
		lea     ecx, [tmpS]
		push    eax
		call    string_append_range
		add     esp, 256 + 8
	L3:
		jmp     TSSGCtrl_LoopSSRFile_LineListLoopContinue

		#undef buffer
		#undef outReserved
	}
}

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_LineListLoopContinue()
{
	__asm
	{
		mov     ecx, 0050267CH
		mov     eax, dword ptr [VIt]
		jmp     ecx

		#undef RT_FORMAT
		#undef this
		#undef LoopVal
		#undef VIt
		#undef tmpS
		#undef SSGS
		#undef _Type
	}
}
