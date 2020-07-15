#include <stddef.h>

#define iswlower(c) ((unsigned)((c) - 'a') < 'z' - 'a' + 1)
#define iswupper(c) ((unsigned)((c) - 'A') < 'Z' - 'A' + 1)

int __cdecl _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	unsigned int ret, c;

	string1 += count;
	string2 += count;
	count ^= -1;
	ret = 0;
	if (++count)
		do
			while (ret = (unsigned int)string1[count] - (c = string2[count]))
				if (!c || !++count)
					goto DONE;
		while (ret != 'a' - 'A' ? ret == 'A' - 'a' && iswlower(c) : iswupper(c));
DONE:
	return ret;
}
