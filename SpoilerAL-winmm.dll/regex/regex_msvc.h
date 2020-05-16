#ifndef _REGEXEC_MSVC_H
#define _REGEXEC_MSVC_H

extern HANDLE hHeap;

#define CODEPAGE     CP_THREAD_ACP
#define HEAP_HANDLE  hHeap
#define HAVE_MEMCPY  1
#define HAVE_MEMMOVE 1
#define HAVE_MEMSET  1
#define HAVE_ABORT   0

#endif
