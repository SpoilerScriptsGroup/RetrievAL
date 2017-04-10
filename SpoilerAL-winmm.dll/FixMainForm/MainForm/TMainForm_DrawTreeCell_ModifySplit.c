#include <windows.h>
#include "bcb6_std_string.h"
#include "TMainForm.h"
#include "TSSGSubject.h"

EXTERN_C void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, bcb6_std_string *line);

static void __fastcall ModifySplit(
	bcb6_std_string *DrawStr, bcb6_std_string *strParam, TMainForm *_this, TSSGSubject *TSSS);

void __declspec(naked) TMainForm_DrawTreeCell_ModifySplitLabel(bcb6_std_string *DrawStr, bcb6_std_string *strParam)
{
	__asm
	{
		#define ReturnAddress 00444FC0H
		#define DrawStr       ecx
		#define strParam      edx
		#define _this         ebx
		#define SSGS          (ebp - 1E0H)

		mov     eax, dword ptr [SSGS]
		push    _this
		mov     dword ptr [esp + 4], eax
		push    ReturnAddress
		jmp     ModifySplit

		#undef ReturnAddress
		#undef DrawStr
		#undef strParam
		#undef _this
		#undef SSGS
	}
}

void __declspec(naked) TMainForm_DrawTreeCell_ModifySplitRoll(bcb6_std_string *tmpS, bcb6_std_string *strParam)
{
	__asm
	{
		#define ReturnAddress 004451C8H
		#define DrawStr       eax
		#define strParam      ecx
		#define _this         ebx
		#define SSGS          (ebp - 1E0H)

		mov     edx, ecx
		mov     ecx, eax
		mov     eax, dword ptr [SSGS]
		push    _this
		mov     dword ptr [esp + 4], eax
		push    ReturnAddress
		jmp     ModifySplit

		#undef ReturnAddress
		#undef DrawStr
		#undef strParam
		#undef _this
		#undef SSGS
	}
}

static void __fastcall ModifySplit(
	bcb6_std_string *DrawStr, bcb6_std_string *strParam, TMainForm *_this, TSSGSubject *SSGS)
{
	if (!bcb6_std_string_empty(strParam))
	{
		bcb6_std_string_ctor_assign(DrawStr, strParam);
		ReplaceDefineDynamic(SSGS, DrawStr);
	}
	else
	{
		TSSGSubject_GetSubjectName(DrawStr, SSGS, &_this->ssgCtrl);
	}
}

