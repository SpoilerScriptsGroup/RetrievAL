#include <windows.h>
#include "intrinsic.h"
#include "TProcessCtrl.h"
#include "TSSGActionListner.h"

#ifndef _M_IX86
BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p)
{
	char c;

	do
	{
		c = *(p++);
	} while (__intrinsic_isspace(c));
	if (c == '_')
		do
		{
			c = *(p++);
		} while (__intrinsic_isspace(c));
	if (c == 'L')
	{
		c = *p;
		if (__intrinsic_isspace(c) || c == '{')
			return FALSE;
	}
	return TRUE;
}
#else
__declspec(naked) BOOLEAN __fastcall TSSGCtrl_IsRemoteProcess(LPCSTR p)
{
	__asm
	{
	L1:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L1
		cmp     al, '\r'
		ja      L2
		cmp     al, '\t'
		jae     L1
		jmp     L5
	L2:
		cmp     al, '_'
		jne     L4
	L3:
		mov     al, byte ptr [ecx]
		inc     ecx
		cmp     al, ' '
		je      L3
		cmp     al, '\r'
		ja      L4
		cmp     al, '\t'
		jae     L3
		jmp     L5
	L4:
		cmp     al, 'L'
		jne     L5
		mov     cl, byte ptr [ecx]
		xor     al, al
		cmp     cl, ' '
		je      L6
		cmp     cl, '{'
		je      L6
		cmp     cl, '\r'
		ja      L5
		cmp     cl, '\t'
		jae     L6
	L5:
		mov     al, 1
	L6:
		ret
	}
}
#endif

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

