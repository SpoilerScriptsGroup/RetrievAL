#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <errno.h>

#pragma warning(disable:4273)

errno_t __cdecl _tgetenv_s(size_t *requiredCount, TCHAR *buffer, size_t bufferCount, const TCHAR *varname)
{
	if (requiredCount && (buffer || !bufferCount) && varname)
		if ((*requiredCount = GetEnvironmentVariable(varname, buffer, bufferCount) < bufferCount)
			return 0;
		else
			return ERANGE;
	return EINVAL;
}

TCHAR * __cdecl _tgetenv(const TCHAR *varname)
{
	static TCHAR buffer[256];
	size_t count;
	errno_t status;

	if (status = _tgetenv_s(&count, buffer, _countof(buffer), varname))
		errno = status;
	else if (count)
		return buffer;
	return NULL;
}
