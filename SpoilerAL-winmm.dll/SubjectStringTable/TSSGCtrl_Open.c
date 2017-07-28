#include <windows.h>
#include "TProcessCtrl.h"
#include "TSSGActionListner.h"

BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);

HANDLE __cdecl TSSGCtrl_Open(TSSGCtrl *_this, TSSGSubject *SSGS, DWORD Mode)
{
	HANDLE hProcess;

	hProcess = TSSGCtrl_IsRemoteProcess(TSSGSubject_GetAddressStr(SSGS)->_M_start) ?
		TProcessCtrl_Open(&_this->processCtrl, Mode) :
		GetCurrentProcess();
	if (_this->ssgActionListner)
		if (hProcess)
			TSSGActionListner_OnProcessOpen(_this->ssgActionListner, SSGS, Mode);
		else
			TSSGActionListner_OnProcessOpenError(_this->ssgActionListner, SSGS);
	return hProcess;
}

