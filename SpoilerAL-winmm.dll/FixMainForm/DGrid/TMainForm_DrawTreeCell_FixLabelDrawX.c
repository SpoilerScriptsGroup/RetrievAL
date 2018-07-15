
__declspec(naked) void __cdecl TMainForm_DrawTreeCell_FixLabelDrawX()
{
	__asm
	{
		mov     edx, 00444E79H// break
		mov     eax, 00444E67H
		cmp     byte ptr [edi + 60H], 3
		cmovne  edx, eax
		cmovne  eax, dword ptr [ebx + 518H]
		jmp     edx
	}
}
