#include <windows.h>
#include "TMainForm.h"

void __declspec(naked) __cdecl TSearchForm_AddressLBoxDblClick_SubjectAccess(TMainForm* mainForm, TSSGSubject* SelectS)
{
	extern const DWORD F00439F10;

	__asm
	{
		mov     eax, dword ptr [esp + 8]
		cmp     [eax]TSSGSubject.type, stDIR
		je      NO_ACCESS
		jmp     dword ptr [F00439F10]

	NO_ACCESS:
		ret
	}
}
