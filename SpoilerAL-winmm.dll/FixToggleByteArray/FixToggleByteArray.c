#include <windows.h>
#include "intrinsic.h"
#include "TStringDivision.h"

extern HANDLE hHeap;

bcb6_std_string * __cdecl FixToggleByteArray(bcb6_std_string *Result, TStringDivision *strD, bcb6_std_string *Src, bcb6_std_string Token, unsigned long Index, unsigned long Option)
{
	LPSTR lpFind, lpPtr;
	char  ch;

	bcb6_std_string_dtor(&Token);
	bcb6_std_string_ctor(Result);
	lpFind = NULL;
	for (lpPtr = Src->_M_start; (ch = *lpPtr) != '\0'; lpPtr++)
	{
		if (!__intrinsic_isleadbyte(ch))
		{
			if (ch == '-')
			{
				lpFind = lpPtr;
				break;
			}
			if (ch == '$')
			{
				ch = *(lpPtr + 1);
				if ((BYTE)ch < (BYTE)'1' || (BYTE)ch > (BYTE)'4')
					continue;
				lpPtr++;
				while ((ch = *(++lpPtr)) != '\0')
				{
					if (!__intrinsic_isleadbyte(ch))
					{
						if (ch == '$')
						{
							if (*(lpPtr + 1) == '$')
								break;
							else
								continue;
						}
						if (ch != '\\')
							continue;
						if ((ch = *(++lpPtr)) == '\0')
							break;
						if (!__intrinsic_isleadbyte(ch))
							continue;
					}
					if ((ch = *(++lpPtr)) == '\0')
						break;
				}
				if (ch != '\0')
					continue;
				else
					break;
			}
			if (ch != '\\')
				continue;
			if ((ch = *(++lpPtr)) == '\0')
				break;
			if (!__intrinsic_isleadbyte(ch))
				continue;
		}
		if (*(++lpPtr) == '\0')
			break;
	}
	if (lpFind == NULL)
	{
		bcb6_std_string_assign_cstr_with_length(Result, "-", 1);
	}
	else
	{
		size_t count;

		*lpFind = '\0';
		bcb6_std_string_assign_cstr_with_length(Result, Src->_M_start, lpFind - Src->_M_start);
		count = Src->_M_finish - (lpFind + 1) + 1;
		Src->_M_finish = Src->_M_start + count - 1;
		__movsb(Src->_M_start, lpFind + 1, count);
	}
	return Result;
}
