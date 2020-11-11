#include <windows.h>
#include <tchar.h>

#ifdef _UNICODE
#define _tcsrstr _wcsrstr
#define _tmemrmem _wmemrmem
wchar_t * __cdecl _wmemrmem(const wchar_t *haystack, size_t haystacklen, const wchar_t *needle, size_t needlelen);
#else
#define _tcsrstr _strrstr
#define _tmemrmem _memrmem
void * __cdecl _memrmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
#pragma function(strlen)
#endif

TCHAR * __cdecl _tcsrstr(const TCHAR *haystack, const TCHAR *needle)
{
	size_t needlelen;

	needlelen = _tcslen(needle);
	if (!needlelen)
		return (TCHAR *)haystack;
	return _tmemrmem(haystack, _tcslen(haystack), needle, needlelen);
}
