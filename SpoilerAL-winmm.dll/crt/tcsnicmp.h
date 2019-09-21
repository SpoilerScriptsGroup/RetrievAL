#include <windows.h>

#ifdef _MBCS
#ifdef _MBSNBICMP
#define _tcsnicmp _mbsnbicmp
#else
#define _tcsnicmp _mbsnicmp
#endif
#elif defined(_UNICODE)
#define _tcsnicmp _wcsnicmp
#else
#define _tcsnicmp _strnicmp
#endif

#pragma warning(disable:4028)

#ifndef _M_IX86
#ifndef _MBCS
int __cdecl _tcsnicmp(const TCHAR *string1, const TCHAR *string2, size_t count)
{
	string1 += count;
	string2 += count;
	count = ~count;
	while (++count)
	{
#ifdef _UNICODE
		wchar_t c1, c2;
#else
		unsigned char c1, c2;
#endif

		c1 = string1[count];
		c2 = string2[count];
		if (!(c1 -= c2))
			if (c2)
				continue;
			else
				break;
		if (c1 == 'A' - 'a')
		{
			if ((c2 - 'a') <= 'z' - 'a')
				continue;
		}
		else if (c1 == 'a' - 'A')
		{
			if ((c2 - 'a') <= 'Z' - 'A')
				continue;
		}
		c1 += c2;
		return (int)c1 - (int)c2;
	}
	return 0;
}
#elif defined(_MBSNBICMP)
int __cdecl _mbsnbicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	int ret;

	ret = CompareStringA(GetThreadLocale(), NORM_IGNORECASE, string1, count, string2, count);
	return ret ? ret - CSTR_EQUAL : _NLSCMPERROR;
}
#else
int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	const unsigned char *p, c;
	int                 ret;

	if (!count)
		return 0;
	p = string1;
	do
		if (!(c = *(p++)))
			break;
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, c) && !*(p++))
			break;
	while (--count);
	count = p - string1;
	ret = CompareStringA(GetThreadLocale(), NORM_IGNORECASE, string1, (int)count, string2, (int)count);
	return ret ? ret - CSTR_EQUAL : _NLSCMPERROR;
}
#endif
#else
#ifndef _MBCS
__declspec(naked) int __cdecl _tcsnicmp(const TCHAR *string1, const TCHAR *string2, size_t count)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define sizeof_tchar 2
	#define t(r)         r##x
#else
	#define tchar_ptr    byte ptr
	#define sizeof_tchar 1
	#define t(r)         r##l
#endif

	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]
		mov     edi, dword ptr [string2 + 8]
		mov     ecx, dword ptr [count + 8]
		xor     eax, eax
		xor     edx, edx
#ifdef _UNICODE
		lea     esi, [esi + ecx * sizeof_tchar]
		lea     edi, [edi + ecx * sizeof_tchar]
#else
		add     esi, ecx
		add     edi, ecx
#endif
		xor     ecx, -1

		align   16
	L1:
		inc     ecx
		jz      L2
		mov     t(a), tchar_ptr [esi + ecx * sizeof_tchar]
		mov     t(d), tchar_ptr [edi + ecx * sizeof_tchar]
		sub     eax, edx
		jnz     L3
		test    edx, edx
		jnz     L1
	L2:
		pop     edi
		pop     esi
		ret

		align   16
	L3:
		cmp     eax, 'a' - 'A'
		jne     L5
		sub     edx, 'A'
		xor     eax, eax
		cmp     edx, 'Z' - 'A'
		ja      L4
		xor     edx, edx
		jmp     L1

	L4:
		add     eax, 'a' - 'A'
		jmp     L7

		align   16
	L5:
		cmp     eax, 'A' - 'a'
		jne     L7
		sub     edx, 'a'
		xor     eax, eax
		cmp     edx, 'z' - 'a'
		ja      L6
		xor     edx, edx
		jmp     L1

	L6:
		sub     eax, 'a' - 'A'
		jmp     L7

		align   16
	L7:
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
	}

	#undef tchar_ptr
	#undef sizeof_tchar
	#undef t
}
#elif defined(_MBSNBICMP)
__declspec(naked) int __cdecl _mbsnbicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		mov     ecx, dword ptr [count]
		mov     edx, dword ptr [string2]
		mov     eax, dword ptr [string1]
		push    ecx
		push    edx
		push    ecx
		push    eax
		call    GetThreadLocale
		push    NORM_IGNORECASE
		push    eax
		call    CompareStringA
		xor     ecx, ecx
		sub     eax, 1
		adc     ecx, -1
		and     eax, 7FFFFFFFH
		add     eax, ecx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#else
__declspec(naked) int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		mov     esi, dword ptr [count + 8]
		xor     eax, eax
		test    esi, esi
		jz      L4
		mov     ebx, dword ptr [string1 + 8]

		align   16
	L1:
		mov     al, byte ptr [ebx]
		inc     ebx
		test    al, al
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     cl, byte ptr [ebx]
		inc     ebx
		test    cl, cl
		jz      L3
		xor     eax, eax
	L2:
		dec     esi
		jnz     L1
	L3:
		mov     eax, dword ptr [string1 + 8]
		mov     ecx, dword ptr [string2 + 8]
		pop     esi
		sub     ebx, eax
		push    ebx
		push    ecx
		push    ebx
		push    eax
		call    GetThreadLocale
		push    NORM_IGNORECASE
		push    eax
		call    CompareStringA
		xor     ecx, ecx
		sub     eax, 1
		adc     ecx, -1
		and     eax, 7FFFFFFFH
		add     eax, ecx
		pop     ebx
		ret

		align   16
	L4:
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
#endif
