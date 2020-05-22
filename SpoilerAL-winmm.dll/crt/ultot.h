#include <windows.h>
#include <stdint.h>
#include <tchar.h>
#include "ui32tot\ui32tot.h"

#define OPTIMIZABLE_C 1

#if !defined(_M_IX86) || OPTIMIZABLE_C
TCHAR * __cdecl _ultot(unsigned long value, TCHAR *str, int radix)
{
	/* check radix */
	switch (radix)
	{
	case 2:
		/* binary */
		_ui32to2t(value, str);
		break;
	case 4:
		/* base 4 */
		_ui32to4t(value, str);
		break;
	case 8:
		/* octal */
		_ui32to8t(value, str);
		break;
	case 10:
		/* decimal */
		_ui32to10t(value, str);
		break;
	case 16:
		/* hexadecimal */
		_ui32to16t(value, str, TRUE);
		break;
	case 32:
		/* base 32 */
		_ui32to32t(value, str, TRUE);
		break;
	case  3: case  5: case  6: case  7: case  9: case 11: case 12: case 13:
	case 14: case 15: case 17: case 18: case 19: case 20: case 21: case 22:
	case 23: case 24: case 25: case 26: case 27: case 28: case 29: case 30:
	case 31: case 33: case 34: case 35: case 36:
		/* the other base */
		internal_ui32tot(value, str, TRUE, radix);
		break;
	default:
		/* invalid */
		*str = TEXT('\0');
		break;
	}
	return str;
}
#else
#pragma warning(disable:4414)

__declspec(naked) TCHAR * __cdecl _ultot(unsigned long value, TCHAR *str, int radix)
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
#ifdef _UNICODE
		#define tchar word
#else
		#define tchar byte
#endif
		#define value (esp + 4)
		#define str   (esp + 8)
		#define radix (esp + 12)

		mov     eax, dword ptr [radix]
		mov     edx, dword ptr [str]
		mov     ecx, dword ptr [value]
		cmp     eax, 36 + 1
		jae     RADIX_INVALID
		jmp     dword ptr [JumpTable + eax * 4]

	LABEL(RADIX_2)
		call    _ui32to2t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_4)
		call    _ui32to4t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_8)
		call    _ui32to8t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_10)
		call    _ui32to10t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_16)
		push    TRUE
		call    _ui32to16t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_32)
		push    TRUE
		call    _ui32to32t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_OTHER)
		push    eax
		push    TRUE
		call    internal_ui32tot
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_INVALID)
		mov     tchar ptr [edx], '\0'
		mov     eax, edx
		ret

		#undef tchar
		#undef value
		#undef str
		#undef radix
	}
	#undef DEFINE_LABEL
	#undef LABEL
}
#endif

#include "ui32tot\ui32tot_source.h"
