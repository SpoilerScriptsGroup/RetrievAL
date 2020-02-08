#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <errno.h>

#pragma warning(disable:4273)

errno_t _tgetenv_s(size_t *requiredCount, TCHAR *buffer, size_t bufferCount, const TCHAR *varname)
{
	if (requiredCount && (buffer || bufferCount) && varname)
	{
		*requiredCount = GetEnvironmentVariable(varname, buffer, bufferCount);
		return 0;
	}
	return EINVAL;
}

TCHAR *_tgetenv(const TCHAR *varname)
{
	static TCHAR buffer[256];
	size_t count;
	errno_t error;

	if (error = _tgetenv_s(&count, buffer, _countof(buffer), varname))
		errno = error;
	else if (count)
		return buffer;
	return NULL;
}
