#ifndef strnlen
#define strnlen inline_strnlen
#if _MSC_VER >= 600
__forceinline
#else
static __inline
#endif
char *inline_strnlen(const char *string, size_t maxlen)
{
	__asm
	{
		mov     eax, dword ptr [maxlen]
		test    eax, eax
		jz      L1
		mov     edi, dword ptr [string]
		mov     edx, eax
		mov     ecx, eax
		xor     eax, eax
		repne   scasb
		setne   al
		xor     ecx, -1
		add     eax, edx
		add     eax, ecx
	L1:
	}
}
#endif
