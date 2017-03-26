#include <windows.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include "ProcessContainsModule.h"

typedef struct {
	size_t  nClassNameLength;
	LPCSTR  lpClassName;
	size_t  nWindowNameLength;
	LPCSTR  lpWindowName;
	LPCWSTR lpModuleName;
	HWND    hWnd;
	DWORD   dwProcessId;
} ENUM_WINDOWS_PARAM;

static BOOL CALLBACK EnumWindowsProc(HWND hWnd, ENUM_WINDOWS_PARAM *param)
{
	char   lpString[1024];
	size_t nLength;
	DWORD  dwProcessId;

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
	if (!GetWindowThreadProcessId(hWnd, &dwProcessId))
		return TRUE;
	if (param->lpModuleName && !ProcessContainsModuleW(dwProcessId, param->lpModuleName))
		return TRUE;
	param->hWnd = hWnd;
	param->dwProcessId = dwProcessId;
	return FALSE;
}

HWND __stdcall FindWindowContainsModule(
	IN           LPCSTR  lpClassName,
	IN  OPTIONAL LPCSTR  lpWindowName,
	IN  OPTIONAL LPCSTR  lpModuleName,
	OUT OPTIONAL LPDWORD lpdwProcessId)
{
	ENUM_WINDOWS_PARAM param;
	wchar_t            lpWideCharStr[MAX_PATH];

	if (lpModuleName && !MultiByteToWideChar(CP_ACP, 0, lpModuleName, -1, lpWideCharStr, _countof(lpWideCharStr)))
		return NULL;
	param.nClassNameLength  = strlen(lpClassName);
	param.lpClassName       = lpClassName;
	param.nWindowNameLength = lpWindowName ? strlen(lpWindowName) : 0;
	param.lpWindowName      = lpWindowName;
	param.lpModuleName      = lpModuleName ? lpWideCharStr : NULL;
	param.hWnd              = NULL;
	param.dwProcessId       = 0;
	EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&param);
	if (lpdwProcessId)
		*lpdwProcessId = param.dwProcessId;
	return param.hWnd;
}
