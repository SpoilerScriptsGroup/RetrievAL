#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TCanvas.h"
#include "TWinControl.h"
#include "TDrawGrid.h"
#include "TMainForm.h"
#include "TSSGSubject.h"
#include "SSGSubjectProperty.h"

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_FixDefaultColWidth()
{
	static void __stdcall FixDefaultColWidth(TSSGSubject *SSGS, LPVOID Canvas, int X, const string *Text);

	__asm
	{
		#define TCanvas_InternalTextOut 0055E418H
		#define Canvas                  eax
		#define X                       edx
		#define Y                       ecx
		#define SSGS                    edi
		#define DrawStr                 (ebp - 18H)

		push    TCanvas_InternalTextOut
		push    ecx
		mov     ecx, ebp
		push    eax
		sub     ecx, 24
		push    edx

		push    ecx
		push    X
		push    Canvas
		push    SSGS
		call    FixDefaultColWidth

		pop     edx
		pop     eax
		pop     ecx
		ret

		#undef TCanvas_InternalTextOut
		#undef Canvas
		#undef X
		#undef Y
		#undef SSGS
		#undef DrawStr
	}
}

static void __stdcall FixDefaultColWidth(TSSGSubject *SSGS, LPVOID Canvas, int X, const string *Text)
{
	SIZE      size;
	TMainForm *_this;

	GetTextExtentPoint32A(TCanvas_GetHandle(Canvas), Text->_M_start, string_length(Text), &size);
	size.cx += X + 6;
	if (SSGS)
	{
		TSSGSubjectProperty *elem;

		elem = GetSubjectProperty(SSGS);
		if (elem)
			elem->Width = size.cx;
	}
	else
	{
		TitleWidth = size.cx;
	}
	_this = MainForm;
	if (_this->DGrid->DefaultColWidth < size.cx)
	{
		HWND         DGridHandle;
		long         lStyle;
		unsigned int ClientHeight;
		unsigned int SubjectCount;
		unsigned int SubjectHeight;
		unsigned int FillHeight;
		int          wBar;

		DGridHandle = TWinControl_GetHandle(_this->DGrid);
		lStyle = GetWindowLongA(DGridHandle, GWL_STYLE);
		TDrawGrid_SetDefaultColWidth(_this->DGrid, size.cx);
		ClientHeight = TDrawGrid_GetClientHeight(_this->DGrid);
		SubjectCount = _this->DGrid->RowCount - 1;
		SubjectHeight = _this->DGrid->DefaultRowHeight * SubjectCount;
		if (ClientHeight >= SubjectHeight)
		{
			FillHeight = ClientHeight - SubjectHeight;
			wBar = SB_HORZ;
		}
		else
		{
			FillHeight = _this->DGrid->DefaultRowHeight + ClientHeight % _this->DGrid->DefaultRowHeight;
			wBar = SB_BOTH;
		}
		TDrawGrid_SetRowHeights(_this->DGrid, SubjectCount, FillHeight);
		ShowScrollBar(DGridHandle, wBar, TRUE);
		if (!(lStyle & WS_HSCROLL))
		{
			SCROLLINFO si;

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_RANGE;
			si.nMin = 0;
			si.nMax = _this->DGrid->DefaultColWidth - TDrawGrid_GetClientWidth(_this->DGrid);
			SetScrollInfo(DGridHandle, SB_HORZ, &si, FALSE);
		}
	}
}
