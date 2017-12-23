#ifndef _TRANSCODEMULTIBYTE_H_
#define _TRANSCODEMULTIBYTE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

int __stdcall TranscodeMultiByte(
	UINT   uSrcCodePage,
	DWORD  dwSrcFlags,
	LPCSTR lpSrcStr,
	int    cchSrcLength,
	UINT   uDestCodePage,
	DWORD  dwDestFlags,
	LPSTR  lpDestStr,
	int    cchDestLength,
	LPCSTR lpDefaultChar,
	LPBOOL lpUsedDefaultChar);

#define MultiByteToUtf8(    \
    CodePage,               \
    dwFlags,                \
    lpMultiByteStr,         \
    cchMultiByte,           \
    lpUtf8Str,              \
    cchUtf8)                \
                            \
    TranscodeMultiByte(     \
        CodePage,           \
        dwFlags,            \
        lpMultiByteStr,     \
        cchMultiByte,       \
        CP_UTF8,            \
        0,                  \
        lpUtf8Str,          \
        cchUtf8,            \
        NULL,               \
        NULL)

#define Utf8ToMultiByte(    \
    CodePage,               \
    dwFlags,                \
    lpUtf8Str,              \
    cchUtf8,                \
    lpMultiByteStr,         \
    cchMultiByte,           \
    lpDefaultChar,          \
    lpUsedDefaultChar)      \
                            \
    TranscodeMultiByte(     \
        CP_UTF8,            \
        0,                  \
        lpUtf8Str,          \
        cchUtf8,            \
        CodePage,           \
        dwFlags,            \
        lpMultiByteStr,     \
        cchMultiByte,       \
        lpDefaultChar,      \
        lpUsedDefaultChar)

#ifdef __cplusplus
}
#endif

#endif	// _TRANSCODEMULTIBYTE_H_
