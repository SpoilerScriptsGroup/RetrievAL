#include <windows.h>
#include <tchar.h>
#ifndef _tcsnicmp
#ifdef _MBCS
#ifdef _MBSNBICMP
#define _tcsnicmp _mbsnbicmp
#else
#define _tcsnicmp _mbsnicmp
#endif
#elif defined(_UNICODE)
#define _tcsnicmp wcsnicmp
#else
#define _tcsnicmp strnicmp
#endif
#endif

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _tcsnicmp(const TCHAR *string1, const TCHAR *string2, size_t count)
{
	string1 += count;
	string2 += count;
	count = ~count;
	while (++count)
	{
		TCHAR c1, c2;

		c1 = string1[count];
		c2 = string2[count];
		if (!(c1 -= c2))
		{
#ifdef _MBCS
			if (!c2)
				break;
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
				continue;
#ifdef _MBSNBICMP
			if (++count)
#endif
			{
				c1 = string1[count];
				c2 = string2[count];
				if (!(c1 -= c2))
#endif
					if (c2)
						continue;
					else
						break;
#ifdef _MBCS
			}
#endif
		}
		else
		{
			if (c1 == 'A' - 'a')
			{
#ifdef _UNICODE
				if ((short)c2 >= 'a' && c2 <= 'z')
#else
				if ((char)c2 >= 'a' && c2 <= 'z')
#endif
					continue;
			}
			else if (c1 == 'a' - 'A')
			{
#ifdef _UNICODE
				if ((short)c2 >= 'A' && c2 <= 'Z')
#else
				if ((char)c2 >= 'A' && c2 <= 'Z')
#endif
					continue;
			}
		}
		c1 += c2;
#ifdef _UNICODE
		return (int)c1 - (int)c2;
#else
		return (int)(unsigned char)c1 - (int)(unsigned char)c2;
#endif
	}
	return 0;
}
#else
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
#ifndef _MBCS
		#define cnt ecx
#else
		#define cnt edi
#endif

		push    ebx
		push    esi
#ifndef _MBCS
		push    cnt
#endif
		mov     ebx, dword ptr [string1 + 12]
		mov     esi, dword ptr [string2 + 12]
		mov     cnt, dword ptr [count + 12]
#ifdef _UNICODE
		lea     ebx, [ebx + cnt * sizeof_tchar]
		lea     esi, [esi + cnt * sizeof_tchar]
#else
		add     ebx, cnt
		add     esi, cnt
#endif
		xor     cnt, -1

		align   16
	L1:
		inc     cnt
		jz      L2
		mov     t(a), tchar_ptr [ebx + cnt * sizeof_tchar]
		mov     t(d), tchar_ptr [esi + cnt * sizeof_tchar]
		sub     t(a), t(d)
		jnz     L3
#ifdef _MBCS
		test    t(d), t(d)
		jz      L2
		push    ecx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
#ifdef _MBSNBICMP
		inc     cnt
		jz      L2
#endif
		mov     t(a), tchar_ptr [ebx + cnt * sizeof_tchar]
		mov     t(d), tchar_ptr [esi + cnt * sizeof_tchar]
		cmp     t(a), t(d)
		jne     L6
#endif
		test    t(d), t(d)
		jnz     L1
	L2:
		xor     eax, eax
#ifdef _MBCS
		pop     cnt
#endif
		pop     esi
		pop     ebx
		ret

		align   16
	L3:
		cmp     t(a), 'A' - 'a'
		je      L4
		cmp     t(a), 'a' - 'A'
		jne     L5
		cmp     t(d), 'A'
		jl      L5
		cmp     t(d), 'Z'
		jbe     L1
		jmp     L5

		align   16
	L4:
		cmp     t(d), 'a'
		jl      L5
		cmp     t(d), 'z'
		jbe     L1
	L5:
		add     t(a), t(d)
#ifdef _MBCS
	L6:
		pop     cnt
#endif
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
		#undef cnt
	}
	#undef sizeof_tchar
	#undef tchar_ptr
	#undef t
}
#endif
