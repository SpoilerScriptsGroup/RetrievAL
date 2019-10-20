#include <windows.h>
#include <tchar.h>

#pragma warning(disable:4273)

#ifdef _MBCS
#define _tcsistr _mbsistr
#define _tcsichr _mbsichr
extern unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c);
#elif defined(_UNICODE)
#define _tcsistr _wcsistr
#define _tcsichr _wcsichr
#define _tmemicmp _wmemicmp
extern wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c);
extern int __cdecl _wmemicmp(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#else
#define _tcsistr _stristr
#define _tcsichr _strichr
#define _tmemicmp _memicmp
extern char * __cdecl _strichr(const char *string, int c);
extern int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count);
#endif

TCHAR * __cdecl _tcsistr(const TCHAR *string1, const TCHAR *string2)
{
	size_t length1, length2;

#ifndef _MBCS
	length2 = _tcslen(string2);
#else
	length2 = strlen((const char *)string2);
#endif
	if (!length2)
		return (TCHAR *)string1;
#ifndef _MBCS
	if (length2 == 1)
		return _tcsichr(string1, *string2);
#else
	if (length2 <= 2)
		if (length2 < 2)
			return _mbsichr(string1, string2[0]);
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, string2[0]))
			return _mbsichr(string1, ((unsigned int)string2[0] << 8) | string2[1]);
#endif
#ifndef _MBCS
	length1 = _tcslen(string1);
#else
	length1 = strlen((const char *)string1);
#endif
	if (length2 <= length1)
	{
		ptrdiff_t offset;

		string1 -= (offset = length2 - length1 - 1);
		do
#ifndef _MBCS
			if (_tmemicmp(string1 + offset, string2, length2) == 0)
				return (TCHAR *)string1 + offset;
#else
			if (_mbsnbicmp(string1 + offset, string2, length2) == 0)
				return (TCHAR *)string1 + offset;
			else if (IsDBCSLeadByteEx(CP_THREAD_ACP, string1[offset]) && !++offset)
				break;
#endif
		while (++offset);
	}
	return NULL;
}
