#include <windows.h>

#ifndef _tcsrev
#if !defined(_UNICODE) && !defined(_MBCS)
#define _tcsrev _strrev
#elif defined(_MBCS)
#define _tcsrev _mbsrev
#else
#define _tcsrev _wcsrev
#endif
#endif

#ifdef _MBCS
extern HANDLE hHeap;
#endif

#ifndef _M_IX86
#ifndef _MBCS
TCHAR * __cdecl _tcsrev(TCHAR *string)
{
	size_t length;

	if ((length = _tcslen(string)) > 1)
	{
		TCHAR *p1, *p2, c1, c2;

		p2 = (p1 = string) + length - 1;
		do
		{
			c1 = *p1;
			c2 = *p2;
			*p1 = c2;
			*p2 = c1;
			p1++;
			p2--;
		} while (p1 < p2);
	}
	return string;
}
#else	// _MBCS
const char * __cdecl _mbsrev(const char *string)
{
	size_t         length;
	unsigned short *buffer;

	if ((length = strlen((const char *)string)) &&
		length <= (~(size_t)0 >> 1) &&
		(buffer = (unsigned short *)HeapAlloc(hHeap, 0, length * 2)))
	{
		unsigned char  *mbs;
		unsigned short *wcs, w;

		mbs = string;
		wcs = buffer;
		do
			*(wcs++) =
				!IsDBCSLeadByteEx(CP_THREAD_ACP, *mbs) || !mbs[1] ?
					*(mbs++) :
					*(((unsigned short *)mbs)++);
		while (*mbs);
		mbs = string;
		do
			if ((w = *(--wcs)) < 0x100)
				*(mbs++) = (unsigned char)w;
			else
				*(((unsigned short *)mbs)++) = w;
		while (wcs != buffer);
		HeapFree(hHeap, 0, buffer);
	}
	return string;
}
#endif	// _MBCS
#else	// _M_IX86
#ifndef _MBCS
__declspec(naked) TCHAR * __cdecl _tcsrev(TCHAR *string)
{
#ifdef _UNICODE
	#define scast        scasw
	#define tchar_ptr    word ptr
	#define t(r)         r##x
	#define inc_tchar(r) add r, 2
	#define dec_tchar(r) sub r, 2
#else
	#define scast        scasb
	#define tchar_ptr    byte ptr
	#define t(r)         r##l
	#define inc_tchar(r) inc r
	#define dec_tchar(r) dec r
#endif

	__asm
	{
		#define string (esp + 4)

		push    esi
		push    edi
		mov     edi, dword ptr [string + 8]                 // edi = string; save return value
		xor     eax, eax                                    // search value (null)
		mov     ecx, -1                                     // ecx = -1
		mov     esi, edi                                    // esi = pointer to string
		repne   scast                                       // find null
		sub     edi, size TCHAR * 2                       // string is not empty, move di pointer back
		                                                    // edi points to last non-null char
		mov     eax, esi                                    // return value: string addr
		cmp     ecx, -2                                     // is string empty? (if offset value is 0, the
		je      done                                        // cmp below will not catch it and we'll hang).

		align   16
	lupe:
		mov     t(c), tchar_ptr [esi]                       // get front char...
		mov     t(d), tchar_ptr [edi]                       //   and end char
		mov     tchar_ptr [esi], t(d)                       // put end char in front...
		mov     tchar_ptr [edi], t(c)                       //   and front char at end
		inc_tchar(esi)                                      // front moves up...
		dec_tchar(edi)                                      //   and end moves down
		cmp     esi, edi                                    // see if pointers have crossed yet
		jb      lupe                                        // exit when pointers meet (or cross)

	done:
		pop     edi
		pop     esi
		ret                                                 // __cdecl return

		#undef string
	}

	#undef scast
	#undef tchar_ptr
	#undef t
	#undef inc_tchar
	#undef dec_tchar
}
#else	// _MBCS
__declspec(naked) const char * __cdecl _mbsrev(const char *string)
{
	__asm
	{
		#define string (esp + 4)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebx, dword ptr [string + 16]
		xor     eax, eax
		mov     edi, ebx
		mov     ecx, -1
		repne scasb
		dec     eax
		inc     ecx
		xor     eax, ecx
		mov     esi, ebx
		add     eax, eax
		jbe     L7                                          // CF=1 or ZF=1
		push    eax
		push    0
		push    dword ptr [hHeap]
		call    HeapAlloc
		test    eax, eax
		jz      L7
		mov     edi, eax
		xor     eax, eax
		mov     al, byte ptr [esi]
		inc     esi
		mov     ebp, edi

		align   16
	L1:
		mov     byte ptr [edi], al
		inc     edi
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		xor     eax, eax
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L3
	L2:
		mov     byte ptr [edi], al
		inc     edi
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jnz     L1
		jmp     L4

		align   16
	L3:
		mov     byte ptr [edi], al
		inc     edi
	L4:
		mov     ecx, ebx

		align   16
	L5:
		mov     ax, word ptr [edi - 2]
		sub     edi, 2
		mov     byte ptr [ecx], al
		inc     ecx
		test    ah, ah
		jz      L6
		mov     byte ptr [ecx], ah
		inc     ecx
	L6:
		cmp     edi, ebp
		jne     L5

		push    ebp
		push    0
		push    dword ptr [hHeap]
		call    HeapFree
	L7:
		mov     eax, ebx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef string
	}
}
#endif	// _MBCS
#endif	// _M_IX86
