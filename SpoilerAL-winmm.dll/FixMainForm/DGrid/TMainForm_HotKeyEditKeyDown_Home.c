#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

void __stdcall TMainForm_HotKeyEditKeyDown_Home(TMainForm *_this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth;
	SCROLLINFO si;
	int        row;
	int        topRow;

	DGridHandle = TWinControl_GetHandle(_this->DGrid);
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
				rect.left = -MulDiv(_this->DGrid->DefaultColWidth - clientWidth, pos, range);
			}
		}
	}
	rect.right = rect.left + _this->DGrid->DefaultColWidth;
	row = _this->invertGridRow >= 0 ? _this->invertGridRow : 0;
	_this->invertGridRow = 0;
	topRow = _this->DGrid->TopRow >= 0 ? _this->DGrid->TopRow : 0;
	rect.top = (row - topRow) * _this->DGrid->DefaultRowHeight;
	rect.bottom = rect.top + _this->DGrid->DefaultRowHeight;
	TMainForm_DrawTreeCell(_this, _this->DGrid->Canvas, row, &rect);
	rect.top = -topRow * _this->DGrid->DefaultRowHeight;
	rect.bottom = rect.top + _this->DGrid->DefaultRowHeight;
	TMainForm_DrawTreeCell(_this, _this->DGrid->Canvas, 0, &rect);
	TDrawGrid_SetTopRow(_this->DGrid, 0);
}
