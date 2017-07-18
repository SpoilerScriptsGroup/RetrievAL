#include <windows.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include "ProcessContainsModule.h"

#ifdef __BORLANDC__
#pragma warn -8060
#endif

extern HANDLE hHeap;

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
	char         lpString[1024];
	unsigned int uLength;
	DWORD        dwProcessId;

	if (!param->bIsRegex)
	{
		if (param->lpClassName)
		{
			uLength = GetClassNameA(hWnd, lpString, _countof(lpString));
			if (uLength != param->nClassNameLength)
				return TRUE;
			if (memcmp(lpString, param->lpClassName, uLength) != 0)
				return TRUE;
		}
		if (param->lpWindowName)
		{
			uLength = GetWindowTextA(hWnd, lpString, _countof(lpString));
			if (uLength < param->nWindowNameLength)
				return TRUE;
			if (memcmp(lpString, param->lpWindowName, param->nWindowNameLength) != 0)
				return TRUE;
		}
	}
	else
	{
		return TRUE;
	}
	if (!GetWindowThreadProcessId(hWnd, &dwProcessId))
		return TRUE;
	if (param->lpModuleName)
		if (!ProcessContainsModule(dwProcessId, param->bIsRegex, param->lpModuleName))
			return TRUE;
	param->hWnd = hWnd;
	param->dwProcessId = dwProcessId;
	return FALSE;
}

HWND __stdcall FindWindowContainsModule(
	IN           BOOL    bIsRegex,
	IN  OPTIONAL LPCSTR  lpClassName,
	IN  OPTIONAL LPCSTR  lpWindowName,
	IN  OPTIONAL LPCSTR  lpModuleName,
	OUT OPTIONAL LPDWORD lpdwProcessId)
{
	ENUM_WINDOWS_PARAM param;

	param.hWnd = NULL;
	param.dwProcessId = 0;
	if (!(param.bIsRegex = bIsRegex))
	{
		wchar_t lpWideCharStr[MAX_PATH];

		if (!(param.lpModuleName = lpModuleName) ||
			!MultiByteToWideChar(CP_THREAD_ACP, 0, lpModuleName, -1, (LPWSTR)(param.lpModuleName = lpWideCharStr), _countof(lpWideCharStr)))
		{
			param.nClassNameLength  = lpClassName ? strlen(lpClassName) : 0;
			param.lpClassName       = lpClassName;
			param.nWindowNameLength = lpWindowName ? strlen(lpWindowName) : 0;
			param.lpWindowName      = lpWindowName;
			EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&param);
		}
	}
	if (lpdwProcessId)
		*lpdwProcessId = param.dwProcessId;
	return param.hWnd;
}
