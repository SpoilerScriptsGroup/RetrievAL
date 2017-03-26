#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

void __stdcall TMainForm_HotKeyEditKeyDown_PageUp(TMainForm *_this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        topRow;
	int        row;
	int        clientRows;
	int        bottomRow;

	DGridHandle = TWinControl_GetHandle(_this->DGrid);
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
				rect.left = -MulDiv(_this->DGrid->DefaultColWidth - clientWidth, pos, range);
			}
		}
	}
	rect.right = rect.left + _this->DGrid->DefaultColWidth;
	if (_this->invertGridRow < 0)
		_this->invertGridRow = 0;
	topRow = _this->DGrid->TopRow >= 0 ? _this->DGrid->TopRow : 0;
	rect.top = (_this->invertGridRow - topRow) * _this->DGrid->DefaultRowHeight;
	rect.bottom = rect.top + _this->DGrid->DefaultRowHeight;
	row = _this->invertGridRow;
	clientRows = clientHeight / _this->DGrid->DefaultRowHeight;
	_this->invertGridRow = _this->invertGridRow > clientRows ? _this->invertGridRow - clientRows : 0;
	TMainForm_DrawTreeCell(_this, _this->DGrid->Canvas, row, &rect);
	bottomRow = topRow - 1;
	if (bottomRow < clientRows)
		bottomRow = clientRows;
	if (_this->invertGridRow > bottomRow)
		_this->invertGridRow = bottomRow;
	rect.top = (_this->invertGridRow - topRow) * _this->DGrid->DefaultRowHeight;
	rect.bottom = rect.top + _this->DGrid->DefaultRowHeight;
	TMainForm_DrawTreeCell(_this, _this->DGrid->Canvas, _this->invertGridRow, &rect);
	if (topRow)
	{
		topRow = topRow > clientRows ? topRow - clientRows : 0;
		if (_this->invertGridRow < topRow)
			_this->invertGridRow = topRow;
		TDrawGrid_SetTopRow(_this->DGrid, topRow);
	}
}
