#if _MSC_VER > 1200
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#pragma intrinsic(wcslen)
#else
#ifndef wcslen
#include "_wcslen.h"
#define wcslen _wcslen
#endif
#endif
