#include <string.h>

extern int __cdecl _utf8nbicmp(const unsigned char *string1, const unsigned char *string2, size_t count);

unsigned char * __cdecl _utf8istr(const unsigned char *string1, const unsigned char *string2)
{
	size_t        length1, length2;
	ptrdiff_t     offset;
	unsigned char c;

	length1 = strlen((const char *)string1);
	length2 = strlen((const char *)string2);
	if (!length2)
		return (unsigned char *)string1 + length1;
	if (length1 <= length2)
		if (length1 == length2 && _utf8nbicmp(string1, string2, length1) == 0)
			return (unsigned char *)string1;
		else
			return NULL;
	string1 -= (offset = length2 - length1);
	do
		if (_utf8nbicmp(string1 + offset, string2, length2) == 0)
			return (unsigned char *)string1 + offset;
	while (((char)(c = string1[offset]) >= 0 || ++offset && c <= 0xDF || ++offset && c <= 0xEF || ++offset) && ++offset);
	return NULL;
}
