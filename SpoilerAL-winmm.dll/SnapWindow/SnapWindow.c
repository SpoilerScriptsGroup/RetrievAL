#include "SnapWindow.h"
#include <limits.h>
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#include <assert.h>

#define FIXED_ARRAY 4
#define SNAP_PIXELS 8

#ifdef _MSC_VER
void __cdecl bad_alloc();
#endif

#ifdef __BORLANDC__
#pragma warn -8060
#pragma warn -8065
#define CallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam) \
	CallWindowProcA((FARPROC)(WNDPROC)(lpPrevWndFunc), hWnd, Msg, wParam, lParam)
#endif

extern BOOL __stdcall GetWindowRectangle(IN HWND hWnd, OUT LPRECT lpRect);
extern BOOL __stdcall GetWindowMargin(IN HWND hWnd, OUT LPRECT lprcMargin);

#if defined(_M_IX86) || defined(_M_X64)
#pragma pack(push, 1)
typedef struct {
#if defined(_M_IX86)
	DWORD     mov;      // mov     dword ptr [esp + 4], \       ; 00000000 _ C7. 44 24, 04,
	LPVOID    this;     //                              this    ; 00000004 _ ????????
	BYTE      jmp;      // jmp     \                            ; 00000008 _ E9,
	ptrdiff_t relproc;  //         WindowProc                   ; 00000009 _ ????????
#elif defined(_M_X64)
	WORD      mov1;     // mov     rax, \                       ; 00000000 _ 48: B8,
	WNDPROC   wndproc;  //              WindowProc              ; 00000002 _ ????????????????
	WORD      mov2;     // mov     rcx, \                       ; 0000000A _ 48: B9,
	LPVOID    this;     //              this                    ; 0000000C _ ????????????????
	DWORD     jmp;      // jmp     rax                          ; 00000014 _ 48: FF. E0
#endif
} THUNK;
#pragma pack(pop)
#endif

typedef struct {
#if defined(_M_IX86) || defined(_M_X64)
	THUNK   Thunk;
#endif
	HWND    hWnd;
	WNDPROC PrevWndProc;
	BOOL    Enabled;
	POINTS  EnterSizeMovePos;
	RECT    EnterSizeMoveRect;
} SNAPINFO;

#if !FIXED_ARRAY
extern HANDLE hHeap;
#endif
extern HANDLE hExecutableHeap;

#if !FIXED_ARRAY
static SNAPINFO **SnapInfo = NULL;
#else
static SNAPINFO *SnapInfo = NULL;
#endif

#if !FIXED_ARRAY
static size_t NumberOfElements = 0;
#else
#define NumberOfElements (size_t)FIXED_ARRAY
#endif

static void __stdcall Detach(SNAPINFO *this);
#define OnDestroy Detach

static void __stdcall OnEnterSizeMove(SNAPINFO *this)
{
	*(LPDWORD)&this->EnterSizeMovePos = GetMessagePos();
	GetWindowRectangle(this->hWnd, &this->EnterSizeMoveRect);
}

static void __stdcall OnSizing(SNAPINFO *this, UINT fwSide, LPRECT pRect)
{
	HMONITOR hMonitor;
	RECT     rcMargin;
#if !FIXED_ARRAY
	SNAPINFO **p, **end;
#else
	SNAPINFO *p, *end;
#endif

	if (!this->Enabled)
		return;
	for (end = (p = SnapInfo) + NumberOfElements; p != end; p++)
	{
		#define HORZ 1
		#define VERT 2

		RECT rect;
		int  flags;

#if !FIXED_ARRAY
		if (*p == this || !IsWindowVisible((*p)->hWnd) || !GetWindowRectangle((*p)->hWnd, &rect))
#else
		if (p == this || !IsWindowVisible(p->hWnd) || !GetWindowRectangle(p->hWnd, &rect))
#endif
			continue;
		flags = HORZ | VERT;
		do
		{
			do	// do { ... } while (0);
			{
				if (!(flags & HORZ) || pRect->top > rect.bottom || pRect->bottom < rect.top)
					break;
				if (fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT)
					if (pRect->left >= rect.left - SNAP_PIXELS && pRect->left < rect.left + SNAP_PIXELS)
						pRect->left = rect.left;
					else if (pRect->left >= rect.right - SNAP_PIXELS && pRect->left < rect.right + SNAP_PIXELS)
						pRect->left = rect.right;
					else
						break;
				else if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT)
					if (pRect->right >= rect.left - SNAP_PIXELS && pRect->right < rect.left + SNAP_PIXELS)
						pRect->right = rect.left;
					else if (pRect->right >= rect.right - SNAP_PIXELS && pRect->right < rect.right + SNAP_PIXELS)
						pRect->right = rect.right;
					else
						break;
				else
					break;
				flags &= ~HORZ;
			} while (0);
			if (!(flags & VERT) || pRect->left > rect.right || pRect->right < rect.left)
				break;
			if (fwSide >= WMSZ_TOP && fwSide <= WMSZ_TOPRIGHT)
				if (pRect->top >= rect.top - SNAP_PIXELS && pRect->top < rect.top + SNAP_PIXELS)
					pRect->top = rect.top;
				else if (pRect->top >= rect.bottom - SNAP_PIXELS && pRect->top < rect.bottom + SNAP_PIXELS)
					pRect->top = rect.bottom;
				else
					break;
			else if (fwSide >= WMSZ_BOTTOM && fwSide <= WMSZ_BOTTOMRIGHT)
				if (pRect->bottom >= rect.top - SNAP_PIXELS && pRect->bottom < rect.top + SNAP_PIXELS)
					pRect->bottom = rect.top;
				else if (pRect->bottom >= rect.bottom - SNAP_PIXELS && pRect->bottom < rect.bottom + SNAP_PIXELS)
					pRect->bottom = rect.bottom;
				else
					break;
			else
				break;
		} while (flags &= ~VERT);

		#undef HORZ
		#undef VERT
	}
	if (GetWindowMargin(this->hWnd, &rcMargin))
	{
		if (fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT)
			pRect->left -= rcMargin.left;
		else if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT)
			pRect->right += rcMargin.right;
		if (fwSide >= WMSZ_TOP && fwSide <= WMSZ_TOPRIGHT)
			pRect->top -= rcMargin.top;
		else if (fwSide >= WMSZ_BOTTOM && fwSide <= WMSZ_BOTTOMRIGHT)
			pRect->bottom += rcMargin.bottom;
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
				if (pRect->left >= mi.rcWork.left - SNAP_PIXELS && pRect->left < mi.rcWork.left + SNAP_PIXELS)
					pRect->left = mi.rcWork.left;
			}
			else if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT)
			{
				if (pRect->right >= mi.rcWork.right - SNAP_PIXELS && pRect->right < mi.rcWork.right + SNAP_PIXELS)
					pRect->right = mi.rcWork.right;
			}
			if (fwSide >= WMSZ_TOP && fwSide <= WMSZ_TOPRIGHT)
			{
				if (pRect->top >= mi.rcWork.top - SNAP_PIXELS && pRect->top < mi.rcWork.top + SNAP_PIXELS)
					pRect->top = mi.rcWork.top;
			}
			else if (fwSide >= WMSZ_BOTTOM && fwSide <= WMSZ_BOTTOMRIGHT)
			{
				if (pRect->bottom >= mi.rcWork.bottom - SNAP_PIXELS && pRect->bottom < mi.rcWork.bottom + SNAP_PIXELS)
					pRect->bottom = mi.rcWork.bottom;
			}
		}
	}
}

static void __stdcall OnMoving(SNAPINFO *this, LPRECT pRect)
{
	POINTS   pt;
	long     x, y;
#if !FIXED_ARRAY
	SNAPINFO **p, **end;
#else
	SNAPINFO *p, *end;
#endif
	RECT     rcMargin;
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
	for (end = (p = SnapInfo) + NumberOfElements; p != end; p++)
	{
		#define HORZ 1
		#define VERT 2

		RECT rect;
		int  flags;

#if !FIXED_ARRAY
		if (*p == this || !IsWindowVisible((*p)->hWnd) || !GetWindowRectangle((*p)->hWnd, &rect))
#else
		if (p == this || !IsWindowVisible(p->hWnd) || !GetWindowRectangle(p->hWnd, &rect))
#endif
			continue;
		flags = HORZ | VERT;
		do
		{
			do	// do { ... } while (0);
			{
				if (!(flags & HORZ) || pRect->top > rect.bottom || pRect->bottom < rect.top)
					break;
				if (pRect->left >= rect.left - SNAP_PIXELS && pRect->left < rect.left + SNAP_PIXELS)
				{
					pRect->right += rect.left - pRect->left;
					pRect->left = rect.left;
				}
				else if (pRect->left >= rect.right - SNAP_PIXELS && pRect->left < rect.right + SNAP_PIXELS)
				{
					pRect->right += rect.right - pRect->left;
					pRect->left = rect.right;
				}
				else if (pRect->right >= rect.left - SNAP_PIXELS && pRect->right < rect.left + SNAP_PIXELS)
				{
					pRect->left += rect.left - pRect->right;
					pRect->right = rect.left;
				}
				else if (pRect->right >= rect.right - SNAP_PIXELS && pRect->right < rect.right + SNAP_PIXELS)
				{
					pRect->left += rect.right - pRect->right;
					pRect->right = rect.right;
				}
				else
					break;
				flags &= ~HORZ;
			} while (0);
			if (!(flags & VERT) || pRect->left > rect.right || pRect->right < rect.left)
				break;
			if (pRect->top >= rect.top - SNAP_PIXELS && pRect->top < rect.top + SNAP_PIXELS)
			{
				pRect->bottom += rect.top - pRect->top;
				pRect->top = rect.top;
			}
			else if (pRect->top >= rect.bottom - SNAP_PIXELS && pRect->top < rect.bottom + SNAP_PIXELS)
			{
				pRect->bottom += rect.bottom - pRect->top;
				pRect->top = rect.bottom;
			}
			else if (pRect->bottom >= rect.top - SNAP_PIXELS && pRect->bottom < rect.top + SNAP_PIXELS)
			{
				pRect->top += rect.top - pRect->bottom;
				pRect->bottom = rect.top;
			}
			else if (pRect->bottom >= rect.bottom - SNAP_PIXELS && pRect->bottom < rect.bottom + SNAP_PIXELS)
			{
				pRect->top += rect.bottom - pRect->bottom;
				pRect->bottom = rect.bottom;
			}
			else
				break;
		} while (flags &= ~VERT);

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
			if (pRect->left >= mi.rcWork.left - SNAP_PIXELS && pRect->left < mi.rcWork.left + SNAP_PIXELS)
			{
				pRect->right += mi.rcWork.left - pRect->left;
				pRect->left = mi.rcWork.left;
			}
			else if (pRect->right >= mi.rcWork.right - SNAP_PIXELS && pRect->right < mi.rcWork.right + SNAP_PIXELS)
			{
				pRect->left += mi.rcWork.right - pRect->right;
				pRect->right = mi.rcWork.right;
			}
			if (pRect->top >= mi.rcWork.top - SNAP_PIXELS && pRect->top < mi.rcWork.top + SNAP_PIXELS)
			{
				pRect->bottom += mi.rcWork.top - pRect->top;
				pRect->top = mi.rcWork.top;
			}
			else if (pRect->bottom >= mi.rcWork.bottom - SNAP_PIXELS && pRect->bottom < mi.rcWork.bottom + SNAP_PIXELS)
			{
				pRect->top += mi.rcWork.bottom - pRect->bottom;
				pRect->bottom = mi.rcWork.bottom;
			}
		}
	}
	if (GetWindowMargin(this->hWnd, &rcMargin))
	{
		pRect->left   -= rcMargin.left  ;
		pRect->top    -= rcMargin.top   ;
		pRect->right  += rcMargin.right ;
		pRect->bottom += rcMargin.bottom;
	}
}

static SNAPINFO * __fastcall FindElement(HWND hWnd)
{
#if !FIXED_ARRAY
	SNAPINFO **p, **end;
#else
	SNAPINFO *p, *end;
#endif

	if (!SnapInfo || !hWnd)
		return NULL;
	for (end = (p = SnapInfo) + NumberOfElements; p != end; p++)
#if !FIXED_ARRAY
		if ((*p)->hWnd == hWnd)
			return *p;
#else
		if (p->hWnd == hWnd)
			return p;
#endif
	return NULL;
}

#if defined(_M_IX86) || defined(_M_X64)
static LRESULT CALLBACK WindowProc(SNAPINFO *this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#else
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SNAPINFO *this;

	this = FindElement(hWnd);
	if (!this)
		return 0;
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
		OnDestroy(this);
		break;
	}
#if defined(_M_IX86) || defined(_M_X64)
	return CallWindowProcA(this->PrevWndProc, this->hWnd, uMsg, wParam, lParam);
#else
	return CallWindowProcA(this->PrevWndProc, hWnd, uMsg, wParam, lParam);
#endif
}

static SNAPINFO * __fastcall AppendElement(HWND hWnd)
{
	SNAPINFO *this;

#if !FIXED_ARRAY
	if (SnapInfo)
	{
		SNAPINFO **p, **end;

		for (end = (p = SnapInfo) + NumberOfElements; p != end; p++)
			if ((*p)->hWnd == hWnd)
				return NULL;
	}
	if (this = (SNAPINFO *)HeapAlloc(hExecutableHeap, 0, sizeof(SNAPINFO)))
	{
		if (SnapInfo = SnapInfo ?
			(SNAPINFO **)HeapReAlloc(hHeap, 0, SnapInfo, sizeof(SNAPINFO *) * (NumberOfElements + 1)) :
			(SNAPINFO **)HeapAlloc(hHeap, 0, sizeof(SNAPINFO *)))
		{
			SnapInfo[NumberOfElements++] = this;
			return this;
		}
		HeapFree(hExecutableHeap, 0, this);
	}
#else
	if (SnapInfo)
	{
		SNAPINFO *p, *end;

		this = NULL;
		for (end = (p = SnapInfo) + NumberOfElements; p != end; p++)
		{
			if (p->hWnd)
			{
				if (p->hWnd == hWnd)
					return NULL;
			}
			else
			{
				if (!this)
					this = p;
			}
		}
		if (this)
			return this;
	}
	else
	{
		if (this = (SNAPINFO *)HeapAlloc(hExecutableHeap, 0, sizeof(SNAPINFO) * NumberOfElements))
			return SnapInfo = this;
	}
#endif
#ifdef _MSC_VER
	bad_alloc();
#endif
	return NULL;
}

BOOL __fastcall AttachSnapWindow(HWND hWnd)
{
	SNAPINFO *this;

	if (!IsWindow(hWnd))
		return FALSE;
	if (!(this = AppendElement(hWnd)))
		return FALSE;
#if defined(_M_IX86)
	/*
		mov     dword ptr [esp + 4], this   ; 00000000 _ C7. 44 24, 04, ????????(d)
		jmp     WindowProc                  ; 00000008 _ E9, ????????
	*/
	this->Thunk.mov     = 0x042444C7;
	this->Thunk.this    = this;
	this->Thunk.jmp     = 0xE9;
	this->Thunk.relproc = (ptrdiff_t)WindowProc - (ptrdiff_t)(&this->Thunk.relproc + 1);
#elif defined(_M_X64)
	/*
		mov     rax, WindowProc             ; 00000000 _ 48: B8, ????????????????
		mov     rcx, this                   ; 0000000A _ 48: B9, ????????????????
		jmp     rax                         ; 00000014 _ 48: FF. E0
	*/
	this->Thunk.mov1    = 0xB848;
	this->Thunk.wndproc = WindowProc;
	this->Thunk.mov2    = 0xB948;
	this->Thunk.this    = this;
	this->Thunk.jmp     = 0x00E0FF48;
#endif
#if defined(_M_IX86) || defined(_M_X64)
	FlushInstructionCache(GetCurrentProcess(), &this->Thunk, sizeof(this->Thunk));
#endif
	this->hWnd = hWnd;
	this->Enabled = TRUE;
#if defined(_M_IX86) || defined(_M_X64)
	this->PrevWndProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)&this->Thunk);
#else
	this->PrevWndProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
#endif
	return TRUE;
}

void __fastcall DetachSnapWindow(HWND hWnd)
{
	SNAPINFO *this;

	this = FindElement(hWnd);
	if (this)
		Detach(this);
}

static void __stdcall Detach(SNAPINFO *this)
{
#if !FIXED_ARRAY
	SNAPINFO **p, **end;

	SetWindowLongPtrA(this->hWnd, GWLP_WNDPROC, (LONG_PTR)this->PrevWndProc);
	for (end = (p = SnapInfo) + NumberOfElements; p != end; p++)
	{
		if (*p != this)
			continue;
		memcpy(p, p + 1, (size_t)(SnapInfo + NumberOfElements--) - (size_t)(p + 1));
		break;
	}
	HeapFree(hExecutableHeap, 0, this);
	if (NumberOfElements)
	{
		LPVOID lpMem;

		if (lpMem = HeapReAlloc(hHeap, 0, SnapInfo, sizeof(SNAPINFO) * NumberOfElements))
			SnapInfo = (SNAPINFO **)lpMem;
	}
	else
	{
		HeapFree(hHeap, 0, SnapInfo);
		SnapInfo = NULL;
	}
#else
	SNAPINFO *p, *end;

	SetWindowLongPtrA(this->hWnd, GWLP_WNDPROC, (LONG_PTR)this->PrevWndProc);
	this->hWnd = NULL;
	for (end = (p = SnapInfo) + NumberOfElements; p != end; p++)
		if (p->hWnd)
			return;
	HeapFree(hExecutableHeap, 0, SnapInfo);
	SnapInfo = NULL;
#endif
}

BOOL __fastcall EnableSnapWindow(HWND hWnd, BOOL bEnable)
{
	BOOL     bResult;
	SNAPINFO *this;

	bResult = FALSE;
	this = FindElement(hWnd);
	if (this)
	{
		bResult = !this->Enabled;
		this->Enabled = bEnable;
	}
	return bResult;
}
