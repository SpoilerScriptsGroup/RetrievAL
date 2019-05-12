#include <windows.h>

#ifdef _MBCS
#define _tcsicmp _mbsicmp
#elif defined(_UNICODE)
#define _tcsicmp _wcsicmp
#else
#define _tcsicmp _stricmp
#endif

#pragma warning(disable:4028)

#ifndef _M_IX86
int __cdecl _tcsicmp(const TCHAR *string1, const TCHAR *string2)
{
	for (; ; )
	{
		TCHAR c1, c2;

		c1 = *(string1++);
		c2 = *(string2++);
		if (!(c1 -= c2))
		{
#ifdef _MBCS
			if (!c2)
				break;
			if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
				continue;
			c1 = *(string1++);
			c2 = *(string2++);
			if (c1 == c2)
#endif
				if (c2)
					continue;
				else
					break;
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
			c1 += c2;
		}
#ifdef _UNICODE
		return (int)c1 - (int)c2;
#else
		return (int)(unsigned char)c1 - (int)(unsigned char)c2;
#endif
	}
	return 0;
}
#else
__declspec(naked) int __cdecl _tcsicmp(const TCHAR *string1, const TCHAR *string2)
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
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		sub     esi, ebx

		align   16
	L1:
		mov     t(c), tchar_ptr [ebx]
		xor     eax, eax
		mov     t(a), tchar_ptr [ebx + esi]
		inc_tchar(ebx)
		sub     t(c), t(a)
		jnz     L3
#ifdef _MBCS
		test    t(a), t(a)
		jz      L2
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L1
		mov     t(c), tchar_ptr [ebx]
		xor     eax, eax
		mov     t(a), tchar_ptr [ebx + esi]
		inc_tchar(ebx)
		cmp     t(c), t(a)
		jne     L6
#endif
		test    t(a), t(a)
		jnz     L1
#ifdef _MBCS
	L2:
#endif
		pop     esi
		pop     ebx
		ret

		align   16
	L3:
		cmp     t(c), 'A' - 'a'
		je      L4
		cmp     t(c), 'a' - 'A'
		jne     L5
		sub     t(a), 'A'
		cmp     t(a), 'Z' - 'A'
		jbe     L1
		add     t(a), 'A'
		jmp     L5

		align   16
	L4:
		sub     t(a), 'a'
		cmp     t(a), 'z' - 'a'
		jbe     L1
		add     t(a), 'a'
	L5:
		add     t(c), t(a)
#ifdef _MBCS
	L6:
#endif
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
