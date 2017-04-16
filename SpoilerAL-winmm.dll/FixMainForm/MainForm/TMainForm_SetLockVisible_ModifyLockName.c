#include "TMainForm.h"

static void __stdcall ModifyLockName(TMainForm * _this, bcb6_std_string* tmpS, bcb6_std_string* LockName);

__declspec(naked) void __cdecl Caller_TMainForm_SetLockVisible_ModifyLockName(bcb6_std_string* dest, bcb6_std_string* src)
{
	__asm
	{
		#define _this ebx
		#define dest  edx
		#define src   eax

		pop     ecx
		push    src
		push    dest
		push    _this
		push    ecx
		jmp     ModifyLockName

		#undef _this
		#undef dest
		#undef src
	}
}

static void __stdcall ModifyLockName(TMainForm * _this, bcb6_std_string* tmpS, bcb6_std_string* lockName)
{
	if (bcb6_std_string_length(lockName) != 1 || *bcb6_std_string_begin(lockName) != '@')
		bcb6_std_string_ctor_assign(tmpS, lockName);
	else
		TSSGSubject_GetSubjectName(tmpS, _this->selectSubject, &_this->ssgCtrl);
}
