#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_ReserveLoopIndex()
{
	__asm
	{
		#define v       (esp + 0x04)
		#define n       (esp + 0x08)
		#define m       (esp + 0x0C)
		#define Indices (ebp + 0x14)

		mov     ecx, dword ptr [Indices]
		mov     edx, dword ptr [n]
		inc     edx
		call    vector_dword_reserve
		mov     ecx, dword ptr [Indices]
		mov     edx, dword ptr [m]
		call    vector_dword_push_back
		mov     ecx, dword ptr [v]
		mov     edx, dword ptr [n]
		imul    edx, dword ptr [m]
		jmp     vector_string_reserve
		
		#undef Indices
		#undef x
		#undef n
		#undef v
	}
}
