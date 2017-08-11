#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSString.h"

static void __fastcall TSSString_Read_terminate_Data(TSSString* _this, string *Data, unsigned long Pos);

void __declspec(naked) Caller_TSSString_Read_terminate_Data()
{
	__asm
	{
		#define _this ebx
		#define Data  (ebp - 1CH)

		lea     edx, [Data]
		mov     ecx, _this
		jmp     TSSString_Read_terminate_Data

		#undef _this
		#undef Data
	}
}

static void __fastcall TSSString_Read_terminate_Data(TSSString* _this, string *Data, unsigned long Pos)
{
	if (!_this->isUnicode)
		*(Data->_M_finish = Data->_M_start + Pos) = '\0';
}
