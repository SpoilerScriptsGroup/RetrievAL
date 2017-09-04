#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

string * __cdecl TStringDivision_Remove(
	string          *Result,
	TStringDivision *this,
	string          *Src,
	string          Token,
	char            *Dest,
	unsigned long   Option)
{
	size_t tokenLength;

	string_ctor_assign(Result, Src);
	tokenLength = string_length(&Token);
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

			resultLength = string_length(Result);
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
						string_resize(Result, resultLength += destLength - tokenLength);
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
	string_dtor(&Token);
	return Result;
}
