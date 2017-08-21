#include <windows.h>
#include "TWinControl.h"
#include "TMainForm.h"
#undef MainForm
#include "verbose.h"

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

static void __fastcall ctor(TMainForm *_this);

__declspec(naked) void __cdecl TMainForm_ctor()
{
	__asm
	{
		mov     ecx, dword ptr [ebp - 4H]
		call    ctor
		mov     eax, 00401FAEH
		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		jmp     eax
	}
}

static void __fastcall ctor(TMainForm *_this)
{
	verbose(VERBOSE_INFO, "TMainForm::ctor - begin");

	InitializeProcessMonitor();
	InitializeMenuId(_this);
	AppendDebugWithoutMouseOverModeMenu(_this);
	AppendToolMenu(_this);
	ModifyVersionString(_this);
	TMainForm_PrevWindowProc = (WNDPROC)SetWindowLongPtrA(TWinControl_GetHandle(_this), GWLP_WNDPROC, (LONG_PTR)TMainForm_WindowProc);
	TMainForm_PrevDGridProc = (WNDPROC)SetWindowLongPtrA(TWinControl_GetHandle(_this->DGrid), GWLP_WNDPROC, (LONG_PTR)TMainForm_DGridProc);
	InitializeWaitCursor();

	verbose(VERBOSE_INFO, "TMainForm::ctor - end");
}
