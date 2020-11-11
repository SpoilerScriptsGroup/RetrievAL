#include <stdint.h>
#include <tchar.h>
#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotl16)
#ifndef _WIN64
#pragma intrinsic(_addcarry_u32)
#define _add_uintptr(a, b, out) _addcarry_u32(0, a, b, out)
#else
#pragma intrinsic(_addcarry_u64)
#define _add_uintptr(a, b, out) _addcarry_u64(0, a, b, out)
#endif
#endif

#ifndef _M_IX86
TCHAR * __cdecl _tcsrev(TCHAR *string)
{
#if !defined(_MSC_VER) || _MSC_VER < 1310
	size_t length;

	if ((length = _tcslen(string)) > 1)
	{
		TCHAR *p1, *p2, c;

		p2 = (p1 = string) + length - 1;
		do
		{
			c = *p1;
			*(p1++) = *p2;
			*(p2--) = c;
		} while (p1 < p2);
	}
	return string;
#else
	size_t    length;
	uint8_t   *first, *last;
	ptrdiff_t offset;

	last = (first = (uint8_t *)string) + (length = _tcslen(string)) * sizeof(TCHAR) - sizeof(uint32_t) * 4;
	if (length >= 16 / sizeof(TCHAR))
	{
		do
		{
			uint32_t a, b, c, d;

			a = *(uint32_t *)first;
			b = *(uint32_t *)(first + 4);
			c = *(uint32_t *)(last + 8);
			d = *(uint32_t *)(last + 12);
#ifdef _UNICODE
			a = _rotl(a, 16);
			b = _rotl(b, 16);
			c = _rotl(c, 16);
			d = _rotl(d, 16);
#else
			a = _byteswap_ulong(a);
			b = _byteswap_ulong(b);
			c = _byteswap_ulong(c);
			d = _byteswap_ulong(d);
#endif
			*(uint32_t *)(last + 12) = a;
			*(uint32_t *)(last + 8) = b;
			*(uint32_t *)(first + 4) = c;
			*(uint32_t *)first = d;
			last -= 8;
			first += 8;
		} while (last >= first);
	}
	offset = last - first;
	if (_add_uintptr(offset, 8, &offset))
	{
		uint32_t a, b;

		a = *(uint32_t *)first;
		b = *(uint32_t *)(first + offset + 4);
#ifdef _UNICODE
		a = _rotl(a, 16);
		b = _rotl(b, 16);
#else
		a = _byteswap_ulong(a);
		b = _byteswap_ulong(b);
#endif
		*(uint32_t *)(first + offset + 4) = a;
		offset -= 8;
		*(uint32_t *)first = b;
		first += 4;
	}
#ifdef _UNICODE
	if (_add_uintptr(offset, 4, &offset))
	{
		uint16_t a, b;

		a = *(uint16_t *)first;
		b = *(uint16_t *)(first + offset + 2);
		*(uint16_t *)(first + offset + 2) = a;
		*(uint16_t *)first = b;
	}
#else
	if (_add_uintptr(offset, 4, &offset))
	{
		uint16_t a, b;

		a = *(uint16_t *)first;
		b = *(uint16_t *)(first + offset + 2);
		a = _rotl16(a, 8);
		b = _rotl16(b, 8);
		*(uint16_t *)(first + offset + 2) = a;
		offset -= 4;
		*(uint16_t *)first = b;
		first += 2;
	}
	if (_add_uintptr(offset, 2, &offset))
	{
		uint8_t a, b;

		a = *first;
		b = *(first + offset + 1);
		*(first + offset + 1) = a;
		*first = b;
	}
#endif
	return string;
#endif
}
#else	// _M_IX86
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
		lea     edi, [edi + eax * 2]                        // edi points to last null char - dword * 4
#else
		add     edi, eax                                    // edi points to last null char - dword * 4
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
		add     edi, 8
		jnc     lt8
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
		add     edi, 4
		jnc     done
		mov     cx, word ptr [esi]                          // get front char...
		mov     dx, word ptr [esi + edi + 2]                //   and end char
		mov     word ptr [esi + edi + 2], cx                // put front char in end...
		mov     word ptr [esi], dx                          //   and end char at front
#else
		add     edi, 4
		jnc     lt4
		mov     cx, word ptr [esi]                          // get front chars...
		mov     dx, word ptr [esi + edi + 2]                //   and end chars
		rol     cx, 8                                       // swap front chars...
		rol     dx, 8                                       //   and end chars
		mov     word ptr [esi + edi + 2], cx                // put front chars in end...
		sub     edi, 4                                      //   and end moves down
		mov     word ptr [esi], dx                          // put end chars in front...
		add     esi, 2                                      //   and front moves up
	lt4:
		add     edi, 2
		jnc     done
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
#endif	// _M_IX86
