#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <windows.h>
#include <stdio.h>
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

#ifndef _M_IX86
string * __cdecl TStringDivision_ToString(
	OUT    string        *Result,
	IN     unsigned long Src,
	IN     const char    *Format)
{
	char         buffer[512];
	unsigned int length;

	length = _snprintf(buffer, _countof(buffer), Format ? Format : "%u", Src);
	if (length >= _countof(buffer))
		length = (int)length >= 0 ? _countof(buffer) - 1 : strlen(buffer);
	return string_ctor_assign_cstr_with_length(Result, buffer, length);
}
#else
#pragma function(strlen)
__declspec(naked) string * __cdecl TStringDivision_ToString(
	OUT    string        *Result,
	IN     unsigned long Src,
	IN     const char    *Format)
{
	static char field_u[] = "%u";

	__asm
	{
		#define Result (esp +  4)
		#define Src    (esp +  8)
		#define Format (esp + 12)

		mov     eax, dword ptr [Format]
		mov     ecx, offset field_u
		sub     esp, 512
		test    eax, eax
		cmovz   eax, ecx
		mov     ecx, dword ptr [Src + 512]
		mov     edx, esp
		push    ecx
		push    eax
		push    512
		push    edx
		call    _snprintf
		cmp     eax, 512
		jb      L1
		test    eax, eax
		lea     ecx, [esp + 4 * 4]
		mov     eax, 512 - 1
		jns     L1
		mov     dword ptr [esp], ecx
		call    strlen
	L1:
		mov     ecx, dword ptr [Result + (4 * 4 + 512)]
		add     esp, 4 * 4
		mov     edx, esp
		push    eax
		call    string_ctor_assign_cstr_with_length
		add     esp, 512
		ret

		#undef Result
		#undef Src
		#undef Format
	}
}
#endif
