#include <tchar.h>

#ifdef _UNICODE
#define type wchar_t
#define _tmemrimem _wmemrimem
#define _tmemrichr _wmemrichr
#define _tmemicmp _wmemicmp
extern wchar_t * __cdecl _wmemrichr(const wchar_t *buffer, wint_t c, size_t count);
extern int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#else
#define type void
#define _tmemrimem _memrimem
#define _tmemrichr _memrichr
#define _tmemicmp _memicmp
extern void * __cdecl _memrichr(const void *buffer, wint_t c, size_t count);
#endif

type * __cdecl _tmemrimem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	if (needlelen && haystacklen >= needlelen)
	{
		TCHAR *p, c;

		p = (TCHAR *)haystack + haystacklen - needlelen + 1;
		c = *(TCHAR *)needle;
		do
			if (!(p = _tmemrichr(haystack, c, p - (TCHAR *)haystack)))
				break;
			else if (_tmemicmp(p, needle, needlelen) == 0)
				return p;
		while (p != haystack);
	}
	return NULL;
}
