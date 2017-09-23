#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <float.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"

string * __cdecl TStringDivision_ToStringDouble(string *Result, double Src, const char *Format)
{
	char       buffer[512];
	const char *s;
	size_t     length;

	do	// do { ... } while (0);
	{
		const char floatMinString[] = "-340282346638528859811704183484516925440";
		const char doubleMinString[] = "-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368";

		if (!Format || (Format[0] == '%' && Format[1] == 'f' && Format[2] == '\0'))
		{
			if (Src == -FLT_MAX)
			{
				s = floatMinString;
				length = 40;
				break;
			}
			else if (Src == -DBL_MAX)
			{
				s = doubleMinString;
				length = 310;
				break;
			}
			else if (Src == FLT_MAX)
			{
				s = floatMinString + 1;
				length = 39;
				break;
			}
			else if (Src == DBL_MAX)
			{
				s = doubleMinString + 1;
				length = 309;
				break;
			}
		}
		length = _snprintf(buffer, _countof(buffer), !Format || _isnan(Src) ? "%f" : Format, Src);
		if ((unsigned int)length >= _countof(buffer))
			length = 0;
		s = buffer;
	} while (0);
	string_ctor_assign_cstr_with_length(Result, s, length);
	return Result;
}
