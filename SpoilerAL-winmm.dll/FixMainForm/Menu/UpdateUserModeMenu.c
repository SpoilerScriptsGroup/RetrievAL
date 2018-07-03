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
	mii.fState = MFS_UNCHECKED;
	if (MainForm->userMode == 4)
	{
		SetMenuItemInfoA(hMenu, 0, TRUE, &mii);
		SetMenuItemInfoA(hMenu, 1, TRUE, &mii);
		SetMenuItemInfoA(hMenu, 2, TRUE, &mii);
		SetMenuItemInfoA(hMenu, 3, TRUE, &mii);
	}
	else
	{
		SetMenuItemInfoA(hMenu, 4, TRUE, &mii);
	}
	mii.fState = MFS_CHECKED;
	SetMenuItemInfoA(hMenu, MainForm->userMode, TRUE, &mii);

	mii.fState = MainForm->isNowValueDraw ? MFS_CHECKED : MFS_UNCHECKED;
	SetMenuItemInfoA(TMenuItem_GetHandle(MainForm->M_View), wNowValueDrawId, FALSE, &mii);
}
