#ifndef _PROCESSCONTAINSMODULE_H_
#define _PROCESSCONTAINSMODULE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL __stdcall ProcessInfoValidation(
	IN DWORD   dwProcessId,
	IN BOOL    bIsRegex,
	IN LPCVOID lpModuleName,
	IN LPCVOID lpCmdLineArg);

#ifdef __cplusplus
}
#endif

#endif	// _PROCESSCONTAINSMODULE_H_
