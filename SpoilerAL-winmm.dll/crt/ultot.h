#include <windows.h>
#include <stdint.h>
#include <tchar.h>
#include "ui32tot\ui32tot.h"

#define OPTIMIZABLE_C 1

#if !defined(_M_IX86) || OPTIMIZABLE_C
TCHAR * __cdecl _ultot(unsigned long value, TCHAR *str, int radix)
{
	/* check radix */
	if (radix == 10)
		/* decimal */
		_ui32to10t(value, str);
	else if (radix == 16)
		/* hexadecimal */
		_ui32to16t(value, str, TRUE);
	else if (radix == 8)
		/* octal */
		_ui32to8t(value, str);
	else if (radix == 2)
		/* binary */
		_ui32to2t(value, str);
	else if (radix == 4)
		/* base 4 */
		_ui32to4t(value, str);
	else if (radix == 32)
		/* base 32 */
		_ui32to32t(value, str, TRUE);
	else if (radix >= 2 && radix <= 36)
		/* the other base */
		internal_ui32tot(value, str, TRUE, radix);
	else
		*str = TEXT('\0');
	return str;
}
#else
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
		cmp     eax, 36
		ja      L1
		jmp     dword ptr [JumpTable + eax * 4]
	L1:
		jmp     RADIX_INVALID

	LABEL(RADIX_2)
		mov     ecx, dword ptr [value]
		call    _ui32to2t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_4)
		mov     ecx, dword ptr [value]
		call    _ui32to4t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_8)
		mov     ecx, dword ptr [value]
		call    _ui32to8t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_10)
		mov     ecx, dword ptr [value]
		call    _ui32to10t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_16)
		mov     ecx, dword ptr [value]
		push    TRUE
		call    _ui32to16t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_32)
		mov     ecx, dword ptr [value]
		push    TRUE
		call    _ui32to32t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_OTHER)
		mov     ecx, dword ptr [value]
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
