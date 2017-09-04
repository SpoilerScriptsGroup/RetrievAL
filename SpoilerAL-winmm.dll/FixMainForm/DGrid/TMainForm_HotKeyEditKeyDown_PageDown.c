#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

void __stdcall TMainForm_HotKeyEditKeyDown_PageDown(TMainForm *this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        topRow;
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
	clientRows = clientHeight / this->DGrid->DefaultRowHeight;
	bottomRow = topRow + clientRows;
	if (this->invertGridRow < topRow)
	{
		this->invertGridRow = bottomRow;
		rect.top = clientRows * this->DGrid->DefaultRowHeight;
	}
	else
	{
		int row;

		rect.top = (this->invertGridRow - topRow) * this->DGrid->DefaultRowHeight;
		rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
		row = this->invertGridRow;
		if (this->invertGridRow < bottomRow)
		{
			int maxRow;

			this->invertGridRow += clientRows;
			maxRow = this->DGrid->RowCount - 2;
			if (this->invertGridRow > maxRow)
				this->invertGridRow = maxRow;
		}
		else
		{
			this->invertGridRow = bottomRow + clientRows - 1;
		}
		TMainForm_DrawTreeCell(this, this->DGrid->Canvas, row, &rect);
		rect.top = (this->invertGridRow - topRow) * this->DGrid->DefaultRowHeight;
	}
	rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
	TMainForm_DrawTreeCell(this, this->DGrid->Canvas, this->invertGridRow, &rect);
	if (topRow < (this->DGrid->RowCount > clientRows ? this->DGrid->RowCount - clientRows : 0))
	{
		int maxTopRow;

		topRow = bottomRow;
		maxTopRow = this->DGrid->RowCount - clientRows;
		if (topRow > maxTopRow)
			topRow = maxTopRow;
		TDrawGrid_SetTopRow(this->DGrid, topRow);
	}
}
