#ifndef _ISBADPTR_H_
#define _ISBADPTR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL __stdcall IsBadReadPtrCompatible(IN LPCVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadWritePtrCompatible(IN LPVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadStringPtrCompatibleA(IN LPCSTR lpsz, IN UINT_PTR ucchMax);
BOOL __stdcall IsBadStringPtrCompatibleW(IN LPCWSTR lpsz, IN UINT_PTR ucchMax);
BOOL __stdcall IsBadCodePtrCompatible(IN FARPROC lpfn);
BOOL __stdcall IsBadReadPtrEx(IN HANDLE hProcess, IN LPCVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadWritePtrEx(IN HANDLE hProcess, IN LPVOID lp, IN UINT_PTR ucb);
BOOL __stdcall IsBadStringPtrExA(IN HANDLE hProcess, IN LPCSTR lpsz, IN UINT_PTR ucchMax);
BOOL __stdcall IsBadStringPtrExW(IN HANDLE hProcess, IN LPCWSTR lpsz, IN UINT_PTR ucchMax);
BOOL __stdcall IsBadCodePtrEx(IN HANDLE hProcess, IN FARPROC lpfn);

#define IsBadReadPtr    IsBadReadPtrCompatible
#define IsBadWritePtr   IsBadWritePtrCompatible
#define IsBadStringPtrA IsBadStringPtrCompatibleA
#define IsBadStringPtrW IsBadStringPtrCompatibleW
#define IsBadCodePtr    IsBadCodePtrCompatible

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
