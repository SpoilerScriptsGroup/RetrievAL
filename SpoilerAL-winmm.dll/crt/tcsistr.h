#include <windows.h>
#include <tchar.h>
#ifndef _tcsistr
#ifdef _MBCS
#define _tcsistr _mbsistr
#elif defined(_UNICODE)
#define _tcsistr _wcsistr
#else
#define _tcsistr _stristr
#endif
#endif

#ifdef _UNICODE
#define ISNOTALPHA(c) ((short)(c) < 'A' || (unsigned short)(c) > 'Z' && ((unsigned short)(c) < 'a' || (unsigned short)(c) > 'z'))
#else
#define ISNOTALPHA(c) ((char)(c) < 'A' || (unsigned char)(c) > 'Z' && ((unsigned char)(c) < 'a' || (unsigned char)(c) > 'z'))
#endif

TCHAR * __cdecl _tcsistr(const TCHAR *string1, const TCHAR *string2)
{
	size_t    length1, length2;
	ptrdiff_t offset;

#ifdef _UNICODE
	length1 = wcslen(string1);
	length2 = wcslen(string2);
#else
	length1 = strlen((const char *)string1);
	length2 = strlen((const char *)string2);
#endif
	if (!length2)
		return (TCHAR *)string1 + length1;
	if (length1 <= length2)
		if (length1 == length2 && _tcsnicmp(string1, string2, length1) == 0)
			return (TCHAR *)string1;
		else
			return NULL;
	if (length2 == 1)
	{
		TCHAR c;

		c = *string2;
		if (ISNOTALPHA(c))
#ifdef _MBCS
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c))
#endif
				return _tcschr(string1, c);
	}
	string1 -= (offset = length2 - length1);
	do
		if (_tcsnicmp(string1 + offset, string2, length2) == 0)
			return (TCHAR *)string1 + offset;
#ifdef _MBCS
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, string1[offset]) && !++offset)
			break;
#endif
	while (++offset);
	return NULL;
}
