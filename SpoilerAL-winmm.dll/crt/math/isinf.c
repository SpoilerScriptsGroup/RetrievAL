#ifndef _M_IX86
int __cdecl isinf(double x)
{
	#define LSW(value) ((unsigned long int *)&(value))[0]
	#define MSW(value) ((unsigned long int *)&(value))[1]

	return (MSW(x) & 0x7FFFFFFF) == 0x7FF00000 && LSW(x) == 0x00000000;

	#undef LSW
	#undef MSW
}
#else
__declspec(naked) int __cdecl isinf(double x)
{
	__asm
	{
		#define LSW (esp + 4)
		#define MSW (esp + 8)

		mov     edx, dword ptr [LSW]
		mov     ecx, dword ptr [MSW]
		xor     eax, eax
		add     ecx, ecx
		xor     ecx, 0x7FF00000 * 2
		or      ecx, edx
		setz    al
		ret

		#undef LSW
		#undef MSW
	}
}
#endif
