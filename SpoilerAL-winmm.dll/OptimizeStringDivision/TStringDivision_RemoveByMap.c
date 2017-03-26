#include <windows.h>
#include "intrinsic.h"
#include "TStringDivision.h"

#pragma function(memcmp)

bcb6_std_string * __cdecl TStringDivision_RemoveByMap(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_map    *ReplaceMap,
	unsigned long   Option)
{
	size_t resultLength;

	bcb6_std_string_ctor_assign(Result, Src);
	if (ReplaceMap == NULL)
		ReplaceMap = &_this->replaceMap;
	resultLength = bcb6_std_string_length(Result);
	if (resultLength)
	{
		LPSTR p;

		p = Result->_M_start;
		while (p < Result->_M_finish)
		{
			bcb6_std_map_iterator it;

			it = bcb6_std_map_end(ReplaceMap);
			do
			{
				bcb6_std_string *token;
				size_t          tokenLength;
				bcb6_std_string *dest;
				size_t          destLength;

				bcb6_std_map_iterator_decrement(it);
				token = (bcb6_std_string *)bcb6_std_pair_first(it);
				tokenLength = bcb6_std_string_length(token);
				if (tokenLength == 0)
					continue;
				if (memcmp(p, token->_M_start, tokenLength) != 0)
					continue;
				dest = (bcb6_std_string *)bcb6_std_pair_second(it, bcb6_std_string);
				destLength = bcb6_std_string_length(dest);
				if (destLength == tokenLength)
				{
					memcpy(p, dest->_M_start, destLength);
					p += destLength;
				}
				else if (destLength < tokenLength)
				{
					LPSTR copySrc;

					if (destLength != 0)
						memcpy(p, dest->_M_start, destLength);
					copySrc = p + tokenLength;
					memcpy(p += destLength, copySrc, Result->_M_finish - copySrc + 1);
					Result->_M_finish = Result->_M_start + (resultLength -= tokenLength - destLength);
				}
				else
				{
					LPSTR moveDest;

					p -= (size_t)Result->_M_start;
					bcb6_std_string_reserve(Result, resultLength += destLength - tokenLength);
					*(Result->_M_finish = Result->_M_start + resultLength) = '\0';
					p += (size_t)Result->_M_start;
					moveDest = p + destLength;
					memmove(moveDest, p + tokenLength, Result->_M_finish - moveDest);
					memcpy(p, dest->_M_start, destLength);
					p = moveDest;
				}
				goto NESTED_CONTINUE;
			} while (it != bcb6_std_map_begin(ReplaceMap));
			if (!__intrinsic_isleadbyte(*p))
				p++;
			else
				p += 2;
		NESTED_CONTINUE:;
		}
	}
	return Result;
}
