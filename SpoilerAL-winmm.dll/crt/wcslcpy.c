#undef _MBCS
#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif
#include <tchar.h>
#ifndef _tcslcpy
#define _tcslcpy wcslcpy
#endif
#include "tcslcpy.h"
