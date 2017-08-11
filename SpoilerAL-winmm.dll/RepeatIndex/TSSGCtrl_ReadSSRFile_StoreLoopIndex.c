#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector.h"

EXTERN_C void __fastcall vector_DWORD_push_back(void *v, DWORD value);

__declspec(naked) void __cdecl TSSGCtrl_ReadSSRFile_StoreLoopIndex()
{
	__asm
	{
		#define TSSGCtrl_LoopSSRFile 00501C2CH
		#define LoopVal              (esp + 10H)
		#define Indices              (ebp + 14H)

		mov     edx, dword ptr [LoopVal]
		mov     ecx, dword ptr [Indices]
		push    TSSGCtrl_LoopSSRFile
		jmp     vector_DWORD_push_back

		#undef TSSGCtrl_LoopSSRFile
		#undef LoopVal
		#undef Indices
	}
}
