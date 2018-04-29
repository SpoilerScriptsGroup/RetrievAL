#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif
#include <tchar.h>
#ifndef _tcslcat
#define _tcslcat wcslcat
#endif
#include "tcslcat.h"
