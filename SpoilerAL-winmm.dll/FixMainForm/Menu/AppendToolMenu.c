#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "TMainForm.h"
#undef MainForm
#include "TMainMenu.h"

extern WORD wMaxMenuId;
DWORD   dwToolMenuId = MAXDWORD;
wchar_t lpMenuProfileName[MAX_PATH] = { L'\0' };

BOOL __stdcall AppendToolMenu(TMainForm *MainForm)
{
	UINT          uItem;
	MENUITEMINFOW mii;
	wchar_t       lpKeyName[16];
	wchar_t       lpMenuString[MAX_PATH];
	HMENU         hPopupMenu;
	HMENU         hMenu;

	uItem = 0;
	mii.cch = GetPrivateProfileStringW(L"MenuString", _ultow(uItem, lpKeyName, 10), L"", lpMenuString, _countof(lpMenuString), lpMenuProfileName);
	if (mii.cch == 0)
		goto SUCCESS;
	hPopupMenu = CreatePopupMenu();
	if (hPopupMenu == NULL)
		goto FAILED1;
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_SUBMENU | MIIM_TYPE;
	mii.fType = MFT_STRING;
	mii.fMask = MIIM_ID | MIIM_TYPE;
	mii.wID = wMaxMenuId + 1;
	mii.dwTypeData = lpMenuString;
	for (; ; )
	{
		if (!InsertMenuItemW(hPopupMenu, uItem, TRUE, &mii))
			goto FAILED2;
		mii.cch = GetPrivateProfileStringW(L"MenuString", _ultow(++uItem, lpKeyName, 10), L"", lpMenuString, _countof(lpMenuString), lpMenuProfileName);
		if (mii.cch == 0)
			break;
		mii.wID++;
	}
	hMenu = TMainMenu_GetHandle(MainForm->MainMenu1);
	if (hMenu == NULL)
		goto FAILED2;
	mii.fMask = MIIM_SUBMENU | MIIM_TYPE;
	mii.hSubMenu = hPopupMenu;
	mii.dwTypeData = L"\x30C4\x30FC\x30EB(&T)";	// L"ÉcÅ[Éã(&T)"
	mii.cch = 7;
	if (!InsertMenuItemW(hMenu, 4, TRUE, &mii))
		goto FAILED2;
	dwToolMenuId = wMaxMenuId + 1;
	wMaxMenuId = mii.wID;
SUCCESS:
	return TRUE;

FAILED2:
	DestroyMenu(hPopupMenu);
FAILED1:
	return FALSE;
}
