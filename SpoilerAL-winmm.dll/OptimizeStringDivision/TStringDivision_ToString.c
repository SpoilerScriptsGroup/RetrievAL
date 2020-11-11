#define _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include <windows.h>
#include <stdio.h>
#define USING_NAMESPACE_BCB6_STD
#include "TStringDivision.h"

EXTERN_C size_t __fastcall _ui32to10a(unsigned __int32 value, char *buffer);

#ifndef _M_IX86
string * __cdecl TStringDivision_ToString(
	OUT    string        *Result,
	IN     unsigned long Src,
	IN     const char    *Format)
{
	char   buffer[512];
	size_t length;

	if (!Format)
		length = _ui32to10a(Src, buffer);
	else if ((length = _snprintf(buffer, _countof(buffer), Format, Src)) >= _countof(buffer))
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
	__asm
	{
		#define Result (esp +  4)
		#define Src    (esp +  8)
		#define Format (esp + 12)

		mov     eax, dword ptr [Format]
		sub     esp, 512
		mov     ecx, dword ptr [Src + 512]
		mov     edx, esp
		test    eax, eax
		jnz     L1
		call    _ui32to10a
		mov     ecx, dword ptr [Result + 512]
		jmp     L3

		align   16
	L1:
		push    ecx
		push    eax
		push    512
		push    edx
		call    _snprintf
		cmp     eax, 512
		jb      L2
		test    eax, eax
		lea     ecx, [esp + 4 * 4]
		mov     eax, 512 - 1
		jns     L2
		mov     dword ptr [esp], ecx
		call    strlen
	L2:
		mov     ecx, dword ptr [Result + (4 * 4 + 512)]
		add     esp, 4 * 4
	L3:
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
