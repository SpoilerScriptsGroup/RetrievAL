#include <windows.h>
#include <tchar.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
#ifndef _MBCS
#ifndef _UNICODE
char * __cdecl _strichr(const char *string, int c)
#else
wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
#endif
{
	const TCHAR *p;
	TCHAR       c1, c2;

	c1 = (TCHAR)c | ('a' - 'A');
	if ((unsigned)(c1 - 'a') > 'z' - 'a')
		return _tcschr(string, c);
	p = string - 1;
	do
		if (((c2 = *(++p)) | ('a' - 'A')) == c1)
			goto DONE;
	while (c2);
	p = NULL;
DONE:
	return (TCHAR *)p;
}
#else
unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c)
{
	const unsigned char *p;

	if (!(c & ~0xFF))
	{
		unsigned char c1, c2;

		c1 = (unsigned char)c | ('a' - 'A');
		if ((unsigned)(c1 - 'a') > 'z' - 'a')
			goto MBSCHR;
		p = string - 1;
		do
			if (((c2 = *(++p)) | ('a' - 'A')) == c1)
				goto DONE;
		while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(++p)));
	}
	else if (!(c & ~0xFFFF))
	{
		LCID          Locale;
		char          lpSrcStr[2];
		WORD          wCharType;
		unsigned char c2;

		lpSrcStr[0] = (char)(c >> 8);
		lpSrcStr[1] = (char)c;
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c >> 8))
			goto MBSCHR;
		Locale = GetThreadLocale();
		if (!GetStringTypeA(Locale, CT_CTYPE1, lpSrcStr, 2, &wCharType) || !(wCharType & (C1_UPPER | C1_LOWER)))
			goto MBSCHR;
		p = string - 1;
		while (c2 = *(++p))
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
				continue;
			else if (!p[1])
				break;
			else if (CompareStringA(Locale, NORM_IGNORECASE, p, 2, lpSrcStr, 2) == CSTR_EQUAL)
				goto DONE;
			else
				p++;
	}
	p = NULL;
DONE:
	return (unsigned char *)p;

MBSCHR:
	return _mbschr(string, c);
}
#endif
#else
#ifndef _MBCS
#ifndef _UNICODE
__declspec(naked) char * __cdecl _strichr(const char *string, int c)
#else
__declspec(naked) wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
#endif
{
#ifdef _UNICODE
	#define sizeof_tchar 2
	#define tchar_ptr    word ptr
	#define inc_tchar(r) add r, 2
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define sizeof_tchar 1
	#define tchar_ptr    byte ptr
	#define inc_tchar(r) inc r
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     t(c), tchar_ptr [c]
		mov     eax, dword ptr [string]
		or      t(c), 'a' - 'A'
		dec_tchar(eax)
		mov     t(d), t(c)
		sub     t(c), 'a'
		cmp     t(c), 'z' - 'a'
		ja      _tcschr
		push    ebx

		align   16
	L1:
		mov     t(c), tchar_ptr [eax + sizeof_tchar]
		inc_tchar(eax)
		mov     t(b), t(c)
		or      t(c), 'a' - 'A'
		cmp     t(c), t(d)
		je      L2
		test    t(b), t(b)
		jnz     L1
		xor     eax, eax
	L2:
		pop     ebx
		ret

		#undef string
		#undef c
	}

	#undef sizeof_tchar
	#undef tchar_ptr
	#undef inc_tchar
	#undef dec_tchar
	#undef t
}
#else
__declspec(naked) unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [c]
		mov     ecx, dword ptr [string]
		test    eax, not 0FFH
		jnz     L4
		or      eax, 'a' - 'A'
		dec     ecx
		mov     edx, eax
		sub     eax, 'a'
		cmp     eax, 'z' - 'a'
		ja      _mbschr
		push    ebx
		push    esi
		xor     eax, eax
		mov     ebx, edx
		mov     esi, ecx

		align   16
	L1:
		mov     cl, byte ptr [esi + 1]
		inc     esi
		mov     al, cl
		or      cl, 'a' - 'A'
		cmp     cl, bl
		je      L2
		test    al, al
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		inc     esi
		xor     eax, eax
		cmp     byte ptr [esi], 0
		jne     L1
		jmp     L3

		align   16
	L2:
		mov     eax, esi
	L3:
		pop     esi
		pop     ebx
		ret

		align   16
	L4:
		test    eax, not 0FFFFH
		jnz     L9
		xchg    al, ah
		push    eax
		and     eax, 0FFH
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      MBSCHR1
		call    GetThreadLocale
		push    ebx
		push    esi
		push    edi
		push    0

		#define Locale      ebx
		#define lpSrcStr    edi
		#define lpCharType  esp

		mov     esi, dword ptr [string + 20]
		lea     lpSrcStr, [esp + 16]
		mov     Locale, eax
		push    lpCharType
		push    2
		push    lpSrcStr
		push    CT_CTYPE1
		push    Locale
		call    GetStringTypeA
		dec     esi
		test    eax, eax
		pop     ecx
		jz      MBSCHR2
		test    ecx, C1_UPPER or C1_LOWER
		jz      MBSCHR2
		xor     eax, eax
		jmp     L6

		align   16
	L5:
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L6
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L8
		push    2
		push    lpSrcStr
		push    2
		push    esi
		push    NORM_IGNORECASE
		push    Locale
		call    CompareStringA
		cmp     eax, CSTR_EQUAL
		je      L7
		inc     esi
		xor     eax, eax
	L6:
		mov     al, byte ptr [esi + 1]
		inc     esi
		test    al, al
		jnz     L5
		jmp     L8

		align   16
	L7:
		mov     eax, esi
	L8:
		pop     edi
		pop     esi
		pop     ebx
		pop     ecx
		ret

		align   16
	L9:
		xor     eax, eax
		ret

		align   16
	MBSCHR2:
		pop     edi
		pop     esi
		pop     ebx
	MBSCHR1:
		pop     ecx
		jmp     _mbschr

		#undef string
		#undef c
		#undef Locale
		#undef lpSrcStr
		#undef lpCharType
	}
}
#endif
#endif
