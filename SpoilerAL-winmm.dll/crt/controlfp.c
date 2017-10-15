#include <float.h>

static unsigned short __fastcall get_word(unsigned int flag);
static unsigned int __fastcall get_flag(unsigned short word);

__declspec(naked) unsigned int __cdecl _controlfp(unsigned int _new, unsigned int mask)
{
	__asm
	{
		push    ecx
		fstcw   word ptr [esp]
		mov     cx, word ptr [esp]
		call    get_flag
		mov     ecx, dword ptr [esp + 8]
		mov     edx, dword ptr [esp + 12]
		and     ecx, edx
		xor     edx, -1
		and     eax, edx
		or      ecx, eax
		mov     dword ptr [esp + 8], ecx
		call    get_word
		mov     cx, word ptr [esp]
		and     cx, 0E0C0H
		or      cx, ax
		mov     eax, dword ptr [esp + 8]
		mov     word ptr [esp], cx
		fstcw   word ptr [esp]
		pop     ecx
		ret
	}
}

static unsigned short __fastcall get_word(unsigned int flag)
{
	unsigned short word;

	word = 0;
	if (flag & _EM_INVALID   )       word |= 0x0001;
	if (flag & _EM_DENORMAL  )       word |= 0x0002;
	if (flag & _EM_ZERODIVIDE)       word |= 0x0004;
	if (flag & _EM_OVERFLOW  )       word |= 0x0008;
	if (flag & _EM_UNDERFLOW )       word |= 0x0010;
	if (flag & _EM_INEXACT   )       word |= 0x0020;
	if (flag & _RC_DOWN      )       word |= 0x0400;
	if (flag & _RC_UP        )       word |= 0x0800;
	if (flag & _IC_AFFINE    )       word |= 0x1000;
	if ((flag &= _MCW_PC) == _PC_53) word |= 0x0200;
	else if (flag != _PC_24)         word |= 0x0300;
	return word;
}

static unsigned int __fastcall get_flag(unsigned short word)
{
	unsigned int flag;

	flag = 0;
	if (word & 0x0001)       flag |= _EM_INVALID   ;
	if (word & 0x0002)       flag |= _EM_DENORMAL  ;
	if (word & 0x0004)       flag |= _EM_ZERODIVIDE;
	if (word & 0x0008)       flag |= _EM_OVERFLOW  ;
	if (word & 0x0010)       flag |= _EM_UNDERFLOW ;
	if (word & 0x0020)       flag |= _EM_INEXACT   ;
	if (word & 0x0400)       flag |= _RC_DOWN      ;
	if (word & 0x0800)       flag |= _RC_UP        ;
	if (word & 0x1000)       flag |= _IC_AFFINE    ;
	if (!(word &= 0x0300))   flag |= _PC_24        ;
	else if (word == 0x0200) flag |= _PC_53        ;
	return flag;
}
