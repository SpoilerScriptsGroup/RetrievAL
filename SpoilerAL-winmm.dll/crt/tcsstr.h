#include <windows.h>
#include <tchar.h>

#ifdef _MBCS
#define _tcsstr _mbsstr
#elif defined(_UNICODE)
#define _tcsstr wcsstr
#else
#define _tcsstr strstr
#endif

#pragma warning(disable:4028 4142)

TCHAR * __cdecl _tcsstr(const TCHAR *string1, const TCHAR *string2)
{
	size_t    length1, length2, size;
	ptrdiff_t offset;

#ifndef _MBCS
	length1 = _tcslen(string1);
	length2 = _tcslen(string2);
#else
	length1 = strlen((const char *)string1);
	length2 = strlen((const char *)string2);
#endif
	if (!length2)
		return (TCHAR *)string1;
	if (length2 >= length1)
		return length2 == length1 && memcmp(string1, string2, length2 * sizeof(TCHAR)) == 0 ?
			(TCHAR *)string1 :
			NULL;
#ifndef _MBCS
	if (length2 == 1)
		return _tcschr(string1, *string2);
#else
	if (length2 <= 2)
		if (length2 < 2)
			return _mbschr(string1, string2[0]);
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, string2[0]))
			return _mbschr(string1, ((unsigned int)string2[0] << 8) | string2[1]);
#endif
	string1 -= (offset = length2 - length1);
	size = length2 * sizeof(TCHAR);
	do
		if (memcmp(string1 + offset, string2, size) == 0)
			return (TCHAR *)string1 + offset;
#ifdef _MBCS
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, string1[offset]) && !++offset)
			break;
#endif
	while (++offset);
	return NULL;
}
