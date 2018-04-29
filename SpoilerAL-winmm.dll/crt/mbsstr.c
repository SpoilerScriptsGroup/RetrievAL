#include <windows.h>

unsigned char * __cdecl _mbsstr(const unsigned char *string1, const unsigned char *string2)
{
	size_t              length1, length2;
	unsigned char       c;
	const unsigned char *end;

	length1 = strlen((const char *)string1);
	length2 = strlen((const char *)string2);
	if (!length2)
		return (unsigned char *)string1 + length1;
	if (length1 <= length2)
		if (length1 == length2 && memcmp(string1, string2, length1) == 0)
			return (unsigned char *)string1;
		else
			return NULL;
	c = *(string2++);
	if (!--length2)
		return _mbschr(string1, c);
	end = string1 + length1 - length2;
	do
		if (*string1 == c && memcmp(string1 + 1, string2, length2) == 0)
			return (unsigned char *)string1;
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, *string1))
			string1++;
	while (++string1 < end);
	return NULL;
}
