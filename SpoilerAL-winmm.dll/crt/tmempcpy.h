#include <tchar.h>

#ifdef _UNICODE
#define type wchar_t
#define _tmempcpy wmempcpy
#else
#define type void
#define _tmempcpy mempcpy
#endif

#pragma function(memcpy)

type * __cdecl _tmempcpy(type *dest, type *src, size_t count)
{
	return (TCHAR *)memcpy(dest, src, count * sizeof(TCHAR)) + count;
}
