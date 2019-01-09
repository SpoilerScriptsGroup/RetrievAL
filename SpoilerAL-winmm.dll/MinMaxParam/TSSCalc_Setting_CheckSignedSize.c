#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSCalc.h"

static void __stdcall Setting_CheckSignedSize(TSSCalc *this, string *s1, string *s2);

__declspec(naked) void __cdecl TSSCalc_Setting_CheckSignedSize()
{
	__asm
	{
		#define sizeof_string 24
		#define ReturnAddress 004C18D9H
		#define this          ebx
		#define tmpV          edi

		mov     eax, dword ptr [tmpV]
		add     eax, sizeof_string * 2
		push    eax
		sub     eax, sizeof_string * (2 - 1)
		push    eax
		push    this
		push    ReturnAddress
		jmp     Setting_CheckSignedSize

		#undef sizeof_string
		#undef ReturnAddress
		#undef this
		#undef tmpV
	}
}

__declspec(naked) void __cdecl TSSBundleCalc_Setting_CheckSignedSize()
{
	__asm
	{
		#define sizeof_string 24
		#define ReturnAddress 004BD14BH
		#define this          ebx
		#define tmpV          edi

		mov     eax, dword ptr [tmpV]
		add     eax, sizeof_string * 4
		push    eax
		sub     eax, sizeof_string * (4 - 3)
		push    eax
		push    this
		push    ReturnAddress
		jmp     Setting_CheckSignedSize

		#undef sizeof_string
		#undef ReturnAddress
		#undef this
		#undef tmpV
	}
}

static void __stdcall Setting_CheckSignedSize(TSSCalc *this, string *s1, string *s2)
{
	if (this->max <= SHRT_MAX && this->min >= SHRT_MIN &&
		(string_length(s1) != 3 || (*(LPDWORD)string_begin(s1) != BSWAP32('min\0') && *(LPDWORD)string_begin(s1) != BSWAP32('max\0'))) &&
		(string_length(s2) != 3 || (*(LPDWORD)string_begin(s2) != BSWAP32('max\0') && *(LPDWORD)string_begin(s2) != BSWAP32('min\0'))))
	{
		this->size = (this->max <= CHAR_MAX && this->min >= CHAR_MIN) ? 1 : 2;
	}
	else
	{
		this->size = 4;
	}
}
