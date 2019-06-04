#include <windows.h>
#include "TMainForm.h"
#include "TMenuItem.h"

extern WORD wNowValueDrawId;

EXTERN_C void __cdecl UpdateUserModeMenu()
{
	HMENU         hMenu;
	MENUITEMINFOA mii;

	hMenu = TMenuItem_GetHandle(MainForm->M_UserMode);
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE;
	for (UINT item = 0; item <= 5; item++)
	{
		mii.fState = item != MainForm->userMode ? MFS_UNCHECKED : MFS_CHECKED;
		SetMenuItemInfoA(hMenu, item, TRUE, &mii);
	}
	mii.fState = MainForm->isNowValueDraw ? MFS_CHECKED : MFS_UNCHECKED;
	SetMenuItemInfoA(TMenuItem_GetHandle(MainForm->M_View), wNowValueDrawId, FALSE, &mii);
}
