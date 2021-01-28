#define USING_NAMESPACE_BCB6_STD
#include "TMainForm.h"

void __stdcall FormatNameString(TSSGCtrl *this, TSSGSubject *SSGS, string *s);

static void __fastcall ModifyLockName(TMainForm *this, TSSGSubject *SSGS, string *tmpS, string *lockStr)
{
	if (string_length(lockStr) != 1 || *string_begin(lockStr) != '@')
	{
		string_ctor_assign(tmpS, lockStr);
		FormatNameString(&this->ssgCtrl, SSGS, tmpS);
	}
	else
	{
		TSSGSubject_GetSubjectName(tmpS, this->selectSubject, &this->ssgCtrl);
	}
}

__declspec(naked) void __stdcall Caller_TMainForm_SetLockVisible_ModifyLockName(string *dest, string *src)
{
	__asm
	{
		#define this ebx
		#define SSGS edi
		#define dest edx
		#define src  eax

		mov     edx, SSGS
		mov     ecx, this
		jmp     ModifyLockName

		#undef this
		#undef SSGS
		#undef dest
		#undef src
	}
}
