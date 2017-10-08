
__declspec(naked) double __cdecl _fpreset()
{
	__asm
	{
		finit                           ; Initialize the FPU
		ret
	}
}
