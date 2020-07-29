#include <windows.h>
#include "TControl.h"
#include "TMainForm.h"

extern const DWORD _TFont_SetName;

__declspec(naked) void __cdecl TMainForm_LoadSetting_ListLBox_Font_SetName()
{
	__asm
	{
		#define this edi

		push    eax
		push    edx
		mov     eax, dword ptr [this + TMainForm.LockCBox]
		mov     eax, dword ptr [eax + TControl.Font]
		call    dword ptr [_TFont_SetName]
		mov     eax, dword ptr [this + TMainForm.ToggleCBox]
		mov     edx, dword ptr [esp]
		mov     eax, dword ptr [eax + TControl.Font]
		call    dword ptr [_TFont_SetName]
		mov     eax, dword ptr [this + TMainForm.MultiLBox]
		mov     edx, dword ptr [esp]
		mov     eax, dword ptr [eax + TControl.Font]
		call    dword ptr [_TFont_SetName]
		mov     eax, dword ptr [this + TMainForm.ListLBox]
		mov     edx, dword ptr [esp]
		mov     eax, dword ptr [eax + TControl.Font]
		call    dword ptr [_TFont_SetName]
		pop     edx
		pop     eax
		jmp     dword ptr [_TFont_SetName]

		#undef this
	}
}
