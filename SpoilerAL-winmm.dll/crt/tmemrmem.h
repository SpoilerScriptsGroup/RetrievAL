#define wmemcmp inline_wmemcmp
#include <wchar.h>
#undef wmemcmp
#include <tchar.h>

#ifdef _UNICODE
#define type wchar_t
#define _tmemrmem _wmemrmem
#define _tmemrchr _wmemrchr
#define _tmemcmp wmemcmp
extern wchar_t * __cdecl _wmemrchr(const wchar_t *buffer, wint_t c, size_t count);
extern int __cdecl wmemcmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#else
#define type void
#define _tmemrmem _memrmem
#define _tmemrchr _memrchr
#define _tmemcmp memcmp
#pragma function(memcmp)
extern void * __cdecl _memrchr(const void *buffer, wint_t c, size_t count);
#endif

type * __cdecl _tmemrmem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	if (needlelen && haystacklen >= needlelen)
	{
		TCHAR *p, c;

		p = (TCHAR *)haystack + haystacklen - needlelen + 1;
		c = *(TCHAR *)needle;
		do
			if (!(p = _tmemrchr(haystack, c, p - (TCHAR *)haystack)))
				break;
			else if (_tmemcmp(p, needle, needlelen) == 0)
				return p;
		while (p != haystack);
	}
	return NULL;
}
