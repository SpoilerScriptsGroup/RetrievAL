#include <windows.h>

EXTERN_C void __cdecl Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam();
EXTERN_C void __cdecl Caller_TSSGCtrl_GetSSGDataFile_Parsing();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_TrimString();
EXTERN_C void __cdecl TSSGCtrl_GetSimpleByteCode_unless_Unicode();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_CopyOrMapping();
EXTERN_C void __cdecl TStringDivision_Find_unless_TokenIsEmpty();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile();

#define PUSH_ECX                   (BYTE)0x51
#define PUSH_IMM8                  (BYTE)0x6A
#define MOV_EAX_DWORD_PTR_EBP_IMM8 (WORD)0x458B
#define LEA_ECX_EBP_ADD_IMM8       (WORD)0x4D8D
#define NOP                        (BYTE)0x90
#define NOP_X2                     (WORD)0xC089//9090
#define JMP_REL32                  (BYTE)0xE9
#define JMP_REL8                   (BYTE)0xEB

EXTERN_C void __cdecl Attach_NocachedMemoryList()
{
	// TSSGCtrl::GetSSGDataFile
	*(LPBYTE )0x004EE11B = PUSH_IMM8;
	*(LPBYTE )0x004EE11C = 0x00;
	*(LPBYTE )0x004EE11D = JMP_REL8;
	*(LPBYTE )0x004EE11E = 0x004EE121 - (0x004EE11E + sizeof(BYTE));
	*(LPWORD )0x004EE11F = NOP_X2;

	*(LPDWORD)(0x004EE180 + 1) = (DWORD)Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam - (0x004EE180 + 1 + sizeof(DWORD));

	*(LPBYTE )(0x004EE19E + 2) = 0x07;
	*(LPDWORD)(0x004EE1B9 + 2) = 0x000000A8;
	*(LPDWORD)(0x004EE23F + 1) = 0x00000007;

	/*
		if (!IsNocache)
		{
			StrSize = TStringDivision::ToULongDef(tmpV[2], 1);
			RowSize = TStringDivision::ToULongDef(tmpV[3], 1);
			StepSize = TStringDivision::ToULongDef(tmpV[4], 1);
		}
		else
		{
			StrSize = Parsing(SSGS, tmpV[2], 4, "List", Address, 0);
			RowSize = Parsing(SSGS, tmpV[3], 4, "List", Address, 0);
			StepSize = Parsing(SSGS, tmpV[4], 4, "List", Address, 0);
		}
	*/
	*(LPBYTE )0x004EE30D = JMP_REL32;
	*(LPDWORD)0x004EE30E = (DWORD)Caller_TSSGCtrl_GetSSGDataFile_Parsing - (0x004EE30E + sizeof(DWORD));

	*(LPDWORD)(0x004EE3B5 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_TrimString - (0x004EE3B5 + 1 + sizeof(DWORD));

	// *(LPWSTR)&tmpC[StrSize] = L'\0';
	*(LPBYTE )0x004EEDC5 = 0x66;
	*(LPWORD )0x004EEDC6 = 0x04C7;
	*(LPDWORD)0x004EEDC8 = 0xE9000030;
	*(LPDWORD)0x004EEDCC = 0;// jmp $
	// TProcessCtrl::OneRead to ReadProcessMemory(SHandle, Address, tmpC, StrSize, NULL)
	*(LPBYTE )0x004EEE52 = PUSH_IMM8;
	*(LPBYTE )0x004EEE53 = 0;
	*(LPBYTE )0x004EEE54 = 0x56  ;// push esi
	*(LPWORD )0x004EEE55 = 0xB5FF;// push [ebp - ...
	*(LPDWORD)0x004EEE57 = -0x0420;
	*(LPWORD )0x004EEE5B = 0xB5FF;// push [ebp - ...
	*(LPDWORD)0x004EEE5D = -0x02FC;
	*(LPDWORD)0x004EEE66 = (DWORD)ReadProcessMemory - (0x004EEE66 + sizeof(DWORD));
	*(LPWORD )0x004EEE6A = 0xC085;// test eax, eax
	*(LPWORD )0x004EEE6C = 0x840F;// jz
	*(LPDWORD)0x004EEE6E = 0x004EF33D - (0x004EEE6E + sizeof(DWORD));
	// SKIP memcpy(tmpC, SIt, StrSize)
	*(LPBYTE )0x004EEE72 = JMP_REL8;
	*(LPBYTE )0x004EEE73 = 0x004EEE8C - (0x004EEE73 + sizeof(BYTE));
	*(LPBYTE )0x004EEE74 = NOP;

	/*
		string EndCode((const string &)(EndWord != "unicode" ? GetSimpleByteCode(SSGS, EndWord) : string()));
	*/
	*(LPDWORD)(0x004EEFE6 + 1) = (DWORD)TSSGCtrl_GetSimpleByteCode_unless_Unicode - (0x004EEFE6 + 1 + sizeof(DWORD));

	// TProcessCtrl::OneRead to ReadProcessMemory(SHandle, Address, tmpC, StrSize, NULL)
	__movsb(0x004EF084, 0x004EEE52, 0x14);
	*(LPDWORD)0x004EF098 = (DWORD)ReadProcessMemory - (0x004EF098 + sizeof(DWORD));
	*(LPDWORD)0x004EF09C = 0x8500408D;// lea eax,[eax]; test r32,r32

	/*
		if (!EndCode.empty())
			memcpy(SIt, tmpC, StrSize);
		else
			WideCharToMultiByte(CP_THREAD_ACP, 0, (LPCWSTR)tmpC, StrSize + 2 >> 1, SIt, StrSize + 1, NULL, NULL);
	*/
	*(LPDWORD)(0x004EF0A8 + 2) = -0x0420;
	*(LPDWORD)(0x004EF0AF + 2) = -0x041C;
	*(LPDWORD)(0x004EF0B6 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_CopyOrMapping - (0x004EF0B6 + 1 + sizeof(DWORD));

	/*
		unsigned long Pos = !EndCode.empty() ? strD.Find(string(SIt), EndCode) : string::npos;
	*/
	*(LPDWORD)(0x004EF0EF + 2) = -0x041C;
	*(LPDWORD)(0x004EF11E + 1) = (DWORD)TStringDivision_Find_unless_TokenIsEmpty - (0x004EF11E + 1 + sizeof(DWORD));
	// SIt[Pos] = (char)NULL;
	*(LPDWORD)(0x004EF1A2 + 2) = -0x041C;

	// tmpV.push_back(SIt);
	*(LPDWORD)(0x004EF1C8 + 2) = -0x041C;

	*(LPBYTE )0x004EF423 = JMP_REL8;
	*(LPBYTE )0x004EF424 = 0x004EF43B - (0x004EF424 + sizeof(BYTE));
	*(LPBYTE )0x004EF425 = NOP;

	*(LPWORD )0x004EF43B = MOV_EAX_DWORD_PTR_EBP_IMM8;
	*(LPBYTE )0x004EF43D = (char)0x08;
	*(LPWORD )0x004EF43E = LEA_ECX_EBP_ADD_IMM8;
	*(LPBYTE )0x004EF440 = (char)0x10;
	*(LPBYTE )0x004EF441 = PUSH_ECX;
	*(LPWORD )0x004EF442 = LEA_ECX_EBP_ADD_IMM8;
	*(LPBYTE )0x004EF444 = (char)-0x30;
	*(LPBYTE )0x004EF445 = PUSH_ECX;

	*(LPDWORD)(0x004EF447 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile - (0x004EF447 + 1 + sizeof(DWORD));
}
