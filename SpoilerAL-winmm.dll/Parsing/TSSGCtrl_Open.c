#include <windows.h>
#include "TProcessCtrl.h"
#include "TSSGActionListner.h"

BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);

HANDLE __stdcall TSSGCtrl_OpenProcess(TProcessCtrl *processCtrl, DWORD Mode, LPCSTR addressStr)
{
	return TSSGCtrl_IsRemoteProcess(addressStr) ?
		TProcessCtrl_Open(processCtrl, Mode) :
		GetCurrentProcess();
}

HANDLE __stdcall TSSGCtrl_Open(TSSGCtrl *_this, TSSGSubject *SSGS, DWORD Mode, LPCSTR addressStr)
{
	HANDLE hProcess;

	hProcess = TSSGCtrl_OpenProcess(&_this->processCtrl, Mode, addressStr);
	if (_this->ssgActionListner)
		if (hProcess)
			TSSGActionListner_OnProcessOpen(_this->ssgActionListner, SSGS, Mode);
		else
			TSSGActionListner_OnProcessOpenError(_this->ssgActionListner, SSGS);
	return hProcess;
}

