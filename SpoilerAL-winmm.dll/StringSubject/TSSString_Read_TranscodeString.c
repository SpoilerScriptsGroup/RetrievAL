#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSString.h"
#include "TranscodeMultiByte.h"

static void __cdecl TSSString_Read_TranscodeString(string* Data, const char* tmpC, TSSString* SSGS)
{
	int cchMultiByte;
	string_ctor_null(Data);
	switch (SSGS->codePage)
	{
		BOOL warn;
	case CP_ACP:
		string_assign_cstr(Data, tmpC);
		return;
	case MAXWORD:
		string_reserve(Data, SSGS->size);
		cchMultiByte = WideCharToMultiByte(
			CP_THREAD_ACP,
			WC_NO_BEST_FIT_CHARS,
			(LPCWCH)tmpC,
			-1,
			string_begin(Data),
			string_storage_capacity(Data),
			"\a",
			&warn);
		break;
	default:
		string_reserve(Data, SSGS->size);
		cchMultiByte = TranscodeMultiByte(
			SSGS->codePage,
			0,
			tmpC,
			-1,
			CP_THREAD_ACP,
			WC_NO_BEST_FIT_CHARS,
			string_begin(Data),
			string_storage_capacity(Data),
			"\a",
			&warn);
	}
	if (cchMultiByte)
		cchMultiByte--;
	*(string_end(Data) = string_begin(Data) + cchMultiByte) = '\0';
}

void __declspec(naked) Caller_TSSString_Read_TranscodeString(string* Data, const char* tmpC, LPCVOID __a)
{
	__asm
	{
		#define this            ebx

		mov     dword ptr [esp + 12], this
		jmp     TSSString_Read_TranscodeString

		#undef  this
	}
}
