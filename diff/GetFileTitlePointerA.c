#ifdef _UNICODE
#undef _UNICODE
#endif
#ifdef UNICODE
#undef UNICODE
#endif
#include <windows.h>

#define GetFileTitlePointerT GetFileTitlePointerA
#include "GetFileTitlePointerT.c"
