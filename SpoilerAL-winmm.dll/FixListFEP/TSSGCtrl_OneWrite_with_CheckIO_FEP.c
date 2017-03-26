#include <windows.h>
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

BOOLEAN __cdecl TSSGCtrl_OneWrite_with_CheckIO_FEP(TSSGCtrl *_this, TSSGSubject *SSGS, HANDLE ProcessHandle, DWORD Address, LPVOID Data, DWORD Size)
{
	if (SSGS->isFEP)
		*(LPDWORD)Data = TSSGCtrl_CheckIO_FEP(_this, SSGS, *(LPDWORD)Data, TRUE);
	return TSSGCtrl_OneWrite(_this, SSGS, ProcessHandle, Address, Data, Size);
}
