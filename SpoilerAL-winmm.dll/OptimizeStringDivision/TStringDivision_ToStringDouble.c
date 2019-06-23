#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <windows.h>
#include <stdio.h>
#include <float.h>
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

#ifndef _M_IX86
string * __cdecl TStringDivision_ToStringDouble(
	OUT    string     *Result,
	IN     double     Src,
	IN     const char *Format)
{
	char         buffer[512];
	unsigned int length;

	length = _snprintf(buffer, _countof(buffer), Format && !_isnan(Src) ? Format : "%f", Src);
	if ((int)length < 0)
		length = strlen(buffer);
	else if (length > _countof(buffer) - 1)
		length = _countof(buffer) - 1;
	return string_ctor_assign_cstr_with_length(Result, buffer, length);
}
#else
#pragma function(strlen)
__declspec(naked) string * __cdecl TStringDivision_ToStringDouble(
	OUT    string     *Result,
	IN     double     Src,
	IN     const char *Format)
{
	static char field_f[] = "%f";

	__asm
	{
		#define Result (esp +  4)
		#define Src    (esp +  8)
		#define Format (esp + 16)

		mov     eax, dword ptr [Src]
		mov     ecx, dword ptr [Src + 4]
		mov     edx, dword ptr [Format]
		sub     esp, 512
		push    ecx
		push    eax
		test    edx, edx
		jz      L1
		and     ecx, 0x7FFFFFFF
		sub     eax, 1
		sbb     ecx, 0x7FF00000
		jb      L2
	L1:
		mov     edx, offset field_f
	L2:
		lea     eax, [esp + (4 * 2)]
		push    edx
		push    512
		push    eax
		call    _snprintf
		test    eax, eax
		jns     L3
		lea     eax, [esp + 4 * 5]
		push    eax
		call    strlen
		pop     ecx
		jmp     L4
	L3:
		cmp     eax, 512 - 1
		mov     ecx, 512 - 1
		cmova   eax, ecx
	L4:
		mov     ecx, dword ptr [Result + (4 * 5 + 512)]
		add     esp, 4 * 5
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
