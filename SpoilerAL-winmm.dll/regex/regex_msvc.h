#ifndef _REGEXEC_MSVC_H
#define _REGEXEC_MSVC_H

extern HANDLE hHeap;

#define CODE_PAGE    CP_ACP
#define HEAP_HANDLE  hHeap
#define HAVE_MEMCPY  1
#define HAVE_MEMMOVE 1
#define HAVE_MEMSET  1
#define HAVE_STRICMP 1
#define HAVE_ABORT   0

#endif
