#if 0
#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSString.h"

static void __fastcall TSSString_Setting_CheckUnicode(TSSString *_this, LPCSTR stack[]);

void __declspec(naked) Caller_TSSString_Setting_CheckUnicode()
{
	__asm
	{
		#define CallAddress 00415948H
		#define _this       ebx
		#define stack       (esp + 8)

		mov     ecx, _this
		lea     edx, [stack]
		push    CallAddress
		jmp     TSSString_Setting_CheckUnicode

		#undef CallAddress
		#undef _this
		#undef stack
	}
}

static void __fastcall TSSString_Setting_CheckUnicode(TSSString *_this, LPCSTR stack[])
{
	_this->isUnicode = (
		stack[1] - stack[0] == 7 &&
		*(LPDWORD) stack[0]      == BSWAP32('unic') &&
		*(LPDWORD)(stack[0] + 4) == BSWAP32('ode\0'));
	if (_this->isUnicode)
	{
		*(LPDWORD)stack[0] = '0000';
		*(LPBYTE)(stack[1] = stack[0] + 4) = '\0';
		_this->size &= -2;
	}
}
#endif
