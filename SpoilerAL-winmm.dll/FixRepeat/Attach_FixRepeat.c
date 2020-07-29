#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"

EXTERN_C BOOL __cdecl TSSGCtrl_ReadSSRFile_CheckSignedParam();
EXTERN_C unsigned long __fastcall TSSGCtrl_ReadSSRFile_DestReserve(BOOL);
EXTERN_C void __cdecl TSSGCtrl_ReadSSRFile_CompareLoopCounter();
EXTERN_C void __cdecl TSSGCtrl_EnumReadSSR_SwitchTmpS_0();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_FixWordRepeat();
EXTERN_C void __cdecl TSSGCtrl_LoopSSRFile_Format();
EXTERN_C unsigned long __cdecl Parsing(IN TSSGCtrl *this, IN TSSGSubject *SSGS, IN const bcb6_std_string *Src, ...);

static unsigned long __fastcall TSSGCtrl_ReadSSRFile_Parsing(
	unsigned long *const End,
	unsigned long *const Begin,
	vector_string *const tmpV,
	TSSGCtrl      *const SSGC,
	TSSGSubject   *const SSGS)
{
	unsigned long   step;
	bcb6_std_string *end = &vector_at(tmpV, 2);
	if (!TSSGCtrl_GetSSGActionListner(SSGC))
	{
		*Begin = TStringDivision_ToULongDef(&vector_at(tmpV, 1), 0);
		*End   = TStringDivision_ToULongDef(&vector_at(tmpV, 2), 0);
		step   = TStringDivision_ToULongDef(&vector_at(tmpV, 3), 1);
	}
	else
	{
		int error;
		SetLastError(NO_ERROR);
		*Begin = Parsing(SSGC, SSGS, &vector_at(tmpV, 1), 0);
		*End   = Parsing(SSGC, SSGS, &vector_at(tmpV, 2), 0);
		step   = Parsing(SSGC, SSGS, &vector_at(tmpV, 3), 0);
		if (!step) step = 1;
		if ((error = GetLastError()) && error != ERROR_NO_MORE_FILES)
		{
			LPSTR lpBuffer;
			*Begin = 0;
			*End   = 0;
			if (FormatMessageA(
				FORMAT_MESSAGE_MAX_WIDTH_MASK |
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				error,
				0,
				(LPSTR)&lpBuffer,
				sizeof(double),
				NULL))
			{
				TMainForm_Guide(lpBuffer, 0);
				LocalFree(lpBuffer);
			}
		}
	}
	if (string_empty (end) ||
		string_length(end) == 1 && string_at(end, 0) == '_')
	{
		*End   = *Begin;
		*Begin = 0;
	}
	return step;
}

static __declspec(naked) vector_string* __cdecl TSSGCtrl_ReadSSRFile_GetSSGDataFile(
	TSSGCtrl   * this,
	TSSGSubject* SSGS,
	string       FName,
	string       DefaultExt,
	string     * CurrentDir) {
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
	*(LPBYTE )0x004FF123 =         0xFF       ;// push dword ptr [SSGS]
	*(LPDWORD)0x004FF124 = BSWAP32(0x75185756);// push this
	*(LPBYTE )0x004FF128 = JMP_REL8           ;// push tmpV
	*(LPBYTE )0x004FF129 = 0x004FF151 - (0x004FF129 + sizeof(BYTE));

	*(LPBYTE )0x004FF151 =         0x8D       ;  
	*(LPWORD )0x004FF152 = BSWAP16(0x9510    );// lea edx, [Begin]
	*(LPDWORD)0x004FF154 = BSWAP32(0xFFFFFF8D);// lea ecx, [Begin - 4]
	*(LPWORD )0x004FF158 = BSWAP16(0x4AFC);
	*(LPBYTE )0x004FF15A = CALL_REL32;
	*(LPDWORD)0x004FF15B = (DWORD)TSSGCtrl_ReadSSRFile_Parsing - (0x004FF15B + sizeof(DWORD));

	// TSSGCtrl::ReadSSRFile
	*(LPDWORD)0x004FF2B8 = BSWAP32(0x8B0C24 << 8 | CALL_REL32);// mov ecx, [esp]
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
