#if 0
#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSString.h"

static void __fastcall TSSString_Setting_CheckCodePage(TSSString *this, LPCSTR stack[])
{
	this->codePage = CP_ACP;
	size_t length = stack[1] - stack[0];
	if (length == 7)
	{
		if (*(LPDWORD)stack[0] != BSWAP32('unic') || *(LPDWORD)(stack[0] + 4) != BSWAP32('ode\0'))
			return;
		this->codePage = MAXWORD;
		*(LPDWORD)stack[0] = '0000';
		*(LPBYTE)(stack[1] = stack[0] + 4) = '\0';
		this->size &= -2;
	}
	else if (length == 4)
	{
		if (*(LPDWORD)stack[0] != BSWAP32('utf8'))
			return;
		this->codePage = CP_UTF8;
		*(LPDWORD)stack[0] = BSWAP24('00\0');
		stack[1] = stack[0] + 2;
	}
}

void __declspec(naked) Caller_TSSString_Setting_CheckCodePage()
{
	__asm
	{
		#define CallAddress 00415948H
		#define this        ebx
		#define stack       (esp + 8)

		mov     ecx, this
		lea     edx, [stack]
		push    CallAddress
		jmp     TSSString_Setting_CheckUnicode

		#undef CallAddress
		#undef this
		#undef stack
	}
}
#endif
