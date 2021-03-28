#include <windows.h>
#include "bcb6_operator.h"
#define USING_NAMESPACE_BCB6_STD
#include "TWinControl.h"
#include "TMainForm.h"
#undef MainForm
#include "TFont.h"
#include "verbose.h"
#include "SnapWindow.h"

void __cdecl InitializeProcessMonitor();
void __stdcall InitializeMenuId(TMainForm *MainForm);
BOOL __stdcall AppendDebugWithoutMouseOverModeMenu(TMainForm *MainForm);
BOOL __stdcall AppendToolMenu(TMainForm *MainForm);
void __stdcall ModifyVersionString(TMainForm *MainForm);
LRESULT CALLBACK TMainForm_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TMainForm_DGridProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void __cdecl InitializeWaitCursor();

extern WNDPROC TMainForm_PrevWindowProc;
extern WNDPROC TMainForm_PrevDGridProc;

static void __fastcall ctor(TMainForm *this);

__declspec(naked) void __cdecl TMainForm_ctor()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 4H]
		jmp     ctor
	}
}

static void __fastcall ctor(TMainForm *this)
{
	verbose(VRB_INFO, "TMainForm::ctor - begin");

	InitializeProcessMonitor();
	InitializeMenuId(this);
	AppendDebugWithoutMouseOverModeMenu(this);
	AppendToolMenu(this);
	ModifyVersionString(this);
	TMainForm_PrevWindowProc = (WNDPROC)SetWindowLongPtrA(TWinControl_GetHandle(this), GWLP_WNDPROC, (LONG_PTR)TMainForm_WindowProc);
	TMainForm_PrevDGridProc = (WNDPROC)SetWindowLongPtrA(TWinControl_GetHandle(this->DGrid), GWLP_WNDPROC, (LONG_PTR)TMainForm_DGridProc);
	InitializeWaitCursor();
#if SNAPWINDOW
	AttachSnapWindow(TWinControl_GetHandle(this));
#endif
	this->DistractionVec = bcb6_operator_new(sizeof(vector_string));
	vector_ctor(this->DistractionVec);

	verbose(VRB_INFO, "TMainForm::ctor - end");
}
