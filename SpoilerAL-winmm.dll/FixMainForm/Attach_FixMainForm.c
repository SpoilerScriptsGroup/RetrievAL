#include <windows.h>
#include "intrinsic.h"

EXTERN_C void __cdecl TMainForm_ctor();
EXTERN_C void __cdecl TMainForm_dtor();
EXTERN_C void __cdecl TMainForm_LoadSetting_ListLBox_Font_SetName();
EXTERN_C void __cdecl TMainForm_LoadSetting_SetUserMode();
EXTERN_C void __cdecl Caller_TMainForm_DGridSelectCell();
EXTERN_C void __cdecl TMainForm_SubjectAccess_FixDirSameChildren();
EXTERN_C void __cdecl TSSGSubject_Write_WithDrawTree();
EXTERN_C void __cdecl TMainForm_HotKeyEditKeyDown_SwitchKey();
EXTERN_C void __cdecl Caller_TMainForm_HotKeyEditKeyDown_Up();
EXTERN_C void __cdecl Caller_TMainForm_HotKeyEditKeyDown_Down();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_DrawHover();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_FixLabelDrawX();
EXTERN_C void __cdecl Caller_TMainForm_DrawTreeCell_FixDefaultColWidth();
EXTERN_C void __cdecl TMainForm_DrawTreeCell_DrawFocusRect();
EXTERN_C void __cdecl Caller_TMainForm_DrawTree();
EXTERN_C void __cdecl TMainForm_DGridMouseMove_DrawTree();
EXTERN_C void __cdecl TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible();
EXTERN_C void __cdecl TMainForm_FormResize_CheckTreeSize();
EXTERN_C void __cdecl Caller_TMainForm_FormMouseWheel();
EXTERN_C void __cdecl TMainForm_CheckTreeSize();
EXTERN_C void __cdecl TMainForm_M_TitleSelectClick_OpenSSG();
EXTERN_C void __cdecl TGuideForm_ctor();
EXTERN_C void __cdecl TGuideForm_UpdateUserModeMenu();
EXTERN_C void __cdecl TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage();
EXTERN_C void __fastcall TMainForm_M_CustomizeClick_RedrawCalcImage(LPCVOID);

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

EXTERN_C void Attach_FixMainForm()
{
	// TMainForm::TMainForm
	*(LPBYTE )0x00401FA8 = JMP_REL32;
	*(LPDWORD)0x00401FA9 = (DWORD)TMainForm_ctor - (0x00401FA9 + sizeof(DWORD));
	*(LPBYTE )0x00401FAD = NOP;

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

	// TMainForm::GoCalcEnter
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
	*(LPDWORD)0x004431F8 = (DWORD)Caller_TMainForm_HotKeyEditKeyDown_Up - (0x004431F8 + sizeof(DWORD));
	*(LPWORD )0x004431FC = NOP_X2;

	// TMainForm::HotKeyEditKeyDown
	*(LPBYTE )0x00443267 = JMP_REL32;
	*(LPDWORD)0x00443268 = (DWORD)Caller_TMainForm_HotKeyEditKeyDown_Down - (0x00443268 + sizeof(DWORD));
	*(LPWORD )0x0044326C = NOP_X2;

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x00444C09 = JMP_REL32;
	*(LPDWORD)0x00444C0A = (DWORD)TMainForm_DrawTreeCell_DrawHover - (0x00444C0A + sizeof(DWORD));
	*(LPDWORD)0x00444C0E = NOP_X4;

	// TMainForm::DrawTreeCell
	*(LPBYTE )0x00444E27 = JMP_REL32;
	*(LPDWORD)0x00444E28 = (DWORD)TMainForm_DrawTreeCell_FixLabelDrawX - (0x00444E28 + sizeof(DWORD));
	*(LPBYTE )0x00444E2C = NOP;

	// TMainForm::DrawTreeCell
	// sub     esp, 8                                  ; 004457F0 _ 83. EC, 08
	// fstp    qword ptr [esp]                         ; 004457F3 _ DD. 1C 24
	// lea     edx, [ebp - 0FCH]                       ; 004457F6 _ 8D. 95, FFFFFF04
	// push    edx                                     ; 004457FC _ 52
	// call    TStringDivision::ToStringDouble         ; 004457FD _ E8, 00069026
	// add     esp, 16                                 ; 00445802 _ 83. C4, 10
	*(LPDWORD)0x004457F0 = 0xDD08EC83;
	*(LPWORD )0x004457F4 = 0x241C;
	*(LPDWORD)0x004457FE = 0x00069026;
	*(LPBYTE )0x00445804 = 0x10;

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445C82 + 1) = (DWORD)Caller_TMainForm_DrawTreeCell_FixDefaultColWidth - (0x00445C82 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445CD9 + 1) = (DWORD)Caller_TMainForm_DrawTreeCell_FixDefaultColWidth - (0x00445CD9 + 1 + sizeof(DWORD));

	// TMainForm::DrawTreeCell
	*(LPDWORD)(0x00445DDE + 1) = (DWORD)Caller_TMainForm_DrawTreeCell_FixDefaultColWidth - (0x00445DDE + 1 + sizeof(DWORD));

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

	// TGuideForm::TGuideForm
	*(LPBYTE )0x0048C242 = JMP_REL32;
	*(LPDWORD)0x0048C243 = (DWORD)TGuideForm_ctor - (0x0048C243 + sizeof(DWORD));

	// TGuideForm::UpdateUserModeMenu
	*(LPBYTE )0x0048D172 = JMP_REL32;
	*(LPDWORD)0x0048D173 = (DWORD)TGuideForm_UpdateUserModeMenu - (0x0048D173 + sizeof(DWORD));
	*(LPBYTE )0x0048D177 = NOP;

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
