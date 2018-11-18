#include <windows.h>
#include "TWinControl.h"
#include "TMainForm.h"
#undef MainForm
#include "verbose.h"

void __cdecl ClearGuideBuffer();
void __cdecl DeleteWaitCursor();
void __cdecl DeleteProcessMonitor();
void __cdecl SubjectStringTable_dtor();

extern WNDPROC TMainForm_PrevWindowProc;
extern WNDPROC TMainForm_PrevDGridProc;

static void __fastcall dtor(TMainForm *this);

__declspec(naked) void __cdecl TMainForm_dtor()
{
	__asm
	{
		mov     ecx, dword ptr ds:[_MainForm]
		call    dtor
		mov     eax, 0045FDF2H
		dec     dword ptr [ebx + 1CH]
		lea     edx, [esi + 0C10H]
		jmp     eax
	}
}

static void __fastcall dtor(TMainForm *this)
{
	verbose(VERBOSE_INFO, "TMainForm::dtor - begin");

	ClearGuideBuffer();
	DeleteWaitCursor();
	SetWindowLongPtrA(TWinControl_GetHandle(this->DGrid), GWLP_WNDPROC, (LONG_PTR)TMainForm_PrevDGridProc);
	SetWindowLongPtrA(TWinControl_GetHandle(this), GWLP_WNDPROC, (LONG_PTR)TMainForm_PrevWindowProc);
	DeleteProcessMonitor();
	SubjectStringTable_dtor();

	verbose(VERBOSE_INFO, "TMainForm::dtor - end");
}
