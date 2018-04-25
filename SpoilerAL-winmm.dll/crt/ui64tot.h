#include <windows.h>
#include <stdint.h>
#include <tchar.h>

#ifdef _UNICODE
#define _UI64TONT(n) _ui64to##n##w
#else
#define _UI64TONT(n) _ui64to##n##a
#endif

#define _ui64to10t _UI64TONT(10)
#define _ui64to2t  _UI64TONT(2)
#define _ui64to4t  _UI64TONT(4)
#define _ui64to8t  _UI64TONT(8)
#define _ui64to16t _UI64TONT(16)
#define _ui64to32t _UI64TONT(32)
#define _ui64tont  _UI64TONT(n)

size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64to2t(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64to4t(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer);
size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix);

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
		#define value_lo (esp + 4)
		#define value_hi (esp + 8)
		#define str      (esp + 12)
		#define radix    (esp + 16)

		cmp     dword ptr [value_hi], 0
		jne     L1
		mov     eax, dword ptr [str]
		mov     ecx, dword ptr [radix]
		mov     dword ptr [str - 4], eax
		mov     dword ptr [radix - 4], ecx
		jmp     _ultot
	L1:
		jmp     internal_ui64tot

		#undef value_lo
		#undef value_hi
		#undef str
		#undef radix
	}
}

static TCHAR * __cdecl internal_ui64tot(uint64_t value, TCHAR *str, int radix)
{
#endif

#if !defined(_M_IX86) || OPTIMIZABLE_C
	{
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
		else
			/* the other base */
			_ui64tont(value, str, TRUE, radix);
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
		#define value_lo (esp + 4)
		#define value_hi (esp + 8)
		#define str      (esp + 12)
		#define radix    (esp + 16)

		mov     edx, dword ptr [value_hi]
		mov     eax, dword ptr [radix]
		test    edx, edx
		jnz     L1
		mov     ecx, dword ptr [str]
		mov     dword ptr [str - 4], ecx
		mov     dword ptr [radix - 4], eax
		jmp     _ultot

		align   16
	L1:
		cmp     eax, 32
		ja      L2
		jmp     dword ptr [JumpTable + eax * 4]
	L2:
		jmp     RADIX_OTHER

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
		push    edx
		push    eax
		mov     edx, TRUE
		call    _ui64to16t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_32)
		mov     eax, dword ptr [value_lo]
		mov     ecx, dword ptr [str]
		push    edx
		push    eax
		mov     edx, TRUE
		call    _ui64to32t
		mov     eax, dword ptr [str]
		ret

	LABEL(RADIX_OTHER)
		mov     ecx, dword ptr [str]
		push    eax
		mov     eax, dword ptr [value_lo + 4]
		push    edx
		push    eax
		mov     edx, TRUE
		call    _ui64tont
		mov     eax, dword ptr [str]
		ret

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
