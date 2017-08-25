#include <windows.h>
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"

__declspec(naked) void __cdecl TMainForm_HotKeyEditKeyDown_Down()
{
	static void __stdcall HotKeyEditKeyDown_Down(TMainForm *_this);

	__asm
	{
		#define ReturnAddress 00443484H
		#define _this         ebx

		push    _this
		push    ReturnAddress
		jmp     HotKeyEditKeyDown_Down

		#undef ReturnAddress
		#undef _this
	}
}

static void __stdcall HotKeyEditKeyDown_Down(TMainForm *_this)
{
	HWND       DGridHandle;
	RECT       rect;
	int        clientWidth, clientHeight;
	SCROLLINFO si;
	int        topRow;
	int        clientRows;

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
	do	/* do { ... } while (0); */
	{
		if (_this->invertGridRow < topRow)
		{
			_this->invertGridRow = topRow + 1;
			rect.top = _this->DGrid->DefaultRowHeight;
			rect.bottom = _this->DGrid->DefaultRowHeight * 2;
		}
		else
		{
			if (_this->invertGridRow >= _this->DGrid->RowCount - 2)
				break;
			rect.top = (_this->invertGridRow - topRow) * _this->DGrid->DefaultRowHeight;
			rect.bottom = rect.top + _this->DGrid->DefaultRowHeight;
			TMainForm_DrawTreeCell(_this, _this->DGrid->Canvas, _this->invertGridRow++, &rect);
			rect.top = rect.bottom;
			rect.bottom += _this->DGrid->DefaultRowHeight;
		}
		TMainForm_DrawTreeCell(_this, _this->DGrid->Canvas, _this->invertGridRow, &rect);
	} while (0);
	clientRows = clientHeight / _this->DGrid->DefaultRowHeight;
	if (topRow < _this->DGrid->RowCount - clientRows)
	{
		int bottomRow;

		bottomRow = topRow + clientRows;
		if (_this->invertGridRow > bottomRow)
		{
			_this->invertGridRow = bottomRow;
			rect.top = (_this->invertGridRow - topRow) * _this->DGrid->DefaultRowHeight;
			rect.bottom = rect.top + _this->DGrid->DefaultRowHeight;
			TMainForm_DrawTreeCell(_this, _this->DGrid->Canvas, _this->invertGridRow, &rect);
		}
		TDrawGrid_SetTopRow(_this->DGrid, topRow + 1);
	}
}
