#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"

static unsigned long __stdcall CheckFunnel(TSSGCtrl *SSGC, TSSGSubject *SSGS, const string *Str)
{
	extern BOOL FixTheProcedure;

	const size_t length = string_length(Str);
	const unsigned long Val = FixTheProcedure ? length :
		(length >  2) ? *(LPDWORD)string_begin(Str) :
		(length == 2) ? *(LPWORD )string_c_str(Str) :
		(length != 0) ? *(LPBYTE )string_c_str(Str) :
		0;
	TSSGCtrl_CheckFunnel(SSGC, SSGS, Val);
	return reNO_ERROR;
}

__declspec(naked) void __cdecl TSSString_Write_CheckFunnel()
{
	__asm
	{
		#define SSGC          (ebp + 0x0C)
		#define SSGS          (ebp + 0x08)
		#define Str           (ebp - 0x1C)

		mov     edx, dword ptr [SSGS]
		lea     eax, [Str]
		mov     ecx, dword ptr [SSGC]
		push    eax
		push    edx
		push    ecx
		call    CheckFunnel

		#undef Str
		#undef SSGS
		#undef SSGC
	}
}

