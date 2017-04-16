#include <windows.h>
#include "bcb6_std_vector_string.h"

__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_ReserveLoopIndex()
{
	__asm
	{
		#define v       (esp +  4H)
		#define n       (esp +  8H)
		#define Indices (ebp + 14H)

		mov     eax, dword ptr [n]
		mov     ecx, dword ptr [Indices]
		shl     eax, 2
		push    eax
		push    ecx
		call    dword ptr [bcb6_std_vector_reserve]
		add     esp, 8
		jmp     dword ptr [bcb6_std_vector_string_reserve]

		#undef v
		#undef n
		#undef Indices
	}
}
