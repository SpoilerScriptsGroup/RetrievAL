#include <tchar.h>

#ifdef _UNICODE
#define _tcpcpy wcpcpy
#else
#define _tcpcpy stpcpy
#pragma function(strlen)
#endif

#pragma function(memcpy)

TCHAR * __cdecl _tcpcpy(TCHAR *dest, TCHAR *src, size_t count)
{
	size_t length = _tcslen(src);
	return (TCHAR *)memcpy(dest, src, (length + 1) * sizeof(TCHAR)) + length;
}
