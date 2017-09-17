#ifdef _UNICODE
#undef _UNICODE
#endif
#ifdef UNICODE
#undef UNICODE
#endif
#define _ultot       _ultoa
#define _ui32tont(n) _ui32to##n##a
#include "ultot.h"
