#include <windows.h>
#include "bcb6_std_string.h"
#include "TMainForm.h"
#include "TSSGSubject.h"

EXTERN_C void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, bcb6_std_string *line);

static void __fastcall ModifySplit(bcb6_std_string *dest, bcb6_std_string *src, TMainForm *_this, TSSGSubject *TSSS);

void __declspec(naked) TMainForm_DrawTreeCell_ModifySplitRoll(bcb6_std_string *tmpS, bcb6_std_string *strParam)
{
	__asm
	{
		#define ReturnAddress 00444FC0H
		#define _this         ebx
		#define SSGS          (ebp - 1E0H)
		#define tmpS          eax
		#define strParam      ecx

		mov     edx, ecx
		mov     ecx, eax
		mov     eax, dword ptr [SSGS]
		push    _this
		mov     dword ptr [esp + 4], eax
		push    ReturnAddress
		jmp     ModifySplit

		#undef ReturnAddress
		#undef _this
		#undef SSGS
		#undef tmpS
		#undef strParam
	}
}

void __declspec(naked) TMainForm_DrawTreeCell_ModifySplitLabel(bcb6_std_string *DrawStr, bcb6_std_string *strParam)
{
	__asm
	{
		#define ReturnAddress 004451C8H
		#define _this         ebx
		#define SSGS          (ebp - 1E0H)
		#define DrawStr       ecx
		#define strParam      edx

		mov     eax, dword ptr [SSGS]
		push    _this
		mov     dword ptr [esp + 4], eax
		push    ReturnAddress
		jmp     ModifySplit

		#undef ReturnAddress
		#undef _this
		#undef SSGS
		#undef DrawStr
		#undef strParam
	}
}

static void __fastcall ModifySplit(bcb6_std_string *dest, bcb6_std_string *src, TMainForm *_this, TSSGSubject *SSGS)
{
	if (!bcb6_std_string_empty(src))
	{
		bcb6_std_string_ctor_assign(dest, src);
		ReplaceDefineDynamic(SSGS, dest);
	}
	else
	{
		TSSGSubject_GetSubjectName(dest, SSGS, &_this->ssgCtrl);
	}
}

