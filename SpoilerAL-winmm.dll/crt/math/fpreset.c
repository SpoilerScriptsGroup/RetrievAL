
__declspec(naked) void __cdecl _fpreset()
{
	__asm
	{
		finit                           ; Initialize the FPU
		ret
	}
}
