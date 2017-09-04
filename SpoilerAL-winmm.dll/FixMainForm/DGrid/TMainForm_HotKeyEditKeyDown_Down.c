#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

__declspec(naked) void __cdecl TMainForm_HotKeyEditKeyDown_Down()
{
	static void __stdcall HotKeyEditKeyDown_Down(TMainForm *this);

	__asm
	{
		#define ReturnAddress 00443484H
		#define this          ebx

		push    this
		push    ReturnAddress
		jmp     HotKeyEditKeyDown_Down

		#undef ReturnAddress
		#undef this
	}
}

static void __stdcall HotKeyEditKeyDown_Down(TMainForm *this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        topRow;
	int        clientRows;

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
	if (this->invertGridRow < 0)
		this->invertGridRow = 0;
	topRow = this->DGrid->TopRow >= 0 ? this->DGrid->TopRow : 0;
	do	/* do { ... } while (0); */
	{
		if (this->invertGridRow < topRow)
		{
			this->invertGridRow = topRow + 1;
			rect.top = this->DGrid->DefaultRowHeight;
			rect.bottom = this->DGrid->DefaultRowHeight * 2;
		}
		else
		{
			if (this->invertGridRow >= this->DGrid->RowCount - 2)
				break;
			rect.top = (this->invertGridRow - topRow) * this->DGrid->DefaultRowHeight;
			rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
			TMainForm_DrawTreeCell(this, this->DGrid->Canvas, this->invertGridRow++, &rect);
			rect.top = rect.bottom;
			rect.bottom += this->DGrid->DefaultRowHeight;
		}
		TMainForm_DrawTreeCell(this, this->DGrid->Canvas, this->invertGridRow, &rect);
	} while (0);
	clientRows = clientHeight / this->DGrid->DefaultRowHeight;
	if (topRow < this->DGrid->RowCount - clientRows)
	{
		int bottomRow;

		bottomRow = topRow + clientRows;
		if (this->invertGridRow > bottomRow)
		{
			this->invertGridRow = bottomRow;
			rect.top = (this->invertGridRow - topRow) * this->DGrid->DefaultRowHeight;
			rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
			TMainForm_DrawTreeCell(this, this->DGrid->Canvas, this->invertGridRow, &rect);
		}
		TDrawGrid_SetTopRow(this->DGrid, topRow + 1);
	}
}
