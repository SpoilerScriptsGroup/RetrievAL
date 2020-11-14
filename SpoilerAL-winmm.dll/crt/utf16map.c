#include <wchar.h>
#include <intrin.h>

#pragma intrinsic(_bittestandset, _bittest)
#pragma warning(disable:4273)

#define UTF16MAP map

#ifdef UTF16MAP
__declspec(align(16)) static unsigned long UTF16MAP[0x10000 / 32];
#endif

#include "wcsspn.h"
#ifndef _DEBUG
#include "wcscspn.h"
#endif
