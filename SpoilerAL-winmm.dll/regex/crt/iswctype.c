#include <windows.h>

int __cdecl iswctype(wint_t c, wctype_t desc)
{
	WORD wCharType;
	return GetStringTypeW(CT_CTYPE1, &c, 1, &wCharType) && (wCharType & desc);
}
