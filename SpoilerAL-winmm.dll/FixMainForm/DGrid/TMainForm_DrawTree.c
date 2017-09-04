#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

__declspec(naked) void __cdecl Caller_TMainForm_DrawTree(TMainForm *this, LPVOID DestCanvas, long LeftOffset, long TopOffset)
{
	__asm
	{
		#define this       (esp +  4)
		#define DestCanvas (esp +  8)
		#define LeftOffset (esp + 12)
		#define TopOffset  (esp + 16)

		mov     eax, dword ptr [TopOffset ]
		mov     edx, dword ptr [LeftOffset]
		push    FALSE
		push    eax
		mov     ecx, dword ptr [DestCanvas + 8]
		mov     eax, dword ptr [this       + 8]
		push    edx
		push    ecx
		push    eax
		call    TMainForm_DrawTree
		ret

		#undef this
		#undef DestCanvas
		#undef LeftOffset
		#undef TopOffset
	}
}

void __stdcall TMainForm_DrawTree(TMainForm *this, LPVOID DestCanvas, long LeftOffset, long TopOffset, BOOL IgnoreDebugString)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        span;
	int        i, end;
	LPVOID     ssgActionListner;

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_POS;
	if (GetScrollInfo(DGridHandle, SB_HORZ, &si))
	{
		int pos;

		pos = si.nPos - si.nMin;
		if (pos)
		{
			int range;

			range = si.nMax - si.nMin;
			if (range)
			{
				LeftOffset -= MulDiv(this->DGrid->DefaultColWidth - clientWidth, pos, range);
			}
		}
	}

	span = this->DGrid->DefaultRowHeight;
	rect.left = LeftOffset;
	rect.right = this->DGrid->DefaultColWidth + LeftOffset;
	if (DestCanvas)
	{
		rect.top = TopOffset;
	}
	else
	{
		rect.top = 0;
		DestCanvas = this->DGrid->Canvas;
	}
	rect.bottom = rect.top + span;

	i = this->DGrid->TopRow;
	end = clientHeight / this->DGrid->DefaultRowHeight;
	if (clientHeight % this->DGrid->DefaultRowHeight)
		end++;
	end += i;
	if (end > this->DGrid->RowCount)
		end = this->DGrid->RowCount;
	if (IgnoreDebugString)
	{
		ssgActionListner = this->ssgCtrl.ssgActionListner;
		this->ssgCtrl.ssgActionListner = NULL;
	}
	for (end--; i < end; i++)
	{
		TMainForm_DrawTreeCell(this, DestCanvas, i, &rect);
		rect.top += span;
		rect.bottom += span;
	}
	rect.bottom = rect.top + TDrawGrid_GetRowHeights(this->DGrid, end);
	TMainForm_DrawTreeCell(this, DestCanvas, end, &rect);
	if (IgnoreDebugString)
		this->ssgCtrl.ssgActionListner = ssgActionListner;
}
