#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#define _NO_CRT_STDIO_INLINE
#include <stdio.h>
#define USING_NAMESPACE_BCB6_STD
#include "TSSGCtrl.h"

void __cdecl Caller_ParsingWithVal();
void __cdecl TSSGCtrl_LoopSSRFile_LineListLoopContinue();
uint64_t __cdecl InternalParsing(TSSGCtrl* SSGCtrl, TSSGSubject* SSGS, const string* Src, BOOL IsInteger, va_list ArgPtr);

static list_iterator __fastcall TSSGCtrl_LoopSSRFile_Format(
	string       *const tmpS,
	TSSGSubject  *const SSGS,
	enum Repeat   const Type,
	TSSGCtrl     *const SSGC,
	unsigned long const LoopVal,
	list_iterator       VIt)
{
	char           buf[0x0400];
	size_t   const arg[] = { 3, (size_t)"Val", LoopVal, 0/* highword */, 0/* sentinel */ };
	uint64_t const value = InternalParsing(SSGC, SSGS, (string *)&VIt->_M_data[__alignof(double)], TRUE, (va_list)arg);
	int      const width = _snprintf(buf, _countof(buf), string_c_str((string *)&list_iterator_increment(VIt)->_M_data[__alignof(double)]), value);
	if (width >= 0) string_append_cstr_with_length(tmpS, buf, width);
	return VIt;
}

__declspec(naked) list_iterator __stdcall TSSGCtrl_LoopSSRFile_switch_Type(enum Repeat Type, TSSGCtrl *SSGC, unsigned long LoopVal, list_iterator VIt)
{
	__asm
	{
		#define this      (ebp + 8H)
		#define LoopVal   (ebp + 14H)
		#define VIt       (ebp - 180H)
		#define tmpS      (ebp - 78H)
		#define SSGS      (ebp - 60H)
		#define _Type     edi

#if 1
		lea     edx, [SSGS]
		lea     ecx, [tmpS]
		cmp     dword ptr [esp + 4], rtFORMAT
#pragma warning(suppress: 4414)
		je      TSSGCtrl_LoopSSRFile_Format
		mov     eax, [esp + 16]
		ret     4 * 4
#else
		cmp     _Type, rtFORMAT
		jne     L3
		sub     esp, 256 + 8

		#define buffer      esp
		#define outReserved (esp + 256)

		mov     eax, dword ptr [LoopVal]
		mov     edx, dword ptr [VIt]
		push    eax
		add     edx, 16
		lea     ecx, [SSGS]
		mov     eax, dword ptr [this]
		push    edx
		push    ecx
		push    eax
		call    Caller_ParsingWithVal
		mov     ecx, dword ptr [VIt]
		add     esp, 16
		mov     ecx, dword ptr [ecx]
		mov     edx, buffer
		mov     dword ptr [VIt], ecx
		mov     ecx, dword ptr [ecx + 16]
		push    eax
		push    ecx
		push    256
		push    edx
		call    _snprintf
		add     esp, 16
		cmp     eax, 256
		mov     edx, buffer
		jb      L2
		test    eax, eax
		js      L1
		mov     eax, 256 - 1
		jmp     L2

	L1:
		xor     eax, eax
	L2:
		add     eax, buffer
		lea     ecx, [tmpS]
		push    eax
		call    string_append_range
		add     esp, 256 + 8
	L3:
		jmp     TSSGCtrl_LoopSSRFile_LineListLoopContinue
#endif

		#undef buffer
		#undef outReserved
	}
}

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_LineListLoopContinue()
{
	static const DWORD X0050267C = 0x0050267C;

	__asm
	{
		mov     eax, dword ptr [VIt]
		jmp     dword ptr [X0050267C]

		#undef this
		#undef LoopVal
		#undef VIt
		#undef tmpS
		#undef SSGS
		#undef _Type
	}
}
