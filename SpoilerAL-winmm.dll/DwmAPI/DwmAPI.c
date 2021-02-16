#include <windows.h>
#include <dwmapi.h>

extern DWORD __stdcall GetOSMajorVersion();

BOOL __stdcall GetWindowRectangle(IN HWND hWnd, OUT LPRECT lpRect)
{
	return GetOSMajorVersion() >= 6 ?
		DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, lpRect, sizeof(RECT)) == S_OK :
		GetWindowRect(hWnd, lpRect);
}

BOOL __stdcall GetWindowMargin(IN HWND hWnd, OUT LPRECT lpRect)
{
	RECT rc;

	if (GetOSMajorVersion() >= 6 &&
		DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rc, sizeof(RECT)) == S_OK &&
		GetWindowRect(hWnd, lpRect))
	{
		lpRect->left   -= rc.left  ;
		lpRect->top    -= rc.top   ;
		lpRect->right  -= rc.right ;
		lpRect->bottom -= rc.bottom;
		return TRUE;
	}
	return FALSE;
}
