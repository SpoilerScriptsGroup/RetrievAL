#include <windows.h>
#include <stdint.h>
#include <tchar.h>
#include "ui64tot\ui64tot.h"

#define OPTIMIZABLE_C 1

#if !defined(_M_IX86) || OPTIMIZABLE_C
TCHAR * __cdecl _ui64tot(unsigned __int64 value, TCHAR *str, int radix)
{
	/* check radix */
	if (radix == 10)
		/* decimal */
		_ui64to10t(value, str);
	else if (radix == 16)
		/* hexadecimal */
		_ui64to16t(value, str, TRUE);
	else if (radix == 8)
		/* octal */
		_ui64to8t(value, str);
	else if (radix == 2)
		/* binary */
		_ui64to2t(value, str);
	else if (radix == 4)
		/* base 4 */
		_ui64to4t(value, str);
	else if (radix == 32)
		/* base 32 */
		_ui64to32t(value, str, TRUE);
	else if (radix >= 2 && radix <= 36)
		/* the other base */
		internal_ui64tot(value, str, TRUE, radix);
	else
		*str = TEXT('\0');
	return str;
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
#ifdef _UNICODE
		#define tchar word
#else
		#define tchar byte
#endif
		#define value_lo (esp + 4)
		#define value_hi (esp + 8)
		#define str      (esp + 12)
		#define radix    (esp + 16)

		mov     eax, dword ptr [radix]
		mov     ecx, dword ptr [str]
		cmp     eax, 36
		ja      L2
		jmp     dword ptr [JumpTable + eax * 4]
	L2:
		jmp     RADIX_INVALID

	LABEL(RADIX_2)
		mov     edx, dword ptr [value_hi]
		mov     eax, dword ptr [value_lo]
		push    edx
		push    eax
		call    _ui64to2t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_4)
		mov     edx, dword ptr [value_hi]
		mov     eax, dword ptr [value_lo]
		push    edx
		push    eax
		call    _ui64to4t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_8)
		mov     edx, dword ptr [value_hi]
		mov     eax, dword ptr [value_lo]
		push    edx
		push    eax
		call    _ui64to8t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_10)
		mov     edx, dword ptr [value_hi]
		mov     eax, dword ptr [value_lo]
		push    edx
		push    eax
		call    _ui64to10t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_16)
		mov     edx, dword ptr [value_hi]
		mov     eax, dword ptr [value_lo]
		push    edx
		push    eax
		mov     edx, TRUE
		call    _ui64to16t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_32)
		mov     edx, dword ptr [value_hi]
		mov     eax, dword ptr [value_lo]
		push    edx
		push    eax
		mov     edx, TRUE
		call    _ui64to32t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_OTHER)
		mov     edx, dword ptr [value_hi]
		push    eax
		mov     eax, dword ptr [value_lo + 4]
		push    edx
		push    eax
		mov     edx, TRUE
		call    internal_ui64tot
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_INVALID)
		mov     tchar ptr [ecx], '\0'
		mov     eax, ecx
		ret

		#undef tchar
		#undef value_lo
		#undef value_hi
		#undef str
		#undef radix
	}
	#undef DEFINE_LABEL
	#undef LABEL
}
#endif

#include "ui64tot\ui64tot_source.h"
