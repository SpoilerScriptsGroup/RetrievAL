#ifdef _UNICODE
#undef _UNICODE
#endif
#ifdef UNICODE
#undef UNICODE
#endif
#define _ui32tont(n) _ui32to##n##a
#include "ui32tont.h
"
