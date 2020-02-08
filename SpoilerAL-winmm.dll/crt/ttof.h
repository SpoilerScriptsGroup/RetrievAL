#include <stdlib.h>
#include <tchar.h>

#pragma warning(disable:4273)

double __cdecl _ttof(const TCHAR *string)
{
	return _tcstod(string, NULL);
}
