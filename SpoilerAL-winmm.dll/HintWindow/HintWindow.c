#include "HintWindow.h"
#include "commctrl.h"
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TApplication.h"
#include "TWinControl.h"
#include "TDrawGrid.h"

static HWND      hToolTip = NULL;
static TOOLINFOA ti = { sizeof(TOOLINFOA), TTF_IDISHWND | TTF_SUBCLASS };
static HHOOK     hHook = NULL;
static DWORD     dwTrackPos;
static TDrawGrid *DrawGrid;

static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam);

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
    SendMessageA(hToolTip, TTM_SETMAXTIPWIDTH, 0, INT_MAX);
	SendMessageA(hToolTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM(2500, 0));
	SendMessageA(hToolTip, TTM_SETDELAYTIME, TTDT_INITIAL, MAKELPARAM(0, 0));
	SendMessageA(hToolTip, TTM_SETDELAYTIME, TTDT_RESHOW, MAKELPARAM(0, 0));
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
	char lpClassName[16];

	if (hHook || !this->Control || !hToolTip)
		return;
	dwTrackPos = GetMessagePos();
	ti.lpszText = (LPSTR)this->Control->Hint;
	if ((hWnd = TWinControl_GetHandle(this->Control)) == (HWND)ti.uId)
	{
		SendMessageA(hToolTip, TTM_UPDATETIPTEXTA, 0, (LPARAM)&ti);
	}
	else
	{
		if (ti.uId)
			SendMessageA(hToolTip, TTM_DELTOOLA, 0, (LPARAM)&ti);
		ti.uId = (UINT_PTR)hWnd;
		SendMessageA(hToolTip, TTM_ADDTOOLA, 0, (LPARAM)&ti);
	}
	DrawGrid = NULL;
	if (GetClassNameA((HWND)ti.uId, lpClassName, _countof(lpClassName)) == 9 &&
		*(LPDWORD)&lpClassName[0] == BSWAP32('TDra') &&
		*(LPDWORD)&lpClassName[4] == BSWAP32('wGri') &&
		*(LPWORD )&lpClassName[8] == BSWAP16('d\0'))
	{
		ScreenToClient((HWND)ti.uId, &ptTrack);
		TDrawGrid_MouseToCell(DrawGrid = (TDrawGrid *)this->Control, ptTrack.x, ptTrack.y, &ptTrack.x, &ptTrack.y);
	}
	hHook = SetWindowsHookExA(WH_CALLWNDPROCRET, CallWndRetProc, NULL, GetCurrentThreadId());
}

static LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;

	lResult = CallNextHookEx(hHook, nCode, wParam, lParam);
	if (nCode >= 0)
	{
		DWORD dwCursorPos;
		BOOL  bSuccess;

		dwCursorPos = GetMessagePos();
		bSuccess = TRUE;
		if (DrawGrid)
		{
			POINT pt;

			pt.x = (short)dwCursorPos;
			pt.y = (long)dwCursorPos >> 16;
			if (bSuccess = ScreenToClient((HWND)ti.uId, &pt))
			{
				TDrawGrid_MouseToCell(DrawGrid, pt.x, pt.y, &pt.x, &pt.y);
				dwCursorPos = MAKELONG(pt.x, pt.y);
			}
		}
		if (!bSuccess || dwCursorPos != dwTrackPos || !IsWindowVisible((HWND)ti.uId))
		{
			UnhookWindowsHookEx(hHook);
			SendMessageA(hToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&ti);
			hHook = NULL;
		}
	}
	return lResult;
}

