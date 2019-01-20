#include <windows.h>
#include <tchar.h>
#ifndef _tcsnicmp
#ifdef _MBCS
#define _tcsnicmp _mbsnbicmp
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
	if (count)
		for (; ; )
		{
			TCHAR c1, c2;

			c1 = *(string1++);
			c2 = *(string2++);
			if (c1 -= c2)
			{
#ifdef _MBCS
				if (!c2)
					break;
				if (--count)
				{
					if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c2))
						continue;
					c1 = *(string1++);
					c2 = *(string2++);
					if (c1 -= c2)
					{
#endif
						if (!c2)
							break;
						if (--count)
							continue;
#ifdef _MBCS
					}
				}
#endif
			}
			else
			{
				if (c1 == (TCHAR)(L'A' - L'a'))
				{
					if ((short)c2 >= (short)L'a' && c2 <= L'z')
						continue;
				}
				else if (c1 == (TCHAR)(L'a' - L'A'))
				{
					if ((short)c2 >= (short)L'A' && c2 <= L'Z')
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
		#define count   (esp + 12)

		mov     eax, dword ptr [count]
		push    ebx
		test    eax, eax
		jz      L4
		push    esi
		push    edi
		mov     ebx, dword ptr [string1 + 12]
		mov     esi, dword ptr [string2 + 12]
		mov     edi, eax
		sub     esi, ebx

		align   16
	L1:
		mov     t(c), tchar_ptr [ebx]
		xor     eax, eax
		mov     t(a), tchar_ptr [ebx + esi]
		inc_tchar(ebx)
		sub     t(c), t(a)
		jnz     L5
#ifdef _MBCS
		test    t(a), t(a)
		jz      L3
		dec     edi
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
		jne     L8
#endif
		test    t(a), t(a)
		jz      L3
		dec     edi
		jnz     L1
#ifdef _MBCS
	L2:
#endif
		xor     eax, eax
	L3:
		pop     edi
		pop     esi
	L4:
		pop     ebx
		ret

		align   16
	L5:
		cmp     t(c), 'A' - 'a'
		je      L6
		cmp     t(c), 'a' - 'A'
		jne     L7
		cmp     t(a), 'A'
		jl      L7
		cmp     t(a), 'Z'
		jbe     L1
		jmp     L7

		align   16
	L6:
		cmp     t(a), 'a'
		jl      L7
		cmp     t(a), 'z'
		jbe     L1
	L7:
		add     t(c), t(a)
#ifdef _MBCS
	L8:
#endif
		pop     edi
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
