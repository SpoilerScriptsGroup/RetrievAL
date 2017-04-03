#include "intrinsic.h"

unsigned char * __cdecl _mbsstr_cp932(const unsigned char *str, const unsigned char *strSearch)
{
	size_t length1, length2;

	length1 = strlen((const char *)str);
	length2 = strlen((const char *)strSearch);
	if (length1 > length2)
	{
		for (const unsigned char *p = str, *end = str + length1 - length2; p <= end; p++)
		{
			if (memcmp(p, strSearch, length2) == 0)
				return (unsigned char *)p;
			if (__intrinsic_isleadbyte_cp932(*p))
				p++;
		}
	}
	else if (length1 == length2)
	{
		if (memcmp(str, strSearch, length1) == 0)
			return (unsigned char *)str;
	}
	return NULL;
}
