#include <windows.h>

extern const BOOL EnableParserFix;
extern const DWORD __InitExceptBlockLDTC;

__declspec(naked) void __cdecl Caller_StrToProcessAccessElementVecWithoutRel()
{
	static const DWORD X00506F81 = 0x00506F81;

	__asm
	{
		cmp     dword ptr [EnableParserFix], 0
		je      L2
		mov     ecx, dword ptr [esp +  4]
		mov     edx, dword ptr [esp     ]
		movups  xmm0, [esp + 8]
		movups  xmm1, [esp + 8 + 0x10]
		push    ecx
		push    offset L1
		movups  xmmword ptr [esp + 8       ], xmm0
		movups  xmmword ptr [esp + 8 + 0x10], xmm1
		mov     dword ptr   [esp + 8 + 0x20], 0
		mov     dword ptr   [esp + 8 + 0x24], edx
	L2:
		push    ebp
		mov     ebp, esp
		sub     esp, 4092
		jmp     dword ptr [X00506F81]

		align   16
	L1:
		add     esp, 8
		jmp     dword ptr [esp + 32]
	}
}
