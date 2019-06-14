#include <stdint.h>

#ifndef _M_IX86
int __cdecl _isnan(double x)
{
	#define LSW(value) ((unsigned long int *)&(value))[0]
	#define MSW(value) ((unsigned long int *)&(value))[1]

	//return (MSW(x) & 0x7FF00000) == 0x7FF00000 && ((MSW(x) & 0x000FFFFF) || LSW(x));
	return (*(uint64_t *)&x & 0x7FFFFFFFFFFFFFFF) >= 0x7FF0000000000001;

	#undef LSW
	#undef MSW
}
#else
__declspec(naked) int __cdecl _isnan(double x)
{
	__asm
	{
		#define LSW (esp + 4)
		#define MSW (esp + 8)

		mov     eax, dword ptr [LSW]
		mov     ecx, dword ptr [MSW]
		and     ecx, 0x7FFFFFFF
		sub     eax, 1
		sbb     ecx, 0x7FF00000
		mov     eax, 1
		sbb     eax, 0
		ret

		#undef LSW
		#undef MSW
	}
}
#endif
