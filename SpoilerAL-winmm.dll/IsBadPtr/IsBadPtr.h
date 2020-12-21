#ifndef _ISBADPTR_H_
#define _ISBADPTR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IsBadReadPtr    IsBadReadPtrCompatible
#define IsBadWritePtr   IsBadWritePtrCompatible
#define IsBadStringPtrA IsBadStringPtrCompatibleA
#define IsBadStringPtrW IsBadStringPtrCompatibleW
#define IsBadCodePtr    IsBadCodePtrCompatible

BOOL __stdcall IsBadReadPtr(IN LPCVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadWritePtr(IN LPVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadStringPtrA(IN LPCSTR lpsz, IN UINT_PTR ucchMax);
BOOL __stdcall IsBadStringPtrW(IN LPCWSTR lpsz, IN UINT_PTR ucchMax);
BOOL __stdcall IsBadCodePtr(IN FARPROC lpfn);
BOOL __stdcall IsBadReadPtrEx(IN HANDLE hProcess, IN LPCVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadWritePtrEx(IN HANDLE hProcess, IN LPVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadStringPtrExA(IN HANDLE hProcess, IN LPCSTR lpsz, IN UINT_PTR ucchMax);
BOOL __stdcall IsBadStringPtrExW(IN HANDLE hProcess, IN LPCWSTR lpsz, IN UINT_PTR ucchMax);
BOOL __stdcall IsBadCodePtrEx(IN HANDLE hProcess, IN FARPROC lpfn);

#ifdef __cplusplus
}
#endif

#ifdef _UNICODE
#define IsBadStringPtr   IsBadStringPtrW
#define IsBadStringPtrEx IsBadStringPtrExW
#else
#define IsBadStringPtr IsBadStringPtrA
#define IsBadStringPtrEx IsBadStringPtrExA
#endif

#endif	// _ISBADPTR_H_
