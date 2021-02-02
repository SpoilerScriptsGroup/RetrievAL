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

#ifdef _UNICODE
#undef PROCESSENTRY32
typedef PROCESSENTRY32 PROCESSENTRY32A;
#define PROCESSENTRY32 PROCESSENTRY32W
#else
typedef PROCESSENTRY32 PROCESSENTRY32A;
#endif
typedef PROCESSENTRY32A * PPROCESSENTRY32A;
typedef PROCESSENTRY32A * LPPROCESSENTRY32A;

#ifdef _UNICODE
#undef Process32First
#undef Process32Next
static __forceinline BOOL Process32FirstA(HANDLE hSnapshot, LPPROCESSENTRY32A lppe)
{
	return Process32First(hSnapshot, lppe);
}
static __forceinline BOOL Process32NextA(HANDLE hSnapshot, LPPROCESSENTRY32A lppe)
{
	return Process32Next(hSnapshot, lppe);
}
#define Process32First Process32FirstW
#define Process32Next Process32NextW
#else
#define Process32FirstA Process32First
#define Process32NextA Process32Next
#endif

/***** Module walking *************************************************/

#ifdef _UNICODE
#undef MODULEENTRY32
typedef MODULEENTRY32 MODULEENTRY32A;
#define MODULEENTRY32 MODULEENTRY32W
#else
typedef MODULEENTRY32 MODULEENTRY32A;
#endif
typedef MODULEENTRY32A * PMODULEENTRY32A;
typedef MODULEENTRY32A * LPMODULEENTRY32A;

#ifdef _UNICODE
#undef Module32First
#undef Module32Next
static __forceinline BOOL Module32FirstA(HANDLE hSnapshot, LPMODULEENTRY32A lpme)
{
	return Module32First(hSnapshot, lpme);
}
static __forceinline BOOL Module32NextA(HANDLE hSnapshot, LPMODULEENTRY32A lpme)
{
	return Module32Next(hSnapshot, lpme);
}
#define Module32First Module32FirstW
#define Module32Next Module32NextW
#else
#define Module32FirstA Module32First
#define Module32NextA Module32Next
#endif

#ifdef __cplusplus
}
#endif

#endif // _INC_TOOLHELP32_FIX
