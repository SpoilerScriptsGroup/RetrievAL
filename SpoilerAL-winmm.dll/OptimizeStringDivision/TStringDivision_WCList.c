#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_vector.h"
#include "TStringDivision.h"

#pragma function(memcmp)

EXTERN_C void __fastcall bcb6_std_vector_string_clear(bcb6_std_vector *vec);
EXTERN_C void __fastcall bcb6_std_vector_string_resize(bcb6_std_vector *vec, size_t size);

__inline void bcb6_std_vector_string_push_back(bcb6_std_vector *vec, bcb6_std_string *str)
{
	bcb6_std_vector_string_resize(vec, bcb6_std_vector_size(vec, bcb6_std_string) + 1);
	bcb6_std_string_assign(bcb6_std_vector_end(vec, bcb6_std_string) - 1, str);
}

unsigned long __stdcall TStringDivision_Find_WithoutTokenDtor(
	TStringDivision *_this,
	bcb6_std_string *Src,
	LPCSTR          TokenBegin,
	LPCSTR          TokenEnd,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option);

BOOLEAN __cdecl TStringDivision_WCList(
	TStringDivision *_this,
	bcb6_std_string *Val1,
	bcb6_std_string *Val2,
	bcb6_std_vector *List1,
	bcb6_std_vector *List2)
{
	bcb6_std_string StockS1;
	bcb6_std_string StockS2;
	bcb6_std_string NextWord;
	size_t          Val1Length;
	size_t          Val2Length;
	size_t          Val1Pos, Val2Pos;
	BOOLEAN         Ret;

	bcb6_std_string_ctor(&StockS1);
	bcb6_std_string_ctor(&StockS2);
	bcb6_std_string_ctor(&NextWord);

	if (List1 != NULL)
		bcb6_std_vector_string_clear(List1);
	if (List2 != NULL)
		bcb6_std_vector_string_clear(List2);
	Val1Length = bcb6_std_string_length(Val1);
	Val2Length = bcb6_std_string_length(Val2);
	Val1Pos = 0;
	Val2Pos = 0;
	Ret = TRUE;
	for (; Val1Pos < Val1Length; Val1Pos++)
	{
		size_t charLength;
		char   PosVal;

		PosVal = Val1->_M_start[Val1Pos];
		if (!__intrinsic_isleadbyte(PosVal))
		{
			if (PosVal == '*')
			{
				size_t FindPos;
				size_t NextPos;
				size_t QCount;

				if (!bcb6_std_string_empty(&StockS1))
				{
					// ここまでの文字列が一致するなら、push
					if (!bcb6_std_string_equals(&StockS1, &StockS2))
					{
						Ret = FALSE;
						break;
					}
					if (List1 != NULL)
						bcb6_std_vector_string_push_back(List1, &StockS1);
					if (List2 != NULL)
						bcb6_std_vector_string_push_back(List2, &StockS2);
				}
				bcb6_std_string_assign_cstr_with_length(&StockS1, "*", 1);
				bcb6_std_string_clear(&StockS2);

				FindPos = SIZE_MAX;
				NextPos = Val1Pos;
				QCount = 0;	// '?'の連続数
				while (++NextPos < Val1Length)
				{
					PosVal = Val1->_M_start[NextPos];
					if (!__intrinsic_isleadbyte(PosVal))
					{
						if (PosVal == '*')
						{
							bcb6_std_string_add_char(&StockS1, '*');
						}
						else if (PosVal == '?')
						{
							bcb6_std_string_add_char(&StockS1, '?');
							QCount++;
						}
						else
						{
							FindPos = NextPos;
							break;
						}
					}
					else
					{
						FindPos = NextPos;
						break;
					}
					Val1Pos = NextPos;
				}
				if (FindPos != SIZE_MAX)
				{
					size_t TailPos;

					for (TailPos = FindPos; TailPos < Val1Length; TailPos++)
					{
						char ch;

						ch = Val1->_M_start[TailPos];
						if (!__intrinsic_isleadbyte(ch))
						{
							if (ch == '*' || ch == '?')
								break;
						}
						else
						{
							TailPos++;
						}
					}
					bcb6_std_string_assign_cstr_with_length(&NextWord, Val1->_M_start + FindPos, TailPos - FindPos);
				}

				if (QCount)
				{
					// '?'との併用なら
					while (Val2Pos < Val2Length)
					{
						if (!__intrinsic_isleadbyte(Val2->_M_start[Val2Pos]))
						{
							bcb6_std_string_add_char(&StockS2, Val2->_M_start[Val2Pos++]);
						}
						else
						{
							// 2バイト文字
							bcb6_std_string_add_wide_char(&StockS2, *(wchar_t *)(Val2->_M_start + Val2Pos));
							Val2Pos += 2;
						}

						if (--QCount == 0)
							break;
					}
					if (QCount)
					{
						// Val2は終端。'?'が消化しきれていない！
						Ret = FALSE;
						// 比較ループを抜ける
						break;
					}
				}

				if (bcb6_std_string_empty(&NextWord))
				{
					// 次語がない=最後まで'?'か'*'だった
					// (且つ、'?'の個数分は適用済み)
					bcb6_std_string_add_string(&StockS2, Val2->_M_start + Val2Pos, Val2Length - Val2Pos);
					Val2Pos = Val2Length;
				}
				else
				{
					size_t WordPos;

					WordPos = TStringDivision_Find_WithoutTokenDtor(_this, Val2, NextWord._M_start, NextWord._M_finish, Val2Pos, (unsigned long)SIZE_MAX, 0);

					if (WordPos == (unsigned long)SIZE_MAX)
					{
						// 次語を発見できなかった
						Ret = FALSE;
						// 比較ループを抜ける
						break;
					}
					bcb6_std_string_add_string(&StockS2, Val2->_M_start + Val2Pos, WordPos - Val2Pos);

					Val2Pos = WordPos;
				}

				if (List1 != NULL)
					bcb6_std_vector_string_push_back(List1, &StockS1);
				if (List2 != NULL)
					bcb6_std_vector_string_push_back(List2, &StockS2);
				bcb6_std_string_clear(&StockS1);
				bcb6_std_string_clear(&StockS2);
				continue;
			}
			else if (PosVal == '?')
			{
				size_t QCount;

				if (!bcb6_std_string_empty(&StockS1))
				{
					// ここまでの文字列が一致するなら、push
					if (!bcb6_std_string_equals(&StockS1, &StockS2))
					{
						Ret = FALSE;
						break;
					}
					if (List1 != NULL)
						bcb6_std_vector_string_push_back(List1, &StockS1);
					if (List2 != NULL)
						bcb6_std_vector_string_push_back(List2, &StockS2);
					bcb6_std_string_clear(&StockS1);
					bcb6_std_string_clear(&StockS2);
				}

				QCount = 1;	// '?'の連続数
				while (++Val1Pos < Val1Length)
				{
					if (Val1->_M_start[Val1Pos] != '?')
					{
						// '?'が連続していないなら
						Val1Pos--;
						break;
					}
					QCount++;
				}

				// 必要分、'?'を追加
				bcb6_std_string_append(&StockS1, QCount, '?');

				while (Val2Pos < Val2Length)
				{
					if (!__intrinsic_isleadbyte(Val2->_M_start[Val2Pos]))
					{
						bcb6_std_string_add_char(&StockS2, Val2->_M_start[Val2Pos++]);
					}
					else
					{
						// 2バイト文字
						bcb6_std_string_add_wide_char(&StockS2, *(wchar_t *)(Val2->_M_start + Val2Pos));
						Val2Pos += 2;
					}

					if (--QCount == 0)
						break;
				}

				if (QCount)
				{
					BOOL EndFlag;

					// Val2は終端。Val1の残りは'*'だけか？
					EndFlag = FALSE;
					for (++Val1Pos; Val1Pos < Val1Length; Val1Pos++)
					{
						PosVal = Val1->_M_start[Val1Pos];
						if (PosVal != '*')
						{
							EndFlag = TRUE;
							break;
						}
						bcb6_std_string_add_char(&StockS1, '*');
					}
					// 最後まで'*'だったなら、Val1PosとVal1Lengthは既に等価(Loop脱出)
					if (EndFlag)
					{
						Ret = FALSE;
						// 比較ループを抜ける
						break;
					}
				}

				if (List1 != NULL)
					bcb6_std_vector_string_push_back(List1, &StockS1);
				if (List2 != NULL)
					bcb6_std_vector_string_push_back(List2, &StockS2);
				bcb6_std_string_clear(&StockS1);
				bcb6_std_string_clear(&StockS2);
				continue;
			}
			else
			{
				if (PosVal != Val2->_M_start[Val2Pos])
				{
					Ret = FALSE;
					break;
				}
			}
			charLength = 1;
		}
		else
		{
			if (PosVal != Val2->_M_start[Val2Pos] || Val1->_M_start[Val1Pos + 1] != Val2->_M_start[Val2Pos + 1])
			{
				Ret = FALSE;
				break;
			}
			charLength = 2;
		}

		for (; ; )
		{
			bcb6_std_string_add_char(&StockS1, PosVal);
			bcb6_std_string_add_char(&StockS2, Val2->_M_start[Val2Pos++]);

			if (Val2Pos >= Val2Length)
			{
				// Val2は終端。Val1の残りは'*'だけか？
				if (List1 != NULL)
					bcb6_std_vector_string_push_back(List1, &StockS1);
				if (List2 != NULL)
					bcb6_std_vector_string_push_back(List2, &StockS2);
				bcb6_std_string_clear(&StockS1);
				bcb6_std_string_clear(&StockS2);

				for (++Val1Pos; Val1Pos < Val1Length; Val1Pos++)
				{
					PosVal = Val1->_M_start[Val1Pos];
					if (PosVal != '*')
					{
						Ret = FALSE;
						break;
					}
					bcb6_std_string_add_char(&StockS1, '*');
				}
				goto NESTED_BREAK;
			}
			if (!--charLength)
				break;
			Val1Pos++;
		}
	}
NESTED_BREAK:

	if (Ret)
	{
		// まだ一致しているときだけ、残りを比較
		if (bcb6_std_string_equals(&StockS1, &StockS2) && (Val2Pos >= Val2Length))
		{
			if (!bcb6_std_string_empty(&StockS1))
			{
				if (List1 != NULL)
					bcb6_std_vector_string_push_back(List1, &StockS1);
				if (List2 != NULL)
					bcb6_std_vector_string_push_back(List2, &StockS2);
			}
		}
		else
			Ret = FALSE;
	}

	bcb6_std_string_dtor(&NextWord);
	bcb6_std_string_dtor(&StockS2);
	bcb6_std_string_dtor(&StockS1);

	return Ret;
}
