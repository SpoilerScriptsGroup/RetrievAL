#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSString.h"
#include "TranscodeMultiByte.h"

static void __fastcall TSSString_string_reserve_MapToCodePage(TSSString *SSGS, string *value, string *dest)
{
	size_t cch = string_length(value);
	string_ctor_null(dest);
	switch (SSGS->codePage)
	{
	case CP_ACP:
		string_assign_cstr_with_length(dest, string_c_str(value), cch);
		return;
	case MAXWORD:
		string_reserve(dest, cch * sizeof(wchar_t));
		cch = MultiByteToWideChar(
			CP_THREAD_ACP,
			0,
			string_c_str(value),
			cch,
			(LPWSTR)string_begin(dest),
			cch) * sizeof(wchar_t);
		break;
	default:
		string_reserve(dest, cch << 1);
		cch = TranscodeMultiByte(
			CP_THREAD_ACP,
			0,
			string_c_str(value),
			cch,
			SSGS->codePage,
			0,
			string_begin(dest),
			cch << 1,
			NULL,
			NULL);
	}
	*(string_end(dest) = string_begin(dest) + min(cch, SSGS->size)) = '\0';
}

void __declspec(naked) Caller_TSSString_Write_Str_ctor()
{
	__asm
	{// edx is value already
		#define this          (ebp + 8)

		mov     ecx, dword ptr [this]
		jmp     TSSString_string_reserve_MapToCodePage

		#undef this
	}
}

void __declspec(naked) Caller_TSSString_ToByteCode_Val_ctor()
{
	__asm
	{
		#define this          (ebp + 12)

		mov     edx, ebx
		mov     ecx, dword ptr [this]
		jmp     TSSString_string_reserve_MapToCodePage

		#undef this
	}
}
