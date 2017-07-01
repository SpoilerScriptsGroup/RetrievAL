#ifndef _REGEXEC_MSVC_H
#define _REGEXEC_MSVC_H

extern HANDLE hHeap;
#define HEAP_HANDLE hHeap

#define getenv(varname) NULL

#ifdef MB_CUR_MAX
#undef MB_CUR_MAX
#endif
#define MB_CUR_MAX 2

#define nl_langinfo(item) ""

#define HAVE_MEMCPY           1
#define HAVE_MEMMOVE          1
#define HAVE_MEMSET           1
#define HAVE_STRICMP          1
#define HAVE_STRCHR           1
#define HAVE_GETENV           1
#define HAVE_MB_CUR_MAX       1
#define HAVE_LANGINFO_CODESET 1

#endif
