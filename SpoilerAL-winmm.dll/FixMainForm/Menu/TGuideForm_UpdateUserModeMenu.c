void __cdecl UpdateUserModeMenu();

__declspec(naked) void __cdecl TGuideForm_UpdateUserModeMenu()
{
	__asm
	{
		mov     dword ptr [edx + 448H], eax
		push    0048D178H
		jmp     UpdateUserModeMenu
	}
}
