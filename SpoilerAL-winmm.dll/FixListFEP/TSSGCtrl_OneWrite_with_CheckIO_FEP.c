#include <windows.h>
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

BOOLEAN __cdecl TSSGCtrl_OneWrite_with_CheckIO_FEP(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPDWORD Data, DWORD Size)
{
	extern BOOL FixTheProcedure;
	unsigned long Val = *Data;
	if (SSGS->isFEP) {
		Val = TSSGCtrl_CheckIO_FEP(this, SSGS, Val, TRUE);
		if (!FixTheProcedure)
			*Data = Val;
	}
	return TSSGCtrl_OneWrite(this, SSGS, ProcessHandle, Address, &Val, Size);
}
