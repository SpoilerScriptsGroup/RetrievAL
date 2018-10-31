#ifndef _M_IX86
int __cdecl _finite(double x)
{
	#define MSW(value) ((unsigned long int *)&(value))[1]

	return (MSW(x) & 0x7FF00000) != 0x7FF00000;

	#undef MSW
}
#else
__declspec(naked) int __cdecl _finite(double x)
{
	__asm
	{
		#define MSW (esp + 8)

		mov     ecx, dword ptr [MSW]
		xor     eax, eax
		add     ecx, ecx
		cmp     ecx, 0x7FF00000 * 2
		adc     eax, 0
		ret

		#undef MSW
	}
}
#endif
