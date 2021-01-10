#ifndef _SNAPWINDOW_H_
#define _SNAPWINDOW_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

// referenced at "winmm.c"
// referenced at "FixMainForm\TMainForm_ctor.c"
// referenced at "FixMainForm\TFindNameForm_ctor.c"
// referenced at "FixMainForm\TGuideForm_ctor.c"
// referenced at "FixMainForm\TSearchForm_ctor.c"
#define SNAPWINDOW 1

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
