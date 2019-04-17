__declspec(naked) void __cdecl _chkesp()
{
	__asm
	{
		jnz     L1
		int     3
	L1:
		ret
	}
}
