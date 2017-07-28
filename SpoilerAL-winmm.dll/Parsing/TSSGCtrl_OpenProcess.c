#include <windows.h>
#include "TProcessCtrl.h"

BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);

HANDLE __stdcall TSSGCtrl_OpenProcess(TProcessCtrl *processCtrl, DWORD Mode, LPCSTR addressStr)
{
	return TSSGCtrl_IsRemoteProcess(addressStr) ?
		TProcessCtrl_Open(processCtrl, Mode) :
		GetCurrentProcess();
}

