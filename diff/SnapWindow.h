#ifndef _SNAPPING_H_
#define _SNAPPING_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL __fastcall AttachSnapWindow(HWND hWnd);
void __fastcall DetachSnapWindow(HWND hWnd);
BOOL __fastcall EnableSnap(HWND hWnd, BOOL bEnable);

#ifdef __cplusplus
}
#endif

#endif	// _SNAPPING_H_
