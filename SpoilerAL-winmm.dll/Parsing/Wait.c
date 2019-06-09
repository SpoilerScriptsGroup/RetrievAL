#include <windows.h>
#include <intrin.h>

#pragma intrinsic(__emulu)

void __stdcall Wait(DWORD dwMilliseconds)
{
	ULONGLONG qwNow, qwEnd, qwHundredNano;
	MSG       msg;

	if (!dwMilliseconds)
		return;
	if (dwMilliseconds != INFINITE)
	{
		GetSystemTimeAsFileTime((LPFILETIME)&qwNow);
		qwEnd = qwNow + __emulu(dwMilliseconds, 10000);
	}
	for (; ; )
	{
		if (MsgWaitForMultipleObjects(0, NULL, FALSE, dwMilliseconds, QS_ALLEVENTS) == WAIT_TIMEOUT)
			break;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			if (msg.message != WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				PostQuitMessage(msg.wParam);
				return;
			}
		if (dwMilliseconds == INFINITE)
			continue;
		GetSystemTimeAsFileTime((LPFILETIME)&qwNow);
		if (qwEnd <= qwNow)
			break;
		qwHundredNano = qwEnd - qwNow;
#ifndef _WIN64
		if (qwHundredNano <= MAXDWORD / 10000)
			if (dwMilliseconds = (DWORD)qwHundredNano / 10000)
				continue;
			else
				break;
#endif
		if (!(dwMilliseconds = (DWORD)(qwHundredNano / 10000)))
			break;
	}
}
