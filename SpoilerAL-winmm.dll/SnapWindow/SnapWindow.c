#include "SnapWindow.h"

#define SNAP_PIXELS 8

#ifdef __BORLANDC__
#pragma warn -8060
#define CallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam) \
	CallWindowProcA((FARPROC)(WNDPROC)(lpPrevWndFunc), hWnd, Msg, wParam, lParam)
#endif

extern HANDLE hHeap;

typedef struct {
	HWND    hWnd;
	WNDPROC PrevWindowProc;
	BOOL    Enabled;
	POINTS  EnterSizeMovePos;
	RECT    EnterSizeMoveRect;
} SNAPINFO;

static SNAPINFO         *SnapArray = NULL;
static SNAPINFO         *EndOfSnap = NULL;
static CRITICAL_SECTION cs;

#define SizeOfSnap ((size_t)EndOfSnap - (size_t)SnapArray)

static void __stdcall OnEnterSizeMove(SNAPINFO *Snap)
{
	*(LPDWORD)&Snap->EnterSizeMovePos = GetMessagePos();
	GetWindowRect(Snap->hWnd, &Snap->EnterSizeMoveRect);
}

static void __stdcall OnSizing(SNAPINFO *Snap, UINT fwSide, LPRECT pRect)
{
	SNAPINFO *p;
	HMONITOR hMonitor;

	if (!Snap->Enabled)
		return;
	EnterCriticalSection(&cs);
	for (p = SnapArray; p != EndOfSnap; p++)
	{
		#define HORZ 1
		#define VERT 2

		RECT rect;
		int  flags;

		if (p == Snap || !IsWindowVisible(p->hWnd) || !GetWindowRect(p->hWnd, &rect))
			continue;
		flags = HORZ | VERT;
		do
		{
			if ((flags & HORZ) && pRect->top <= rect.bottom && pRect->bottom >= rect.top)
			{
				if (fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT)
				{
					if (pRect->left >= rect.left - SNAP_PIXELS && pRect->left <= rect.left + SNAP_PIXELS)
					{
						pRect->left = rect.left;
						flags &= ~HORZ;
					}
					else if (pRect->left >= rect.right - SNAP_PIXELS && pRect->left <= rect.right + SNAP_PIXELS)
					{
						pRect->left = rect.right;
						flags &= ~HORZ;
					}
				}
				else if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT)
				{
					if (pRect->right >= rect.left - SNAP_PIXELS && pRect->right <= rect.left + SNAP_PIXELS)
					{
						pRect->right = rect.left;
						flags &= ~HORZ;
					}
					else if (pRect->right >= rect.right - SNAP_PIXELS && pRect->right <= rect.right + SNAP_PIXELS)
					{
						pRect->right = rect.right;
						flags &= ~HORZ;
					}
				}
			}
			if ((flags & VERT) && pRect->left <= rect.right && pRect->right >= rect.left)
			{
				if (fwSide >= WMSZ_TOP && fwSide <= WMSZ_TOPRIGHT)
				{
					if (pRect->top >= rect.top - SNAP_PIXELS && pRect->top <= rect.top + SNAP_PIXELS)
					{
						pRect->top = rect.top;
						flags &= ~VERT;
						continue;
					}
					else if (pRect->top >= rect.bottom - SNAP_PIXELS && pRect->top <= rect.bottom + SNAP_PIXELS)
					{
						pRect->top = rect.bottom;
						flags &= ~VERT;
						continue;
					}
				}
				else if (fwSide >= WMSZ_BOTTOM && fwSide <= WMSZ_BOTTOMRIGHT)
				{
					if (pRect->bottom >= rect.top - SNAP_PIXELS && pRect->bottom <= rect.top + SNAP_PIXELS)
					{
						pRect->bottom = rect.top;
						flags &= ~VERT;
						continue;
					}
					else if (pRect->bottom >= rect.bottom - SNAP_PIXELS && pRect->bottom <= rect.bottom + SNAP_PIXELS)
					{
						pRect->bottom = rect.bottom;
						flags &= ~VERT;
						continue;
					}
				}
			}
			break;
		} while (flags);

		#undef HORZ
		#undef VERT
	}
	hMonitor = MonitorFromWindow(Snap->hWnd, MONITOR_DEFAULTTONEAREST);
	if (hMonitor)
	{
		MONITORINFO mi;

		mi.cbSize = sizeof(mi);
		if (GetMonitorInfo(hMonitor, &mi))
		{
			if (fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT)
			{
				if (pRect->left >= mi.rcWork.left - SNAP_PIXELS && pRect->left <= mi.rcWork.left + SNAP_PIXELS)
					pRect->left = mi.rcWork.left;
			}
			else if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT)
			{
				if (pRect->right >= mi.rcWork.right - SNAP_PIXELS && pRect->right <= mi.rcWork.right + SNAP_PIXELS)
					pRect->right = mi.rcWork.right;
			}
			if (fwSide >= WMSZ_TOP && fwSide <= WMSZ_TOPRIGHT)
			{
				if (pRect->top >= mi.rcWork.top - SNAP_PIXELS && pRect->top <= mi.rcWork.top + SNAP_PIXELS)
					pRect->top = mi.rcWork.top;
			}
			else if (fwSide >= WMSZ_BOTTOM && fwSide <= WMSZ_BOTTOMRIGHT)
			{
				if (pRect->bottom >= mi.rcWork.bottom - SNAP_PIXELS && pRect->bottom <= mi.rcWork.bottom + SNAP_PIXELS)
					pRect->bottom = mi.rcWork.bottom;
			}
		}
	}
	LeaveCriticalSection(&cs);
}

static void __stdcall OnMoving(SNAPINFO *Snap, LPRECT pRect)
{
	POINTS   pt;
	long     x, y;
	SNAPINFO *p;
	HMONITOR hMonitor;

	if (!Snap->Enabled)
		return;
	EnterCriticalSection(&cs);
	*(LPDWORD)&pt = GetMessagePos();
	x = pt.x - Snap->EnterSizeMovePos.x;
	y = pt.y - Snap->EnterSizeMovePos.y;
	pRect->left   = Snap->EnterSizeMoveRect.left   + x;
	pRect->top    = Snap->EnterSizeMoveRect.top    + y;
	pRect->right  = Snap->EnterSizeMoveRect.right  + x;
	pRect->bottom = Snap->EnterSizeMoveRect.bottom + y;
	for (p = SnapArray; p != EndOfSnap; p++)
	{
		#define HORZ 1
		#define VERT 2

		RECT rect;
		int  flags;

		if (p == Snap || !IsWindowVisible(p->hWnd) || !GetWindowRect(p->hWnd, &rect))
			continue;
		flags = HORZ | VERT;
		do
		{
			if ((flags & HORZ) && pRect->top <= rect.bottom && pRect->bottom >= rect.top)
			{
				if (pRect->left >= rect.left - SNAP_PIXELS && pRect->left <= rect.left + SNAP_PIXELS)
				{
					pRect->right += rect.left - pRect->left;
					pRect->left = rect.left;
					flags &= ~HORZ;
				}
				else if (pRect->left >= rect.right - SNAP_PIXELS && pRect->left <= rect.right + SNAP_PIXELS)
				{
					pRect->right += rect.right - pRect->left;
					pRect->left = rect.right;
					flags &= ~HORZ;
				}
				else if (pRect->right >= rect.left - SNAP_PIXELS && pRect->right <= rect.left + SNAP_PIXELS)
				{
					pRect->left += rect.left - pRect->right;
					pRect->right = rect.left;
					flags &= ~HORZ;
				}
				else if (pRect->right >= rect.right - SNAP_PIXELS && pRect->right <= rect.right + SNAP_PIXELS)
				{
					pRect->left += rect.right - pRect->right;
					pRect->right = rect.right;
					flags &= ~HORZ;
				}
			}
			if ((flags & VERT) && pRect->left <= rect.right && pRect->right >= rect.left)
			{
				if (pRect->top >= rect.top - SNAP_PIXELS && pRect->top <= rect.top + SNAP_PIXELS)
				{
					pRect->bottom += rect.top - pRect->top;
					pRect->top = rect.top;
					flags &= ~VERT;
					continue;
				}
				else if (pRect->top >= rect.bottom - SNAP_PIXELS && pRect->top <= rect.bottom + SNAP_PIXELS)
				{
					pRect->bottom += rect.bottom - pRect->top;
					pRect->top = rect.bottom;
					flags &= ~VERT;
					continue;
				}
				else if (pRect->bottom >= rect.top - SNAP_PIXELS && pRect->bottom <= rect.top + SNAP_PIXELS)
				{
					pRect->top += rect.top - pRect->bottom;
					pRect->bottom = rect.top;
					flags &= ~VERT;
					continue;
				}
				else if (pRect->bottom >= rect.bottom - SNAP_PIXELS && pRect->bottom <= rect.bottom + SNAP_PIXELS)
				{
					pRect->top += rect.bottom - pRect->bottom;
					pRect->bottom = rect.bottom;
					flags &= ~VERT;
					continue;
				}
			}
			break;
		} while (flags);

		#undef HORZ
		#undef VERT
	}
	hMonitor = MonitorFromWindow(Snap->hWnd, MONITOR_DEFAULTTONEAREST);
	if (hMonitor)
	{
		MONITORINFO mi;

		mi.cbSize = sizeof(mi);
		if (GetMonitorInfo(hMonitor, &mi))
		{
			if (pRect->left >= mi.rcWork.left - SNAP_PIXELS && pRect->left <= mi.rcWork.left + SNAP_PIXELS)
			{
				pRect->right += mi.rcWork.left - pRect->left;
				pRect->left = mi.rcWork.left;
			}
			else if (pRect->right >= mi.rcWork.right - SNAP_PIXELS && pRect->right <= mi.rcWork.right + SNAP_PIXELS)
			{
				pRect->left += mi.rcWork.right - pRect->right;
				pRect->right = mi.rcWork.right;
			}
			if (pRect->top >= mi.rcWork.top - SNAP_PIXELS && pRect->top <= mi.rcWork.top + SNAP_PIXELS)
			{
				pRect->bottom += mi.rcWork.top - pRect->top;
				pRect->top = mi.rcWork.top;
			}
			else if (pRect->bottom >= mi.rcWork.bottom - SNAP_PIXELS && pRect->bottom <= mi.rcWork.bottom + SNAP_PIXELS)
			{
				pRect->top += mi.rcWork.bottom - pRect->bottom;
				pRect->bottom = mi.rcWork.bottom;
			}
		}
	}
	LeaveCriticalSection(&cs);
}

static SNAPINFO * __stdcall FindSnap(HWND hWnd, BOOL bLocked)
{
	SNAPINFO *find;

	find = NULL;
	if (SnapArray)
	{
		SNAPINFO *p;

		if (!bLocked)
			EnterCriticalSection(&cs);
		for (p = SnapArray; p != EndOfSnap; p++)
		{
			if (p->hWnd == hWnd)
			{
				find = p;
				break;
			}
		}
		if (!bLocked)
			LeaveCriticalSection(&cs);
	}
	return find;
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SNAPINFO *Snap;
	LRESULT  lResult;

	Snap = FindSnap(hWnd, FALSE);
	if (!Snap)
		return 0;
	switch (uMsg)
	{
	case WM_ENTERSIZEMOVE:
		OnEnterSizeMove(Snap);
		break;
	case WM_SIZING:
		OnSizing(Snap, (UINT)wParam, (LPRECT)lParam);
		break;
	case WM_MOVING:
		OnMoving(Snap, (LPRECT)lParam);
		break;
	}
	lResult = CallWindowProcA(Snap->PrevWindowProc, hWnd, uMsg, wParam, lParam);
	if (uMsg == WM_DESTROY)
		DetachSnapWindow(hWnd);
	return lResult;
}

BOOL __fastcall AttachSnapWindow(HWND hWnd)
{
	BOOL bResult;

	bResult = FALSE;
	if (IsWindow(hWnd))
	{
		SNAPINFO *MemBlock;

		if (!SnapArray)
			InitializeCriticalSection(&cs);
		EnterCriticalSection(&cs);
		if (SnapArray)
		{
			if (!FindSnap(hWnd, TRUE))
			{
				MemBlock = (SNAPINFO *)HeapReAlloc(hHeap, 0, SnapArray, SizeOfSnap + sizeof(SNAPINFO));
				if (MemBlock)
				{
					(LPBYTE)EndOfSnap += (ptrdiff_t)MemBlock - (ptrdiff_t)SnapArray;
					SnapArray = MemBlock;
				}
			}
		}
		else
		{
			EndOfSnap = SnapArray = MemBlock = (SNAPINFO *)HeapAlloc(hHeap, 0, sizeof(SNAPINFO));
		}
		if (MemBlock)
		{
			SNAPINFO *Snap;

			Snap = EndOfSnap++;
			Snap->hWnd = hWnd;
			Snap->PrevWindowProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
			Snap->Enabled = TRUE;
			bResult = TRUE;
		}
		LeaveCriticalSection(&cs);
		if (!SnapArray)
			DeleteCriticalSection(&cs);
	}
	return bResult;
}

void __fastcall DetachSnapWindow(HWND hWnd)
{
	SNAPINFO *Snap;

	if (!SnapArray)
		return;
	EnterCriticalSection(&cs);
	Snap = FindSnap(hWnd, TRUE);
	if (Snap)
	{
		if (--EndOfSnap != SnapArray)
		{
			SNAPINFO *MemBlock;

			memcpy(Snap, Snap + 1, (size_t)EndOfSnap - (size_t)Snap);
			MemBlock = (SNAPINFO *)HeapReAlloc(hHeap, 0, SnapArray, SizeOfSnap);
			if (MemBlock)
			{
				(LPBYTE)EndOfSnap += (ptrdiff_t)MemBlock - (ptrdiff_t)SnapArray;
				SnapArray = MemBlock;
			}
		}
		else
		{
			HeapFree(hHeap, 0, SnapArray);
			EndOfSnap = SnapArray = NULL;
		}
	}
	LeaveCriticalSection(&cs);
	if (!SnapArray)
		DeleteCriticalSection(&cs);
}

BOOL __fastcall EnableSnapWindow(HWND hWnd, BOOL bEnable)
{
	BOOL bResult;

	bResult = FALSE;
	if (SnapArray)
	{
		SNAPINFO *Snap;

		EnterCriticalSection(&cs);
		Snap = FindSnap(hWnd, TRUE);
		if (Snap)
		{
			bResult = !Snap->Enabled;
			Snap->Enabled = bEnable;
		}
		LeaveCriticalSection(&cs);
		if (!SnapArray)
			DeleteCriticalSection(&cs);
	}
	return bResult;
}
