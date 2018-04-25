#include <windows.h>

wchar_t * __cdecl wcsstr(const wchar_t *str, const wchar_t *strSearch)
{
	size_t length1, length2;

	length1 = wcslen(str);
	length2 = wcslen(strSearch);
	if (length1 > length2)
		for (const wchar_t *end = (wchar_t *)((size_t)(str + length1) - (length2 *= sizeof(wchar_t))); str <= end; str++)
			if (memcmp(str, strSearch, length2) == 0)
				return (wchar_t *)str;
	else if (length1 == length2)
		if (memcmp(str, strSearch, length1 * sizeof(wchar_t)) == 0)
			return (wchar_t *)str;
	return NULL;
}
