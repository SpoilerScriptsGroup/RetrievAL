
__declspec(naked) void __cdecl TMainForm_M_TitleSelectClick_OpenSSG()
{
	/*
	nowSelectSubjectIndex = -2;
	ssgCtrl.OpenSSG(&selectScript);
	*/

	__asm
	{
		mov     ecx, 004FCE80H
		mov     dword ptr [ebx + 554H], -2
		jmp     ecx
	}
}
