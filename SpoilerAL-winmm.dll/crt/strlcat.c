#undef _MBCS
#undef _UNICODE
#undef UNICODE
#include <tchar.h>
#ifndef _tcslcat
#define _tcslcat strlcat
#endif
#include "tcslcat.h"
