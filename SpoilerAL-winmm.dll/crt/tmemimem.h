#define wmemichr inline_wmemichr
#define wmemicmp inline_wmemicmp
#include <wchar.h>
#undef wmemichr
#undef wmemicmp
#include <tchar.h>

#pragma warning(disable:4273)

#ifdef _UNICODE
#define type wchar_t
#define _tmemimem _wmemimem
#define _tmemichr _wmemichr
#define _tmemicmp _wmemicmp
extern wchar_t * __cdecl _wmemichr(const wchar_t * buffer, wint_t c, size_t count);
extern int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#else
#define type void
#define _tmemimem _memimem
#define _tmemichr _memichr
#define _tmemicmp _memicmp
extern void * __cdecl _memichr(const void *buffer, int c, size_t count);
extern int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count);
#endif

type * __cdecl _tmemimem(const type *haystack, size_t haystacklen, const type *needle, size_t needlelen)
{
	if (needlelen && needlelen <= haystacklen)
	{
		ptrdiff_t offset;

		if (needlelen == 1)
			return _tmemichr(haystack, *(TCHAR *)needle, haystacklen);
		(TCHAR *)haystack -= (offset = needlelen - haystacklen - 1);
		do
			if (_tmemicmp((TCHAR *)haystack + offset, needle, needlelen) == 0)
				return (TCHAR *)haystack + offset;
		while (++offset);
	}
	return NULL;
}
