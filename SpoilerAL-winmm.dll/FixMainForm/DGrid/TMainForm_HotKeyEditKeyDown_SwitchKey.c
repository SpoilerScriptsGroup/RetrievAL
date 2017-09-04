#include <windows.h>
#include "TMainForm.h"

void __stdcall TMainForm_HotKeyEditKeyDown_PageUp(TMainForm *this);
void __stdcall TMainForm_HotKeyEditKeyDown_PageDown(TMainForm *this);
void __stdcall TMainForm_HotKeyEditKeyDown_End(TMainForm *this);
void __stdcall TMainForm_HotKeyEditKeyDown_Home(TMainForm *this);

__declspec(naked) void __cdecl TMainForm_HotKeyEditKeyDown_SwitchKey()
{
	__asm
	{
		#define ReturnAddress 00443484H
		#define this          ebx

		dec     cx
		jz      L1
		dec     cx
		jz      L2
		dec     cx
		jz      L3
		dec     cx
		jz      L4
		sub     cx, VK_UP - VK_HOME
		mov     eax, 004431E4H
		mov     edx, 004431F7H
		jz      L5
		jmp     eax
	L1:
		push    this
		push    ReturnAddress
		jmp     TMainForm_HotKeyEditKeyDown_PageUp
	L2:
		push    this
		push    ReturnAddress
		jmp     TMainForm_HotKeyEditKeyDown_PageDown
	L3:
		push    this
		push    ReturnAddress
		jmp     TMainForm_HotKeyEditKeyDown_End
	L4:
		push    this
		push    ReturnAddress
		jmp     TMainForm_HotKeyEditKeyDown_Home
	L5:
		jmp     edx

		#undef ReturnAddress
		#undef this
	}
}
