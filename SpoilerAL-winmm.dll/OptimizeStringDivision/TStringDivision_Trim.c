#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

string * __cdecl TStringDivision_TrimDefault(
	string          *Result,
	TStringDivision *_this,
	string          *Src,
	LPVOID          Reserved,
	unsigned long   Option)
{
	LPCSTR first, last;

	first = Src->_M_start;
	if (Option & ET_TRIM_L)
	{
		while (*first == ' ' || *first == '\t')
			first++;
	}
	last = Src->_M_finish;
	if (Option & ET_TRIM_R)
	{
		while (--last >= first && (*last == ' ' || *last == '\t'));
		last++;
	}
	string_ctor_assign_range(Result, first, last);
	return Result;
}

string * __cdecl TStringDivision_TrimFull(
	string          *Result,
	TStringDivision *_this,
	string          *Src,
	LPVOID          Reserved,
	unsigned long   Option)
{
	LPCSTR first, last;

	first = Src->_M_start;
	if (Option & ET_TRIM_L)
	{
		while (*first == ' ' || *first == '\t' || *first == '\r' || *first == '\n')
			first++;
	}
	last = Src->_M_finish;
	if (Option & ET_TRIM_R)
	{
		while (--last >= first && (*last == ' ' || *last == '\t' || *last == '\r' || *last == '\n'));
		last++;
	}
	string_ctor_assign_range(Result, first, last);
	return Result;
}

string * __cdecl TStringDivision_Trim(
	string          *Result,
	TStringDivision *_this,
	string          *Src,
	set             *TrimSet,
	unsigned long   Option)
{
	string_ctor_assign(Result, Src);
	if ((Option & ET_TRIM) && !string_empty(Result))
	{
		LPCSTR begin;

		begin = Result->_M_start;
		if (Option & ET_TRIM_L)
		{
			do
			{
				set_iterator it;

				it = set_end(TrimSet);
				do
				{
					string *str;
					size_t length;

					set_iterator_decrement(it);
					str = (string *)it->element;
					if (memcmp(begin, str->_M_start, length = string_length(str)) == 0)
					{
						begin += length;
						goto NESTED_CONTINUE1;
					}
				} while (it != set_begin(TrimSet));
				break;
			NESTED_CONTINUE1:;
			} while (begin < Result->_M_finish);
		}
		if ((Option & ET_TRIM_R) && Result->_M_finish > begin)
		{
#if !CODEPAGE_SUPPORT
			BOOL         reverseScan;
#endif
			set_iterator it;
			LPCSTR       end;

#if !CODEPAGE_SUPPORT
			reverseScan = TRUE;
			it = set_end(TrimSet);
			do
			{
				string *str;

				set_iterator_decrement(it);
				str = (string *)it->element;
				if (string_length(str) > 1 || __intrinsic_istailbyte(*str->_M_start))
				{
					reverseScan = FALSE;
					break;
				}
			} while (it != set_begin(TrimSet));
			end = NULL;
			if (reverseScan)
			{
				LPCSTR p;

				p = Result->_M_finish;
				do
				{
					char ch;

					ch = *--p;
					it = set_end(TrimSet);
					do
					{
						string *str;

						set_iterator_decrement(it);
						str = (string *)it->element;
						if (*str->_M_start == ch)
							goto NESTED_CONTINUE2;
					} while (it != set_begin(TrimSet));
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
					it = set_end(TrimSet);
					do
					{
						string *str;
						size_t length;

						set_iterator_decrement(it);
						str = (string *)it->element;
						if (memcmp(p, str->_M_start, length = string_length(str)) == 0)
						{
							if (end == NULL)
								end = p;
							p += length;
							goto NESTED_CONTINUE3;
						}
					} while (it != set_begin(TrimSet));
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
