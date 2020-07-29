#define USING_NAMESPACE_BCB6
#define USING_NAMESPACE_BCB6_STD
#include "TSSCalc.h"
#include "bcb6_operator.h"
#include "SSGSubjectProperty.h"

#define MakeSubjectClass_SwitchSubjectTypeTable 0x004EC41F

__declspec(naked) TSSCalc * __cdecl new_TSSCalc()
{
	extern const DWORD __InitExceptBlockLDTC;

	__asm
	{
		push    ebp
		mov     eax, 006354ACH
		mov     ebp, esp
		sub     esp, 136
		push    ebx
		push    esi
		mov     ecx, dword ptr ds:[MakeSubjectClass_SwitchSubjectTypeTable + stCALC * 4]
		lea     ebx, [ebp - 124]
		push    edi
		push    ecx
		jmp     dword ptr [__InitExceptBlockLDTC]
	}
}

