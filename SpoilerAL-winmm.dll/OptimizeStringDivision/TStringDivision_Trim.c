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

	assert((Reserved3 & etTRIM) == etTRIM);

	first = string_begin(Src);
	last = string_end(Src);
	if (first > last)
	{
		char c;

		while ((c = *(first++)) == ' ' || c == '\t');
		if (last >= first)
		{
			while ((c = *(--last)) == ' ' || c == '\t');
			--first;
			++last;
			return string_ctor_assign_cstr_with_length(Result, first, last - first);
		}
	}
	return string_ctor(Result);
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

		mov     eax, dword ptr [Src]
		mov     ecx, dword ptr [eax]
		mov     edx, dword ptr [eax + 4]
		cmp     ecx, edx
		jb      L1
		jmp     L3

		align   16
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
		cmp     edx, ecx
		jb      L3
		dec     edx

		align   16
	L2:
		mov     al, byte ptr [edx]
		dec     edx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		je      L2
		sub     edx, ecx
		pop     eax
		add     edx, 3
		dec     ecx
		push    edx
		push    eax
		mov     edx, ecx
		mov     ecx, dword ptr [Result + 4]
		jmp     string_ctor_assign_cstr_with_length

		align   16
	L3:
		mov     ecx, dword ptr [Result]
		jmp     string_ctor

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

	assert((Reserved3 & etTRIM) == etTRIM);

	first = string_begin(Src);
	last = string_end(Src);
	if (first > last)
	{
		char c;

		while ((c = *(first++)) == ' ' || c == '\t' || c == '\r' || c == '\n');
		if (last >= first)
		{
			while ((c = *(--last)) == ' ' || c == '\t' || c == '\r' || c == '\n');
			--first;
			++last;
			return string_ctor_assign_cstr_with_length(Result, first, last - first);
		}
	}
	return string_ctor(Result);
}
#else
__declspec(naked) string * __cdecl TStringDivision_TrimFull(
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

		mov     eax, dword ptr [Src]
		mov     ecx, dword ptr [eax]
		mov     edx, dword ptr [eax + 4]
		cmp     ecx, edx
		jb      L1
		jmp     L3

		align   16
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
		cmp     al, '\r'
		je      L1
		cmp     al, '\n'
		je      L1
		cmp     edx, ecx
		jb      L3
		dec     edx
		jmp     L2

		align   16
	L2:
		mov     al, byte ptr [edx]
		dec     edx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		je      L2
		cmp     al, '\r'
		je      L2
		cmp     al, '\n'
		je      L2
		sub     edx, ecx
		pop     eax
		add     edx, 3
		dec     ecx
		push    edx
		push    eax
		mov     edx, ecx
		mov     ecx, dword ptr [Result + 4]
		jmp     string_ctor_assign_cstr_with_length

		align   16
	L3:
		mov     ecx, dword ptr [Result]
		jmp     string_ctor

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

		begin = string_c_str(Result);
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
					str = (string *)it->_M_value_field;
					if (memcmp(begin, string_c_str(str), length = string_length(str)) == 0)
					{
						begin += length;
						goto NESTED_CONTINUE1;
					}
				} while (it != set_begin(TrimSet));
				break;
			NESTED_CONTINUE1:;
			} while (begin < string_end(Result));
		}
		if ((Option & etTRIM_R) && string_end(Result) > begin)
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
				str = (string *)it->_M_value_field;
				if (string_length(str) > 1 || __intrinsic_istrailbyte(*string_begin(str)))
				{
					reverseScan = FALSE;
					break;
				}
			} while (it != set_begin(TrimSet));
			end = NULL;
			if (reverseScan)
			{
				LPCSTR p;

				p = string_end(Result);
				do
				{
					char ch;

					ch = *--p;
					it = set_end(TrimSet);
					do
					{
						string *str;

						set_iterator_decrement(it);
						str = (string *)it->_M_value_field;
						if (*string_begin(str) == ch)
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
						str = (string *)it->_M_value_field;
						if (memcmp(p, string_c_str(str), length = string_length(str)) == 0)
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
				} while (p < string_end(Result));
			}
			if (end != NULL)
			{
				size_t length;

				length = end - begin;
				string_end(Result) = string_begin(Result) + length;
				if (begin != string_c_str(Result))
					memcpy(string_begin(Result), begin, length);
				*string_end(Result) = '\0';
				return Result;
			}
		}
		if (begin != string_c_str(Result))
		{
			size_t length;

			length = string_end(Result) - begin;
			string_end(Result) = string_begin(Result) + length;
			memcpy(string_begin(Result), begin, length + 1);
		}
	}
	return Result;
}

#ifndef _M_IX86
char * __msfastcall TrimLeftSpace(const char *first)
{
	char c;

	while ((c = *(first++)) == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1);
	return (char *)(first - 1);
}
#else
__declspec(naked) char * __msfastcall TrimLeftSpace(const char *first)
{
	__asm
	{
		#define first ecx

	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L1
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L1
		lea     eax, [ecx - 1]
		ret

		#undef first
	}
}
#endif

#ifndef _M_IX86
char * __msfastcall TrimRightSpace(const char *first, const char *last)
{
	char c;

	while (first < last)
		if ((c = *(--last)) != ' ' && (unsigned char)(c - '\t') >= '\r' - '\t' + 1)
			return (char *)last + 1;
	return (char *)last;
}
#else
__declspec(naked) char * __msfastcall TrimRightSpace(const char *first, const char *last)
{
	__asm
	{
		#define first ecx
		#define last  edx

	L1:
		cmp     ecx, edx
		jae     L2
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L1
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L1
		lea     eax, [edx + 1]
		ret

		align   16
	L2:
		mov     eax, edx
		ret

		#undef first
		#undef last
	}
}
#endif

#ifndef _M_IX86
unsigned __int64 __msreturn __msfastcall __reg64return_TrimSpace(const char *first, const char *last)
{
	if (first < last)
	{
		char c;

		while ((c = *(first++)) == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1);
		if (last >= first)
		{
			while ((c = *(--last)) == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1);
			return (unsigned __int32)--first | ((unsigned __int64)++last << 32);
		}
	}
	return (unsigned __int32)last | ((unsigned __int64)last << 32);
}
#else
__declspec(naked) unsigned __int64 __msreturn __msfastcall __reg64return_TrimSpace(const char *first, const char *last)
{
	__asm
	{
		#define first ecx
		#define last  edx

		cmp     ecx, edx
		jb      L2
	L1:
		mov     eax, edx
		ret

		align   16
	L2:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L2
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L2
		cmp     edx, ecx
		jb      L1
		dec     edx
		jmp     L3

		align   16
	L3:
		mov     al, byte ptr [edx]
		dec     edx
		cmp     al, ' '
		je      L3
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L3
		add     edx, 2
		lea     eax, [ecx - 1]
		ret

		#undef first
		#undef last
	}
}
#endif

#ifndef _M_IX86
char * __msfastcall TrimLeftBlank(const char *first)
{
	char c;

	while ((c = *(first++)) == ' ' || c == '\t');
	return (char *)(first - 1);
}
#else
__declspec(naked) char * __msfastcall TrimLeftBlank(const char *first)
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
char * __msfastcall TrimRightBlank(const char *first, const char *last)
{
	char c;

	while (first < last)
		if ((c = *(--last)) != ' ' && c != '\t')
			return (char *)last + 1;
	return (char *)last;
}
#else
__declspec(naked) char * __msfastcall TrimRightBlank(const char *first, const char *last)
{
	__asm
	{
		#define first ecx
		#define last  edx

	L1:
		cmp     ecx, edx
		jae     L2
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
		lea     eax, [edx + 1]
		ret

		align   16
	L2:
		mov     eax, edx
		ret

		#undef first
		#undef last
	}
}
#endif

#ifndef _M_IX86
unsigned __int64 __msreturn __msfastcall __reg64return_TrimBlank(const char *first, const char *last)
{
	if (first < last)
	{
		char c;

		while ((c = *(first++)) == ' ' || c == '\t');
		if (last >= first)
		{
			while ((c = *(--last)) == ' ' || c == '\t');
			return (unsigned __int32)--first | ((unsigned __int64)++last << 32);
		}
	}
	return (unsigned __int32)last | ((unsigned __int64)last << 32);
}
#else
__declspec(naked) unsigned __int64 __msreturn __msfastcall __reg64return_TrimBlank(const char *first, const char *last)
{
	__asm
	{
		#define first ecx
		#define last  edx

		cmp     ecx, edx
		jb      L2
	L1:
		mov     eax, edx
		ret

		align   16
	L2:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		je      L2
		cmp     edx, ecx
		jb      L1
		dec     edx

		align   16
	L3:
		mov     al, byte ptr [edx]
		dec     edx
		cmp     al, ' '
		je      L3
		cmp     al, '\t'
		je      L3
		add     edx, 2
		lea     eax, [ecx - 1]
		ret

		#undef first
		#undef last
	}
}
#endif
