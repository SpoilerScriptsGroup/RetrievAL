#include "ToolTip.h"

#if USE_TOOLTIP

#define USING_NAMESPACE_BCB6_STD
#include "TWinControl.h"
#include "TMainForm.h"
#include "TCalcValBox.h"
#include "TListBox.h"

static HWND      hToolTip = NULL;
static TOOLINFOA ti = { sizeof(TOOLINFOA), TTF_TRACK };
static HHOOK     hHook = NULL;
extern BOOL      bActive = FALSE;

static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);

void __cdecl CreateToolTip()
{
	if (hToolTip)
		return;
	hToolTip = CreateWindowExA(
		0,
		TOOLTIPS_CLASSA,
		NULL,
		TTS_NOPREFIX | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		ti.hinst = GetModuleHandleA(NULL),
		NULL);
	if (!hToolTip)
		return;
	SendMessageA(hToolTip, TTM_ADDTOOLA, 0, (LPARAM)&ti);
}

void __cdecl DestroyToolTip()
{
	if (hHook)
	{
		UnhookWindowsHookEx(hHook);
		hHook = NULL;
	}
	if (hToolTip)
	{
		DestroyWindow(hToolTip);
		hToolTip = NULL;
	}
	bActive = FALSE;
}

static DWORD __stdcall ToClientPos(DWORD dwPos)
{
	HWND hWnd;

	hWnd = GetActiveWindow();
	if (hWnd)
	{
		RECT rect;

		GetClientRect(hWnd, &rect);
		ClientToScreen(hWnd, (LPPOINT)&rect);
		ClientToScreen(hWnd, (LPPOINT)&rect + 1);
		return MAKELONG(
			min(max((short)LOWORD(dwPos), rect.left), rect.right),
			min(max((short)HIWORD(dwPos), rect.top), rect.bottom));
	}
	return dwPos;
}

static __inline DWORD GetFocusedPos()
{
	DWORD  dwMessagePos;
	HWND   hWnd;
	LPVOID listBox;
	int    index;
	POINT  pt;
	RECT   rect;

	dwMessagePos = GetMessagePos();
	switch (MainForm->nowPanelType)
	{
	case atLONG:
	case atDOUBLE:
		hWnd = TWinControl_GetHandle(vector_at(&MainForm->calcImage->valBox, 1).edit);
		break;
	case atSTRING:
		hWnd = TWinControl_GetHandle(MainForm->StringNewValEdit);
		break;
	case atBOOL:
		hWnd = TWinControl_GetHandle(MainForm->ToggleCBox);
		break;
	case atLONG_INDEX:
		listBox = MainForm->ListLBox;
		goto LIST_ITEM_POS;
	case atBOOL_VECTOR:
		listBox = MainForm->MultiLBox;
	LIST_ITEM_POS:
		index = TListBox_GetItemIndex(listBox);
		if (index < 0)
			goto FAILED;
		index -= TListBox_GetTopIndex(listBox);
		pt.x = 0;
		pt.y = (index + 1) * TListBox_GetItemHeight(listBox);
		ClientToScreen(TWinControl_GetHandle(listBox), &pt);
		return MAKELONG(pt.x, pt.y);
	default:
		goto FAILED;
	}
	if (!hWnd)
		goto FAILED;
	GetWindowRect(hWnd, &rect);
	return MAKELONG(rect.left, rect.bottom);
FAILED:
	return ToClientPos(dwMessagePos);
}

__declspec(naked) void __cdecl ShowInfoToolTip()
{
	__asm
	{
		mov     edx, TTI_INFO
		mov     ecx, eax
		jmp     ShowToolTip
	}
}

void __fastcall ShowToolTip(LPCSTR lpText, HICON hIcon)
{
	DWORD  dwTrackPos;
	LPCSTR lpTitle;
	HWND hMain = TWinControl_GetHandle(MainForm);

	if (!hToolTip)
		return;
	if (hIcon == (HICON)TTI_ERROR)
	{
		dwTrackPos = GetFocusedPos();
		lpTitle = "Error";
	}
	else
	{
		dwTrackPos = ToClientPos(GetMessagePos());
		lpTitle = hIcon == (HICON)TTI_INFO ? "Notice" : NULL;
		if (!IS_INTRESOURCE(hIcon))
		{
			lpTitle = (LPCSTR)hIcon;
			hIcon = (HICON)TTI_WARNING;
		}
	}

	SetWindowPos(hToolTip, IsWindowVisible(hMain) ? HWND_NOTOPMOST : HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(hMain, hToolTip, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SendMessageA(hToolTip, TTM_SETTITLEA, (WPARAM)hIcon, (LPARAM)lpTitle);
	ti.lpszText = (LPSTR)lpText;
	SendMessageA(hToolTip, TTM_UPDATETIPTEXTA, 0, (LPARAM)&ti);
	SendMessageA(hToolTip, TTM_TRACKPOSITION, 0, (LPARAM)dwTrackPos);
	SendMessageA(hToolTip, TTM_TRACKACTIVATE, bActive = TRUE, (LPARAM)&ti);
	if (hHook)
		return;
	hHook = SetWindowsHookExA(WH_CBT, CBTProc, NULL, GetCurrentThreadId());
}

static __inline void HideToolTip()
{
	if (bActive)
		SendMessageA(hToolTip, TTM_TRACKACTIVATE, bActive = FALSE, (LPARAM)&ti);
	if (!hHook)
		return;
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
}

static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch (nCode)
	{
	case HCBT_KEYSKIPPED:
		if (wParam <= VK_MENU && wParam >= VK_SHIFT || (long)lParam < 0)
			break;
		/* FALLTHROUGH */
	case HCBT_CLICKSKIPPED:
		HideToolTip();
		return 0;
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

#endif	// USE_TOOLTIP

