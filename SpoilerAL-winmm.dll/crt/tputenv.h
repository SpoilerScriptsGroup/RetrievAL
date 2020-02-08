#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <errno.h>

#pragma warning(disable:4273)

errno_t __cdecl _tputenv_s(TCHAR const *name, TCHAR const *value)
{
	if (name && value && *name)
		if (SetEnvironmentVariable(name, *value ? value : NULL))
			return 0;
		else
			return ENOMEM;
	return EINVAL;
}

int _tputenv(const TCHAR *envstring)
{
	const char *p;
	char name[256], *dest, c;

	p = envstring;
	dest = name;
	if ((c = *(p++)) == '=')
		goto FAILED1;
	do
		if (!c)
			goto FAILED1;
		else if (dest == name + _countof(name) - 1)
			goto FAILED2;
		else
			*(dest++) = c;
	while ((c = *(p++)) != '=');
	*dest = '\0';
	if (!SetEnvironmentVariable(name, *p ? p : NULL))
		goto FAILED2;
	return 0;

FAILED1:
	errno = EINVAL;
	return -1;

FAILED2:
	errno = ENOMEM;
	return -1;
}
