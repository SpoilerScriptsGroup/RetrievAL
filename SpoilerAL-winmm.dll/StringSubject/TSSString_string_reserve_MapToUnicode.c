#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSString.h"

static void __fastcall TSSString_string_reserve_MapToUnicode(TSSString* _this, bcb6_std_string *Val, bcb6_std_string *s, size_t n);

void __declspec(naked) Caller_TSSString_Write_WriteString_reserve()
{
	__asm
	{
		#define ReturnAddress 0052B360H
		#define _this         (ebp + 8)
		#define Str           (ebp - 1CH)

		mov     dword ptr [esp], ReturnAddress
		lea     edx, [Str]
		mov     ecx, [_this]
		jmp     TSSString_string_reserve_MapToUnicode

		#undef ReturnAddress
		#undef _this
		#undef Str
	}
}

void __declspec(naked) Caller_TSSString_ToByteCode_tmpS_reserve()
{
	__asm
	{
		#define ReturnAddress 0052B805H
		#define _this         ecx
		#define Val           (ebp - 1CH)

		mov     dword ptr [esp], eax
		push    edx
		push    ReturnAddress
		lea     edx, [Val]
		jmp     TSSString_string_reserve_MapToUnicode

		#undef ReturnAddress
		#undef _this
		#undef Val
	}
}

static void __fastcall TSSString_string_reserve_MapToUnicode(TSSString* _this, bcb6_std_string *Val, bcb6_std_string *s, size_t n)
{
	bcb6_std_string_allocate(s, n);
	if (_this->isUnicode)
	{
		size_t unicodeSize = MultiByteToWideChar(
			CP_ACP,
			0,
			bcb6_std_string_c_str(Val),
			bcb6_std_string_length(Val),
			(LPWSTR)bcb6_std_string_begin(s),
			n / 2) * 2;
		size_t length = _this->size < unicodeSize ? _this->size : unicodeSize;
		bcb6_std_string_assign_cstr_with_length(Val, bcb6_std_string_begin(s), length);
		bcb6_std_string_clear(s);
	}
}
