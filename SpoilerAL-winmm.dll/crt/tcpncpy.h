#include <tchar.h>

#ifdef _UNICODE
#define type wchar_t
#define _tcpncpy wcpncpy
#else
#define type void
#define _tcpncpy stpncpy
#endif

#pragma function(memcpy)

TCHAR * __cdecl _tcpncpy(TCHAR *dest, const TCHAR *src, size_t count)
{
	size_t length;

	length = _tcsnlen(src, count);
	if (count)
		memcpy(dest, src, (length + (length < count)) * sizeof(TCHAR));
	return dest + length;
}
