#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"
#include "TTitleSelectForm.h"
#include "TFindNameForm.h"

EXTERN_C int __fastcall GetTextWidth(TWinControl *WinControl, const string *s);
EXTERN_C void __cdecl TTitleSelectForm_FormCreate();
EXTERN_C void __cdecl TTitleSelectForm_FormClose();

static void __fastcall TTitleSelectForm_FindSSG_DistractionVec_ctor(vector_string *const DistractionVec)
{
	*DistractionVec = *MainForm->DistractionVec;
}

static void __cdecl TTitleSelectForm_FindSSG_DistractionVec_destroy_aux(
	const string        *const begin,
	const string        *const end,
	const vector_string *const DistractionVec)
{
	*MainForm->DistractionVec = *DistractionVec;
}

#define PUSH_ECX  (BYTE)0x51
#define NOP       (BYTE)0x90
#define NOP_X4   (DWORD)0x00401F0F
#define RET_IMM16 (BYTE)0xC2
#define CALL_REL  (BYTE)0xE8
#define JMP_REL32 (BYTE)0xE9
#define JMP_REL8  (BYTE)0xEB

#define MAKEDWORD(a, b, c, d) (DWORD)MAKELONG(MAKEWORD(a, b), MAKEWORD(c, d))

EXTERN_C void __cdecl Attach_FixTitleSelectForm()
{
	// TTitleSelectForm::FindSSG
	*(LPBYTE )0x004716E9 = 0x83;
	*(LPDWORD)0x004716EC = BSWAP32(0x028D4DE8);
	*(LPBYTE )0x004716F0 = CALL_REL;
	*(LPDWORD)0x004716F1 = (DWORD)TTitleSelectForm_FindSSG_DistractionVec_ctor - (0x004716F1 + sizeof(DWORD));
	//   skip TStringFiler
	*(LPBYTE )0x004725E9 = JMP_REL32;
	*(LPDWORD)0x004725EA = 0;

	*(LPBYTE )0x00472785 = JMP_REL8;
	*(LPBYTE )0x00472786 = (BYTE)(0x00472780 - (0x00472786 + sizeof(BYTE)));
	*(LPDWORD)0x00472787 = NOP_X4;
	//   omit DistractionVec dtor
	*(LPBYTE )0x00472CF7 = 0x8D;
	*(LPBYTE )0x00472CF8 = 0x95;
	*(LPDWORD)0x00472CF9 = -(signed)sizeof(vector_string);
	*(LPDWORD)0x00472D1F = (DWORD)TTitleSelectForm_FindSSG_DistractionVec_destroy_aux - (0x00472D1F + sizeof(DWORD));

	*(LPBYTE )0x00472D4D = JMP_REL8;

	// TTitleSelectForm::SortCmbBoxChange
	*(LPDWORD)0x00473D63 = (DWORD)TTitleSelectForm_SortFunc;

	// if (TitleLength < (i = GetTextWidth(TitleDGrid, &it->GetTitle()))) TitleLength = i;
	/*
		add     esp, 12                                 ; 0047429A _ 83. C4, 0C
		lea     edx, [ebp - 84]                         ; 0047429D _ 8D. 55, AC
		mov     ecx, dword ptr [esi + 824]              ; 004742A0 _ 8B. 8E, 00000338
		call    GetTextWidth                            ; 004742A6 _ E8, ????????
		cmp     eax, dword ptr [ebp - 548]              ; 004742AB _ 3B. 85, FFFFFDDC
		jbe     004742CEH                               ; 004742B1 _ 76, 1B
		mov     dword ptr [ebp - 548], eax              ; 004742B3 _ 89. 85, FFFFFDDC
		jmp     004742CEH                               ; 004742B9 _ EB, 13
	*/
	*(LPWORD )0x0047429A = BSWAP16(0x83C4);
	*(LPDWORD)0x0047429C = BSWAP32(0x0C8D55AC);
	*(LPDWORD)0x004742A0 = BSWAP32(0x8B8E3803);
	*(LPWORD )0x004742A4 = BSWAP16(0x0000);
	*(LPBYTE )0x004742A6 = 0xE8;
	*(LPDWORD)0x004742A7 = (DWORD)GetTextWidth - (0x004742A7 + sizeof(DWORD));
	*(LPBYTE )0x004742AB = 0x3B;
	*(LPDWORD)0x004742AC = BSWAP32(0x85DCFDFF);
	*(LPDWORD)0x004742B0 = BSWAP32(0xFF761B89);
	*(LPDWORD)0x004742B4 = BSWAP32(0x85DCFDFF);
	*(LPDWORD)0x004742B8 = BSWAP32(0xFFEB133B);

	/*
		jmp     004743B8H                               ; 00474306 _ E9, 000000AD
	*/
	*(LPWORD )0x00474306 = BSWAP16(0xE9AD);
	*(LPDWORD)0x00474308 = BSWAP32(0x00000090);

	// if(KeyLength < (i = GetTextWidth(KeyDGrid, &tmpKey))) KeyLength = i;
	/*
		mov     ecx, dword ptr [esi + 828]              ; 00474A28 _ 8B. 8E, 0000033C
		lea     edx, [ebp - 48]                         ; 00474A2E _ 8D. 55, D0
		call    GetTextWidth                            ; 00474A31 _ E8, ????????
		cmp     eax, dword ptr [ebp - 544]              ; 00474A36 _ 3B. 85, FFFFFDE0
		jbe     00474A45H                               ; 00474A3C _ 76, 07
		mov     dword ptr [ebp - 544], eax              ; 00474A3E _ 89. 85, FFFFFDE0
		nop                                             ; 00474A44 _ 90
		lea     eax, [ebp-30H]                          ; 00474A45 _ 8D. 45, D0
		lea     ecx, [eax-54H]                          ; 00474A48 _ 8D. 48, AC
	*/
	*(LPDWORD)0x00474A28 = BSWAP32(0x8B8E3C03);
	*(LPDWORD)0x00474A2C = BSWAP32(0x00008D55);
	*(LPWORD )0x00474A30 = BSWAP16(0xD0E8);
	*(LPDWORD)0x00474A32 = (DWORD)GetTextWidth - (0x00474A32 + sizeof(DWORD));
	*(LPWORD )0x00474A36 = BSWAP16(0x3B85);
	*(LPDWORD)0x00474A38 = BSWAP32(0xE0FDFFFF);
	*(LPDWORD)0x00474A3C = BSWAP32(0x76078985);
	*(LPDWORD)0x00474A40 = BSWAP32(0xE0FDFFFF);
	*(LPDWORD)0x00474A44 = BSWAP32(0x908D45D0);
	*(LPDWORD)0x00474A48 = BSWAP32(0x8D48AC3B);

	// if (TitleLength < (i = GetTextWidth(TitleDGrid, &it->GetTitle()))) TitleLength = i;
	/*
		add     esp, 12                                 ; 00474D07 _ 83. C4, 0C
		lea     edx, [ebp - 264]                        ; 00474D0A _ 8D. 95, FFFFFEF8
		mov     ecx, dword ptr [esi + 824]              ; 00474D10 _ 8B. 8E, 00000338
		call    GetTextWidth                            ; 00474D16 _ E8, ????????
		cmp     eax, dword ptr [ebp - 548]              ; 00474D1B _ 3B. 85, FFFFFDDC
		jbe     00474D3EH                               ; 00474D21 _ 76, 1B
		mov     dword ptr [ebp - 548], eax              ; 00474D23 _ 89. 85, FFFFFDDC
		jmp     00474D3EH                               ; 00474D29 _ EB, 13
	*/
	*(LPBYTE )0x00474D07 = 0x83;
	*(LPDWORD)0x00474D08 = BSWAP32(0xC40C8D95);
	*(LPDWORD)0x00474D0C = BSWAP32(0xF8FEFFFF);
	*(LPDWORD)0x00474D10 = BSWAP32(0x8B8E3803);
	*(LPWORD )0x00474D14 = BSWAP16(0x0000);
	*(LPBYTE )0x00474D16 = 0xE8;
	*(LPDWORD)0x00474D17 = (DWORD)GetTextWidth - (0x00474D17 + sizeof(DWORD));
	*(LPBYTE )0x00474D1B = 0x3B;
	*(LPDWORD)0x00474D1C = BSWAP32(0x85DCFDFF);
	*(LPDWORD)0x00474D20 = BSWAP32(0xFF761B89);
	*(LPDWORD)0x00474D24 = BSWAP32(0x85DCFDFF);
	*(LPDWORD)0x00474D28 = BSWAP32(0xFFEB133B);

	/*
		jmp     00474E29H                               ; 00474D7F _ E9, 000000A5
	*/
	*(LPBYTE )0x00474D7F = 0xE9;
	*(LPDWORD)0x00474D80 = BSWAP32(0xA5000000);
	*(LPBYTE )0x00474D84 = 0x90;

	// KeyDGrid->DefaultColWidth = KeyLength + (KeyLength ? 2 : 0);
	/*
		mov     edx, dword ptr [ebp - 544]              ; 00474EB5 _ 8B. 95, FFFFFDE0
		mov     eax, dword ptr [esi + 828]              ; 00474EBB _ 8B. 86, 0000033C
		test    edx, edx                                ; 00474EC1 _ 85. D2
		jz      00474EC8H                               ; 00474EC3 _ 74, 03
		add     edx, 2                                  ; 00474EC5 _ 83. C2, 02
		call    0055902CH                               ; 00474EC8 _ E8, 000E415F
		jmp     00474F07H                               ; 00474ECD _ EB, 38
	*/
	*(LPDWORD)0x00474EB4 = BSWAP32(0x018B95E0);
	*(LPDWORD)0x00474EB8 = BSWAP32(0xFDFFFF8B);
	*(LPDWORD)0x00474EBC = BSWAP32(0x863C0300);
	*(LPDWORD)0x00474EC0 = BSWAP32(0x0085D274);
	*(LPDWORD)0x00474EC4 = BSWAP32(0x0383C202);
	*(LPDWORD)0x00474EC8 = BSWAP32(0xE85F410E);
	*(LPDWORD)0x00474ECC = BSWAP32(0x00EB3890);

	// TitleDGrid->DefaultColWidth = TitleLength + (TitleLength ? 2 : 0);
	/*
		mov     edx, dword ptr [ebp - 548]              ; 00474F41 _ 8B. 95, FFFFFDDC
		mov     eax, dword ptr [esi + 824]              ; 00474F47 _ 8B. 86, 00000338
		test    edx, edx                                ; 00474F4D _ 85. D2
		jz      00474F54H                               ; 00474F4F _ 74, 03
		add     edx, 2                                  ; 00474F51 _ 83. C2, 02
		call    0055902CH                               ; 00474F54 _ E8, 000E40D3
		jmp     00474F93H                               ; 00474F59 _ EB, 38
	*/
	*(LPDWORD)0x00474F40 = BSWAP32(0x018B95DC);
	*(LPDWORD)0x00474F44 = BSWAP32(0xFDFFFF8B);
	*(LPDWORD)0x00474F48 = BSWAP32(0x86380300);
	*(LPDWORD)0x00474F4C = BSWAP32(0x0085D274);
	*(LPDWORD)0x00474F50 = BSWAP32(0x0383C202);
	*(LPDWORD)0x00474F54 = BSWAP32(0xE8D3400E);
	*(LPDWORD)0x00474F58 = BSWAP32(0x00EB3890);

	// TTitleSelectForm::CnvString
	*(LPBYTE )0x00476940 = JMP_REL32;
	*(LPDWORD)0x00476941 = (DWORD)TTitleSelectForm_CnvString - (0x00476941 + sizeof(DWORD));
	*(LPBYTE )0x00476945 = NOP;

	// TTitleSelectForm::FindEditChange
	// if (StrSize < (i = GetTextWidth(FindLBox, &toScriptVec->at(i)->GetTitle()))) StrSize = i;
	/*
		add     esp, 12                                 ; 00477C10 _ 83. C4, 0C
		lea     edx, [ebp - 208]                        ; 00477C13 _ 8D. 95, FFFFFF30
		mov     ecx, dword ptr [ebx + 816]              ; 00477C19 _ 8B. 8B, 00000330
		call    GetTextWidth                            ; 00477C1F _ E8, ????????
		cmp     eax, dword ptr [ebp - 408]              ; 00477C24 _ 3B. 85, FFFFFE68
		jbe     00477C47H                               ; 00477C2A _ 76, 1B
		mov     dword ptr [ebp - 408], eax              ; 00477C2C _ 89. 85, FFFFFE68
		jmp     00477C47H                               ; 00477C32 _ EB, 13
	*/
	*(LPDWORD)0x00477C10 = BSWAP32(0x83C40C8D);
	*(LPDWORD)0x00477C14 = BSWAP32(0x9530FFFF);
	*(LPDWORD)0x00477C18 = BSWAP32(0xFF8B8B30);
	*(LPDWORD)0x00477C1C = BSWAP32(0x030000E8);
	*(LPDWORD)0x00477C20 = (DWORD)GetTextWidth - (0x00477C20 + sizeof(DWORD));
	*(LPDWORD)0x00477C24 = BSWAP32(0x3B8568FE);
	*(LPDWORD)0x00477C28 = BSWAP32(0xFFFF761B);
	*(LPDWORD)0x00477C2C = BSWAP32(0x898568FE);
	*(LPDWORD)0x00477C30 = BSWAP32(0xFFFFEB13);

	/*
		jmp     00477DC1H                               ; 00477C9F _ E9, 0000011D
	*/
	*(LPBYTE )0x00477C9F = 0xE9;
	*(LPDWORD)0x00477CA0 = BSWAP32(0x1D010000);
	*(LPBYTE )0x00477CA4 = 0x90;

	// findMode = 0;
	// SendMessageA(FindLBox->Handle, LB_SETHORIZONTALEXTENT, StrSize + (StrSize ? 2 : 0), 0);
	/*
		mov     ecx, dword ptr [ebp - 408]              ; 0047801A _ 8B. 8D, FFFFFE68
		mov     eax, dword ptr [ebx + 816]              ; 00478020 _ 8B. 83, 00000330
		test    ecx, ecx                                ; 00478026 _ 85. C9
		jz      0047802DH                               ; 00478028 _ 74, 03
		add     ecx, 2                                  ; 0047802A _ 83. C1, 02
		xor     edx, edx                                ; 0047802D _ 33. D2
		mov     dword ptr [ebx + 888], edx              ; 0047802F _ 89. 93, 00000378
		push    edx                                     ; 00478035 _ 52
		push    ecx                                     ; 00478036 _ 51
		push    LB_SETHORIZONTALEXTENT                  ; 00478037 _ 68, 00000194
		call    0058750CH   ; (TWinControl_GetHandle)   ; 0047803C _ E8, 0010F4CB
		push    eax                                     ; 00478041 _ 50
		call    SendMessageA                            ; 00478042 _ FF. 15, 00654E60(d)
		jmp     0047808FH                               ; 00478048 _ EB, 45
	*/
	*(LPWORD )0x0047801A = BSWAP16(0x8B8D);
	*(LPDWORD)0x0047801C = BSWAP32(0x68FEFFFF);
	*(LPDWORD)0x00478020 = BSWAP32(0x8B833003);
	*(LPDWORD)0x00478024 = BSWAP32(0x000085C9);
	*(LPDWORD)0x00478028 = BSWAP32(0x740383C1);
	*(LPDWORD)0x0047802C = BSWAP32(0x0233D289);
	*(LPDWORD)0x00478030 = BSWAP32(0x93780300);
	*(LPDWORD)0x00478034 = BSWAP32(0x00525168);
	*(LPDWORD)0x00478038 = BSWAP32(0x94010000);
	*(LPDWORD)0x0047803C = BSWAP32(0xE8CBF410);
	*(LPDWORD)0x00478040 = BSWAP32(0x0050FF15);
	*(LPDWORD)0x00478044 = BSWAP32(0x604E6500);
	*(LPDWORD)0x00478048 = BSWAP32(0xEB45908B);

	// TTitleSelectForm::TitleDGridMouseMove
	/*
		jmp     004783F0H                               ; 004783E2 _ EB, 0C
	*/
	*(LPWORD )0x004783E2 = MAKEWORD(JMP_REL8, 0x004783F0 - (0x004783E2 + 2));
	*(LPDWORD)0x004783E4 = MAKEDWORD(NOP, NOP, NOP, NOP);

	// TTitleSelectForm::KeyDGridMouseMove
	/*
		ret     8                                       ; 0047937C _ C2, 0008
	*/
	*(LPDWORD)0x0047937C = RET_IMM16 | (0x0008 << 8) | (PUSH_ECX << 24);

	// TFindNameForm::CnvString
	*(LPBYTE )0x00486528 = JMP_REL32;
	*(LPDWORD)0x00486529 = (DWORD)TFindNameForm_CnvString - (0x00486529 + sizeof(DWORD));
	*(LPBYTE )0x0048652D = NOP;

	// TTitleSelectForm::FormCreate
	*(LPDWORD)0x0061AD08 = (DWORD)TTitleSelectForm_FormCreate;

	// TTitleSelectForm::FormClose
	*(LPDWORD)0x0061AD19 = (DWORD)TTitleSelectForm_FormClose;
}
