#include <windows.h>
#include <tchar.h>

#ifdef _UNICODE
#define _tcsristr _wcsristr
#define _tmemrimem _wmemrimem
wchar_t * __cdecl _wmemrimem(const wchar_t *haystack, size_t haystacklen, const wchar_t *needle, size_t needlelen);
#else
#define _tcsristr _strristr
#define _tmemrimem _memrimem
void * __cdecl _memrimem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
#pragma function(strlen)
#endif

TCHAR * __cdecl _tcsristr(const TCHAR *haystack, const TCHAR *needle)
{
	size_t needlelen;

	needlelen = _tcslen(needle);
	if (!needlelen)
		return (TCHAR *)haystack;
	return _tmemrimem(haystack, _tcslen(haystack), needle, needlelen);
}
