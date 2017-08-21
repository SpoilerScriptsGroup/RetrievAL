#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TMainForm.h"
#include "TSSArg.h"

string * __fastcall SubjectStringTable_GetString(string *s);

static void __fastcall ModifyNowValueBoolVector(string *DrawStr, TSSArg *Arg);

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueCalc()
{
	__asm
	{
		#define SSC                            edi
		#define offsetof_TSSCalc_nowValHeadStr 160
		#define Format                         (esp + 12)

		lea     ecx, [SSC + offsetof_TSSCalc_nowValHeadStr]
		call    SubjectStringTable_GetString
		mov     ecx, dword ptr [eax + 4]
		mov     eax, dword ptr [eax]
		mov     dl, byte ptr [eax]
		inc     eax
		cmp     eax, ecx
		jae     L1
		cmp     dl, '_'
		jne     L1
		mov     dword ptr [Format], eax
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

		lea     ecx, [SSFC + offsetof_TSSFloatCalc_nowValHeadStr]
		call    SubjectStringTable_GetString
		mov     ecx, dword ptr [eax + 4]
		mov     eax, dword ptr [eax]
		mov     dl, byte ptr [eax]
		inc     eax
		cmp     eax, ecx
		jae     L1
		cmp     dl, '_'
		jne     L1
		mov     dword ptr [Format], eax
	L1:
		jmp     dword ptr [TStringDivision_ToStringDouble]

		#undef SSFC
		#undef offsetof_TSSFloatCalc_nowValHeadStr
		#undef Format
	}
}

__declspec(naked) void __cdecl TMainForm_DrawTreeCell_ModifyNowValueBoolVector(string* DrawStr, const char* first, const char *last, void *reserved)
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

static void __fastcall ModifyNowValueBoolVector(string *DrawStr, TSSArg *Arg)
{
	string s;
	size_t insertLength, requireLength;
	LPSTR  p, dest;

	TSSArg_ToString(&s, Arg);
	insertLength = string_length(&s);
	requireLength = string_length(DrawStr) + insertLength + 2;
	if (requireLength >= (size_t)(DrawStr->_M_end_of_storage - DrawStr->_M_start))
		string_reserve(DrawStr, requireLength);
	p = DrawStr->_M_finish;
	*p = '[';
	dest = ++p;
	*(LPWORD)(p += insertLength) = BSWAP16(']\0');
	DrawStr->_M_finish = p + 1;
	__movsb(dest, s._M_start, insertLength);
	string_dtor(&s);
}
