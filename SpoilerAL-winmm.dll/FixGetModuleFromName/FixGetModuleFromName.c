#include "TProcessCtrl.h"
#include "TMainForm.h"

TProcessCtrl *__fastcall FixGetModuleFromName(TProcessCtrl *const this)
{
	LPSTR        lpBuffer;
	HANDLE const hProcess = TProcessCtrl_Open(this, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);
	if (hProcess)
		CloseHandle(hProcess);
	else if (TMainForm_GetUserMode(MainForm) >= 3 &&
			 TSSGCtrl_GetSSGActionListner(&MainForm->ssgCtrl) &&
			 FormatMessageA(
				 FORMAT_MESSAGE_MAX_WIDTH_MASK |
				 FORMAT_MESSAGE_ALLOCATE_BUFFER |
				 FORMAT_MESSAGE_IGNORE_INSERTS |
				 FORMAT_MESSAGE_FROM_SYSTEM,
				 NULL,
				 GetLastError(),
				 0,
				 (LPSTR)&lpBuffer,
				 sizeof(double),
				 NULL))
	{
		TMainForm_Guide(lpBuffer, 0);
		LocalFree(lpBuffer);
	}
	return this;
}
