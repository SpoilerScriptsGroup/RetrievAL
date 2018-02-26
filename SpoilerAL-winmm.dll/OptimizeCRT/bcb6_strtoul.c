#include <stdlib.h>
#include "bcb6_errno.h"

EXTERN_C unsigned __int64 __stdcall internal_strtoi64(BOOL is_unsigned, BOOL is_int64, errno_t *errnoptr, const char *nptr, char **endptr, int base);

__declspec(naked) unsigned long __cdecl bcb6_strtoul(const char *nptr, char **endptr, int base)
{
	__asm
	{
		#define nptr   (esp + 4)
		#define endptr (esp + 8)
		#define base   (esp + 12)

		call    dword ptr [_bcb6_errno]
		mov     ecx, dword ptr [base]
		mov     edx, dword ptr [endptr]
		push    eax
		push    ecx
		mov     ecx, dword ptr [nptr + 8]
		push    edx
		push    ecx
		push    eax
		push    FALSE
		push    TRUE
		call    internal_strtoi64
		pop     ecx
#if EINVAL != BCB6_EINVAL
		cmp     dword ptr [ecx], EINVAL
		jne     L1
		mov     dword ptr [ecx], BCB6_EINVAL
	L1:
#endif
#if ERANGE != BCB6_ERANGE
		cmp     dword ptr [ecx], ERANGE
		jne     L2
		mov     dword ptr [ecx], BCB6_ERANGE
	L2 :
#endif
		ret

		#undef nptr
		#undef endptr
		#undef base
	}
}
