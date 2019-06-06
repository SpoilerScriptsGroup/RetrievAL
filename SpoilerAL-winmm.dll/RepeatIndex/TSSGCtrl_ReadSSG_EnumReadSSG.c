#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

extern DWORD  RepeatDepth;
extern BOOL   IsProcessAttached;
extern string ProcessAttachCode;
extern string ProcessDetachCode;

__declspec(naked) void __cdecl TSSGCtrl_ReadSSG_EnumReadSSG()
{
	__asm
	{
		#define _NULL       0
		#define this        ebx
		#define SSGFile     (ebp + 0CH)
		#define ParentStack edi

		mov     dword ptr [RepeatDepth], 0
		mov     ecx, offset ProcessDetachCode
		call    string_dtor
		mov     dword ptr [ProcessDetachCode], 0
		mov     dword ptr [ProcessDetachCode + 4], 0
		mov     dword ptr [ProcessDetachCode + 16], 0
		mov     ecx, offset ProcessAttachCode
		call    string_dtor
		mov     dword ptr [ProcessAttachCode], 0
		mov     dword ptr [ProcessAttachCode + 4], 0
		mov     dword ptr [ProcessAttachCode + 16], 0
		mov     dword ptr [IsProcessAttached], 0
		mov     eax, dword ptr [SSGFile]
		push    -1
		push    0
		push    _NULL
		push    ParentStack
		push    eax
		push    this
		push    004E4708H
		jmp     TSSGCtrl_EnumReadSSG

		#undef _NULL
		#undef this
		#undef SSGFile
		#undef ParentStack
	}
}
