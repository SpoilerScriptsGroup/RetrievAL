#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSString.h"

static void __fastcall TSSString_Read_terminate_Data(TSSString *this, string *Data, unsigned long Pos)
{
	if (!this->codePage)
		*(string_end(Data) = string_begin(Data) + Pos) = '\0';
}

void __declspec(naked) Caller_TSSString_Read_terminate_Data()
{
	__asm
	{
		#define this  ebx
		#define Data (ebp - 0x1C)

		lea     edx, [Data]
		mov     ecx,  this
		jmp     TSSString_Read_terminate_Data
		
		#undef Data
		#undef this
	}
}
