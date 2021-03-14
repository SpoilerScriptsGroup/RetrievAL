#include <windows.h>
#include <dhcpsapi.h>
#include "TFindNameForm.h"
#include "TMainForm.h"
#undef MainForm
#include "intrinsic.h"

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

	// string s = (string)SList->Strings[i].c_str();
	// if (StrSize < (i = TWinControl_GetTextWidth(FindLBox, &s))) StrSize = i;
	/*
		mov     eax, dword ptr [ebp - 580]              ; 0048493A _ 8B. 85, FFFFFDBC
		mov     edx, dword ptr [ebp - 648]              ; 00484940 _ 8B. 95, FFFFFD78
		mov     edi, dword ptr [eax]                    ; 00484946 _ 8B. 38
		lea     ecx, [ebp - 344]                        ; 00484948 _ 8D. 8D, FFFFFEA8
		mov     dword ptr [ecx], 0                      ; 0048494E _ C7. 01, 00000000
		call    dword ptr [edi + 12]                    ; 00484954 _ FF. 57, 0C
		mov     eax, dword ptr [ebp - 344]              ; 00484957 _ 8B. 85, FFFFFEA8
		test    eax, eax                                ; 0048495D _ 85. C0
		jz      004849F4H                               ; 0048495F _ 0F 84, 0000008F
		mov     edx, dword ptr [eax - 4]                ; 00484965 _ 8B. 50, FC
		mov     ecx, dword ptr [esi + 764]              ; 00484968 _ 8B. 8E, 000002FC
		add     edx, eax                                ; 0048496E _ 03. D0
		push    edx                                     ; 00484970 _ 52
		push    eax                                     ; 00484971 _ 50
		mov     edx, esp                                ; 00484972 _ 8B. D4
		call    TWinControl_GetTextWidth                ; 00484974 _ E8, ????????
		mov     ecx, dword ptr [ebp - 512]              ; 00484979 _ 8B. 8D, FFFFFE00
		add     esp, 8                                  ; 0048497F _ 83. C4, 08
		cmp     eax, ecx                                ; 00484982 _ 3B. C1
		jbe     004849F4H                               ; 00484984 _ 76, 6E
		mov     dword ptr [ebp - 512], eax              ; 00484986 _ 89. 85, FFFFFE00
		jmp     004849F4H                               ; 0048498C _ EB, 66
	*/
	*(LPWORD )0x0048493A = BSWAP16(0x8B85);
	*(LPDWORD)0x0048493C = BSWAP32(0xBCFDFFFF);
	*(LPDWORD)0x00484940 = BSWAP32(0x8B9578FD);
	*(LPDWORD)0x00484944 = BSWAP32(0xFFFF8B38);
	*(LPDWORD)0x00484948 = BSWAP32(0x8D8DA8FE);
	*(LPDWORD)0x0048494C = BSWAP32(0xFFFFC701);
	*(LPDWORD)0x00484950 = BSWAP32(0x00000000);
	*(LPDWORD)0x00484954 = BSWAP32(0xFF570C8B);
	*(LPDWORD)0x00484958 = BSWAP32(0x85A8FEFF);
	*(LPDWORD)0x0048495C = BSWAP32(0xFF85C00F);
	*(LPDWORD)0x00484960 = BSWAP32(0x848F0000);
	*(LPDWORD)0x00484964 = BSWAP32(0x008B50FC);
	*(LPDWORD)0x00484968 = BSWAP32(0x8B8EFC02);
	*(LPDWORD)0x0048496C = BSWAP32(0x000003D0);
	*(LPDWORD)0x00484970 = BSWAP32(0x52508BD4);
	*(LPBYTE )0x00484974 = 0xE8;
	*(LPDWORD)0x00484975 = (DWORD)TWinControl_GetTextWidth - (0x00484975 + sizeof(DWORD));
	*(LPBYTE )0x00484979 = 0x8B;
	*(LPWORD )0x0048497A = BSWAP16(0x8D00);
	*(LPDWORD)0x0048497C = BSWAP32(0xFEFFFF83);
	*(LPDWORD)0x00484980 = BSWAP32(0xC4083BC1);
	*(LPDWORD)0x00484984 = BSWAP32(0x766E8985);
	*(LPDWORD)0x00484988 = BSWAP32(0x00FEFFFF);
	*(LPDWORD)0x0048498C = BSWAP32(0xEB669090);

	// findMode = 0;
	// SendMessageA(FindLBox->Handle, LB_SETHORIZONTALEXTENT, StrSize + 20, 0);
	/*
		xor     edx, edx                                ; 00484E01 _ 33. D2
		mov     ecx, dword ptr [ebp - 512]              ; 00484E03 _ 8B. 8D, FFFFFE00
		mov     eax, dword ptr [esi + 764]              ; 00484E09 _ 8B. 86, 000002FC
		add     ecx, 20                                 ; 00484E0F _ 83. C1, 14
		mov     dword ptr [esi + 784], edx              ; 00484E12 _ 89. 96, 00000310
		push    edx                                     ; 00484E18 _ 52
		push    ecx                                     ; 00484E19 _ 51
		push    LB_SETHORIZONTALEXTENT                  ; 00484E1A _ 68, 00000194
		call    0058750CH (TWinControl_GetHandle)       ; 00484E1F _ E8, 001026E8
		push    eax                                     ; 00484E24 _ 50
		call    SendMessageA                            ; 00484E25 _ FF. 15, 00654E60(d)
		jmp     00484E76H                               ; 00484E2B _ EB, 49
	*/
	*(LPDWORD)0x00484E00 = BSWAP32(0xFF33D28B);
	*(LPDWORD)0x00484E04 = BSWAP32(0x8D00FEFF);
	*(LPDWORD)0x00484E08 = BSWAP32(0xFF8B86FC);
	*(LPDWORD)0x00484E0C = BSWAP32(0x02000083);
	*(LPDWORD)0x00484E10 = BSWAP32(0xC1148996);
	*(LPDWORD)0x00484E14 = BSWAP32(0x10030000);
	*(LPDWORD)0x00484E18 = BSWAP32(0x52516894);
	*(LPDWORD)0x00484E1C = BSWAP32(0x010000E8);
	*(LPDWORD)0x00484E20 = BSWAP32(0xE8261000);
	*(LPDWORD)0x00484E24 = BSWAP32(0x50FF1560);
	*(LPDWORD)0x00484E28 = BSWAP32(0x4E6500EB);
	*(LPDWORD)0x00484E2C = BSWAP32(0x49909090);
	*(LPWORD )0x00484E30 = BSWAP16(0x9090);

	// TFindNameForm::EnumSubjectNameFind
	// if (StrSize < (i = TWinControl_GetTextWidth(FindLBox, &Name))) StrSize = i;
	/*
		mov     ecx, dword ptr [esi + 764]              ; 00485A04 _ 8B. 8E, 000002FC
		lea     edx, [ebp - 24]                         ; 00485A0A _ 8D. 55, E8
		call    TWinControl_GetTextWidth                ; 00485A0D _ E8, ????????
		mov     ecx, dword ptr [ebp + 24]               ; 00485A12 _ 8B. 4D, 18
		cmp     eax, dword ptr [ecx]                    ; 00485A15 _ 3B. 01
		jbe     00485A1CH                               ; 00485A17 _ 76, 03
		mov     dword ptr [ecx], eax                    ; 00485A19 _ 89. 01
		nop                                             ; 00485A1B _ 90
	*/
	*(LPDWORD)0x00485A04 = BSWAP32(0x8B8EFC02);
	*(LPDWORD)0x00485A08 = BSWAP32(0x00008D55);
	*(LPWORD )0x00485A0C = BSWAP16(0xE8E8);
	*(LPDWORD)0x00485A0E = (DWORD)TWinControl_GetTextWidth - (0x00485A0E + sizeof(DWORD));
	*(LPWORD )0x00485A12 = BSWAP16(0x8B4D);
	*(LPDWORD)0x00485A14 = BSWAP32(0x183B0176);
	*(LPDWORD)0x00485A18 = BSWAP32(0x03890190);

	// TFindNameForm::FindLBoxClick
	*(LPBYTE )0x0048682D = PUSH_EBX;
	*(LPBYTE )0x0048682E = CALL_REL32;
	*(LPDWORD)0x0048682F = (DWORD)TFindNameForm_FindLBoxClick_ListLBox_set_TopIndex - (0x0048682F + sizeof(DWORD));
}
