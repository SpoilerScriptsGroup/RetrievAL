#include <stddef.h>

int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	if (count != 0)
	{
		short   result;
		wchar_t c1, c2;

		do
		{
			c1 = *string1;
			if (c1 >= L'A' && c1 <= L'Z')
			{
				c1 += L'a' - L'A';
			}
			c2 = *string2;
			if (c2 >= L'A' && c2 <= L'Z')
			{
				c2 += L'a' - L'A';
			}
			result = c1 - c2;
			if (result != 0)
			{
				break;
			}
			string1++;
			string2++;
		}
		while (c1 != L'\0' && (--count) != 0);
		return (int)result;
	}
	return 0;
}
