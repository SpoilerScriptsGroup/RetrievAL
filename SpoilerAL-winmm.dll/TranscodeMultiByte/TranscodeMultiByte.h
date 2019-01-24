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
	int    cbSrcLength,
	UINT   uDestCodePage,
	DWORD  dwDestFlags,
	LPSTR  lpDestStr,
	int    cbDestLength,
	LPCSTR lpDefaultChar,
	LPBOOL lpUsedDefaultChar);

#define MultiByteToUtf8(    \
    CodePage,               \
    dwFlags,                \
    lpMultiByteStr,         \
    cbMultiByte,            \
    lpUtf8Str,              \
    cbUtf8)                 \
                            \
    TranscodeMultiByte(     \
        CodePage,           \
        dwFlags,            \
        lpMultiByteStr,     \
        cbMultiByte,        \
        CP_UTF8,            \
        0,                  \
        lpUtf8Str,          \
        cbUtf8,             \
        NULL,               \
        NULL)

#define Utf8ToMultiByte(    \
    CodePage,               \
    dwFlags,                \
    lpUtf8Str,              \
    cbUtf8,                 \
    lpMultiByteStr,         \
    cbMultiByte,            \
    lpDefaultChar,          \
    lpUsedDefaultChar)      \
                            \
    TranscodeMultiByte(     \
        CP_UTF8,            \
        0,                  \
        lpUtf8Str,          \
        cbUtf8,             \
        CodePage,           \
        dwFlags,            \
        lpMultiByteStr,     \
        cbMultiByte,        \
        lpDefaultChar,      \
        lpUsedDefaultChar)

#ifdef __cplusplus
}
#endif

#endif	// _TRANSCODEMULTIBYTE_H_
