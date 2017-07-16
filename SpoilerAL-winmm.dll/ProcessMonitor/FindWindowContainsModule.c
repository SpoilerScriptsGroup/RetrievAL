#include <windows.h>

#include <stdlib.h>
#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0]))
#endif

#include <regex.h>
#pragma comment(lib, "regex.lib")

#include "ProcessContainsModule.h"

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
		iLength = GetClassNameA(hWnd, lpString, _countof(lpString));
		if (!iLength)
			return TRUE;
		if (regexec(param->lpClassName, lpString, 0, NULL, 0) != 0)
			return TRUE;
		if (param->lpWindowName)
		{
			iLength = GetWindowTextA(hWnd, lpString, _countof(lpString));
			if ((unsigned int)iLength < param->nWindowNameLength)
				return TRUE;
			if (regexec(param->lpWindowName, lpString, 0, NULL, 0) != 0)
				return TRUE;
		}
	}
	if (!GetWindowThreadProcessId(hWnd, &dwProcessId))
		return TRUE;
	if (param->lpModuleName)
	{
		BOOL(__stdcall *ContainsModule)(DWORD, LPCVOID) = param->bIsRegex ?
			(BOOL(__stdcall *)(DWORD, LPCVOID))ProcessContainsRegexModule :
			(BOOL(__stdcall *)(DWORD, LPCVOID))ProcessContainsModuleW;
		if (!ContainsModule(dwProcessId, param->lpModuleName))
			return TRUE;
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
	wchar_t            lpWideCharStr[MAX_PATH];

	param.hWnd = NULL;
	param.dwProcessId = 0;
	if (!(param.lpModuleName = lpModuleName) ||
		!MultiByteToWideChar(CP_ACP, 0, lpModuleName, -1, (LPWSTR)(param.lpModuleName = lpWideCharStr), _countof(lpWideCharStr)))
	{
		size_t nClassNameLength;
		size_t nWindowNameLength;
		size_t nModuleNameLength;
		LPSTR  lpBuffer;

		if (!(param.bIsRegex = bIsRegex))
		{
			param.nClassNameLength  = strlen(lpClassName);
			param.lpClassName       = lpClassName;
			param.nWindowNameLength = lpWindowName ? strlen(lpWindowName) : 0;
			param.lpWindowName      = lpWindowName;
			EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&param);
		}
		else if (lpBuffer = HeapAlloc(hHeap, 0,
			(nClassNameLength = strlen(lpClassName)) + 3 +
			(nWindowNameLength = lpWindowName ? strlen(lpWindowName) : 0) + (lpWindowName ? 3 : 0) +
			(nModuleNameLength = lpModuleName ? strlen(lpModuleName) : 0) + (lpModuleName ? 3 : 0)))
		{
			regex_t reClassName;
			LPSTR   dest;
			LPCSTR  src;

			src = lpClassName;
			lpClassName = dest = lpBuffer;
			*(dest++) = '^';
			memcpy(dest, src, nClassNameLength);
			dest += nClassNameLength;
			*(((LPWORD)dest)++) = (BYTE)'$';
			if (regcomp((regex_t *)(param.lpClassName = &reClassName), lpClassName, REG_EXTENDED | REG_NOSUB) == 0)
			{
				regex_t reWindowName;

				if (lpWindowName)
				{
					src = lpWindowName;
					lpWindowName = dest;
					*(dest++) = '^';
					memcpy(dest, src, nWindowNameLength);
					dest += nWindowNameLength;
					*(((LPWORD)dest)++) = (BYTE)'$';
				}
				if (!(param.lpWindowName = lpWindowName) ||
					regcomp((regex_t *)(param.lpWindowName = &reWindowName), lpWindowName, REG_EXTENDED | REG_NOSUB) == 0)
				{
					regex_t reModuleName;

					if (lpModuleName)
					{
						src = lpModuleName;
						lpModuleName = dest;
						*(dest++) = '^';
						memcpy(dest, src, nModuleNameLength);
						dest += nModuleNameLength;
						*(LPWORD)dest = (BYTE)'$';
					}
					if (!(param.lpModuleName = lpModuleName) ||
						regcomp((regex_t *)(param.lpModuleName = &reModuleName), lpModuleName, REG_EXTENDED | REG_ICASE | REG_NOSUB) == 0)
					{
						param.nClassNameLength  = 0;
						param.nWindowNameLength = 0;
						EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&param);
						if (lpModuleName)
							regfree(&reModuleName);
					}
					if (lpWindowName)
						regfree(&reWindowName);
				}
				regfree(&reClassName);
			}
			HeapFree(hHeap, 0, lpBuffer);
		}
	}
	if (lpdwProcessId)
		*lpdwProcessId = param.dwProcessId;
	return param.hWnd;
}
