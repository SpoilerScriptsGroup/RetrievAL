#include <windows.h>
#include <dhcpsapi.h>
#include "TFindNameForm.h"
#include "TMainForm.h"
#undef MainForm

TSSGSubject * __fastcall TFindNameForm_FindStartBtnClick_FixGetSelectSubject(TMainForm *);

__declspec(naked)
static DWORD_DWORD __fastcall TFindNameForm_FindLBoxClick_ListLBox_set_TopIndex(
	TMainForm           *const MainForm,
	long                 const findIndex,
	TFindNameForm const *const This)
{
	static const DWORD Vcl_StdCtrls_TCustomListBox_set_TopIndex = 0x00545EE8;

	__asm {
		push edx
		push dword ptr [ecx]TMainForm.ListLBox
		mov  eax, [ecx]TMainForm.MultiLBox
		call Vcl_StdCtrls_TCustomListBox_set_TopIndex
		pop  eax
		pop  edx
		ret  4
	}
}

#define PUSH_EBX   (BYTE)0x53
#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8

EXTERN_C void __cdecl Attach_FixFindName()
{
	// TFindNameForm::FindStartBtnClick
	*(LPBYTE )0x0048413F = CALL_REL32;
	*(LPDWORD)0x00484140 = (DWORD)TFindNameForm_FindStartBtnClick_FixGetSelectSubject - (0x00484140 + sizeof(DWORD));
	*(LPBYTE )0x00484144 = NOP;
	
	// TFindNameForm::FindLBoxClick
	*(LPBYTE )0x0048682D = PUSH_EBX;
	*(LPBYTE )0x0048682E = CALL_REL32;
	*(LPDWORD)0x0048682F = (DWORD)TFindNameForm_FindLBoxClick_ListLBox_set_TopIndex - (0x0048682F + sizeof(DWORD));
}
