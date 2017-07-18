#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_set.h"
#include "TStringDivision.h"

#pragma function(memcmp)

bcb6_std_string * __cdecl TStringDivision_Trim(
	bcb6_std_string *Result,
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_set    *TrimSet,
	unsigned long   Option)
{
	bcb6_std_string_ctor_assign(Result, Src);
	if ((Option & ET_TRIM) && !bcb6_std_string_empty(Result))
	{
		LPCSTR begin;

		begin = Result->_M_start;
		if (Option & ET_TRIM_L)
		{
			do
			{
				bcb6_std_set_iterator it;

				it = bcb6_std_set_end(TrimSet);
				do
				{
					bcb6_std_string *str;
					size_t          length;

					bcb6_std_set_iterator_decrement(it);
					str = (bcb6_std_string *)it->element;
					if (memcmp(begin, str->_M_start, length = bcb6_std_string_length(str)) == 0)
					{
						begin += length;
						goto NESTED_CONTINUE1;
					}
				} while (it != bcb6_std_set_begin(TrimSet));
				break;
			NESTED_CONTINUE1:;
			} while (begin < Result->_M_finish);
		}
		if ((Option & ET_TRIM_R) && Result->_M_finish > begin)
		{
#if !CODEPAGE_SUPPORT
			BOOL                  reverseScan;
#endif
			bcb6_std_set_iterator it;
			LPCSTR                end;

#if !CODEPAGE_SUPPORT
			reverseScan = TRUE;
			it = bcb6_std_set_end(TrimSet);
			do
			{
				bcb6_std_string *str;

				bcb6_std_set_iterator_decrement(it);
				str = (bcb6_std_string *)it->element;
				if (bcb6_std_string_length(str) > 1 || __intrinsic_istailbyte(*str->_M_start))
				{
					reverseScan = FALSE;
					break;
				}
			} while (it != bcb6_std_set_begin(TrimSet));
			end = NULL;
			if (reverseScan)
			{
				LPCSTR p;

				p = Result->_M_finish;
				do
				{
					char ch;

					ch = *--p;
					it = bcb6_std_set_end(TrimSet);
					do
					{
						bcb6_std_string *str;

						bcb6_std_set_iterator_decrement(it);
						str = (bcb6_std_string *)it->element;
						if (*str->_M_start == ch)
							goto NESTED_CONTINUE2;
					} while (it != bcb6_std_set_begin(TrimSet));
					break;
				NESTED_CONTINUE2:;
				} while ((end = p) > begin);
			}
			else
#endif
			{
				LPCSTR p;

				p = begin;
				do
				{
					it = bcb6_std_set_end(TrimSet);
					do
					{
						bcb6_std_string *str;
						size_t          length;

						bcb6_std_set_iterator_decrement(it);
						str = (bcb6_std_string *)it->element;
						if (memcmp(p, str->_M_start, length = bcb6_std_string_length(str)) == 0)
						{
							if (end == NULL)
								end = p;
							p += length;
							goto NESTED_CONTINUE3;
						}
					} while (it != bcb6_std_set_begin(TrimSet));
					end = NULL;
					if (!__intrinsic_isleadbyte(*p))
						p++;
					else
						p += 2;
				NESTED_CONTINUE3:;
				} while (p < Result->_M_finish);
			}
			if (end != NULL)
			{
				size_t length;

				length = end - begin;
				Result->_M_finish = Result->_M_start + length;
				if (begin != Result->_M_start)
					memcpy(Result->_M_start, begin, length);
				*Result->_M_finish = '\0';
				return Result;
			}
		}
		if (begin != Result->_M_start)
		{
			size_t length;

			length = Result->_M_finish - begin;
			Result->_M_finish = Result->_M_start + length;
			memcpy(Result->_M_start, begin, length + 1);
		}
	}
	return Result;
}
