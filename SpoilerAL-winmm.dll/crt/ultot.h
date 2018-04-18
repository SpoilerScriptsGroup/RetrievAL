#include <windows.h>
#include <stdint.h>
#include <tchar.h>

#ifdef _UNICODE
#define _UI32TONT(n)         _ui32to##n##w
#define INTERNAL_UI32TONT(n) internal_ui32to##n##w
#else
#define _UI32TONT(n)         _ui32to##n##a
#define INTERNAL_UI32TONT(n) internal_ui32to##n##a
#endif

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
	if (radix == 10)
		/* decimal */
		_ui32to10t(value, str);
	else if (radix == 16)
		/* hexadecimal */
		_ui32to16t(value, str, TRUE);
	else if (radix == 8)
		/* octal */
		_ui32to8t(value, str);
	else if (radix == 2)
		/* binary */
		_ui32to2t(value, str);
	else if (radix == 4)
		/* base 4 */
		_ui32to4t(value, str);
	else if (radix == 32)
		/* base 32 */
		_ui32to32t(value, str, TRUE);
	else if (radix >= 2 && radix <= 36)
		/* the other base */
		internal_ui32tont(value, str, TRUE, radix);
	else
		/* invalid base */
		*str = TEXT('\0');
	return str;
}

#include "ui32tont\ui32tont.h"
