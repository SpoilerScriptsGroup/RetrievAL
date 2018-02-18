#undef _UNICODE
#undef UNICODE
#define _ultot               _ultoa
#define _ui64tot             _ui64toa
#define _UI32TONT(n)         _ui32to##n##a
#define INTERNAL_UI32TONT(n) internal_ui32to##n##a
#define _UI64TONT(n)         _ui64to##n##a
#define INTERNAL_UI64TONT(n) internal_ui64to##n##a
#include "ui64tot.h"
