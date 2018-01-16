#include <windows.h>

__declspec(naked) void __cdecl TSSGCtrl_Funneling_IsEnabled()
{
	extern BOOL EnableParserFix;

	__asm
	{
		mov     eax, dword ptr [EnableParserFix]
		test    al, al
		jz      IsEnabled
		ret

	IsEnabled:
		mov     eax, 0x005111E0
		jmp     eax
	}
}
