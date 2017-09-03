#include "SnapWindow.h"
#include "AllocERW.h"

#ifdef _MSC_VER
void __cdecl bad_alloc();
#endif

#define SNAP_PIXELS 8

#ifdef __BORLANDC__
#pragma warn -8060
#pragma warn -8065
#define CallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam) \
	CallWindowProcA((FARPROC)(WNDPROC)(lpPrevWndFunc), hWnd, Msg, wParam, lParam)
#endif

#ifdef _M_IX86
#pragma pack(push, 1)
typedef struct {
	BYTE    mov_to_eax;         // mov     eax, \           ; 00000000 _ B8,
	LPCVOID ThunkWindowProc;    //         ThunkWindowProc  ; 00000001 _ ????????(d)
	WORD    call_eax;           // call    eax              ; 00000005 _ FF. D0
} THUNK;
#pragma pack(pop)
#define sizeof_THUNK 7

typedef struct {
	THUNK   Thunk;
	HWND    hWnd;
	WNDPROC PrevWndProc;
	BOOL    Enabled;
	POINTS  EnterSizeMovePos;
	RECT    EnterSizeMoveRect;
} SNAPINFO;
#else
typedef struct {
	HWND    hWnd;
	WNDPROC PrevWndProc;
	BOOL    Enabled;
	POINTS  EnterSizeMovePos;
	RECT    EnterSizeMoveRect;
} SNAPINFO;
#endif

static SNAPINFO *SnapArray = NULL;
static SNAPINFO *EndOfSnap = NULL;

#define SizeOfSnap ((size_t)EndOfSnap - (size_t)SnapArray)

static void __stdcall InternalDetachSnapWindow(SNAPINFO *this);

static void __stdcall OnEnterSizeMove(SNAPINFO *this)
{
	*(LPDWORD)&this->EnterSizeMovePos = GetMessagePos();
	GetWindowRect(this->hWnd, &this->EnterSizeMoveRect);
}

static void __stdcall OnSizing(SNAPINFO *this, UINT fwSide, LPRECT pRect)
{
	SNAPINFO *p;
	HMONITOR hMonitor;

	if (!this->Enabled)
		return;
	for (p = SnapArray; p != EndOfSnap; p++)
	{
		#define HORZ 1
		#define VERT 2

		RECT rect;
		int  flags;

		if (p == this || !IsWindowVisible(p->hWnd) || !GetWindowRect(p->hWnd, &rect))
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
	hMonitor = MonitorFromWindow(this->hWnd, MONITOR_DEFAULTTONEAREST);
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
}

static void __stdcall OnMoving(SNAPINFO *this, LPRECT pRect)
{
	POINTS   pt;
	long     x, y;
	SNAPINFO *p;
	HMONITOR hMonitor;

	if (!this->Enabled)
		return;
	*(LPDWORD)&pt = GetMessagePos();
	x = pt.x - this->EnterSizeMovePos.x;
	y = pt.y - this->EnterSizeMovePos.y;
	pRect->left   = this->EnterSizeMoveRect.left   + x;
	pRect->top    = this->EnterSizeMoveRect.top    + y;
	pRect->right  = this->EnterSizeMoveRect.right  + x;
	pRect->bottom = this->EnterSizeMoveRect.bottom + y;
	for (p = SnapArray; p != EndOfSnap; p++)
	{
		#define HORZ 1
		#define VERT 2

		RECT rect;
		int  flags;

		if (p == this || !IsWindowVisible(p->hWnd) || !GetWindowRect(p->hWnd, &rect))
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
	hMonitor = MonitorFromWindow(this->hWnd, MONITOR_DEFAULTTONEAREST);
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
}

static SNAPINFO * __stdcall FindSnap(HWND hWnd)
{
	SNAPINFO *find;

	find = NULL;
	if (SnapArray)
	{
		SNAPINFO *p;

		for (p = SnapArray; p != EndOfSnap; p++)
		{
			if (p->hWnd == hWnd)
			{
				find = p;
				break;
			}
		}
	}
	return find;
}

#ifdef _M_IX86
static LRESULT CALLBACK WindowProc(SNAPINFO *this, UINT uMsg, WPARAM wParam, LPARAM lParam);

__declspec(naked) static void __cdecl ThunkWindowProc()
{
	__asm
	{
		pop     eax
		sub     eax, sizeof_THUNK
		mov     dword ptr [esp + 4], eax
		jmp     WindowProc
	}
}

static LRESULT CALLBACK WindowProc(SNAPINFO *this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND    hWnd;
	WNDPROC lpPrevWndProc;

	hWnd = this->hWnd;
	lpPrevWndProc = this->PrevWndProc;
#else
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SNAPINFO *this;
	WNDPROC  lpPrevWndProc;

	this = FindSnap(hWnd);
	if (!this)
		return 0;
	lpPrevWndProc = this->PrevWndProc;
#endif
	switch (uMsg)
	{
	case WM_ENTERSIZEMOVE:
		OnEnterSizeMove(this);
		break;
	case WM_SIZING:
		OnSizing(this, (UINT)wParam, (LPRECT)lParam);
		break;
	case WM_MOVING:
		OnMoving(this, (LPRECT)lParam);
		break;
	case WM_DESTROY:
		InternalDetachSnapWindow(this);
		break;
	}
	return CallWindowProcA(lpPrevWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL __fastcall AttachSnapWindow(HWND hWnd)
{
	BOOL bResult;

	bResult = FALSE;
	if (IsWindow(hWnd))
	{
		SNAPINFO *MemBlock;

		if (SnapArray)
		{
			if (!FindSnap(hWnd))
			{
				MemBlock = (SNAPINFO *)ReAllocERW(SnapArray, SizeOfSnap + sizeof(SNAPINFO));
				if (MemBlock && MemBlock != SnapArray)
				{
					(LPBYTE)EndOfSnap += (ptrdiff_t)MemBlock - (ptrdiff_t)SnapArray;
					SnapArray = MemBlock;
				}
			}
			else
			{
				MemBlock = NULL;
			}
		}
		else
		{
			EndOfSnap = SnapArray = MemBlock = (SNAPINFO *)AllocERW(sizeof(SNAPINFO));
		}
		if (MemBlock)
		{
			SNAPINFO *Snap;

			Snap = EndOfSnap++;
#ifdef _M_IX86
			/*
				mov     eax, ThunkWindowProc    ; 00000000 _ B8, ????????(d)
				call    eax                     ; 00000005 _ FF. D0
			*/
			Snap->Thunk.mov_to_eax      = 0xB8;
			Snap->Thunk.ThunkWindowProc = (LPCVOID)ThunkWindowProc;
			Snap->Thunk.call_eax        = 0xD0FF;
			FlushInstructionCache(GetCurrentProcess(), &Snap->Thunk, sizeof(Snap->Thunk));
			Snap->hWnd = hWnd;
			Snap->Enabled = TRUE;
			Snap->PrevWndProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)&Snap->Thunk);
#else
			Snap->hWnd = hWnd;
			Snap->Enabled = TRUE;
			Snap->PrevWndProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
#endif
			bResult = TRUE;
		}
#ifdef _MSC_VER
		else
		{
			bad_alloc();
		}
#endif
	}
	return bResult;
}

void __fastcall DetachSnapWindow(HWND hWnd)
{
	SNAPINFO *Snap;

	if (!SnapArray)
		return;
	Snap = FindSnap(hWnd);
	if (!Snap)
		return;
	InternalDetachSnapWindow(Snap);
}

static void __stdcall InternalDetachSnapWindow(SNAPINFO *this)
{
	SetWindowLongPtrA(this->hWnd, GWLP_WNDPROC, (LONG_PTR)this->PrevWndProc);
	if (--EndOfSnap != SnapArray)
	{
		memcpy(this, this + 1, (size_t)EndOfSnap - (size_t)this);
		ReAllocERW(SnapArray, SizeOfSnap);
	}
	else
	{
		FreeERW(SnapArray);
		EndOfSnap = SnapArray = NULL;
	}
}

BOOL __fastcall EnableSnapWindow(HWND hWnd, BOOL bEnable)
{
	BOOL bResult;

	bResult = FALSE;
	if (SnapArray)
	{
		SNAPINFO *Snap;

		Snap = FindSnap(hWnd);
		if (Snap)
		{
			bResult = !Snap->Enabled;
			Snap->Enabled = bEnable;
		}
	}
	return bResult;
}
