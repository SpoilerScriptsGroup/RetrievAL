#include <windows.h>

void __cdecl TSSGCtrl_LoopSSRFile_LineListLoopContinue();

extern BOOL EnableParserFix;
extern const DWORD F0043CC08;

__declspec(naked) void __cdecl TSSGCtrl_LoopSSRFile_FixWordRepeat()
{
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
		mov     eax, 0050222BH
		mov     ecx, 00502676H
		test    esi, esi
		je      L5
		jmp     eax
	L5:
		jmp     ecx
	}
}
