#include <windows.h>
#include "intrinsic.h"
#include "TStringDivision.h"

#pragma function(memcmp)

bcb6_std_string * __cdecl TStringDivision_Remove(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	char            *Dest,
	unsigned long   Option)
{
	size_t tokenLength;

	bcb6_std_string_ctor_assign(Result, Src);
	tokenLength = bcb6_std_string_length(&Token);
	if (tokenLength != 0)
	{
		LPSTR  p;
		size_t destLength;

		p = Result->_M_start;
		destLength = Dest != NULL ? strlen(Dest) : 0;
		if (destLength == tokenLength)
		{
			while (p < Result->_M_finish)
			{
				if (memcmp(p, Token._M_start, destLength) == 0)
				{
					memcpy(p, Dest, destLength);
					p += destLength;
				}
				else if (!__intrinsic_isleadbyte(*p))
					p++;
				else
					p += 2;
			}
		}
		else
		{
			size_t resultLength;

			resultLength = bcb6_std_string_length(Result);
			if (destLength < tokenLength)
			{
				if (destLength == 0)
				{
					while (p < Result->_M_finish)
					{
						if (memcmp(p, Token._M_start, tokenLength) == 0)
						{
							LPSTR copySrc;

							copySrc = p + tokenLength;
							memcpy(p += destLength, copySrc, Result->_M_finish - copySrc + 1);
							Result->_M_finish = Result->_M_start + (resultLength -= tokenLength);
						}
						else if (!__intrinsic_isleadbyte(*p))
							p++;
						else
							p += 2;
					}
				}
				else
				{
					while (p < Result->_M_finish)
					{
						if (memcmp(p, Token._M_start, tokenLength) == 0)
						{
							LPSTR copySrc;

							memcpy(p, Dest, destLength);
							copySrc = p + tokenLength;
							memcpy(p += destLength, copySrc, Result->_M_finish - copySrc + 1);
							Result->_M_finish = Result->_M_start + (resultLength -= tokenLength - destLength);
						}
						else if (!__intrinsic_isleadbyte(*p))
							p++;
						else
							p += 2;
					}
				}
			}
			else
			{
				while (p < Result->_M_finish)
				{
					if (memcmp(p, Token._M_start, tokenLength) == 0)
					{
						LPSTR moveDest;

						p -= (size_t)Result->_M_start;
						bcb6_std_string_reserve(Result, resultLength += destLength - tokenLength);
						*(Result->_M_finish = Result->_M_start + resultLength) = '\0';
						p += (size_t)Result->_M_start;
						moveDest = p + destLength;
						memmove(moveDest, p + tokenLength, Result->_M_finish - moveDest);
						memcpy(p, Dest, destLength);
						p = moveDest;
					}
					else if (!__intrinsic_isleadbyte(*p))
						p++;
					else
						p += 2;
				}
			}
		}
	}
	bcb6_std_string_dtor(&Token);
	return Result;
}
