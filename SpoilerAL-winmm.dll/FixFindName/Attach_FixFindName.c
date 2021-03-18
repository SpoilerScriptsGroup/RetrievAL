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
		mov     eax, dword ptr [ebp - 580]              ; 0048493A _ 8B. 85, FFFFFDBC
		mov     edx, dword ptr [ebp - 648]              ; 00484940 _ 8B. 95, FFFFFD78
		mov     dword ptr [ebp - 344], 0                ; 00484946 _ C7. 85, FFFFFEA8, 00000000
		lea     ecx, [ebp - 344]                        ; 00484950 _ 8D. 8D, FFFFFEA8
		call    dword ptr [edi + 12]                    ; 00484956 _ FF. 57, 0C
		mov     eax, dword ptr [esi + 764]              ; 00484959 _ 8B. 86, 000002FC
		lea     edx, [ebp - 344]                        ; 0048495F _ 8D. 95, FFFFFEA8
		mov     eax, dword ptr [eax + 552]              ; 00484965 _ 8B. 80, 00000228
		mov     ecx, dword ptr [edx]                    ; 0048496B _ 8B. 0A
		test    ecx, ecx                                ; 0048496D _ 85. C9
		jz      004849F4H                               ; 0048496F _ 0F 84, 0000007F
		call    0055E588H   ; (TCanvas_TextWidth)       ; 00484975 _ E8, 000D9C0E
		cmp     dword ptr [ebp - 512], eax              ; 0048497A _ 39. 85, FFFFFE00
		jae     004849F4H                               ; 00484980 _ 73, 72
		mov     dword ptr [ebp - 512], eax              ; 00484982 _ 89. 85, FFFFFE00
		jmp     004849F4H                               ; 00484988 _ EB, 6A
	*/
	*(LPWORD )0x0048493A = BSWAP16(0x8B85);
	*(LPDWORD)0x0048493C = BSWAP32(0xBCFDFFFF);
	*(LPDWORD)0x00484940 = BSWAP32(0x8B9578FD);
	*(LPDWORD)0x00484944 = BSWAP32(0xFFFFC785);
	*(LPDWORD)0x00484948 = BSWAP32(0xA8FEFFFF);
	*(LPDWORD)0x0048494C = BSWAP32(0x00000000);
	*(LPDWORD)0x00484950 = BSWAP32(0x8D8DA8FE);
	*(LPDWORD)0x00484954 = BSWAP32(0xFFFFFF57);
	*(LPDWORD)0x00484958 = BSWAP32(0x0C8B86FC);
	*(LPDWORD)0x0048495C = BSWAP32(0x0200008D);
	*(LPDWORD)0x00484960 = BSWAP32(0x95A8FEFF);
	*(LPDWORD)0x00484964 = BSWAP32(0xFF8B8028);
	*(LPDWORD)0x00484968 = BSWAP32(0x0200008B);
	*(LPDWORD)0x0048496C = BSWAP32(0x0A85C90F);
	*(LPDWORD)0x00484970 = BSWAP32(0x847F0000);
	*(LPDWORD)0x00484974 = BSWAP32(0x00E80E9C);
	*(LPDWORD)0x00484978 = BSWAP32(0x0D003985);
	*(LPDWORD)0x0048497C = BSWAP32(0x00FEFFFF);
	*(LPDWORD)0x00484980 = BSWAP32(0x73728985);
	*(LPDWORD)0x00484984 = BSWAP32(0x00FEFFFF);
	*(LPWORD )0x00484988 = BSWAP16(0xEB6A);

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
		mov     edx, dword ptr [ebp - 24]               ; 00485A0A _ 8B. 55, E8
		mov     ecx, dword ptr [ecx + 552]              ; 00485A0D _ 8B. 89, 00000228
		call    TCanvas_TextWidth                       ; 00485A13 _ E8, ????????
		mov     ecx, dword ptr [ebp + 24]               ; 00485A18 _ 8B. 4D, 18
		cmp     dword ptr [ecx], eax                    ; 00485A1B _ 39. 01
		jae     00485A40H                               ; 00485A1D _ 73, 21
		mov     dword ptr [ecx], eax                    ; 00485A1F _ 89. 01
		jmp     00485A40H                               ; 00485A21 _ EB, 1D
	*/
	*(LPDWORD)0x00485A04 = BSWAP32(0x8B8EFC02);
	*(LPDWORD)0x00485A08 = BSWAP32(0x00008B55);
	*(LPDWORD)0x00485A0C = BSWAP32(0xE88B8928);
	*(LPDWORD)0x00485A10 = BSWAP32(0x020000E8);
	*(LPDWORD)0x00485A14 = (DWORD)TCanvas_TextWidth - (0x00485A14 + sizeof(DWORD));
	*(LPDWORD)0x00485A18 = BSWAP32(0x8B4D1839);
	*(LPDWORD)0x00485A1C = BSWAP32(0x01732189);
	*(LPDWORD)0x00485A20 = BSWAP32(0x01EB1D90);
	*(LPBYTE )0x00485A24 = 0x90;

	// TFindNameForm::FindLBoxClick
	*(LPBYTE )0x0048682D = PUSH_EBX;
	*(LPBYTE )0x0048682E = CALL_REL32;
	*(LPDWORD)0x0048682F = (DWORD)TFindNameForm_FindLBoxClick_ListLBox_set_TopIndex - (0x0048682F + sizeof(DWORD));
}
