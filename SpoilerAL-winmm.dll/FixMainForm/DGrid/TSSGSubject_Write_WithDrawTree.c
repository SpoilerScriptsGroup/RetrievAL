#include "TMainForm.h"

#define SHOW_ERROR_MESSAGE 1

#if SHOW_ERROR_MESSAGE
extern DWORD dwErrorMessageId;
#endif

__declspec(naked) void __cdecl TSSGSubject_Write_WithDrawTree()
{
	__asm
	{
		#define _NULL 0

#if SHOW_ERROR_MESSAGE
		mov     dword ptr [dwErrorMessageId], 0
#endif
		mov     edx, dword ptr [esp + 12]
		mov     eax, dword ptr [esp +  8]
		push    edx
		push    eax
		mov     edx, dword ptr [esp +  8]
		mov     eax, dword ptr [esp + 12]
		mov     dword ptr [esp + 20], edx
		push    eax
		call    dword ptr [ecx + 12]
		mov     ecx, dword ptr ds:[_MainForm]
		mov     dword ptr [esp + 20], eax
		mov     dword ptr [esp + 16], TRUE
		mov     dword ptr [esp + 12], 0
		mov     dword ptr [esp +  8], 0
		mov     dword ptr [esp +  4], _NULL
		mov     dword ptr [esp     ], ecx
		call    TMainForm_DrawTree
		pop     eax
		ret

		#undef _NULL
	}
}
