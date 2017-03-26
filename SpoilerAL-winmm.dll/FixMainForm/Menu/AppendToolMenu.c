#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMainMenu.h"

extern WORD wMaxMenuId;
WORD wToolMenuId;
char lpMenuProfileName[MAX_PATH] = { '\0' };

BOOL __stdcall AppendToolMenu(TMainForm *MainForm)
{
	UINT          uItem;
	MENUITEMINFOA mii;
	char          lpKeyName[16];
	char          lpMenuString[MAX_PATH];
	HMENU         hPopupMenu;
	HMENU         hMenu;

	uItem = 0;
	mii.cch = GetPrivateProfileStringA("MenuString", _ultoa(uItem, lpKeyName, 10), "", lpMenuString, _countof(lpMenuString), lpMenuProfileName);
	if (mii.cch == 0)
		goto SUCCESS;
	hPopupMenu = CreatePopupMenu();
	if (hPopupMenu == NULL)
		goto FAILED1;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_SUBMENU | MIIM_TYPE;
	mii.fType = MFT_STRING;
	mii.fMask = MIIM_ID | MIIM_TYPE;
	mii.wID = wToolMenuId = wMaxMenuId + 1;
	mii.dwTypeData = lpMenuString;
	for (; ; )
	{
		if (!InsertMenuItemA(hPopupMenu, uItem, TRUE, &mii))
			goto FAILED2;
		mii.cch = GetPrivateProfileStringA("MenuString", _ultoa(++uItem, lpKeyName, 10), "", lpMenuString, _countof(lpMenuString), lpMenuProfileName);
		if (mii.cch == 0)
			break;
		mii.wID++;
	}
	hMenu = TMainMenu_GetHandle(MainForm->MainMenu1);
	if (hMenu == NULL)
		goto FAILED2;
	mii.fMask = MIIM_SUBMENU | MIIM_TYPE;
	mii.hSubMenu = hPopupMenu;
	mii.dwTypeData = "ÉcÅ[Éã(&T)";
	mii.cch = 10;
	if (!InsertMenuItemA(hMenu, 4, TRUE, &mii))
		goto FAILED2;
	wMaxMenuId = mii.wID;
SUCCESS:
	return TRUE;

FAILED2:
	DestroyMenu(hPopupMenu);
FAILED1:
	return FALSE;
}
