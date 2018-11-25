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
	if ((int)length < 0)
		length = strlen(buffer);
	else if (length > _countof(buffer) - 1)
		length = _countof(buffer) - 1;
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
		test    eax, eax
		jns     L1
		lea     eax, [esp + 4 * 4]
		push    eax
		call    strlen
		pop     ecx
		jmp     L2
	L1:
		cmp     eax, 512 - 1
		mov     ecx, 512 - 1
		cmova   eax, ecx
	L2:
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
