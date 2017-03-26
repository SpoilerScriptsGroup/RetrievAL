#ifndef _INC_TOOLHELP32_FIX
#define _INC_TOOLHELP32_FIX

#if _MSC_VER > 1000
#pragma once
#endif

#include <tlhelp32.h>

#ifdef __cplusplus
extern "C" {
#endif

/***** Process walking *************************************************/

#ifdef UNICODE
#undef PROCESSENTRY32
typedef PROCESSENTRY32 PROCESSENTRY32A;
#define PROCESSENTRY32 PROCESSENTRY32W
#else
typedef PROCESSENTRY32 PROCESSENTRY32A;
#endif
typedef PROCESSENTRY32A * PPROCESSENTRY32A;
typedef PROCESSENTRY32A * LPPROCESSENTRY32A;

BOOL
WINAPI
Process32FirstA(
    HANDLE hSnapshot,
    LPPROCESSENTRY32A lppe
    );

BOOL
WINAPI
Process32NextA(
    HANDLE hSnapshot,
    LPPROCESSENTRY32A lppe
    );

/***** Module walking *************************************************/

#ifdef UNICODE
#undef MODULEENTRY32
typedef MODULEENTRY32 MODULEENTRY32A;
#define MODULEENTRY32 MODULEENTRY32W
#else
typedef MODULEENTRY32 MODULEENTRY32A;
#endif
typedef MODULEENTRY32A * PMODULEENTRY32A;
typedef MODULEENTRY32A * LPMODULEENTRY32A;

BOOL
WINAPI
Module32FirstA(
    HANDLE hSnapshot,
    LPMODULEENTRY32A lpme
    );

BOOL
WINAPI
Module32NextA(
    HANDLE hSnapshot,
    LPMODULEENTRY32A lpme
    );

#ifdef __cplusplus
}
#endif

#endif // _INC_TOOLHELP32_FIX
