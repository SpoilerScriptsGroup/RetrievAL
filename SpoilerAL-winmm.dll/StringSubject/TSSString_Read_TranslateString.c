#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSString.h"
#include "TranscodeMultiByte.h"

static void __stdcall TSSString_Read_TranslateString(TSSString *this, unsigned long size, string *Data, char *tmpC);

void __declspec(naked) Caller_TSSString_Read_TranslateString()
{
	__asm
	{
		#define NextCallAddress 005D4494H
		#define this            ebx
		#define _size           (ebx + 78H)
		#define Data            (ebp - 1CH)
		#define tmpC            edi

		mov     eax, dword ptr [_size]
		lea     ecx, [Data]
		push    tmpC
		push    ecx
		push    eax
		push    this
		push    NextCallAddress
		jmp     TSSString_Read_TranslateString

		#undef NextCallAddress
		#undef this
		#undef _size
		#undef Data
		#undef tmpC
	}
}

static void __stdcall TSSString_Read_TranslateString(TSSString *this, unsigned long size, string *Data, char *tmpC)
{
	if (this->codePage == TSSSTRING_CP_UNICODE)
	{
		string_resize(Data, size);
		int cchMultiByte =
			WideCharToMultiByte(
				CP_THREAD_ACP,
				0,
				(LPCWSTR)tmpC,
				-1,
				Data->_M_start,
				size + 1,
				NULL,
				NULL);
		if (cchMultiByte != 0)
			cchMultiByte--;
		*(Data->_M_finish = Data->_M_start + cchMultiByte) = '\0';
	}
	else if (this->codePage == TSSSTRING_CP_UTF8)
	{
		string_resize(Data, size);
		int cchMultiByte =
			Utf8ToMultiByte(
				CP_THREAD_ACP,
				0,
				tmpC,
				-1,
				Data->_M_start,
				size + 1,
				NULL,
				NULL);
		if (cchMultiByte != 0)
			cchMultiByte--;
		*(Data->_M_finish = Data->_M_start + cchMultiByte) = '\0';
	}
}
