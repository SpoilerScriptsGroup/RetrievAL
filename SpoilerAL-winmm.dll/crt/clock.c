#include <time.h>
#include <windows.h>

#ifndef _M_IX86
clock_t clock()
{
	static ULONGLONG Ratio = 0;
	ULONGLONG Counter;

	if (!Ratio)
	{
		ULONGLONG Frequency;

		if (!QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency))
			goto FAILED;
		if (!(Ratio = ((ULONGLONG)CLOCKS_PER_SEC << 32) / Frequency))
			goto FAILED;
	}
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&Counter))
		goto FAILED;
	return (Counter * Ratio) >> 32;

FAILED:
	return -1;
}
#else
__declspec(naked) clock_t clock()
{
	extern __cdecl _aulldiv();
	extern __cdecl _allmul();

	#define _CLOCKS_PER_SEC 1000	// (clock_t)1000

	static ULONGLONG Ratio = 0;

	__asm
	{
		mov     eax, dword ptr [Ratio]
		mov     edx, dword ptr [Ratio + 4]
		sub     esp, 8
		or      eax, edx
		push    esp
		jnz     L1
		call    QueryPerformanceFrequency
		test    eax, eax
		jz      L2
		push    _CLOCKS_PER_SEC
		push    0
		call    _aulldiv
		mov     ecx, eax
		sub     esp, 8
		or      eax, edx
		jz      L2
		mov     dword ptr [Ratio], ecx
		mov     dword ptr [Ratio + 4], edx
		push    esp
	L1:
		call    QueryPerformanceCounter
		test    eax, eax
		jz      L2
		mov     edx, dword ptr [Ratio + 4]
		mov     eax, dword ptr [Ratio]
		push    edx
		push    eax
		call    _allmul
		mov     eax, edx
		ret

		align   16
	L2:
		add     esp, 8
		dec     eax
		ret
	}

	#undef _CLOCKS_PER_SEC
}
#endif
