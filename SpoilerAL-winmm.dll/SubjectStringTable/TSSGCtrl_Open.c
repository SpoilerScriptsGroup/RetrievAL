#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGActionListner.h"
#include "SubjectStringOperator.h"

static __inline const string *GetAddressStrPointer(TSSGSubject *SSGS)
{
	const string *s;

	switch (SSGS->type)
	{
	case stCALC:
	case stLIST:
	case stB_CALC:
	case stB_LIST:
		s = (string *)((LPBYTE)SSGS + 136);
		break;
	case stTOGGLE:
	case stB_TOGGLE:
		s = (string *)((LPBYTE)SSGS + 120);
		break;
	case stSTRING:
	case stBITLIST:
	case stCOPY:
		s = (string *)((LPBYTE)SSGS + 128);
		break;
	case stD_LIST:
	case stD_TOGGLE:
	case stFLOAT_CALC:
	case stB_FLOAT_CALC:
		s = (string *)((LPBYTE)SSGS + 152);
		break;
	case stTRACE:
		s = (string *)((LPBYTE)SSGS + 96);
		break;
	default:
		return NULL;
	}
	return SubjectStringTable_GetString(s);
}

HANDLE __cdecl TSSGCtrl_Open(TSSGCtrl *this, TSSGSubject *SSGS, DWORD Mode)
{
	HANDLE       hProcess;
	const string *addressStr;

	addressStr = GetAddressStrPointer(SSGS);
	hProcess = (!addressStr || TSSGCtrl_IsRemoteProcess(string_c_str(addressStr))) ?
		TProcessCtrl_Open(&this->processCtrl, Mode) :
		GetCurrentProcess();
	if (this->ssgActionListner && this->script.sPos)
		if (hProcess)
			TSSGActionListner_OnProcessOpen(this->ssgActionListner, SSGS, Mode);
		else
			TSSGActionListner_OnProcessOpenError(this->ssgActionListner, SSGS);
	return hProcess;
}

