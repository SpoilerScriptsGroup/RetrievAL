#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <shlwapi.h>
#define USING_NAMESPACE_BCB6_STD
#include "intrinsic.h"
#include "TMainForm.h"
#include "TMenuItem.h"
#include "TComboBox.h"

extern WORD    wBegginerModeId;
extern WORD    wDebuggerModeId;
extern WORD    wMaxMenuId;
extern WORD    wDebugWithoutMouseOverModeId;
extern WORD    wDebugWithoutCalculationId;
extern WORD    wCollapseDirsId;
extern WORD    wNowValueDrawId;
extern DWORD   dwToolMenuId;
extern wchar_t lpMenuProfileName[MAX_PATH];

void __cdecl UpdateUserModeMenu();

void(__cdecl * const TSSGCtrl_ChangeDirectorySubject)(
	TSSGCtrl            *SSGC,
	TSSGSubject         *SSGS,
	vector_PTSSGSubject *TreeVec,
	unsigned long        SSGSIndex) = (LPVOID)0x00502964;

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
	else if (wID == wDebugWithoutCalculationId)
	{
		if (MainForm->userMode == 5)
			return;
		MainForm->userMode = 5;
		UpdateUserModeMenu();
		if (MainForm->guideForm != NULL)
			TComboBox_SetItemIndex(MainForm->guideForm->UserModeCmbBox, MainForm->userMode);
	}
	else if (wID == wCollapseDirsId) {
		for (TSSGSubject** it = vector_end(&MainForm->treeSubjectVec);
			 --it >= vector_begin(&MainForm->treeSubjectVec);
			 ) {
			if ((*it)->type == stDIR && (*it)->isOpen)
				TSSGCtrl_ChangeDirectorySubject(
					&MainForm->ssgCtrl,
					*it,
					&MainForm->treeSubjectVec,
					it - vector_begin(&MainForm->treeSubjectVec) + 1);
		}
		TMainForm_CheckTreeSize(MainForm, TRUE);
	}
	else if (wID == wNowValueDrawId) {
		MainForm->isNowValueDraw = !MainForm->isNowValueDraw;
		UpdateUserModeMenu();
	}
	else if (wID >= dwToolMenuId)
	{
		wchar_t lpKeyName   [16];
		wchar_t lpOperation [MAX_PATH];
		wchar_t lpFile      [MAX_PATH];
		wchar_t lpParameters[MAX_PATH];
		wchar_t lpDirectory [MAX_PATH];
		wchar_t lpShowCmd   [20];
		wchar_t *p;
		UINT    nShowCmd;

		_ultow(wID - dwToolMenuId, lpKeyName, 10);
		GetPrivateProfileStringW(L"Operation" , lpKeyName, L"", lpOperation , _countof(lpOperation ), lpMenuProfileName);
		GetPrivateProfileStringW(L"File"      , lpKeyName, L"", lpFile      , _countof(lpFile      ), lpMenuProfileName);
		GetPrivateProfileStringW(L"Parameters", lpKeyName, L"", lpParameters, _countof(lpParameters), lpMenuProfileName);
		GetPrivateProfileStringW(L"Directory" , lpKeyName, L"", lpDirectory , _countof(lpDirectory ), lpMenuProfileName);
		GetPrivateProfileStringW(L"ShowCmd"   , lpKeyName, L"", lpShowCmd   , _countof(lpShowCmd   ), lpMenuProfileName);
		for (p = lpShowCmd; *p; p++)
			*p = __intrinsic_toupper(*p);
		do	/* do { ... } while (0); */
		{
			wchar_t *endptr;

			if (*(LPDWORD)lpShowCmd == MAKELONG(L'S', L'W'))
			{
				if (*(LPDWORD)(lpShowCmd + 2) == MAKELONG(L'_', L'H'))
				{
					if (*(LPDWORD)(lpShowCmd + 4) == MAKELONG(L'I', L'D') &&
						*(LPDWORD)(lpShowCmd + 6) == MAKELONG(L'E', L'\0'))
					{
						nShowCmd = SW_HIDE;
						break;
					}
				}
				else if (*(LPDWORD)(lpShowCmd + 2) == MAKELONG(L'_', L'M'))
				{
					if (*(LPDWORD)(lpShowCmd + 4) == MAKELONG(L'A', L'X'))
					{
						if (*(LPDWORD)(lpShowCmd +  6) == MAKELONG(L'I', L'M') &&
							*(LPDWORD)(lpShowCmd +  8) == MAKELONG(L'I', L'Z') &&
							*(LPDWORD)(lpShowCmd + 10) == MAKELONG(L'E', L'\0'))
						{
							nShowCmd = SW_MAXIMIZE;
							break;
						}
					}
					else if (*(LPDWORD)(lpShowCmd + 4) == MAKELONG(L'I', L'N'))
					{
						if (*(LPDWORD)(lpShowCmd +  6) == MAKELONG(L'I', L'M') &&
							*(LPDWORD)(lpShowCmd +  8) == MAKELONG(L'I', L'Z') &&
							*(LPDWORD)(lpShowCmd + 10) == MAKELONG(L'E', L'\0'))
						{
							nShowCmd = SW_MINIMIZE;
							break;
						}
					}
				}
				else if (*(LPDWORD)(lpShowCmd + 2) == MAKELONG(L'_', L'R'))
				{
					if (*(LPDWORD)(lpShowCmd +  4) == MAKELONG(L'E', L'S') &&
						*(LPDWORD)(lpShowCmd +  6) == MAKELONG(L'T', L'O') &&
						*(LPDWORD)(lpShowCmd +  8) == MAKELONG(L'R', L'E') &&
						*         (lpShowCmd + 10) ==          L'\0')
					{
						nShowCmd = SW_RESTORE;
						break;
					}
				}
				else if (*(LPDWORD)(lpShowCmd + 2) == MAKELONG(L'_', L'S'))
				{
					if (*(LPDWORD)(lpShowCmd + 4) == MAKELONG(L'H', L'O'))
					{
						if (*(LPDWORD)(lpShowCmd + 6) == MAKELONG(L'W', L'\0'))
						{
							nShowCmd = SW_SHOW;
							break;
						}
						else if (*(LPDWORD)(lpShowCmd + 6) == MAKELONG(L'W', L'D'))
						{
							if (*(LPDWORD)(lpShowCmd +  8) == MAKELONG(L'E', L'F') &&
								*(LPDWORD)(lpShowCmd + 10) == MAKELONG(L'A', L'U') &&
								*(LPDWORD)(lpShowCmd + 12) == MAKELONG(L'L', L'T') &&
								*         (lpShowCmd + 14) ==          L'\0')
							{
								nShowCmd = SW_SHOWDEFAULT;
								break;
							}
						}
						else if (*(LPDWORD)(lpShowCmd + 6) == MAKELONG(L'W', L'M'))
						{
							if (*(LPDWORD)(lpShowCmd + 8) == MAKELONG(L'A', L'X'))
							{
								if (*(LPDWORD)(lpShowCmd + 10) == MAKELONG(L'I', L'M') &&
									*(LPDWORD)(lpShowCmd + 12) == MAKELONG(L'I', L'Z') &&
									*(LPDWORD)(lpShowCmd + 14) == MAKELONG(L'E', L'D') &&
									*         (lpShowCmd + 16) ==          L'\0')
								{
									nShowCmd = SW_SHOWMAXIMIZED;
									break;
								}
							}
							else if (*(LPDWORD)(lpShowCmd + 8) == MAKELONG(L'I', L'N'))
							{
								if (*(LPDWORD)(lpShowCmd + 10) == MAKELONG(L'I', L'M') &&
									*(LPDWORD)(lpShowCmd + 12) == MAKELONG(L'I', L'Z') &&
									*(LPDWORD)(lpShowCmd + 14) == MAKELONG(L'E', L'D') &&
									*         (lpShowCmd + 16) ==          L'\0')
								{
									nShowCmd = SW_SHOWMINIMIZED;
									break;
								}
							}
							else if (*(LPDWORD)(lpShowCmd + 8) == MAKELONG(L'I', L'N'))
							{
								if (*(LPDWORD)(lpShowCmd + 10) == MAKELONG(L'N', L'O') &&
									*(LPDWORD)(lpShowCmd + 12) == MAKELONG(L'A', L'C') &&
									*(LPDWORD)(lpShowCmd + 14) == MAKELONG(L'T', L'I') &&
									*(LPDWORD)(lpShowCmd + 16) == MAKELONG(L'V', L'E') &&
									*         (lpShowCmd + 18) ==          L'\0')
								{
									nShowCmd = SW_SHOWMINNOACTIVE;
									break;
								}
							}
						}
						else if (*(LPDWORD)(lpShowCmd + 6) == MAKELONG(L'W', L'N'))
						{
							if (*(LPWORD)(lpShowCmd + 8) == MAKELONG(L'A', L'\0'))
							{
								nShowCmd = SW_SHOWNA;
								break;
							}
							else if (*(LPDWORD)(lpShowCmd + 8) == MAKELONG(L'O', L'A'))
							{
								if (*(LPDWORD)(lpShowCmd + 10) == MAKELONG(L'C', L'T') &&
									*(LPDWORD)(lpShowCmd + 12) == MAKELONG(L'I', L'V') &&
									*(LPDWORD)(lpShowCmd + 14) == MAKELONG(L'A', L'T') &&
									*(LPDWORD)(lpShowCmd + 16) == MAKELONG(L'E', L'\0'))
								{
									nShowCmd = SW_SHOWNOACTIVATE;
									break;
								}
							}
							else if (*(LPDWORD)(lpShowCmd + 8) == MAKELONG(L'O', L'R'))
							{
								if (*(LPDWORD)(lpShowCmd + 10) == MAKELONG(L'M', L'A') &&
									*(LPDWORD)(lpShowCmd + 12) == MAKELONG(L'L', L'\0'))
								{
									nShowCmd = SW_SHOWNORMAL;
									break;
								}
							}
						}
					}
				}
				else if (*(LPDWORD)(lpShowCmd + 2) == MAKELONG(L'_', L'F'))
				{
					if (*(LPDWORD)(lpShowCmd +  4) == MAKELONG(L'O', L'R') &&
						*(LPDWORD)(lpShowCmd +  6) == MAKELONG(L'C', L'E') &&
						*(LPDWORD)(lpShowCmd +  8) == MAKELONG(L'M', L'I') &&
						*(LPDWORD)(lpShowCmd + 10) == MAKELONG(L'N', L'I') &&
						*(LPDWORD)(lpShowCmd + 12) == MAKELONG(L'M', L'I') &&
						*(LPDWORD)(lpShowCmd + 14) == MAKELONG(L'Z', L'E') &&
						*         (lpShowCmd + 16) ==          L'\0')
					{
						nShowCmd = SW_FORCEMINIMIZE;
						break;
					}
				}
			}
			errno = 0;
			nShowCmd = wcstoul(lpShowCmd, &endptr, 0);
			if (nShowCmd > SW_MAX || errno)
				nShowCmd = SW_SHOWNORMAL;
		} while (0);
		if (!*lpFile)
		{
			TSSGCtrl     *ssgc;
			wchar_t      ssgcDir[MAX_PATH];
			wchar_t      ssgcFile[MAX_PATH];
			unsigned int cchMultiByte;

			ssgc = &MainForm->ssgCtrl;
			cchMultiByte = MultiByteToWideChar(CP_THREAD_ACP, 0, string_c_str(&ssgc->ssgDir), -1, NULL, 0);
			if (cchMultiByte > _countof(ssgcDir))
				return;
			MultiByteToWideChar(CP_THREAD_ACP, 0, string_c_str(&ssgc->ssgDir), -1, ssgcDir, _countof(ssgcDir));
			cchMultiByte = MultiByteToWideChar(CP_THREAD_ACP, 0, string_c_str(&ssgc->script.filePath), -1, NULL, 0);
			if (cchMultiByte > _countof(ssgcFile))
				return;
			MultiByteToWideChar(CP_THREAD_ACP, 0, string_c_str(&ssgc->script.filePath), -1, ssgcFile, _countof(ssgcFile));
			PathCombineW(lpFile, ssgcDir, ssgcFile);
		}
		ShellExecuteW(hWnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
	}
}
