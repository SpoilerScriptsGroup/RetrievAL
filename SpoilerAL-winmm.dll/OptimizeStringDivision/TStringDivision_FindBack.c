#include <windows.h>
#include "intrinsic.h"
#include "TStringDivision.h"

#define ESCAPE_TAG          '\\'
#define MAX_NEST_TAG_LENGTH 2

unsigned long __cdecl TStringDivision_FindBack(
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option)
{
	size_t TokenLength;
	size_t SrcLength;
	LPCSTR SrcIt, SrcEnd;
	size_t NestStartTagLength;
	size_t NestEndTagLength;
	size_t FindIndex;

	if (ToIndex == FromIndex)
		goto FAILED;

	TokenLength = bcb6_std_string_length(&Token);
	SrcLength = bcb6_std_string_length(Src);

	// 原文より比較文の方が短いなんて論外(^^;
	if (SrcLength < TokenLength)
		goto FAILED;

	// 検索終端がおかしければ補正
	if (SrcLength < FromIndex || SrcLength < FromIndex + TokenLength)
		FromIndex = SrcLength - TokenLength + 1;

	SrcIt = Src->_M_start + ToIndex;
	SrcEnd = Src->_M_start + FromIndex;

	if (Option & DT_NEST)
	{
		NestStartTagLength = bcb6_std_string_length(&_this->nestStartTag);
		NestEndTagLength = bcb6_std_string_length(&_this->nestEndTag);
		if (NestStartTagLength == 0 || NestEndTagLength == 0)
			goto FAILED;
	}

	// ただのパターンマッチングならBoyer-Moore法って手もあるが、
	// 2バイト文字やネスト、エスケープシーケンスも許可しているので1つづつ(^^;)

	FindIndex = (unsigned long)SIZE_MAX;
	if (Option & DT_NEST)
	{
		// ネストチェックあり
		if (Option & DT_ESCAPE)
		{
			// エスケープシーケンス使用
			while (SrcIt < SrcEnd)
			{
				if (*SrcIt != ESCAPE_TAG)
				{
					if (SrcIt[0] == _this->nestStartTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == _this->nestStartTag._M_start[1]))
					{
						size_t NCount;

						// ネスト開始
						NCount = 1;
						SrcIt += NestStartTagLength;
						while (SrcIt < SrcEnd)
						{
							if (*SrcIt != ESCAPE_TAG)
							{
								if (SrcIt[0] == _this->nestStartTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == _this->nestStartTag._M_start[1]))
								{
									// さらにネスト
									SrcIt += NestStartTagLength;
									NCount++;
									continue;
								}
								if (SrcIt[0] == _this->nestEndTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == _this->nestEndTag._M_start[1]))
								{
									// ネスト(一段)解除
									SrcIt += NestEndTagLength;
									if (--NCount == 0)
										break;	// ネスト完全脱出
									continue;
								}
							}
							else
							{
								SrcIt++;
							}
							if (!__intrinsic_isleadbyte(*SrcIt))
								SrcIt++;
							else
								SrcIt += 2;
						}
						continue;	// 直後にまたネスト開始もありえるでの。
					}

					// 基本比較処理
					if (memcmp(SrcIt, Token._M_start, TokenLength) == 0)
						FindIndex = SrcIt - Src->_M_start;
				}
				else
				{
					// エスケープシーケンスに引っかかりました
					SrcIt++;
				}

				if (!__intrinsic_isleadbyte(*SrcIt))
					SrcIt++;
				else
					SrcIt += 2;
			}
		}
		else
		{
			while (SrcIt < SrcEnd)
			{
				if (SrcIt[0] == _this->nestStartTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == _this->nestStartTag._M_start[1]))
				{
					size_t NCount;

					// ネスト開始
					NCount = 1;
					SrcIt += NestStartTagLength;
					while (SrcIt < SrcEnd)
					{
						if (SrcIt[0] == _this->nestStartTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == _this->nestStartTag._M_start[1]))
						{
							// さらにネスト
							SrcIt += NestStartTagLength;
							NCount++;
							continue;
						}
						if (SrcIt[0] == _this->nestEndTag._M_start[0] && (NestStartTagLength <= 1 || SrcIt[1] == _this->nestEndTag._M_start[1]))
						{
							// ネスト(一段)解除
							SrcIt += NestEndTagLength;
							if (--NCount == 0)
								break;	// ネスト完全脱出
							continue;
						}
						if (!__intrinsic_isleadbyte(*SrcIt))
							SrcIt++;
						else
							SrcIt += 2;
					}
					continue;	// 直後にまたネスト開始もありえるでの。
				}

				// 基本比較処理
				if (memcmp(SrcIt, Token._M_start, TokenLength) == 0)
					FindIndex = SrcIt - Src->_M_start;

				if (!__intrinsic_isleadbyte(*SrcIt))
					SrcIt++;
				else
					SrcIt += 2;
			}
		}
	}
	else if (Option & DT_ESCAPE)
	{
		// エスケープシーケンス使用
		while (SrcIt < SrcEnd)
		{
			if (*SrcIt != ESCAPE_TAG)
			{
				// 基本比較処理
				if (memcmp(SrcIt, Token._M_start, TokenLength) == 0)
					FindIndex = SrcIt - Src->_M_start;
			}
			else
			{
				// エスケープシーケンスに引っかかりました
				SrcIt++;
			}
			if (!__intrinsic_isleadbyte(*SrcIt))
				SrcIt++;
			else
				SrcIt += 2;
		}
	}
	else
	{
		// 単純検索
		while (SrcIt < SrcEnd)
		{
			// 基本比較処理
			if (memcmp(SrcIt, Token._M_start, TokenLength) == 0)
				FindIndex = SrcIt - Src->_M_start;

			if (!__intrinsic_isleadbyte(*SrcIt))
				SrcIt++;
			else
				SrcIt += 2;
		}
	}

	bcb6_std_string_dtor(&Token);
	return (unsigned long)FindIndex;

FAILED:
	bcb6_std_string_dtor(&Token);
	return (unsigned long)SIZE_MAX;
}

