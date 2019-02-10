#include <windows.h>
#ifdef _MBCS
#define _tcslwr _mbslwr
#elif defined(_UNICODE)
#define _tcslwr _wcslwr
#else
#define _tcslwr _strlwr
#endif

#ifndef _M_IX86
TCHAR * __cdecl _tcslwr(TCHAR *string)
{
	TCHAR *p, c;

	p = string;
	while (c = *(p++))
#ifdef _MBCS
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c)) {
#endif
#ifdef _UNICODE
			if ((short)c >= L'A' && c <= L'Z')
#else
			if ((char)c >= 'A' && (unsigned char)c <= 'Z')
#endif
				p[-1] = c + ('a' - 'A');
#ifdef _MBCS
		} else if (!*(p++))
			break;
#endif
	return string;
}
#else
__declspec(naked) TCHAR * __cdecl _tcslwr(TCHAR *string)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define inc_tchar(r) add r, 2
	#define t(r)         r##x
	#define sizeof_tchar 2
#else
	#define tchar_ptr    byte ptr
	#define inc_tchar(r) inc r
	#define t(r)         r##l
	#define sizeof_tchar 1
#endif

	__asm
	{
		#define string (esp + 4)
#ifdef _MBCS
		#define p      esi
		#define c      t(b)
#else
		#define p      ecx
		#define c      t(a)
#endif

#ifdef _MBCS
		push    ebx
		push    esi
		xor     ebx, ebx
		mov     p, dword ptr [string + 8]
#else
		mov     p, dword ptr [string]
#endif
		jmp     L3

		align   16
	L1:
#ifdef _MBCS
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jnz     L2
#endif
		cmp     c, 'A'
		jl      L3
		cmp     c, 'Z'
		ja      L3
		add     c, 'a' - 'A'
		mov     tchar_ptr [p - sizeof_tchar], c
#ifdef _MBCS
		jmp     L3

		align   16
	L2:
		mov     c, tchar_ptr [p]
		inc_tchar(p)
		test    c, c
		jz      L4
#endif
	L3:
		mov     c, tchar_ptr [p]
		inc_tchar(p)
		test    c, c
		jnz     L1
#ifdef _MBCS
	L4:
		mov     eax, dword ptr [string + 8]
		pop     esi
		pop     ebx
#else
		mov     eax, dword ptr [string]
#endif
		ret

		#undef string
		#undef p
		#undef c
	}

	#undef tchar_ptr
	#undef inc_tchar
	#undef t
	#undef sizeof_tchar
}
#endif
