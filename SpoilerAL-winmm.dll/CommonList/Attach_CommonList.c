#include <windows.h>

EXTERN_C void __cdecl TMainForm_SubjectAccess_StoreFileNamePrefix();
EXTERN_C void __cdecl TMainForm_SubjectAccess_CorrectFileName();

EXTERN_C void Attach_CommonList()
{
	// TMainForm::SubjectAccess
	// case ssgCtrl::atLONG_INDEX:
	/*
		push    0                               ; 0043A4ED _ 6A, 00
		sub     esp, 24                         ; 0043A4EF _ 83. EC, 18
		mov     word ptr [esi + 10H], 260       ; 0043A4F2 _ 66: C7. 46, 10, 0104
		lea     ecx, [ebp - 0A8H]               ; 0043A4F8 _ 8D. 8D, FFFFFF58
		mov     eax, esp                        ; 0043A4FE _ 8B. C4
		push    ecx                             ; 0043A500 _ 51
		inc     dword ptr [esi + 1CH]           ; 0043A501 _ FF. 46, 1C
		push    00602CD8H                       ; 0043A504 _ 68, 00602CD8(d)
	*/
	*(LPDWORD)0x0043A4EC = 0x83006A00;
	*(LPDWORD)0x0043A4F0 = 0xC76618EC;
	*(LPDWORD)0x0043A4F4 = 0x01041046;
	*(LPDWORD)0x0043A4F8 = 0xFF588D8D;
	*(LPDWORD)0x0043A4FC = 0xC48BFFFF;
	*(LPDWORD)0x0043A500 = 0x1C46FF51;
	*(LPDWORD)0x0043A504 = 0x602CD868;
	*(LPBYTE )0x0043A508 = 0x00;

	*(LPDWORD)(0x0043A54F + 1) = (DWORD)TMainForm_SubjectAccess_StoreFileNamePrefix - (0x0043A54F + 1 + sizeof(DWORD));

	/*
		jmp     0043CA53H                       ; 0043A5A2 _ E9, 000024AC
	*/
	*(LPDWORD)0x0043A5A3 = 0x000024AC;

	*(LPDWORD)(0x0043A648 + 1) = (DWORD)TMainForm_SubjectAccess_CorrectFileName - (0x0043A648 + 1 + sizeof(DWORD));

	/*
		jmp     0043CA4FH                       ; 0043A796 _ E9, 000022B4
		nop                                     ; 0043A79B _ 90
	*/
	*(LPWORD )0x0043A796 = 0xB4E9;
	*(LPDWORD)0x0043A798 = 0x90000022;

	// TMainForm::SubjectAccess
	// case ssgCtrl::atBOOL_VECTOR:
	/*
		push    0                               ; 0043B145 _ 6A, 00
		sub     esp, 24                         ; 0043B147 _ 83. EC, 18
		mov     word ptr [esi + 10H], 764       ; 0043B14A _ 66: C7. 46, 10, 02FC
		lea     eax, [ebp - 214H]               ; 0043B150 _ 8D. 85, FFFFFDEC
		mov     edx, esp                        ; 0043B156 _ 8B. D4
		push    eax                             ; 0043B158 _ 50
		inc     dword ptr [esi + 1CH]           ; 0043B159 _ FF. 46, 1C
		push    00602D5FH                       ; 0043B15C _ 68, 00602D5F(d)
	*/
	*(LPDWORD)0x0043B144 = 0x83006A00;
	*(LPDWORD)0x0043B148 = 0xC76618EC;
	*(LPDWORD)0x0043B14C = 0x02FC1046;
	*(LPDWORD)0x0043B150 = 0xFDEC858D;
	*(LPDWORD)0x0043B154 = 0xD48BFFFF;
	*(LPDWORD)0x0043B158 = 0x1C46FF50;
	*(LPDWORD)0x0043B15C = 0x602D5F68;
	*(LPBYTE )0x0043B160 = 0x00;

	*(LPDWORD)(0x0043B1A7 + 1) = (DWORD)TMainForm_SubjectAccess_StoreFileNamePrefix - (0x0043B1A7 + 1 + sizeof(DWORD));

	/*
		jmp     0043CA53H                       ; 0043B1FA _ E9, 00001854
	*/
	*(LPDWORD)0x0043B1FB = 0x00001854;

	*(LPDWORD)(0x0043B2A0 + 1) = (DWORD)TMainForm_SubjectAccess_CorrectFileName - (0x0043B2A0 + 1 + sizeof(DWORD));

	/*
		sub     dword ptr [esi + 1CH], 3        ; 0043CA4F _ 83. 6E, 1C, 03
		pop     ecx                             ; 0043CA53 _ 59
        jmp     0043CBF1H                       ; 0043CA54 _ E9, 00000198
		nop                                     ; 0043CA59 _ 90
		nop                                     ; 0043CA5A _ 90
		nop                                     ; 0043CA5B _ 90
		nop                                     ; 0043CA5C _ 90
	*/
	*(LPBYTE )0x0043CA4F = 0x83;
	*(LPDWORD)0x0043CA50 = 0x00001854;
	*(LPDWORD)0x0043CA50 = 0x59031C6E;
	*(LPDWORD)0x0043CA54 = 0x000198E9;
	*(LPDWORD)0x0043CA58 = 0x90909000;
	*(LPBYTE )0x0043CA5C = 0x90;

	// TSSDoubleList::Read
	/*
		mov     eax, dword ptr [eax]            ; 004C43D6 _ 8B. 00
		lea     edx, [edx + edx * 2]            ; 004C43D8 _ 8D. 14 52
		mov     eax, dword ptr [eax + edx * 8]  ; 004C43DB _ 8B. 04 D0
		push    0                               ; 004C43DE _ 6A, 00
		push    eax                             ; 004C43E0 _ 50
		call    strtoul                         ; 004C43E1 _ E8, <offset strtoul - 004C43E6>
		add     esp, 12                         ; 004C43E6 _ 83. C4, 0C
	*/
	*(LPWORD )0x004C43D6 = 0x008B;
	*(LPDWORD)0x004C43D8 = 0x8B52148D;
	*(LPDWORD)0x004C43DC = 0x006AD004;
	*(LPDWORD)0x004C43E2 = (DWORD)strtoul - 0x004C43E6;
	*(LPBYTE )0x004C43E8 = 0x0C;

	// TSSDoubleList::Write
	/*
		mov     eax, dword ptr [eax]            ; 004C54BD _ 8B. 00
		lea     edx, [edx + edx * 2]            ; 004C54BF _ 8D. 14 52
		mov     eax, dword ptr [eax + edx * 8]  ; 004C54C2 _ 8B. 04 D0
		push    0                               ; 004C54C5 _ 6A, 00
		push    eax                             ; 004C54C7 _ 50
		call    strtoul                         ; 004C54C8 _ E8, <offset strtoul - 004C54CD>
		add     esp, 12                         ; 004C54CD _ 83. C4, 0C
	*/
	*(LPDWORD)0x004C54BC = 0x8D008BFF;
	*(LPDWORD)0x004C54C0 = 0x048B5214;
	*(LPDWORD)0x004C54C4 = 0x50006AD0;
	*(LPDWORD)0x004C54C9 = (DWORD)strtoul - 0x004C54CD;
	*(LPBYTE )0x004C54CF = 0x0C;
}
