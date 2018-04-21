#include <windows.h>
#include <stdint.h>
#include <tchar.h>

#ifdef _UNICODE
#define _UI32TONT(n) _ui32to##n##w
#else
#define _UI32TONT(n) _ui32to##n##a
#endif

#define _ui32to10t _UI32TONT(10)
#define _ui32to2t  _UI32TONT(2)
#define _ui32to4t  _UI32TONT(4)
#define _ui32to8t  _UI32TONT(8)
#define _ui32to16t _UI32TONT(16)
#define _ui32to32t _UI32TONT(32)
#define _ui32tont  _UI32TONT(n)

size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to2t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to4t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to8t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to16t(uint32_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui32to32t(uint32_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui32tont(uint32_t value, TCHAR *buffer, BOOL upper, unsigned int radix);

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
	else
		/* the other base */
		_ui32tont(value, str, TRUE, radix);
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

	static const lable_t JumpTable[] = {
		RADIX_OTHER,
		RADIX_OTHER,
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
	};

	__asm
	{
		#define value (esp + 4)
		#define str   (esp + 8)
		#define radix (esp + 12)

		mov     eax, dword ptr [radix]
		mov     edx, dword ptr [str]
		cmp     eax, 32
		ja      L1
		jmp     dword ptr [JumpTable + eax * 4]
	L1:
		jmp     RADIX_OTHER

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
		call    _ui32tont
		mov     eax, dword ptr [str]
		ret

		#undef value
		#undef str
		#undef radix
	}
	#undef DEFINE_LABEL
	#undef LABEL
}
#endif

#include "ui32tont\ui32tont.h"
