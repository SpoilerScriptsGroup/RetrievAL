#include "TMainForm.h"

static void __cdecl ModifyLockName(TMainForm * _this, TSSGSubject *SSGS, bcb6_std_string* tmpS, bcb6_std_string* LockName);

__declspec(naked) void __cdecl Caller_TMainForm_SetLockVisible_ModifyLockName(bcb6_std_string* dest, bcb6_std_string* src)
{
	__asm
	{
		#define _this ebx
		#define SSGS  edi
		#define dest  edx
		#define src   eax

		push    src
		push    dest
		push    SSGS
		push    _this
		call    ModifyLockName
		add     esp, 16
		ret

		#undef _this
		#undef SSGS
		#undef dest
		#undef src
	}
}

static void __cdecl ModifyLockName(TMainForm * _this, TSSGSubject *SSGS, bcb6_std_string* tmpS, bcb6_std_string* lockName)
{
	if (bcb6_std_string_length(lockName) != 1 || *bcb6_std_string_begin(lockName) != '@')
		bcb6_std_string_ctor_assign(tmpS, lockName);
	else
		TSSGSubject_GetSubjectName(tmpS, SSGS, &_this->ssgCtrl);
}
