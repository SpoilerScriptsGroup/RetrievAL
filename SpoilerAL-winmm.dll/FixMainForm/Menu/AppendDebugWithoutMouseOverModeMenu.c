#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMenuItem.h"

extern WORD wMaxMenuId;
WORD wDebugWithoutMouseOverModeId;

BOOL __stdcall AppendDebugWithoutMouseOverModeMenu(TMainForm *MainForm)
{
	MENUITEMINFOA mii;

	wDebugWithoutMouseOverModeId = ++wMaxMenuId;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_SUBMENU | MIIM_TYPE;
	mii.fType = MFT_STRING | MFT_RADIOCHECK;
	mii.fMask = MIIM_ID | MIIM_TYPE;
	mii.wID = wDebugWithoutMouseOverModeId;
	mii.dwTypeData = "SSGデバッグモード - マウスオーバーを除く (&4)";
	mii.cch = 49;
	return InsertMenuItemA(TMenuItem_GetHandle(MainForm->M_UserMode), 4, TRUE, &mii);
}
