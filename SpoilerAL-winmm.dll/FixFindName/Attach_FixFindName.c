#include <windows.h>
#include <dhcpsapi.h>
#define USING_NAMESPACE_BCB6_STD
#include "TFindNameForm.h"
#include "TMainForm.h"
#undef MainForm
#include "intrinsic.h"

EXTERN_C TSSGSubject * __fastcall TFindNameForm_FindStartBtnClick_FixGetSelectSubject(TMainForm *);

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

	// if (StrSize < (i = FindLBox->Canvas->TextWidth(SList->Strings[i].c_str()))) StrSize = i;
	/*
		mov     eax, dword ptr [esi + 764]              ; 00484962 _ 8B. 86, 000002FC
		lea     edx, [ebp - 344]                        ; 00484968 _ 8D. 95, FFFFFEA8
		mov     eax, dword ptr [eax + 552]              ; 0048496E _ 8B. 80, 00000228
		mov     ecx, dword ptr [edx]                    ; 00484974 _ 8B. 0A
		test    ecx, ecx                                ; 00484976 _ 85. C9
		jz      0048498DH                               ; 00484978 _ 74, 13
		call    0055E588H   ; (TCanvas_TextWidth)       ; 0048497A _ E8, 000D9C09
		cmp     dword ptr [ebp - 512], eax              ; 0048497F _ 39. 85, FFFFFE00
		jae     0048498DH                               ; 00484985 _ 73, 06
		mov     dword ptr [ebp - 512], eax              ; 00484987 _ 89. 85, FFFFFE00
		mov     edx, 2                                  ; 0048498D _ BA, 00000002
		lea     eax, [ebp - 344]                        ; 00484992 _ 8D. 85, FFFFFEA8
		call    005E0EA8H                               ; 00484998 _ E8, 0015C50B
		jmp     004849F4H                               ; 0048499D _ EB, 55
	*/
	*(LPWORD )0x00484962 = BSWAP16(0x8B86);
	*(LPDWORD)0x00484964 = BSWAP32(0xFC020000);
	*(LPDWORD)0x00484968 = BSWAP32(0x8D95A8FE);
	*(LPDWORD)0x0048496C = BSWAP32(0xFFFF8B80);
	*(LPDWORD)0x00484970 = BSWAP32(0x28020000);
	*(LPDWORD)0x00484974 = BSWAP32(0x8B0A85C9);
	*(LPDWORD)0x00484978 = BSWAP32(0x7413E809);
	*(LPDWORD)0x0048497C = BSWAP32(0x9C0D0039);
	*(LPDWORD)0x00484980 = BSWAP32(0x8500FEFF);
	*(LPDWORD)0x00484984 = BSWAP32(0xFF730689);
	*(LPDWORD)0x00484988 = BSWAP32(0x8500FEFF);
	*(LPDWORD)0x0048498C = BSWAP32(0xFFBA0200);
	*(LPDWORD)0x00484990 = BSWAP32(0x00008D85);
	*(LPDWORD)0x00484994 = BSWAP32(0xA8FEFFFF);
	*(LPDWORD)0x00484998 = BSWAP32(0xE80BC515);
	*(LPDWORD)0x0048499C = BSWAP32(0x00EB5590);
	*(LPDWORD)0x004849A0 = BSWAP32(0x90909033);

	// findMode = 0;
	// SendMessageA(FindLBox->Handle, LB_SETHORIZONTALEXTENT, StrSize + (StrSize ? 2 : 0), 0);
	/*
		mov     ecx, dword ptr [ebp - 512]              ; 00484E01 _ 8B. 8D, FFFFFE00
		mov     eax, dword ptr [esi + 764]              ; 00484E07 _ 8B. 86, 000002FC
		test    ecx, ecx                                ; 00484E0D _ 85. C9
		jz      00484E14H                               ; 00484E0F _ 74, 03
		add     ecx, 2                                  ; 00484E11 _ 83. C1, 02
		xor     edx, edx                                ; 00484E14 _ 33. D2
		mov     dword ptr [esi + 784], edx              ; 00484E16 _ 89. 96, 00000310
		push    edx                                     ; 00484E1C _ 52
		push    ecx                                     ; 00484E1D _ 51
		push    LB_SETHORIZONTALEXTENT                  ; 00484E1E _ 68, 00000194
		call    0058750CH   ; (TWinControl_GetHandle)   ; 00484E23 _ E8, 001026E4
		push    eax                                     ; 00484E28 _ 50
		call    SendMessageA                            ; 00484E29 _ FF. 15, 00654E60(d)
		jmp     00484E76H                               ; 00484E2F _ EB, 45
	*/
	*(LPDWORD)0x00484E00 = BSWAP32(0xFF8B8D00);
	*(LPDWORD)0x00484E04 = BSWAP32(0xFEFFFF8B);
	*(LPDWORD)0x00484E08 = BSWAP32(0x86FC0200);
	*(LPDWORD)0x00484E0C = BSWAP32(0x0085C974);
	*(LPDWORD)0x00484E10 = BSWAP32(0x0383C102);
	*(LPDWORD)0x00484E14 = BSWAP32(0x33D28996);
	*(LPDWORD)0x00484E18 = BSWAP32(0x10030000);
	*(LPDWORD)0x00484E1C = BSWAP32(0x52516894);
	*(LPDWORD)0x00484E20 = BSWAP32(0x010000E8);
	*(LPDWORD)0x00484E24 = BSWAP32(0xE4261000);
	*(LPDWORD)0x00484E28 = BSWAP32(0x50FF1560);
	*(LPDWORD)0x00484E2C = BSWAP32(0x4E6500EB);
	*(LPWORD )0x00484E30 = BSWAP16(0x4590);

	// TFindNameForm::EnumSubjectNameFind
	// if (StrSize < (i = FindLBox->Canvas->TextWidth(Name.c_str()))) StrSize = i;
	/*
		mov     ecx, dword ptr [esi + 764]              ; 00485A04 _ 8B. 86, 000002FC
		lea     edx, [ebp - 24]                         ; 00485A0A _ 8D. 55, E8
		mov     ecx, dword ptr [ecx + 552]              ; 00485A0D _ 8B. 89, 00000228
		call    TCanvas_TextWidth_stdstr                ; 00485A13 _ E8, ????????
		mov     ecx, dword ptr [ebp + 24]               ; 00485A18 _ 8B. 4D, 18
		cmp     dword ptr [ecx], eax                    ; 00485A1B _ 39. 01
		jae     00485A40H                               ; 00485A1D _ 73, 21
		mov     dword ptr [ecx], eax                    ; 00485A1F _ 89. 01
		jmp     00485A40H                               ; 00485A21 _ EB, 1D
	*/
	*(LPDWORD)0x00485A04 = BSWAP32(0x8B8EFC02);
	*(LPDWORD)0x00485A08 = BSWAP32(0x00008D55);
	*(LPDWORD)0x00485A0C = BSWAP32(0xE88B8928);
	*(LPDWORD)0x00485A10 = BSWAP32(0x020000E8);
	*(LPDWORD)0x00485A14 = (DWORD)TCanvas_TextWidth_stdstr - (0x00485A14 + sizeof(DWORD));
	*(LPDWORD)0x00485A18 = BSWAP32(0x8B4D1839);
	*(LPDWORD)0x00485A1C = BSWAP32(0x01732189);
	*(LPDWORD)0x00485A20 = BSWAP32(0x01EB1D90);
	*(LPBYTE )0x00485A24 = 0x90;

	// TFindNameForm::FindLBoxClick
	*(LPBYTE )0x0048682D = PUSH_EBX;
	*(LPBYTE )0x0048682E = CALL_REL32;
	*(LPDWORD)0x0048682F = (DWORD)TFindNameForm_FindLBoxClick_ListLBox_set_TopIndex - (0x0048682F + sizeof(DWORD));
}
