#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"

static unsigned long __stdcall CheckFunnel(TSSGCtrl *SSGC, TSSGSubject *SSGS, string *WriteString);

__declspec(naked) void __cdecl TSSString_Write_CheckFunnel()
{
	__asm
	{
		#define ReturnAddress 0052B563H
		#define SSGC          (ebp + 0CH)
		#define SSGS          (ebp + 8H)
		#define Str           (ebp - 1CH)

		mov     edx, dword ptr [SSGS]
		lea     eax, [Str]
		mov     ecx, dword ptr [SSGC]
		push    eax
		push    edx
		push    ecx
		push    ReturnAddress
		jmp     CheckFunnel

		#undef ReturnAddress
		#undef SSGC
		#undef SSGS
		#undef Str
	}
}

#define ssgCtrl_reNO_ERROR 0

static unsigned long __stdcall CheckFunnel(TSSGCtrl *SSGC, TSSGSubject *SSGS, string *Str)
{
	extern BOOL FixTheProcedure;
	size_t length = string_length(Str);
	unsigned long Val = FixTheProcedure ? length :
		(length >  2) ? *(LPDWORD)string_begin(Str) :
		(length == 2) ? *(LPWORD )string_c_str(Str) :
		(length != 0) ? *(LPBYTE )string_c_str(Str) :
		0;
	TSSGCtrl_CheckFunnel(SSGC, SSGS, Val);
	return ssgCtrl_reNO_ERROR;
}

