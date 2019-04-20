#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include "Internal.h"

DWORD SetLastWriteTime(HANDLE hFile, LPFILETIME lpLastWriteTime, LPWSTR lpParameter)
{
	DWORD dwError;

	dwError = ERROR_SUCCESS;
	do
	{
		LPWSTR     lpYear;
		LPWSTR     lpMonth;
		LPWSTR     lpDay;
		LPWSTR     lpHour;
		LPWSTR     lpMinute;
		LPWSTR     lpSecond;
		FILETIME   FileTime;
		FILETIME   LocalFileTime;
		SYSTEMTIME SystemTime;
		DWORD      Number;

		if (!(*lpParameter))
		{
			dwError = ERROR_INVALID_PARAMETER;
			break;
		}
		if (*lpParameter != L',')
		{
			lpYear = lpParameter;
			for (lpMonth = lpYear; *lpMonth; lpMonth++)
				if (*lpMonth == L'-')
					break;
			if (*lpMonth != L'-')
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			*(lpMonth++) = L'\0';
			for (lpDay = lpMonth; *lpDay; lpDay++)
				if (*lpDay == L'-')
					break;
			if (*lpDay != L'-')
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			*(lpDay++) = L'\0';
			for (lpHour = lpDay; *lpHour; lpHour++)
				if (*lpHour == L',')
					break;
			if (*lpHour == L',')
				*(lpHour++) = L'\0';
			else
			{
				lpHour = NULL;
				lpMinute = NULL;
				lpSecond = NULL;
			}
		}
		else
		{
			lpYear = NULL;
			lpMonth = NULL;
			lpDay = NULL;
			lpHour = lpParameter + 1;
		}
		if (lpHour)
		{
			for (lpMinute = lpHour; *lpMinute; lpMinute++)
				if (*lpMinute == L':')
					break;
			if (*lpMinute != L':')
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			*(lpMinute++) = L'\0';
			for (lpSecond = lpMinute; *lpSecond; lpSecond++)
				if (*lpSecond == L':')
					break;
			if (*lpSecond != L':')
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			*(lpSecond++) = L'\0';
		}
		if (!FileTimeToLocalFileTime(lpLastWriteTime, &LocalFileTime))
		{
			dwError = GetLastError();
			break;
		}
		if (!FileTimeToSystemTime(&LocalFileTime, &SystemTime))
		{
			dwError = GetLastError();
			break;
		}
		if (lpYear)
		{
			if (!GetDecimalNumber(lpYear, &Number))
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wYear = (WORD)Number;
		}
		if (lpMonth)
		{
			if (!GetDecimalNumber(lpMonth, &Number))
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wMonth = (WORD)Number;
		}
		if (lpDay)
		{
			if (!GetDecimalNumber(lpDay, &Number))
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wDay = (WORD)Number;
		}
		if (lpHour)
		{
			if (!GetDecimalNumber(lpHour, &Number))
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wHour = (WORD)Number;
		}
		if (lpMinute)
		{
			if (!GetDecimalNumber(lpMinute, &Number))
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wMinute = (WORD)Number;
		}
		if (lpSecond)
		{
			if (!GetDecimalNumber(lpSecond, &Number))
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wSecond = (WORD)Number;
		}
		if (lpSecond)
		{
			if (!GetDecimalNumber(lpSecond, &Number))
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wSecond = (WORD)Number;
		}
		SystemTime.wMilliseconds = 0;
		if (!SystemTimeToFileTime(&SystemTime, &LocalFileTime))
		{
			dwError = GetLastError();
			break;
		}
		if (!LocalFileTimeToFileTime(&LocalFileTime, &FileTime))
		{
			dwError = GetLastError();
			break;
		}
		if (!SetFileTime(hFile, NULL, NULL, &FileTime))
		{
			dwError = GetLastError();
			break;
		}
	} while (0);
	return dwError;
}

