#undef _MBCS
#undef _UNICODE
#undef UNICODE
#include <tchar.h>
#ifndef _tcslcpy
#define _tcslcpy strlcpy
#endif
#include "tcslcpy.h"
