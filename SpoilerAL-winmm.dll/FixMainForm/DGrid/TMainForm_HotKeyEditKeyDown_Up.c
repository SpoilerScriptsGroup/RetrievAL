#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

__declspec(naked) void __cdecl TMainForm_HotKeyEditKeyDown_Up()
{
	static void __stdcall HotKeyEditKeyDown_Up(TMainForm *this);

	__asm
	{
		#define ReturnAddress 00443484H
		#define this          ebx

		push    this
		push    ReturnAddress
		jmp     HotKeyEditKeyDown_Up

		#undef ReturnAddress
		#undef this
	}
}

static void __stdcall HotKeyEditKeyDown_Up(TMainForm *this)
{
	int topRow;

	topRow = this->DGrid->TopRow >= 0 ? this->DGrid->TopRow : 0;
	if (this->invertGridRow)
	{
		HWND       DGridHandle;
		RECT       rect;
		int        clientWidth, clientHeight;
		SCROLLINFO si;

		DGridHandle = TWinControl_GetHandle(this->DGrid);
		GetClientRect(DGridHandle, &rect);
		clientWidth = rect.right - rect.left;
		clientHeight = rect.bottom - rect.top;
		rect.left = 0;
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
					rect.left = -MulDiv(this->DGrid->DefaultColWidth - clientWidth, pos, range);
				}
			}
		}
		rect.right = rect.left + this->DGrid->DefaultColWidth;
		if (this->invertGridRow >= 0)
		{
			int bottomRow;

			rect.top = (this->invertGridRow - topRow) * this->DGrid->DefaultRowHeight;
			rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
			TMainForm_DrawTreeCell(this, this->DGrid->Canvas, this->invertGridRow--, &rect);
			bottomRow = clientHeight / this->DGrid->DefaultRowHeight + topRow - 2;
			if (this->invertGridRow <= bottomRow)
			{
				rect.bottom = rect.top;
				rect.top -= this->DGrid->DefaultRowHeight;
			}
			else
			{
				this->invertGridRow = bottomRow;
				rect.top = (this->invertGridRow - topRow) * this->DGrid->DefaultRowHeight;
				rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
			}
		}
		else
		{
			this->invertGridRow = 0;
			rect.top = -topRow * this->DGrid->DefaultRowHeight;
			rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
		}
		TMainForm_DrawTreeCell(this, this->DGrid->Canvas, this->invertGridRow, &rect);
	}
	if (topRow)
	{
		topRow--;
		if (this->invertGridRow < topRow)
			this->invertGridRow = topRow;
		TDrawGrid_SetTopRow(this->DGrid, topRow);
	}
}
