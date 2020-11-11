#ifndef _M_IX86
char * __cdecl _strinc(const char *current)
{
	return (char *)current + 1;
}
#else
__declspec(naked) char * __cdecl _strinc(const char *current)
{
	__asm
	{
		#define current (esp + 4)

		mov     eax, dword ptr [current]
		inc     eax
		ret

		#undef current
	}
}
#endif
