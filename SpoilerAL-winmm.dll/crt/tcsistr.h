#include <windows.h>
#include <tchar.h>

#ifdef _MBCS
#define _tcsistr _mbsistr
#define _tcsichr _mbsichr
extern unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c);
#elif defined(_UNICODE)
#define _tcsistr _wcsistr
#define _tcsichr _wcsichr
extern wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c);
#else
#define _tcsistr _stristr
#define _tcsichr _strichr
extern char * __cdecl _strichr(const char *string, int c);
#endif

TCHAR * __cdecl _tcsistr(const TCHAR *string1, const TCHAR *string2)
{
	size_t    length1, length2;
	ptrdiff_t offset;

#ifndef _MBCS
	length1 = _tcslen(string1);
	length2 = _tcslen(string2);
#else
	length1 = strlen((const char *)string1);
	length2 = strlen((const char *)string2);
#endif
	if (!length2)
		return (TCHAR *)string1;
	if (length2 >= length1)
		return length2 == length1 && _tcsicmp(string1, string2) == 0 ?
			(TCHAR *)string1 :
			NULL;
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
	string1 -= (offset = length2 - length1);
	do
#ifndef _MBCS
		if (_tcsnicmp(string1 + offset, string2, length2) == 0)
			return (TCHAR *)string1 + offset;
#else
		if (_mbsnbicmp(string1 + offset, string2, length2) == 0)
			return (TCHAR *)string1 + offset;
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, string1[offset]) && !++offset)
			break;
#endif
	while (++offset);
	return NULL;
}
