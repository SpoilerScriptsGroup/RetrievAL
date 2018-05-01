#include <windows.h>

TCHAR * __cdecl _tcsstr(const TCHAR *string1, const TCHAR *string2)
{
	size_t    length1, length2, size;
	TCHAR     c;
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
		if (length1 == length2 && memcmp(string1, string2, length1 * sizeof(TCHAR)) == 0)
			return (TCHAR *)string1;
		else
			return NULL;
	c = *(string2++);
	if (!--length2)
		return _tcschr(string1, c);
	size = length2 * sizeof(TCHAR);
	string1 -= (offset = length2 - length1);
	do
		if (string1[offset] == c && memcmp(string1 + offset + 1, string2, size) == 0)
			return (TCHAR *)string1 + offset;
#ifdef _MBCS
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, string1[offset]) && !++offset)
			break;
#endif
	while (++offset);
	return NULL;
}
