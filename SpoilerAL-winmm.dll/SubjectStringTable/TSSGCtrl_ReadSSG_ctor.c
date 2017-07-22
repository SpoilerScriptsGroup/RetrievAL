#include "SubjectStringTable.h"

extern const DWORD F005D54CC;

__declspec(naked) void __cdecl TSSGCtrl_ReadSSG_ctor()
{
	__asm
	{
		call    SubjectStringTable_clear
		mov     eax, 00633D68H
		jmp     dword ptr [F005D54CC]
	}
}
