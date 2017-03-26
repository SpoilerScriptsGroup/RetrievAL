#include "intrinsic.h"
#include "TStringDivision.h"

EXTERN_C void __fastcall TrimString(bcb6_std_string *s);

EXTERN_C bcb6_std_string * __cdecl TSSGCtrl_ByteArrayRemoveSpace(bcb6_std_string *Result, TStringDivision *StringDivision, bcb6_std_string *Src, bcb6_std_string Token, char *Dest, unsigned long Option)
{
	char *p1;

	bcb6_std_string_dtor(&Token);
	bcb6_std_string_ctor_assign(Result, Src);
	TrimString(Result);
	p1 = Result->_M_start;
	while (p1 < Result->_M_finish)
	{
		char ch;

		ch = *p1;
		if (!__intrinsic_isleadbyte(ch))
		{
			if (__intrinsic_isspace(ch))
			{
				char *p2;

				p2 = p1 + 1;
				while (__intrinsic_isspace(*p2))
					p2++;
				memcpy(p1, p2, Result->_M_finish - p2);
				*(Result->_M_finish -= p2 - p1) = '\0';
				continue;
			}
			if (ch == '$')
			{
				p1 += 2;
				for (; ; )
				{
					if (p1 >= Result->_M_finish)
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
