#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

EXTERN_C string * __cdecl TSSGCtrl_ByteArrayRemoveSpace(string *Result, TStringDivision *StringDivision, string *Src, string Token, char *Dest, unsigned long Option)
{
	char *p1, *p2;

	string_dtor(&Token);
	p1 = string_begin(Src);
	p2 = TrimBlank(&p1, string_end(Src));
	string_ctor_assign_cstr_with_length(Result, p1, p2 - p1);
	p1 = string_begin(Result);
	while (p1 < string_end(Result))
	{
		char ch;

		ch = *p1;
		if (!__intrinsic_isleadbyte(ch))
		{
			if (__intrinsic_isspace(ch))
			{
				p2 = p1 + 1;
				while (__intrinsic_isspace(*p2))
					p2++;
				memcpy(p1, p2, string_end(Result) - p2);
				*(string_end(Result) -= p2 - p1) = '\0';
				continue;
			}
			if (ch == '$')
			{
				p1 += 2;
				for (; ; )
				{
					if (p1 >= string_end(Result))
						return Result;
					ch = *p1;
					if (!__intrinsic_isleadbyte(ch))
					{
						p1++;
						if (ch != '$' || *p1 != '$')
							continue;
						else
							break;
					}
					else
						p1 += 2;
				}
			}
			p1++;
		}
		else
			p1 += 2;
	}
	return Result;
}
