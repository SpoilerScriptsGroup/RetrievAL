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
#ifndef _UNICODE
	const char    *p;
	unsigned char c1, c2, c3;
#else
	const wchar_t *p;
	wchar_t       c1, c2, c3;
#endif

	c1 = (TCHAR)c;
	if ((c1 -= 'a') > 'z' - 'a' && (c1 += 'a' - 'A') > 'Z' - 'A')
		return _tcschr(string, c);
	c2 = c1 + 'a';
	c3 = c1 + 'A';
	p = string - 1;
	do
		if ((c1 = *(++p)) == c2 || c1 == c3)
			goto DONE;
	while (c1);
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
		unsigned char c1, c2, c3;

		c1 = (unsigned char)c;
		if ((c1 -= 'a') > 'z' - 'a' && (c1 += 'a' - 'A') > 'Z' - 'A')
			goto MBSCHR;
		c2 = c1 + 'a';
		c3 = c1 + 'A';
		p = string - 1;
		do
			if ((c1 = *(++p)) == c2 || c1 == c3)
				goto DONE;
		while (c1 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c1) || *(++p)));
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
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     eax, dword ptr [string]
		mov     cl, byte ptr [c]
		sub     cl, 'a'
		jae     L1
		add     cl, 'a' - 'A'
	L1:
		cmp     cl, 'Z' - 'A'
		ja      strchr
		mov     dl, cl
		dec     eax
		add     dl, 'a'
		add     cl, 'A'
		mov     dh, cl

		align   16
	L2:
		mov     cl, byte ptr [eax + 1]
		inc     eax
		cmp     cl, dl
		je      L3
		cmp     cl, dh
		je      L3
		test    cl, cl
		jnz     L2
		xor     eax, eax
	L3:
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
		push    ebx
		mov     bx, dx
		sub     eax, 2
		add     dx, 'a'
		add     bx, 'A'

		align   16
	L2:
		mov     cx, word ptr [eax + 2]
		add     eax, 2
		cmp     cx, dx
		je      L3
		cmp     cx, bx
		je      L3
		test    cx, cx
		jnz     L2
		xor     eax, eax
	L3:
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
		jnz     L5
		sub     eax, 'a'
		jae     L1
		add     eax, 'a' - 'A'
	L1:
		cmp     eax, 'Z' - 'A'
		ja      _mbschr
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
	L2:
		mov     al, byte ptr [esi + 1]
		inc     esi
		cmp     al, bl
		je      L3
		cmp     al, bh
		je      L3
		test    al, al
		jz      L4
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		inc     esi
		xor     eax, eax
		mov     al, byte ptr [esi]
		test    al, al
		jnz     L2
		jmp     L4

		align   16
	L3:
		mov     eax, esi
	L4:
		pop     esi
		pop     ebx
		ret

		align   16
	L5:
		test    eax, not 0FFFFH
		jnz     L10
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
		jmp     L7

		align   16
	L6:
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L7
		mov     cl, byte ptr [esi + 1]
		xor     eax, eax
		test    cl, cl
		jz      L9
		push    2
		push    lpSrcStr
		push    2
		push    esi
		push    NORM_IGNORECASE
		push    Locale
		call    CompareStringA
		cmp     eax, CSTR_EQUAL
		je      L8
		inc     esi
		xor     eax, eax
	L7:
		mov     al, byte ptr [esi + 1]
		inc     esi
		test    al, al
		jnz     L6
		jmp     L9

		align   16
	L8:
		mov     eax, esi
	L9:
		pop     edi
		pop     esi
		pop     ebx
		pop     ecx
		ret

		align   16
	L10:
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
