#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UNICODE
#define UNICODE 1
#endif
#define _ui64tot     _ui64tow
#define _ui64totn(n) _ui64tow ## n
#include "ui64tot.c"
