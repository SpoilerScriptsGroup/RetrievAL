#include <windows.h>
#include "intrinsic.h"
#include "TStringDivision.h"

#pragma function(memcmp)

unsigned long __stdcall TStringDivision_Find_WithoutTokenDtor(
	TStringDivision *_this,
	bcb6_std_string *Src,
	LPCSTR          TokenBegin,
	LPCSTR          TokenEnd,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option)
{
	size_t TokenLength;
	size_t SrcLength;
	LPCSTR SrcIt, SrcEnd;
	size_t NestStartTagLength;
	size_t NestEndTagLength;
	size_t EscapeTagLength;

	if (FromIndex == ToIndex)
		goto FAILED;

	TokenLength = TokenEnd - TokenBegin;
	SrcLength = bcb6_std_string_length(Src);

	// 原文より比較文の方が短いなんて論外(^^;
	if (SrcLength < TokenLength)
		goto FAILED;

	// 検索終端がおかしければ補正
	if (SrcLength < ToIndex || SrcLength < ToIndex + TokenLength)
		ToIndex = SrcLength - TokenLength + 1;

	SrcIt = Src->_M_start + FromIndex;
	SrcEnd = Src->_M_start + ToIndex;

	if (Option & DT_NEST)
	{
		NestStartTagLength = bcb6_std_string_length(&_this->nestStartTag);
		NestEndTagLength = bcb6_std_string_length(&_this->nestEndTag);
		if (NestStartTagLength == 0 || NestEndTagLength == 0)
			goto FAILED;
	}
	if (Option & DT_ESCAPE)
	{
		EscapeTagLength = bcb6_std_string_length(&_this->escapeTag);
		if (EscapeTagLength == 0)
			goto FAILED;
	}

	// ただのパターンマッチングならBoyer-Moore法って手もあるが、
	// 2バイト文字やネスト、エスケープシーケンスも許可しているので1つづつ(^^;)

	if (Option & DT_NEST)
	{
		// ネストチェックあり
		if (Option & DT_ESCAPE)
		{
			// エスケープシーケンス使用
			while (SrcIt < SrcEnd)
			{
				if (memcmp(SrcIt, _this->escapeTag._M_start, EscapeTagLength) != 0)
				{
					if (memcmp(SrcIt, _this->nestStartTag._M_start, NestStartTagLength) == 0)
					{
						size_t NCount;

						// ネスト開始
						NCount = 1;
						SrcIt += NestStartTagLength;
						while (SrcIt < SrcEnd)
						{
							if (memcmp(SrcIt, _this->escapeTag._M_start, EscapeTagLength) != 0)
							{
								if (memcmp(SrcIt, _this->nestStartTag._M_start, NestStartTagLength) == 0)
								{
									// さらにネスト
									SrcIt += NestStartTagLength;
									NCount++;
									continue;
								}
								if (memcmp(SrcIt, _this->nestEndTag._M_start, NestEndTagLength) == 0)
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
								SrcIt += EscapeTagLength;
							}
							if (!__intrinsic_isleadbyte(*SrcIt))
								SrcIt++;
							else
								SrcIt += 2;
						}
						continue;	// 直後にまたネスト開始もありえるでの。
					}

					// 基本比較処理
					if (memcmp(SrcIt, TokenBegin, TokenLength) == 0)
						goto TOKEN_FOUND;
				}
				else
				{
					// エスケープシーケンスに引っかかりました
					SrcIt += EscapeTagLength;
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
				if (memcmp(SrcIt, _this->nestStartTag._M_start, NestStartTagLength) == 0)
				{
					size_t NCount;

					// ネスト開始
					NCount = 1;
					SrcIt += NestStartTagLength;
					while (SrcIt < SrcEnd)
					{
						if (memcmp(SrcIt, _this->nestStartTag._M_start, NestStartTagLength) == 0)
						{
							// さらにネスト
							SrcIt += NestStartTagLength;
							NCount++;
							continue;
						}
						if (memcmp(SrcIt, _this->nestEndTag._M_start, NestEndTagLength) == 0)
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
				if (memcmp(SrcIt, TokenBegin, TokenLength) == 0)
					goto TOKEN_FOUND;

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
			if (memcmp(SrcIt, _this->escapeTag._M_start, EscapeTagLength) != 0)
			{
				// 基本比較処理
				if (memcmp(SrcIt, TokenBegin, TokenLength) == 0)
					goto TOKEN_FOUND;
			}
			else
			{
				// エスケープシーケンスに引っかかりました
				SrcIt += EscapeTagLength;
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
			if (memcmp(SrcIt, TokenBegin, TokenLength) == 0)
			{
		TOKEN_FOUND:
				return (unsigned long)(SrcIt - Src->_M_start);
			}

			if (!__intrinsic_isleadbyte(*SrcIt))
				SrcIt++;
			else
				SrcIt += 2;
		}
	}

FAILED:
	return (unsigned long)SIZE_MAX;
}

unsigned long __cdecl TStringDivision_Find(
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option)
{
	unsigned long Result;

	Result = TStringDivision_Find_WithoutTokenDtor(
		_this,
		Src,
		Token._M_start,
		Token._M_finish,
		FromIndex,
		ToIndex,
		Option);
	bcb6_std_string_dtor(&Token);
	return Result;
}

