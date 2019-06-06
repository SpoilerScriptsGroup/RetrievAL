#include <windows.h>

void __stdcall Wait(DWORD dwMilliseconds)
{
	DWORD   dwNow, dwStart, dwEnd;
	BOOLEAN bCarry;
	MSG     msg;

	if (!dwMilliseconds)
		return;
	dwEnd = (dwStart = dwNow = GetTickCount()) + dwMilliseconds;
	bCarry = dwEnd < dwNow;
	do
	{
		if (MsgWaitForMultipleObjects(0, NULL, FALSE, dwEnd - dwNow, QS_ALLEVENTS) == WAIT_TIMEOUT)
			break;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			if (msg.message != WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				PostMessage(msg.hwnd, WM_QUIT, msg.wParam, 0);
				return;
			}
	} while (dwEnd > (dwNow = GetTickCount()) || (dwEnd != dwNow && dwStart > dwNow && bCarry));
}
