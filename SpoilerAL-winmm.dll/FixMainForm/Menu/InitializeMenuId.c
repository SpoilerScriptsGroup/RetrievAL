#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMainMenu.h"
#include "TMenuItem.h"

WORD wBegginerModeId;
WORD wDebuggerModeId;
WORD wMaxMenuId;

UINT __stdcall GetMaxMenuItemId(HMENU hMenu, UINT uMaxMenuId)
{
	MENUITEMINFOA mii;
	UINT          uCount;
	UINT          uItem;

	uCount = GetMenuItemCount(hMenu);
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_SUBMENU;
	for (uItem = 0; uItem < uCount; uItem++)
	{
		UINT uItemId;

		uItemId = GetMenuItemID(hMenu, uItem);
		if (uItemId != UINT_MAX && uMaxMenuId < uItemId)
			uMaxMenuId = uItemId;
		if (!GetMenuItemInfoA(hMenu, uItem, TRUE, &mii))
			continue;
		if (!mii.hSubMenu)
			continue;
		uMaxMenuId = GetMaxMenuItemId(mii.hSubMenu, uMaxMenuId);
	}
	return uMaxMenuId;
}

void __stdcall InitializeMenuId(TMainForm *MainForm)
{
	HMENU userModeHandle;

	userModeHandle = TMenuItem_GetHandle(MainForm->M_UserMode);
	wBegginerModeId = GetMenuItemID(userModeHandle, 0);
	wDebuggerModeId = GetMenuItemID(userModeHandle, 3);
	wMaxMenuId = GetMaxMenuItemId(TMainMenu_GetHandle(MainForm->MainMenu1), 0);
}
