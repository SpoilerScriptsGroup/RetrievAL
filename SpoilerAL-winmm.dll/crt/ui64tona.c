#ifdef _UNICODE
#undef _UNICODE
#endif
#ifdef UNICODE
#undef UNICODE
#endif
#define _ui32tont(n) _ui32to##n##a
#define _ui64tont(n) _ui64to##n##a
#include "ui64tont.c"
