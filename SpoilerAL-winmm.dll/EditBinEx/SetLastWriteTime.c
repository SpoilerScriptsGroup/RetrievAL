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

		if (*lpParameter == L'\0')
		{
			dwError = ERROR_INVALID_PARAMETER;
			break;
		}
		if (*lpParameter != L',')
		{
			lpYear = lpParameter;
			for (lpMonth = lpYear; *lpMonth != L'\0'; lpMonth++)
			{
				if (*lpMonth == L'-')
				{
					break;
				}
			}
			if (*lpMonth != L'-')
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			*(lpMonth++) = L'\0';
			for (lpDay = lpMonth; *lpDay != L'\0'; lpDay++)
			{
				if (*lpDay == L'-')
				{
					break;
				}
			}
			if (*lpDay != L'-')
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			*(lpDay++) = L'\0';
			for (lpHour = lpDay; *lpHour != L'\0'; lpHour++)
			{
				if (*lpHour == L',')
				{
					break;
				}
			}
			if (*lpHour == L',')
			{
				*(lpHour++) = L'\0';
			}
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
		if (lpHour != NULL)
		{
			for (lpMinute = lpHour; *lpMinute != L'\0'; lpMinute++)
			{
				if (*lpMinute == L':')
				{
					break;
				}
			}
			if (*lpMinute != L':')
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			*(lpMinute++) = L'\0';
			for (lpSecond = lpMinute; *lpSecond != L'\0'; lpSecond++)
			{
				if (*lpSecond == L':')
				{
					break;
				}
			}
			if (*lpSecond != L':')
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			*(lpSecond++) = L'\0';
		}
		if (FileTimeToLocalFileTime(lpLastWriteTime, &LocalFileTime) == FALSE)
		{
			dwError = GetLastError();
			break;
		}
		if (FileTimeToSystemTime(&LocalFileTime, &SystemTime) == FALSE)
		{
			dwError = GetLastError();
			break;
		}
		if (lpYear != NULL)
		{
			if (GetDecimalNumber(lpYear, &Number) == FALSE)
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wYear = (WORD)Number;
		}
		if (lpMonth != NULL)
		{
			if (GetDecimalNumber(lpMonth, &Number) == FALSE)
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wMonth = (WORD)Number;
		}
		if (lpDay != NULL)
		{
			if (GetDecimalNumber(lpDay, &Number) == FALSE)
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wDay = (WORD)Number;
		}
		if (lpHour != NULL)
		{
			if (GetDecimalNumber(lpHour, &Number) == FALSE)
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wHour = (WORD)Number;
		}
		if (lpMinute != NULL)
		{
			if (GetDecimalNumber(lpMinute, &Number) == FALSE)
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wMinute = (WORD)Number;
		}
		if (lpSecond != NULL)
		{
			if (GetDecimalNumber(lpSecond, &Number) == FALSE)
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wSecond = (WORD)Number;
		}
		if (lpSecond != NULL)
		{
			if (GetDecimalNumber(lpSecond, &Number) == FALSE)
			{
				dwError = ERROR_INVALID_PARAMETER;
				break;
			}
			SystemTime.wSecond = (WORD)Number;
		}
		SystemTime.wMilliseconds = 0;
		if (SystemTimeToFileTime(&SystemTime, &LocalFileTime) == FALSE)
		{
			dwError = GetLastError();
			break;
		}
		if (LocalFileTimeToFileTime(&LocalFileTime, &FileTime) == FALSE)
		{
			dwError = GetLastError();
			break;
		}
		if (SetFileTime(hFile, NULL, NULL, &FileTime) == FALSE)
		{
			dwError = GetLastError();
			break;
		}
	} while (0);
	return dwError;
}

