#include <windows.h>
#include "TMainForm.h"

extern const DWORD _TMainForm_HotKeyEditKeyDown;

void __stdcall TMainForm_OnDGridLButtonDown(TMainForm *this, LPARAM lParam);
void __stdcall TMainForm_OnDGridLButtonUp(TMainForm *this, LPARAM lParam);

WNDPROC TMainForm_PrevDGridProc      = NULL;
BOOLEAN TMainForm_DGridLButtonDblClk = FALSE;

__declspec(naked) LRESULT CALLBACK TMainForm_DGridProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
	if (uMsg == WM_LBUTTONDOWN)
	{
		TMainForm_DGridLButtonDblClk = FALSE;
		TMainForm_OnDGridLButtonDown(MainForm, lParam);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		TMainForm_OnDGridLButtonUp(MainForm, lParam);
	}
	else if (uMsg == WM_LBUTTONDBLCLK)
	{
		TMainForm_OnDGridLButtonDblClk = TRUE;
	}
	else if (uMsg == WM_MOUSEWHEEL)
	{
		BOOLEAN Handled;

		Handled = FALSE;
		TMainForm_FormMouseWheel(
			MainForm,
			NULL,
			0,
			GET_WHEEL_DELTA_WPARAM(wParam),
			(POINT){ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) },
			&Handled);
		return 0;
	}
	else if (uMsg == WM_KEYDOWN)
	{
		Handled = FALSE;
		TMainForm_HotKeyEditKeyDown(
			MainForm,
			NULL,
			(LPWORD)&wParam,
			0);
		return 0;
	}
	return CallWindowProcA(TMainForm_PrevDGridProc, hwnd, uMsg, wParam, lParam);
	*/

	__asm
	{
		mov     eax, dword ptr [TMainForm_PrevDGridProc]
		pop     ecx
		push    eax
		push    ecx

		#define lpPrevWndFunc (esp +  4)
		#define hWnd          (esp +  8)
		#define uMsg          (esp + 12)
		#define wParam        (esp + 16)
		#define lParam        (esp + 20)

		mov     eax, dword ptr [uMsg]
		mov     ecx, dword ptr [lParam]
		cmp     eax, WM_LBUTTONDOWN
		je      L1
		cmp     eax, WM_LBUTTONUP
		je      L2
		cmp     eax, WM_LBUTTONDBLCLK
		je      L3
		cmp     eax, WM_MOUSEWHEEL
		je      L4
		cmp     eax, WM_KEYDOWN
		je      L5
		jmp     CallWindowProcA

	L1:
		mov     eax, dword ptr ds:[_MainForm]
		mov     edx, CallWindowProcA
		mov     byte ptr [TMainForm_DGridLButtonDblClk], FALSE
		push    ecx
		push    eax
		push    edx
		jmp     TMainForm_OnDGridLButtonDown

	L2:
		mov     eax, dword ptr ds:[_MainForm]
		mov     edx, CallWindowProcA
		push    ecx
		push    eax
		push    edx
		jmp     TMainForm_OnDGridLButtonUp

	L3:
		mov     byte ptr [TMainForm_DGridLButtonDblClk], TRUE
		jmp     CallWindowProcA

	L4:
		mov     edx, dword ptr [wParam]
		sub     esp, 12
		sar     edx, 16
		mov     eax, esp
		shl     ecx, 16
		push    edx
		sar     ecx, 16
		mov     edx, dword ptr [lParam + 16]
		sar     edx, 16
		push    eax
		mov     dword ptr [eax], ecx
		mov     dword ptr [eax + 4], edx
		add     eax, 8
		mov     ecx, dword ptr ds:[_MainForm]
		xor     edx, edx
		push    eax
		push    edx
		mov     dword ptr [eax], edx
		call    _TMainForm_FormMouseWheel
		add     esp, 12
		xor     eax, eax
		ret     20

	L5:
		mov     eax, dword ptr ds:[_MainForm]
		lea     ecx, [esp + 16]
		xor     edx, edx
		push    0
		call    dword ptr [_TMainForm_HotKeyEditKeyDown]
		xor     eax, eax
		ret     20

		#undef lpPrevWndFunc
		#undef hWnd
		#undef uMsg
		#undef wParam
		#undef lParam
	}
}
