#include <windows.h>
#include <assert.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

#ifndef _M_IX86
string * __cdecl TStringDivision_TrimDefault(
	OUT    string          *Result,
	IN     TStringDivision *Reserved1,
	IN     const string    *Src,
	IN     const void      *Reserved2,
	IN     unsigned long   Reserved3)
{
	LPCSTR first, last;
	char   c;

	assert((Reserved3 & etTRIM) == etTRIM);

	first = string_begin(Src);
	last = string_end(Src);
	do
		c = *(first++);
	while (c == ' ' || c == '\t');
	first--;
	while (last > first)
	{
		c = *(last - 1);
		last--;
		if (c == ' ' || c == '\t')
			continue;
		last++;
		break;
	}
	return string_ctor_assign_range(Result, first, last);
}
#else
__declspec(naked) string * __cdecl TStringDivision_TrimDefault(
	OUT    string          *Result,
	IN     TStringDivision *Reserved1,
	IN     const string    *Src,
	IN     const void      *Reserved2,
	IN     unsigned long   Reserved3)
{
	__asm
	{
		#define Result    (esp +  4)
		#define Reserved1 (esp +  8)
		#define Src       (esp + 12)
		#define Reserved2 (esp + 16)
		#define Reserved3 (esp + 20)

		mov     ecx, dword ptr [Src]
		mov     edx, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4]
	L1:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
	L2:
		cmp     ecx, edx
		jb      L3
		mov     al, byte ptr [ecx - 1]
		dec     ecx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		je      L2
		inc     ecx
	L3:
		pop     eax
		push    ecx
		push    eax
		dec     edx
		mov     ecx, dword ptr [Result + 4]
		jmp     string_ctor_assign_range

		#undef Result
		#undef Reserved1
		#undef Src
		#undef Reserved2
		#undef Reserved3
	}
}
#endif

#ifndef _M_IX86
string * __cdecl TStringDivision_TrimFull(
	OUT    string          *Result,
	IN     TStringDivision *Reserved1,
	IN     const string    *Src,
	IN     const void      *Reserved2,
	IN     unsigned long   Reserved3)
{
	LPCSTR first, last;
	char   c;

	assert((Reserved3 & etTRIM) == etTRIM);

	first = string_begin(Src);
	last = string_end(Src);
	do
		c = *(first++);
	while (c == ' ' || c == '\t' || c == '\r' || c == '\n');
	first--;
	while (last > first)
	{
		c = *(last - 1);
		last--;
		if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
			continue;
		last++;
		break;
	}
	return string_ctor_assign_range(Result, first, last);
}
#else
__declspec(naked) string * __cdecl TStringDivision_TrimFull(
	OUT    string          *Result,
	IN     TStringDivision *Reserved1,
	IN     const string    *Src,
	IN     LPVOID          Reserved2,
	IN     unsigned long   Reserved3)
{
	__asm
	{
		#define Result    (esp +  4)
		#define Reserved1 (esp +  8)
		#define Src       (esp + 12)
		#define Reserved2 (esp + 16)
		#define Reserved3 (esp + 20)

		mov     ecx, dword ptr [Src]
		mov     edx, dword ptr [ecx]
		mov     ecx, dword ptr [ecx + 4]
	L1:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
		cmp     al, '\r'
		je      L1
		cmp     al, '\n'
		je      L1
	L2:
		cmp     ecx, edx
		jb      L3
		mov     al, byte ptr [ecx - 1]
		dec     ecx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		je      L2
		cmp     al, '\r'
		je      L2
		cmp     al, '\n'
		je      L2
		inc     ecx
	L3:
		pop     eax
		push    ecx
		push    eax
		dec     edx
		mov     ecx, dword ptr [Result + 4]
		jmp     string_ctor_assign_range

		#undef Result
		#undef Reserved1
		#undef Src
		#undef Reserved2
		#undef Reserved3
	}
}
#endif

string * __cdecl TStringDivision_Trim(
	OUT    string          *Result,
	IN     TStringDivision *Reserved,
	IN     const string    *Src,
	IN     const set       *TrimSet,
	IN     unsigned long   Option)
{
	string_ctor_assign(Result, Src);
	if ((Option & etTRIM) && !string_empty(Result))
	{
		LPCSTR begin;

		begin = Result->_M_start;
		if (Option & etTRIM_L)
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
		if ((Option & etTRIM_R) && Result->_M_finish > begin)
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
				if (string_length(str) > 1 || __intrinsic_istrailbyte(*str->_M_start))
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
