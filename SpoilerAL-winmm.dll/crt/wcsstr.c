#include <string.h>

wchar_t * __cdecl wcsstr(const wchar_t *string1, const wchar_t *string2)
{
	size_t        length1, length2;
	wchar_t       c;
	const wchar_t *end;

	length1 = wcslen(string1);
	length2 = wcslen(string2);
	if (!length2)
		return (wchar_t *)string1 + length1;
	if (length1 <= length2)
		if (length1 == length2 && memcmp(string1, string2, length1 * sizeof(wchar_t)) == 0)
			return (wchar_t *)string1;
		else
			return NULL;
	c = *(string2++);
	if (!--length2)
		return wcschr(string1, c);
	end = (const wchar_t *)((char *)(string1 + length1) - (length2 *= sizeof(wchar_t)));
	do
		if (*string1 == c && memcmp(string1 + 1, string2, length2) == 0)
			return (wchar_t *)string1;
	while (++string1 < end);
	return NULL;
}
