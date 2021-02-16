#include "SnapWindow.h"
#include <limits.h>
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#include <assert.h>
#include <stdlib.h>

#define FIXED_ARRAY 4
#define SNAP_PIXELS 8
#if defined(_M_IX86) || defined(_M_X64)
#define SORTED      (!FIXED_ARRAY || FIXED_ARRAY >= 16)
#else
#define SORTED      1
#endif

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
	DWORD     mov;      //  mov     dword ptr [esp + 4], \          ; 00000000 _ C7. 44 24, 04,
	LPVOID    thisPtr;  //                               thisPtr    ; 00000004 _ ????????
	BYTE      jmp;      //  jmp     \                               ; 00000008 _ E9,
	ptrdiff_t relproc;  //          WindowProc                      ; 00000009 _ ????????
#elif defined(_M_X64)
	WORD      mov1;     //  mov     rax, \                          ; 00000000 _ 48: B8,
	WNDPROC   wndproc;  //               offset WindowProc          ; 00000002 _ ????????????????
	WORD      mov2;     //  mov     rcx, \                          ; 0000000A _ 48: B9,
	LPVOID    thisPtr;  //               thisPtr                    ; 0000000C _ ????????????????
	DWORD     jmp;      //  jmp     rax                             ; 00000014 _ 48: FF. E0
#endif
} THUNK;
#pragma pack(pop)
#endif

#if defined(_M_IX86) || defined(_M_X64)
static __forceinline void InitThunk(THUNK *thunk, LPVOID thisPtr, WNDPROC WindowProc)
{
#if defined(_M_IX86)
	/*
		mov     dword ptr [esp + 4], thisPtr    ; 00000000 _ C7. 44 24, 04, ????????(d)
		jmp     WindowProc                      ; 00000008 _ E9, ????????
	*/
	thunk->mov     = 0x042444C7;
#if !SORTED
	thunk->thisPtr = thisPtr;
#endif
	thunk->jmp     = 0xE9;
#if !SORTED
	thunk->relproc = (ptrdiff_t)WindowProc - (ptrdiff_t)(&thunk->relproc + 1);
	FlushInstructionCache(GetCurrentProcess(), thunk, sizeof(THUNK));
#endif
#elif defined(_M_X64)
	/*
		mov     rax, WindowProc                 ; 00000000 _ 48: B8, ????????????????
		mov     rcx, thisPtr                    ; 0000000A _ 48: B9, ????????????????
		jmp     rax                             ; 00000014 _ 48: FF. E0
	*/
	thunk->mov1    = 0xB848;
	thunk->wndproc = WindowProc;
	thunk->mov2    = 0xB948;
#if !SORTED
	thunk->thisPtr = thisPtr;
#endif
	thunk->jmp     = 0x00E0FF48;
#if !SORTED
	FlushInstructionCache(GetCurrentProcess(), thunk, offsetof(THUNK, jmp) + 3);
#endif
#endif
}
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
	BOOL    HasMargin;
	RECT    Margin;
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

static void __stdcall OnEnterSizeMove(SNAPINFO *this)
{
	*(LPDWORD)&this->EnterSizeMovePos = GetMessagePos();
	GetWindowRectangle(this->hWnd, &this->EnterSizeMoveRect);
	this->HasMargin = GetWindowMargin(this->hWnd, &this->Margin);
}

static void __stdcall OnSizing(SNAPINFO *this, UINT fwSide, LPRECT pRect)
{
#if !FIXED_ARRAY
	SNAPINFO **p, **end;
#else
	SNAPINFO *p, *end;
#endif
	HMONITOR hMonitor;

	if (!this->Enabled)
		return;
	if (this->HasMargin)
	{
		pRect->left   -= this->Margin.left  ;
		pRect->top    -= this->Margin.top   ;
		pRect->right  -= this->Margin.right ;
		pRect->bottom -= this->Margin.bottom;
	}
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
					else if (pRect->left >= rect.right - (SNAP_PIXELS - 1) + 1 && pRect->left < rect.right + (SNAP_PIXELS + 1))
						pRect->left = rect.right;
					else
						break;
				else if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT)
					if (pRect->right >= rect.left - SNAP_PIXELS && pRect->right < rect.left + SNAP_PIXELS)
						pRect->right = rect.left;
					else if (pRect->right >= rect.right - (SNAP_PIXELS - 1) && pRect->right < rect.right + (SNAP_PIXELS + 1))
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
				else if (pRect->top >= rect.bottom - (SNAP_PIXELS - 1) && pRect->top < rect.bottom + (SNAP_PIXELS + 1))
					pRect->top = rect.bottom;
				else
					break;
			else if (fwSide >= WMSZ_BOTTOM && fwSide <= WMSZ_BOTTOMRIGHT)
				if (pRect->bottom >= rect.top - SNAP_PIXELS && pRect->bottom < rect.top + SNAP_PIXELS)
					pRect->bottom = rect.top;
				else if (pRect->bottom >= rect.bottom - (SNAP_PIXELS - 1) && pRect->bottom < rect.bottom + (SNAP_PIXELS + 1))
					pRect->bottom = rect.bottom;
				else
					break;
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
			if (fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT)
			{
				if (pRect->left >= mi.rcWork.left - SNAP_PIXELS && pRect->left < mi.rcWork.left + SNAP_PIXELS)
					pRect->left = mi.rcWork.left;
			}
			else if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT)
			{
				if (pRect->right >= mi.rcWork.right - (SNAP_PIXELS - 1) && pRect->right < mi.rcWork.right + (SNAP_PIXELS + 1))
					pRect->right = mi.rcWork.right;
			}
			if (fwSide >= WMSZ_TOP && fwSide <= WMSZ_TOPRIGHT)
			{
				if (pRect->top >= mi.rcWork.top - SNAP_PIXELS && pRect->top < mi.rcWork.top + SNAP_PIXELS)
					pRect->top = mi.rcWork.top;
			}
			else if (fwSide >= WMSZ_BOTTOM && fwSide <= WMSZ_BOTTOMRIGHT)
			{
				if (pRect->bottom >= mi.rcWork.bottom - (SNAP_PIXELS - 1) && pRect->bottom < mi.rcWork.bottom + (SNAP_PIXELS + 1))
					pRect->bottom = mi.rcWork.bottom;
			}
		}
	}
	if (this->HasMargin)
	{
		pRect->left   += this->Margin.left  ;
		pRect->top    += this->Margin.top   ;
		pRect->right  += this->Margin.right ;
		pRect->bottom += this->Margin.bottom;
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
				else if (pRect->left >= rect.right - (SNAP_PIXELS - 1) && pRect->left < rect.right + (SNAP_PIXELS + 1))
				{
					pRect->right += rect.right - pRect->left;
					pRect->left = rect.right;
				}
				else if (pRect->right >= rect.left - SNAP_PIXELS && pRect->right < rect.left + SNAP_PIXELS)
				{
					pRect->left += rect.left - pRect->right;
					pRect->right = rect.left;
				}
				else if (pRect->right >= rect.right - (SNAP_PIXELS - 1) && pRect->right < rect.right + (SNAP_PIXELS + 1))
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
			else if (pRect->top >= rect.bottom - (SNAP_PIXELS - 1) && pRect->top < rect.bottom + (SNAP_PIXELS + 1))
			{
				pRect->bottom += rect.bottom - pRect->top;
				pRect->top = rect.bottom;
			}
			else if (pRect->bottom >= rect.top - SNAP_PIXELS && pRect->bottom < rect.top + SNAP_PIXELS)
			{
				pRect->top += rect.top - pRect->bottom;
				pRect->bottom = rect.top;
			}
			else if (pRect->bottom >= rect.bottom - (SNAP_PIXELS - 1) && pRect->bottom < rect.bottom + (SNAP_PIXELS + 1))
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
			else if (pRect->right >= mi.rcWork.right - (SNAP_PIXELS - 1) && pRect->right < mi.rcWork.right + (SNAP_PIXELS + 1))
			{
				pRect->left += mi.rcWork.right - pRect->right;
				pRect->right = mi.rcWork.right;
			}
			if (pRect->top >= mi.rcWork.top - SNAP_PIXELS && pRect->top < mi.rcWork.top + SNAP_PIXELS)
			{
				pRect->bottom += mi.rcWork.top - pRect->top;
				pRect->top = mi.rcWork.top;
			}
			else if (pRect->bottom >= mi.rcWork.bottom - (SNAP_PIXELS - 1) && pRect->bottom < mi.rcWork.bottom + (SNAP_PIXELS + 1))
			{
				pRect->top += mi.rcWork.bottom - pRect->bottom;
				pRect->bottom = mi.rcWork.bottom;
			}
		}
	}
	if (this->HasMargin)
	{
		pRect->left   += this->Margin.left  ;
		pRect->top    += this->Margin.top   ;
		pRect->right  += this->Margin.right ;
		pRect->bottom += this->Margin.bottom;
	}
}

static void __stdcall OnDestroy(SNAPINFO *this)
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

		if (lpMem = HeapReAlloc(hHeap, 0, SnapInfo, sizeof(SNAPINFO *) * NumberOfElements))
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

#if SORTED
static int __cdecl CompareSnapInfo(const void *elem1, const void *elem2)
{
#ifndef _WIN64
	unsigned int lhs, rhs;

#if !FIXED_ARRAY
	lhs = (unsigned int)(*(SNAPINFO **)elem1)->hWnd;
	rhs = (unsigned int)(*(SNAPINFO **)elem2)->hWnd;
#else
	lhs = (unsigned int)((SNAPINFO *)elem1)->hWnd;
	rhs = (unsigned int)((SNAPINFO *)elem2)->hWnd;
#endif
	return lhs - rhs;
#else
	unsigned __int64 lhs, rhs;

#if !FIXED_ARRAY
	lhs = (unsigned __int64)(*(SNAPINFO **)elem1)->hWnd;
	rhs = (unsigned __int64)(*(SNAPINFO **)elem2)->hWnd;
#else
	lhs = (unsigned __int64)((SNAPINFO *)elem1)->hWnd;
	rhs = (unsigned __int64)((SNAPINFO *)elem2)->hWnd;
#endif
	return lhs != rhs ? lhs >= rhs ? 1 : -1 : 0;
#endif
}
#endif

#if SORTED
__forceinline
#endif
static SNAPINFO * __fastcall FindElement(HWND hWnd)
{
#if SORTED
#if !FIXED_ARRAY
	SNAPINFO *key, **elem;

	key = (SNAPINFO *)((char *)&hWnd - offsetof(SNAPINFO, hWnd));
	elem = (SNAPINFO **)bsearch(&key, SnapInfo, NumberOfElements, sizeof(SNAPINFO *), CompareSnapInfo);
	return elem ? *elem : NULL;
#else
	return (SNAPINFO *)bsearch((void *)((char *)&hWnd - offsetof(SNAPINFO, hWnd)), SnapInfo, NumberOfElements, sizeof(SNAPINFO), CompareSnapInfo);
#endif
#else
	SNAPINFO *p, *end;

	if ((p = SnapInfo) && hWnd)
		for (end = p + NumberOfElements; p != end; p++)
			if (p->hWnd == hWnd)
				return p;
	return NULL;
#endif
}

#if defined(_M_IX86) || defined(_M_X64)
static LRESULT CALLBACK WindowProc(SNAPINFO *this, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#else
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SNAPINFO *this;

	this = (SNAPINFO *)FindElement(hWnd);
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
	if (FindElement(hWnd))
		return NULL;
	if (this = (SNAPINFO *)HeapAlloc(hExecutableHeap, HEAP_ZERO_MEMORY, sizeof(SNAPINFO)))
	{
		LPVOID lpMem;

		if (lpMem = SnapInfo ?
			HeapReAlloc(hHeap, 0, SnapInfo, sizeof(SNAPINFO *) * (NumberOfElements + 1)) :
			HeapAlloc(hHeap, 0, sizeof(SNAPINFO *)))
		{
			return (SnapInfo = (SNAPINFO **)lpMem)[NumberOfElements++] = this;
		}
		HeapFree(hExecutableHeap, 0, this);
	}
#else
	if (SnapInfo)
	{
#if SORTED
		if (FindElement(hWnd))
			return NULL;
		if (!SnapInfo->hWnd)
			return SnapInfo;
#else
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
			return (SNAPINFO *)memset(this, 0, sizeof(SNAPINFO));
#endif
	}
	else
	{
		if (this = (SNAPINFO *)HeapAlloc(hExecutableHeap, HEAP_ZERO_MEMORY, sizeof(SNAPINFO) * NumberOfElements))
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
#if SORTED && (defined(_M_IX86) || defined(_M_X64))
	HANDLE   process;
#if !FIXED_ARRAY
	SNAPINFO **p, **end;
#else
	SNAPINFO *p, *end;
#endif
#endif

	if (!IsWindow(hWnd))
		return FALSE;
	if (!(this = AppendElement(hWnd)))
		return FALSE;
#if defined(_M_IX86) || defined(_M_X64)
	InitThunk(&this->Thunk, this, (WNDPROC)WindowProc);
#endif
	this->hWnd = hWnd;
	this->Enabled = TRUE;
#if !defined(_M_IX86) && !defined(_M_X64)
	this->PrevWndProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
#elif !SORTED
	this->PrevWndProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)&this->Thunk);
#else
	this->PrevWndProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
#endif
#if SORTED
#if !FIXED_ARRAY
	qsort(SnapInfo, NumberOfElements, sizeof(SNAPINFO *), CompareSnapInfo);
#else
	qsort(SnapInfo, NumberOfElements, sizeof(SNAPINFO), CompareSnapInfo);
#endif
#if defined(_M_IX86) || defined(_M_X64)
	process = GetCurrentProcess();
	for (end = (p = SnapInfo) + NumberOfElements; p != end; p++)
	{
#if !FIXED_ARRAY
		this = *p;
#else
		if (!(this = p)->hWnd)
			continue;
#endif
		this->Thunk.thisPtr = this;
#if defined(_M_IX86)
		this->Thunk.relproc = (ptrdiff_t)WindowProc - (ptrdiff_t)(&this->Thunk.relproc + 1);
		FlushInstructionCache(process, &this->Thunk, sizeof(THUNK));
#elif defined(_M_X64)
		FlushInstructionCache(process, &this->Thunk, offsetof(THUNK, jmp) + 3);
#endif
		SetWindowLongPtrA(this->hWnd, GWLP_WNDPROC, (LONG_PTR)&this->Thunk);
	}
#endif
#endif
	return TRUE;
}

void __fastcall DetachSnapWindow(HWND hWnd)
{
	SNAPINFO *this;

	this = (SNAPINFO *)FindElement(hWnd);
	if (this)
		OnDestroy(this);
}

BOOL __fastcall EnableSnapWindow(HWND hWnd, BOOL bEnable)
{
	SNAPINFO *this;
	BOOL     bResult;

	this = (SNAPINFO *)FindElement(hWnd);
	bResult = FALSE;
	if (this)
	{
		bResult = !this->Enabled;
		this->Enabled = bEnable;
	}
	return bResult;
}
