#ifndef _ERRORMESSAGE_H_
#define _ERRORMESSAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#if defined(__cplusplus) && !defined(__BORLANDC__)
extern "C" {
#endif

extern DWORD dwErrorMessageId;

#define SetErrorMessageId(_dwError_) (dwErrorMessageId = (_dwError_))
#define GetErrorMessageId()          ((const DWORD)dwErrorMessageId)
#define ClearErrorMessageId()        SetErrorMessageId(ERROR_SUCCESS)

void __cdecl ShowErrorMessage();

HANDLE __stdcall OpenProcessWithSaveError(
	IN           DWORD   dwDesiredAccess,
	IN           BOOL    bInheritHandle,
	IN           DWORD   dwProcessId);

BOOL __stdcall WriteProcessMemoryWithSaveError(
	IN           HANDLE  hProcess,
	IN           LPVOID  lpBaseAddress,
	IN           LPCVOID lpBuffer,
	IN           SIZE_T  nSize,
	OUT OPTIONAL SIZE_T  *lpNumberOfBytesWritten);

#ifdef __BORLANDC__
#define OpenProcess        OpenProcessWithSaveError
#define WriteProcessMemory WriteProcessMemoryWithSaveError
#endif

#if defined(__cplusplus) && !defined(__BORLANDC__)
}
#endif

#endif	// _ERRORMESSAGE_H_
