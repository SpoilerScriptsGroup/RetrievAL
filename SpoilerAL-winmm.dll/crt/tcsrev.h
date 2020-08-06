#include <windows.h>
#include <tchar.h>

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
unsigned char * __cdecl _mbsrev(unsigned char *string)
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
#pragma function(_tcslen)
__declspec(naked) TCHAR * __cdecl _tcsrev(TCHAR *string)
{
#ifdef _UNICODE
	#define scast        scasw
	#define tchar        word
	#define t(r)         r##x
	#define inc_tchar(r) add r, 2
	#define dec_tchar(r) sub r, 2
#else
	#define scast        scasb
	#define tchar        byte
	#define t(r)         r##l
	#define dec_tchar(r) dec r
#endif

	__asm
	{
		#define string (esp + 4)

		push    esi
		push    edi
		mov     edi, -4
		mov     esi, dword ptr [string + 8]                 // esi = string; save return value
		add     edi, esi
		push    esi
		call    _tcslen                                     // find null
		test    eax, eax                                    // is not string empty? (if offset value is 0, the
		lea     edi, [edi + eax * size TCHAR]               // edi points to last null char - 4
		mov     eax, esi                                    // return value: string addr
		jne     entry                                       // cmp below will not catch it and we'll hang).
		pop     ecx
		jmp     done

		align   16
#ifdef _UNICODE
	lupe:
		mov     ecx, dword ptr [esi]                        // get front chars...
		mov     edx, dword ptr [edi]                        //   and end chars
		rol     ecx, 16                                     // swap front chars...
		add     esi, 4                                      //   and moves down
		rol     edx, 16                                     // swap end chars...
		sub     edi, 4                                      //   and moves up
		mov     dword ptr [esi - 4], edx                    // put front chars in end...
		mov     dword ptr [edi + 4], ecx                    //   and end chars at front
	entry:
		cmp     esi, edi                                    // see if pointers have crossed yet
		jb      lupe                                        // exit when pointers meet (or cross)

		pop     ecx
		jne     done
		mov     t(c), tchar ptr [esi]                       // get front char...
		mov     t(d), tchar ptr [edi + 4 - size TCHAR]      //   and end char
		mov     tchar ptr [esi], t(d)                       // put front char in end...
		mov     tchar ptr [edi + 4 - size TCHAR], t(c)      //   and end char at front
#else
	lupe:
		mov     ecx, dword ptr [esi]                        // get front chars...
		mov     edx, dword ptr [edi]                        //   and end chars
		bswap   ecx                                         // swap front chars...
		bswap   edx                                         //   and end chars
		mov     dword ptr [edi], ecx                        // put end chars in front...
		sub     edi, 4                                      //   and moves down
		mov     dword ptr [esi], edx                        // put front chars in end...
		add     esi, 4                                      //   and moves up
	entry:
		cmp     esi, edi                                    // see if pointers have crossed yet
		jb      lupe                                        // exit when pointers meet (or cross)

		pop     ecx
		jne     less_than_dword
		mov     ecx, dword ptr [esi]                        // get chars
		bswap   ecx                                         // swap chars
		mov     dword ptr [esi], ecx                        // put chars
		jmp     done

		align   16
	less_than_dword:
		add     edi, 4 - size TCHAR
		sub     edi, esi
		jbe     done
		mov     t(c), tchar ptr [esi]                       // get front char...
		mov     t(d), tchar ptr [esi + edi]                 //   and end char
		mov     tchar ptr [esi], t(d)                       // put front char in end...
		mov     tchar ptr [esi + edi], t(c)                 //   and end char at front
		dec_tchar(edi)
		jz      done
		mov     t(c), tchar ptr [esi + size TCHAR]          // get front char...
		mov     t(d), tchar ptr [esi + edi]                 //   and end char
		mov     tchar ptr [esi + size TCHAR], t(d)          // put front char in end...
		mov     tchar ptr [esi + edi], t(c)                 //   and end char at front
		dec_tchar(edi)
		jz      done
		mov     t(c), tchar ptr [esi + (size TCHAR * 2)]    // get front char...
		mov     t(d), tchar ptr [esi + edi]                 //   and end char
		mov     tchar ptr [esi + (size TCHAR * 2)], t(d)    // put front char in end...
		mov     tchar ptr [esi + edi], t(c)                 //   and end char at front
#endif

	done:
		pop     edi
		pop     esi
		ret                                                 // __cdecl return

		#undef string
	}

	#undef scast
	#undef tchar
	#undef t
	#undef dec_tchar
}
#else	// _MBCS
__declspec(naked) unsigned char * __cdecl _mbsrev(unsigned char *string)
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
