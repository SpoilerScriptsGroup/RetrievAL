void __cdecl UpdateUserModeMenu();

__declspec(naked) void __cdecl TMainForm_LoadSetting_SetUserMode()
{
	__asm
	{
		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		dec     dword ptr [ebx + 1CH]
		push    00437E31H
		jmp     UpdateUserModeMenu
	}
}
