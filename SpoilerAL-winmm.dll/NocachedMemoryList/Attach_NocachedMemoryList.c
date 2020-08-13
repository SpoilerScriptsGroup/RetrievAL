#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "bcb6_std_map.h"

EXTERN_C void __cdecl Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam();
EXTERN_C void __cdecl Caller_TSSGCtrl_GetSSGDataFile_Parsing();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_TrimString();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_ExtractStringStub();
EXTERN_C void __cdecl TSSGCtrl_GetSimpleByteCode_unless_Unicode();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_CopyOrMapping();
EXTERN_C void __cdecl TStringDivision_Find_unless_TokenIsEmpty();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile();
EXTERN_C void __cdecl TSSGCtrl_GetSSGDataFile_ClearAtBreak();
EXTERN_C void __cdecl TSSGCtrl_SetSSGDataFile_IgnoreEmptyData();

string *(__cdecl *const TStringDivision_Lower)(
	string          *retVal,
	void            *this,
	string           Src,
	unsigned long    Start,
	unsigned long    End) = (LPVOID)0x004AE4AC;

static __declspec(naked) void __cdecl TSSGCtrl_SetSSGDataFile_LowerStub(
	string    *const Path,
	void      *const strD,
	string           FileName,
	unsigned long    Start,
	unsigned long    End)
{
	__asm {// ecx is Path already
		mov   edx, eax// FileName
		mov   eax, [edx]string._M_start
		cmp   byte ptr [eax], '_'
		jne   CONTINUE

		call  string_ctor_assign
		lea   ecx, [esp + 0x0C]
		jmp   string_dtor

		align 16
	CONTINUE:
		jmp   TStringDivision_Lower
		ud2
	}
}

static __declspec(naked) map_iterator __cdecl TSSGCtrl_SetSSGDataFile_findStub(map* dataFileMap, string* Path) {
	__asm {// compatible with __msfastcall
		mov   eax, [edx]string._M_start
		cmp   byte ptr [eax], '_'
		je    EXTRACT
		jmp   map_string_find

		align 16
	EXTRACT:// return dataFileMap.end()
		mov   eax, [ecx + 8]
		ret
	}
}

#define PUSH_ECX                   (BYTE)0x51
#define PUSH_IMM8                  (BYTE)0x6A
#define MOV_EAX_DWORD_PTR_EBP_IMM8 (WORD)0x458B
#define LEA_ECX_EBP_ADD_IMM8       (WORD)0x4D8D
#define NOP                        (BYTE)0x90
#define NOP_X2                     (WORD)0x9066
#define CALL_REL                   (BYTE)0xE8
#define JMP_REL32                  (BYTE)0xE9
#define JMP_REL8                   (BYTE)0xEB
#define JNZ_SHORT                  (BYTE)0x75

EXTERN_C void __cdecl Attach_NocachedMemoryList()
{
	// TSSGCtrl::GetSSGDataFile
#if 0
	*(LPBYTE )0x004EE11B = PUSH_IMM8;
	*(LPDWORD)0x004EE11C = 0x401F0F00;
	*(LPBYTE )0x004EE120 = 0x00;// nop
#endif

	*(LPDWORD)(0x004EE180 + 1) = (DWORD)Caller_TSSGCtrl_GetSSGDataFile_CheckNocacheParam - (0x004EE180 + 1 + sizeof(DWORD));

	*(LPBYTE )(0x004EE19E + 2) = 0x07;// tmpV.resize
	*(LPDWORD)(0x004EE1B9 + 2) = sizeof(string) * 7;
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
	*(LPBYTE )0x004EE2FA = CALL_REL;
	*(LPDWORD)0x004EE2FB = (DWORD)Caller_TSSGCtrl_GetSSGDataFile_Parsing - (0x004EE2FB + sizeof(DWORD));
	*(LPBYTE )0x004EE2FF =         0x8B;// mov esi, edx
	*(LPDWORD)0x004EE300 = BSWAP32(0xF285F6 << 8 | JNZ_SHORT);// test esi, esi
	*(LPBYTE )0x004EE304 = 0x004EE34E - (0x004EE304 + sizeof(BYTE));
	*(LPBYTE )0x004EE305 = 0x85;// test
	*(LPBYTE )0x004EE306 = 0xC0;// eax, eax

#ifdef NO_FIXED_AT_PARSING
	*(LPDWORD)(0x004EE3B5 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_TrimString - (0x004EE3B5 + 1 + sizeof(DWORD));
#endif

	// *(LPWSTR)&tmpC[StrSize] = L'\0';
	*(LPBYTE )0x004EEDC5 = 0x66;
	*(LPWORD )0x004EEDC6 = 0x04C7;
	*(LPDWORD)0x004EEDC8 = 0xE9000030;
	*(LPDWORD)0x004EEDCC = 0;// jmp $

	// SIt => tmpC
	*(LPDWORD)0x004EEE55 = -0x0420;
	// SKIP memcpy(tmpC, SIt, StrSize)
	*(LPBYTE )0x004EEE75 = JMP_REL8;
	*(LPBYTE )0x004EEE76 = 0x004EEE8C - (0x004EEE76 + sizeof(BYTE));
	*(LPBYTE )0x004EEE77 = 0x0F;// NOP DWORD PTR [EAX + EAX*1 + 0]
	*(LPDWORD)0x004EEE78 = 0x0000441F;

#if 1
	// }else{	//I’[•¶ŽšŽw’è
	*(LPDWORD)(0x004EEE3E + 2) = (DWORD)TSSGCtrl_GetSSGDataFile_ExtractStringStub - (0x004EEE3E + 2 + sizeof(DWORD));

	// SIt => tmpC
	*(LPDWORD)0x004EF087 = -0x0420;
	// SKIP memcpy(tmpC, SIt, StrSize)
	*(LPBYTE )0x004EF0A7 = JMP_REL8;
	*(LPBYTE )0x004EF0A8 = 0x004EF0BE - (0x004EF0A8 + sizeof(BYTE));
	*(LPBYTE )0x004EF0A9 = 0x0F;// NOP DWORD PTR [EAX + EAX*1 + 0]
	*(LPWORD )0x004EF0AA = 0x441F;
	*(LPWORD )0x004EF0AC = 0x0000;
#else
	/*
		string EndCode((const string &)(EndWord != "unicode" ? GetSimpleByteCode(SSGS, EndWord) : string()));
	*/
	*(LPDWORD)(0x004EEFE6 + 1) = (DWORD)TSSGCtrl_GetSimpleByteCode_unless_Unicode - (0x004EEFE6 + 1 + sizeof(DWORD));

	// SIt => tmpC
	*(LPDWORD)0x004EF087 = -0x0420;

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
#endif

#if 0
	*(LPDWORD)(0x004EE307 + 2) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EE307 + 2 + sizeof(DWORD));
	*(LPDWORD)(0x004EE585 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EE585 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004EE780 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EE780 + 1 + sizeof(DWORD));
	*(LPDWORD)(0x004EEC13 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_ClearAtBreak - (0x004EEC13 + 1 + sizeof(DWORD));

	// TSSGCtrl::SetSSGDataFile
	*(LPBYTE )0x004F0A48 = JMP_REL32;
	*(LPDWORD)0x004F0A49 = (DWORD)TSSGCtrl_SetSSGDataFile_IgnoreEmptyData - (0x004F0A49 + sizeof(DWORD));
	*(LPDWORD)0x004F0A4D = 0x00401F0F;// nop
#else
	*(LPBYTE)(0x004EF416 + 1) = 0x004EF430 - (0x004EF416 + 1 + sizeof(BYTE));
	// failed break
	*(LPBYTE )0x004EF423 = JMP_REL32;
	*(LPDWORD)0x004EF424 = 0x004EF581 - (0x004EF424 + sizeof(DWORD));

	*(LPDWORD)0x004EF428 = 0x00841F0F;
	*(LPDWORD)0x004EF42C = 0;// nop

	*(LPWORD )0x004EF430 = 0x458B;
	*(LPBYTE )0x004EF432 = 0xE8;
	*(LPWORD )0x004EF436 = 0x453B;
	*(LPBYTE )0x004EF438 = 0xEC;
#endif

#if 0
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
#endif

#if 0
	*(LPDWORD)(0x004EF447 + 1) = (DWORD)TSSGCtrl_GetSSGDataFile_FixSetSSGDataFile - (0x004EF447 + 1 + sizeof(DWORD));
#elif 0
	// TSSGCtrl::SetSSGDataFile
	*(LPDWORD)(0x004F19DA + 1) = (DWORD)TSSGCtrl_SetSSGDataFile_findStub - (0x004F19DA + 1 + sizeof(DWORD));
#endif// replaced at FixGetSSGDataFile

	// TSSGCtrl::GetSSGDataFile
	//   strD.Lower(FName) => FName
	*(LPBYTE )0x004EF455 =         0x8D;
	*(LPWORD )0x004EF456 = BSWAP16(0x5510);
	*(LPDWORD)0x004EF458 = BSWAP32(0x8D8D70FE);
	*(LPWORD )0x004EF45C = BSWAP16(0xFFFF);
	*(LPBYTE )0x004EF45E = CALL_REL;
	*(LPDWORD)0x004EF45F = (DWORD)string_ctor_assign - (0x004EF45F + sizeof(DWORD));
	*(LPBYTE )0x004EF463 = JMP_REL8;
	*(LPBYTE )0x004EF464 = 0x004EF499 - (0x004EF464 + sizeof(BYTE));

	// TSSGCtrl::SetSSGDataFile
	*(LPDWORD)(0x004F193F + 1) = (DWORD)TSSGCtrl_SetSSGDataFile_LowerStub - (0x004F193F + 1 + sizeof(DWORD));
}
