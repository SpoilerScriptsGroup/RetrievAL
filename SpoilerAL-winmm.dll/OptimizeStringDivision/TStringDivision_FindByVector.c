#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

unsigned long __stdcall TStringDivision_Find_WithoutTokenDtor(
	IN     TStringDivision *this,
	IN     const string    *Src,
	IN     const char      *Token,
	IN     size_t          TokenLength,
	IN     unsigned long   FromIndex,
	IN     unsigned long   ToIndex,
	IN     unsigned long   Option);

unsigned long __cdecl TStringDivision_FindByVector(
	IN     TStringDivision     *this,
	IN     const string        *Src,
	IN     const vector_string *Token,
	OUT    unsigned long       *FindElementIndex,
	IN     unsigned long       FromIndex,
	IN     unsigned long       ToIndex,
	IN     unsigned long       Option)
{
	size_t SrcLength;
	string *TokenIt;

	if (FromIndex == ToIndex)
		return (unsigned long)SIZE_MAX;

	SrcLength = string_length(Src);
	if (SrcLength < ToIndex)
		ToIndex = SrcLength;

	if (FindElementIndex == NULL)
	{
		// 発見トークンの番号不要
		if (Option == 0)
		{
			LPCSTR SrcIt, SrcEnd;

			// エスケープシーケンス等無考慮
			SrcIt = Src->_M_start + FromIndex;
			SrcEnd = Src->_M_start + ToIndex;
			while (SrcIt < SrcEnd)
			{
				for (TokenIt = Token->_M_start; TokenIt != Token->_M_finish; TokenIt++)
					if (memcmp(SrcIt, TokenIt->_M_start, string_length(TokenIt)) == 0)
						return SrcIt - Src->_M_start;
				if (!__intrinsic_isleadbyte(*SrcIt))
					SrcIt++;
				else
					SrcIt += 2;
			}
		}
		else
		{
			size_t MinPos, tmpI;

			// エスケープシーケンス等を考慮
			MinPos = (unsigned long)SIZE_MAX;
			for (TokenIt = Token->_M_start; TokenIt != Token->_M_finish; TokenIt++)
			{
				tmpI = TStringDivision_Find_WithoutTokenDtor(this, Src, string_c_str(TokenIt), string_length(TokenIt), FromIndex, ToIndex, Option);
				if (tmpI < MinPos)
					MinPos = tmpI;
			}
			if (MinPos != (unsigned long)SIZE_MAX)
				return (unsigned long)MinPos;
		}
	}
	else
	{
		if (Option == 0)
		{
			LPCSTR SrcIt, SrcEnd;

			// エスケープシーケンス等無考慮
			SrcIt = Src->_M_start + FromIndex;
			SrcEnd = Src->_M_start + ToIndex;
			while (SrcIt < SrcEnd)
			{
				for (TokenIt = Token->_M_start; TokenIt != Token->_M_finish; TokenIt++)
				{
					if (memcmp(SrcIt, TokenIt->_M_start, string_length(TokenIt)) == 0)
					{
						*FindElementIndex = TokenIt - Token->_M_start;
						return SrcIt - Src->_M_start;
					}
				}
				if (!__intrinsic_isleadbyte(*SrcIt))
					SrcIt++;
				else
					SrcIt += 2;
			}
		}
		else
		{
			size_t i, MinPos, TokenNum, tmpI;

			// エスケープシーケンス等を考慮
			i = 0;
			MinPos = (unsigned long)SIZE_MAX;
			for (TokenIt = Token->_M_start; TokenIt != Token->_M_finish; TokenIt++)
			{
				tmpI = TStringDivision_Find_WithoutTokenDtor(this, Src, string_c_str(TokenIt), string_length(TokenIt), FromIndex, ToIndex, Option);
				if (tmpI < MinPos)
				{
					MinPos = tmpI;
					TokenNum = i;
				}
				i++;
			}
			if (MinPos != (unsigned long)SIZE_MAX)
			{
				*FindElementIndex = TokenNum;
				return (unsigned long)MinPos;
			}
		}
	}

	return (unsigned long)SIZE_MAX;
}
