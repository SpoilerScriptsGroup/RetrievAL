#include <windows.h>
#include "TProcessCtrl.h"
#include "TSSGActionListner.h"

BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p);
bcb6_std_string * __fastcall SubjectStringTable_GetString(bcb6_std_string *s);

enum ssgCtrl {
	stNONE = 0,
	stDIR,
	stCALC,
	stTOGGLE,
	stLIST,
	stSTRING,
	stBITLIST,
	stB_TOGGLE,
	stB_CALC,
	stB_LIST,
	stD_LIST,
	stTRACE,
	stD_TOGGLE,
	stCOPY,
	stFLOAT_CALC,
	stB_FLOAT_CALC,
	stSPLIT,
};

static __inline const bcb6_std_string *GetAddressStrPointer(TSSGSubject *SSGS)
{
	bcb6_std_string *s;

	switch (SSGS->type)
	{
	case stCALC:
	case stLIST:
	case stB_CALC:
	case stB_LIST:
		s = (bcb6_std_string *)((LPBYTE)SSGS + 136);
		break;
	case stTOGGLE:
	case stB_TOGGLE:
		s = (bcb6_std_string *)((LPBYTE)SSGS + 120);
		break;
	case stSTRING:
	case stCOPY:
	case stBITLIST:
		s = (bcb6_std_string *)((LPBYTE)SSGS + 128);
		break;
	case stD_LIST:
	case stD_TOGGLE:
	case stFLOAT_CALC:
	case stB_FLOAT_CALC:
		s = (bcb6_std_string *)((LPBYTE)SSGS + 152);
		break;
	case stTRACE:
		s = (bcb6_std_string *)((LPBYTE)SSGS + 96);
		break;
	default:
		return NULL;
	}
	return SubjectStringTable_GetString(s);
}

HANDLE __cdecl TSSGCtrl_Open(TSSGCtrl *_this, TSSGSubject *SSGS, DWORD Mode)
{
	HANDLE                hProcess;
	const bcb6_std_string *addressStr;

	addressStr = GetAddressStrPointer(SSGS);
	hProcess = (!addressStr || TSSGCtrl_IsRemoteProcess(addressStr->_M_start)) ?
		TProcessCtrl_Open(&_this->processCtrl, Mode) :
		GetCurrentProcess();
	if (_this->ssgActionListner)
		if (hProcess)
			TSSGActionListner_OnProcessOpen(_this->ssgActionListner, SSGS, Mode);
		else
			TSSGActionListner_OnProcessOpenError(_this->ssgActionListner, SSGS);
	return hProcess;
}

