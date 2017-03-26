#ifndef _UNICODE
#define _UNICODE 1
#endif

#include <windows.h>

#if _MSC_VER < 1400
#define TCHAR wchar_t
#define _tcslen wcslen
#endif

#define SHRINK_BUFFER 1
#define ParseArgumentT ParseArgumentW
#include "ParseArgumentT.c"
