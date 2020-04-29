#include <windows.h>
#include <tchar.h>

#ifdef _MBCS
#include <intrin.h>
#ifndef _WIN64
#pragma intrinsic(_subborrow_u32)
#define _sub_uintptr(a, b, out) _subborrow_u32(0, a, b, out)
#else
#pragma intrinsic(_subborrow_u64)
#define _sub_uintptr(a, b, out) _subborrow_u64(0, a, b, out)
#endif
extern unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c);
#pragma function(strlen)
#elif defined(_UNICODE)
#define _tcsristr _wcsristr
#define _tmemrimem _wmemrimem
wchar_t * __cdecl _wmemrimem(const wchar_t *haystack, size_t haystacklen, const wchar_t *needle, size_t needlelen);
#else
#define _tcsristr _strristr
#define _tmemrimem _memrimem
void * __cdecl _memrimem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
#pragma function(strlen)
#endif

#ifndef _MBCS
TCHAR * __cdecl _tcsristr(const TCHAR *string1, const TCHAR *string2)
{
	size_t needlelen;

	needlelen = _tcslen(string2);
	if (!needlelen)
		return (TCHAR *)string1;
	return _tmemrimem(string1, _tcslen(string1), string2, needlelen);
}
#else
unsigned char * __cdecl _mbsristr(const unsigned char *string1, const unsigned char *string2)
{
	size_t        length1, length2;
	unsigned char *result;

	length2 = strlen((const char *)string2);
	if (!length2)
		return (unsigned char *)string1;
	length1 = strlen((const char *)string1);
	result = NULL;
	if (!_sub_uintptr(length1, length2, &length1))
	{
		unsigned char *p, *end;
		size_t        clen;
		unsigned int  c;

		end = (p = (unsigned char *)string1) + length1 + (clen = 1);
		if (IsDBCSLeadByteEx(CP_THREAD_ACP, c = string2[0]))
		{
			c = (c << 8) | string2[1];
			clen++;
			end++;
		}
		do
			if (!(p = _mbsichr(p, c)) || p >= end)
				break;
			else if (_mbsnbicmp(p, string2, length2) == 0)
				result = p;
		while ((p += clen) < end);
	}
	return result;
}
#endif
