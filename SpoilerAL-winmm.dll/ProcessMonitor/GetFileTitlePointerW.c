#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif

#include <windows.h>

#define GetFileTitlePointerT GetFileTitlePointerW
#include "GetFileTitlePointerT.h"
