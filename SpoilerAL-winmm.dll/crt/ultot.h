#if defined(_ultot) && defined(_UI32TONT) && defined(INTERNAL_UI32TONT)
#include <windows.h>
#include <stdint.h>
#include "digitstbl.h"

#define _ui32to10t        _UI32TONT(10)
#define _ui32to2t         _UI32TONT(2)
#define _ui32to4t         _UI32TONT(4)
#define _ui32to8t         _UI32TONT(8)
#define _ui32to16t        _UI32TONT(16)
#define _ui32to32t        _UI32TONT(32)
#define internal_ui32tont INTERNAL_UI32TONT(n)

size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to2t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to4t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to8t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to16t(uint32_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui32to32t(uint32_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall internal_ui32tont(uint32_t value, TCHAR *buffer, BOOL upper, unsigned int radix);

TCHAR * __cdecl _ultot(unsigned long value, TCHAR *str, int radix)
{
	/* check radix */
	switch (radix)
	{
	case 2:
		/* binary */
		_ui32to2t(value, str);
		break;
	case 4:
		/* base 4 */
		_ui32to4t(value, str);
		break;
	case 8:
		/* octal */
		_ui32to8t(value, str);
		break;
	case 10:
		/* decimal */
		_ui32to10t(value, str);
		break;
	case 16:
		/* hexadecimal */
		_ui32to16t(value, str, TRUE);
		break;
	case 32:
		/* base 32 */
		_ui32to32t(value, str, TRUE);
		break;
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
		internal_ui32tont(value, str, TRUE, radix);
		break;
	default:
		/* invalid base */
		*str = TEXT('\0');
		break;
	}
	return str;
}

#include "ui32tont\ui32tont.h"

#endif
