#include <windows.h>
#include <float.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSFloatCalc.h"

static void __stdcall Setting_MinMax(TSSFloatCalc *this, string *s1, string *s2);

__declspec(naked) void __cdecl TSSFloatCalc_Setting_MinMax()
{
	__asm
	{
		#define this ebx
		#define tmpV edi

		mov     eax, dword ptr [tmpV]
		add     eax, size string * 4
		push    eax
		sub     eax, size string * (4 - 2)
		push    eax
		sub     eax, size string * (2 - 1)
		push    eax
		push    this
		call    Setting_MinMax
		pop     edx
		ret

		#undef this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleFloatCalc_Setting_MinMax()
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

static void __stdcall Setting_MinMax(TSSFloatCalc *this, string *s1, string *s2)
{
	if (string_length(s1) == 3)
		if (*(LPDWORD)string_begin(s1) == BSWAP32('min\0'))
			this->min = this->size == 8 ? -DBL_MAX : -FLT_MAX;
		else if (*(LPDWORD)string_begin(s1) == BSWAP32('max\0'))
			this->min = this->size == 8 ? DBL_MAX : FLT_MAX;
	if (string_length(s2) == 3)
		if (*(LPDWORD)string_begin(s2) == BSWAP32('min\0'))
			this->max = this->size == 8 ? -DBL_MAX : -FLT_MAX;
		else if (*(LPDWORD)string_begin(s2) == BSWAP32('max\0'))
			this->max = this->size == 8 ? DBL_MAX : FLT_MAX;
}
