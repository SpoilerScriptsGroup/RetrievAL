#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

extern HANDLE hHeap;

string * __cdecl FixToggleByteArray(string *Result, TStringDivision *strD, string *Src, string Token, unsigned long Index, unsigned long Option)
{
	LPSTR lpFind, lpPtr;
	char  ch;

	string_dtor(&Token);
	string_ctor(Result);
	lpFind = NULL;
	for (lpPtr = string_begin(Src); (ch = *lpPtr) != '\0'; lpPtr++)
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
				if ((BYTE)ch < (BYTE)'0' || (BYTE)ch > (BYTE)'8')
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
		string_assign_cstr_with_length(Result, "-", 1);
	}
	else
	{
		size_t count;

		*lpFind = '\0';
		string_assign_range(Result, string_c_str(Src), lpFind);
		count = string_end(Src) - (lpFind + 1) + 1;
		string_end(Src) = string_begin(Src) + count - 1;
		__movsb(string_begin(Src), lpFind + 1, count);
	}
	return Result;
}
