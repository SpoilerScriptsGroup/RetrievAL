#include <windows.h>

void __cdecl TSSGCtrl_LoopSSRFile_LineListLoopContinue();

extern BOOL EnableParserFix;
extern const DWORD F0043CC08;

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_FixWordRepeat()
{
	static const DWORD X0050222B = 0x0050222B;
	static const DWORD X00502676 = 0x00502676;

	__asm
	{
		mov     eax, dword ptr [EnableParserFix]
		mov     dword ptr [ebp - 204H], ecx
		test    eax, eax
		jz      L4
		test    esi, esi
		jle     L3
		xor     edi, edi
		jmp     L2

	L1:
		mov     edx, dword ptr [ebp - 204H]
		lea     ecx, [ebp - 200H]
		mov     eax, dword ptr [edx + 4H]
		push    ecx
		push    eax
		mov     edx, dword ptr [edx]
		push    edx
		lea     ecx, [ebp - 78H]
		push    ecx
		call    dword ptr [F0043CC08]
		add     esp, 16
		inc     edi
	L2:
		cmp     esi, edi
		ja      L1
	L3:
		jmp     TSSGCtrl_LoopSSRFile_LineListLoopContinue

	L4:
		test    esi, esi
		je      L5
		jmp     dword ptr [X0050222B]

	L5:
		jmp     dword ptr [X00502676]
	}
}
