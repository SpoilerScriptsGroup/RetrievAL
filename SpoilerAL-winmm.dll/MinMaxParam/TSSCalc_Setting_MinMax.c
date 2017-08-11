#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSCalc.h"

static void __stdcall Setting_MinMax(TSSCalc *_this, string *s1, string *s2);

__declspec(naked) void __cdecl TSSCalc_Setting_MinMax()
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
		pop     ecx
		ret

		#undef sizeof_string
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleCalc_Setting_MinMax()
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

static void __stdcall Setting_MinMax(TSSCalc *_this, string *s1, string *s2)
{
	if (_this->size > 4)
		_this->size = 4;
	if (string_length(s1) == 3)
		if (*(LPDWORD)s1->_M_start == BSWAP32('min\0'))
			_this->min = _this->isUnsigned ? 0 : (long)LONG_MIN >> ((4 - _this->size) * 8);
		else if (*(LPDWORD)s1->_M_start == BSWAP32('max\0'))
			_this->min = (_this->isUnsigned ? ULONG_MAX : LONG_MAX) >> ((4 - _this->size) * 8);
	if (string_length(s2) == 3)
		if (*(LPDWORD)s2->_M_start == BSWAP32('min\0'))
			_this->max = _this->isUnsigned ? 0 : (long)LONG_MIN >> ((4 - _this->size) * 8);
		else if (*(LPDWORD)s2->_M_start == BSWAP32('max\0'))
			_this->max = (_this->isUnsigned ? ULONG_MAX : LONG_MAX) >> ((4 - _this->size) * 8);
}
