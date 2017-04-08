#include "intrinsic.h"
#include "TMainForm.h"
#include "TSSArg.h"

static void __fastcall ModifyNowValueBoolVector(bcb6_std_string *DrawStr, TSSArg *Arg);

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueCalc()
{
	__asm
	{
		#define SSC                            edi
		#define offsetof_TSSCalc_nowValHeadStr 160
		#define Format                         (esp + 12)

		mov     ecx, dword ptr [SSC + offsetof_TSSCalc_nowValHeadStr]
		mov     eax, dword ptr [SSC + offsetof_TSSCalc_nowValHeadStr + 4]
		sub     eax, ecx
		mov     dl, byte ptr [ecx]
		cmp     eax, 1
		jbe     L1
		cmp     dl, '_'
		jne     L1
		inc     ecx
		mov     dword ptr [Format], ecx
	L1:
		jmp     dword ptr [TStringDivision_ToString]

		#undef SSC
		#undef offsetof_TSSCalc_nowValHeadStr
		#undef Format
	}
}

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueFloatCalc()
{
	__asm
	{
		#define SSFC                                edi
		#define offsetof_TSSFloatCalc_nowValHeadStr 176
		#define Format                              (esp + 16)

		mov     ecx, dword ptr [SSFC + offsetof_TSSFloatCalc_nowValHeadStr]
		mov     eax, dword ptr [SSFC + offsetof_TSSFloatCalc_nowValHeadStr + 4]
		sub     eax, ecx
		mov     dl, byte ptr [ecx]
		cmp     eax, 1
		jbe     L1
		cmp     dl, '_'
		jne     L1
		inc     ecx
		mov     dword ptr [Format], ecx
	L1:
		jmp     dword ptr [TStringDivision_ToStringDouble]

		#undef SSFC
		#undef offsetof_TSSFloatCalc_nowValHeadStr
		#undef Format
	}
}

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueBoolVector(bcb6_std_string* DrawStr, const char* first, const char *last, void *reserved)
{
	__asm
	{
		#define ReturnAddress 00445B80H
		#define DrawStr       (ebp - 18H)
		#define Arg           (ebp - 254H)

		mov     edx, dword ptr [Arg]
		lea     ecx, [DrawStr]
		push    ReturnAddress
		jmp     ModifyNowValueBoolVector

		#undef ReturnAddress
		#undef DrawStr
		#undef Arg
	}
}

static void __fastcall ModifyNowValueBoolVector(bcb6_std_string *DrawStr, TSSArg *Arg)
{
	bcb6_std_string s;
	size_t          insertLength, requireLength;
	LPSTR           p, dest;

	TSSArg_ToString(&s, Arg);
	insertLength = bcb6_std_string_length(&s);
	requireLength = bcb6_std_string_length(DrawStr) + insertLength + 2;
	if (requireLength >= (size_t)(DrawStr->_M_end_of_storage - DrawStr->_M_start))
		bcb6_std_string_allocate(DrawStr, requireLength);
	p = DrawStr->_M_finish;
	*p = '[';
	dest = ++p;
	*(LPWORD)(p += insertLength) = BSWAP16(']\0');
	DrawStr->_M_finish = p + 1;
	__movsb(dest, s._M_start, insertLength);
	bcb6_std_string_dtor(&s);
}
