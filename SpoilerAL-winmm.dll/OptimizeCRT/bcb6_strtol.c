#include <stdlib.h>
#include "bcb6_errno.h"

EXTERN_C unsigned __int64 __stdcall internal_strtoi64(BOOL is_unsigned, BOOL is_int64, errno_t *errnoptr, const char *nptr, char **endptr, int base);

__declspec(naked) long __cdecl bcb6_strtol(const char *nptr, char **endptr, int base)
{
	__asm
	{
		#define nptr   (esp + 4)
		#define endptr (esp + 8)
		#define base   (esp + 12)

		mov     eax, dword ptr [base]
		push    0
		mov     edx, esp
		push    eax
		mov     eax, dword ptr [endptr + 8]
		mov     ecx, dword ptr [nptr + 8]
		push    eax
		push    ecx
		push    edx
		push    FALSE
		push    FALSE
		call    internal_strtoi64
		pop     ecx
		test    ecx, ecx
		jz      L2
		cmp     ecx, EINVAL
		jne     L1
		mov     ecx, BCB6_EINVAL
	L1:
		push    eax
		push    ecx
		call    dword ptr [_bcb6_errno]
		pop     ecx
		mov     dword ptr [eax], ecx
		pop     eax
	L2:
		ret

		#undef nptr
		#undef endptr
		#undef base
	}
}
