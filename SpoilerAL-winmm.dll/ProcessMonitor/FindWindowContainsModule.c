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
	size_t nLength;
	DWORD  dwProcessId;

	if (!param->bIsRegex)
	{
		nLength = GetClassNameA(hWnd, lpString, _countof(lpString));
		if (nLength != param->nClassNameLength)
			return TRUE;
		if (memcmp(lpString, param->lpClassName, nLength) != 0)
			return TRUE;
		if (param->lpWindowName)
		{
			nLength = GetWindowTextA(hWnd, lpString, _countof(lpString));
			if (nLength < param->nWindowNameLength)
				return TRUE;
			if (memcmp(lpString, param->lpWindowName, param->nWindowNameLength) != 0)
				return TRUE;
		}
	}
	else
	{
		regmatch_t match;

		if (!GetClassNameA(hWnd, lpString, _countof(lpString)))
			return TRUE;
		if (regexec(param->lpClassName, lpString, 1, &match, 0) != 0)
			return TRUE;
		if (param->lpWindowName)
		{
			nLength = GetWindowTextA(hWnd, lpString, _countof(lpString));
			if (nLength < param->nWindowNameLength)
				return TRUE;
			if (regexec(param->lpWindowName, lpString, 1, &match, 0) != 0)
				return TRUE;
		}
	}
	if (!GetWindowThreadProcessId(hWnd, &dwProcessId))
		return TRUE;
	if (param->lpModuleName)
	{
		if (!param->bIsRegex)
		{
			if (!ProcessContainsModuleW(dwProcessId, param->lpModuleName))
				return TRUE;
		}
		else
		{
			if (!ProcessContainsRegexModule(dwProcessId, param->lpModuleName))
				return TRUE;
		}
	}
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
	regex_t            regClassName;
	regex_t            regWindowName;
	regex_t            regModuleName;
	wchar_t            lpWideCharStr[MAX_PATH];

	if (lpModuleName && !MultiByteToWideChar(CP_ACP, 0, lpModuleName, -1, lpWideCharStr, _countof(lpWideCharStr)))
		return NULL;
	if (!(param.bIsRegex = bIsRegex))
	{
		param.nClassNameLength  = strlen(lpClassName);
		param.lpClassName       = lpClassName;
		param.nWindowNameLength = lpWindowName ? strlen(lpWindowName) : 0;
		param.lpWindowName      = lpWindowName;
	}
	else
	{
		if (regcomp((regex_t *)(param.lpClassName = &regClassName), lpClassName, REG_EXTENDED | REG_NEWLINE | REG_NOSUB) != 0)
			return NULL;
		if (param.lpWindowName = lpWindowName)
			if (regcomp((regex_t *)(param.lpWindowName = &regWindowName), lpWindowName, REG_EXTENDED | REG_NEWLINE | REG_NOSUB) != 0)
				return NULL;
		if (param.lpModuleName = lpModuleName)
			if (regcomp((regex_t *)(param.lpModuleName = &regModuleName), lpModuleName, REG_EXTENDED | REG_ICASE | REG_NEWLINE | REG_NOSUB) != 0)
				return NULL;
		param.nClassNameLength  = 0;
		param.nWindowNameLength = 0;
	}
	param.lpModuleName = lpModuleName ? lpWideCharStr : NULL;
	param.hWnd         = NULL;
	param.dwProcessId  = 0;
	EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&param);
	if (bIsRegex)
	{
		if (lpModuleName)
			regfree(&regModuleName);
		if (lpWindowName)
			regfree(&regWindowName);
		regfree(&regClassName);
	}
	if (lpdwProcessId)
		*lpdwProcessId = param.dwProcessId;
	return param.hWnd;
}
