#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

string * __cdecl TStringDivision_RemoveByMap(
	OUT    string          *Result,
	IN     TStringDivision *this,
	IN     const string    *Src,
	IN     const map       *ReplaceMap,
	IN     unsigned long   Reserved)
{
	size_t resultLength;

	string_ctor_assign(Result, Src);
	if (ReplaceMap == NULL)
		ReplaceMap = &this->replaceMap;
	resultLength = string_length(Result);
	if (resultLength)
	{
		LPSTR p;

		p = string_begin(Result);
		while (p < string_end(Result))
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
				if (memcmp(p, string_c_str(token), tokenLength) != 0)
					continue;
				dest = (string *)pair_second(it, string);
				destLength = string_length(dest);
				if (destLength == tokenLength)
				{
					memcpy(p, string_c_str(dest), destLength);
					p += destLength;
				}
				else if (destLength < tokenLength)
				{
					LPSTR copySrc;

					if (destLength != 0)
						memcpy(p, string_c_str(dest), destLength);
					copySrc = p + tokenLength;
					memcpy(p += destLength, copySrc, string_end(Result) - copySrc + 1);
					string_end(Result) = string_begin(Result) + (resultLength -= tokenLength - destLength);
				}
				else
				{
					LPSTR moveDest;

					p -= (size_t)string_begin(Result);
					string_resize(Result, resultLength += destLength - tokenLength);
					p += (size_t)string_begin(Result);
					moveDest = p + destLength;
					memmove(moveDest, p + tokenLength, string_end(Result) - moveDest);
					memcpy(p, string_c_str(dest), destLength);
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
