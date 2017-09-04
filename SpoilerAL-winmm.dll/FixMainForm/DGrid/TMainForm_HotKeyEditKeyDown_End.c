#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

void __stdcall TMainForm_HotKeyEditKeyDown_End(TMainForm *this)
{
	HWND DGridHandle;
	RECT rect;
	int  clientWidth, clientHeight;
	int  topRow;
	int  maxRow;
	int  clientRows;

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	topRow = this->DGrid->TopRow >= 0 ? this->DGrid->TopRow : 0;
	maxRow = this->DGrid->RowCount > 2 ? this->DGrid->RowCount - 2 : 0;
	if (this->invertGridRow != maxRow)
	{
		RECT       rect;
		SCROLLINFO si;
		int        row;

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
		row = this->invertGridRow >= 0 ? this->invertGridRow : 0;
		this->invertGridRow = maxRow;
		rect.top = (row - topRow) * this->DGrid->DefaultRowHeight;
		rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
		TMainForm_DrawTreeCell(this, this->DGrid->Canvas, row, &rect);
		rect.top = (maxRow - topRow) * this->DGrid->DefaultRowHeight;
		rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
		TMainForm_DrawTreeCell(this, this->DGrid->Canvas, maxRow, &rect);
	}
	clientRows = clientHeight / this->DGrid->DefaultRowHeight;
	TDrawGrid_SetTopRow(this->DGrid, this->DGrid->RowCount > clientRows ? this->DGrid->RowCount - clientRows : 0);
}
