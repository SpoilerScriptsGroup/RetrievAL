#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

void __stdcall ReplaceDefineDynamic(void *SSGS, void *line);

extern BOOL EnableParserFix;

__declspec(naked) void __cdecl TSSGCtrl_Funneling_ReplaceDefineDynamic()
{
	__asm
	{
		#define Src  eax
		#define SSGS (ebp + 0CH)

		cmp     dword ptr [EnableParserFix], 0
		je      L1
		sub     esp, 24
		mov     edx, Src
		mov     ecx, esp
		call    string_ctor_assign
		mov     eax, dword ptr [SSGS]
		push    esp
		push    eax
		call    ReplaceDefineDynamic
		mov     eax, dword ptr [esp + 24 + 40]
		mov     ecx, dword ptr [esp + 24 + 36]
		push    eax
		push    ecx
		mov     eax, dword ptr [esp + 24 + 40]
		mov     ecx, dword ptr [esp + 24 + 36]
		push    eax
		push    ecx
		mov     eax, dword ptr [esp + 24 + 40]
		mov     ecx, dword ptr [esp + 24 + 36]
		push    eax
		push    ecx
		mov     eax, dword ptr [esp + 24 + 40]
		mov     ecx, dword ptr [esp + 24 + 36]
		push    eax
		push    ecx
		mov     ecx, dword ptr [esp + 24 + 36]
		lea     eax, [esp + 32]
		push    eax
		push    ecx
		call    TStringDivision_List
		add     esp, 40
		mov     ecx, esp
		push    eax
		call    string_dtor
		mov     eax, dword ptr [esp]
		add     esp, 4 + 24
		ret

		align   16
	L1:
		jmp     TStringDivision_List

		#undef Src
		#undef SSGS
	}
}
