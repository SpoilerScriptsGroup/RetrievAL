#include <windows.h>
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

BOOLEAN __cdecl TSSGCtrl_OneRead_with_CheckIO_FEP(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, LPCVOID Address, LPDWORD Data, DWORD Size)
{
	if (TSSGCtrl_OneRead(this, SSGS, ProcessHandle, (DWORD)Address, Data, Size))
	{
		if (SSGS->withFEP)
			*Data = TSSGCtrl_CheckIO_FEP(this, SSGS, *Data, FALSE);
		return TRUE;
	}
	return FALSE;
}
