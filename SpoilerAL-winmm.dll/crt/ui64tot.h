#if defined(_ultot) && defined(_ui64tot) && defined(_ui32tont) && defined(_ui64tont) && defined(internal_ui64tont)
#include <windows.h>
#include <stdint.h>
#include "digitstbl.h"

#define internal_ui64to10t internal_ui64tont(10)
#define internal_ui64to2t  internal_ui64tont(2)
#define internal_ui64to4t  internal_ui64tont(4)
#define internal_ui64to8t  internal_ui64tont(8)
#define internal_ui64to16t internal_ui64tont(16)
#define internal_ui64to32t internal_ui64tont(32)

TCHAR * __cdecl _ultot(unsigned long value, TCHAR *str, int radix);
size_t __fastcall internal_ui64to10t(uint64_t value, TCHAR *buffer);
size_t __fastcall internal_ui64to2t(uint64_t value, TCHAR *buffer);
size_t __fastcall internal_ui64to4t(uint64_t value, TCHAR *buffer);
size_t __fastcall internal_ui64to8t(uint64_t value, TCHAR *buffer);
size_t __fastcall internal_ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall internal_ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper);

#define OPTIMIZABLE_C 1

#if !defined(_M_IX86)
TCHAR * __cdecl _ui64tot(unsigned __int64 value, TCHAR *str, int radix)
{
	if (!(value >> 32))
	{
		return _ultot((unsigned long)value, str, radix);
	}
	else
#elif OPTIMIZABLE_C
__declspec(naked) TCHAR * __cdecl _ui64tot(unsigned __int64 value, TCHAR *str, int radix)
{
	static TCHAR * __cdecl internal_ui64tot(uint64_t value, TCHAR *str, int radix);

	__asm
	{
		cmp     dword ptr [esp + 8], 0
		jne     L1
		mov     eax, dword ptr [esp + 12]
		mov     ecx, dword ptr [esp + 16]
		mov     dword ptr [esp +  8], eax
		mov     dword ptr [esp + 12], ecx
		jmp     _ultot
	L1:
		jmp     internal_ui64tot
	}
}

static TCHAR * __cdecl internal_ui64tot(uint64_t value, TCHAR *str, int radix)
{
#endif

#if !defined(_M_IX86) || OPTIMIZABLE_C
	{
		TCHAR *p1, *p2;

		/* check radix */
		switch (radix)
		{
		case 2:
			/* binary */
			internal_ui64to2t(value, str);
			return str;
		case 4:
			/* base 4 */
			internal_ui64to4t(value, str);
			return str;
		case 8:
			/* octal */
			internal_ui64to8t(value, str);
			return str;
		case 10:
			/* decimal */
			internal_ui64to10t(value, str);
			return str;
		case 16:
			/* hexadecimal */
			internal_ui64to16t(value, str, TRUE);
			return str;
		case 32:
			/* base 32 */
			internal_ui64to32t(value, str, TRUE);
			return str;
		case 3: case 5: case 6: case 7: case 9:
		case 11 + 'A' - 'A':
		case 11 + 'B' - 'A':
		case 11 + 'C' - 'A':
		case 11 + 'D' - 'A':
		case 11 + 'E' - 'A':
		case 11 + 'G' - 'A':
		case 11 + 'H' - 'A':
		case 11 + 'I' - 'A':
		case 11 + 'J' - 'A':
		case 11 + 'K' - 'A':
		case 11 + 'L' - 'A':
		case 11 + 'M' - 'A':
		case 11 + 'N' - 'A':
		case 11 + 'O' - 'A':
		case 11 + 'P' - 'A':
		case 11 + 'Q' - 'A':
		case 11 + 'R' - 'A':
		case 11 + 'S' - 'A':
		case 11 + 'T' - 'A':
		case 11 + 'U' - 'A':
		case 11 + 'W' - 'A':
		case 11 + 'X' - 'A':
		case 11 + 'Y' - 'A':
		case 11 + 'Z' - 'A':
			/* the other base */
			p1 = str;
			do
			{
				unsigned long remainder;

				remainder = value % (unsigned int)radix;
				value /= (unsigned int)radix;
				*(p1++) = (TCHAR)digitsHexLarge[remainder];
			} while (value);
			break;
		default:
			/* invalid base */
			*str = TEXT('\0');
			return str;
		}

		/* We now have the digit of the number in the buffer, but in reverse */
		/* order.  Thus we reverse them now.                                 */

		*(p1--) = TEXT('\0');   /* terminate string; p points to last digit */
		p2 = str;
		while (p1 > p2)         /* repeat until halfway */
		{
			TCHAR c1, c2;

			c1 = *p1;           /* swap *p1 and *p2 */
			c2 = *p2;
			*(p1--) = c2;
			*(p2++) = c1;       /* advance to next two digits */
		}
		return str;
	}
}
#else
__declspec(naked) TCHAR * __cdecl _ui64tot(unsigned __int64 value, TCHAR *str, int radix)
{
	#define DEFINE_LABEL(label) static void __cdecl label()

	#define LABEL(label)                          \
	    }                                         \
	}                                             \
	__declspec(naked) static void __cdecl label() \
	{                                             \
	    __asm                                     \
	    {

	typedef void(__cdecl *lable_t)();

	DEFINE_LABEL(RADIX_2);
	DEFINE_LABEL(RADIX_4);
	DEFINE_LABEL(RADIX_8);
	DEFINE_LABEL(RADIX_10);
	DEFINE_LABEL(RADIX_16);
	DEFINE_LABEL(RADIX_32);
	DEFINE_LABEL(RADIX_OTHER);
	DEFINE_LABEL(RADIX_INVALID);

	static const lable_t JumpTable[] = {
		RADIX_INVALID,
		RADIX_INVALID,
		RADIX_2,
		RADIX_OTHER,
		RADIX_4,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_8,
		RADIX_OTHER,
		RADIX_10,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_16,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_32,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
		RADIX_OTHER,
	};

	__asm
	{
#ifndef _UNICODE
		#define t(r)          r##l
		#define tchar         byte
		#define movt          mov
		#define inc_tchar(r)  inc r
		#define dec_tchar(r)  dec r
#else
		#define t(r)          r##x
		#define tchar         word
		#define movt          movzx
		#define inc_tchar(r)  add r, 2
		#define dec_tchar(r)  sub r, 2
#endif
		#define value_lo (esp + 4)
		#define value_hi (esp + 8)
		#define str      (esp + 12)
		#define radix    (esp + 16)

		mov     edx, dword ptr [value_hi]
		mov     ecx, dword ptr [radix]
		test    edx, edx
		jnz     L1
		mov     eax, dword ptr [str]
		mov     dword ptr [str - 4], eax
		mov     dword ptr [radix - 4], ecx
		jmp     _ultot

		align   16
	L1:
		cmp     ecx, 36
		ja      L2
		jmp     dword ptr [JumpTable + ecx * 4]
	L2:
		jmp     RADIX_INVALID

	LABEL(RADIX_2)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    edx
		push    eax
		call    internal_ui64to2t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_4)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    edx
		push    eax
		call    internal_ui64to4t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_8)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    edx
		push    eax
		call    internal_ui64to8t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_10)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    edx
		push    eax
		call    internal_ui64to10t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_16)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    TRUE
		push    edx
		push    eax
		call    internal_ui64to16t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_32)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    TRUE
		push    edx
		push    eax
		call    internal_ui64to32t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_OTHER)
		push    ebx
		push    esi
		push    edi
		mov     ebx, edx
		mov     esi, dword ptr [value_lo + 12]
		mov     edi, dword ptr [str + 12]

		#define r_value_lo esi
		#define r_value_hi ebx
		#define r_radix    ecx
		#define p1         edi
		#define p2         ecx

		align   16
	L3:
		mov     eax, r_value_hi
		xor     edx, edx
		div     r_radix
		mov     r_value_hi, eax
		mov     eax, r_value_lo
		div     r_radix
		mov     r_value_lo, eax
		mov     edx, eax
		imul    eax, r_radix
		sub     edx, eax
		movt    t(a), byte ptr [digitsHexLarge + edx]
		mov     edx, r_value_lo
		mov     tchar ptr [p1], t(a)
		inc_tchar(p1)
		or      edx, r_value_hi
		jnz     L3

		mov     eax, dword ptr [str + 12]
		mov     tchar ptr [p1], '\0'
		dec_tchar(p1)
		mov     p2, eax
		cmp     p1, eax
		jbe     L5

		align   16
	L4:
		mov     t(b), tchar ptr [p1]
		mov     t(d), tchar ptr [p2]
		mov     tchar ptr [p1], t(d)
		mov     tchar ptr [p2], t(b)
		dec_tchar(p1)
		inc_tchar(p2)
		cmp     p1, p2
		ja      L4
	L5:
		#undef  r_value_lo
		#undef  r_value_hi
		#undef  r_radix
		#undef  p1
		#undef  p2

		pop     edi
		pop     esi
		pop     ebx
		ret

	LABEL(RADIX_INVALID)
		mov     eax, dword ptr [str]
		mov     tchar ptr [eax], '\0'
		ret

		#undef t
		#undef tchar
		#undef movt
		#undef inc_tchar
		#undef dec_tchar
		#undef value_lo
		#undef value_hi
		#undef str
		#undef radix
	}
	#undef DEFINE_LABEL
	#undef LABEL
}
#endif

#include "ui64tont\ui64tont.h"

#endif
