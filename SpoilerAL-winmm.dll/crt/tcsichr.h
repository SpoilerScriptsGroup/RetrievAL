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
#ifdef _UNICODE
	wchar_t c2, c3, c4, *p;
#else
	unsigned char c2, c3, c4, *p;
#endif

	if ((c -= 'a') > 'z' - 'a' && (c += 'a' - 'A') > 'Z' - 'A')
		goto TCSCHR;
	c2 = (TCHAR)c + 'a';
	c3 = (TCHAR)c + 'A';
	if ()
	p = string - 1;
	do
		if ((c4 = *(++p)) == c2 || c4 == c3)
			goto DONE;
	while (c4);
	p = NULL;
DONE:
	return p;

TCSCHR:
	return _tcschr(string, c);
}
#else
unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c)
{
	unsigned char *p;

	if (!(c & ~0xFF))
	{
		unsigned char c2, c3, c4;

		if ((c -= 'a') > 'z' - 'a' && (c += 'a' - 'A') > 'Z' - 'A')
			goto MBSCHR;
		c2 = (unsigned char)c + 'a';
		c3 = (unsigned char)c + 'A';
		p = (unsigned char *)string - 1;
		do
			if ((c4 = *(++p)) == c2 || c4 == c3)
				goto DONE;
		while (c4 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c4) || *(++p)));
	}
	else if (!(c & ~0xFFFF))
	{
		LCID          Locale;
		char          lpSrcStr[2];
		WORD          CharType;
		unsigned char c2;

		lpSrcStr[0] = (unsigned char)(c >> 8);
		lpSrcStr[1] = (unsigned char)c;
		if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c >> 8))
			goto MBSCHR;
		Locale = GetThreadLocale();
		if (!GetStringTypeA(Locale, CT_CTYPE3, lpSrcStr, 2, &CharType) || !(CharType & C3_ALPHA))
			goto MBSCHR;
		p = (unsigned char *)string - 1;
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
	return p;

MBSCHR:
	return _mbschr(string, c);
}
#endif
#else
#ifndef _MBCS
#ifndef _UNICODE
__declspec(naked) char * __cdecl _strichr(const char *string, int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [string]
		mov     dl, byte ptr [c]
		sub     dl, 'a'
		jae     L1
		add     dl, 'a' - 'A'
	L1:
		cmp     dl, 'Z' - 'A'
		ja      strchr
	L2:
		dec     eax
		mov     cl, dl
		add     dl, 'a'
		add     cl, 'A'
		mov     dh, cl

		align   16
	L3:
		mov     cl, byte ptr [eax + 1]
		inc     eax
		cmp     cl, dl
		je      L4
		cmp     cl, dh
		je      L4
		test    cl, cl
		jnz     L3
		xor     eax, eax
	L4:
		ret

		#undef string
		#undef c
	}
}
#else
__declspec(naked) wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [string]
		mov     dx, word ptr [c]
		sub     dx, 'a'
		jae     L1
		add     dx, 'a' - 'A'
	L1:
		cmp     dx, 'Z' - 'A'
		ja      wcschr
	L2:
		push    ebx
		mov     bx, dx
		sub     eax, 2
		add     dx, 'a'
		add     bx, 'A'

		align   16
	L3:
		mov     cx, word ptr [eax + 2]
		add     eax, 2
		cmp     cx, dx
		je      L4
		cmp     cx, bx
		je      L4
		test    cx, cx
		jnz     L3
		xor     eax, eax
	L4:
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
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
		jae     L1
		add     eax, 'a' - 'A'
	L1:
		cmp     eax, 'Z' - 'A'
		ja      _mbschr
	L2:
		push    ebx
		push    esi
		mov     esi, dword ptr [string + 8]
		add     al, 'a'
		mov     bl, al
		sub     al, 'a' - 'A'
		mov     bh, al
		xor     eax, eax
		dec     esi

		align   16
	L3:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bl
		je      L4
		cmp     al, bh
		je      L4
		test    al, al
		jz      L5
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L3
		inc     esi
		xor     eax, eax
		mov     al, byte ptr [esi]
		test    al, al
		jnz     L3
		jmp     L5

		align   16
	L4:
		mov     eax, esi
	L5:
		pop     esi
		pop     ebx
		ret

		align   16
	L6:
		test    eax, not 0FFFFH
		jnz     L11
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

		mov     Locale, eax
		lea     lpSrcStr, [esp + 16]
		mov     esi, dword ptr [string + 20]
		push    lpCharType
		push    2
		push    lpSrcStr
		push    CT_CTYPE3
		push    Locale
		call    GetStringTypeA
		dec     esi
		test    eax, eax
		pop     ecx
		jz      MBSCHR2
		test    ecx, C3_ALPHA
		jz      MBSCHR2
		xor     eax, eax
		jmp     L8

		align   16
	L7:
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L8
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L10
		push    2
		push    lpSrcStr
		push    2
		push    esi
		push    NORM_IGNORECASE
		push    Locale
		call    CompareStringA
		cmp     eax, CSTR_EQUAL
		je      L9
		inc     esi
		xor     eax, eax
	L8:
		mov     al, byte ptr [esi + 1]
		inc     esi
		test    al, al
		jnz     L7
		jmp     L10

		align   16
	L9:
		mov     eax, esi
	L10:
		pop     edi
		pop     esi
		pop     ebx
		pop     ecx
		ret

		align   16
	L11:
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
