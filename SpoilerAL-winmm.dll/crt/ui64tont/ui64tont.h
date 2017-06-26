#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t __fastcall _ui64to10a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to10w(unsigned __int64 value, wchar_t *buffer);
size_t __fastcall _ui64to16a(unsigned __int64 value, char *buffer, BOOL upper);
size_t __fastcall _ui64to16w(unsigned __int64 value, wchar_t *buffer, BOOL upper);
size_t __fastcall _ui64to8a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to8w(unsigned __int64 value, wchar_t *buffer);

#ifdef __cplusplus
}
#endif
