#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"
#include "TProcessAccessElementBase.h"

extern const BOOL EnableParserFix;
extern const DWORD bcb6_std_string_substr;

uint64_t __cdecl InternalParsing(TSSGCtrl* SSGCtrl, TSSGSubject* SSGS, const string* Src, BOOL IsInteger, va_list ArgPtr);

static unsigned long __fastcall OffsetRel(list* CodeList, TProcessAccessElementBase* NowAE, unsigned long Rel)
{
	for (list_iterator it = list_begin(CodeList); it != list_end(CodeList); list_iterator_increment(it))
		Rel += TProcessAccessElementBase_GetSize(*(TProcessAccessElementBase**)it->_M_data, TRUE);
	Rel += TProcessAccessElementBase_GetSize(NowAE, TRUE);
	return Rel;
}

static intptr_t __fastcall TSSGCtrl_StrToProcessAccessElementVec_Data_ctor(vector_byte *Data, char size, uint64_t Val)
{
	vector_ctor(Data);
	if (size >= '1' && size <= '8')
	{
		vector_byte_reserve(Data, sizeof(uint64_t));
		*(uint64_t*)vector_begin(Data) = Val;
		vector_end(Data) += size - '0';
		return 0x0050BCA8;// break;
	}// parsing code, but no inserting if 0 bytes
	return 0x0050BC21;// continue;
}

__declspec(naked) void __cdecl Caller_ParsingWithRel()
{
	static const char lpszRel[] = "Rel";
	extern BOOL FixTheProcedure;
	__asm
	{
		cmp     dword ptr [EnableParserFix], 0
		je      L1

		#define this          (ebp + 0x0C)
		#define SSGS          (ebp + 0x10)
		#define Code          (ebp + 0x14)
		#define Rel           (ebp + 0x2C)
		#define i              esi
		#define CodeList      (ebp - 0x10)
		#define NowAE         (ebp - 0x085C)
		#define PosEnd        (ebp - 0x0D60)
		#define Src           (ebp - 0x03E8)
		#define Data          (ebp - 0x03C8)

		mov     word ptr [ebx + 0x10], 0x08B4
		push    0
		push    0
		push    dword ptr [Rel]
		cmp     FixTheProcedure, 0
		je      L2
		mov     edx, [NowAE]
		lea     ecx, [CodeList]
		call    OffsetRel
		push    eax
	L2:
		push    offset lpszRel
		push    3
		push    esp
		push    TRUE
		mov     ecx, dword ptr [PosEnd]
		lea     eax, [i + 2]
		sub     ecx, eax
		lea     edx, [Src]
		push    edx
		push    ecx
		push    eax
		lea     ecx, [Code]
		push    ecx
		push    edx
		call    dword ptr [bcb6_std_string_substr]
		add     esp, 16
		mov     ecx, dword ptr [SSGS]
		mov     eax, dword ptr [this]
		push    ecx
		push    eax
		call    InternalParsing
		add     esp, 20 + 20
		push    edx
		push    eax
		lea     ecx, [Src]
		call    string_dtor
		mov     eax, [Code]
		movzx   edx, [eax + i + 1]
		lea     ecx, [Data]
		mov      word ptr [ebx + 0x10], 0x08D8
		inc     dword ptr [ebx + 0x1C]
		call    TSSGCtrl_StrToProcessAccessElementVec_Data_ctor
		add     dword ptr [ebx + 0x1C], 3
		mov      word ptr [ebx + 0x10], 0x0890
		jmp     eax

		#undef PosEnd
		#undef Src
		#undef Data
		#undef NowAE
		#undef CodeList
		#undef Rel
		#undef Code
		#undef SSGS
		#undef this

		align   16
	L1:
		push    0
		mov     ecx, 0050B4D9H
		lea     eax, [esi + 2H]
		jmp     ecx
	}
}
