#undef _UNICODE
#undef UNICODE
#define _ultot               _ultoa
#define _UI32TONT(n)         _ui32to##n##a
#define INTERNAL_UI32TONT(n) internal_ui32to##n##a
#include "ultot.h"
