#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_operator.h"
#include "bcb6_std_list.h"
#include "TProcessAccessElementData.h"
#include "TSSGSubject.h"

static __declspec(naked) void TProcessAccessElementMaskData_Execute_Masking()
{
#define data      (ebx + 0x10)
#define mask      (ebx + 0x28)
#define Size      (ebp - 0xCC)
#define tmpV      (ebp - 0x38)
#define MaskedSrc (ebp - 0x50)
	__asm {
		mov   ecx, dword ptr [Size]
		test  ecx, ecx
		jz    EMPTY
		// esi is volatile in here
		mov   esi, dword ptr [mask]
		mov   edx, dword ptr [tmpV]
	AGAIN:
		movzx eax, byte ptr [esi + ecx - 1]
		not   eax
		and   byte ptr [edx + ecx - 1], al
		loop  AGAIN
	EMPTY:
		mov   eax, dword ptr [data]
		mov   dword ptr [MaskedSrc], eax
		ret
	}
#undef  MaskedSrc
#undef  tmpV
#undef  Size
#undef  mask
#undef  data
}

static void __fastcall TSSBitList_Setting_maskByteCalc(TSSBitList* SSBL, bcb6_std_vector_string* ListFile) {
	extern void __stdcall ReplaceDefineDynamic(TSSGSubject* SSGS, bcb6_std_string* line);
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

static uintptr_t __inline hex(intptr_t register c)
{
	uintptr_t register v = 0;
	if ((c -= '0') >= 0 && c < 10)
		v = c;
	else if ((c -= 'A' - ':') >= 10 && c < 0x10)
		v = c;
	else if ((c -= 'a' - 'A') >= 10 && c < 0x10)
		v = c;
	return v;
}

static TProcessAccessElementMaskData* __fastcall TSSGCtrl_StrToProcessAccessElementVec_default(
	list                   * const CodeList,
	const char             * const code,
	TProcessAccessElementMaskData* NowAE)
{
	BYTE Mask, Data;

	if (code[0] != '?')
	{
		Mask = 0x00;
		Data = hex(code[0]) << 4;
	}
	else
	{
		Mask = 0xF0;
		Data = 0x00;
	}

	if (code[1] != '?')
		Data |= hex(code[1]);
	else
		Mask |= 0x0F;

	if (!Mask) switch (TProcessAccessElement_GetType(NowAE))
	{
	case atMASK_DATA:
		vector_byte_push_back(TProcessAccessElementMaskData_GetMask(NowAE), 0x00);
		goto L_PUSH_DATA;
	default:
		list_dword_push_back(CodeList, (LPDWORD)&NowAE);
		 NowAE = bcb6_operator_new(sizeof(TProcessAccessElementMaskData));
		*NowAE = (const TProcessAccessElementMaskData) { (LPVOID)0x00627294, atDATA };
		vector_byte_reserve  (TProcessAccessElementData_GetData(NowAE), sizeof(uint64_t));
	case atDATA:
	L_PUSH_DATA:
		vector_byte_push_back(TProcessAccessElementData_GetData(NowAE), Data);
	}
	else switch (TProcessAccessElement_GetType(NowAE))
	{
		vector_byte data;
	case atDATA:
		data = *TProcessAccessElementData_GetData(NowAE);
		bcb6_operator_delete(NowAE);
		 NowAE = bcb6_operator_new(sizeof(TProcessAccessElementMaskData));
		*NowAE = (const TProcessAccessElementMaskData) { (LPVOID)0x0063FE38, atMASK_DATA };
		*TProcessAccessElementData_GetData(NowAE) = data;
		vector_byte_reserve  (TProcessAccessElementMaskData_GetMask(NowAE), vector_capacity(&data));
		vector_byte_resize   (TProcessAccessElementMaskData_GetMask(NowAE), vector_size    (&data));
		goto L_PUSH_MASK_DATA;
	default:
		list_dword_push_back(CodeList, (LPDWORD)&NowAE);
		 NowAE = bcb6_operator_new(sizeof(TProcessAccessElementMaskData));
		*NowAE = (const TProcessAccessElementMaskData) { (LPVOID)0x0063FE38, atMASK_DATA };
		vector_byte_reserve  (TProcessAccessElementData_GetData    (NowAE), sizeof(uint64_t));
		vector_byte_reserve  (TProcessAccessElementMaskData_GetMask(NowAE), sizeof(uint64_t));
	case atMASK_DATA:
	L_PUSH_MASK_DATA:
		vector_byte_push_back(TProcessAccessElementData_GetData    (NowAE), Data);
		vector_byte_push_back(TProcessAccessElementMaskData_GetMask(NowAE), Mask);
	}
	return NowAE;
}

static __declspec(naked) void TSSGCtrl_StrToProcessAccessElementVec_defaultStub()
{
#define Code     (ebp + 0x14)
#define CodeList (ebp - 0x10)
#define i         esi
#define NowAE    (ebp - 0x085C)
	__asm {
		push dword ptr [NowAE]
		mov  eax, dword ptr [Code]
		lea  edx, dword ptr [eax + i]
		lea  ecx, dword ptr [CodeList]
		call TSSGCtrl_StrToProcessAccessElementVec_default
		mov  dword ptr [NowAE], eax
		mov  eax, 0x0050E100
		jmp  eax
	}
#undef  NowAE
#undef  i
#undef  CodeList
#undef  Code
}

#define CALL_REL32 (BYTE)0xE8
#define JMP_REL32  (BYTE)0xE9
#define JMP_REL8   (BYTE)0xEB
#define NOP        (BYTE)0x90
#define OR_MEM32   (BYTE)0x09

EXTERN_C void __cdecl FixMaskBytes()
{
	// TProcessAccessElementMaskData::Execute
#if 1
	*(LPBYTE )0x0048EB9C = 0x85;
	*(LPBYTE )0x0048EBB0 = CALL_REL32;
	*(LPDWORD)0x0048EBB1 = (DWORD)TProcessAccessElementMaskData_Execute_Masking - (0x0048EBB1 + sizeof(DWORD));
	*(LPBYTE )0x0048EBB5 = JMP_REL32;
	*(LPDWORD)0x0048EBB6 = 0x0048F02E - (0x0048EBB6 + sizeof(DWORD));
	//   omit dtor for MaskedSrc
	*(LPBYTE )0x0048F17E = JMP_REL8;
#endif

	// TSSBitList::Setting
#ifdef TREAT_LACK_AS_EMPTY
	*(LPBYTE )0x004B862D = CALL_REL32;
	*(LPDWORD)0x004B862E = (DWORD)TSSBitList_Setting_maskByteStub - (0x004B862E + sizeof(DWORD));
	*(LPBYTE )0x004B8632 = JMP_REL32;
	*(LPDWORD)0x004B8633 = 0x004B86E2 - (0x004B8633 + sizeof(DWORD));
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
	*(LPDWORD)(0x005084E5 + 1) = (DWORD)TSSGCtrl_StrToProcessAccessElementVec_defaultStub - (0x005084E5 + 1 + sizeof(DWORD));

	/*
		lea     eax, [esi + 2]  ; 0050C6ED _ 8D. 46, 02
	*/
	*(LPDWORD)(0x0050C6EC) = BSWAP32(0xFF8D4602);

	/*
		lea     ecx, [esi + 2]  ; 0050C702 _ 8D. 4E, 02
	*/
	*(LPDWORD)(0x0050C700) = BSWAP32(0xFFFF8D4E);
}
