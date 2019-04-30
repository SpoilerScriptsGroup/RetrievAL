#include <windows.h>
#include "TMainForm.h"
#include "TWinControl.h"
#include "ApplicationMessage.h"

void __cdecl TSearchReportListner_OnReport_InvokeDrawProgress(WPARAM searchForm, unsigned long Pos)
{
	PostMessageA(TWinControl_GetHandle(MainForm), WM_DRAW_PROGRESS, searchForm, Pos);
}
