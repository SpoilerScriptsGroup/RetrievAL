#include <windows.h>

#define WAIT_CURSOR_DELAY 200

static CRITICAL_SECTION          cs;
static HANDLE           volatile hTimerQueue;
static HANDLE           volatile hTimer;
static HCURSOR          volatile hOldCursor;
static size_t           volatile nNumberOfWait;

static void CALLBACK WaitCursorProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

EXTERN_C BOOL __cdecl InitializeWaitCursor()
{
	hTimer = NULL;
	hOldCursor = NULL;
	nNumberOfWait = 0;
#ifndef _WIN64
	if (hTimerQueue = CreateTimerQueue())
		InitializeCriticalSection(&cs);
	return (BOOL)hTimerQueue;
#else
	if (!(hTimerQueue = CreateTimerQueue()))
		return FALSE;
	InitializeCriticalSection(&cs);
	return TRUE;
#endif
}

EXTERN_C void __cdecl DeleteWaitCursor()
{
	if (hTimerQueue)
	{
		if (hTimer)
		{
			#pragma warning(suppress: 6031)
			DeleteTimerQueueTimer(hTimerQueue, hTimer, NULL);
			hTimer = NULL;
		}
		#pragma warning(suppress: 6031)
		DeleteTimerQueue(hTimerQueue);
		hTimerQueue = NULL;
		if (hOldCursor)
		{
			SetCursor(hOldCursor);
			hOldCursor = NULL;
		}
		nNumberOfWait = 0;
		DeleteCriticalSection(&cs);
	}
}

EXTERN_C void __cdecl BeginWaitCursor()
{
	EnterCriticalSection(&cs);
	if (!nNumberOfWait++)
		if (!CreateTimerQueueTimer((PHANDLE)&hTimer, hTimerQueue, WaitCursorProc, (LPVOID)GetCurrentThreadId(), WAIT_CURSOR_DELAY, 0, WT_EXECUTEONLYONCE))
			hTimer = NULL;
	LeaveCriticalSection(&cs);
}

EXTERN_C void __cdecl EndWaitCursor()
{
	EnterCriticalSection(&cs);
	if (!--nNumberOfWait)
	{
		if (hTimer)
		{
			#pragma warning(suppress: 6031)
			DeleteTimerQueueTimer(hTimerQueue, hTimer, NULL);
			hTimer = NULL;
		}
		if (hOldCursor)
		{
			SetCursor(hOldCursor);
			hOldCursor = NULL;
		}
	}
	LeaveCriticalSection(&cs);
}

static void CALLBACK WaitCursorProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	DWORD  idAttach;
	HANDLE hPrevTimer;

	EnterCriticalSection(&cs);
	if (AttachThreadInput(idAttach = GetCurrentThreadId(), (DWORD)lpParameter, TRUE))
	{
		HCURSOR hWaitCursor;

		if (hWaitCursor = LoadCursorA(NULL, (LPCSTR)IDC_WAIT))
			hOldCursor = SetCursor(hWaitCursor);
		AttachThreadInput(idAttach, (DWORD)lpParameter, FALSE);
	}
	hPrevTimer = hTimer;
	hTimer = NULL;
	LeaveCriticalSection(&cs);
	#pragma warning(suppress: 6031)
	DeleteTimerQueueTimer(hTimerQueue, hPrevTimer, NULL);
}
