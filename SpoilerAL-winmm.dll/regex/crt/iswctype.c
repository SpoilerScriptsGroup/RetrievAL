#include <stdlib.h>

extern const unsigned char _ctype[128];

int __cdecl iswctype(wint_t wc, wctype_t desc)
{
	return wc < _countof(_ctype) && (_ctype[wc] & desc);
}
