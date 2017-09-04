#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

void __stdcall ReplaceDefine(void *attributeSelector, void *line);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_ReplaceDefine()
{
	__asm
	{
		#define TStringDivision_ToULongDef          004AE6C0H
		#define this                                edi
		#define offsetof_TSSGCtrl_attributeSelector 32
		#define VIt                                 esi
		#define tmpS                                (ebp - 68H)

		mov     eax, dword ptr [EnableParserFix]
		mov     ecx, TStringDivision_ToULongDef
		test    eax, eax
		jz      L2
		mov     edx, VIt
		lea     ecx, [tmpS]
		call    string_ctor_assign
		lea     eax, [tmpS]
		push    eax
		lea     eax, [this + offsetof_TSSGCtrl_attributeSelector]
		push    eax
		call    ReplaceDefine
		push    eax
		mov     eax, dword ptr [tmpS]
		mov     ecx, esp
		push    0
		push    ecx
		push    eax
		call    strtoul
		add     esp, 12
		mov     ecx, dword ptr [tmpS]
		pop     edx
		cmp     ecx, edx
		jne     L1
		mov     dword ptr [esp + 8], 004FF399H
		add     esp, 8
	L1:
		push    eax
		lea     ecx, [tmpS]
		call    string_dtor
		pop     eax
		ret

		align   16
	L2:
		jmp     ecx

		#undef TStringDivision_ToULongDef
		#undef this
		#undef offsetof_TSSGCtrl_attributeSelector
		#undef VIt
		#undef tmpS
	}
}
