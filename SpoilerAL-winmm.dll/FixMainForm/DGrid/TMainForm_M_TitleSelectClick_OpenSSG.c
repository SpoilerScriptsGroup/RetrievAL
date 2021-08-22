#include "TMainForm.h"

extern const DWORD F004FCE80;

/*
void __cdecl TMainForm_M_TitleSelectClick_OpenSSG(TSSGCtrl &ssgCtrl, TSSGScriptStruct &selectScript, int &nowSelectSubjectIndex)
{
	ChangeSubjectPanel(ssgCtrl::atDIR);
	nowSelectSubjectIndex = -2;
	ssgCtrl.OpenSSG(&selectScript);
}
*/
__declspec(naked) void __cdecl TMainForm_M_TitleSelectClick_OpenSSG()
{
	__asm
	{
		push    atDIR
		push    ebx
		call    TMainForm_ChangeSubjectPanel
		add     esp, 8
		mov     [ebx]TMainForm.nowSelectSubjectIndex, -2
		jmp     dword ptr [F004FCE80]
		ud2
	}
}
