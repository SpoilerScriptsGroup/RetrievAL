#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif
#define _UI32TONT(n)         _ui32to##n##w
#define INTERNAL_UI32TONT(n) internal_ui32to##n##w
#define _UI64TONT(n)         _ui64to##n##w
#define INTERNAL_UI64TONT(n) internal_ui64to##n##w
#include "ui64tont.h"
