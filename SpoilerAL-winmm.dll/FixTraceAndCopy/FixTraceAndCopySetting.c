#define USING_NAMESPACE_BCB6_GLOBAL
#include "bcb6_global_operator.h"

#pragma warning(disable:4733)

extern BOOL EnableParserFix;
extern const DWORD F005D54CC;
extern const DWORD F004FE200;

static void __stdcall FixTraceAndCopySetting(void *_this, void *SSGC);

__declspec(naked) void __cdecl FixCopySetting()
{
	__asm
	{
		#define SSGC                      (ebp + 0CH)
		#define _this                     ebx
		#define _vector_string_deallocate 00415F90H

		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, _vector_string_deallocate
		test    eax, eax
		jz      L1
	    push    dword ptr [SSGC]
	    push    _this
		push    ecx
		jmp     FixTraceAndCopySetting
	L1:
		jmp     ecx

		#undef SSGC
		#undef _this
		#undef _vector_string_deallocate
	}
}

__declspec(naked) void __cdecl FixTraceSetting()
{
	__asm
	{
		#define SSGC                      (ebp + 0CH)
		#define _this                     ebx
		#define _vector_string_deallocate 00415F90H

		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, _vector_string_deallocate
		test    eax, eax
		jz      L1
	    push    dword ptr [SSGC]
	    push    _this
		push    ecx
		jmp     FixTraceAndCopySetting
	L1:
		jmp     ecx

		#undef SSGC
		#undef _this
		#undef _vector_string_deallocate
	}
}

__declspec(naked) static void __stdcall FixTraceAndCopySetting(void *_this, void *SSGC)
{
	__asm
	{
		push    ebp
		mov     eax, 006164C4H
		mov     ebp, esp
		sub     esp, 40
		call    dword ptr [F005D54CC]
		push    12
		call    dword ptr [operator_new]
		test    eax, eax
		jz      L1
		mov     dword ptr [eax], 00615314H
		mov     dword ptr [eax + 4H], 3
		mov     byte ptr [eax + 8H], 1
	L1:
		mov     ecx, dword ptr [ebp + 8H]
		mov     dword ptr [esp], eax
		push    ecx
		mov     ecx, dword ptr [ebp + 0CH]
		push    1
		push    ecx
		call    dword ptr [F004FE200]
		mov     ecx, dword ptr [ebp - 40]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
		ret     8
	}
}
