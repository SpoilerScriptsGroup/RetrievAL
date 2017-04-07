#include "intrinsic.h"
#include "TMainForm.h"
#include "TSSArg.h"

static void __fastcall ModifyNowValueCalc(LPCSTR nowValHeadStr, const char **Format);
static void __fastcall ModifyNowValueBoolVector(bcb6_std_string *DrawStr, TSSArg *Arg);

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueCalc()
{
	__asm
	{
		#define SSC                            edi
		#define offsetof_TSSCalc_nowValHeadStr 160

		mov     ecx, dword ptr [SSC + offsetof_TSSCalc_nowValHeadStr]
		lea     edx, [esp + 12]
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
		lea     edx, [esp + 16]
		jmp     ModifyNowValueCalc

		#undef SSFC
		#undef offsetof_TSSFloatCalc_nowValHeadStr
	}
}

static __declspec(naked) void __fastcall ModifyNowValueCalc(LPCSTR nowValHeadStr, const char **Format)
{
	__asm
	{
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, '_'
		jne     L1
		mov     dword ptr [edx], ecx
	L1:
		jmp     dword ptr [TStringDivision_ToString]
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
	size_t          insertLength, textLength, requireLength;
	LPSTR           p, dest;

	TSSArg_ToString(&s, Arg);
	insertLength = bcb6_std_string_length(&s);
	textLength = bcb6_std_string_length(DrawStr);
	requireLength = textLength + insertLength + 2;
	if (requireLength >= (size_t)(DrawStr->_M_end_of_storage - DrawStr->_M_start))
		bcb6_std_string_allocate(DrawStr, requireLength);
	p = DrawStr->_M_start + textLength;
	*p = '[';
	dest = ++p;
	*(LPWORD)(p += insertLength) = BSWAP16(']\0');
	DrawStr->_M_finish = p + 1;
	__movsb(dest, s._M_start, insertLength);
	bcb6_std_string_dtor(&s);
}
