#include <windows.h>
#include "TMainForm.h"

void __declspec(naked) __cdecl TSearchForm_AddressLBoxDblClick_SubjectAccess(TMainForm* mainForm, TSSGSubject* SelectS)
{
	extern const DWORD F00439F10;

	__asm
	{
		mov     eax, dword ptr [esp + 8]
		cmp     byte ptr [eax + 5], 1
		je      NO_ACCESS	// type == ssgCtrl::stDIR
		jmp     dword ptr [F00439F10]

	NO_ACCESS:
		ret
	}
}
