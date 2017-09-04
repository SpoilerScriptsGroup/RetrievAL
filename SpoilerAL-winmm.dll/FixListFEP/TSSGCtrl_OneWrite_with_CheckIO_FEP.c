#include <windows.h>
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

BOOLEAN __cdecl TSSGCtrl_OneWrite_with_CheckIO_FEP(TSSGCtrl *this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size)
{
	if (SSGS->isFEP)
		*(LPDWORD)Data = TSSGCtrl_CheckIO_FEP(this, SSGS, *(LPDWORD)Data, TRUE);
	return TSSGCtrl_OneWrite(this, SSGS, ProcessHandle, Address, Data, Size);
}
