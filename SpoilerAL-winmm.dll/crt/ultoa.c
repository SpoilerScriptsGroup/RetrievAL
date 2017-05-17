#ifdef _UNICODE
#undef _UNICODE
#endif
#ifdef UNICODE
#undef UNICODE
#endif
#define _ultot     _ultoa
#define _ultotn(n) _ultoa ## n
#include "ultot.c"
