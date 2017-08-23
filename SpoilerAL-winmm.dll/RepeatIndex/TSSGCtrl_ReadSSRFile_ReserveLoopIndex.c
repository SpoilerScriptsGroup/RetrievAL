#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_ReserveLoopIndex()
{
	__asm
	{
		#define v       (esp +  4H)
		#define n       (esp +  8H)
		#define Indices (ebp + 14H)

		mov     ecx, dword ptr [Indices]
		mov     edx, dword ptr [n]
		call    vector_dword_reserve
		mov     ecx, dword ptr [v]
		mov     edx, dword ptr [n]
		jmp     vector_string_reserve

		#undef v
		#undef n
		#undef Indices
	}
}
