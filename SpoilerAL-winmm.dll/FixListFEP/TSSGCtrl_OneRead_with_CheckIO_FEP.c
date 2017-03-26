#include <windows.h>
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

BOOLEAN __cdecl TSSGCtrl_OneRead_with_CheckIO_FEP(TSSGCtrl *_this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size)
{
	if (!TSSGCtrl_OneRead(_this, SSGS, ProcessHandle, Address, Data, Size))
		return FALSE;
	if (SSGS->isFEP)
		*(LPDWORD)Data = TSSGCtrl_CheckIO_FEP(_this, SSGS, *(LPDWORD)Data, FALSE);
	return TRUE;
}
