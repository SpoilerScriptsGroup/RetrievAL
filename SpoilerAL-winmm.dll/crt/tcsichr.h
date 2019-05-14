#include <windows.h>

#pragma warning(disable:4414)

#ifdef _MBCS
#define _tcsichr _mbsichr
#define _tcschr _mbschr
extern unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c);
#elif defined(_UNICODE)
#define _tcsichr _wcsichr
#define _tcschr wcschr
extern wchar_t * __cdecl wcschr(const wchar_t *string, wint_t c);
#else
#define _tcsichr _strichr
#define _tcschr strchr
extern char * __cdecl strchr(const char *string, int c);
#endif

#pragma warning(disable:4102)

#ifndef _M_IX86
#ifndef _MBCS
#ifdef _UNICODE
wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
#else
char * __cdecl _strichr(const char *string, int c)
#endif
{
	TCHAR c2, c3, c4, *p;

#ifdef _UNICODE
	if ((c -= 'a') > 'z' - 'a' && (c += 'a' - 'A') > 'Z' - 'A')
#else
	if ((unsigned char)(c -= 'a') > 'z' - 'a' && (unsigned char)(c += 'a' - 'A') > 'Z' - 'A')
#endif
		goto TCSCHR;
	c2 = (TCHAR)c + 'a';
	c3 = (TCHAR)c + 'A';
	p = (TCHAR *)string;
	do
		if ((c4 = *(p++)) == c2 || c4 == c3)
			return p - 1;
	while (c4);
	return NULL;

TCSCHR:
	return _tcschr(string, c);
}
#else
unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c)
{
	if (!(c & ~0xFF))
	{
		unsigned char c2, c3, c4, *p;

		if ((c -= 'a') > 'z' - 'a' && (c += 'a' - 'A') > 'Z' - 'A')
			goto MBSCHR;
		c2 = (unsigned char)c + 'a';
		c3 = (unsigned char)c + 'A';
		p = (unsigned char *)string;
		do
			if ((c4 = *(p++)) == c2 || c4 == c3)
				return p - 1;
		while (c4 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c4) || *(p++)));
	}
	else if (!(c & ~0xFFFF))
	{
		LCID          Locale;
		char          lpSrcStr[2];
		WORD          CharType;
		unsigned char *p, c2;

		lpSrcStr[0] = (unsigned char)(c >> 8);
		lpSrcStr[1] = (unsigned char)c;
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c >> 8))
			goto MBSCHR;
		Locale = GetThreadLocale();
		if (!GetStringTypeA(Locale, CT_CTYPE3, lpSrcStr, 2, &CharType) || !(CharType & C3_ALPHA))
			goto MBSCHR;
		p = (unsigned char *)string;
		if (!(c2 = *p))
			goto RETURN_NULL;
		do
			if (IsDBCSLeadByteEx(CP_THREAD_ACP, c2)) {
				if (GetStringTypeA(Locale, CT_CTYPE3, p, 2, &CharType) && (CharType & C3_ALPHA))
					if (CompareStringA(Locale, NORM_IGNORECASE, p, 2, lpSrcStr, 2) == CSTR_EQUAL)
						return p;
				if (!*(++p))
					break;
			}
		while (c2 = *(++p));
	}
RETURN_NULL:
	return NULL;

MBSCHR:
	return _mbschr(string, c);
}
#endif
#else
#ifndef _MBCS
#ifdef _UNICODE
__declspec(naked) wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
#else
__declspec(naked) char * __cdecl _strichr(const char *string, int c)
#endif
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define inc_tchar(r) add r, 2
	#define t(r)         r##x
#else
	#define tchar_ptr    byte ptr
	#define inc_tchar(r) inc r
	#define t(r)         r##l
#endif

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)
#ifdef _UNICODE
		#define c2     ax
		#define c3     bx
		#define c4     dx
		#define p      ecx
#else
		#define c2     al
		#define c3     ah
		#define c4     dl
		#define p      ecx
#endif

		mov     t(a), tchar_ptr [c]
		sub     t(a), 'a'
		jb      L1
		cmp     t(a), 'z' - 'a'
		jbe     L2
		jmp     _tcschr

		align   16
	L1:
		add     t(a), 'a' - 'A'
		cmp     t(a), 'Z' - 'A'
		ja      _tcschr
	L2:
#ifdef _UNICODE
		push    ebx
		mov     bx, ax
		mov     ecx, dword ptr [string + 4]
		add     ax, 'a'
		add     bx, 'A'
#else
		mov     ah, al
		mov     ecx, dword ptr [string]
		add     ax, 'Aa'
#endif

		align   16
	L3:
		mov     c4, tchar_ptr [p]
		inc_tchar(p)
		cmp     c4, c2
		je      L4
		cmp     c4, c3
		je      L4
		test    c4, c4
		jnz     L3
		xor     eax, eax
#ifdef _UNICODE
		pop     ebx
#endif
		ret

		align   16
	L4:
#ifdef _UNICODE
		pop     ebx
		lea     eax, [p - 2]
#else
		lea     eax, [p - 1]
#endif
		ret

		#undef string
		#undef c
		#undef c2
		#undef c3
		#undef c4
		#undef p
	}

	#undef tchar_ptr
	#undef inc_tchar
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
		test    eax, not 0FFH
		jnz     L6
		sub     eax, 'a'
		jb      L1
		cmp     eax, 'z' - 'a'
		jbe     L2
		jmp     _mbschr

		align   16
	L1:
		add     eax, 'a' - 'A'
		cmp     eax, 'Z' - 'A'
		ja      _mbschr
	L2:
		push    ebx
		mov     ebx, eax
		push    esi
		mov     bh, al
		mov     esi, dword ptr [string + 8]
		add     bx, 'Aa'

		#define c2     bl
		#define c3     bh
		#define c4     al
		#define p      esi

		align   16
	L3:
		mov     c4, byte ptr [p]
		inc     p
		cmp     c4, c2
		je      L5
		cmp     c4, c3
		je      L5
		test    eax, eax
		jz      L4
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L3
		mov     c4, byte ptr [p]
		inc     p
		and     eax, 0FFH
		jnz     L3
	L4:
		pop     esi
		pop     ebx
		ret

		align   16
	L5:
		lea     eax, [p - 1]
		pop     esi
		pop     ebx
		ret

		align   16
	L6:
		test    eax, not 0FFFFH
		jnz     RETURN_NULL
		mov     byte ptr [esp + 8], ah
		mov     byte ptr [esp + 9], al
		shr     eax, 8
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      _mbschr
		call    GetThreadLocale
		push    ebx
		push    esi
		push    edi
		push    0

		#undef  string
		#undef  c2
		#undef  p

		#define Locale      ebx
		#define lpSrcStr    edi
		#define lpCharType  esp
		#define p           esi
		#define string      (esp + 16 + 4)
		#define c2          al
		#define c2d         eax

		mov     Locale, eax
		lea     lpSrcStr, [esp + 16 + 8]
		mov     p, dword ptr [string]

		push    lpCharType
		push    2
		push    lpSrcStr
		push    CT_CTYPE3
		push    Locale
		call    GetStringTypeA
		test    eax, eax
		jz      MBSCHR
		mov     ecx, dword ptr [lpCharType]
		mov     c2, byte ptr [p]
		test    ecx, C3_ALPHA
		jz      MBSCHR
		and     c2d, 0FFH
		jz      RETURN_NULL

		align   16
	L7:
		push    c2d
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L9
		push    lpCharType
		push    2
		push    p
		push    CT_CTYPE3
		push    Locale
		call    GetStringTypeA
		test    eax, eax
		jz      L8
		test    dword ptr [lpCharType], C3_ALPHA
		jz      L8
		push    2
		push    lpSrcStr
		push    2
		push    p
		push    NORM_IGNORECASE
		push    Locale
		call    CompareStringA
		cmp     eax, CSTR_EQUAL
		jne     L9
		jmp     RETURN_POINTER

		align   16
	L8:
		mov     c2, byte ptr [p + 1]
		inc     p
		test    c2, c2
		jz      RETURN_NULL
	L9:
		mov     c2, byte ptr [p + 1]
		inc     p
		and     c2d, 0FFH
		jnz     L7

		align   16
	RETURN_NULL:
		xor     eax, eax
		pop     ecx
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	MBSCHR:
		pop     eax
		pop     edi
		pop     esi
		pop     ebx
		jmp     _mbschr

		align   16
	RETURN_POINTER:
		mov     eax, p
		pop     ecx
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
		#undef c2
		#undef c2d
		#undef c3
		#undef c4
		#undef p
		#undef Locale
		#undef lpSrcStr
		#undef lpCharType
	}
}
#endif
#endif
