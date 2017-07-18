#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "TSSString.h"

static void __stdcall TSSString_Read_UnicodeString(TSSString *_this, unsigned long size, bcb6_std_string *Data, char *tmpC);

void __declspec(naked) Caller_TSSString_Read_UnicodeString()
{
	__asm
	{
		#define NextCallAddress 005D4494H
		#define _this           ebx
		#define _size           (ebx + 78H)
		#define Data            (ebp - 1CH)
		#define tmpC            edi

		mov     eax, dword ptr [_size]
		lea     ecx, [Data]
		push    tmpC
		push    ecx
		push    eax
		push    _this
		push    NextCallAddress
		jmp     TSSString_Read_UnicodeString

		#undef NextCallAddress
		#undef _this
		#undef _size
		#undef Data
		#undef tmpC
	}
}

static void __stdcall TSSString_Read_UnicodeString(TSSString *_this, unsigned long size, bcb6_std_string *Data, char *tmpC)
{
	if (_this->isUnicode)
	{
		bcb6_std_string_reserve(Data, size);
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
}
