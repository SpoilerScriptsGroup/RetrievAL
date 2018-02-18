#if defined(_ultot) && defined(_ui64tot) && defined(_UI32TONT) && defined(INTERNAL_UI32TONT) && defined(_UI64TONT) && defined(INTERNAL_UI64TONT)
#include <windows.h>
#include <stdint.h>
#include "digitstbl.h"

#define _ui64to10t         _UI64TONT(10)
#define _ui64to2t          _UI64TONT(2)
#define _ui64to4t          _UI64TONT(4)
#define _ui64to8t          _UI64TONT(8)
#define _ui64to16t         _UI64TONT(16)
#define _ui64to32t         _UI64TONT(32)
#define internal_ui64tont  INTERNAL_UI64TONT(n)

TCHAR * __cdecl _ultot(unsigned long value, TCHAR *str, int radix);
size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64to2t(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64to4t(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall internal_ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix);

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
		/* check radix */
		switch (radix)
		{
		case 2:
			/* binary */
			_ui64to2t(value, str);
			break;
		case 4:
			/* base 4 */
			_ui64to4t(value, str);
			break;
		case 8:
			/* octal */
			_ui64to8t(value, str);
			break;
		case 10:
			/* decimal */
			_ui64to10t(value, str);
			break;
		case 16:
			/* hexadecimal */
			_ui64to16t(value, str, TRUE);
			break;
		case 32:
			/* base 32 */
			_ui64to32t(value, str, TRUE);
			break;
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
			internal_ui64tont(value, str, TRUE, radix);
			break;
		default:
			/* invalid base */
			*str = TEXT('\0');
			break;
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
		call    _ui64to2t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_4)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    edx
		push    eax
		call    _ui64to4t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_8)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    edx
		push    eax
		call    _ui64to8t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_10)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    edx
		push    eax
		call    _ui64to10t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_16)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    TRUE
		push    edx
		push    eax
		call    _ui64to16t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_32)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    TRUE
		push    edx
		push    eax
		call    _ui64to32t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_OTHER)
		push    ecx
		push    TRUE
		mov     eax, dword ptr [value_lo + 8]
		mov     ecx, dword ptr [str + 8]
		push    edx
		push    eax
		call    internal_ui64tont
		mov     eax, dword ptr [str]
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
