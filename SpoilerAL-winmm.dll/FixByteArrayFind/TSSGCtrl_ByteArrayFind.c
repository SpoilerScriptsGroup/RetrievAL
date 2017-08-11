#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TStringDivision.h"

LPCSTR __fastcall TSSGCtrl_ByteArraySkipReplacementType(
	LPCSTR        SrcIt,
	LPCSTR        SrcEnd,
	unsigned long Option)
{
	SrcIt++;
	for (; ; )
	{
		char ch;

		ch = *(SrcIt++);
		if (SrcIt >= SrcEnd)
			break;
		if (!__intrinsic_isleadbyte(ch))
		{
			if (ch != '\\')
			{
				if (ch != '$' || *SrcIt != '$')
					continue;
				SrcIt++;
				break;
			}
			if (!(Option & DT_ESCAPE))
				continue;
			ch = *SrcIt;
			if (__intrinsic_isleadbyte(ch))
				SrcIt++;
		}
		SrcIt++;
	}
	return SrcIt;
}

#define TokenLength 2
unsigned long __cdecl TSSGCtrl_ByteArrayFind(
	TStringDivision *StringDivision,
	string          *Src,
	string          Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option)
{
	size_t SrcLength;
	LPCSTR SrcIt, SrcEnd;

	if (FromIndex == ToIndex)
		goto FAILED;

	SrcLength = string_length(Src);

	if (SrcLength < TokenLength)
		goto FAILED;

	if (SrcLength < ToIndex || SrcLength < ToIndex + TokenLength)
		ToIndex = SrcLength - TokenLength + 1;

	SrcIt = Src->_M_start + FromIndex;
	SrcEnd = Src->_M_start + ToIndex;

	while (SrcIt < SrcEnd)
	{
		char ch;

		ch = *(SrcIt++);
		if (!__intrinsic_isleadbyte(ch))
		{
			size_t NCount;

			switch (ch)
			{
			case '*':
				ch = *SrcIt;
				if (Token._M_start[0] == '*' && Token._M_start[1] == ch)
					goto TOKEN_FOUND;
				switch (ch)
				{
				case '<':
				case '[':
				case '{':
					goto NEST_START_TAG1;
				default:
					continue;
				}
			case '<':
				if (*SrcIt != '_')
					continue;
			NEST_START_TAG1:
				NCount = 1;
				SrcIt++;
				for (; ; )
				{
					ch = *(SrcIt++);
					if (SrcIt >= SrcEnd)
						goto FAILED;
					if (!__intrinsic_isleadbyte(ch))
					{
						switch (ch)
						{
						case '*':
							switch (*SrcIt)
							{
							case '<':
							case '[':
							case '{':
								goto NEST_START_TAG2;
							case '>':
							case ']':
							case '}':
								goto NEST_END_TAG;
							default:
								continue;
							}
						case '<':
							if (*SrcIt != '_')
								continue;
						NEST_START_TAG2:
							NCount++;
							SrcIt++;
							continue;
						case '_':
							if (*SrcIt != '>')
								continue;
						NEST_END_TAG:
							if (--NCount == 0)
								goto NESTED_BREAK;
							SrcIt++;
							continue;
						case '$':
							SrcIt = TSSGCtrl_ByteArraySkipReplacementType(SrcIt, SrcEnd, Option);
							if (SrcIt < SrcEnd)
								continue;
							else
								goto FAILED;
						case '\\':
							if (!(Option & DT_ESCAPE))
								continue;
							ch = *SrcIt;
							if (__intrinsic_isleadbyte(ch))
								SrcIt++;
							break;
						default:
							continue;
						}
					}
					SrcIt++;
				}
			NESTED_BREAK:
				continue;
			case '$':
				SrcIt = TSSGCtrl_ByteArraySkipReplacementType(SrcIt, SrcEnd, Option);
				continue;
			case '\\':
				if (!(Option & DT_ESCAPE))
					continue;
				ch = *SrcIt;
				if (__intrinsic_isleadbyte(ch))
					SrcIt++;
				break;
			default:
				if (Token._M_start[0] == ch && Token._M_start[1] == *SrcIt)
				{
			TOKEN_FOUND:
					string_dtor(&Token);
					return SrcIt - Src->_M_start - 1;
				}
				continue;
			}
		}
		SrcIt++;
	}
FAILED:
	string_dtor(&Token);
	return SIZE_MAX;
}
#undef TokenLength
