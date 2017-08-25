#include "TMainForm.h"

#define SHOW_ERROR_MESSAGE 1

#if SHOW_ERROR_MESSAGE
extern DWORD dwErrorMessageId;
#endif

__declspec(naked) void __cdecl TSSGSubject_Write_WithDrawTree()
{
	__asm
	{
		#define _NULL     0
		#define _MainForm 0064CE2CH

#if SHOW_ERROR_MESSAGE
		mov     dword ptr [dwErrorMessageId], 0
		mov     ecx, dword ptr [ecx + 12]
#endif
		mov     edx, dword ptr [esp + 12]
		mov     eax, dword ptr [esp +  8]
		push    edx
		push    eax
		mov     edx, dword ptr [esp +  8]
		mov     eax, dword ptr [esp + 12]
		mov     dword ptr [esp + 20], edx
		push    eax
#if SHOW_ERROR_MESSAGE
		call    ecx
#else
		call    dword ptr [ecx + 12]
#endif
		mov     dword ptr [esp + 20], eax
		mov     eax, _MainForm
		mov     dword ptr [esp + 16], TRUE
		mov     eax, dword ptr [eax]
		mov     dword ptr [esp + 12], 0
		mov     dword ptr [esp +  8], 0
		mov     dword ptr [esp +  4], _NULL
		mov     dword ptr [esp     ], eax
		call    TMainForm_DrawTree
		pop     eax
		ret

		#undef _NULL
		#undef _MainForm
	}
}
