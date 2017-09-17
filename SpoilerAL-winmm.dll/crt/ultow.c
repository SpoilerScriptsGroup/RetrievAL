#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif
#define _ultot       _ultow
#define _ui32tont(n) _ui32to##n##w
#include "ultot.h"
