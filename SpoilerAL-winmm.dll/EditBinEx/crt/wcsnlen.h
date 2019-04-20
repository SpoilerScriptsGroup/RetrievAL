#ifndef wcsnlen
#define wcsnlen inline_wcsnlen
#if _MSC_VER >= 600
__forceinline
#else
static __inline
#endif
wchar_t *inline_wcsnlen(const wchar_t *string, size_t maxlen)
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
		repne scasw
		setne   al
		xor     ecx, -1
		add     eax, edx
		add     eax, ecx
	L1:
	}
}
#endif
