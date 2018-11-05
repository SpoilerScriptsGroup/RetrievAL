#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#include <mbstring.h>
#include "intrinsic.h"
#include "TMainForm.h"
#include "TSSString.h"
#include "TWinControl.h"
#include "TCalcValBox.h"
#include "TSSCalc.h"
#include "TSSFloatCalc.h"

void(__cdecl * const TMainForm_GoCalcEnter)(TMainForm* mainForm) = (LPVOID)0x0043F454;
void(__cdecl * const TMainForm_GoCalcHexChange)(TMainForm* mainForm, boolean IsCalcHex) = (LPVOID)0x00447194;
void(__cdecl * const TCalcImage_SetStatus)(TCalcImage* calcImage, long Index, byte Status) = (LPVOID)0x00483678;
byte(__cdecl * const TCalcImage_GetStatus)(TCalcImage* calcImage, long Index) = (LPVOID)0x004836B4;
TSSGSubject* (__cdecl * const TSSGCtrl_GetTargetSubject)(TSSGSubject* SSGS) = (LPVOID)0x004EDF84;

static void __declspec(naked) __fastcall TCalcImage_DrawBtnStub(TCalcImage* calcImage, long No, boolean IsPress) {
	static void (__fastcall * const TCalcImage_DrawBtn)() = (LPVOID)0x0048292C;
	__asm {// adapt to Borland
		mov  eax, ecx
		pop  ecx
		xchg ecx, [esp]
		jmp  TCalcImage_DrawBtn
	}
}

static void __cdecl TMainForm_CalcButtonPushFunction(TMainForm* mainForm, long BtnNum) {
	HWND edit = TWinControl_GetHandle(bcb6_std_vector_type_at(&mainForm->calcImage->valBox, TCalcValBox, 1).edit);
	long chrs = '0' + BtnNum, sta, end, len;
	TSSGSubject* TargetS = TSSGCtrl_GetTargetSubject(mainForm->selectSubject);
	if (TargetS) {
		unsigned long type = TSSGSubject_GetArgType(TargetS);
		if (type == atDOUBLE && mainForm->isCalcHex) {
			TMainForm_GoCalcHexChange(mainForm, FALSE);
			TCalcImage_SetStatus(mainForm->calcImage, 18, TCalcImage_GetStatus(mainForm->calcImage, 18) & 0xFE | !mainForm->isCalcHex);
			TCalcImage_DrawBtnStub(mainForm->calcImage, 18, FALSE);
		}
		switch (BtnNum) {
		case 0xA:
		case 0xB:
		case 0xC:
		case 0xD:
		case 0xE:
		case 0xF:
			if (!mainForm->isCalcHex) return;
			chrs = '7' + BtnNum;
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)&chrs);
			break;
		case 16:// +/-
			if (mainForm->isCalcHex) return;
			chrs = '-';
			SendMessageA(edit, EM_GETSEL, (WPARAM)&sta, (LPARAM)&end);
			if (sta || end < GetWindowTextLengthA(edit)) {
				CHAR head[2];
				GetWindowTextA(edit, head, sizeof(head));
				if (*head == '-') {
					chrs = '\0';
					SendMessageA(edit, EM_SETSEL, 0, 1);
				} else {
					SendMessageA(edit, EM_SETSEL, 0, 0);
				}
			}
			SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)&chrs);
			SendMessageA(edit, EM_SETSEL, LONG_MAX, LONG_MAX);
			break;
		case 17:// BS
			chrs = '\0';
			SendMessageA(edit, EM_GETSEL, (WPARAM)NULL, (LPARAM)&end);
			SendMessageA(edit, EM_SETSEL, end - 1, end);
			SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)&chrs);
			break;
		case 18:// 16/10
			if (type == atDOUBLE) {
				TCalcImage_SetStatus(mainForm->calcImage, 18, TCalcImage_GetStatus(mainForm->calcImage, 18) & 0xFE | mainForm->isCalcHex);
				TCalcImage_DrawBtnStub(mainForm->calcImage, 18, TRUE);
				return;
			}
			TMainForm_GoCalcHexChange(mainForm, !mainForm->isCalcHex);
			break;
		case 19:// enter
			TMainForm_GoCalcEnter(mainForm);
			break;
		case 20:// min
		case 21:// max
			{
				char buff[0x100];
				switch (type) {
				case atLONG:
					{
						TSSCalc* SSGS = (TSSCalc*)TargetS;
						if (mainForm->isCalcHex)
							_snprintf(buff, sizeof(buff), "%0*X", TSSGSubject_GetSize(TargetS) << 1, BtnNum == 20 ? SSGS->min : SSGS->max);
						else
							_snprintf(buff, sizeof(buff), SSGS->isUnsigned ? "%u" : "%d", BtnNum == 20 ? SSGS->min : SSGS->max);
						break;
					}
				case atDOUBLE:
					{
						TSSFloatCalc* SSGS = (TSSFloatCalc*)TargetS;
						_snprintf(buff, sizeof(buff), "%f", BtnNum == 20 ? SSGS->min : SSGS->max);
						break;
					}
				default:
					return;
				}
				SendMessageA(edit, EM_SETSEL, 0, LONG_MAX);
				SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)buff);
				break;
			}
		case 22:// .
			if (type != atDOUBLE) return;
			chrs = '.';
			len = GetWindowTextLengthA(edit);
			SendMessageA(edit, EM_GETSEL, (WPARAM)&sta, (LPARAM)&end);
			if (sta || end < len) {
				extern HANDLE hHeap;
				LPSTR lpMem = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, len + 1);
				if (lpMem) {
					LPSTR pos = NULL;
					GetWindowTextA(edit, lpMem, len + 1);
					if (len == 1 && *lpMem == '-')
						chrs = BSWAP16('0.');
					else
						pos = _mbschr(lpMem, '.');
					HeapFree(hHeap, 0, lpMem);
					if (pos) return;
				}
			} else chrs = BSWAP16('0.');
			SendMessageA(edit, EM_REPLACESEL, FALSE, (LPARAM)&chrs);
			break;
		}
	}
}

static void __fastcall TMainForm_GoCalcEnter_selectAll(TMainForm* mainForm) {
	HWND edit = TWinControl_GetHandle(bcb6_std_vector_type_at(&mainForm->calcImage->valBox, TCalcValBox, 1).edit);
	SendMessageA(edit, EM_SETSEL, 0, LONG_MAX);
	SendMessageA(edit, WM_SETFOCUS, (WPARAM)NULL, 0);
}

static void __declspec(naked) __fastcall TMainForm_GoCalcEnter_destroyMessage(bcb6_std_string* this, DWORD two) {
	extern const DWORD F005E0EA8;
	extern BOOL FixTheProcedure;
	__asm {// Borland's fastcall
		call F005E0EA8
		mov  ecx, ebx
	//	mov  dword ptr[esp], 0x0043FB70
		jmp  TMainForm_GoCalcEnter_selectAll
	}
}

static HANDLE __fastcall TMainForm_SubjectAccess_GetCautionHandle(TMainForm* this, TSSString* SSGS) {
	return SSGS->caution ? TWinControl_GetHandle(this->CautionREdit) : NULL;
}

static __declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueString() {
	__asm {
		mov   edx, edi
		mov   ecx, ebx
		call  TMainForm_SubjectAccess_GetCautionHandle
		mov   ecx, 0x00445931
		mov   edx, 0x00445B4A
		test  eax, eax
		cmovz edx, ecx
		jmp   edx
	}
}

static __declspec(naked) void __fastcall TMainForm_SubjectAccess_CautiousString(LPVOID StringNewValEdit) {
	__asm {// Borland's fastcall,     eax is StringNewValEdit
		call dword ptr [edx + 0xC0]// edx is StringNewValEdit's VTable, call SetFocus
		mov  edx, [ebp - 0x02FC]// TSSString *SSGS
		mov  ecx, ebx
		call TMainForm_SubjectAccess_GetCautionHandle
		test eax, eax
		jnz  CAUTION
		ret
	CAUTION:
		push dword ptr [ebp - 0xE4]// TSSArgString.value.c_str()
		push 0
		push WM_SETTEXT
		push eax
		call SendMessageA
		// CautionTabS->TabVisible = true;
		mov  edx, 1
		mov  eax, [ebx + 0x0408]
		mov  ecx, 0x00594684
		call ecx
		// PageCtrl->ActivePage = CautionTabS;
		mov  edx, [ebx + 0x0408]
		mov  eax, [ebx + 0x03C4]
		mov  ecx, 0x00594E6C
		jmp  ecx
	}
}

static __declspec(naked) void __cdecl TMainForm_StringEnterBtnClick_GetSubjectName(bcb6_std_string* retval, TSSGSubject* this, TSSGCtrl* SSGC) {
	__asm {
		// this = TMainForm*->selectSubject
		mov  ecx, [esi + 0x0524]
		xchg ecx, [esp + 8]
		jmp  TSSGSubject_GetSubjectName
	}
}

static void (__cdecl * const TSSGCtrl_IsLocked)(TSSGCtrl* this, TSSGSubject* SSGS) = (void*)0x004FE9B0;
static __declspec(naked) void __cdecl TMainForm_SetLockVisible_IsLocked(TSSGCtrl* SSGC, TSSGSubject* SSGS) {
	__asm {
		// SSGS = TMainForm*->selectSubject
		mov  edx, [ebx + 0x0524]
		xchg edx, [esp + 8]
		jmp  TSSGCtrl_IsLocked
	}
}

EXTERN_C void __cdecl TMainForm_ctor();
EXTERN_C void __cdecl TMainForm_dtor();
EXTERN_C void __cdecl TMainForm_LoadSetting_ListLBox_Font_SetName();
EXTERN_C void __cdecl TMainForm_LoadSetting_SetUserMode();
EXTERN_C void __cdecl Caller_TMainForm_DGridSelectCell();
EXTERN_C void __cdecl TMainForm_SubjectAccess_FixDirSameChildren();
EXTERN_C void __cdecl TSSGSubject_Write_WithDrawTree();
EXTERN_C void __cdecl TMainForm_HotKeyEditKeyDown_SwitchKey();
EXTERN_C void __cdecl TMainForm_HotKeyEditKeyDown_Up();
EXTERN_C void __cdecl TMainForm_HotKeyEditKeyDown_Down();
EXTERN_C void __cdecl Caller_TMainForm_SetLockVisible_ModifyLockName();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_DrawHover();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_FixLabelDrawX();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifySplitRoll();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifySplitLabel();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifyNowValueBoolVector();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifyNowValueCalc();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_ModifyNowValueFloatCalc();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_FixDefaultColWidth();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_DrawFocusRect();
EXTERN_C void __cdecl Caller_TMainForm_DrawTree();
EXTERN_C void __cdecl TMainForm_DGridMouseMove_DrawTree();
EXTERN_C void __cdecl TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible();
EXTERN_C void __cdecl TMainForm_FormResize_CheckTreeSize();
EXTERN_C void __cdecl Caller_TMainForm_FormMouseWheel();
EXTERN_C void __cdecl TMainForm_CheckTreeSize();
EXTERN_C void __cdecl TMainForm_M_TitleSelectClick_OpenSSG();
EXTERN_C void __cdecl TFindNameForm_ctor();
EXTERN_C void __cdecl TGuideForm_ctor();
EXTERN_C void __cdecl TGuideForm_UpdateUserModeMenu();
EXTERN_C void __cdecl TSearchForm_ctor();
EXTERN_C void __cdecl TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage();
EXTERN_C void __fastcall TMainForm_M_CustomizeClick_RedrawCalcImage(void *this);

#define JB_REL32              (WORD )0x820F
#define JB_REL8               (BYTE )0x72
#define ADD_ESP               (WORD )0xC483
#define MOV_ECX_DWORD_PTR_EAX (WORD )0x088B
#define MOV_ECX_DWORD_PTR_ECX (WORD )0x098B
#define MOV_ECX_EBX           (WORD )0xCB8B
#define CALL_REL32            (BYTE )0xE8
#define JMP_REL32             (BYTE )0xE9
#define JMP_REL8              (BYTE )0xEB
#define NOP                   (BYTE )0x90
#define NOP_X2                (WORD )0x9090
#define NOP_X4                (DWORD)0x90909090

EXTERN_C void __cdecl Attach_FixMainForm()
{
	// TMainForm::TMainForm
	*(LPBYTE )0x00401FA5 = CALL_REL32;
	*(LPDWORD)0x00401FA6 = (DWORD)TMainForm_ctor - (0x00401FA6 + sizeof(DWORD));
	*(LPBYTE )0x00401FAA = NOP;

	// TMainForm::~TMainForm
	*(LPBYTE )0x0045FDE9 = JMP_REL32;
	*(LPDWORD)0x0045FDEA = (DWORD)TMainForm_dtor - (0x0045FDEA + sizeof(DWORD));
	*(LPDWORD)0x0045FDEE = NOP_X4;

	// TMainForm::LoadSetting
	*(LPDWORD)(0x004307A0 + 1) = (DWORD)TMainForm_LoadSetting_ListLBox_Font_SetName - (0x004307A0 + 1 + sizeof(DWORD));

	*(LPBYTE )0x00437E28 = JMP_REL32;
	*(LPDWORD)0x00437E29 = (DWORD)TMainForm_LoadSetting_SetUserMode - (0x00437E29 + sizeof(DWORD));
	*(LPBYTE )0x00437E2D = NOP;

	// TMainForm::DGridSelectCell
	*(LPBYTE )0x00439E90 = JMP_REL32;
	*(LPDWORD)0x00439E91 = (DWORD)Caller_TMainForm_DGridSelectCell - (0x00439E91 + sizeof(DWORD));
	*(LPBYTE )0x00439E95 = NOP;

	// TMainForm::SubjectAccess
	*(LPDWORD)(0x00439F42 + 1) = (DWORD)TMainForm_SubjectAccess_FixDirSameChildren - (0x00439F42 + 1 + sizeof(DWORD));

	*(LPWORD )0x0043A2E6 = BSWAP16(0x8B93);// mov edx, dword ptr [ebx + ... 
	*(LPDWORD)0x0043A2E8 = offsetof(TMainForm, selectSubject);

	*(LPWORD )0x0043A88E = BSWAP16(0x8B8B);// mov ecx, dword ptr [ebx + ... 
	*(LPDWORD)0x0043A890 = offsetof(TMainForm, selectSubject);

	*(LPBYTE )0x0043B06F = CALL_REL32;
	*(LPDWORD)0x0043B070 = (DWORD)TMainForm_SubjectAccess_CautiousString - (0x0043B070 + sizeof(DWORD));
	*(LPBYTE )0x0043B074 = NOP;

	// TMainForm::ToggleCBoxClick
	*(LPBYTE )0x0043D52D = CALL_REL32;
	*(LPDWORD)0x0043D52E = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043D52E + sizeof(DWORD));
	*(LPBYTE )0x0043D532 = NOP;

	// TMainForm::ListLBoxClick
	*(LPBYTE )0x0043D774 = CALL_REL32;
	*(LPDWORD)0x0043D775 = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043D775 + sizeof(DWORD));
	*(LPBYTE )0x0043D779 = NOP;

	// TMainForm::MultiListUpdate
	*(LPWORD )0x0043E148 = MOV_ECX_DWORD_PTR_EAX;
	*(LPBYTE )0x0043E14A = CALL_REL32;
	*(LPDWORD)0x0043E14B = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043E14B + sizeof(DWORD));
	*(LPBYTE )0x0043E14F = NOP;

	// TMainForm::StringEnterBtnClick
	*(LPBYTE )0x0043EC5B = CALL_REL32;
	*(LPDWORD)0x0043EC5C = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043EC5C + sizeof(DWORD));
	*(LPBYTE )0x0043EC60 = NOP;

	*(LPDWORD)0x0043EEE7 = (DWORD)TMainForm_StringEnterBtnClick_GetSubjectName - (0x0043EEE7 + sizeof(DWORD));

	// TMainForm::GoCalcEnter
	*(LPDWORD)0x0043F692 = (DWORD)TMainForm_GoCalcEnter_destroyMessage - (0x0043F692 + sizeof(DWORD));
	*(LPDWORD)0x0043F6D1 = (DWORD)TMainForm_GoCalcEnter_destroyMessage - (0x0043F6D1 + sizeof(DWORD));
	*(LPDWORD)0x0043F965 = (DWORD)TMainForm_GoCalcEnter_destroyMessage - (0x0043F965 + sizeof(DWORD));
	*(LPDWORD)0x0043F9AA = (DWORD)TMainForm_GoCalcEnter_destroyMessage - (0x0043F9AA + sizeof(DWORD));

	*(LPWORD )0x0043FA6E = MOV_ECX_DWORD_PTR_ECX;
	*(LPBYTE )0x0043FA70 = CALL_REL32;
	*(LPDWORD)0x0043FA71 = (DWORD)TSSGSubject_Write_WithDrawTree - (0x0043FA71 + sizeof(DWORD));
	*(LPBYTE )0x0043FA75 = NOP;

	// TMainForm::HotKeyEditKeyDown
	*(LPBYTE )0x004431DE = JMP_REL32;
	*(LPDWORD)0x004431DF = (DWORD)TMainForm_HotKeyEditKeyDown_SwitchKey - (0x004431DF + sizeof(DWORD));
	*(LPBYTE )0x004431E3 = NOP;

	// TMainForm::HotKeyEditKeyDown
	*(LPBYTE )0x004431F7 = JMP_REL32;
	*(LPDWORD)0x004431F8 = (DWORD)TMainForm_HotKeyEditKeyDown_Up - (0x004431F8 + sizeof(DWORD));
	*(LPWORD )0x004431FC = NOP_X2;

	// TMainForm::HotKeyEditKeyDown
	*(LPBYTE )0x00443267 = JMP_REL32;
	*(LPDWORD)0x00443268 = (DWORD)TMainForm_HotKeyEditKeyDown_Down - (0x00443268 + sizeof(DWORD));
	*(LPWORD )0x0044326C = NOP_X2;

	// TMainForm::SetCalcNowValue
	*(LPBYTE )0x00440608 = NOP;
	*(LPBYTE )0x00440609 = JMP_REL32;

	*(LPBYTE )0x00440ECE = JB_REL8;

	// TMainForm::CalcButtonPushFunc
	*(LPBYTE )0x00443490 = JMP_REL32;
	*(LPDWORD)0x00443491 = (DWORD)TMainForm_CalcButtonPushFunction - (0x00443491 + sizeof(DWORD));
	*(LPDWORD)0x00443495 = NOP_X4;

	// TMainForm::SetLockVisible
	*(LPDWORD)(0x004444DB + 1) = (DWORD)Caller_TMainForm_SetLockVisible_ModifyLockName - (0x004444DB + 1 + sizeof(DWORD));
	*(LPDWORD)(0x00444619 + 1) = (DWORD)Caller_TMainForm_SetLockVisible_ModifyLockName - (0x00444619 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004447E9 + 1) = (DWORD)TMainForm_SetLockVisible_IsLocked - (0x004447E9 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x00444C09 = JMP_REL32;
	*(LPDWORD)0x00444C0A = (DWORD)TMainForm_DrawTreeCell_DrawHover - (0x00444C0A + sizeof(DWORD));
	*(LPDWORD)0x00444C0E = NOP_X4;

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x00444E61 = JMP_REL32;
	*(LPDWORD)0x00444E62 = (DWORD)TMainForm_DrawTreeCell_FixLabelDrawX - (0x00444E62 + sizeof(DWORD));
	*(LPBYTE )0x00444E66 = NOP;

	// replaced at "SubjectStringTable\SubjectStringOperator.c" - TMainForm_DrawTreeCell_GetStrParam
#if 0
	// TMainForm::DrawTreeCell
	//   string tmpS( SS->GetStrParam() );
	*(LPDWORD)(0x00444FBB + 1) = (DWORD)TMainForm_DrawTreeCell_ModifySplitRoll - (0x00444FBB + 1 + sizeof(DWORD));

	//   DrawStr = SS->GetStrParam();
	*(LPDWORD)(0x004451C3 + 1) = (DWORD)TMainForm_DrawTreeCell_ModifySplitLabel - (0x004451C3 + 1 + sizeof(DWORD));
#endif

	// TMainForm::DrawTreeCell
	*(LPDWORD)0x00445402 = (DWORD)TMainForm_DrawTreeCell_ModifyNowValueString;

	// TMainForm::DrawTreeCell
	*(LPDWORD)0x00445406 = (DWORD)TMainForm_DrawTreeCell_ModifyNowValueBoolVector;

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445452 + 1) = (DWORD)TMainForm_DrawTreeCell_ModifyNowValueCalc - (0x00445452 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	/*
		sub     esp, 8                                  ; 004457F0 _ 83. EC, 08
		fstp    qword ptr [esp]                         ; 004457F3 _ DD. 1C 24
		lea     edx, [ebp - 0FCH]                       ; 004457F6 _ 8D. 95, FFFFFF04
		push    edx                                     ; 004457FC _ 52
		call    TStringDivision::ToStringDouble         ; 004457FD _ E8, <offset TMainForm_DrawTreeCell_ModifyNowValueFloatCalc - 00445802H>
		add     esp, 16                                 ; 00445802 _ 83. C4, 10
	*/
	*(LPDWORD)0x004457F0 = BSWAP32(0x83EC08DD);
	*(LPWORD )0x004457F4 = BSWAP16(0x1C24    );
	*(LPDWORD)0x004457FE = (DWORD)TMainForm_DrawTreeCell_ModifyNowValueFloatCalc - (0x004457FE + sizeof(DWORD));
	*(LPBYTE )0x00445804 = 0x10;

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445C82 + 1) = (DWORD)TMainForm_DrawTreeCell_FixDefaultColWidth - (0x00445C82 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445CD9 + 1) = (DWORD)TMainForm_DrawTreeCell_FixDefaultColWidth - (0x00445CD9 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445DDE + 1) = (DWORD)TMainForm_DrawTreeCell_FixDefaultColWidth - (0x00445DDE + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x0044602C = JMP_REL8;
	*(LPBYTE )0x0044602D = 0x6D;
	*(LPBYTE )0x0044602E = NOP;

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x0044609B = JMP_REL32;
	*(LPDWORD)0x0044609C = (DWORD)TMainForm_DrawTreeCell_DrawFocusRect - (0x0044609C + sizeof(DWORD));

	// TMainForm::DrawTree
	*(LPBYTE )0x00446448 = JMP_REL32;
	*(LPDWORD)0x00446449 = (DWORD)Caller_TMainForm_DrawTree - (0x00446449 + sizeof(DWORD));
	*(LPBYTE )0x0044644D = NOP;

	// TMainForm::GetFontColor
	((LPVOID*)0x0044652D)[9] = ((LPVOID*)0x0044652D)[0];
	((LPVOID*)0x0044652D)[0] = ((LPVOID*)0x0044652D)[7];

	// TMainForm::ChangeSubjectPanel
	*(LPDWORD)0x004465BF = 0;// jmp $

	// TMainForm::FillPanelImage
	*(LPBYTE )0x0044695A = JMP_REL32;
	*(LPDWORD)0x0044695B = 0x00446A32 - (0x0044695B + sizeof(DWORD));
	*(LPWORD )0x0044695F = NOP_X2;

	// TMainForm::DGridMouseMove
	*(LPBYTE )0x00446C5F = JMP_REL32;
	*(LPDWORD)0x00446C60 = (DWORD)TMainForm_DGridMouseMove_DrawTree - (0x00446C60 + sizeof(DWORD));
	*(LPBYTE )0x00446C64 = NOP;

	// TMainForm::ChangeSubjectPanel
	*(LPDWORD)(0x00446716 + 1) = (DWORD)TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible - (0x00446716 + 1 + sizeof(DWORD));

	// TMainForm::FormResize
	*(LPDWORD)(0x00446DB2 + 1) = (DWORD)TMainForm_FormResize_CheckTreeSize - (0x00446DB2 + 1 + sizeof(DWORD));

	// TMainForm::FormMouseWheel
	*(LPBYTE )0x00446DC0 = JMP_REL32;
	*(LPDWORD)0x00446DC1 = (DWORD)Caller_TMainForm_FormMouseWheel - (0x00446DC1 + sizeof(DWORD));

	// TMainForm::CheckTreeSize
	*(LPBYTE )0x00446E88 = JMP_REL32;
	*(LPDWORD)0x00446E89 = (DWORD)TMainForm_CheckTreeSize - (0x00446E89 + sizeof(DWORD));
	*(LPBYTE )0x00446E8D = NOP;

	// TMainForm::M_TitleSelectClick
	*(LPDWORD)(0x0044993B + 1) = (DWORD)TMainForm_M_TitleSelectClick_OpenSSG - (0x0044993B + 1 + sizeof(DWORD));

	// TMainForm::LoadCLD
	/*
		mov     eax, dword ptr [esi]                    ; 00454C91 _ 8B. 06
		add     eax, 360                                ; 00454C93 _ 05, 00000168
		mov     ecx, esp                                ; 00454C98 _ 8B. CC
		push    eax                                     ; 00454C9A _ 50
		push    ecx                                     ; 00454C9B _ 51
	*/
	*(LPWORD )0x00454C92 = BSWAP16(    0x0605);
	*(LPDWORD)0x00454C94 =         0x00000168 ;
	*(LPDWORD)0x00454C98 = BSWAP32(0x8BCC5051);

	// TFindNameForm::TFindNameForm
	*(LPBYTE )0x0048397C = CALL_REL32;
	*(LPDWORD)0x0048397D = (DWORD)TFindNameForm_ctor - (0x0048397D + sizeof(DWORD));
	*(LPBYTE )0x00483981 = NOP;

	// TGuideForm::TGuideForm
	*(LPDWORD)(0x0048C23D + 1) = (DWORD)TGuideForm_ctor - (0x0048C23D + 1 + sizeof(DWORD));

	// TGuideForm::UpdateUserModeMenu
	*(LPBYTE )0x0048D172 = JMP_REL32;
	*(LPDWORD)0x0048D173 = (DWORD)TGuideForm_UpdateUserModeMenu - (0x0048D173 + sizeof(DWORD));
	*(LPBYTE )0x0048D177 = NOP;

	// TSearchForm::TSearchForm
	/*
		call    TSearchForm_ctor                        ; 004919AF _ E8, ????????
		jmp     004919B8H                               ; 004919B4 _ EB, 02
	*/
	*(LPBYTE )0x004919AF = CALL_REL32;
	*(LPDWORD)0x004919B0 = (DWORD)TSearchForm_ctor - (0x004919B0 + sizeof(DWORD));
	*(LPDWORD)0x004919B4 = BSWAP32(0xEB029090);
	*(LPWORD )0x004919B6 = NOP_X2;

	// TMainForm::M_CustomizeClick
	/*
		test    eax, eax                                ; 0044B24F _ 85. C0
		jnz     0044B258H                               ; 0044B251 _ 75, 05
		jmp     0044B131H                               ; 0044B253 _ E9, FFFFFED9
	*/
	*(LPBYTE )0x0044B24F =               0x85 ;
	*(LPDWORD)0x0044B250 = BSWAP32(0xC07505E9);
	*(LPDWORD)0x0044B254 =         0xFFFFFED9 ;

	// TMainForm::M_CustomizeClick
	/*
		mov     eax, edx                                ; 0044B26C _ 8B. C2
		mov     edx, 3                                  ; 0044B26E _ BA, 00000003
		call    dword ptr [ecx - 4]                     ; 0044B273 _ FF. 51, FC
		mov     word ptr [edi + 16], 332                ; 0044B276 _ 66: C7. 47, 10, 014C
		jmp     0044B131H                               ; 0044B27C _ E9, FFFFFEB0
		nop                                             ; 0044B281 _ 90
	*/
	*(LPDWORD)0x0044B26C = BSWAP32(0x8BC2BA03);
	*(LPDWORD)0x0044B270 = BSWAP32(0x000000FF);
	*(LPDWORD)0x0044B274 = BSWAP32(0x51FC66C7);
	*(LPDWORD)0x0044B278 = BSWAP32(0x47104C01);
	*(LPDWORD)0x0044B27C = BSWAP32(0xE9B0FEFF);
	*(LPWORD )0x0044B280 = BSWAP16(0xFF90    );

	// TMainForm::M_CustomizeClick
	*(LPBYTE )(0x0044B2E4 + 1) = 0x2F;
	*(LPBYTE )0x0044B30D = JMP_REL32;
	*(LPDWORD)0x0044B30E = (DWORD)TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage - (0x0044B30E + sizeof(DWORD));
	*(LPWORD )0x0044B312 = ADD_ESP;
	*(LPBYTE )0x0044B314 = 0x10;
	*(LPWORD )0x0044B315 = MOV_ECX_EBX;
	*(LPBYTE )0x0044B317 = CALL_REL32;
	*(LPDWORD)0x0044B318 = (DWORD)TMainForm_M_CustomizeClick_RedrawCalcImage - (0x0044B318 + sizeof(DWORD));

	// TMainForm::M_CustomizeClick
	*(LPBYTE )0x0044B327 = JMP_REL8;
	*(LPBYTE )0x0044B328 = 0x0D;

	// TSSGActionListner::OnProcessOpen
	*(LPBYTE )0x0052EB4B = JB_REL8;

	// TSSGActionListner::OnSubjectDisabled
	*(LPBYTE )(0x0052EBFD + 2) = 0x03;

	// TSSGActionListner::OnProcessOpenError
	*(LPBYTE )(0x0052EEA0 + 2) = 0x03;

	// TSSGActionListner::OnSubjectReadError
	*(LPBYTE )(0x0052EFB2 + 2) = 0x03;

	// TSSGActionListner::OnSubjectWriteError
	*(LPBYTE )(0x0052F164 + 2) = 0x03;

	// TSSGActionListner::OnSubjectReadSuccess
	*(LPBYTE )(0x0052F345 + 2) = 0x03;

	// TSSGActionListner::OnSubjectWriteSuccess
	*(LPBYTE )(0x0052F6F9 + 2) = 0x03;

	// TSSGActionListner::OnParsingError
	*(LPBYTE )(0x0052F870 + 2) = 0x03;

	// TSSGActionListner::OnParsingDoubleProcess
	*(LPWORD )0x0052FAD4 = JB_REL32;

	// TMainForm::LoadSetting
	*(LPWORD )0x0060294A = BSWAP16('ÉS');
	*(LPDWORD)0x0060294C = BSWAP32('ÉVÉb');
	*(LPDWORD)0x00602950 = BSWAP16('ÉN');
}
