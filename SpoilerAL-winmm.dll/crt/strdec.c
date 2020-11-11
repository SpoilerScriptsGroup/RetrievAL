#ifndef _M_IX86
char * __cdecl _strdec(const char *start, const char *current)
{
	return start < current ? (char *)current - 1 : NULL;
}
#else
__declspec(naked) char * __cdecl _strdec(const char *start, const char *current)
{
	__asm
	{
		#define start   (esp + 4)
		#define current (esp + 8)

		mov     ecx, dword ptr [start]
		mov     eax, dword ptr [current]
		cmp     ecx, eax
		jae     L1
		dec     eax
		ret

		align   16
	L1:
		xor     eax, eax
		ret

		#undef start
		#undef current
	}
}
#endif
