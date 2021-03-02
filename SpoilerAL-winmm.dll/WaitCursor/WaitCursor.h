#ifndef _WAITCURSOR_H_
#define _WAITCURSOR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL __cdecl InitializeWaitCursor();
void __cdecl DeleteWaitCursor();
void __cdecl BeginWaitCursor();
void __cdecl EndWaitCursor();

#ifdef __cplusplus
}
#endif

#endif	// _WAITCURSOR_H_

