#include <windows.h>
#include <stdlib.h>
#include <assert.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

#ifndef _M_IX86
string * __cdecl TStringDivision_Editing(
	IN  string          *Result,
	IN  TStringDivision *this,
	IN  const string    *Src,
	IN  unsigned long   Option)
{
	if (Option & etTRIM)
	{
		assert((Option & etTRIM) == etTRIM);

		if (Option & etREPLACE)
		{
#if 0
			string s;

			TStringDivision_RemoveByMap(Result, this, TStringDivision_TrimDefault(&s, NULL, Src, NULL, 0), NULL, 0);
			string_dtor(&s);
			return Result;
#else
			#define SHRINK_SIZEOF_STRING (                                     \
			    (&string_end((string *)NULL) > &string_begin((string *)NULL) ? \
			        (size_t)&string_end((string *)NULL) :                      \
			        (size_t)&string_begin((string *)NULL)) +                   \
			    sizeof(void *))

			BYTE  s[SHRINK_SIZEOF_STRING];
			LPSTR first, last;
			char  c;

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
			string_begin((string *)s) = first;
			string_end((string *)s) = last;
			return TStringDivision_RemoveByMap(Result, this, (string *)s, NULL, 0);

			#undef SHRINK_SIZEOF_STRING
#endif
		}
		else
		{
			return TStringDivision_TrimDefault(Result, NULL, Src, NULL, 0);
		}
	}
	else if (Option & etREPLACE)
	{
		return TStringDivision_RemoveByMap(Result, this, Src, NULL, 0);
	}
	else
	{
		return string_ctor_assign(Result, Src);
	}
}
#else
__declspec(naked) string * __cdecl TStringDivision_Editing(
	IN  string          *Result,
	IN  TStringDivision *this,
	IN  const string    *Src,
	IN  unsigned long   Option)
{
	__asm
	{
		#define Result   (esp +  4)
		#define this     (esp +  8)
		#define Src      (esp + 12)
		#define Option   (esp + 16)

		mov     eax, dword ptr [Option]
		mov     edx, dword ptr [Src]
		test    eax, etTRIM
		jz      L5
		test    eax, etREPLACE
		jz      L4
		mov     ecx, dword ptr [edx]
		mov     edx, dword ptr [edx + 4]
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L1
		cmp     al, '\t'
		je      L1
	L2:
		cmp     edx, ecx
		jb      L3
		mov     al, byte ptr [edx - 1]
		dec     edx
		cmp     al, ' '
		je      L2
		cmp     al, '\t'
		je      L2
		inc     edx
	L3:
		dec     ecx
		push    edx
		push    ecx
		mov     ecx, dword ptr [this + 8]
		mov     eax, dword ptr [Result + 8]
		mov     edx, esp
		push    0
		push    0
		push    edx
		push    ecx
		push    eax
		call    TStringDivision_RemoveByMap
		add     esp, 28
		ret
	L4:
		mov     eax, dword ptr [Result]
		push    0
		push    0
		push    edx
		push    0
		push    eax
		call    TStringDivision_TrimDefault
		add     esp, 20
		ret
	L5:
		test    eax, etREPLACE
		jz      L6
		mov     ecx, dword ptr [this]
		mov     eax, dword ptr [Result]
		push    0
		push    0
		push    edx
		push    ecx
		push    eax
		call    TStringDivision_RemoveByMap
		add     esp, 20
		ret
	L6:
		mov     ecx, dword ptr [Result]
		jmp     string_ctor_assign

		#undef Result
		#undef this
		#undef Src
		#undef Option
	}
}
#endif
