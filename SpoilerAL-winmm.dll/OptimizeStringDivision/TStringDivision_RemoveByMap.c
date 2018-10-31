#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

string * __cdecl TStringDivision_RemoveByMap(
	OUT string          *Result,
	IN  TStringDivision *this,
	IN  const string    *Src,
	IN  const map       *ReplaceMap,
	IN  unsigned long   Reserved)
{
	size_t resultLength;

	string_ctor_assign(Result, Src);
	if (ReplaceMap == NULL)
		ReplaceMap = &this->replaceMap;
	resultLength = string_length(Result);
	if (resultLength)
	{
		LPSTR p;

		p = Result->_M_start;
		while (p < Result->_M_finish)
		{
			map_iterator it;

			it = map_end(ReplaceMap);
			do
			{
				string *token;
				size_t tokenLength;
				string *dest;
				size_t destLength;

				map_iterator_decrement(it);
				token = (string *)pair_first(it);
				tokenLength = string_length(token);
				if (tokenLength == 0)
					continue;
				if (memcmp(p, token->_M_start, tokenLength) != 0)
					continue;
				dest = (string *)pair_second(it, string);
				destLength = string_length(dest);
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
					string_resize(Result, resultLength += destLength - tokenLength);
					p += (size_t)Result->_M_start;
					moveDest = p + destLength;
					memmove(moveDest, p + tokenLength, Result->_M_finish - moveDest);
					memcpy(p, dest->_M_start, destLength);
					p = moveDest;
				}
				goto NESTED_CONTINUE;
			} while (it != map_begin(ReplaceMap));
			if (!__intrinsic_isleadbyte(*p))
				p++;
			else
				p += 2;
		NESTED_CONTINUE:;
		}
	}
	return Result;
}
