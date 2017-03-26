#include <windows.h>
#ifndef __BORLANDC__
#include <intrin.h>
#include "TMainForm.h"
#include "TGuideForm.h"
#include "TWinControl.h"
#else
#include "intrinsic.h"
#define TWinControl_GetHandle(WinControl) (WinControl)->Handle
#endif
#include "ApplicationMessage.h"

#ifndef __BORLANDC__
#ifndef _WIN64
#pragma intrinsic(_BitScanReverse)
#else
#pragma intrinsic(_BitScanReverse64)
#endif
#pragma intrinsic(__movsb)
#endif

extern HANDLE  hHeap;

static LPSTR   lpszTextBuffer = NULL;
static size_t  nCapacity      = 0;
static size_t  nTextLength    = 0;
static BOOLEAN bClear         = FALSE;

void __cdecl ClearGuideBuffer()
{
	if (lpszTextBuffer)
	{
		HeapFree(hHeap, 0, lpszTextBuffer);
		lpszTextBuffer = NULL;
	}
	nCapacity = 0;
	nTextLength = 0;
}

#ifndef __BORLANDC__
void __cdecl TGuideForm_Guide(TGuideForm *_this, const char *Mes, BOOLEAN IsClear)
#else
void TGuideForm::Guide(const char *Mes, bool IsClear)
#endif
{
	size_t        length, required;
	size_t        prevLength;
	unsigned char *dest;

	if (IsClear)
	{
		bClear = IsClear;
		nTextLength = 0;
	}
	length = strlen(Mes);
	required = nTextLength + length + sizeof(char) * 2;
	if ((INT_PTR)required < 0)
		return;
	if (required > nCapacity)
	{
		size_t size;

		if (nCapacity)
		{
			LPVOID lpMem;

			size = nCapacity;
			while ((size *= 2) < required);
			lpMem = HeapReAlloc(hHeap, 0, lpszTextBuffer, size);
			if (!lpMem)
				return;
			lpszTextBuffer = (LPSTR)lpMem;
		}
		else
		{
			unsigned long bits;

#ifndef _WIN64
			_BitScanReverse(&bits, required - 1);
#else
			_BitScanReverse64(&bits, required - 1);
#endif
			size = (size_t)1 << (bits + 1);
			lpszTextBuffer = (LPSTR)HeapAlloc(hHeap, 0, size);
			if (!lpszTextBuffer)
				return;
		}
		nCapacity = size;
	}
	prevLength = nTextLength;
	dest = lpszTextBuffer + nTextLength;
	nTextLength += length;
	*(LPWORD)(lpszTextBuffer + nTextLength++) = (BYTE)'\n';
	__movsb(dest, Mes, length);
	if (prevLength)
		return;
	PostMessageA(TWinControl_GetHandle(MainForm), WM_DRAW_GUIDE_BUFFER, 0, 0);
}

LRESULT __stdcall DrawGuideBuffer(WNDPROC lpPrevWndFunc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (nTextLength)
	{
		HWND hWnd;

		hWnd = TWinControl_GetHandle(MainForm->guideForm->REdit);
		if (bClear)
		{
			bClear = FALSE;
			SendMessageA(hWnd, WM_SETTEXT, 0, (LPARAM)lpszTextBuffer);
		}
		else
		{
			SendMessageA(hWnd, EM_SETSEL, -1, -1);
			SendMessageA(hWnd, EM_REPLACESEL, FALSE, (LPARAM)lpszTextBuffer);
			PostMessageA(hWnd, EM_SCROLLCARET, 0, 0);
		}
		nTextLength = 0;
	}
	return 0;
}
