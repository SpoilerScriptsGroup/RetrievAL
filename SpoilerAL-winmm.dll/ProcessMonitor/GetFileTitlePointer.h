#ifndef _GETFILETITLEPOINTER_H_
#define _GETFILETITLEPOINTER_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

char * __fastcall GetFileTitlePointerA(const char *lpFileNeme);
wchar_t * __fastcall GetFileTitlePointerW(const wchar_t *lpFileNeme);

#ifdef __cplusplus
}
#endif

#endif	// _GETFILETITLEPOINTER_H_
