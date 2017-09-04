#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

void __stdcall TMainForm_HotKeyEditKeyDown_Home(TMainForm *this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth;
	SCROLLINFO si;
	int        row;
	int        topRow;

	DGridHandle = TWinControl_GetHandle(this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
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
	this->invertGridRow = 0;
	topRow = this->DGrid->TopRow >= 0 ? this->DGrid->TopRow : 0;
	rect.top = (row - topRow) * this->DGrid->DefaultRowHeight;
	rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
	TMainForm_DrawTreeCell(this, this->DGrid->Canvas, row, &rect);
	rect.top = -topRow * this->DGrid->DefaultRowHeight;
	rect.bottom = rect.top + this->DGrid->DefaultRowHeight;
	TMainForm_DrawTreeCell(this, this->DGrid->Canvas, 0, &rect);
	TDrawGrid_SetTopRow(this->DGrid, 0);
}
