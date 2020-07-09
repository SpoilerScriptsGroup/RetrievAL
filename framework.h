// header.h : 標準のシステム インクルード ファイルのインクルード ファイル、
// またはプロジェクト専用のインクルード ファイル
//

#pragma once
//#define _NO_CRT_STDIO_INLINE
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>
// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <intrin.h>
#include <winternl.h>
#include <Shlwapi.h>

typedef struct excDLLinfoRec {
	unsigned version;	
	void*    varAddr;
} *excDLLinfoPtr;

EXTERN_C void __cdecl __GetExceptDLLinfo(excDLLinfoPtr infoPtr);

EXTERN_C __declspec(dllimport) int ___CPPdebugHook;// require address relocation

EXTERN_C BOOL WINAPI _DllMainCRTStartup(HINSTANCE const instance, DWORD const reason, LPVOID const reserved);
