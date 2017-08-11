#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TMainForm.h"

void __stdcall FormatNameString(TSSGCtrl *_this, TSSGSubject *SSGS, string *s);

static void __stdcall ModifyLockName(TMainForm *_this, TSSGSubject *SSGS, string *tmpS, string *LockName);

__declspec(naked) void __cdecl Caller_TMainForm_SetLockVisible_ModifyLockName(string *dest, string *src)
{
	__asm
	{
		#define _this ebx
		#define SSGS  edi
		#define dest  edx
		#define src   eax

		pop     ecx
		push    src
		push    dest
		push    SSGS
		push    _this
		push    ecx
		jmp     ModifyLockName

		#undef _this
		#undef dest
		#undef src
	}
}

static void __stdcall ModifyLockName(TMainForm *_this, TSSGSubject *SSGS, string *tmpS, string *lockName)
{
	if (string_length(lockName) != 1 || *string_begin(lockName) != '@')
	{
		string_ctor_assign(tmpS, lockName);
		FormatNameString(&_this->ssgCtrl, SSGS, tmpS);
	}
	else
	{
		TSSGSubject_GetSubjectName(tmpS, _this->selectSubject, &_this->ssgCtrl);
	}
}
