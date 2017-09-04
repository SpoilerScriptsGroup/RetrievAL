#ifndef _SNAPWINDOW_H_
#define _SNAPWINDOW_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL __fastcall AttachSnapWindow(HWND hWnd);
void __fastcall DetachSnapWindow(HWND hWnd);
BOOL __fastcall EnableSnapWindow(HWND hWnd, BOOL bEnable);

#ifdef __cplusplus
}
#endif

#endif	// _SNAPWINDOW_H_
