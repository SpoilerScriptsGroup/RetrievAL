#include <windows.h>
#include <assert.h>
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TApplication.h"
#include "TBitmap.h"
#include "TCanvas.h"
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"
#include "TSSGSubject.h"
#include "TSSGCtrl.h"
#include "TSSGScriptStruct.h"
#include "TSSSplit.h"
#include "WaitCursor.h"
#include "SSGSubjectProperty.h"

void __cdecl TMainForm_CheckTreeSize(TMainForm *_this, BOOLEAN AllWidthCheck)
{
	static BOOL   InProcessing = FALSE;
	HWND          DGridHandle;
	LPVOID        BSCanvas;
	SIZE          TextSize;
	unsigned long RowHeight;
	unsigned long SubjectCount;
	unsigned long SubjectWidth;
	unsigned long SubjectHeight;
	unsigned long cx;
	unsigned long cy;
	unsigned int  uShow, uHide;
	unsigned int  FillHeight;

	#define lpszDagger      (LPCSTR)0x0060313B
	#define ssgCtrl_stSPLIT 16

	// cancel recursive call
	if (InProcessing)
		return;
	InProcessing = TRUE;

	// set wait cursor
	BeginWaitCursor();

	// suspend redraw
	DGridHandle = TWinControl_GetHandle(_this->DGrid);
	SendMessageA(DGridHandle, WM_SETREDRAW, FALSE, 0);

	// 文字サイズ基準の調整
	BSCanvas = TBitmap_GetCanvas(_this->treeBackScreen);
	GetTextExtentPoint32A(TCanvas_GetHandle(BSCanvas), lpszDagger, 2, &TextSize);
	_this->treeFontWidth = TextSize.cx;
	RowHeight = TextSize.cy + 8;
	TDrawGrid_SetDefaultRowHeight(_this->DGrid, RowHeight);

	// 横方向のサイズ調整
	SubjectCount = bcb6_std_vector_size(&_this->treeSubjectVec, TSSGSubject *) + 1;
	if (SubjectCount + 1 != (unsigned long)_this->DGrid->RowCount)
		TDrawGrid_SetRowCount(_this->DGrid, SubjectCount + 1);

	// calculate max width from all rows
	SubjectWidth = 0;
	for (TSSGSubject **it = (TSSGSubject **)_this->treeSubjectVec._M_start; it != _this->treeSubjectVec._M_finish; it++)
	{
		TSSGSubjectProperty *elem;

		elem = GetSubjectProperty(*it);
		if (elem == NULL)
			continue;
		if (SubjectWidth < elem->Width)
			SubjectWidth = elem->Width;
	}

	// calculate height of all rows
	SubjectHeight = SubjectCount * RowHeight;

	// calculate client area without scrollbar
	cx = _this->DGrid->Width;
	cy = _this->DGrid->Height;
	cx -= (unsigned int)GetSystemMetrics(SM_CXEDGE) * 2;
	cy -= (unsigned int)GetSystemMetrics(SM_CYEDGE) * 2;

	// subtract scrollbar size
	if (cx >= SubjectWidth)
	{
		if (cy < SubjectHeight)
		{
			cx -= GetSystemMetrics(SM_CXVSCROLL);
			if (cx < SubjectWidth)
				goto SUBTRACT_CYHSCROLL;
		}
		uShow = 0;
	}
	else
	{
	SUBTRACT_CYHSCROLL:
		cx = SubjectWidth;
		cy -= GetSystemMetrics(SM_CYHSCROLL);
		uShow = SB_HORZ + 1;
	}
	TDrawGrid_SetDefaultColWidth(_this->DGrid, cx);

	// filling free space by last row
	if (cy >= SubjectHeight)
	{
		TDrawGrid_SetTopRow(_this->DGrid, 0);
		FillHeight = cy - SubjectHeight;
	}
	else
	{
		FillHeight = RowHeight + cy % RowHeight;
		uShow |= SB_VERT + 1;
	}
	TDrawGrid_SetRowHeights(_this->DGrid, SubjectCount, FillHeight);

	assert(SB_HORZ + 1 == 1);
	assert(SB_VERT + 1 == 2);
	assert(SB_BOTH     == 3);

	// update scroll bar
	uHide = ~uShow;
	if (uHide &= SB_BOTH)
		ShowScrollBar(DGridHandle, uHide - (uHide != SB_BOTH), FALSE);
	if (uShow)
		ShowScrollBar(DGridHandle, uShow - (uShow != SB_BOTH), TRUE);

	// resume redraw
	SendMessageA(DGridHandle, WM_SETREDRAW, TRUE, 0);
	InvalidateRect(DGridHandle, NULL, FALSE);

	// restore cursor
	EndWaitCursor();

	// restore static flag
	InProcessing = FALSE;
}
