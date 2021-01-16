#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSString.h"

void __declspec(naked) Caller_TSSString_Read_terminate_Data()
{
	static void __fastcall TSSString_Read_terminate_Data();

	__asm
	{
		#define Pos   edx

		cmp     Pos, -1
#pragma warning(suppress: 4414)
		jne     TSSString_Read_terminate_Data
		ret     4
		
		#undef Pos
	}
}

static void __fastcall TSSString_Read_terminate_Data(string *Data, unsigned long Pos, TSSString *this)
{
	*(string_end(Data) = string_begin(Data) + Pos) = '\0';
}
