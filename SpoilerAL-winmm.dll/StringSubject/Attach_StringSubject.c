#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSString.h"

#if 0
EXTERN_C void __cdecl Caller_TSSString_Setting_CheckCodePage();
#endif
EXTERN_C void __cdecl Caller_TSSString_Read_TranscodeString();
EXTERN_C void __cdecl Caller_TSSString_Read_terminate_Data();
EXTERN_C void __cdecl Caller_TSSString_Write_Str_ctor();
EXTERN_C void __cdecl Caller_TSSString_ToByteCode_Val_ctor();

static size_t __fastcall TSSString_Setting_ExtraArgs(TSSString *SSGS, vector_string *tmpV) {
	string* sz3 = &vector_at(tmpV, 3);
	SSGS->cautious = vector_size(tmpV) > 3 && string_length(sz3) == 7
		&& *(LPDWORD)string_begin(sz3) == BSWAP32('caut')
		&& *(LPDWORD)&string_at(sz3, sizeof(DWORD)) == BSWAP32('ion\0')
		;
	return vector_size_by_type(tmpV, unsigned __int8);
}

static __declspec(naked) size_t TSSString_Setting_ExtraArgsStub() {
	__asm {
		mov edx, edi
		mov ecx, ebx
		jmp TSSString_Setting_ExtraArgs
	}// must return tmpV.size() in bytes
}

EXTERN_C void __cdecl Attach_StringSubject()
{
	// TSSString::Setting
	//   tmpV.resize(3);
	*(LPWORD )0x0052A7F8 = 0xE8;// call ...
	*(LPDWORD)0x0052A7F9 = (DWORD)TSSString_Setting_ExtraArgsStub - (0x0052A7F9 + sizeof(DWORD));

	// replaced at "SubjectStringTable\SubjectStringOperator.c" - TSSString_Setting_SetEndWord
#if 0
	// TSSString::Setting
	*(LPDWORD)(0x0052AABA + 1) = (DWORD)Caller_TSSString_Setting_CheckCodePage - (0x0052AABA + 1 + sizeof(DWORD));
#endif

	// TSSString::Read
	//   new char[size+1]; => new char[size+2];
	*(LPDWORD)0x0052AF98 = BSWAP32(0x8D4B7F41);// lea ecx, [ebx + 0x7F]; inc  ecx
	*(LPDWORD)0x0052AF9C = BSWAP32(0x8BFC6A0F);// mov edi,  esp        ; push 0x0F

	*(LPDWORD)0x0052AFA8 = BSWAP32(0x8BE78945);// mov esp,  edi
	*(LPDWORD)0x0052AFAC = BSWAP32(0xB48B4378);// mov dword ptr [ebp - 0x4C], eax
	*(LPBYTE )0x0052AFB0 =         0x40       ;// mov eax, dword ptr [ebx + 0x78]; inc eax

	//   tmpC[size] = (char)NULL; => *(LPWSTR)&tmpC[size] = L'\0';
	*(LPBYTE )(0x0052AFBA + 1) =         0x53       ;// eax => edx
	*(LPWORD ) 0x0052AFBE      = BSWAP16(0x66C7    );// mov word ptr 
	*(LPDWORD) 0x0052AFC0      = BSWAP32(0x04170000);// [edi + edx], 0
	*(LPBYTE ) 0x0052AFC4      =         0x90       ;// nop

	//   string Data(tmpC);
	*(LPDWORD)(0x0052B01E + 1) = (DWORD)Caller_TSSString_Read_TranscodeString - (0x0052B01E + 1 + sizeof(DWORD));

	//   skip 'SSGC.stD.Find' if specified code page
	*(LPBYTE )0x0052B045 = 0x74;// je
	*(LPBYTE )0x0052B046 = 0x0052B04E - (0x0052B046 + sizeof(BYTE));
	*(LPBYTE )0x0052B047 =         0x66   ;// cmp
	*(LPWORD )0x0052B048 = BSWAP16(0x837B);// word ptr [ebx + ...], ??
	*(LPBYTE )0x0052B04A = offsetof(TSSString, codePage);
	*(LPBYTE )0x0052B04B = CP_ACP;
	*(LPBYTE )0x0052B04C = 0x74;// je
	*(LPBYTE )0x0052B04D = 0x0052B053 - (0x0052B04D + sizeof(BYTE));
	*(LPBYTE )0x0052B04E = 0xE9;// jmp

	//   Data[Pos]=(byte)NULL;
	*(LPBYTE )0x0052B11D =         0x53       ;// push this
	*(LPWORD )0x0052B11E = BSWAP16(0x8BD0    );// mov  edx, Pos
	*(LPDWORD)0x0052B120 = BSWAP32(0x8D4DE4E8);// lea  ecx, Data
	*(LPDWORD)0x0052B124 = (DWORD)Caller_TSSString_Read_terminate_Data - (0x0052B124 + sizeof(DWORD));
	*(LPBYTE )0x0052B128 =         0x90   ;

	// TSSString::Write
	*(LPDWORD)(0x0052B2DC + 1) = (DWORD)Caller_TSSString_Write_Str_ctor - (0x0052B2DC + 1 + sizeof(DWORD));
	*(LPBYTE )(0x0052B2EB + 2) = 4;// stack size to discard

	// TSSString::ToByteCode
	*(LPDWORD)(0x0052B7C3 + 1) = (DWORD)Caller_TSSString_ToByteCode_Val_ctor - (0x0052B7C3 + 1 + sizeof(DWORD));
	*(LPBYTE )(0x0052B7CC + 2) = 4;// stack size to discard
}
