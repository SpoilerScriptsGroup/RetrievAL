
__declspec(naked) void __cdecl TMainForm_DrawTreeCell_FixLabelDrawX()
{
	__asm
	{
		mov     dl, byte ptr [edi + 60H]
		mov     eax, 00444E2DH
		cmp     dl, 3
		jne     L1
		mov     eax, 00444E79H
	L1:
		mov     edx, edi
		jmp     eax
	}
}
