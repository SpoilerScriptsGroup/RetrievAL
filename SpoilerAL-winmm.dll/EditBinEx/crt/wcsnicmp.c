#include <stddef.h>

int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	if (count)
	{
		short   result;
		wchar_t c1, c2;

		do
		{
			if ((c1 = *string1) >= L'A' && c1 <= L'Z')
				c1 += L'a' - L'A';
			if ((c2 = *string2) >= L'A' && c2 <= L'Z')
				c2 += L'a' - L'A';
			result = c1 - c2;
			if (result)
				break;
			string1++;
			string2++;
		} while (c1 && (--count));
		return (int)result;
	}
	return 0;
}
