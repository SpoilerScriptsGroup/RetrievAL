#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

void __stdcall TMainForm_HotKeyEditKeyDown_PageUp(TMainForm *this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        topRow;
	int        row;
	int        clientRows;
	int        bottomRow;

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
	rect.top = (this->invertGridRow - topRow) * this->DGrid->DefaultRowHeight;
	rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
	row = this->invertGridRow;
	clientRows = clientHeight / this->DGrid->DefaultRowHeight;
	this->invertGridRow = this->invertGridRow > clientRows ? this->invertGridRow - clientRows : 0;
	TMainForm_DrawTreeCell(this, this->DGrid->Canvas, row, &rect);
	bottomRow = topRow - 1;
	if (bottomRow < clientRows)
		bottomRow = clientRows;
	if (this->invertGridRow > bottomRow)
		this->invertGridRow = bottomRow;
	rect.top = (this->invertGridRow - topRow) * this->DGrid->DefaultRowHeight;
	rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
	TMainForm_DrawTreeCell(this, this->DGrid->Canvas, this->invertGridRow, &rect);
	if (topRow)
	{
		topRow = topRow > clientRows ? topRow - clientRows : 0;
		if (this->invertGridRow < topRow)
			this->invertGridRow = topRow;
		TDrawGrid_SetTopRow(this->DGrid, topRow);
	}
}
