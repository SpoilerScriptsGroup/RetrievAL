#include <stdlib.h>

extern const unsigned char _ctype[128];

int __cdecl _isctype(int c, int desc)
{
	return (unsigned)c < _countof(_ctype) && (_ctype[c] & desc);
}
