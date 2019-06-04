#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMenuItem.h"

extern WORD wMaxMenuId;
WORD wDebugWithoutMouseOverModeId;
WORD wDebugWithoutCalculationId;
WORD wCollapseDirsId;
WORD wNowValueDrawId;

BOOL __stdcall AppendDebugWithoutMouseOverModeMenu(TMainForm *MainForm)
{
	MENUITEMINFOA mii;

	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_ID | MIIM_TYPE;
	mii.fType = MFT_STRING | MFT_RADIOCHECK;
	mii.wID = wDebugWithoutMouseOverModeId = ++wMaxMenuId;
	mii.hbmpChecked = NULL;
	mii.dwTypeData = "SSGデバッグモード - マウスオーバーを除く (&4)";
	mii.cch = 45;
	if (!InsertMenuItemA(TMenuItem_GetHandle(MainForm->M_UserMode), 4, TRUE, &mii))
		return FALSE;

	//mii.cbSize = sizeof(mii);
	//mii.fMask = MIIM_ID | MIIM_TYPE;
	//mii.fType = MFT_STRING | MFT_RADIOCHECK;
	mii.wID = wDebugWithoutCalculationId = ++wMaxMenuId;
	//mii.hbmpChecked = NULL;
	mii.dwTypeData = "SSGデバッグモード - マウスオーバー、演算を除く (&5)";
	mii.cch = 51;
	if (!InsertMenuItemA(TMenuItem_GetHandle(MainForm->M_UserMode), 5, TRUE, &mii))
		return FALSE;

	//mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_ID | MIIM_STRING | MIIM_FTYPE;
	mii.fType = MFT_STRING;
	mii.wID = wCollapseDirsId = ++wMaxMenuId;
	//mii.hbmpChecked = NULL;
	mii.dwTypeData = "階層を畳む(&D)";
	mii.cch = 14;
	if (!InsertMenuItemA(TMenuItem_GetHandle(MainForm->M_View), 0, TRUE, &mii))
		return FALSE;

	//mii.cbSize = sizeof(mii);
	//mii.fMask = MIIM_ID | MIIM_STRING | MIIM_FTYPE;
	//mii.fType = MFT_STRING;
	mii.wID = wNowValueDrawId = ++wMaxMenuId;
	//mii.hbmpChecked = NULL;
	mii.dwTypeData = "現在値の描写(&V)";
	mii.cch = 11;
	return InsertMenuItemA(TMenuItem_GetHandle(MainForm->M_View), -1, TRUE, &mii);
}
