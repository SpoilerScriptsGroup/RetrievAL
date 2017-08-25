#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

__declspec(naked) void __cdecl TSSGCtrl_GetSSGDataFile_ClearAtBreak()
{
	__asm
	{
		#define tmpV (ebp - 30H)

		push    004EF423H
		lea     ecx, [tmpV]
		jmp     vector_string_clear

		#undef tmpV
	}
}
