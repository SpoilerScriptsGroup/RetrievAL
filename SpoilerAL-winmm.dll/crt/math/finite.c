#include <stdint.h>

#ifndef _M_IX86
int __cdecl _finite(double x)
{
	#define LSW(value) ((unsigned long int *)&(value))[0]
	#define MSW(value) ((unsigned long int *)&(value))[1]

	//return (*(uint64_t *)&x & 0x7FF0000000000000) != 0x7FF0000000000000;
	return (MSW(x) & 0x7FF00000) != 0x7FF00000;

	#undef LSW
	#undef MSW
}
#else
__declspec(naked) int __cdecl _finite(double x)
{
	__asm
	{
		#define LSW (esp + 4)
		#define MSW (esp + 8)

		mov     ecx, dword ptr [MSW]
		xor     eax, eax
		add     ecx, ecx
		cmp     ecx, 0x7FF00000 * 2
		adc     eax, 0
		ret

		#undef LSW
		#undef MSW
	}
}
#endif
