#include <windows.h>

#define WAIT_CURSOR_DELAY 200

static CRITICAL_SECTION          cs;
static HANDLE           volatile hThread;
static DWORD            volatile dwThreadId;
static HCURSOR          volatile hOldCursor;
static DWORD            volatile dwNumberOfWait;
static DWORD            volatile dwStartTime;

static DWORD WINAPI WaitCursorProc(LPVOID lpParameter)
{
	for (; ; )
	{
		if (!dwStartTime)
		{
			MSG msg;

		PUMP_MESSAGE:
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message != WM_QUIT)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					PostQuitMessage(msg.wParam);
					break;
				}
			}
			else
			{
				Sleep(1);
			}
			continue;
		}
		while (GetTickCount() - dwStartTime < WAIT_CURSOR_DELAY)
		{
			Sleep(1);
			if (!dwStartTime)
				goto PUMP_MESSAGE;
		}
		EnterCriticalSection(&cs);
		if (dwStartTime)
		{
			dwStartTime = 0;
			if (AttachThreadInput(dwThreadId, (DWORD)lpParameter, TRUE))
			{
				HCURSOR hWaitCursor;

				hWaitCursor = LoadCursorA(NULL, (LPCSTR)IDC_WAIT);
				if (hWaitCursor)
					hOldCursor = SetCursor(hWaitCursor);
				AttachThreadInput(dwThreadId, (DWORD)lpParameter, FALSE);
			}
		}
		LeaveCriticalSection(&cs);
	}
	hThread = NULL;
	dwThreadId = 0;
	return 0;
}

EXTERN_C void __cdecl InitializeWaitCursor()
{
	InitializeCriticalSection(&cs);
#if 0
	hThread = NULL;
	return;
#endif
	hThread = CreateThread(NULL, 0, WaitCursorProc, (LPVOID)GetCurrentThreadId(), 0, (LPDWORD)&dwThreadId);
	if (!hThread)
		dwThreadId = 0;
	hOldCursor = NULL;
	dwNumberOfWait = 0;
	dwStartTime = 0;
}

EXTERN_C void __cdecl DeleteWaitCursor()
{
	if (hThread)
	{
		TerminateThread(hThread, 0);
		hThread = NULL;
		dwThreadId = 0;
		if (hOldCursor)
		{
			SetCursor(hOldCursor);
			hOldCursor = NULL;
		}
		dwNumberOfWait = 0;
		dwStartTime = 0;
	}
	DeleteCriticalSection(&cs);
}

EXTERN_C void __cdecl BeginWaitCursor()
{
	EnterCriticalSection(&cs);
	if (hThread && !dwNumberOfWait++)
	{
		DWORD dwTickCount;

		dwTickCount = GetTickCount();
		if (!dwTickCount)
			dwTickCount--;
		dwStartTime = dwTickCount;
	}
	LeaveCriticalSection(&cs);
}

EXTERN_C void __cdecl EndWaitCursor()
{
	EnterCriticalSection(&cs);
	if (hThread && dwNumberOfWait && !--dwNumberOfWait)
	{
		dwStartTime = 0;
		if (hOldCursor)
		{
			SetCursor(hOldCursor);
			hOldCursor = NULL;
		}
	}
	LeaveCriticalSection(&cs);
}

