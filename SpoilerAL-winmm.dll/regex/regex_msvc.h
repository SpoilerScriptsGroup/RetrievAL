#ifndef _REGEXEC_MSVC_H
#define _REGEXEC_MSVC_H

extern HANDLE hHeap;

#define VARIABLE_LOCALE 0
#define HEAP_HANDLE     hHeap
#define HAVE_MEMCPY     1
#define HAVE_MEMMOVE    1
#define HAVE_MEMSET     1
#define HAVE_ABORT      0

#endif
