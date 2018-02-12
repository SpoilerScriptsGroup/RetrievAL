#if defined(_ultot) && defined(_ui32tont)
#include <windows.h>
#include "digitstbl.h"

#define _ui32to10t _ui32tont(10)
#define _ui32to2t  _ui32tont(2)
#define _ui32to4t  _ui32tont(4)
#define _ui32to8t  _ui32tont(8)
#define _ui32to16t _ui32tont(16)
#define _ui32to32t _ui32tont(32)

size_t __fastcall _ui32to10t(unsigned __int32 value, TCHAR *buffer);
size_t __fastcall _ui32to2t(unsigned __int32 value, TCHAR *buffer);
size_t __fastcall _ui32to4t(unsigned __int32 value, TCHAR *buffer);
size_t __fastcall _ui32to8t(unsigned __int32 value, TCHAR *buffer);
size_t __fastcall _ui32to16t(unsigned __int32 value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui32to32t(unsigned __int32 value, TCHAR *buffer, BOOL upper);

TCHAR * __cdecl _ultot(unsigned long value, TCHAR *str, int radix)
{
	TCHAR *p1, *p2;

	/* check radix */
	switch (radix)
	{
	case 2:
		/* binary */
		_ui32to2t(value, str);
		return str;
	case 4:
		/* base 4 */
		_ui32to4t(value, str);
		return str;
	case 8:
		/* octal */
		_ui32to8t(value, str);
		return str;
	case 10:
		/* decimal */
		_ui32to10t(value, str);
		return str;
	case 16:
		/* hexadecimal */
		_ui32to16t(value, str, TRUE);
		return str;
	case 32:
		/* base 32 */
		_ui32to32t(value, str, TRUE);
		return str;
	case 3: case 5: case 6: case 7: case 9:
	case 11 + 'A' - 'A':
	case 11 + 'B' - 'A':
	case 11 + 'C' - 'A':
	case 11 + 'D' - 'A':
	case 11 + 'E' - 'A':
	case 11 + 'G' - 'A':
	case 11 + 'H' - 'A':
	case 11 + 'I' - 'A':
	case 11 + 'J' - 'A':
	case 11 + 'K' - 'A':
	case 11 + 'L' - 'A':
	case 11 + 'M' - 'A':
	case 11 + 'N' - 'A':
	case 11 + 'O' - 'A':
	case 11 + 'P' - 'A':
	case 11 + 'Q' - 'A':
	case 11 + 'R' - 'A':
	case 11 + 'S' - 'A':
	case 11 + 'T' - 'A':
	case 11 + 'U' - 'A':
	case 11 + 'W' - 'A':
	case 11 + 'X' - 'A':
	case 11 + 'Y' - 'A':
	case 11 + 'Z' - 'A':
		/* the other base */
		p1 = str;
		do
		{
			unsigned long remainder;

			remainder = value % (unsigned int)radix;
			value /= (unsigned int)radix;
			*(p1++) = (TCHAR)digitsHexLarge[remainder];
		} while (value);
		break;
	default:
		/* invalid base */
		*str = TEXT('\0');
		return str;
	}

	/* We now have the digit of the number in the buffer, but in reverse */
	/* order.  Thus we reverse them now.                                 */

	*(p1--) = TEXT('\0');   /* terminate string; p points to last digit */
	p2 = str;
	while (p1 > p2)         /* repeat until halfway */
	{
		TCHAR c1, c2;

		c1 = *p1;           /* swap *p1 and *p2 */
		c2 = *p2;
		*(p1--) = c2;
		*(p2++) = c1;       /* advance to next two digits */
	}
	return str;
}

#include "ui32tont\ui32tont.h"

#endif
