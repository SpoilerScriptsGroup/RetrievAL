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
	if (first >= last)
	{
		first = last;
	}
	else
	{
		char c;

		while ((c = *(first++)) == ' ' || c == '\t')
			if (first == last)
				goto TRIMED;
		while ((c = *(--last)) == ' ' || c == '\t');
		--first;
		++last;
	}
TRIMED:
	return string_ctor_assign_cstr_with_length(Result, first, last - first);
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
		mov     ecx, edx
		jmp     L4

		align   16
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		jne     L3
	L2:
		cmp     ecx, edx
		jne     L1
		jmp     L4

		align   16
	L3:
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L3
		cmp     al, '\t'
		je      L3
		inc     edx
		dec     ecx
	L4:
		pop     eax
		sub     edx, ecx
		push    edx
		push    eax
		mov     edx, ecx
		mov     ecx, dword ptr [Result + 4]
		jmp     string_ctor_assign_cstr_with_length

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
	if (first >= last)
	{
		first = last;
	}
	else
	{
		char c;

		while ((c = *(first++)) == ' ' || c == '\t' || c == '\r' || c == '\n')
			if (first == last)
				goto TRIMED;
		while ((c = *(--last)) == ' ' || c == '\t' || c == '\r' || c == '\n');
		--first;
		++last;
	}
TRIMED:
	return string_ctor_assign_cstr_with_length(Result, first, last - first);
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
		mov     ecx, edx
		jmp     L4

		align   16
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		je      L2
		cmp     al, '\r'
		je      L2
		cmp     al, '\n'
		jne     L3
	L2:
		cmp     ecx, edx
		jne     L1
		jmp     L4

		align   16
	L3:
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L3
		cmp     al, '\t'
		je      L3
		cmp     al, '\r'
		je      L3
		cmp     al, '\n'
		je      L3
		inc     edx
		dec     ecx
	L4:
		pop     eax
		sub     edx, ecx
		push    edx
		push    eax
		mov     edx, ecx
		mov     ecx, dword ptr [Result + 4]
		jmp     string_ctor_assign_cstr_with_length

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
	unsigned char c;

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
	if (first >= last)
	{
		first = last;
	}
	else
	{
		unsigned char c;

		do
			if ((c = *(--last)) != ' ' && (unsigned char)(c - '\t') >= '\r' - '\t' + 1)
				return (char *)last + 1;
		while (last != first);
	}
	return (char *)last;
}
#else
__declspec(naked) char * __msfastcall TrimRightSpace(const char *first, const char *last)
{
	__asm
	{
		#define first ecx
		#define last  edx

		cmp     ecx, edx
		jae     L3

		align   16
	L1:
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L2
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jae     L4
	L2:
		cmp     ecx, edx
		jne     L1
	L3:
		mov     eax, edx
		ret

		align   16
	L4:
		lea     eax, [edx + 1]
		ret

		#undef first
		#undef last
	}
}
#endif

#ifndef _M_IX86
unsigned __int64 __msreturn __msfastcall __ui64return_TrimSpace(const char *first, const char *last)
{
	if (first >= last)
	{
		first = last;
	}
	else
	{
		unsigned char c;

		while ((c = *(first++)) == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1)
			if (first == last)
				goto TRIMED;
		while ((c = *(--last)) == ' ' || (unsigned char)(c - '\t') < '\r' - '\t' + 1);
		--first;
		++last;
	}
TRIMED:
	return (unsigned __int32)last | ((unsigned __int64)first << 32);
}
#else
__declspec(naked) unsigned __int64 __msreturn __msfastcall __ui64return_TrimSpace(const char *first, const char *last)
{
	__asm
	{
		#define first ecx
		#define last  edx

		cmp     ecx, edx
		jb      L1
		mov     eax, edx
		jmp     L5

		align   16
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L2
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jae     L3
	L2:
		cmp     ecx, edx
		jne     L1
		jmp     L4

		align   16
	L3:
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L3
		sub     al, '\t'
		cmp     al, '\r' - '\t' + 1
		jb      L3
		inc     edx
		dec     ecx
	L4:
		mov     eax, edx
		mov     edx, ecx
	L5:
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
	if (first >= last)
	{
		first = last;
	}
	else
	{
		char c;

		do
			if ((c = *(--last)) != ' ' && c != '\t')
				return (char *)last + 1;
		while (last != first);
	}
	return (char *)last;
}
#else
__declspec(naked) char * __msfastcall TrimRightBlank(const char *first, const char *last)
{
	__asm
	{
		#define first ecx
		#define last  edx

		cmp     ecx, edx
		jae     L3

		align   16
	L1:
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		jne     L4
	L2:
		cmp     ecx, edx
		jne     L1
	L3:
		mov     eax, edx
		ret

		align   16
	L4:
		lea     eax, [edx + 1]
		ret

		#undef first
		#undef last
	}
}
#endif

#ifndef _M_IX86
unsigned __int64 __msreturn __msfastcall __ui64return_TrimBlank(const char *first, const char *last)
{
	if (first >= last)
	{
		first = last;
	}
	else
	{
		char c;

		while ((c = *(first++)) == ' ' || c == '\t')
			if (first == last)
				goto TRIMED;
		while ((c = *(--last)) == ' ' || c == '\t');
		--first;
		++last;
	}
TRIMED:
	return (unsigned __int32)last | ((unsigned __int64)first << 32);
}
#else
__declspec(naked) unsigned __int64 __msreturn __msfastcall __ui64return_TrimBlank(const char *first, const char *last)
{
	__asm
	{
		#define first ecx
		#define last  edx

		cmp     ecx, edx
		jb      L1
		mov     eax, edx
		jmp     L5

		align   16
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		jne     L3
	L2:
		cmp     ecx, edx
		jne     L1
		jmp     L4

		align   16
	L3:
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L3
		cmp     al, '\t'
		je      L3
		inc     edx
		dec     ecx
	L4:
		mov     eax, edx
		mov     edx, ecx
	L5:
		ret

		#undef first
		#undef last
	}
}
#endif
