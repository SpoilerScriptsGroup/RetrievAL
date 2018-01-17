
__declspec(naked) void __cdecl FixAdjustByValue()
{
	__asm
	{
		mov     cl, byte ptr [ebx + 78H]
		cmp     cl, 3
		ja      L1
		neg     cl
		and     cl, 3
		shl     cl, 3
		shl     eax, cl
	L1:
		bswap   eax
		mov     dword ptr [esp + 8], eax
		mov     eax, 004AE7A0H
		jmp     eax
	}
}
