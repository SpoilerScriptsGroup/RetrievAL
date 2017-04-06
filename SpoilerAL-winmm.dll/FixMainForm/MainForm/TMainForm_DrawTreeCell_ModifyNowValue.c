#include "intrinsic.h"
#include "TMainForm.h"
#include "TSSArg.h"

static void __fastcall ModifyNowValueCalc(LPCSTR nowValHeadStr);
static void __fastcall ModifyNowValueDefault(bcb6_std_string *DrawStr, TSSArg *Arg);

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueCalc()
{
	__asm
	{
		#define SSC                            edi
		#define offsetof_TSSCalc_nowValHeadStr 160

		mov     ecx, dword ptr [SSC + offsetof_TSSCalc_nowValHeadStr]
		jmp     ModifyNowValueCalc

		#undef SSC
		#undef offsetof_TSSCalc_nowValHeadStr
	}
}

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueFloatCalc()
{
	__asm
	{
		#define SSFC                                edi
		#define offsetof_TSSFloatCalc_nowValHeadStr 176

		mov     ecx, dword ptr [SSFC + offsetof_TSSFloatCalc_nowValHeadStr]
		jmp     ModifyNowValueCalc

		#undef SSFC
		#undef offsetof_TSSFloatCalc_nowValHeadStr
	}
}

static __declspec(naked) void __fastcall ModifyNowValueCalc(LPCSTR nowValHeadStr)
{
	__asm
	{
		cmp     byte ptr [ecx], '_'
		jne     L1
		inc     ecx
		mov     dword ptr [esp + 12], ecx
	L1:
		jmp     dword ptr [TStringDivision_ToString]
	}
}

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValue(bcb6_std_string* DrawStr, const char* first, const char *last, void *reserved)
{
	__asm
	{
		#define ReturnAddress 00445B80H
		#define DrawStr       (ebp - 18H)
		#define Arg           (ebp - 254H)

		mov     edx, dword ptr [Arg]
		lea     ecx, [DrawStr]
		push    ReturnAddress
		jmp     ModifyNowValueDefault

		#undef ReturnAddress
		#undef DrawStr
		#undef Arg
	}
}

static void __fastcall ModifyNowValueDefault(bcb6_std_string *DrawStr, TSSArg *Arg)
{
	bcb6_std_string s;
	size_t          length, n;

	TSSArg_ToString(&s, Arg);
	length = bcb6_std_string_length(&s);
	n = length + 2;
	if (n >= (size_t)(DrawStr->_M_end_of_storage - DrawStr->_M_start))
		bcb6_std_string_allocate(DrawStr, n);
	*DrawStr->_M_start = '[';
	DrawStr->_M_finish = DrawStr->_M_start + n;
	*(LPWORD)(DrawStr->_M_finish - 1) = BSWAP16(']\0');
	__movsb(DrawStr->_M_start + 1, s._M_start, length);
	bcb6_std_string_dtor(&s);
}
