#ifndef _UI32TOT_H_
#define _UI32TOT_H_

#if _MSC_VER > 1000
#pragma once
#endif

typedef int BOOL;

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
size_t __fastcall internal_ui32toa(unsigned __int32 value, char *buffer, BOOL upper, unsigned int radix);
size_t __fastcall internal_ui32tow(unsigned __int32 value, wchar_t *buffer, BOOL upper, unsigned int radix);

#ifdef __cplusplus
}
#endif

#ifdef _UNICODE
#define _ui32to10t       _ui32to10w
#define _ui32to2t        _ui32to2w
#define _ui32to4t        _ui32to4w
#define _ui32to8t        _ui32to8w
#define _ui32to16t       _ui32to16w
#define _ui32to32t       _ui32to32w
#define internal_ui32tot internal_ui32tow
#else
#define _ui32to10t       _ui32to10a
#define _ui32to2t        _ui32to2a
#define _ui32to4t        _ui32to4a
#define _ui32to8t        _ui32to8a
#define _ui32to16t       _ui32to16a
#define _ui32to32t       _ui32to32a
#define internal_ui32tot internal_ui32toa
#endif

#endif	// _UI32TOT_H_
