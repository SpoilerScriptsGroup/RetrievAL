#define wmemichr inline_wmemichr
#define wmemicmp inline_wmemicmp
#include <wchar.h>
#undef wmemichr
#undef wmemicmp
#include <tchar.h>

#ifdef _UNICODE
#define type wchar_t
#define _tmemimem _wmemimem
#define _tmemichr _wmemichr
#define _tmemicmp _wmemicmp
extern wchar_t * __cdecl _wmemichr(const wchar_t *buffer, wint_t c, size_t count);
extern int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#else
#define type void
#define _tmemimem _memimem
#define _tmemichr _memichr
#define _tmemicmp _memicmp
extern void * __cdecl _memichr(const void *buffer, int c, size_t count);
#endif

type * __cdecl _tmemimem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	if (needlelen && haystacklen >= needlelen)
	{
		TCHAR *p, *end, c;

		end = (p = (TCHAR *)haystack) + haystacklen - needlelen + 1;
		c = *(TCHAR *)needle;
		do
			if (!(p = _tmemichr(p, c, end - p)))
				break;
			else if (_tmemicmp(p, needle, needlelen) == 0)
				return p;
		while (end > ++p);
	}
	return NULL;
}
