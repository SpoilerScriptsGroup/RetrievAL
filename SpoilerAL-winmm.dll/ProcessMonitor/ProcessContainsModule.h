#ifndef _PROCESSCONTAINSMODULE_H_
#define _PROCESSCONTAINSMODULE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL __stdcall ProcessContainsModuleW(
	IN DWORD   dwProcessId,
	IN LPCWSTR lpModuleName);

BOOL __stdcall ProcessContainsModuleA(
	IN DWORD   dwProcessId,
	IN LPCSTR  lpModuleName);

BOOL __stdcall ProcessContainsRegexModule(
	IN DWORD         dwProcessId,
	IN const regex_t *lpModuleName);

#ifdef __cplusplus
}
#endif

#endif	// _PROCESSCONTAINSMODULE_H_
