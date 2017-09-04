#if 0
#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TMainForm.h"
#include "TSSGSubject.h"

EXTERN_C void __stdcall ReplaceDefineDynamic(TSSGSubject *SSGS, string *line);

static void __fastcall ModifySplit(string *dest, string *src, TMainForm *this, TSSGSubject *TSSS);

void __declspec(naked) TMainForm_DrawTreeCell_ModifySplitRoll(string *tmpS, string *strParam)
{
	__asm
	{
		#define ReturnAddress 00444FC0H
		#define this          ebx
		#define SSGS          (ebp - 1E0H)
		#define tmpS          eax
		#define strParam      ecx

		mov     edx, ecx
		mov     ecx, eax
		mov     eax, dword ptr [SSGS]
		push    this
		mov     dword ptr [esp + 4], eax
		push    ReturnAddress
		jmp     ModifySplit

		#undef ReturnAddress
		#undef this
		#undef SSGS
		#undef tmpS
		#undef strParam
	}
}

void __declspec(naked) TMainForm_DrawTreeCell_ModifySplitLabel(string *DrawStr, string *strParam)
{
	__asm
	{
		#define ReturnAddress 004451C8H
		#define this          ebx
		#define SSGS          (ebp - 1E0H)
		#define DrawStr       ecx
		#define strParam      edx

		mov     eax, dword ptr [SSGS]
		push    this
		mov     dword ptr [esp + 4], eax
		push    ReturnAddress
		jmp     ModifySplit

		#undef ReturnAddress
		#undef this
		#undef SSGS
		#undef DrawStr
		#undef strParam
	}
}

static void __fastcall ModifySplit(string *dest, string *src, TMainForm *this, TSSGSubject *SSGS)
{
	if (!string_empty(src))
	{
		string_ctor_assign(dest, src);
		ReplaceDefineDynamic(SSGS, dest);
	}
	else
	{
		TSSGSubject_GetSubjectName(dest, SSGS, &this->ssgCtrl);
	}
}
#endif
