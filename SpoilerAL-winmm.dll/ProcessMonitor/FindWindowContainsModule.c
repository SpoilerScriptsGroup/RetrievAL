#include <windows.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include <regex.h>
#pragma comment(lib, "regex.lib")

#include "ProcessContainsModule.h"

typedef struct {
	BOOL    bIsRegex;
	size_t  nClassNameLength;
	LPCVOID lpClassName;
	size_t  nWindowNameLength;
	LPCVOID lpWindowName;
	LPCVOID lpModuleName;
	HWND    hWnd;
	DWORD   dwProcessId;
} ENUM_WINDOWS_PARAM;

static BOOL CALLBACK EnumWindowsProc(HWND hWnd, ENUM_WINDOWS_PARAM *param)
{
	char   lpString[1024];
	int    iLength;
	DWORD  dwProcessId;

	if (!param->bIsRegex)
	{
		iLength = GetClassNameA(hWnd, lpString, _countof(lpString));
		if (iLength != param->nClassNameLength)
			return TRUE;
		if (memcmp(lpString, param->lpClassName, iLength) != 0)
			return TRUE;
		if (param->lpWindowName)
		{
			iLength = GetWindowTextA(hWnd, lpString, _countof(lpString));
			if ((unsigned int)iLength < param->nWindowNameLength)
				return TRUE;
			if (memcmp(lpString, param->lpWindowName, param->nWindowNameLength) != 0)
				return TRUE;
		}
	}
	else
	{
		regmatch_t match;

		iLength = GetClassNameA(hWnd, lpString, _countof(lpString));
		if (!iLength)
			return TRUE;
		if (regexec(param->lpClassName, lpString, 1, &match, 0) != 0)
			return TRUE;
		if (match.rm_so || match.rm_eo != iLength)
			return TRUE;
		if (param->lpWindowName)
		{
			iLength = GetWindowTextA(hWnd, lpString, _countof(lpString));
			if ((unsigned int)iLength < param->nWindowNameLength)
				return TRUE;
			if (regexec(param->lpWindowName, lpString, 1, &match, 0) != 0)
				return TRUE;
			if (match.rm_so || match.rm_eo != iLength)
				return TRUE;
		}
	}
	if (!GetWindowThreadProcessId(hWnd, &dwProcessId))
		return TRUE;
	if (param->lpModuleName)
		if (!(param->bIsRegex ? (BOOL(__stdcall *)(DWORD, LPCVOID))ProcessContainsRegexModule : (BOOL(__stdcall *)(DWORD, LPCVOID))ProcessContainsModuleW)(dwProcessId, param->lpModuleName))
			return TRUE;
	param->hWnd = hWnd;
	param->dwProcessId = dwProcessId;
	return FALSE;
}

HWND __stdcall FindWindowContainsModule(
	IN           BOOL    bIsRegex,
	IN           LPCSTR  lpClassName,
	IN  OPTIONAL LPCSTR  lpWindowName,
	IN  OPTIONAL LPCSTR  lpModuleName,
	OUT OPTIONAL LPDWORD lpdwProcessId)
{
	ENUM_WINDOWS_PARAM param;
	wchar_t            lpWideCharStr[MAX_PATH];

	param.hWnd = NULL;
	param.dwProcessId = 0;
	if (!(param.lpModuleName = lpModuleName) ||
		!MultiByteToWideChar(CP_ACP, 0, lpModuleName, -1, (LPWSTR)(param.lpModuleName = lpWideCharStr), _countof(lpWideCharStr)))
	{
		if (!(param.bIsRegex = bIsRegex))
		{
			param.nClassNameLength  = strlen(lpClassName);
			param.lpClassName       = lpClassName;
			param.nWindowNameLength = lpWindowName ? strlen(lpWindowName) : 0;
			param.lpWindowName      = lpWindowName;
			EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&param);
		}
		else
		{
			regex_t regClassName;

			if (regcomp((regex_t *)(param.lpClassName = &regClassName), lpClassName, REG_EXTENDED) == 0)
			{
				regex_t regWindowName;

				if (!(param.lpWindowName = lpWindowName) ||
					regcomp((regex_t *)(param.lpWindowName = &regWindowName), lpWindowName, REG_EXTENDED) == 0)
				{
					regex_t regModuleName;

					if (!(param.lpModuleName = lpModuleName) ||
						regcomp((regex_t *)(param.lpModuleName = &regModuleName), lpModuleName, REG_EXTENDED | REG_ICASE) == 0)
					{
						param.nClassNameLength  = 0;
						param.nWindowNameLength = 0;
						EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&param);
						if (lpModuleName)
							regfree(&regModuleName);
					}
					if (lpWindowName)
						regfree(&regWindowName);
				}
				regfree(&regClassName);
			}
		}
	}
	if (lpdwProcessId)
		*lpdwProcessId = param.dwProcessId;
	return param.hWnd;
}
