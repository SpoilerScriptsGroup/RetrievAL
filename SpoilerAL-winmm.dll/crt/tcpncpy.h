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
	if (count)
	{
		size_t length;

		memcpy(dest, src, (length = _tcsnlen(src, count)) * sizeof(TCHAR));
		memset(dest += length, 0, (count - length) * sizeof(TCHAR));
	}
	return dest;
}
