#define wmemchr inline_wmemchr
#define wmemcmp inline_wmemcmp
#include <wchar.h>
#undef wmemchr
#undef wmemcmp
#include <tchar.h>

#ifdef _UNICODE
#define type wchar_t
#define _tmemmem _wmemmem
#define _tmemchr wmemchr
#define _tmemcmp wmemcmp
extern wchar_t * __cdecl wmemchr(const wchar_t *buffer, wint_t c, size_t count);
extern int __cdecl wmemcmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#else
#define type void
#define _tmemmem _memmem
#define _tmemchr memchr
#define _tmemcmp memcmp
#pragma function(memcmp)
#endif

type * __cdecl _tmemmem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	if (needlelen && haystacklen >= needlelen)
	{
		TCHAR *p, *end, c;

		end = (p = (TCHAR *)haystack) + haystacklen - needlelen + 1;
		c = *(TCHAR *)needle;
		do
			if (!(p = _tmemchr(p, c, end - p)))
				break;
			else if (_tmemcmp(p, needle, needlelen) == 0)
				return p;
		while (end > ++p);
	}
	return NULL;
}
