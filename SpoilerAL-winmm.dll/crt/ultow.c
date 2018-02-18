#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif
#define _ultot               _ultow
#define _UI32TONT(n)         _ui32to##n##w
#define INTERNAL_UI32TONT(n) internal_ui32to##n##w
#include "ultot.h"
