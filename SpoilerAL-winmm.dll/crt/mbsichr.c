#include <windows.h>
#include <mbstring.h>

#pragma warning(disable:4414)

#ifndef _M_IX86
unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c)
{
	const unsigned char *p;

	if (c <= 0xFF)
	{
		unsigned char c1, c2;

		c1 = (unsigned char)c | ('a' - 'A');
		if ((unsigned)(c1 - 'a') >= 'z' - 'a' + 1)
			goto MBSCHR;
		p = string - 1;
		do
			if (((c2 = *(++p)) | ('a' - 'A')) == c1)
				goto DONE;
		while (c2 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2) || *(++p)));
	}
	else if (c <= 0xFFFF)
	{
		char lpSrcStr[2];

		lpSrcStr[0] = (char)(c >> 8);
		lpSrcStr[1] = (char)c;
		if (IsDBCSLeadByteEx(CP_THREAD_ACP, c >> 8))
		{
			LCID          Locale;
			WORD          wCharType;
			unsigned char c2;

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
	}
	p = NULL;
DONE:
	return (unsigned char *)p;

MBSCHR:
	return _mbschr(string, c);
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
		cmp     eax, 0FFH
		ja      L4
		or      eax, 'a' - 'A'
		dec     ecx
		mov     edx, eax
		sub     eax, 'a'
		cmp     eax, 'z' - 'a' + 1
		jae     _mbschr
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
		cmp     eax, 0FFFFH
		ja      L7
		xchg    al, ah
		push    eax
		and     eax, 0FFH
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L11
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
		jz      L8
		test    ecx, C1_UPPER or C1_LOWER
		jz      L8
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
	L6:
		mov     al, byte ptr [esi + 1]
		inc     esi
		test    al, al
		jnz     L5
		jmp     L10

		align   16
	L7:
		xor     eax, eax
		ret

		align   16
	L8:
		pop     edi
		pop     esi
		pop     ebx
		pop     ecx
		jmp     _mbschr

		align   16
	L9:
		mov     eax, esi
	L10:
		pop     edi
		pop     esi
		pop     ebx
	L11:
		pop     ecx
		ret

		#undef string
		#undef c
		#undef Locale
		#undef lpSrcStr
		#undef lpCharType
	}
}
#endif
