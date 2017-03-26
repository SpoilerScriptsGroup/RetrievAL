#include <windows.h>

EXTERN_C void __cdecl TMainForm_SubjectAccess_StoreFileNamePrefix();
EXTERN_C void __cdecl TMainForm_SubjectAccess_CorrectFileName();
EXTERN_C void __cdecl TMainForm_SubjectAccess_ToULongDef();

EXTERN_C void Attach_CommonList()
{
	// TMainForm::SubjectAccess
	// case ssgCtrl::atLONG_INDEX:
	/*
		push    0                                   ; 0043A4ED _ 6A, 00
		sub     esp, 24                             ; 0043A4EF _ 83. EC, 18
		mov     word ptr [esi + 10H], 260           ; 0043A4F2 _ 66: C7. 46, 10, 0104
		lea     ecx, [ebp - 0A8H]                   ; 0043A4F8 _ 8D. 8D, FFFFFF58
		mov     eax, esp                            ; 0043A4FE _ 8B. C4
		push    ecx                                 ; 0043A500 _ 51
		inc     dword ptr [esi + 1CH]               ; 0043A501 _ FF. 46, 1C
		push    00602CD8H                           ; 0043A504 _ 68, 00602CD8(d)
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
		jmp     0043A79AH                           ; 0043A5A2 _ E9, 000001F3
	*/
	*(LPDWORD)0x0043A5A3 = 0x000001F3;

	*(LPDWORD)(0x0043A648 + 1) = (DWORD)TMainForm_SubjectAccess_CorrectFileName - (0x0043A648 + 1 + sizeof(DWORD));

	/*
		sub     dword ptr [esi + 1CH], 3            ; 0043A796 _ 83. 6E, 1C, 03
		pop     ecx                                 ; 0043A79A _ 59
        jmp     0043CBF1H                           ; 0043A79B _ E9, 00002451
		nop                                         ; 0043A7A0 _ 90
		nop                                         ; 0043A7A1 _ 90
		nop                                         ; 0043A7A2 _ 90
		nop                                         ; 0043A7A3 _ 90
	*/
	*(LPWORD )0x0043A796 = 0x6E83;
	*(LPDWORD)0x0043A798 = 0xE959031C;
	*(LPDWORD)0x0043A79C = 0x00002451;
	*(LPDWORD)0x0043A7A0 = 0x90909090;

	// TSSDoubleList::Read
	/*
		mov     eax, dword ptr [eax]                ; 004C43D6 _ 8B. 00
		lea     edx, [edx + edx * 2]                ; 004C43D8 _ 8D. 14 52
		mov     eax, dword ptr [eax + edx * 8]      ; 004C43DB _ 8B. 04 D0
		xor     ecx, ecx                            ; 004C43DE _ 33. C9
		push    eax                                 ; 004C43E0 _ 50
		call    TMainForm_SubjectAccess_ToULongDef  ; 004C43E1 _ E8, <offset TMainForm_SubjectAccess_ToULongDef - 004C43E6>
	*/
	*(LPWORD )0x004C43D6 = 0x008B;
	*(LPDWORD)0x004C43D8 = 0x8B52148D;
	*(LPDWORD)0x004C43DC = 0xC933D004;
	*(LPDWORD)0x004C43E2 = (DWORD)TMainForm_SubjectAccess_ToULongDef - 0x004C43E6;

	// TSSDoubleList::Write
	/*
		mov     eax, dword ptr [eax]                ; 004C54BD _ 8B. 00
		lea     edx, [edx + edx * 2]                ; 004C54BF _ 8D. 14 52
		mov     eax, dword ptr [eax + edx * 8]      ; 004C54C2 _ 8B. 04 D0
		mov     cl, 4                               ; 004C54C5 _ B1, 04
		push    eax                                 ; 004C54C7 _ 50
		call    TMainForm_SubjectAccess_ToULongDef  ; 004C54C8 _ E8, <offset TMainForm_SubjectAccess_ToULongDef - 004C54CD>
	*/
	*(LPDWORD)0x004C54BC = 0x8D008BFF;
	*(LPDWORD)0x004C54C0 = 0x048B5214;
	*(LPDWORD)0x004C54C4 = 0x5004B1D0;
	*(LPDWORD)0x004C54C9 = (DWORD)TMainForm_SubjectAccess_ToULongDef - 0x004C54CD;

	// TSSDoubleList::ToByteCode
	/*
		mov     eax, dword ptr [eax]                ; 004C679B _ 8B. 00
		lea     edx, [edx + edx * 2]                ; 004C679D _ 8D. 14 52
		mov     eax, dword ptr [eax + edx * 8]      ; 004C67A0 _ 8B. 04 D0
		mov     cl, 8                               ; 004C67A3 _ B1, 08
		push    eax                                 ; 004C67A5 _ 50
		call    TMainForm_SubjectAccess_ToULongDef  ; 004C67A6 _ E8, <offset TMainForm_SubjectAccess_ToULongDef - 004C67AB>
	*/
	*(LPBYTE )0x004C679B = 0x8B;
	*(LPDWORD)0x004C679C = 0x52148D00;
	*(LPDWORD)0x004C67A0 = 0xB1D0048B;
	*(LPBYTE )0x004C67A4 = 0x08;
	*(LPDWORD)0x004C67A7 = (DWORD)TMainForm_SubjectAccess_ToULongDef - 0x004C67AB;
}
