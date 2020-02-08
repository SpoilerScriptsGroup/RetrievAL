#include <stdlib.h>
#include <tchar.h>

#pragma warning(disable:4273)

long __cdecl _ttol(const TCHAR *string)
{
	return _tcstol(string, NULL, 10);
}

unsigned long __cdecl _ttoul(const TCHAR *string)
{
	return _tcstoul(string, NULL, 10);
}

__int64 __cdecl _ttoi64(const TCHAR *string)
{
	return _tcstoi64(string, NULL, 10);
}

unsigned __int64 __cdecl _ttoui64(const TCHAR *string)
{
	return _tcstoui64(string, NULL, 10);
}
