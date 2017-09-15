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

#if !FIXED_ARRAY
DWORD __stdcall GetPageSize();
#endif

#ifdef __BORLANDC__
#pragma warn -8060
#pragma warn -8065
#define CallWindowProcA(lpPrevWndFunc, hWnd, Msg, wParam, lParam) \
	CallWindowProcA((FARPROC)(WNDPROC)(lpPrevWndFunc), hWnd, Msg, wParam, lParam)
#endif

#ifdef _M_IX86
#pragma pack(push, 1)
typedef struct {
	DWORD     mov;      // mov     dword ptr [esp + 4], \       ; 00000000 _ C7. 44 24, 04,
	LPVOID    this;     //                              this    ; 00000004 _ ????????
	BYTE      jmp;      // jmp     \                            ; 00000008 _ E9,
	ptrdiff_t relproc;  //         WindowProc                   ; 00000009 _ ????????
} THUNK;
#pragma pack(pop)

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

static SNAPINFO *FirstPage = NULL;
#if !FIXED_ARRAY
static size_t NumberOfElements = 0;
#else
#define NumberOfElements (size_t)FIXED_ARRAY
#endif

#if !FIXED_ARRAY
#define NextPage(Page, PageSize) ((SNAPINFO **)((LPBYTE)(Page) + (PageSize) - sizeof(SNAPINFO *) * 2))
#define PrevPage(Page, PageSize) ((SNAPINFO **)((LPBYTE)(Page) + (PageSize) - sizeof(SNAPINFO *)))
#endif

static void __stdcall Detach(SNAPINFO *this);

#if !FIXED_ARRAY
static size_t __fastcall GetMaxElementsInPage(DWORD dwPageSize)
{
	static size_t MaxElementsInPage = 0;

	assert(sizeof(SNAPINFO) <= 4096 - 1 - sizeof(SNAPINFO *) * 2);
	if (!MaxElementsInPage)
		MaxElementsInPage =
			(((size_t)dwPageSize - sizeof(SNAPINFO *) * 2) * CHAR_BIT) /
			(sizeof(SNAPINFO) * CHAR_BIT + 1);
	assert(MaxElementsInPage != 0);
	return MaxElementsInPage;
}
#endif

static void __stdcall OnEnterSizeMove(SNAPINFO *this)
{
	*(LPDWORD)&this->EnterSizeMovePos = GetMessagePos();
	GetWindowRect(this->hWnd, &this->EnterSizeMoveRect);
}

static void __stdcall OnSizing(SNAPINFO *this, UINT fwSide, LPRECT pRect)
{
#if !FIXED_ARRAY
	DWORD    dwPageSize;
	size_t   MaxElementsInPage;
	SNAPINFO *page;
#endif
	HMONITOR hMonitor;

	if (!this->Enabled)
		return;
#if !FIXED_ARRAY
	dwPageSize = GetPageSize();
	MaxElementsInPage = GetMaxElementsInPage(dwPageSize);
	page = FirstPage;
	do
	{
		LPBYTE present;
		size_t num, index;

		present = (LPBYTE)(page + MaxElementsInPage);
		num = NumberOfElements;
		index = 0;
		while (index < MaxElementsInPage)
#else
	{
		SNAPINFO *p, *end;

		for (end = (p = FirstPage) + NumberOfElements; p != end; p++)
#endif
		{
			#define HORZ 1
			#define VERT 2

			RECT     rect;
			int      flags;
#if !FIXED_ARRAY
			BYTE     c;
			SNAPINFO *p;

			if (!(c = present[index / CHAR_BIT]))
			{
				index += CHAR_BIT;
				continue;
			}
			if (!(c & (1 << (index & (CHAR_BIT - 1)))) || (p = page + index) == this || !IsWindowVisible(p->hWnd) || !GetWindowRect(p->hWnd, &rect))
			{
				index++;
				continue;
			}
#else
			if (p == this || !IsWindowVisible(p->hWnd) || !GetWindowRect(p->hWnd, &rect))
				continue;
#endif
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
#if !FIXED_ARRAY
			if (!--num)
				break;
			index++;
#endif

			#undef HORZ
			#undef VERT
		}
#if !FIXED_ARRAY
	} while ((page = *NextPage(page, dwPageSize)) != FirstPage);
#else
	}
#endif
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
#if !FIXED_ARRAY
	DWORD    dwPageSize;
	size_t   MaxElementsInPage;
	SNAPINFO *page;
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
#if !FIXED_ARRAY
	dwPageSize = GetPageSize();
	MaxElementsInPage = GetMaxElementsInPage(dwPageSize);
	page = FirstPage;
	do
	{
		LPBYTE present;
		size_t num, index;

		present = (LPBYTE)(page + MaxElementsInPage);
		num = NumberOfElements;
		index = 0;
		while (index < MaxElementsInPage)
#else
	{
		SNAPINFO *p, *end;

		for (end = (p = FirstPage) + NumberOfElements; p != end; p++)
#endif
		{
			#define HORZ 1
			#define VERT 2

			RECT     rect;
			int      flags;
#if !FIXED_ARRAY
			BYTE     c;
			SNAPINFO *p;

			if (!(c = present[index / CHAR_BIT]))
			{
				index += CHAR_BIT;
				continue;
			}
			if (!(c & (1 << (index & (CHAR_BIT - 1)))) || (p = page + index) == this || !IsWindowVisible(p->hWnd) || !GetWindowRect(p->hWnd, &rect))
			{
				index++;
				continue;
			}
#else
			if (p == this || !IsWindowVisible(p->hWnd) || !GetWindowRect(p->hWnd, &rect))
				continue;
#endif
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
#if !FIXED_ARRAY
			if (!--num)
				break;
			index++;
#endif

			#undef HORZ
			#undef VERT
		}
#if !FIXED_ARRAY
	} while ((page = *NextPage(page, dwPageSize)) != FirstPage);
#else
	}
#endif
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

static SNAPINFO * __fastcall FindElement(HWND hWnd)
{
	SNAPINFO *find;

	find = NULL;
#if !FIXED_ARRAY
	if (FirstPage)
	{
		DWORD    dwPageSize;
		size_t   MaxElementsInPage;
		SNAPINFO *page;

		dwPageSize = GetPageSize();
		MaxElementsInPage = GetMaxElementsInPage(dwPageSize);
		page = FirstPage;
		do
		{
			LPBYTE present;
			size_t num, index;

			present = (LPBYTE)(page + MaxElementsInPage);
			num = NumberOfElements;
			index = 0;
			while (index < MaxElementsInPage)
			{
				BYTE c;

				if (!(c = present[index / CHAR_BIT]))
					index += CHAR_BIT;
				else if (!(c & (1 << (index & (CHAR_BIT - 1)))))
					index++;
				else if (page[index].hWnd != hWnd)
				{
					if (!--num)
						break;
					index++;
				}
				else
				{
					find = page + index;
					break;
				}
			}
			if (index != MaxElementsInPage)
				break;
		} while ((page = *NextPage(page, dwPageSize)) != FirstPage);
	}
#else
	if (FirstPage && hWnd)
	{
		SNAPINFO *p, *end;

		for (end = (p = FirstPage) + NumberOfElements; p != end; p++)
		{
			if (p->hWnd == hWnd)
			{
				find = p;
				break;
			}
		}
	}
#endif
	return find;
}

#if !FIXED_ARRAY
static SNAPINFO * __fastcall FindElementAndBlank(HWND hWnd, SNAPINFO **pblank, SNAPINFO **ppage)
#else
static SNAPINFO * __fastcall FindElementAndBlank(HWND hWnd, SNAPINFO **pblank)
#endif
{
	SNAPINFO *find, *blank;
#if !FIXED_ARRAY
	SNAPINFO *page;
#endif

	find = NULL;
	blank = NULL;
	if (FirstPage)
	{
#if !FIXED_ARRAY
		DWORD  dwPageSize;
		size_t MaxElementsInPage;

		dwPageSize = GetPageSize();
		MaxElementsInPage = GetMaxElementsInPage(dwPageSize);
		page = FirstPage;
		do
		{
			LPBYTE present;
			size_t num, index;

			present = (LPBYTE)(page + MaxElementsInPage);
			num = NumberOfElements;
			index = 0;
			while (index < MaxElementsInPage)
			{
				BYTE c;

				if ((c = present[index / CHAR_BIT]) & (1 << (index & (CHAR_BIT - 1))))
				{
					if (page[index].hWnd != hWnd)
					{
						if (!--num)
							break;
					}
					else
					{
						find = page + index;
						break;
					}
				}
				else
				{
					if (!blank)
						blank = page + index;
					if (!c)
					{
						index += CHAR_BIT;
						continue;
					}
				}
				index++;
			}
			if (index != MaxElementsInPage)
				break;
		} while ((page = *NextPage(page, dwPageSize)) != FirstPage);
#else
		SNAPINFO *p, *end;

		for (end = (p = FirstPage) + NumberOfElements; p != end; p++)
		{
			if (p->hWnd)
			{
				if (p->hWnd == hWnd)
					find = p;
			}
			else
			{
				if (!blank)
					blank = p;
			}
		}
#endif
	}
	*pblank = blank;
#if !FIXED_ARRAY
	*ppage = blank ? page : NULL;
#endif
	return find;
}

#ifdef _M_IX86
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

	this = FindElement(hWnd);
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
		Detach(this);
		break;
	}
	return CallWindowProcA(lpPrevWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL __fastcall AttachSnapWindow(HWND hWnd)
{
	SNAPINFO *this;
#if !FIXED_ARRAY
	DWORD    dwPageSize;
	size_t   MaxElementsInPage;
#endif

#if !FIXED_ARRAY
	assert(sizeof(SNAPINFO) <= 4096 - 1 - sizeof(SNAPINFO *) * 2);
#endif
	if (!IsWindow(hWnd))
		return FALSE;
#if !FIXED_ARRAY
	dwPageSize = GetPageSize();
	MaxElementsInPage = GetMaxElementsInPage(dwPageSize);
	if (FirstPage)
	{
		SNAPINFO *page;

		if (FindElementAndBlank(hWnd, &this, &page))
			return FALSE;
		if (!this)
		{
			this = (SNAPINFO *)VirtualAlloc(NULL, GetPageSize(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			if (this)
			{
				SNAPINFO **lastPage;

				lastPage = PrevPage(FirstPage, dwPageSize);
				*PrevPage(this, dwPageSize) = *lastPage;
				*NextPage(this, dwPageSize) = FirstPage;
				*NextPage(*lastPage, dwPageSize) = this;
				*lastPage = this;
				*(LPBYTE)(this + MaxElementsInPage) = 1;
			}
		}
		else
		{
			size_t index;

			index = this - page;
			((LPBYTE)(page + MaxElementsInPage))[index / CHAR_BIT] |= 1 << (index & (CHAR_BIT - 1));
		}
	}
	else
	{
		this = (SNAPINFO *)VirtualAlloc(NULL, GetPageSize(), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (this)
		{
			*(LPBYTE)(this + MaxElementsInPage) = 1;
			*PrevPage(this, dwPageSize) = this;
			*NextPage(this, dwPageSize) = this;
			FirstPage = this;
		}
	}
#else
	if (FirstPage)
	{
		if (FindElementAndBlank(hWnd, &this))
			return FALSE;
	}
	else
	{
		this = (SNAPINFO *)VirtualAlloc(NULL, sizeof(SNAPINFO) * NumberOfElements, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (this)
			FirstPage = this;
	}
#endif
	if (!this)
	{
#ifdef _MSC_VER
		bad_alloc();
#endif
		return FALSE;
	}
#ifdef _M_IX86
	/*
		mov     dword ptr [esp + 4], this   ; 00000000 _ C7. 44 24, 04, ????????(d)
		jmp     WindowProc                  ; 00000008 _ E9, ????????
	*/
	this->Thunk.mov     = 0x042444C7;
	this->Thunk.this    = this;
	this->Thunk.jmp     = 0xE9;
	this->Thunk.relproc = (ptrdiff_t)WindowProc - (ptrdiff_t)(&this->Thunk.relproc + 1);
	FlushInstructionCache(GetCurrentProcess(), &this->Thunk, sizeof(this->Thunk));
	this->hWnd = hWnd;
	this->Enabled = TRUE;
	this->PrevWndProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)&this->Thunk);
#else
	this->hWnd = hWnd;
	this->Enabled = TRUE;
	this->PrevWndProc = (WNDPROC)SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
#endif
#if !FIXED_ARRAY
	NumberOfElements++;
#endif
	return TRUE;
}

void __fastcall DetachSnapWindow(HWND hWnd)
{
	SNAPINFO *this;

#if !FIXED_ARRAY
	assert(sizeof(SNAPINFO) <= 4096 - 1 - sizeof(SNAPINFO *) * 2);
#endif
	this = FindElement(hWnd);
	if (this)
		Detach(this);
}

static void __stdcall Detach(SNAPINFO *this)
{
#if !FIXED_ARRAY
	DWORD    dwPageSize;
	size_t   index;
	SNAPINFO *page;
	size_t   MaxElementsInPage;
	LPBYTE   present, p, end;
	SNAPINFO *prev, *next;

	SetWindowLongPtrA(this->hWnd, GWLP_WNDPROC, (LONG_PTR)this->PrevWndProc);
	dwPageSize = GetPageSize();
	index = ((size_t)this & (dwPageSize - 1)) / sizeof(SNAPINFO);
	*(size_t *)this &= -(ptrdiff_t)dwPageSize;
	page = FirstPage;
	while (page != this)
		if ((page = *NextPage(page, dwPageSize)) == FirstPage)
			return;
	NumberOfElements--;
	MaxElementsInPage = GetMaxElementsInPage(dwPageSize);
	present = (LPBYTE)(page + MaxElementsInPage);
	present[index / CHAR_BIT] &= ~(1 << (index & (CHAR_BIT - 1)));
	end = (p = present) + (MaxElementsInPage + (CHAR_BIT - 1)) / CHAR_BIT;
	do
	{
		if (*p)
			return;
	} while (++p != end);
	prev = *PrevPage(page, dwPageSize);
	next = *NextPage(page, dwPageSize);
	*NextPage(prev, dwPageSize) = next;
	*PrevPage(next, dwPageSize) = prev;
	VirtualFree(page, 0, MEM_RELEASE);
	if (page == FirstPage)
		FirstPage = NULL;
#else
	SNAPINFO *p, *end;

	SetWindowLongPtrA(this->hWnd, GWLP_WNDPROC, (LONG_PTR)this->PrevWndProc);
	this->hWnd = NULL;
	for (end = (p = FirstPage) + NumberOfElements; p != end; p++)
		if (p->hWnd)
			return;
	VirtualFree(FirstPage, 0, MEM_RELEASE);
	FirstPage = NULL;
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
