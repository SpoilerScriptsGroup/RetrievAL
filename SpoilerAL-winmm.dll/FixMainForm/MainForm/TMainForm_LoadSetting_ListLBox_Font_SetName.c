#include <windows.h>

extern const DWORD _TFont_SetName;

__declspec(naked) void __cdecl TMainForm_LoadSetting_ListLBox_Font_SetName()
{
	__asm
	{
		#define this                          edi
		#define offsetof_TMainForm_LockCBox   0x03D0
		#define offsetof_TMainForm_ToggleCBox 0x03DC
		#define offsetof_TMainForm_MultiLBox  0x03E4
		#define offsetof_TMainForm_ListLBox   0x03E8
		#define offsetof_TControl_Font        0x68

		push    eax
		push    edx
		mov     eax, dword ptr [this + offsetof_TMainForm_LockCBox]
		mov     eax, dword ptr [eax + offsetof_TControl_Font]
		call    dword ptr [_TFont_SetName]
		mov     eax, dword ptr [this + offsetof_TMainForm_ToggleCBox]
		mov     edx, dword ptr [esp]
		mov     eax, dword ptr [eax + offsetof_TControl_Font]
		call    dword ptr [_TFont_SetName]
		mov     eax, dword ptr [this + offsetof_TMainForm_MultiLBox]
		mov     edx, dword ptr [esp]
		mov     eax, dword ptr [eax + offsetof_TControl_Font]
		call    dword ptr [_TFont_SetName]
		mov     eax, dword ptr [this + offsetof_TMainForm_ListLBox]
		mov     edx, dword ptr [esp]
		mov     eax, dword ptr [eax + offsetof_TControl_Font]
		call    dword ptr [_TFont_SetName]
		pop     edx
		pop     eax
		jmp     dword ptr [_TFont_SetName]

		#undef this
		#undef offsetof_TMainForm_LockCBox
		#undef offsetof_TMainForm_ToggleCBox
		#undef offsetof_TMainForm_MultiLBox
		#undef offsetof_TMainForm_ListLBox
		#undef offsetof_TControl_Font
	}
}
