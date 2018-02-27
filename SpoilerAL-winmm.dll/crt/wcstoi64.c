#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif
#ifndef _DEBUG
#define MAKE_CRT 1
#ifdef _MSC_VER
#include <windows.h>
EXTERN_C errno_t _terrno;
#define ERRORNO _terrno
#endif
#endif
#include "tcstoi64.h"
