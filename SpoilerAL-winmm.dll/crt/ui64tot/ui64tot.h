#ifndef _UI64TON_H_
#define _UI64TON_H_

#if _MSC_VER > 1000
#pragma once
#endif

typedef int BOOL;

#ifdef __cplusplus
extern "C" {
#endif

size_t __fastcall _ui64to10a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to10w(unsigned __int64 value, wchar_t *buffer);
size_t __fastcall _ui64to2a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to2w(unsigned __int64 value, wchar_t *buffer);
size_t __fastcall _ui64to4a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to4w(unsigned __int64 value, wchar_t *buffer);
size_t __fastcall _ui64to8a(unsigned __int64 value, char *buffer);
size_t __fastcall _ui64to8w(unsigned __int64 value, wchar_t *buffer);
size_t __fastcall _ui64to16a(unsigned __int64 value, char *buffer, BOOL upper);
size_t __fastcall _ui64to16w(unsigned __int64 value, wchar_t *buffer, BOOL upper);
size_t __fastcall _ui64to32a(unsigned __int64 value, char *buffer, BOOL upper);
size_t __fastcall _ui64to32w(unsigned __int64 value, wchar_t *buffer, BOOL upper);
size_t __fastcall internal_ui64toa(unsigned __int64 value, char *buffer, BOOL upper, unsigned int radix);
size_t __fastcall internal_ui64tow(unsigned __int64 value, wchar_t *buffer, BOOL upper, unsigned int radix);

#ifdef __cplusplus
}
#endif

#ifdef _UNICODE
#define _ui64to10t       _ui64to10w
#define _ui64to2t        _ui64to2w
#define _ui64to4t        _ui64to4w
#define _ui64to8t        _ui64to8w
#define _ui64to16t       _ui64to16w
#define _ui64to32t       _ui64to32w
#define internal_ui64tot internal_ui64tow
#else
#define _ui64to10t       _ui64to10a
#define _ui64to2t        _ui64to2a
#define _ui64to4t        _ui64to4a
#define _ui64to8t        _ui64to8a
#define _ui64to16t       _ui64to16a
#define _ui64to32t       _ui64to32a
#define internal_ui64tot internal_ui64toa
#endif

#endif	// _UI64TON_H_
