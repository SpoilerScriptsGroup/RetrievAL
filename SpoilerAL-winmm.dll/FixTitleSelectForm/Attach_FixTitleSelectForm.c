#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"
#include "TTitleSelectForm.h"
#include "TFindNameForm.h"

EXTERN_C void __cdecl TTitleSelectForm_FormCreate();

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

	// if ((i = TitleDGrid->Canvas->TextWidth(it->GetTitle().c_str())) && TitleLength < (i += 18)) TitleLength = i;
	/*
		mov     ecx, dword ptr [esi + 824]              ; 0047429A _ 8B. 8E, 00000338
		add     esp, 12                                 ; 004742A0 _ 83. C4, 0C
		mov     ecx, dword ptr [ecx + 520]              ; 004742A3 _ 8B. 89, 00000208
		lea     edx, [ebp - 84]                         ; 004742A9 _ 8D. 55, AC
		call    TCanvas_TextWidth_std_string            ; 004742AC _ E8, ????????
		test    eax, eax                                ; 004742B1 _ 85. C0
		jz      004742CEH                               ; 004742B3 _ 74, 19
		mov     ecx, dword ptr [ebp - 548]              ; 004742B5 _ 8B. 8D, FFFFFDDC
		add     eax, 18                                 ; 004742BB _ 83. C0, 12
		cmp     ecx, eax                                ; 004742BE _ 3B. C8
		jae     004742CEH                               ; 004742C0 _ 73, 0C
		mov     dword ptr [ebp - 548], eax              ; 004742C2 _ 89. 85, FFFFFDDC
		jmp     004742CEH                               ; 004742C8 _ EB, 04
	*/
	*(LPWORD )0x0047429A = BSWAP16(0x8B8E);
	*(LPDWORD)0x0047429C = BSWAP32(0x38030000);
	*(LPDWORD)0x004742A0 = BSWAP32(0x83C40C8B);
	*(LPDWORD)0x004742A4 = BSWAP32(0x89080200);
	*(LPDWORD)0x004742A8 = BSWAP32(0x008D55AC);
	*(LPBYTE )0x004742AC = 0xE8;
	*(LPDWORD)0x004742AD = (DWORD)TCanvas_TextWidth_std_string - (0x004742AD + sizeof(DWORD));
	*(LPBYTE )0x004742B1 = 0x85;
	*(LPWORD )0x004742B2 = BSWAP16(0xC074);
	*(LPDWORD)0x004742B4 = BSWAP32(0x198B8DDC);
	*(LPDWORD)0x004742B8 = BSWAP32(0xFDFFFF83);
	*(LPDWORD)0x004742BC = BSWAP32(0xC0123BC8);
	*(LPDWORD)0x004742C0 = BSWAP32(0x730C8985);
	*(LPDWORD)0x004742C4 = BSWAP32(0xDCFDFFFF);
	*(LPDWORD)0x004742C8 = BSWAP32(0xEB0490FF);

	/*
		jmp     004743B8H                               ; 00474306 _ E9, 000000AD
	*/
	*(LPWORD )0x00474306 = BSWAP16(0xE9AD);
	*(LPDWORD)0x00474308 = BSWAP32(0x00000090);

	// if ((i = KeyDGrid->Canvas->TextWidth(tmpKey.c_str())) && KeyLength < (i += 3)) KeyLength = i;
	/*
		mov     ecx, dword ptr [esi + 828]              ; 00474A28 _ 8B. 8E, 0000033C
		lea     edx, [ebp - 48]                         ; 00474A2E _ 8D. 55, D0
		mov     ecx, dword ptr [ecx + 520]              ; 00474A31 _ 8B. 89, 00000208
		call    TCanvas_TextWidth_std_string            ; 00474A37 _ E8, ????????
		test    eax, eax                                ; 00474A3C _ 85. C0
		jz      00474A4FH                               ; 00474A3E _ 74, 0F
		add     eax, 3                                  ; 00474A40 _ 83. C0, 03
		lea     ecx, [ebp - 544]                        ; 00474A43 _ 8D. 8D, FFFFFDE0
		cmp     dword ptr [ecx], eax                    ; 00474A49 _ 39. 01
		jae     00474A4FH                               ; 00474A4B _ 73, 02
		mov     dword ptr [ecx], eax                    ; 00474A4D _ 89. 01
	*/
	*(LPDWORD)0x00474A28 = BSWAP32(0x8B8E3C03);
	*(LPDWORD)0x00474A2C = BSWAP32(0x00008D55);
	*(LPDWORD)0x00474A30 = BSWAP32(0xD08B8908);
	*(LPDWORD)0x00474A34 = BSWAP32(0x020000E8);
	*(LPDWORD)0x00474A38 = (DWORD)TCanvas_TextWidth_std_string - (0x00474A38 + sizeof(DWORD));
	*(LPDWORD)0x00474A3C = BSWAP32(0x85C0740F);
	*(LPDWORD)0x00474A40 = BSWAP32(0x83C0038D);
	*(LPDWORD)0x00474A44 = BSWAP32(0x8DE0FDFF);
	*(LPDWORD)0x00474A48 = BSWAP32(0xFF390173);
	*(LPDWORD)0x00474A4C = BSWAP32(0x0289018B);

	// if ((i = TitleDGrid->Canvas->TextWidth(it->GetTitle().c_str())) && TitleLength < (i += 18)) TitleLength = i;
	/*
		mov     ecx, dword ptr [esi + 824]              ; 00474D07 _ 8B. 8E, 00000338
		add     esp, 12                                 ; 00474D0D _ 83. C4, 0C
		mov     ecx, dword ptr [ecx + 520]              ; 00474D10 _ 8B. 89, 00000208
		lea     edx, [ebp - 264]                        ; 00474D16 _ 8D. 95, FFFFFEF8
		call    TCanvas_TextWidth_std_string            ; 00474D1C _ E8, ????????
		test    eax, eax                                ; 00474D21 _ 85. C0
		jz      00474D3EH                               ; 00474D23 _ 74, 19
		mov     ecx, dword ptr [ebp - 548]              ; 00474D25 _ 8B. 8D, FFFFFDDC
		add     eax, 18                                 ; 00474D2B _ 83. C0, 12
		cmp     ecx, eax                                ; 00474D2E _ 3B. C8
		jae     00474D3EH                               ; 00474D30 _ 73, 0C
		mov     dword ptr [ebp - 548], eax              ; 00474D32 _ 89. 85, FFFFFDDC
		jmp     00474D3EH                               ; 00474D38 _ EB, 04
	*/
	*(LPBYTE )0x00474D07 = 0x8B;
	*(LPDWORD)0x00474D08 = BSWAP32(0x8E380300);
	*(LPDWORD)0x00474D0C = BSWAP32(0x0083C40C);
	*(LPDWORD)0x00474D10 = BSWAP32(0x8B890802);
	*(LPDWORD)0x00474D14 = BSWAP32(0x00008D95);
	*(LPDWORD)0x00474D18 = BSWAP32(0xF8FEFFFF);
	*(LPBYTE )0x00474D1C = 0xE8;
	*(LPDWORD)0x00474D1D = (DWORD)TCanvas_TextWidth_std_string - (0x00474D1D + sizeof(DWORD));
	*(LPBYTE )0x00474D21 = 0x85;
	*(LPWORD )0x00474D22 = BSWAP16(0xC074);
	*(LPDWORD)0x00474D24 = BSWAP32(0x198B8DDC);
	*(LPDWORD)0x00474D28 = BSWAP32(0xFDFFFF83);
	*(LPDWORD)0x00474D2C = BSWAP32(0xC0123BC8);
	*(LPDWORD)0x00474D30 = BSWAP32(0x730C8985);
	*(LPDWORD)0x00474D34 = BSWAP32(0xDCFDFFFF);
	*(LPDWORD)0x00474D38 = BSWAP32(0xEB0490FF);

	/*
		jmp     00474E29H                               ; 00474D7F _ E9, 000000A5
	*/
	*(LPBYTE )0x00474D7F = 0xE9;
	*(LPDWORD)0x00474D80 = BSWAP32(0xA5000000);
	*(LPBYTE )0x00474D84 = 0x90;

	// if (TitleLength < KeyLength) TitleLength = KeyLength;
	/*
		mov     edx, dword ptr [ebp - 544]              ; 00474E42 _ 8B. 95, FFFFFDE0
		mov     eax, dword ptr [ebp - 548]              ; 00474E48 _ 8B. 85, FFFFFDDC
		cmp     eax, edx                                ; 00474E4E _ 3B. C2
		jae     00474E58H                               ; 00474E50 _ 73, 06
		mov     dword ptr [ebp - 548], edx              ; 00474E52 _ 89. 95, FFFFFDDC
		lea     ecx, [ebp - 132]                        ; 00474E58 _ 8D. 8D, FFFFFF7C
		call    string_dtor                             ; 00474E5E _ E8, ????????
		jmp     00474EB5H                               ; 00474E63 _ EB, 50
	*/
	*(LPWORD )0x00474E42 = BSWAP16(0x8B95);
	*(LPDWORD)0x00474E44 = BSWAP32(0xE0FDFFFF);
	*(LPDWORD)0x00474E48 = BSWAP32(0x8B85DCFD);
	*(LPDWORD)0x00474E4C = BSWAP32(0xFFFF3BC2);
	*(LPDWORD)0x00474E50 = BSWAP32(0x73068995);
	*(LPDWORD)0x00474E54 = BSWAP32(0xDCFDFFFF);
	*(LPDWORD)0x00474E58 = BSWAP32(0x8D8D7CFF);
	*(LPWORD )0x00474E5C = BSWAP16(0xFFFF);
	*(LPBYTE )0x00474E5E = 0xE8;
	*(LPDWORD)0x00474E5F = (DWORD)string_dtor - (0x00474E5F + sizeof(DWORD));
	*(LPBYTE )0x00474E63 = 0xEB;
	*(LPDWORD)0x00474E64 = BSWAP32(0x50909090);
	*(LPBYTE )0x00474E68 = 0x90;

	// KeyDGrid->DefaultColWidth = KeyLength;
	/*
		mov     edx, dword ptr [ebp - 544]              ; 00474EAF _ 8B. 95, FFFFFDE0
		mov     eax, dword ptr [esi + 828]              ; 00474EB5 _ 8B. 86, 0000033C
		call    0055902CH                               ; 00474EBB _ E8, 000E416C
		jmp     00474F07H                               ; 00474EC0 _ EB, 45
	*/
	*(LPBYTE )0x00474EAF = 0x8B;
	*(LPDWORD)0x00474EB0 = BSWAP32(0x95E0FDFF);
	*(LPDWORD)0x00474EB4 = BSWAP32(0xFF8B863C);
	*(LPDWORD)0x00474EB8 = BSWAP32(0x030000E8);
	*(LPDWORD)0x00474EBC = BSWAP32(0x6C410E00);
	*(LPDWORD)0x00474EC0 = BSWAP32(0xEB459090);
	*(LPBYTE )0x00474EC4 = 0x90;

	// TitleDGrid->DefaultColWidth = TitleLength;
	/*
		mov     edx, dword ptr [ebp - 548]              ; 00474F3B _ 8B. 95, FFFFFDDC
		mov     eax, dword ptr [esi + 824]              ; 00474F41 _ 8B. 86, 00000338
		call    0055902CH                               ; 00474F47 _ E8, 000E40E0
		jmp     00474F93H                               ; 00474F4C _ EB, 45
	*/
	*(LPBYTE )0x00474F3B = 0x8B;
	*(LPDWORD)0x00474F3C = BSWAP32(0x95DCFDFF);
	*(LPDWORD)0x00474F40 = BSWAP32(0xFF8B8638);
	*(LPDWORD)0x00474F44 = BSWAP32(0x030000E8);
	*(LPDWORD)0x00474F48 = BSWAP32(0xE0400E00);
	*(LPDWORD)0x00474F4C = BSWAP32(0xEB459090);
	*(LPBYTE )0x00474F50 = 0x90;

	// TTitleSelectForm::CnvString
	*(LPBYTE )0x00476940 = JMP_REL32;
	*(LPDWORD)0x00476941 = (DWORD)TTitleSelectForm_CnvString - (0x00476941 + sizeof(DWORD));
	*(LPBYTE )0x00476945 = NOP;

	// TTitleSelectForm::FindEditChange
	// if (StrSize < (i = FindLBox->Canvas->TextWidth(toScriptVec->at(i)->GetTitle().c_str()))) StrSize = i;
	/*
		mov     ecx, dword ptr [ebx + 816]              ; 00477C10 _ 8B. 8B, 00000330
		add     esp, 12                                 ; 00477C16 _ 83. C4, 0C
		mov     ecx, dword ptr [ecx + 552]              ; 00477C19 _ 8B. 89, 00000228
		lea     edx, [ebp - 208]                        ; 00477C1F _ 8D. 95, FFFFFF30
		call    TCanvas_TextWidth_std_string            ; 00477C25 _ E8, ????????
		cmp     dword ptr [ebp - 408], eax              ; 00477C2A _ 39. 85, FFFFFE68
		jae     00477C47H                               ; 00477C30 _ 73, 15
		mov     dword ptr [ebp - 408], eax              ; 00477C32 _ 89. 85, FFFFFE68
		jmp     00477C47H                               ; 00477C38 _ EB, 0D
	*/
	*(LPDWORD)0x00477C10 = BSWAP32(0x8B8B3003);
	*(LPDWORD)0x00477C14 = BSWAP32(0x000083C4);
	*(LPDWORD)0x00477C18 = BSWAP32(0x0C8B8928);
	*(LPDWORD)0x00477C1C = BSWAP32(0x0200008D);
	*(LPDWORD)0x00477C20 = BSWAP32(0x9530FFFF);
	*(LPWORD )0x00477C24 = BSWAP16(0xFFE8);
	*(LPDWORD)0x00477C26 = (DWORD)TCanvas_TextWidth_std_string - (0x00477C26 + sizeof(DWORD));
	*(LPWORD )0x00477C2A = BSWAP16(0x3985);
	*(LPDWORD)0x00477C2C = BSWAP32(0x68FEFFFF);
	*(LPDWORD)0x00477C30 = BSWAP32(0x73158985);
	*(LPDWORD)0x00477C34 = BSWAP32(0x68FEFFFF);
	*(LPWORD )0x00477C38 = BSWAP16(0xEB0D);

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
}
