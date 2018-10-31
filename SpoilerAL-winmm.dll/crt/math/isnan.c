#ifndef _M_IX86
int __cdecl _isnan(double x)
{
	#define LSW(value) ((unsigned long int *)&(value))[0]
	#define MSW(value) ((unsigned long int *)&(value))[1]

	return (MSW(x) & 0x7FF00000) == 0x7FF00000 && ((MSW(x) & 0x000FFFFF) || LSW(x));

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

		mov     edx, dword ptr [LSW]
		mov     ecx, dword ptr [MSW]
		xor     eax, eax
		add     ecx, ecx
		sub     ecx, 0x7FF00000 * 2
		jb      L1
		or      ecx, edx
		setnz   al
	L1:
		ret

		#undef LSW
		#undef MSW
	}
}
#endif
