#undef _UNICODE
#undef UNICODE
#define _UI32TONT(n)         _ui32to##n##a
#define INTERNAL_UI32TONT(n) internal_ui32to##n##a
#include "ui32tont.h"
