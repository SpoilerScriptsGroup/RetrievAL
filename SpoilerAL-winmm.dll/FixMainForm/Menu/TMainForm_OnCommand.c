#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "intrinsic.h"
#include "TMainForm.h"
#include "TMenuItem.h"
#include "TComboBox.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

extern WORD wBegginerModeId;
extern WORD wDebuggerModeId;
extern WORD wMaxMenuId;
extern WORD wDebugWithoutMouseOverModeId;
extern WORD wToolMenuId;
extern char lpMenuProfileName[MAX_PATH];

void __cdecl UpdateUserModeMenu();

EXTERN_C void __stdcall TMainForm_OnCommand(HWND hWnd, WORD wNotifyCode, WORD wID, HWND hwndCtl)
{
	if (wNotifyCode != 0 || wID > wMaxMenuId)
		return;
	if (wID >= wBegginerModeId && wID <= wDebuggerModeId)
	{
		if (MainForm->userMode != 4)
			return;
		MainForm->userMode = wID - wBegginerModeId;
		UpdateUserModeMenu();
		if (MainForm->guideForm != NULL)
			TComboBox_SetItemIndex(MainForm->guideForm->UserModeCmbBox, MainForm->userMode);
	}
	else if (wID == wDebugWithoutMouseOverModeId)
	{
		if (MainForm->userMode == 4)
			return;
		MainForm->userMode = 4;
		UpdateUserModeMenu();
		if (MainForm->guideForm != NULL)
			TComboBox_SetItemIndex(MainForm->guideForm->UserModeCmbBox, MainForm->userMode);
	}
	else if (wID >= wToolMenuId)
	{
		char lpKeyName   [16];
		char lpOperation [MAX_PATH];
		char lpFile      [MAX_PATH];
		char lpParameters[MAX_PATH];
		char lpDirectory [MAX_PATH];
		char lpShowCmd   [20];
		char *p;
		UINT nShowCmd;

		_ultoa(wID - wToolMenuId, lpKeyName, 10);
		GetPrivateProfileStringA("Operation" , lpKeyName, "", lpOperation , _countof(lpOperation ), lpMenuProfileName);
		GetPrivateProfileStringA("File"      , lpKeyName, "", lpFile      , _countof(lpFile      ), lpMenuProfileName);
		GetPrivateProfileStringA("Parameters", lpKeyName, "", lpParameters, _countof(lpParameters), lpMenuProfileName);
		GetPrivateProfileStringA("Directory" , lpKeyName, "", lpDirectory , _countof(lpDirectory ), lpMenuProfileName);
		GetPrivateProfileStringA("ShowCmd"   , lpKeyName, "", lpShowCmd   , _countof(lpShowCmd   ), lpMenuProfileName);
		for (p = lpShowCmd; *p != '\0'; p++)
			*p = __intrinsic_toupper(*p);
		do
		{
			unsigned __int64 ull;
			char             *endptr;

			if (*(LPDWORD)lpShowCmd == BSWAP32('SW_H'))
			{
				if (*(LPDWORD)(lpShowCmd + 4) == BSWAP32('IDE\0'))
				{
					nShowCmd = SW_HIDE;
					break;
				}
			}
			else if (*(LPDWORD)lpShowCmd == BSWAP32('SW_M'))
			{
				if (*(LPDWORD)(lpShowCmd + 8) == BSWAP32('IZE\0'))
				{
					if (*(LPDWORD)(lpShowCmd + 4) == BSWAP32('AXIM'))
					{
						nShowCmd = SW_MAXIMIZE;
						break;
					}
					else if (*(LPDWORD)(lpShowCmd + 4) == BSWAP32('INIM'))
					{
						nShowCmd = SW_MINIMIZE;
						break;
					}
				}
			}
			else if (*(LPDWORD)lpShowCmd == BSWAP32('SW_R'))
			{
				if (*(LPDWORD)(lpShowCmd + 4) == BSWAP32('ESTO') &&
					(*(LPDWORD)(lpShowCmd + 8) & 0x00FFFFFF) == BSWAP24('RE\0'))
				{
					nShowCmd = SW_RESTORE;
					break;
				}
			}
			else if (*(LPDWORD)lpShowCmd == BSWAP32('SW_S'))
			{
				if (*(LPDWORD)(lpShowCmd + 4) == BSWAP32('HOW\0'))
				{
					nShowCmd = SW_SHOW;
					break;
				}
				else if (*(LPDWORD)(lpShowCmd + 4) == BSWAP32('HOWD'))
				{
					if (*(LPDWORD)(lpShowCmd + 8) == BSWAP32('EFAU') &&
						(*(LPDWORD)(lpShowCmd + 12) & 0x00FFFFFF) == BSWAP24('LT\0'))
					{
						nShowCmd = SW_SHOWDEFAULT;
						break;
					}
				}
				else if (*(LPDWORD)(lpShowCmd + 4) == BSWAP32('HOWM'))
				{
					if (*(LPDWORD)(lpShowCmd + 12) == BSWAP32('IZED'))
					{
						if (lpShowCmd[13] == '\0')
						{
							if (*(LPDWORD)(lpShowCmd + 8) == BSWAP32('AXIM'))
							{
								nShowCmd = SW_SHOWMAXIMIZED;
								break;
							}
							else if (*(LPDWORD)(lpShowCmd + 8) == BSWAP32('INIM'))
							{
								nShowCmd = SW_SHOWMINIMIZED;
								break;
							}
						}
					}
					else if (*(LPDWORD)(lpShowCmd + 8) == BSWAP32('INNO'))
					{
						if (*(LPDWORD)(lpShowCmd + 12) == BSWAP32('ACTI') &&
							(*(LPDWORD)(lpShowCmd + 16) & 0x00FFFFFF) == BSWAP24('VE\0'))
						{
							nShowCmd = SW_SHOWMINNOACTIVE;
							break;
						}
					}
				}
				else if (*(LPDWORD)(lpShowCmd + 4) == BSWAP32('HOWN'))
				{
					if (*(LPWORD)(lpShowCmd + 8) == BSWAP16('A\0'))
					{
						nShowCmd = SW_SHOWMINNOACTIVE;
						break;
					}
					else if (*(LPDWORD)(lpShowCmd + 8) == BSWAP32('OACT'))
					{
						if (*(LPDWORD)(lpShowCmd + 12) == BSWAP32('IVAT') &&
							*(LPWORD)(lpShowCmd + 16) == BSWAP16('E\0'))
						{
							nShowCmd = SW_SHOWNOACTIVATE;
							break;
						}
					}
					else if (*(LPDWORD)(lpShowCmd + 8) == BSWAP32('ORMA'))
					{
						if (*(LPWORD)(lpShowCmd + 12) == BSWAP16('L\0'))
						{
							nShowCmd = SW_SHOWNORMAL;
							break;
						}
					}
				}
			}
			else if (*(LPDWORD)lpShowCmd == BSWAP32('SW_F'))
			{
				if (*(LPDWORD)(lpShowCmd + 4) == BSWAP32('ORCE') &&
					*(LPDWORD)(lpShowCmd + 8) == BSWAP32('MINI') &&
					*(LPDWORD)(lpShowCmd + 12) == BSWAP32('MIZE') &&
					lpShowCmd[16] == '\0')
				{
					nShowCmd = SW_FORCEMINIMIZE;
					break;
				}
			}
			ull = _strtoui64(lpShowCmd, &endptr, 0);
			if (ull <= SW_MAX && *endptr == '\0')
				nShowCmd = (UINT)ull;
			else
				nShowCmd = SW_SHOWNORMAL;
		} while (0);
		if (!lpFile[0])
		{
			TSSGCtrl *ssgc = &MainForm->ssgCtrl;
			PathCombineA(lpFile, string_c_str(&ssgc->ssgDir), string_c_str(&ssgc->script.filePath));
		}
		ShellExecuteA(hWnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
	}
}
