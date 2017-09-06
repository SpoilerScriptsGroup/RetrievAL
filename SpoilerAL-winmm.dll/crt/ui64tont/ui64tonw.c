#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif
#define _ui32tont(n) _ui32to##n##w
#define _ui64tont(n) _ui64to##n##w
#include "ui64tont.h"
