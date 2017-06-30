#ifndef _PROCESSMONITOR_H_
#define _PROCESSMONITOR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void __cdecl InitializeProcessMonitor();
void __cdecl DeleteProcessMonitor();
void __cdecl StopProcessMonitor();

DWORD __stdcall FindProcessId(
	IN          BOOL   bIsRegex,
	IN          LPCSTR lpProcessName,
	IN OPTIONAL LPCSTR lpModuleName);

#ifdef __cplusplus
}
#endif

#endif	// _PROCESSMONITOR_H_
