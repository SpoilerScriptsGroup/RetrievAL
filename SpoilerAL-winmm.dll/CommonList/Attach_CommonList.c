#include <windows.h>

EXTERN_C void __cdecl TMainForm_SubjectAccess_StoreFileNamePrefix();
EXTERN_C void __cdecl TMainForm_SubjectAccess_CorrectListItemText();
EXTERN_C void __cdecl TMainForm_SubjectAccess_ToULongDef();
#define TSSGCtrl_LoopSSRFile_StoreFileNamePrefix TMainForm_SubjectAccess_StoreFileNamePrefix
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSR_SwitchCommonList();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_CommonList();

#define JMP_REL32 (BYTE)0xE9
#define NOP       (BYTE)0x90
#define NOP_X2    (WORD)0x9090

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

	*(LPDWORD)(0x0043A648 + 1) = (DWORD)TMainForm_SubjectAccess_CorrectListItemText - (0x0043A648 + 1 + sizeof(DWORD));

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
	*(LPDWORD)0x004C6798 = 0x8BFFFFFD;
	*(LPDWORD)0x004C679C = 0x52148D00;
	*(LPDWORD)0x004C67A0 = 0xB1D0048B;
	*(LPBYTE )0x004C67A4 = 0x08;
	*(LPDWORD)0x004C67A7 = (DWORD)TMainForm_SubjectAccess_ToULongDef - 0x004C67AB;

	// TSSGCtrl::EnumReadSSR
	*(LPDWORD)(0x004FFDD5 + 1) = (DWORD)TSSGCtrl_EnumReadSSR_SwitchCommonList - (0x004FFDD5 + 1 + sizeof(DWORD));

	// TSSGCtrl::LoopSSRFile
	// Type == ssgCtrl::rtFILE
	/*
		push    0                                   ; 00502091 _ 6A, 00
		sub     esp, 24                             ; 00502093 _ 83. EC, 18
		mov     word ptr [ebx + 10H], 212           ; 00502096 _ 66: C7. 43, 10, 00D4
		lea     eax, [ebp - 0F8H]                   ; 0050209C _ 8D. 85, FFFFFF08
		mov     edx, esp                            ; 005020A2 _ 8B. D4
		push    eax                                 ; 005020A4 _ 50
		inc     dword ptr [ebx + 1CH]               ; 005020A5 _ FF. 43, 1C
		push    00631C0DH                           ; 005020A8 _ 68, 00631C0D(d)
	*/
	*(LPDWORD)0x00502090 = 0x83006A00;
	*(LPDWORD)0x00502094 = 0xC76618EC;
	*(LPDWORD)0x00502098 = 0x00D41043;
	*(LPDWORD)0x0050209C = 0xFF08858D;
	*(LPDWORD)0x005020A0 = 0xD48BFFFF;
	*(LPDWORD)0x005020A4 = 0x1C43FF50;
	*(LPDWORD)0x005020A8 = 0x631C0D68;
	*(LPDWORD)0x005020AC = 0x35E85200;

	*(LPDWORD)(0x005020EA + 1) = (DWORD)TSSGCtrl_LoopSSRFile_StoreFileNamePrefix - (0x005020EA + 1 + sizeof(DWORD));

	/*
		jnz     0050210BH                           ; 005020FF _ 75, 0A
		pop     ecx                                 ; 00502101 _ 59
		jmp     005026B6H                           ; 00502102 _ E9, 000005AF
		nop                                         ; 00502107 _ 90
		nop                                         ; 00502108 _ 90
		nop                                         ; 00502109 _ 90
		nop                                         ; 0050210A _ 90
		mov     eax, dword ptr [ebp - 180H]         ; 0050210B _ 8B. 85, FFFFFE80
		mov     ecx, dword ptr [ebp + 14H]          ; 00502111 _ 8B. 4D, 14
		mov     eax, dword ptr [eax]                ; 00502114 _ 8B. 00
		push    ecx                                 ; 00502116 _ 51
		mov     dword ptr [ebp - 180H], eax         ; 00502117 _ 89. 85, FFFFFE80
		lea     edx, [ebp - 60H]                    ; 0050211D _ 8D. 55, A0
	*/
	*(LPBYTE )0x005020FF = 0x75;
	*(LPDWORD)0x00502100 = 0xAFE9590A;
	*(LPDWORD)0x00502104 = 0x90000005;
	*(LPDWORD)0x00502108 = 0x8B909090;
	*(LPDWORD)0x0050210C = 0xFFFE8085;
	*(LPDWORD)0x00502110 = 0x144D8BFF;
	*(LPDWORD)0x00502114 = 0x8951008B;
	*(LPDWORD)0x00502118 = 0xFFFE8085;
	*(LPDWORD)0x0050211C = 0xA0558DFF;

	*(LPDWORD)(0x005021C7 + 1) = (DWORD)TSSGCtrl_LoopSSRFile_CommonList - (0x005021C7 + 1 + sizeof(DWORD));

	/*
		add     esp, 20                             ; 005021D2 _ 83. C4, 14
	*/
	*(LPBYTE )0x005021D4 = 0x14;
}
