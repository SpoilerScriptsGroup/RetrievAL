#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSCalc.h"

static void __stdcall Setting_CheckSignedSize(TSSCalc *_this, string *s1, string *s2);

__declspec(naked) void __cdecl TSSCalc_Setting_CheckSignedSize()
{
	__asm
	{
		#define sizeof_string 24
		#define ReturnAddress 004C18D9H
		#define _this         ebx
		#define tmpV          edi

		mov     eax, dword ptr [tmpV]
		add     eax, sizeof_string * 2
		push    eax
		sub     eax, sizeof_string * (2 - 1)
		push    eax
		push    _this
		push    ReturnAddress
		jmp     Setting_CheckSignedSize

		#undef sizeof_string
		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleCalc_Setting_CheckSignedSize()
{
	__asm
	{
		#define sizeof_string 24
		#define ReturnAddress 004BD14BH
		#define _this         ebx
		#define tmpV          edi

		mov     eax, dword ptr [tmpV]
		add     eax, sizeof_string * 4
		push    eax
		sub     eax, sizeof_string * (4 - 3)
		push    eax
		push    _this
		push    ReturnAddress
		jmp     Setting_CheckSignedSize

		#undef sizeof_string
		#undef ReturnAddress
		#undef _this
		#undef tmpV
	}
}

static void __stdcall Setting_CheckSignedSize(TSSCalc *_this, string *s1, string *s2)
{
	if (_this->max <= SHRT_MAX && _this->min >= SHRT_MIN &&
		(string_length(s1) != 3 || (*(LPDWORD)s1->_M_start != BSWAP32('min\0') && *(LPDWORD)s1->_M_start != BSWAP32('max\0'))) &&
		(string_length(s2) != 3 || (*(LPDWORD)s2->_M_start != BSWAP32('max\0') && *(LPDWORD)s2->_M_start != BSWAP32('min\0'))))
	{
		_this->size = (_this->max <= CHAR_MAX && _this->min >= CHAR_MIN) ? 1 : 2;
	}
	else
	{
		_this->size = 4;
	}
}
