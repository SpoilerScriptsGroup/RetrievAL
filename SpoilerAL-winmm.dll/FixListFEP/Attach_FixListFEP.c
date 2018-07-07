#include <windows.h>
#include "TSSGSubject.h"

#pragma pack(push, 1)
typedef struct _TSSBitList {
	TSSGSubject     grandParent;
	bcb6_std_string lockStr;
	BYTE            size;
	BYTE            padding[7];
	bcb6_std_string addressStr;
	bcb6_std_string indexFileName;
	bcb6_std_string realFileName;
	unsigned long   maskByte;
} TSSBitList;
#pragma pack(pop)

void __stdcall ReplaceDefineDynamic(TSSGSubject* SSGS, bcb6_std_string* line);

static void __fastcall TSSBitList_Setting_maskByteCalc(TSSBitList* SSBL, bcb6_std_vector_string* ListFile) {
	unsigned long mask = -1LL << SSBL->size * 8;
	size_t End = bcb6_std_vector_size_by_type(ListFile, bcb6_std_string);
	for (size_t i = 0; i < End; ++i) {
		bcb6_std_string* line = &bcb6_std_vector_type_at(ListFile, bcb6_std_string, i);
		//ReplaceDefineDynamic((TSSGSubject*)SSBL, line);
		if (!bcb6_std_string_empty(line))
			mask |= 1 << i;
	}
	SSBL->maskByte = ~mask;
}

static void __declspec(naked) TSSBitList_Setting_maskByteStub() {
	__asm {
		mov  edx, edi
		mov  ecx, ebx
		jmp  TSSBitList_Setting_maskByteCalc
	}
}

static void __declspec(naked) TSSBitList_Write_ValStub() {
	__asm {
		mov   eax, dword ptr [ebp - 0x0220]// Src
		and   eax, dword ptr [ebp - 0x0094]// MaskByte
		mov   dword ptr [ebp - 0x0090], eax// Val
		movzx ecx, byte ptr [ebx + 0x78]   // TSSBitList->size
		ret
	}
}

EXTERN_C void __cdecl TSSBitList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSBundleList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSDoubleList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSList_Setting_CheckFEPParam();
EXTERN_C void __cdecl TSSGCtrl_OneRead_with_CheckIO_FEP();
EXTERN_C void __cdecl TSSGCtrl_OneWrite_with_CheckIO_FEP();

#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8
#define PUSH_IMM32 (BYTE)0x68
#define JMP_REL32  (BYTE)0xE9
#define OR_RM32    (BYTE)0x09

EXTERN_C void __cdecl Attach_FixListFEP()
{
	// TSSBitList::Setting
	*(LPBYTE )0x004B8352 = CALL_REL32;
	*(LPDWORD)0x004B8353 = (DWORD)TSSBitList_Setting_CheckFEPParam - (0x004B8353 + sizeof(DWORD));
	*(LPBYTE )0x004B8357 = NOP;

	*(LPBYTE )0x004B862D = PUSH_IMM32;
	*(LPDWORD)0x004B862E = 0x004B86E2;
	*(LPBYTE )0x004B8632 = JMP_REL32;
	*(LPDWORD)0x004B8633 = (DWORD)TSSBitList_Setting_maskByteStub - (0x004B8633 + sizeof(DWORD));

	// TSSBitList::Read
	*(LPDWORD)(0x004B910F + 1) = (DWORD)TSSGCtrl_OneRead_with_CheckIO_FEP - (0x004B910F + 1 + sizeof(DWORD));

	// TSSBitList::Write
	*(LPDWORD) 0x004BB488 = 0xFF70BD8B;// mov edi, dword ptr [ebp-90h]
	*(LPDWORD) 0x004BB48C = 0xAB0FFFFF;// bts edi, eax  
	*(LPWORD ) 0x004BB490 =     0x89C7;// mov dword ptr [ebp-90h], edi

	*(LPDWORD)(0x004BB522 + 1) = (DWORD)TSSGCtrl_OneRead_with_CheckIO_FEP - (0x004BB522 + 1 + sizeof(DWORD));

	*(LPBYTE ) 0x004BB5D7 = CALL_REL32;
	*(LPDWORD)(0x004BB5D7 + 1) = (DWORD)TSSBitList_Write_ValStub - (0x004BB5D7 + 1 + sizeof(DWORD));

	*(LPBYTE ) 0x004BB6A7 = OR_RM32;

	*(LPBYTE ) 0x004BB614 = OR_RM32;

	*(LPDWORD)(0x004BB6F9 + 1) = (DWORD)TSSGCtrl_OneWrite_with_CheckIO_FEP - (0x004BB6F9 + 1 + sizeof(DWORD));

	// TSSBundleList::Setting
	*(LPBYTE )0x004BEBDC = CALL_REL32;
	*(LPDWORD)0x004BEBDD = (DWORD)TSSBundleList_Setting_CheckFEPParam - (0x004BEBDD + sizeof(DWORD));
	*(LPBYTE )0x004BEBE1 = NOP;

	// TSSBundleList::Read
	// replaced at "Parsing\TSSGCtrl_IsRemoteProcess.c

	// TSSBundleList::Write
	// replaced at "Parsing\TSSGCtrl_IsRemoteProcess.c

	// TSSDoubleList::Setting
	*(LPBYTE )0x004C372A = CALL_REL32;
	*(LPDWORD)0x004C372B = (DWORD)TSSDoubleList_Setting_CheckFEPParam - (0x004C372B + sizeof(DWORD));
	*(LPBYTE )0x004C372F = NOP;

	// TSSDoubleList::Read
	// replaced at "Parsing\TSSGCtrl_IsRemoteProcess.c

	// TSSDoubleList::Write
	// replaced at "Parsing\TSSGCtrl_IsRemoteProcess.c

	// TSSList::Setting
	*(LPBYTE )0x00529A16 = CALL_REL32;
	*(LPDWORD)0x00529A17 = (DWORD)TSSList_Setting_CheckFEPParam - (0x00529A17 + sizeof(DWORD));
	*(LPBYTE )0x00529A1B = NOP;

	// TSSList::Read
	*(LPDWORD)(0x0052A15A + 1) = (DWORD)TSSGCtrl_OneRead_with_CheckIO_FEP - (0x0052A15A + 1 + sizeof(DWORD));

	// TSSList::Write
	*(LPDWORD)(0x0052A30C + 1) = (DWORD)TSSGCtrl_OneWrite_with_CheckIO_FEP - (0x0052A30C + 1 + sizeof(DWORD));
}
