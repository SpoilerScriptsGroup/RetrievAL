#include <windows.h>
#include <tchar.h>

#ifdef _MBCS
extern unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c);
#pragma function(strlen)
#elif defined(_UNICODE)
#define _tcsistr _wcsistr
#define _tmemimem _wmemimem
wchar_t * __cdecl _wmemimem(const wchar_t *haystack, size_t haystacklen, const wchar_t *needle, size_t needlelen);
#else
#define _tcsistr _stristr
#define _tmemimem _memimem
void * __cdecl _memimem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
#pragma function(strlen)
#endif

#ifndef _MBCS
TCHAR * __cdecl _tcsistr(const TCHAR *string1, const TCHAR *string2)
{
	size_t needlelen;

	needlelen = _tcslen(string2);
	if (!needlelen)
		return (TCHAR *)string1;
	return _tmemimem(string1, _tcslen(string1), string2, needlelen);
}
#else
unsigned char * __cdecl _mbsistr(const unsigned char *string1, const unsigned char *string2)
{
	size_t length1, length2;

	length2 = strlen((const char *)string2);
	if (!length2)
		return (unsigned char *)string1;
	length1 = strlen((const char *)string1);
	if (length1 >= length2)
	{
		unsigned char *p, *end;
		size_t        clen;
		unsigned int  c;

		end = (p = (unsigned char *)string1) + length1 - length2 + (clen = 1);
		if (IsDBCSLeadByteEx(CP_THREAD_ACP, c = string2[0]))
		{
			c = (c << 8) | string2[1];
			clen++;
			end++;
		}
		do
			if (!(p = _mbsichr(p, c)))
				break;
			else if (_mbsnbicmp(p, string2, length2) == 0)
				return p;
		while ((p += clen) < end);
	}
	return NULL;
}
#endif
