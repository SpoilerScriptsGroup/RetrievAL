#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"
#include "TSSGCtrl.h"

__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_StoreLoopIndex()
{
	__asm
	{
		#define LoopVal              (esp + 0x10)
		#define Indices              (ebp + 0x14)

		mov     edx, dword ptr [LoopVal]
		mov     ecx, dword ptr [Indices]
		push    TSSGCtrl_LoopSSRFile
		jmp     vector_dword_push_back
		
		#undef Indices
		#undef LoopVal
	}
}
