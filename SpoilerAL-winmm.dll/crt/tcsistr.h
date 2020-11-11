#include <windows.h>
#include <tchar.h>

#ifdef _UNICODE
#define _tcsistr _wcsistr
#define _tmemimem _wmemimem
wchar_t * __cdecl _wmemimem(const wchar_t *haystack, size_t haystacklen, const wchar_t *needle, size_t needlelen);
#else
#define _tcsistr _stristr
#define _tmemimem _memimem
void * __cdecl _memimem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
#pragma function(strlen)
#endif

TCHAR * __cdecl _tcsistr(const TCHAR *haystack, const TCHAR *needle)
{
	size_t needlelen;

	needlelen = _tcslen(needle);
	if (!needlelen)
		return (TCHAR *)haystack;
	return _tmemimem(haystack, _tcslen(haystack), needle, needlelen);
}
