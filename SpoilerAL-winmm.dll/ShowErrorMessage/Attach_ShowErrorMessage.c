#include <windows.h>
#include <winternl.h>
#include <commctrl.h>
#define STRSAFE_NO_CB_FUNCTIONS
#include <strsafe.h>
#include "xx.h"
#include "intrinsic.h"
#include "ErrorMessage.h"
#include "ToolTip\ToolTip.h"

EXTERN_C void __cdecl TProcessCtrl_Write_GetLastError();
EXTERN_C void __cdecl TSSGActionListner_OnSubjectDisabled_SetErrorMessage();

static __declspec(naked) int __stdcall SysUtils_GetExceptionObject_LoadResString(
	HINSTANCE hInstance,
	UINT      uID,
	LPSTR     lpBuffer,
	int       cchBufferMax)
{// EExternalException.CreateFmt(SExternalException, [P.ExceptionCode])
	__asm {
		cmp  dword ptr [esp + 0x041C], 0x005C3109
		je   EXTERN
	REVERT:
		jmp  LoadStringA
		ud2

		align 16
	EXTERN:
		mov  ecx, [ebp - 0x04]
		mov  eax, [ecx]EXCEPTION_RECORD32.ExceptionCode
		cmp  eax, CPP_EXCEPT_CODE
		jne  NTSTAT

		lea  edx, [esp + 0x0C]
		push 0
		mov  eax, esp
		push 0
		mov  ecx, esp
		push STRSAFE_IGNORE_NULLS | STRSAFE_NULL_ON_FAILURE
		push ecx
		push eax
		push dword ptr [edx + size PVOID]
		mov  ecx, [ebp - 0x04]
		push [ecx]EXCEPTION_RECORD32.ExceptionInformation
		push dword ptr [edx + size PVOID]
		push dword ptr [edx]
		call StringCchCopyNExA
		pop  ecx
		pop  eax
		mov  eax, [esp + 0x10]
		sub  eax, ecx
		ret  16

		align 16
	NTSTAT:
		test eax, eax
		jns  FORMAT
		push eax
		call RtlNtStatusToDosError
	FORMAT:
		mov  edx, [esp + 0x0C]
		mov  ecx, [esp + 0x10]
		push 0
		push ecx
		push edx
		push 0
		push eax
		push 0
		push FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM
		call FormatMessageA
		test eax, eax
		jz   REVERT
		ret  16
	}
}

static __declspec(naked) void __stdcall CloseHandleIgnoreInvalid(HANDLE hObject)
{// Avoid SetLastError when close CurrentProcess.
	__asm {
		cmp dword ptr [esp + 4], -1
		je  REVERT
		jmp CloseHandle
		ud2

		align 16
	REVERT:
		ret 4
	}
}

#define MOV_ECX_EAX   (WORD )0xC88B
#define MOV_EDX_IMM32 (BYTE )0xBA
#define CALL_REL32    (BYTE )0xE8
#define JMP_REL32     (BYTE )0xE9
#define JMP_REL8      (BYTE )0xEB
#define NOP           (BYTE )0x90
#define NOP_X2        (WORD )0x9090
#define NOP_X4        (DWORD)0x90909090
#define INT3          (BYTE )0xCC

EXTERN_C void __cdecl Attach_ShowErrorMessage()
{
	// TMainForm::ToggleCBoxClick
	// attach ClearErrorMessageId at "FixMainForm\DGrid\TSSGSubject_Write_WithDrawTree.asm"
	*(LPBYTE )0x0043D620 = CALL_REL32;
	*(LPDWORD)0x0043D621 = (DWORD)ShowErrorMessage - (0x0043D621 + sizeof(DWORD));
	*(LPBYTE )0x0043D625 = JMP_REL8;
	*(LPBYTE )0x0043D626 = 0x0043D647 - (0x0043D626 + sizeof(BYTE));
	*(LPBYTE )0x0043D627 = NOP;

	// TMainForm::ListLBoxClick
	// attach ClearErrorMessageId at "FixMainForm\DGrid\TSSGSubject_Write_WithDrawTree.asm"
	*(LPBYTE )0x0043D8D0 = CALL_REL32;
	*(LPDWORD)0x0043D8D1 = (DWORD)ShowErrorMessage - (0x0043D8D1 + sizeof(DWORD));
	*(LPBYTE )0x0043D8D5 = JMP_REL8;
	*(LPBYTE )0x0043D8D6 = 0x0043D8F7 - (0x0043D8D6 + sizeof(BYTE));
	*(LPBYTE )0x0043D8D7 = NOP;

	// TMainForm::MultiListUpdate
	// attach ClearErrorMessageId at "FixMainForm\DGrid\TSSGSubject_Write_WithDrawTree.asm"
	*(LPBYTE )0x0043E851 = CALL_REL32;
	*(LPDWORD)0x0043E852 = (DWORD)ShowErrorMessage - (0x0043E852 + sizeof(DWORD));
	*(LPBYTE )0x0043E856 = JMP_REL8;
	*(LPBYTE )0x0043E857 = 0x0043E878 - (0x0043E857 + sizeof(BYTE));
	*(LPBYTE )0x0043E858 = NOP;

	// TMainForm::StringEnterBtnClick
	// attach ClearErrorMessageId at "FixMainForm\DGrid\TSSGSubject_Write_WithDrawTree.asm"
	*(LPBYTE )0x0043ED7D = CALL_REL32;
	*(LPDWORD)0x0043ED7E = (DWORD)ShowErrorMessage - (0x0043ED7E + sizeof(DWORD));
	*(LPBYTE )0x0043ED82 = JMP_REL8;
	*(LPBYTE )0x0043ED83 = 0x0043EDA4 - (0x0043ED83 + sizeof(BYTE));
	*(LPBYTE )0x0043ED84 = NOP;

	// TMainForm::GoCalcEnter
	// attach ClearErrorMessageId at "FixMainForm\DGrid\TSSGSubject_Write_WithDrawTree.asm"
	*(LPBYTE )0x0043FACD = CALL_REL32;
	*(LPDWORD)0x0043FACE = (DWORD)ShowErrorMessage - (0x0043FACE + sizeof(DWORD));
	*(LPBYTE )0x0043FAD2 = JMP_REL8;
	*(LPBYTE )0x0043FAD3 = 0x0043FAFA - (0x0043FAD3 + sizeof(BYTE));
	*(LPDWORD)0x0043FAD4 = NOP_X4;

	// TProcessCtrl::OneRead
	// forwarding to ReadProcessMemory
	*(LPDWORD)0x004A6FF4 = BSWAP32(0x580F1004);
	*(LPDWORD)0x004A6FF8 = BSWAP32(0x246A0083);
	*(LPDWORD)0x004A6FFC = BSWAP32(0xEC100F11);
	*(LPDWORD)0x004A7000 = BSWAP32(0x042450E9);
	*(LPDWORD)0x004A7004 = 0x00600E0C - (0x004A7004 + sizeof(DWORD));

	// TProcessCtrl::OneWrite
	// forwarding to WriteProcessMemory
	*(LPDWORD)0x004A760C = BSWAP32(0x580F1004);
	*(LPDWORD)0x004A7610 = BSWAP32(0x246A0083);
	*(LPDWORD)0x004A7614 = BSWAP32(0xEC100F11);
	*(LPDWORD)0x004A7618 = BSWAP32(0x042450E9);
	*(LPDWORD)0x004A761C = 0x00600EA2 - (0x004A761C + sizeof(DWORD));

	// TSSGActionListner::OnSubjectDisabled
	*(LPDWORD)(0x0052EBD5 + 1) = (DWORD)TSSGActionListner_OnSubjectDisabled_SetErrorMessage - (0x0052EBD5 + 1 + sizeof(DWORD));

	// TSSGActionListner::OnSubjectWriteError
	*(LPBYTE )0x0052F1CE = JMP_REL8;
	*(LPBYTE )0x0052F1CF = 0x0052F1F5 - (0x0052F1CF + sizeof(BYTE));
	*(LPWORD )0x0052F1D0 = NOP_X2;
	*(LPBYTE )0x0052F1D2 = NOP;

	// System::LoadResString
	*(LPDWORD)(0x005D4355 + 1) = (DWORD)SysUtils_GetExceptionObject_LoadResString - (0x005D4355 + 1 + sizeof(DWORD));

	// ::CloseHandle
	*(LPBYTE )0x00600BCC = JMP_REL32;
	*(LPDWORD)0x00600BCD = (DWORD)CloseHandleIgnoreInvalid - (0x00600BCD + sizeof(DWORD));
	*(LPBYTE )0x00600BD1 = INT3;

	// ::OpenProcess
	*(LPBYTE )0x00600DE8 = JMP_REL32;
	*(LPDWORD)0x00600DE9 = (DWORD)OpenProcessWithSaveError - (0x00600DE9 + sizeof(DWORD));
	*(LPBYTE )0x00600DED = INT3;

	// ::WriteProcessMemory
	*(LPBYTE )0x00600EA2 = JMP_REL32;
	*(LPDWORD)0x00600EA3 = (DWORD)WriteProcessMemoryWithSaveError - (0x00600EA3 + sizeof(DWORD));
	*(LPBYTE )0x00600EA7 = INT3;

	// "Ç≈ì«Ç›çûÇ›ÉGÉâÅ[ÅB"
	// referenced at TSSGActionListner::OnSubjectWriteError
	*(LPDWORD)(0x0064369D + 2) = BSWAP32('èëÇ´');

#if USE_TOOLTIP
	// TMainForm::M_SaveSubjectStatusClick
	*(LPDWORD)(0x0044BBDE + 1) = (DWORD)ShowInfoToolTip - (0x0044BBDE + 1 + sizeof(DWORD));

	// TMainForm::M_LoadSubjectStatusClick
	*(LPDWORD)(0x0044C15C + 1) = (DWORD)ShowInfoToolTip - (0x0044C15C + 1 + sizeof(DWORD));

	// TMainForm::M_AutoDialogAdjustmentClick
	*(LPDWORD)(0x0044C2ED + 1) = (DWORD)ShowInfoToolTip - (0x0044C2ED + 1 + sizeof(DWORD));
	*(LPDWORD)(0x0044C5AC + 1) = (DWORD)ShowInfoToolTip - (0x0044C5AC + 1 + sizeof(DWORD));

	// TMainForm::M_SystemCheckClick
	*(LPDWORD)(0x00459E82 + 1) = (DWORD)ShowInfoToolTip - (0x00459E82 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x0045A1B3 + 1) = (DWORD)ShowInfoToolTip - (0x0045A1B3 + 1 + sizeof(DWORD));

	// TSearchForm::ReSearchToolBtnClick
	*(LPDWORD)(0x004957C8 + 1) = (DWORD)ShowInfoToolTip - (0x004957C8 + 1 + sizeof(DWORD));

	// TSearchForm::ChangeByteCodeToolBtnClick
	*(LPDWORD)(0x00496B48 + 1) = (DWORD)ShowInfoToolTip - (0x00496B48 + 1 + sizeof(DWORD));

	// TSearchForm::ChangeRangeToolBtnClick
	*(LPDWORD)(0x00496D7B + 1) = (DWORD)ShowInfoToolTip - (0x00496D7B + 1 + sizeof(DWORD));
	*(LPDWORD)(0x00496F66 + 1) = (DWORD)ShowInfoToolTip - (0x00496F66 + 1 + sizeof(DWORD));

	// TSearchForm::P_CopyClick
	*(LPDWORD)(0x004977EB + 1) = (DWORD)ShowInfoToolTip - (0x004977EB + 1 + sizeof(DWORD));

	// TSearchForm::OkBBtnClick
	*(LPDWORD)(0x0049A4DA + 1) = (DWORD)ShowInfoToolTip - (0x0049A4DA + 1 + sizeof(DWORD));

	// ::ShowMessage
	*(LPBYTE )0x0057AEDC = MOV_EDX_IMM32;
	*(LPDWORD)0x0057AEDD = TTI_ERROR;
	*(LPWORD )0x0057AEE1 = MOV_ECX_EAX;
	*(LPBYTE )0x0057AEE3 = JMP_REL32;
	*(LPDWORD)0x0057AEE4 = (DWORD)ShowToolTip - (0x0057AEE4 + sizeof(DWORD));
#endif
}
