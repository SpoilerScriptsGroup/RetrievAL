#include <windows.h>
#include <stdlib.h>
#include <errno.h>
#ifdef __BORLANDC__
typedef int errno_t;
#endif
#include <limits.h>
#include <tchar.h>
#ifdef __BORLANDC__
#ifdef _UNICODE
#define _tcstoi64  _wcstoi64
#define _tcstoui64 _wcstoui64
#else
#define _tcstoi64  _strtoi64
#define _tcstoui64 _strtoui64
#endif
#endif
#include "atoitbl.h"

#ifdef __BORLANDC__
#define __forceinline static __inline
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#pragma intrinsic(__emulu)
#pragma intrinsic(_addcarry_u32)
#define _add_u32(a, b, out) _addcarry_u32(0, a, b, out)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __emulu(unsigned int a, unsigned int b)
{
	__asm
	{
		mov     edx, dword ptr [b]
		mov     eax, dword ptr [a]
		mul     edx
	}
}
__forceinline unsigned char _add_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	__asm
	{
		mov     ecx, dword ptr [a]
		mov     edx, dword ptr [b]
		add     ecx, edx
		mov     edx, dword ptr [out]
		setc    al
		mov     dword ptr [edx], ecx
	}
}
#else
#define __emulu(a, b) ((unsigned __int64)(unsigned int)(a) * (unsigned int)(b))
__forceinline unsigned char _add_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	return (*out = a + b) < b;
}
#endif

#ifndef __BORLANDC__
#define __msreturn
#endif

#if !defined(_DEBUG) && defined(_MSC_VER)
extern errno_t _terrno;
#define ERRNO _terrno
#endif

#ifdef _MSC_VER
#pragma warning(disable:4102)
#define align16 align 16
#else
#define align16
#endif

#ifdef __BORLANDC__
#pragma warn -8058
#pragma warn -8070
#define _errno __errno
#endif

#ifdef _UNICODE
#define INTERNAL_FUNCTION internal_wcstoi64
#else
#define INTERNAL_FUNCTION internal_strtoi64
#endif

#ifndef _M_IX86
unsigned __int64 __msreturn __stdcall INTERNAL_FUNCTION(BOOL is_unsigned, BOOL is_int64, errno_t *errnoptr, const TCHAR *nptr, TCHAR **endptr, int base);
#else
unsigned __int64 __msreturn __cdecl INTERNAL_FUNCTION(BOOL is_unsigned, errno_t *errnoptr, errno_t reserved1, BOOL is_int64, void *reserved2, const TCHAR *nptr, TCHAR **endptr, int base);
#endif

#ifndef _DEBUG

#define FUNCTION_NAME _tcstol
#define TYPE          long
#define IS_UNSIGNED   FALSE
#define IS_INT64      FALSE
#include "tcstoi64crt.h"

#define FUNCTION_NAME _tcstoul
#define TYPE          unsigned long
#define IS_UNSIGNED   TRUE
#define IS_INT64      FALSE
#include "tcstoi64crt.h"

#define FUNCTION_NAME _tcstoi64
#define TYPE          __int64
#define IS_UNSIGNED   FALSE
#define IS_INT64      TRUE
#include "tcstoi64crt.h"

#define FUNCTION_NAME _tcstoui64
#define TYPE          unsigned __int64
#define IS_UNSIGNED   TRUE
#define IS_INT64      TRUE
#include "tcstoi64crt.h"

#endif

#ifndef _M_IX86

unsigned __int64 __msreturn __stdcall INTERNAL_FUNCTION(BOOL is_unsigned, BOOL is_int64, errno_t *errnoptr, const TCHAR *nptr, TCHAR **endptr, int base)
{
    typedef __int32          int32_t;
    typedef __int64          int64_t;
    typedef unsigned __int32 uint32_t;
    typedef unsigned __int64 uint64_t;

#ifdef _UNICODE
    typedef wchar_t       uchar_t;
#else
    typedef unsigned char uchar_t;
#endif

    const uchar_t *p;
    uchar_t       c, sign;
    uint64_t      value;

    p = nptr;                           // p is our scanning pointer

    c = *p;                             // read char
    while (c == ' ' || (c <= '\r' && c >= '\t'))
        c = *(++p);                     // skip whitespace

    sign = c;                           // remember sign char
    if (c == '-' || c == '+')
        c = *(++p);                     // skip sign

    do {
        if (!base)
            // determine base free-lance, based on first two chars of string
            if (c != '0') {
                base = 10;
                break;
            } else if (p[1] == 'x' || p[1] == 'X')
                base = 16;
            else {
                base = 8;
                break;
            }
        else if (base == 1 || (unsigned int)base > 11 + 'Z' - 'A')
            goto INVALID;               // bad base!
        else if (base != 16 || c != '0' || p[1] != 'x' && p[1] != 'X')
            break;
        c = *(p += 2);
    } while (0);

    if (!CTOI(&c, 'z', base))           // convert c to value
        goto NONUMBER;

    value = c;                          // start with value

    do {
        c = *(++p);                     // read next digit

        if (!CTOI(&c, 'z', base))       // convert c to value
            goto STOPPED32;

    } while (!((value = __emulu((uint32_t)value, base) + c) >> 32));

    if (!is_int64)
        goto OVERFLOW;                  // we would have overflowed

    c = *(++p);                         // read next digit

    while (CTOI(&c, 'z', base)) {       // convert c to value

#if __BYTE_ORDER == __LITTLE_ENDIAN
        #define HI(x) ((uint32_t *)&(x))[1]
#else
        #define HI(x) ((uint32_t *)&(x))[0]
#endif

        uint64_t x;

        if (((x = __emulu((uint32_t)(value >> 32), base)) >> 32) ||
            _add_u32((uint32_t)x, (uint32_t)((value = __emulu((uint32_t)value, base) + c) >> 32), &HI(value)))
            goto OVERFLOW;              // we would have overflowed

        c = *(++p);                     // read next digit

        #undef HI
    }

    if (sign != '-')
        if (is_unsigned || (int64_t)value >= 0)
            goto STORE_POINTER;
        else
            value = _I64_MAX;
    else
        if (is_unsigned || value <= -_I64_MIN)
            goto NEGATE;
        else
            value = _I64_MIN;
    goto OUT_OF_RANGE;

INVALID:
    *errnoptr = EINVAL;

NONUMBER:
    value = 0;                          // no number there
    p = (TCHAR *)nptr;                  // store beginning of string in endptr later on
    goto STORE_POINTER;

OVERFLOW:
    // overflow occurred
    if (endptr)
        // point to end of string
        do
            c = *(++p);
        while (CHECK_CTOI(c, 'z', base));
    value =
        !is_unsigned ?
            !is_int64 ?
                sign != '-' ?
                    (uint32_t)LONG_MAX :
                    (uint32_t)LONG_MIN :
                sign != '-' ?
                    _I64_MAX :
                    _I64_MIN :
            _UI64_MAX;
    goto OUT_OF_RANGE;

STOPPED32:
    do {
        if (sign != '-')
            if ((is_int64 | is_unsigned) || (int32_t)value >= 0)
                break;
            else
                value = LONG_MAX;
        else if ((is_int64 | is_unsigned) || (uint32_t)value <= -LONG_MIN) {
NEGATE:
            value = -(int64_t)value;    // negate result if there was a neg sign
            break;
        } else
            value = (uint32_t)LONG_MIN;
OUT_OF_RANGE:
        *errnoptr = ERANGE;
    } while (0);

STORE_POINTER:
    if (endptr)
        *endptr = (TCHAR *)p;           // store pointer to char that stopped the scan

    return value;                       // done.
}

#else

__declspec(naked) unsigned __int64 __msreturn __cdecl INTERNAL_FUNCTION(BOOL is_unsigned, errno_t *errnoptr, errno_t reserved1, BOOL is_int64, void *reserved2, const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define sizeof_tchar 2
#else
	#define tchar_ptr    byte ptr
	#define sizeof_tchar 1
#endif

	__asm
	{
#ifdef _UNICODE
		#define inc_tchar_ptr add esi, 2
		#define at            ax
		#define ct            cx
		#define bt            bx
#else
		#define inc_tchar_ptr inc esi
		#define at            al
		#define ct            cl
		#define bt            bl
#endif

		push    ebx                                             // store register
		push    ebp
		push    esi
		push    edi

		#define is_unsigned (esp + 4 * (4 + 1))
		#define errnoptr    (esp + 4 * (4 + 2))
		#define reserved1   (esp + 4 * (4 + 3))
		#define is_int64    (esp + 4 * (4 + 4))
		#define reserved2   (esp + 4 * (4 + 5))
		#define nptr        (esp + 4 * (4 + 6))
		#define endptr      (esp + 4 * (4 + 7))
		#define base        (esp + 4 * (4 + 8))
		#define sign        (esp + 4 * (4 + 8))

		mov     esi, dword ptr [nptr]                           // esi is our scanning pointer
		xor     eax, eax                                        // start with zero
		xor     ecx, ecx
		xor     edx, edx
		mov     ct, tchar_ptr [esi]                             // read char
		mov     ebx, dword ptr [base]
		jmp     short L2

		align16
	L1:
		mov     ct, tchar_ptr [esi + sizeof_tchar]              // skip whitespace
		inc_tchar_ptr
	L2:
		cmp     ct, ' '
		je      short L1
		cmp     ct, 0DH
		ja      short L3
		cmp     ct, 09H
		jae     short L1
		jmp     L60

		align16
	L3:
		mov     tchar_ptr [sign], ct                            // store sign char
		cmp     ct, '-'                                         // skip sign
		je      short L4
		cmp     ct, '+'
		jne     short L5
	L4:
		mov     ct, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
	L5:
		cmp     ebx, 1
		jae     short L8
		cmp     ct, '0'                                         // determine base free-lance, based on first two chars of string
		jne     short L6
		mov     ct, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
		cmp     ct, 'x'
		je      short L7
		cmp     ct, 'X'
		je      short L7
		mov     ebx, 8
		jmp     L32
	L6:
		mov     ebx, 10
		jmp     short L10
	L7:
		mov     ebx, 16
		jmp     L21

		align16
	L8:
		je      short L9
		cmp     ebx, 10
		je      short L10
		cmp     ebx, 16
		je      L20
		cmp     ebx, 8
		je      L30
		cmp     ebx, 10 + 'Z' - 'A' + 1
		jbe     L40
	L9:
		mov     ecx, dword ptr [errnoptr]                       // bad base!
		mov     dword ptr [ecx], EINVAL
		jmp     L60

		align16
	L10:
		sub     ct, '0'                                         // base == 10
		cmp     ct, '9' - '0'
		jbe     short L11
		jmp     L60                                             // no number there; return 0 and point to beginning of string

		align16
	L11:
		inc_tchar_ptr
		lea     eax, [eax + eax * 4]
		lea     eax, [ecx + eax * 2]
		mov     ct, tchar_ptr [esi]                             // read next char
		sub     ct, '0'                                         // check and convert char to value
		cmp     ct, '9' - '0'
		ja      short L13
		cmp     eax, 19999999H
		jb      short L11
		jne     short L12
		cmp     ct, 5
		jbe     short L11
	L12:
		cmp     dword ptr [is_int64], 0
		jne     short L14
		jmp     L50
	L13:
		jmp     L61

		align16
	L14:
		mov     edi, eax
		mov     ebp, edx
		inc_tchar_ptr
		add     eax, eax
		adc     edx, edx
		add     eax, eax
		adc     edx, edx
		add     eax, edi
		adc     edx, ebp
		add     eax, eax
		adc     edx, edx
		add     eax, ecx
		adc     edx, 0
		mov     ct, tchar_ptr [esi]                             // read next char
		sub     ct, '0'                                         // check and convert char to value
		cmp     ct, '9' - '0'
		ja      short L16
		cmp     edx, 19999999H
		jb      short L14
		jne     short L15
		cmp     eax, 99999999H
		jb      short L14
		jne     short L15
		cmp     ct, 5
		jbe     short L14
	L15:
		jmp     L51
	L16:
		jmp     L63

		align16
	L20:
		cmp     ct, '0'                                         // base == 16
		jne     short L22
		mov     ct, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
		cmp     ct, 'x'
		je      short L21
		cmp     ct, 'X'
		jne     short L25
	L21:
		mov     ct, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
	L22:
#ifdef _UNICODE
		cmp     ct, 'f'
		ja      short L23
#endif
		mov     ct, tchar_ptr [ttoitbl + ecx * sizeof_tchar]
		cmp     ct, 16
		jb      short L24
	L23:
		jmp     L60                                             // no number there; return 0 and point to beginning of string

		align16
	L24:
		shl     eax, 4
		inc_tchar_ptr
		or      eax, ecx
		mov     ct, tchar_ptr [esi]                             // read next char
	L25:
#ifdef _UNICODE
		cmp     ct, 'f'
		ja      short L26
#endif
		mov     ct, tchar_ptr [ttoitbl + ecx * sizeof_tchar]    // check and convert char to value
		cmp     ct, 16
		jae     short L26
		cmp     eax, 10000000H
		jb      short L24
		mov     ebp, dword ptr [is_int64]
		mov     edi, eax
		test    ebp, ebp
		jnz     short L27
		jmp     L50
	L26:
		jmp     L61

		align16
	L27:
		shl     eax, 4
		inc_tchar_ptr
		shl     edx, 4
		or      eax, ecx
		shr     edi, 28
		mov     ct, tchar_ptr [esi]                             // read next char
		or      edx, edi
		mov     edi, eax
#ifdef _UNICODE
		cmp     ct, 'f'
		ja      short L28
#endif
		mov     ct, tchar_ptr [ttoitbl + ecx * sizeof_tchar]    // check and convert char to value
		cmp     ct, 16
		jae     short L28
		cmp     edx, 10000000H
		jb      short L27
		jmp     L51
	L28:
		jmp     L63

		align16
	L30:
		sub     ct, '0'                                         // base == 8
		cmp     ct, '7' - '0'
		jbe     short L31
		jmp     L60                                             // no number there; return 0 and point to beginning of string

		align16
	L31:
		shl     eax, 3
		inc_tchar_ptr
		or      eax, ecx
		mov     ct, tchar_ptr [esi]                             // read next char
	L32:
		sub     ct, '0'                                         // check and convert char to value
		cmp     ct, '7' - '0'
		ja      short L33
		cmp     eax, 20000000H
		jb      short L31
		mov     ebp, dword ptr [is_int64]
		mov     edi, eax
		test    ebp, ebp
		jnz     short L34
		jmp     L50
	L33:
		jmp     L61

		align16
	L34:
		shl     eax, 3
		inc_tchar_ptr
		shl     edx, 3
		or      eax, ecx
		shr     edi, 29
		mov     ct, tchar_ptr [esi]                             // read next char
		or      edx, edi
		mov     edi, eax
		sub     ct, '0'                                         // check and convert char to value
		cmp     ct, '7' - '0'
		ja      short L35
		cmp     edx, 20000000H
		jb      short L34
		jmp     L51
	L35:
		jmp     L63

		align16
	L40:
#ifdef _UNICODE
		cmp     ct, 'z'
		ja      short L41
#endif
		mov     at, tchar_ptr [ttoitbl + ecx * sizeof_tchar]    // base > 1 && base <= 36 && base != 10 && base != 16 && base != 8
		cmp     at, bt
		jb      short L43
		xor     eax, eax
	L41:
		jmp     L60                                             // no number there; return 0 and point to beginning of string

		align16
	L42:
		mul     ebx
		add     eax, ecx
		adc     edx, 0
		jnz     short L45
	L43:
		mov     ct, tchar_ptr [esi + sizeof_tchar]              // read next char
		inc_tchar_ptr
#ifdef _UNICODE
		cmp     ct, 'z'
		ja      short L44
#endif
		mov     ct, tchar_ptr [ttoitbl + ecx * sizeof_tchar]    // check and convert char to value
		cmp     ct, bt
		jb      short L42
	L44:
		jmp     L61
	L45:
		cmp     dword ptr [is_int64], 0
		jne     short L47
		jmp     short L50

		align16
	L46:
		mov     edi, eax
		mov     eax, edx
		mul     ebx
		jc      short L51
		mov     ebp, eax
		mov     eax, edi
		mul     ebx
		add     eax, ecx
		adc     edx, ebp
		jc      short L51
	L47:
		mov     ct, tchar_ptr [esi + sizeof_tchar]              // read next char
		inc_tchar_ptr
#ifdef _UNICODE
		cmp     ct, 'z'
		ja      short L48
#endif
		mov     ct, tchar_ptr [ttoitbl + ecx * sizeof_tchar]
		cmp     ct, bt
		jb      short L46
	L48:
		jmp     L63

		align16
	L50:
		mov     eax, dword ptr [is_unsigned]                    // overflow there
		mov     edi, dword ptr [endptr]
		test    eax, eax
		jnz     short L52
		cmp     tchar_ptr [sign], '-'
		sete    al
		add     eax, 7FFFFFFFH
		jmp     short L53

		align16
	L51:
		mov     eax, dword ptr [is_unsigned]
		mov     edi, dword ptr [endptr]
		test    eax, eax
		jnz     short L52
		cmp     tchar_ptr [sign], '-'
		sete    al
		mov     edx, 80000000H
		sub     eax, 1
		sbb     edx, 0
		jmp     short L53

		align16
	L52:
		or      eax, -1
		or      edx, -1
	L53:
		mov     ebp, dword ptr [errnoptr]
		test    edi, edi
		mov     dword ptr [ebp], ERANGE
		jnz     short L54
		jmp     L68

		align16
	L54:
		mov     ct, tchar_ptr [esi + sizeof_tchar]              // point to end of string
		inc_tchar_ptr
#ifdef _UNICODE
		cmp     ct, 'z'
		ja      short L55
#endif
		cmp     tchar_ptr [ttoitbl + ecx * sizeof_tchar], bt
		jb      short L54
	L55:
		jmp     L67

		align16
	L60:
		mov     esi, dword ptr [nptr]                           // store beginning of string in endptr
		mov     edi, dword ptr [endptr]
		jmp     short L66
	L61:
		mov     ct, tchar_ptr [sign]
		mov     edi, dword ptr [endptr]
		cmp     ct, '-'
		mov     ecx, dword ptr [is_unsigned]
		mov     ebx, dword ptr [is_int64]
		je      short L62
		or      ecx, ebx
		jnz     short L66
		test    eax, eax
		jns     short L66
		mov     eax, 7FFFFFFFH
		jmp     short L65
	L62:
		neg     eax                                             // negate result if there was a neg sign (x <= ULONG_MAX)
		sbb     edx, edx
		or      ecx, ebx
		jnz     short L66
		test    eax, eax
		jle     short L66
		mov     eax, 80000000H
		jmp     short L65
	L63:
		mov     ct, tchar_ptr [sign]
		mov     edi, dword ptr [endptr]
		cmp     ct, '-'
		je      short L64
		cmp     dword ptr [is_unsigned], 0
		jne     short L66
		test    edx, edx
		jns     short L66
		or      eax, -1
		mov     edx, 7FFFFFFFH
		jmp     short L65
	L64:
		neg     edx                                             // negate result if there was a neg sign (x > ULONG_MAX)
		neg     eax
		sbb     edx, 0
		js      short L66
		cmp     dword ptr [is_unsigned], 0
		jne     short L66
		xor     eax, eax
		mov     edx, 80000000H
	L65:
		mov     ecx, dword ptr [errnoptr]
		mov     dword ptr [ecx], ERANGE
	L66:
		test    edi, edi
		jz      short L68
	L67:
		mov     dword ptr [edi], esi                            // store pointer to char that stopped the scan
	L68:
		pop     edi                                             // restore register
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef inc_tchar_ptr
		#undef at
		#undef ct
		#undef bt
		#undef is_unsigned
		#undef errnoptr
		#undef reserved1
		#undef is_int64
		#undef reserved2
		#undef nptr
		#undef endptr
		#undef base
		#undef sign
	}
	#undef tchar_ptr
	#undef sizeof_tchar
}

#endif
