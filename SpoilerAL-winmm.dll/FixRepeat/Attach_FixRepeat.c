#include <windows.h>
#include "TSSGCtrl.h"

unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, ...);
static unsigned long(__cdecl * const TStringDivision_ToULongDef)(const bcb6_std_string* Src, unsigned long Default) = (void*)0x004AE6C0;
static void __fastcall TSSGCtrl_ReadSSRFile_Parsing(TSSGCtrl* const SSGC,
													const bcb6_std_vector_string* const tmpV,
													unsigned long* const Begin,
													unsigned long* const End,
													unsigned long* const Step) {
	bcb6_std_string* end = &bcb6_std_vector_type_at(tmpV, bcb6_std_string, 2);
	if (TSSGCtrl_GetSSGActionListner(SSGC)) {
		static TSSGSubject SSGS = { (void*)0x00617C20 };
		HANDLE hProcess;
		if (hProcess = TSSGCtrl_Open(SSGC, &SSGS, PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION)) {
			*Begin = Parsing(SSGC, &SSGS, &bcb6_std_vector_type_at(tmpV, bcb6_std_string, 1), 0);
			*End   = Parsing(SSGC, &SSGS, &bcb6_std_vector_type_at(tmpV, bcb6_std_string, 2), 0);
			*Step  = Parsing(SSGC, &SSGS, &bcb6_std_vector_type_at(tmpV, bcb6_std_string, 3), 0);
			CloseHandle(hProcess);
			if (!*Step) *Step = 1;
		} else {
			*Begin = 0;
			*End   = 0;
			*Step  = 1;
		}
	} else {
		*Begin = TStringDivision_ToULongDef(&bcb6_std_vector_type_at(tmpV, bcb6_std_string, 1), 0);
		*End   = TStringDivision_ToULongDef(&bcb6_std_vector_type_at(tmpV, bcb6_std_string, 2), 0);
		*Step  = TStringDivision_ToULongDef(&bcb6_std_vector_type_at(tmpV, bcb6_std_string, 3), 1);
	}
	if (bcb6_std_string_empty(end) ||
		bcb6_std_string_length(end) == 1 && bcb6_std_string_at(end, 0) == '_') {
		*End = *Begin;
		*Begin = 0;
	}
}

EXTERN_C BOOL __cdecl TSSGCtrl_ReadSSRFile_CheckSignedParam();
EXTERN_C unsigned long __fastcall TSSGCtrl_ReadSSRFile_DestReserve(BOOL);
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_CompareLoopCounter();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSR_SwitchTmpS_0();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_FixWordRepeat();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_Format();

#define CALL_REL32  (BYTE)0xE8
#define PUSH_EAX    (BYTE)0x50
#define JMP_REL8    (BYTE)0xEB
#define POP_ECX     (BYTE)0x59
#define POP_EBX     (BYTE)0x5B
#define POP_EBP     (BYTE)0x5D
#define POP_ESI     (BYTE)0x5E
#define POP_EDI     (BYTE)0x5F
#define MOV_ESP_EBP (WORD)0xE58B
#define NOP         (BYTE)0x90
#define NOP_X2      (WORD)0x9090
#define RET         (BYTE)0xC3
#define JMP_REL32   (BYTE)0xE9

EXTERN_C void __cdecl Attach_FixRepeat()
{
	//*(LPBYTE )0x004FEB85 = 12;// etTRIM
	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FEBCA = CALL_REL32;
	*(LPDWORD)0x004FEBCB = (DWORD)TSSGCtrl_ReadSSRFile_CheckSignedParam - (0x004FEBCB + sizeof(DWORD));
	*(LPBYTE )0x004FEBCF = PUSH_EAX;

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF123 =       0x8D;
	*(LPDWORD)0x004FF124 = 0xFFFF0885;// lea  eax,[ebp - 0xF8]; push eax
	*(LPDWORD)0x004FF128 = 0x408D50FF;// lea  eax,[eax + 0x04]; push eax
	*(LPDWORD)0x004FF12C = 0x408D5004;// lea  eax,[eax + 0x04]; push eax
	*(LPDWORD)0x004FF130 = 0xD68B5004;// mov  edx, esi
	*(LPWORD )0x004FF134 =     0xCF8B;// mov  ecx, edi
	*(LPBYTE )0x004FF136 = CALL_REL32;// call TSSGCtrl_ReadSSRFile_Parsing
	*(LPDWORD)0x004FF137 = (DWORD)TSSGCtrl_ReadSSRFile_Parsing - (0x004FF137 + sizeof(DWORD));
	*(LPBYTE )0x004FF13B =   JMP_REL8;// jmp  0x004FF165
	*(LPBYTE )0x004FF13C = 0x004FF165 - (0x004FF13C + sizeof(BYTE));

	// TSSGCtrl::ReadSSRFile
	*(LPDWORD)0x004FF2B8 = 0xE8240C8B;// mov ecx,[esp]; call ...
	*(LPDWORD)0x004FF2BC = (DWORD)TSSGCtrl_ReadSSRFile_DestReserve - (0x004FF2BC + sizeof(DWORD));

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF2E5 = JMP_REL32;
	*(LPDWORD)0x004FF2E6 = (DWORD)TSSGCtrl_ReadSSRFile_CompareLoopCounter - (0x004FF2E6 + sizeof(DWORD));
	*(LPBYTE )0x004FF2EA = NOP;

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF309 = JMP_REL32;
	*(LPDWORD)0x004FF30A = (DWORD)TSSGCtrl_ReadSSRFile_CompareLoopCounter - (0x004FF30A + sizeof(DWORD));
	*(LPBYTE )0x004FF30E = NOP;

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF5E0 = POP_ECX;
	*(LPBYTE )0x004FF5E1 = POP_EDI;
	*(LPBYTE )0x004FF5E2 = POP_ESI;
	*(LPBYTE )0x004FF5E3 = POP_EBX;
	*(LPWORD )0x004FF5E4 = MOV_ESP_EBP;
	*(LPBYTE )0x004FF5E6 = POP_EBP;
	*(LPBYTE )0x004FF5E7 = RET;

	// TSSGCtrl::EnumReadSSR
	*(LPBYTE )0x004FFDBD = JMP_REL32;
	*(LPDWORD)0x004FFDBE = (DWORD)TSSGCtrl_EnumReadSSR_SwitchTmpS_0 - (0x004FFDBE + sizeof(DWORD));
	*(LPWORD )0x004FFDC2 = NOP_X2;
	*(LPBYTE )0x004FFDC4 = NOP;

	// TSSGCtrl::LoopSSRFile
	*(LPBYTE )0x0050221D = JMP_REL32;
	*(LPDWORD)0x0050221E = (DWORD)TSSGCtrl_LoopSSRFile_FixWordRepeat - (0x0050221E + sizeof(DWORD));
	*(LPBYTE )0x00502222 = NOP;

	// TSSGCtrl::LoopSSRFile
	*(LPBYTE )0x00502676 = JMP_REL32;
	*(LPDWORD)0x00502677 = (DWORD)TSSGCtrl_LoopSSRFile_Format - (0x00502677 + sizeof(DWORD));
	*(LPBYTE )0x0050267B = NOP;
}
