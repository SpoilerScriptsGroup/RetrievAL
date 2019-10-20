#define wmemchr inline_wmemchr
#define wmemcmp inline_wmemcmp
#include <wchar.h>
#undef wmemchr
#undef wmemcmp
#include <tchar.h>

#pragma warning(disable:4273)

#ifdef _UNICODE
#define type wchar_t
#define _tmemmem _wmemmem
#define _tmemchr wmemchr
#define _tmemcmp wmemcmp
extern wchar_t * __cdecl wmemchr(const wchar_t * buffer, wint_t c, size_t count);
extern int __cdecl wmemcmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#else
#define type void
#define _tmemmem _memmem
#define _tmemchr memchr
#define _tmemcmp memcmp
extern void * __cdecl memchr(const void *buffer, int c, size_t count);
#pragma function(memcmp)
#endif

type * __cdecl _tmemmem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	if (needlelen && needlelen <= haystacklen)
	{
		ptrdiff_t offset;

		if (needlelen == 1)
			return _tmemchr(haystack, *(TCHAR *)needle, haystacklen);
		(TCHAR *)haystack -= (offset = needlelen - haystacklen - 1);
		do
			if (_tmemcmp((TCHAR *)haystack + offset, needle, needlelen) == 0)
				return (TCHAR *)haystack + offset;
		while (++offset);
	}
	return NULL;
}
