#include <windows.h>
#include "TSSGCtrl.h"

HANDLE __stdcall TSSGCtrl_OpenProcess(TProcessCtrl *processCtrl, DWORD Mode, LPCSTR addressStr)
{
	return TSSGCtrl_IsRemoteProcess(addressStr) ?
		TProcessCtrl_Open(processCtrl, Mode) :
		GetCurrentProcess();
}

