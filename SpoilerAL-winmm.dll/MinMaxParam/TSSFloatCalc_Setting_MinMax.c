#include <windows.h>
#include <float.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "TSSFloatCalc.h"

static void __stdcall Setting_MinMax(TSSFloatCalc *_this, bcb6_std_string *s1, bcb6_std_string *s2);

__declspec(naked) void __cdecl TSSFloatCalc_Setting_MinMax()
{
	__asm
	{
		#define sizeof_string 24
		#define _this         ebx
		#define tmpV          edi

		mov     eax, dword ptr [tmpV]
		add     eax, sizeof_string * 4
		push    eax
		sub     eax, sizeof_string * (4 - 2)
		push    eax
		sub     eax, sizeof_string * (2 - 1)
		push    eax
		push    _this
		call    Setting_MinMax
		pop     edx
		ret

		#undef sizeof_string
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleFloatCalc_Setting_MinMax()
{
	__asm
	{
		#define sizeof_string 24
		#define _this         ebx

		add     eax, sizeof_string * 6
		push    eax
		sub     eax, sizeof_string * (6 - 4)
		push    eax
		sub     eax, sizeof_string * (4 - 3)
		push    eax
		push    _this
		call    Setting_MinMax
		pop     eax
		ret

		#undef sizeof_string
		#undef _this
	}
}

static void __stdcall Setting_MinMax(TSSFloatCalc *_this, bcb6_std_string *s1, bcb6_std_string *s2)
{
	if (bcb6_std_string_length(s1) == 3)
		if (*(LPDWORD)s1->_M_start == BSWAP32('min\0'))
			_this->min = _this->size == 8 ? DBL_MIN : FLT_MIN;
		else if (*(LPDWORD)s1->_M_start == BSWAP32('max\0'))
			_this->min = _this->size == 8 ? DBL_MAX : FLT_MAX;
	if (bcb6_std_string_length(s2) == 3)
		if (*(LPDWORD)s2->_M_start == BSWAP32('min\0'))
			_this->max = _this->size == 8 ? DBL_MIN : FLT_MIN;
		else if (*(LPDWORD)s2->_M_start == BSWAP32('max\0'))
			_this->max = _this->size == 8 ? DBL_MAX : FLT_MAX;
}
