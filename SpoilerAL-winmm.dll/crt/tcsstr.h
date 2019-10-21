#include <windows.h>
#include <tchar.h>

#ifdef _MBCS
#pragma function(strlen)
#elif defined(_UNICODE)
#define _tcsstr wcsstr
#define _tmemmem _wmemmem
wchar_t * __cdecl _wmemmem(const wchar_t *haystack, size_t haystacklen, const wchar_t *needle, size_t needlelen);
#else
#define _tcsstr strstr
#define _tmemmem _memmem
void * __cdecl _memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
#pragma function(strlen)
#endif

#ifndef _MBCS
TCHAR * __cdecl _tcsstr(const TCHAR *string1, const TCHAR *string2)
{
	size_t needlelen;

	needlelen = _tcslen(string2);
	if (!needlelen)
		return (TCHAR *)string1;
	return _tmemmem(string1, _tcslen(string1), string2, needlelen);
}
#else
unsigned char * __cdecl _mbsstr(const unsigned char *string1, const unsigned char *string2)
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
			if (!(p = _mbschr(p, c)))
				break;
			else if (memcmp(p, string2, length2) == 0)
				return p;
		while ((p += clen) < end);
	}
	return NULL;
}
#endif
