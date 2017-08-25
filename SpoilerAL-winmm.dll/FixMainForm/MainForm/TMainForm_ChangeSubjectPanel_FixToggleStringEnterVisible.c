#include <windows.h>

extern const DWORD F00580BBC;

__declspec(naked) void __cdecl TMainForm_ChangeSubjectPanel_FixToggleStringEnterVisible()
{
	__asm
	{
		call    dword ptr [F00580BBC]
		xor     edx, edx
		mov     eax, dword ptr [ebx + 3E0H]
		jmp     dword ptr [F00580BBC]
	}
}
