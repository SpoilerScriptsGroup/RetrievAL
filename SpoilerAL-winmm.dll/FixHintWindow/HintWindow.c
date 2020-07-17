#include <windows.h>
#include "commctrl.h"
#define USING_NAMESPACE_BCB6_STD
#include "TApplication.h"
#include "TWinControl.h"

#define EqualPoint(lppt1, lppt2) ((lppt1)->x == (lppt2)->x && (lppt1)->y == (lppt2)->y)

static HWND      hToolTip = NULL;
static TOOLINFOA ti = { sizeof(TOOLINFOA), TTF_IDISHWND | TTF_SUBCLASS };
static HHOOK     hHook = NULL;
static POINT     TrackPos;

static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);

void __cdecl CreateHintWindow()
{
	if (hToolTip)
		return;
	hToolTip = CreateWindowExA(
		0,
		TOOLTIPS_CLASSA,
		NULL,
		TTS_ALWAYSTIP | TTS_NOPREFIX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		ti.hinst = GetModuleHandleA(NULL),
		NULL);
	if (!hToolTip)
		return;
    SendMessage(hToolTip, TTM_SETMAXTIPWIDTH, 0, INT_MAX);
}

void __cdecl DestroyHintWindow()
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
	ti.uId = 0;
}

void __fastcall TApplication_ActivateHint(TApplication *this, LPPOINT CursorPos)
{
	HWND hWnd;

	if (hHook || !this->Control || !hToolTip)
		return;
	TrackPos = *CursorPos;
	SendMessageA(hToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM(this->HintHidePause, 0));
	SendMessageA(hToolTip, TTM_SETDELAYTIME, TTDT_INITIAL, MAKELPARAM(this->HintPause, 0));
	SendMessageA(hToolTip, TTM_SETDELAYTIME, TTDT_RESHOW, MAKELPARAM(this->HintShortPause, 0));
	ti.lpszText = (LPSTR)this->Control->Hint;
	if ((hWnd = TWinControl_GetHandle(this->Control)) == (HWND)ti.uId)
	{
		SendMessageA(hToolTip, TTM_UPDATETIPTEXTA, 0, (LPARAM)&ti);
	}
	else
	{
		SendMessage(hToolTip , TTM_DELTOOL , 0 , (LPARAM)&ti);
		ti.uId = (UINT_PTR)hWnd;
		SendMessage(hToolTip , TTM_ADDTOOLA, 0 , (LPARAM)&ti);
	}
	SetCapture((HWND)ti.uId);
	hHook = SetWindowsHookExA(
		WH_GETMESSAGE,
		GetMsgProc,
		ti.hinst,
		GetCurrentThreadId());
}

static LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;

	lResult = CallNextHookEx(hHook, nCode, wParam, lParam);
	do	// do { ... } while (0);
	{
		if (nCode < 0)
			break;
		if (GetCapture() == (HWND)ti.uId)
		{
			if (EqualPoint(&((LPMSG)lParam)->pt, &TrackPos))
				break;
			ReleaseCapture();
		}
		UnhookWindowsHookEx(hHook);
		SendMessageA(hToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&ti);
		hHook = NULL;
	} while (0);
	return lResult;
}

