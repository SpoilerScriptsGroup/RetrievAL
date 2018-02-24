#ifdef _tcstol
#include <stdlib.h>
#include <errno.h>

#ifndef _M_IX86
long __cdecl _tcstol(const TCHAR *nptr, TCHAR **endptr, int base)
{
#if defined(_UNICODE)
	typedef wchar_t       uchar_t;
	typedef short         schar_t;
#else
	typedef unsigned char uchar_t;
	typedef char          schar_t;
#endif

	long    number;
	errno_t previous, current;

	previous = errno;
	errno = 0;
	while (*nptr == ' ' || ((uchar_t)*nptr <= (uchar_t)'\r' && (uchar_t)*nptr >= (uchar_t)'\t'))
		nptr++;
	number = strtoul(nptr, endptr, base);
	if (!(current = errno))
	{
		if (*nptr != '-')
		{
			if (number < 0)
			{
				errno = ERANGE;
				return LONG_MAX;
			}
		}
		else
		{
			if (number > 0)
			{
				errno = ERANGE;
				return LONG_MIN;
			}
		}
		errno = previous;
		return number;
	}
	else
	{
		return current == ERANGE ?
			*nptr != '-' ?
				LONG_MAX :
				LONG_MIN :
			0;
	}
}
#else
__declspec(naked) long __cdecl _tcstol(const TCHAR *nptr, TCHAR **endptr, int base)
{
#if defined(_UNICODE)
	#define tchar_ptr    word ptr
	#define sizeof_tchar 2
#else
	#define tchar_ptr    byte ptr
	#define sizeof_tchar 1
#endif

	__asm
	{
#if defined(_UNICODE)
		#define inc_tchar_ptr add eax, 2
		#define dec_tchar_ptr sub eax, 2
		#define t             cx
#else
		#define inc_tchar_ptr inc eax
		#define dec_tchar_ptr dec eax
		#define t             cl
#endif

		#define nptr   (esp + 4)
		#define endptr (esp + 8)
		#define base   (esp + 12)

		call    _errno
		push    ebx
		mov     ebx, eax
		mov     edx, dword ptr [eax]
		mov     eax, dword ptr [nptr + 4]
		mov     dword ptr [ebx], 0
		dec_tchar_ptr
		align   16
	L1:
		mov     t, tchar_ptr [eax + sizeof_tchar]
		inc_tchar_ptr
		cmp     t, ' '
		je      L1
		cmp     t, 0DH
		ja      L2
		cmp     t, 09H
		jae     L1
	L2:
		push    edx
		push    ecx
		mov     edx, dword ptr [base + 12]
		mov     ecx, dword ptr [endptr + 12]
		push    edx
		push    ecx
		push    eax
		call    strtoul
		mov     ecx, dword ptr [ebx]
		add     esp, 12
		test    ecx, ecx
		jnz     L5
		pop     ecx
		pop     edx
		cmp     t, '-'
		je      L3
		test    eax, eax
		jns     L4
		jmp     L6
	L3:
		test    eax, eax
		jg      L7
	L4:
		mov     dword ptr [ebx], edx
		pop     ebx
		ret

		align   16
	L5:
		cmp     ecx, ERANGE
		pop     ecx
		pop     edx
		jne     L8
		cmp     t, '-'
		je      L7

		align   16
	L6:
		mov     dword ptr [ebx], ERANGE
		mov     eax, LONG_MAX
		pop     ebx
		ret

		align   16
	L7:
		mov     dword ptr [ebx], ERANGE
		mov     eax, LONG_MIN
		pop     ebx
		ret

		align   16
	L8:
		mov     dword ptr [ebx], EINVAL
		xor     eax, eax
		pop     ebx
		ret

		#undef inc_tchar_ptr
		#undef dec_tchar_ptr
		#undef t
		#undef nptr
		#undef endptr
		#undef base
	}
	#undef tchar_ptr
	#undef sizeof_tchar
}
#endif
#endif
