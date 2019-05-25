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
		else
		{
			if (c1 == 'A' - 'a')
			{
				if ((c2 -= 'a') <= 'z' - 'a')
					continue;
				c2 += 'a';
			}
			else if (c1 == 'a' - 'A')
			{
				if ((c2 -= 'A') <= 'Z' - 'A')
					continue;
				c2 += 'A';
			}
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
int __cdecl __mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	size_t              n;
	const unsigned char *p, c;
	int                 ret;

	if (!(n = count))
		return 0;
	p = string2;
	do
		if (c = *(p++))
		{
			if (IsDBCSLeadByteEx(CP_THREAD_ACP, c) && !++count)
			{
				count--;
				break;
			}
		}
		else
		{
			count -= --n;
		}
	while (--n);
	ret = CompareStringA(GetThreadLocale(), NORM_IGNORECASE, string1, count, string2, count);
	return ret ? ret - CSTR_EQUAL : _NLSCMPERROR;
}
#endif
#else
#ifndef _MBCS
__declspec(naked) int __cdecl _tcsnicmp(const TCHAR *string1, const TCHAR *string2, size_t count)
{
#ifdef _UNICODE
	#define sizeof_tchar 2
	#define tchar_ptr    word ptr
	#define t(r)         r##x
#else
	#define sizeof_tchar 1
	#define tchar_ptr    byte ptr
	#define t(r)         r##l
#endif

	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		mov     ebx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		mov     ecx, dword ptr [count + 8]
#ifdef _UNICODE
		lea     ebx, [ebx + ecx * sizeof_tchar]
		lea     esi, [esi + ecx * sizeof_tchar]
#else
		add     ebx, ecx
		add     esi, ecx
#endif
		xor     ecx, -1

		align   16
	L1:
		inc     ecx
		jz      L2
		mov     t(a), tchar_ptr [ebx + ecx * sizeof_tchar]
		mov     t(d), tchar_ptr [esi + ecx * sizeof_tchar]
		sub     t(a), t(d)
		jnz     L3
		test    t(d), t(d)
		jnz     L1
	L2:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	L3:
		cmp     t(a), 'A' - 'a'
		je      L4
		cmp     t(a), 'a' - 'A'
		jne     L5
		sub     t(d), 'A'
		cmp     t(d), 'Z' - 'A'
		jbe     L1
		add     t(d), 'A'
		jmp     L5

		align   16
	L4:
		sub     t(d), 'a'
		cmp     t(d), 'z' - 'a'
		jbe     L1
		add     t(d), 'a'
	L5:
		add     t(a), t(d)
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
	#undef sizeof_tchar
	#undef tchar_ptr
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
		mov     ecx, eax
		sub     eax, CSTR_EQUAL
		test    ecx, ecx
		mov     ecx, _NLSCMPERROR
		cmovz   eax, ecx
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

		mov     eax, dword ptr [count]
		mov     ecx, dword ptr [string2]
		test    eax, eax
		jz      L5
		push    ebx
		push    esi
		push    edi
		mov     ebx, eax
		mov     esi, ecx
		mov     edi, eax
		xor     ecx, ecx

		align   16
	L1:
		mov     cl, byte ptr [esi]
		inc     esi
		test    cl, cl
		jz      L2
		push    ecx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		xor     ecx, ecx
		test    eax, eax
		setnz   cl
		add     ebx, ecx
		jz      L3
		dec     edi
		jnz     L1
		jmp     L4

		align   16
	L2:
		dec     edi
		sub     ebx, edi
		jmp     L4

		align   16
	L3:
		dec     ebx

		align   16
	L4:
		call    GetThreadLocale
		mov     ecx, dword ptr [string1]
		mov     edx, dword ptr [string2]
		push    ebx
		push    edx
		push    ebx
		push    ecx
		push    NORM_IGNORECASE
		push    eax
		call    CompareStringA
		test    eax, eax
		lea     eax, [eax - CSTR_EQUAL]
		mov     ecx, _NLSCMPERROR
		pop     edi
		cmovz   eax, ecx
		pop     esi
		pop     ebx
	L5:
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
#endif
