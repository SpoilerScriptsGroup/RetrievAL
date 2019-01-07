#include <windows.h>
#include "TSSGCtrl.h"
#include "TMainForm.h"
#include "TStringDivision.h"

EXTERN_C BOOL __cdecl TSSGCtrl_ReadSSRFile_CheckSignedParam();
EXTERN_C unsigned long __fastcall TSSGCtrl_ReadSSRFile_DestReserve(BOOL);
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_CompareLoopCounter();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSR_SwitchTmpS_0();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_FixWordRepeat();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_Format();

unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, ...);
static unsigned long __fastcall TSSGCtrl_ReadSSRFile_Parsing(
	TSSGCtrl     * const SSGC,
	const bcb6_std_vector_string* const tmpV,
	TSSGSubject  * const SSGS,
	unsigned long* const Begin,
	unsigned long* const End)
{
	unsigned long    step;
	bcb6_std_string* end = &bcb6_std_vector_type_at(tmpV, bcb6_std_string, 2);
	if (!TSSGCtrl_GetSSGActionListner(SSGC)) {
		*Begin = TStringDivision_ToULongDef(&bcb6_std_vector_type_at(tmpV, bcb6_std_string, 1), 0);
		*End   = TStringDivision_ToULongDef(&bcb6_std_vector_type_at(tmpV, bcb6_std_string, 2), 0);
		step   = TStringDivision_ToULongDef(&bcb6_std_vector_type_at(tmpV, bcb6_std_string, 3), 1);
	} else {
		int error;
		SetLastError(NO_ERROR);
		*Begin = Parsing(SSGC, SSGS, &bcb6_std_vector_type_at(tmpV, bcb6_std_string, 1), 0);
		*End   = Parsing(SSGC, SSGS, &bcb6_std_vector_type_at(tmpV, bcb6_std_string, 2), 0);
		step   = Parsing(SSGC, SSGS, &bcb6_std_vector_type_at(tmpV, bcb6_std_string, 3), 0);
		if (!step) step = 1;
		if ((error = GetLastError()) && error != ERROR_NO_MORE_FILES) {
			extern HANDLE hHeap;
			LPSTR lpBuffer;
			*Begin = 0;
			*End   = 0;
			if (FormatMessageA(
				FORMAT_MESSAGE_MAX_WIDTH_MASK |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_ARGUMENT_ARRAY,
				NULL,
				error,
				0,
				(LPSTR)&lpBuffer,
				8,
				NULL)) {
				TMainForm_Guide(lpBuffer, FALSE);
				HeapFree(hHeap, 0, lpBuffer);
			}
		}
	}
	if (bcb6_std_string_empty(end) ||
		bcb6_std_string_length(end) == 1 && bcb6_std_string_at(end, 0) == '_') {
		*End = *Begin;
		*Begin = 0;
	}
	return step;
}

static __declspec(naked) bcb6_std_vector_string* __cdecl TSSGCtrl_ReadSSRFile_GetSSGDataFile(
	TSSGCtrl *this,
	TSSGSubject *SSGS,
	bcb6_std_string FName,
	bcb6_std_string DefaultExt,
	bcb6_std_string *CurrentDir) {
	__asm {
		mov eax, [ebp + 0x18]
		mov [esp + 8], eax// SSGS
		jmp TSSGCtrl_GetSSGDataFile
	}
}

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
#define NOP_X2      (WORD)0x9066
#define RET         (BYTE)0xC3
#define JMP_REL32   (BYTE)0xE9

EXTERN_C void __cdecl Attach_FixRepeat()
{
	// TSSGCtrl::ReadSSRFile
	//*(LPBYTE )0x004FEB85 = 12;// etTRIM
	*(LPBYTE )0x004FEBCA = CALL_REL32;
	*(LPDWORD)0x004FEBCB = (DWORD)TSSGCtrl_ReadSSRFile_CheckSignedParam - (0x004FEBCB + sizeof(DWORD));
	*(LPBYTE )0x004FEBCF = PUSH_EAX;

	// TSSGCtrl::ReadSSRFile
	*(LPDWORD)0x004FED89 = (DWORD)TSSGCtrl_ReadSSRFile_GetSSGDataFile - (0x004FED89 + sizeof(DWORD));
	*(LPDWORD)0x004FF107 = (DWORD)TSSGCtrl_ReadSSRFile_GetSSGDataFile - (0x004FF107 + sizeof(DWORD));

	// TSSGCtrl::ReadSSRFile
	*(LPBYTE )0x004FF123 =         0x8D       ;// lea  eax, [ebp - 0xF4]
	*(LPDWORD)0x004FF124 = BSWAP32(0x850CFFFF);// push eax
	*(LPDWORD)0x004FF128 = BSWAP32(0xFF508D40);// lea  eax, [eax + 0x04]
	*(LPDWORD)0x004FF12C = BSWAP32(0x0450FF75);// push eax
	*(LPWORD )0x004FF130 = BSWAP16(0x18EB    );// push dword ptr [ebp + 0x18]
	*(LPBYTE )0x004FF132 = 0x004FF156 - (0x004FF132 + sizeof(BYTE));
	*(LPWORD )0x004FF156 = BSWAP16(0x8BD6    );// mov  edx, esi; tmpV
	*(LPWORD )0x004FF158 = BSWAP16(0x8BCF    );// mov  ecx, edi; this
	*(LPBYTE )0x004FF15A = CALL_REL32;
	*(LPDWORD)0x004FF15B = (DWORD)TSSGCtrl_ReadSSRFile_Parsing - (0x004FF15B + sizeof(DWORD));

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
	//   File.size()*2 => File.size()
	*(LPWORD )0x004FF9F7 = NOP_X2;

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

	// TSSGCtrl::LoopSSRFile
	//   tmpS=""; => tmpS.clear();
	*(LPWORD )0x0050272A = BSWAP16(0x8B45);
	*(LPDWORD)0x0050272C = BSWAP32(0x8889458C);
	*(LPDWORD)0x00502730 = BSWAP32(0xC60000 << 8 | JMP_REL8);
	*(LPBYTE )0x00502734 = 0x00502760 - (0x00502734 + sizeof(BYTE));
}
