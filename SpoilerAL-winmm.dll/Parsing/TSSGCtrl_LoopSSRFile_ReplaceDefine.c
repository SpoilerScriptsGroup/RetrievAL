#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"

extern void __stdcall ReplaceDefine(void *attributeSelector, void *line);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_ReplaceDefine()
{
	__asm
	{
		#define this  (ebp + 8H)
		#define _Str  (ebp - 110H)
		#define begin (ebp - 1F8H)
		#define _end  (ebp - 1F4H)

		mov     ecx, dword ptr [EnableParserFix]
		mov     eax, dword ptr [_end]
		test    ecx, ecx
		jz      L1
		push    eax
		lea     ecx, [_Str]
		call    string_ctor_assign_range
		mov     eax, dword ptr [this]
		lea     ecx, [_Str]
		add     eax, offset TSSGCtrl.attributeSelector
		push    ecx
		push    eax
		call    ReplaceDefine
		mov     ecx, dword ptr [_Str]
		mov     eax, dword ptr [_Str + 4]
		mov     dword ptr [begin], ecx
	L1:
		ret

		#undef this
		#undef _Str
		#undef begin
		#undef _end
	}
}

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_ReplaceDefine_Release()
{
	__asm
	{
		#define _Str (ebp - 110H)

		mov     eax, dword ptr [EnableParserFix]
		lea     ecx, [_Str]
		test    eax, eax
		jz      L1
		call    string_dtor
	L1:
		mov     word ptr [ebx + 10H], 20
		ret

		#undef _Str
	}
}
