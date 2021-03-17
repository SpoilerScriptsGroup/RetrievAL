#include <windows.h>
#include <dhcpsapi.h>
#define USING_NAMESPACE_BCB6_STD
#include "TFindNameForm.h"
#include "TMainForm.h"
#undef MainForm
#include "intrinsic.h"

EXTERN_C int __fastcall GetTextWidth(TWinControl *WinControl, const string *s);
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

	// string s = (string)SList->Strings[i].c_str();
	// if (StrSize < (i = GetTextWidth(FindLBox, &s))) StrSize = i;
	/*
		mov     eax, dword ptr [ebp - 580]              ; 0048493A _ 8B. 85, FFFFFDBC
		mov     edx, dword ptr [ebp - 648]              ; 00484940 _ 8B. 95, FFFFFD78
		mov     dword ptr [ebp - 344], 0                ; 00484946 _ C7. 85, FFFFFEA8, 00000000
		lea     ecx, [ebp - 344]                        ; 00484950 _ 8D. 8D, FFFFFEA8
		call    dword ptr [edi + 12]                    ; 00484956 _ FF. 57, 0C
		mov     eax, dword ptr [ebp - 344]              ; 00484959 _ 8B. 85, FFFFFEA8
		test    eax, eax                                ; 0048495F _ 85. C0
		jz      004849F4H                               ; 00484961 _ 0F 84, 0000008D
		mov     edx, dword ptr [eax - 4]                ; 00484967 _ 8B. 50, FC
		mov     ecx, dword ptr [esi + 764]              ; 0048496A _ 8B. 8E, 000002FC
		add     edx, eax                                ; 00484970 _ 03. D0
		push    edx                                     ; 00484972 _ 52
		push    eax                                     ; 00484973 _ 50
		mov     edx, esp                                ; 00484974 _ 8B. D4
		call    GetTextWidth                            ; 00484976 _ E8, ????????
		mov     ecx, dword ptr [ebp - 512]              ; 0048497B _ 8B. 8D, FFFFFE00
		add     esp, 8                                  ; 00484981 _ 83. C4, 08
		cmp     eax, ecx                                ; 00484984 _ 3B. C1
		jbe     004849F4H                               ; 00484986 _ 76, 6C
		mov     dword ptr [ebp - 512], eax              ; 00484988 _ 89. 85, FFFFFE00
		jmp     004849F4H                               ; 0048498E _ EB, 64
	*/
	*(LPWORD )0x0048493A = BSWAP16(0x8B85);
	*(LPDWORD)0x0048493C = BSWAP32(0xBCFDFFFF);
	*(LPDWORD)0x00484940 = BSWAP32(0x8B9578FD);
	*(LPDWORD)0x00484944 = BSWAP32(0xFFFFC785);
	*(LPDWORD)0x00484948 = BSWAP32(0xA8FEFFFF);
	*(LPDWORD)0x0048494C = BSWAP32(0x00000000);
	*(LPDWORD)0x00484950 = BSWAP32(0x8D8DA8FE);
	*(LPDWORD)0x00484954 = BSWAP32(0xFFFFFF57);
	*(LPDWORD)0x00484958 = BSWAP32(0x0C8B85A8);
	*(LPDWORD)0x0048495C = BSWAP32(0xFEFFFF85);
	*(LPDWORD)0x00484960 = BSWAP32(0xC00F848D);
	*(LPDWORD)0x00484964 = BSWAP32(0x0000008B);
	*(LPDWORD)0x00484968 = BSWAP32(0x50FC8B8E);
	*(LPDWORD)0x0048496C = BSWAP32(0xFC020000);
	*(LPDWORD)0x00484970 = BSWAP32(0x03D05250);
	*(LPWORD )0x00484974 = BSWAP16(0x8BD4);
	*(LPBYTE )0x00484976 = 0xE8;
	*(LPDWORD)0x00484977 = (DWORD)GetTextWidth - (0x00484977 + sizeof(DWORD));
	*(LPBYTE )0x0048497B = 0x8B;
	*(LPDWORD)0x0048497C = BSWAP32(0x8D00FEFF);
	*(LPDWORD)0x00484980 = BSWAP32(0xFF83C408);
	*(LPDWORD)0x00484984 = BSWAP32(0x3BC1766C);
	*(LPDWORD)0x00484988 = BSWAP32(0x898500FE);
	*(LPDWORD)0x0048498C = BSWAP32(0xFFFFEB64);

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
	// if (StrSize < (i = GetTextWidth(FindLBox, &Name))) StrSize = i;
	/*
		mov     ecx, dword ptr [esi + 764]              ; 00485A04 _ 8B. 8E, 000002FC
		lea     edx, [ebp - 24]                         ; 00485A0A _ 8D. 55, E8
		call    GetTextWidth                            ; 00485A0D _ E8, ????????
		mov     ecx, dword ptr [ebp + 24]               ; 00485A12 _ 8B. 4D, 18
		cmp     eax, dword ptr [ecx]                    ; 00485A15 _ 3B. 01
		jbe     00485A1CH                               ; 00485A17 _ 76, 03
		mov     dword ptr [ecx], eax                    ; 00485A19 _ 89. 01
		nop                                             ; 00485A1B _ 90
	*/
	*(LPDWORD)0x00485A04 = BSWAP32(0x8B8EFC02);
	*(LPDWORD)0x00485A08 = BSWAP32(0x00008D55);
	*(LPWORD )0x00485A0C = BSWAP16(0xE8E8);
	*(LPDWORD)0x00485A0E = (DWORD)GetTextWidth - (0x00485A0E + sizeof(DWORD));
	*(LPWORD )0x00485A12 = BSWAP16(0x8B4D);
	*(LPDWORD)0x00485A14 = BSWAP32(0x183B0176);
	*(LPDWORD)0x00485A18 = BSWAP32(0x03890190);

	// TFindNameForm::FindLBoxClick
	*(LPBYTE )0x0048682D = PUSH_EBX;
	*(LPBYTE )0x0048682E = CALL_REL32;
	*(LPDWORD)0x0048682F = (DWORD)TFindNameForm_FindLBoxClick_ListLBox_set_TopIndex - (0x0048682F + sizeof(DWORD));
}
