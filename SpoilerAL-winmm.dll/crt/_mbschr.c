#include <windows.h>
#include <mbstring.h>
#include <limits.h>

unsigned char *_mbschr(const unsigned char *string, unsigned int c)
{
	if (!(c & ~(unsigned int)UCHAR_MAX))
	{
		do
		{
			if (*string == c)
				return (unsigned char *)string;
			if (IsDBCSLeadByte(*string))
				string++;
		} while (*(string++));
	}
	else if (!(c & ~(unsigned int)USHRT_MAX) && *string)
	{
		do
		{
			if (!IsDBCSLeadByte(*string))
				continue;
			if (*(unsigned short *)string == (unsigned short)c)
				return (unsigned char *)string;
			if (!*(++string))
				break;
		} while (*(++string));
	}
	return NULL;
}
