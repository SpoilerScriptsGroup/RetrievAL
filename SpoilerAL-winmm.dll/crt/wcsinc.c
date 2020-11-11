#include <wchar.h>

#ifndef _M_IX86
wchar_t * __cdecl _wcsinc(const wchar_t *current)
{
	return (wchar_t *)current + 1;
}
#else
__declspec(naked) wchar_t * __cdecl _wcsinc(const wchar_t *current)
{
	__asm
	{
		#define current (esp + 4)

		mov     eax, dword ptr [current]
		add     eax, 2
		ret

		#undef current
	}
}
#endif
