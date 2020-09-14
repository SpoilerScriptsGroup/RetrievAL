#include <windows.h>
#include <tchar.h>
#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotl16)
#endif

#ifdef _MBCS
extern HANDLE hHeap;
#endif

#ifndef _M_IX86
#ifndef _MBCS
TCHAR * __cdecl _tcsrev(TCHAR *string)
{
#if !defined(_MSC_VER) || _MSC_VER < 1310
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
#else
	size_t    size;
	BYTE      *first, *last;
	ptrdiff_t offset;

	last = (first = (BYTE *)string) + (size = _tcslen(string) * sizeof(TCHAR)) - sizeof(DWORD) * 2;
	if (size >= 8)
	{
		do
		{
			DWORD i, j;

			i = *(DWORD *)first;
			j = *(DWORD *)(last + 4);
#ifdef _UNICODE
			i = _rotl(i, 16);
			j = _rotl(j, 16);
#else
			i = _byteswap_ulong(i);
			j = _byteswap_ulong(j);
#endif
			*(DWORD *)(last + 4) = i;
			last -= 4;
			*(DWORD *)first = j;
			first += 4;
		} while (first <= last);
	}
	offset = last - first;
#ifdef _UNICODE
	if ((offset += 4) >= 0)
	{
		WORD i, j;

		i = *(WORD *)first;
		j = *(WORD *)(first + offset + 2);
		*(WORD *)(first + offset + 2) = i;
		*(WORD *)first = j;
	}
#else
	if ((offset += 4) >= 0)
	{
		WORD i, j;

		i = *(WORD *)first;
		j = *(WORD *)(first + offset + 2);
		i = _rotl16(i, 8);
		j = _rotl16(j, 8);
		*(WORD *)(first + offset + 2) = i;
		offset -= 4;
		*(WORD *)first = j;
		first += 2;
	}
	if ((offset += 2) >= 0)
	{
		BYTE i, j;

		i = *first;
		j = *(first + offset + 1);
		*(first + offset + 1) = i;
		*first = j;
	}
#endif
	return string;
#endif
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
	__asm
	{
		#define string (esp + 4)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     edi, -16
		mov     esi, dword ptr [string + 16]                // esi = string; save return value
		add     edi, esi
		push    esi
		call    _tcslen                                     // find null
#ifdef _UNICODE
		cmp     eax, 8
		lea     edi, [edi + eax * 2]                        // edi points to last null char - qword * 2
#else
		add     edi, eax                                    // edi points to last null char - qword * 2
		cmp     eax, 16
#endif
		mov     eax, esi
		jb      lt16

		align   16
	lupe:
		mov     ecx, dword ptr [esi]                        // get front chars...
		mov     edx, dword ptr [esi + 4]
		mov     ebx, dword ptr [edi + 8]                    //   and end chars
		mov     ebp, dword ptr [edi + 12]
#ifdef _UNICODE
		rol     ecx, 16                                     // swap front chars...
		rol     edx, 16
		rol     ebx, 16                                     //   and end chars
		rol     ebp, 16
#else
		bswap   ecx                                         // swap front chars...
		bswap   edx
		bswap   ebx                                         //   and end chars
		bswap   ebp
#endif
		mov     dword ptr [edi + 12], ecx                   // put front chars in end...
		mov     dword ptr [edi + 8], edx
		mov     dword ptr [esi + 4], ebx                    //   and end chars at front
		mov     dword ptr [esi], ebp
		sub     edi, 8                                      // end moves down...
		add     esi, 8                                      //   and front moves up
		cmp     edi, esi                                    // see if pointers have crossed yet
		jae     lupe                                        // exit when pointers meet (or cross)

	lt16:
		sub     edi, esi
		pop     ecx
		sub     edi, -8
		jb      lt8
		mov     ecx, dword ptr [esi]                        // get front chars...
		mov     edx, dword ptr [esi + edi + 4]              //   and end chars
#ifdef _UNICODE
		rol     ecx, 16                                     // swap front chars...
		rol     edx, 16                                     //   and end chars
#else
		bswap   ecx                                         // swap front chars...
		bswap   edx                                         //   and end chars
#endif
		mov     dword ptr [esi + edi + 4], ecx              // put front chars in end...
		sub     edi, 8                                      //   and end moves down
		mov     dword ptr [esi], edx                        // put end chars in front...
		add     esi, 4                                      //   and front moves up
	lt8:
#ifdef _UNICODE
		sub     edi, -4
		jb      done
		mov     cx, word ptr [esi]                          // get front char...
		mov     dx, word ptr [esi + edi + 2]                //   and end char
		mov     word ptr [esi + edi + 2], cx                // put front char in end...
		mov     word ptr [esi], dx                          //   and end char at front
#else
		sub     edi, -4
		jb      lt4
		mov     cx, word ptr [esi]                          // get front chars...
		mov     dx, word ptr [esi + edi + 2]                //   and end chars
		rol     cx, 8                                       // swap front chars...
		rol     dx, 8                                       //   and end chars
		mov     word ptr [esi + edi + 2], cx                // put front chars in end...
		sub     edi, 4                                      //   and end moves down
		mov     word ptr [esi], dx                          // put end chars in front...
		add     esi, 2                                      //   and front moves up
	lt4:
		sub     edi, -2
		jb      done
		mov     cl, byte ptr [esi]                          // get front char...
		mov     dl, byte ptr [esi + edi + 1]                //   and end char
		mov     byte ptr [esi + edi + 1], cl                // put front char in end...
		mov     byte ptr [esi], dl                          //   and end char at front
#endif
	done:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret                                                 // __cdecl return

		#undef string
	}
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
