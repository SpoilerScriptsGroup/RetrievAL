#include <windows.h>

__declspec(naked) void __cdecl TMainForm_SubjectAccess_ToULongDef()
{
	static const DWORD JumpTable[] = {
		0x004C5209,
		0x004C627D,
		0x004C7A1E,
	};

	__asm
	{
		and     ecx, 0FFH
		lea     edx, [esp + 8]
		push    ecx
		push    0
		push    edx
		push    eax
		call    strtoul
		mov     ecx, dword ptr [esp + 16 + 4]
		mov     edx, dword ptr [esp + 16 + 8]
		add     esp, 12
		cmp     ecx, edx
		pop     ecx
		je      L1
		ret

		align   16
	L1:
		mov     ecx, dword ptr [JumpTable + ecx]
		add     esp, 12
		jmp     ecx
	}
}
