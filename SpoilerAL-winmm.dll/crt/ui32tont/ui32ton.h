#ifndef _UI32TON_H_
#define _UI32TON_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t __fastcall _ui32to10a(unsigned __int32 value, char *buffer);
size_t __fastcall _ui32to10w(unsigned __int32 value, wchar_t *buffer);
size_t __fastcall _ui32to2a(unsigned __int32 value, char *buffer);
size_t __fastcall _ui32to2w(unsigned __int32 value, wchar_t *buffer);
size_t __fastcall _ui32to4a(unsigned __int32 value, char *buffer);
size_t __fastcall _ui32to4w(unsigned __int32 value, wchar_t *buffer);
size_t __fastcall _ui32to8a(unsigned __int32 value, char *buffer);
size_t __fastcall _ui32to8w(unsigned __int32 value, wchar_t *buffer);
size_t __fastcall _ui32to16a(unsigned __int32 value, char *buffer, BOOL upper);
size_t __fastcall _ui32to16w(unsigned __int32 value, wchar_t *buffer, BOOL upper);
size_t __fastcall _ui32to32a(unsigned __int32 value, char *buffer, BOOL upper);
size_t __fastcall _ui32to32w(unsigned __int32 value, wchar_t *buffer, BOOL upper);

#ifdef __cplusplus
}
#endif

#endif	// _UI32TON_H_
