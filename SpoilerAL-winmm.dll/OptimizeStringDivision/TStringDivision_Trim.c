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

#ifndef _M_IX86
char * __fastcall TrimLeft(const char *first)
{
	char c;

	while ((c = *(first++)) == ' ' || c == '\t');
	return (char *)(first - 1);
}
#else
__declspec(naked) char * __fastcall TrimLeft(const char *first)
{
	__asm
	{
		#define first ecx

	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
		lea     eax, [ecx - 1]
		ret

		#undef first
	}
}
#endif

#ifndef _M_IX86
char * __fastcall TrimRight(const char *first, const char *last)
{
	if (last <= first)
	{
		if (last < first)
			last = first;
	}
	else
	{
		do
		{
			char c;

			if ((c = *(--last)) != ' ' && c != '\t')
			{
				++last;
				break;
			}
		} while (last != first);
	}
	return last;
}
#else
__declspec(naked) char * __fastcall TrimRight(const char *first, const char *last)
{
	__asm
	{
		#define first ecx
		#define last  edx

		cmp     edx, ecx
		ja      L1
		cmovb   edx, ecx
		mov     eax, edx
		jmp     L3

		align   16
	L1:
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		je      L2
		lea     eax, [edx + 1]
		jmp     L3
	L2:
		cmp     edx, ecx
		jne     L1
		mov     eax, edx
	L3:
		ret

		#undef first
		#undef last
	}
}
#endif

#ifndef _M_IX86
char * __fastcall TrimPointer(const char **pfirst, const char *last)
{
	const char *first;

	if (last <= (first = *pfirst))
	{
		if (last < first)
			last = first;
	}
	else
	{
		char c;

		while ((c = *(--last)) == ' ' || c == '\t')
			if (last == first)
				goto TRIMED;
		while ((c = *(first++)) == ' ' || c == '\t');
		--first;
		++last;
	}
TRIMED:
	*pfirst = (char *)first;
	return last;
}
#else
__declspec(naked) char * __fastcall TrimPointer(const char **pfirst, const char *last)
{
	__asm
	{
		#define pfirst ecx
		#define last   edx

		push    ecx
		mov     ecx, dword ptr [ecx]
		cmp     edx, ecx
		ja      L1
		cmovb   edx, ecx
		jmp     L4

		align   16
	L1:
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		jne     L3
	L2:
		cmp     edx, ecx
		jne     L1
		jmp     L4

		align   16
	L3:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L3
		cmp     al, '\t'
		je      L3
		dec     ecx
		inc     edx
	L4:
		mov     eax, edx
		pop     edx
		mov     dword ptr [edx], ecx
		ret

		#undef pfirst
		#undef last
	}
}
#endif

#ifndef _M_IX86
string * __fastcall TrimString(string *s)
{
	char *begin, *end;

	begin = string_begin(s);
	end = string_end(s);
	if (end > begin)
	{
		char c;

		while ((c = *(--end)) == ' ' || c == '\t')
			if (end == begin)
				goto TRIMED;
		while ((c = *(begin++)) == ' ' || c == '\t');
		--begin;
		++end;
		if (begin != string_begin(s))
		{
			size_t length;

			length = end - begin;
			end = string_begin(s) + length;
			memcpy(string_begin(s), begin, length);
		}
	TRIMED:
		*(string_end(s) = end) = '\0';
	}
	return s;
}
#else
#pragma function(memcpy)
__declspec(naked) string * __fastcall TrimString(string *s)
{
	__asm
	{
		#define s ecx

		push    ebx
		mov     edx, dword ptr [ecx]
		mov     ebx, dword ptr [ecx + 4]
		cmp     ebx, edx
		jbe     L5
	L1:
		mov     al, byte ptr [ebx - 1]
		dec     ebx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		jne     L3
	L2:
		cmp     ebx, edx
		jne     L1
		jmp     L4

		align   16
	L3:
		mov     al, byte ptr [edx]
		inc     edx
		cmp     al, ' '
		je      L3
		cmp     al, '\t'
		je      L3
		dec     edx
		inc     ebx
		mov     eax, dword ptr [ecx]
		cmp     eax, edx
		je      L4
		sub     ebx, edx
		push    ecx
		push    ebx
		push    edx
		push    eax
		add     ebx, eax
		call    memcpy
		mov     ecx, dword ptr [esp + 12]
		add     esp, 16
	L4:
		mov     byte ptr [ebx], '\0'
		mov     dword ptr [ecx + 4], ebx
	L5:
		mov     eax, ecx
		pop     ebx
		ret

		#undef s
	}
}
#endif
