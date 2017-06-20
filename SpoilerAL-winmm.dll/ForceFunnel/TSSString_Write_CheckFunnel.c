#include <windows.h>
#include "TSSGCtrl.h"

static unsigned long __stdcall CheckFunnel(TSSGCtrl *SSGC, TSSGSubject *SSGS, bcb6_std_string *WriteString);

__declspec(naked) void __cdecl TSSString_Write_CheckFunnel()
{
	__asm
	{
		#define ReturnAddress 0052B563H
		#define SSGC          (ebp + 0CH)
		#define SSGS          (ebp + 8H)
		#define WriteString   (ebp - 1CH)

		mov     edx, dword ptr [SSGS]
		lea     eax, [WriteString]
		mov     ecx, dword ptr [SSGC]
		push    eax
		push    edx
		push    ecx
		push    ReturnAddress
		jmp     CheckFunnel

		#undef ReturnAddress
		#undef SSGC
		#undef SSGS
		#undef WriteString
	}
}

#define ssgCtrl_reNO_ERROR 0

static unsigned long __stdcall CheckFunnel(TSSGCtrl *SSGC, TSSGSubject *SSGS, bcb6_std_string *WriteString)
{
	size_t length = bcb6_std_string_length(WriteString);
	unsigned long Val =
		(length >  2) ? *(LPDWORD)WriteString->_M_start :
		(length == 2) ? *(LPWORD )WriteString->_M_start :
		(length != 0) ? *(LPBYTE )WriteString->_M_start :
		0;
	TSSGCtrl_CheckFunnel(SSGC, SSGS, Val);
	return ssgCtrl_reNO_ERROR;
}

