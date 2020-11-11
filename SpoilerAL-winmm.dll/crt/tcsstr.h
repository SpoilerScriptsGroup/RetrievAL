#include <windows.h>
#include <tchar.h>

#ifdef _UNICODE
#define _tcsstr wcsstr
#define _tmemmem _wmemmem
wchar_t * __cdecl _wmemmem(const wchar_t *haystack, size_t haystacklen, const wchar_t *needle, size_t needlelen);
#else
#define _tcsstr strstr
#define _tmemmem _memmem
void * __cdecl _memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
#pragma function(strlen)
#endif

TCHAR * __cdecl _tcsstr(const TCHAR *haystack, const TCHAR *needle)
{
	size_t needlelen;

	needlelen = _tcslen(needle);
	if (!needlelen)
		return (TCHAR *)haystack;
	return _tmemmem(haystack, _tcslen(haystack), needle, needlelen);
}
