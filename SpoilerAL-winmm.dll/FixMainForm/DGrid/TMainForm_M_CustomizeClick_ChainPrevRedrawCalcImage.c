
__declspec(naked) void __cdecl TMainForm_M_CustomizeClick_ChainPrevRedrawCalcImage()
{
	__asm
	{
		mov     eax, dword ptr [ebp - 1F4H]
		mov     ecx, 00415948H
		push    eax
		push    0044B312H
		jmp     ecx
	}
}
