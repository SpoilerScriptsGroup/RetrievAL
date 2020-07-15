#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSCalc.h"

static void __stdcall Setting_MinMax(TSSCalc *this, string *s1, string *s2);

__declspec(naked) void __cdecl TSSCalc_Setting_MinMax()
{
	__asm
	{
		#define this          ebx
		#define tmpV          edi

		mov     eax, dword ptr [tmpV]
		add     eax, size string * 4
		push    eax
		sub     eax, size string * (4 - 2)
		push    eax
		sub     eax, size string * (2 - 1)
		push    eax
		push    this
		call    Setting_MinMax
		pop     ecx
		ret

		#undef this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleCalc_Setting_MinMax()
{
	__asm
	{
		#define this ebx

		add     eax, size string * 6
		push    eax
		sub     eax, size string * (6 - 4)
		push    eax
		sub     eax, size string * (4 - 3)
		push    eax
		push    this
		call    Setting_MinMax
		pop     eax
		ret

		#undef this
	}
}

static void __stdcall Setting_MinMax(TSSCalc *this, string *s1, string *s2)
{
	if (this->size > 4)
		this->size = 4;
	if (string_length(s1) == 3)
		if (*(LPDWORD)string_begin(s1) == BSWAP32('min\0'))
			this->min = this->isUnsigned ? 0 : (long)LONG_MIN >> ((4 - this->size) * 8);
		else if (*(LPDWORD)string_begin(s1) == BSWAP32('max\0'))
			this->min = (this->isUnsigned ? ULONG_MAX : LONG_MAX) >> ((4 - this->size) * 8);
	if (string_length(s2) == 3)
		if (*(LPDWORD)string_begin(s2) == BSWAP32('min\0'))
			this->max = this->isUnsigned ? 0 : (long)LONG_MIN >> ((4 - this->size) * 8);
		else if (*(LPDWORD)string_begin(s2) == BSWAP32('max\0'))
			this->max = (this->isUnsigned ? ULONG_MAX : LONG_MAX) >> ((4 - this->size) * 8);
}
