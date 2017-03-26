#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

void __stdcall TMainForm_HotKeyEditKeyDown_End(TMainForm *_this)
{
	HWND DGridHandle;
	RECT rect;
	int  clientWidth, clientHeight;
	int  topRow;
	int  maxRow;
	int  clientRows;

	DGridHandle = TWinControl_GetHandle(_this->DGrid);
	GetClientRect(DGridHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	topRow = _this->DGrid->TopRow >= 0 ? _this->DGrid->TopRow : 0;
	maxRow = _this->DGrid->RowCount > 2 ? _this->DGrid->RowCount - 2 : 0;
	if (_this->invertGridRow != maxRow)
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
					rect.left = -MulDiv(_this->DGrid->DefaultColWidth - clientWidth, pos, range);
				}
			}
		}
		rect.right = rect.left + _this->DGrid->DefaultColWidth;
		row = _this->invertGridRow >= 0 ? _this->invertGridRow : 0;
		_this->invertGridRow = maxRow;
		rect.top = (row - topRow) * _this->DGrid->DefaultRowHeight;
		rect.bottom = rect.top + _this->DGrid->DefaultRowHeight;
		TMainForm_DrawTreeCell(_this, _this->DGrid->Canvas, row, &rect);
		rect.top = (maxRow - topRow) * _this->DGrid->DefaultRowHeight;
		rect.bottom = rect.top + _this->DGrid->DefaultRowHeight;
		TMainForm_DrawTreeCell(_this, _this->DGrid->Canvas, maxRow, &rect);
	}
	clientRows = clientHeight / _this->DGrid->DefaultRowHeight;
	TDrawGrid_SetTopRow(_this->DGrid, _this->DGrid->RowCount > clientRows ? _this->DGrid->RowCount - clientRows : 0);
}
