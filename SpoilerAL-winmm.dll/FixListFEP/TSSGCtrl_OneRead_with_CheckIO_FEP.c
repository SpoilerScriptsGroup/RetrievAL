#include <windows.h>
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

BOOLEAN __cdecl TSSGCtrl_OneRead_with_CheckIO_FEP(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size)
{
	if (!TSSGCtrl_OneRead(this, SSGS, ProcessHandle, Address, Data, Size))
		return FALSE;
	if (SSGS->isFEP)
		*(LPDWORD)Data = TSSGCtrl_CheckIO_FEP(this, SSGS, *(LPDWORD)Data, FALSE);
	return TRUE;
}
