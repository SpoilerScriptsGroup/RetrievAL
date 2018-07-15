#include <windows.h>
#include "intrinsic.h"
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
			mask |= 1 << i;// expect compiling to bts
	}
	SSBL->maskByte = ~mask;
}

static void __declspec(naked) TSSBitList_Setting_maskByteStub() {
	__asm {
		mov edx, edi
		mov ecx, ebx
		jmp TSSBitList_Setting_maskByteCalc
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

#define PUSH_IMM32 (BYTE)0x68
#define JMP_REL32  (BYTE)0xE9
#define NOP        (BYTE)0x90
#define CALL_REL32 (BYTE)0xE8
#define OR_MEM32   (BYTE)0x09

EXTERN_C void __cdecl FixMaskBytes()
{
	// TSSBitList::Setting
#ifdef TREAT_LACK_AS_EMPTY
	*(LPBYTE )0x004B862D = PUSH_IMM32;
	*(LPDWORD)0x004B862E = 0x004B86E2;
	*(LPBYTE )0x004B8632 = JMP_REL32;
	*(LPDWORD)0x004B8633 = (DWORD)TSSBitList_Setting_maskByteStub - (0x004B8633 + sizeof(DWORD));
#else
	*(LPBYTE )0x004B86BD = NOP;
	*(LPWORD )0x004B86BE = BSWAP16(0x33C0    );// xor eax, eax
	*(LPDWORD)0x004B86C0 = BSWAP32(0x0FABC890);// bts eax, ecx
	*(LPBYTE )0x004B86C4 = OR_MEM32;
#endif
	// TSSBitList::Write
	*(LPDWORD)0x004BB488 = BSWAP32(0x8BBD70FF);// mov edi, dword ptr [ebp-90h]
	*(LPDWORD)0x004BB48C = BSWAP32(0xFFFF0FAB);// bts edi, eax  
	*(LPWORD )0x004BB490 = BSWAP16(0xC789    );// mov dword ptr [ebp-90h], edi

	*(LPBYTE )0x004BB5D7 = CALL_REL32;
	*(LPDWORD)0x004BB5D8 = (DWORD)TSSBitList_Write_ValStub - (0x004BB5D8 + sizeof(DWORD));

	*(LPBYTE )0x004BB614 = OR_MEM32;

	*(LPBYTE )0x004BB6A7 = OR_MEM32;

	// TSSGCtrl::StrToProcessAccessElementVec
	/*
		lea     eax, [esi + 2]  ; 0050C6ED _ 8D. 46, 02
	*/
	*(LPDWORD)(0x0050C6EC) = BSWAP32(0xFF8D4602);

	/*
		lea     ecx, [esi + 2]  ; 0050C702 _ 8D. 4E, 02
	*/
	*(LPDWORD)(0x0050C700) = BSWAP32(0xFFFF8D4E);
}
