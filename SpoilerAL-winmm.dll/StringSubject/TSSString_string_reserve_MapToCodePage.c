#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSString.h"
#include "TranscodeMultiByte.h"

static void __fastcall TSSString_string_reserve_MapToCodePage(TSSString *this, string *Val, string *s, size_t n);

void __declspec(naked) Caller_TSSString_Write_WriteString_reserve()
{
	__asm
	{
		#define ReturnAddress 0052B360H
		#define this          (ebp + 8)
		#define Str           (ebp - 1CH)

		mov     dword ptr [esp], ReturnAddress
		lea     edx, [Str]
		mov     ecx, dword ptr [this]
		jmp     TSSString_string_reserve_MapToCodePage

		#undef ReturnAddress
		#undef this
		#undef Str
	}
}

void __declspec(naked) Caller_TSSString_ToByteCode_tmpS_reserve()
{
	__asm
	{
		#define ReturnAddress 0052B805H
		#define this          ecx
		#define Val           (ebp - 1CH)

		mov     dword ptr [esp], eax
		push    edx
		push    ReturnAddress
		lea     edx, [Val]
		jmp     TSSString_string_reserve_MapToCodePage

		#undef ReturnAddress
		#undef this
		#undef Val
	}
}

static void __fastcall TSSString_string_reserve_MapToCodePage(TSSString *this, string *Val, string *s, size_t n)
{
	string_reserve(s, n);
	if (this->codePage == TSSSTRING_CP_UNICODE)
	{
		size_t unicodeSize = MultiByteToWideChar(
			CP_THREAD_ACP,
			0,
			string_c_str(Val),
			string_length(Val),
			(LPWSTR)string_begin(s),
			n / 2) * 2;
		string_assign_cstr_with_length(Val, string_begin(s), min(unicodeSize, this->size));
		string_clear(s);
	}
	else if (this->codePage == TSSSTRING_CP_UTF8)
	{
		size_t utf8Size = MultiByteToUtf8(
			CP_THREAD_ACP,
			0,
			string_c_str(Val),
			string_length(Val),
			string_begin(s),
			n);
		string_assign_cstr_with_length(Val, string_begin(s), min(utf8Size, this->size));
		string_clear(s);
	}
}
