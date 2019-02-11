#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#define USING_NAMESPACE_BCB6_STD
#include "TWinControl.h"
#include "TMainForm.h"
#include "TSSCalc.h"
#include "TSSFloatCalc.h"

void __cdecl TMainForm_CalcButtonPushFunc(TMainForm *this, long BtnNum)
{
	extern HANDLE hHeap;
	const char *emptyString = "";

	HWND          edit;
	unsigned long type, start, end, length;
	const char    *replace;
	TSSGSubject   *TargetS;

	assert(sizeof(BtnNum) >= 2);

	edit = TWinControl_GetHandle(vector_at(&this->calcImage->valBox, 1).edit);
	TargetS = TSSGCtrl_GetTargetSubject(this->selectSubject);
	if (!TargetS)
		return;
	type = TSSGSubject_GetArgType(TargetS);
	if (type == atDOUBLE && this->isCalcHex) {
		TMainForm_GoCalcHexChange(this, FALSE);
		TCalcImage_SetStatus(this->calcImage, 18, TCalcImage_GetStatus(this->calcImage, 18) & 0xFE | !this->isCalcHex);
		TCalcImage_DrawBtn(this->calcImage, 18, FALSE);
	}
	switch (BtnNum) {
	case 0xA:
	case 0xB:
	case 0xC:
	case 0xD:
	case 0xE:
	case 0xF:
		if (!this->isCalcHex) {
			MessageBeep(-1);
			break;
		}
		BtnNum += 'A' - 0x0A - '0';
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		BtnNum += '0';
		SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)&BtnNum);
		break;
	case 16:// +/-
		if (this->isCalcHex) {
			MessageBeep(-1);
			break;
		}
		SendMessageA(edit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
		{
			char          head[2];
			unsigned long pos;

			if (GetWindowTextA(edit, head, 2) && *head == '-') {
				if (start)
					--start;
				if (end)
					--end;
				replace = emptyString;
				pos = 1;
			} else {
				if (start || start == end || end != GetWindowTextLengthA(edit)) {
					if (!++start)
						--start;
					if (!++end)
						--end;
				} else
					end = ULONG_MAX;
				replace = "-";
				pos = 0;
			}
			SendMessageA(edit, EM_SETSEL, 0, pos);
		}
		SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)replace);
		SendMessageA(edit, EM_SETSEL, start, end);
		break;
	case 17:// BS
		SendMessageA(edit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
		if (!end) {
			MessageBeep(-1);
			break;
		}
		// Ignore selection for convenience, use delete key or replaces by inputting.
		SendMessageA(edit, EM_SETSEL, end - 1, end);
		SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)emptyString);
		break;
	case 18:// 16/10
		if (type == atDOUBLE) {
			TCalcImage_SetStatus(this->calcImage, 18, TCalcImage_GetStatus(this->calcImage, 18) & 0xFE | this->isCalcHex);
			TCalcImage_DrawBtn(this->calcImage, 18, TRUE);
			MessageBeep(-1);
			break;
		}
		TMainForm_GoCalcHexChange(this, !this->isCalcHex);
		break;
	case 19:// enter
		TMainForm_GoCalcEnter(this);
		break;
	case 20:// min
	case 21:// max
		{
			char buffer[512];

			switch (type) {
			case atLONG:
				{
					TSSCalc *SSGS = (TSSCalc *)TargetS;
					if (this->isCalcHex)
						_snprintf(buffer, sizeof(buffer), "%0*X", TSSGSubject_GetSize(TargetS) << 1, BtnNum == 20 ? SSGS->min : SSGS->max);
					else
						_snprintf(buffer, sizeof(buffer), SSGS->isUnsigned ? "%u" : "%d", BtnNum == 20 ? SSGS->min : SSGS->max);
					break;
				}
			case atDOUBLE:
				{
					TSSFloatCalc *SSGS = (TSSFloatCalc *)TargetS;
					_snprintf(buffer, sizeof(buffer), "%f", BtnNum == 20 ? SSGS->min : SSGS->max);
					break;
				}
			default:
				return;
			}
			SendMessageA(edit, EM_SETSEL, 0, ULONG_MAX);
			SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)buffer);
		}
		break;
	case 22:// .
		if (type != atDOUBLE) {
			MessageBeep(-1);
			break;
		}
		replace = "0.";
		length = GetWindowTextLengthA(edit);
		SendMessageA(edit, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
		if (start || end < length) {
			char *lpMem;

			lpMem = (char *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, length + 1);
			if (lpMem) {
				const void *find;

				find = NULL;
				length = GetWindowTextA(edit, lpMem, length + 1);
				if (length != 1 || *lpMem != '-') {
					find = memchr(lpMem, '.', length);
					replace++;
				}
				HeapFree(hHeap, 0, lpMem);
				if (find)
					break;
			} else
				replace++;
		}
		SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)replace);
		break;
	}
}
