#include <windows.h>

char *__fastcall Unescape(char *first, char *last)
{
	while (first < last)
	{
		char   c;
		size_t count;

		if ((c = *first) != '\\')
		{
			first++;
			if (IsDBCSLeadByte(c))
				first++;
			continue;
		}
		switch (c = *(first + 1))
		{
		case 'a':
			*first = '\a';
			break;
		case 'b':
			*first = '\b';
			break;
		case 'f':
			*first = '\f';
			break;
		case 'n':
			*first = '\n';
			break;
		case 'r':
			*first = '\r';
			break;
		case 't':
			*first = '\t';
			break;
		case 'v':
			*first = '\v';
			break;
		default:
			*first = c;
		}
		count = (last--) - (first++);
		memcpy(first, first + 1, count);
	}
	return last;
}

