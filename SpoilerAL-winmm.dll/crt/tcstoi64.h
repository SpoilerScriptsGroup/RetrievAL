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
__forceinline unsigned char _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *out)
{
	__asm
	{
		xor     al, al
		mov     cl, byte ptr [c_in]
		cmp     al, cl
		mov     eax, dword ptr [a]
		adc     eax, dword ptr [b]
		mov     ecx, dword ptr [out]
		mov     dword ptr [ecx], eax
		setc    al
	}
}
#else
#define __emulu(a, b) ((unsigned __int64)(unsigned int)(a) * (unsigned int)(b))
__forceinline unsigned char _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *out)
{
	return ((*out = a + b) < a) | (c_in && !++(*out));
}
#endif

#ifndef __BORLANDC__
#define __msreturn
#endif

#ifndef _DEBUG
#define MAKE_CRT 1
#endif

#if !defined(_DEBUG) && defined(_MSC_VER)
extern errno_t _terrno;
#define ERRNO _terrno
#endif

#ifdef _UNICODE
#define INTERNAL_FUNCTION internal_wcstoi64
#else
#define INTERNAL_FUNCTION internal_strtoi64
#endif

unsigned __int64 __msreturn __stdcall INTERNAL_FUNCTION(BOOL is_unsigned, BOOL is_int64, errno_t *errnoptr, const TCHAR *nptr, TCHAR **endptr, int base);

#ifndef _M_IX86
#ifdef __BORLANDC__
#pragma warn -8058
#endif

#if MAKE_CRT
long __cdecl _tcstol(const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef ERRNO
	return (long)INTERNAL_FUNCTION(FALSE, FALSE, &ERRNO, nptr, endptr, base);
#else
	errno_t e = 0;
	long r = (long)INTERNAL_FUNCTION(FALSE, FALSE, &e, nptr, endptr, base);
	if (e)
		errno = e;
	return r;
#endif
}

unsigned long __cdecl _tcstoul(const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef ERRNO
	return (unsigned long)INTERNAL_FUNCTION(TRUE, FALSE, &ERRNO, nptr, endptr, base);
#else
	errno_t e = 0;
	unsigned long r = (unsigned long)INTERNAL_FUNCTION(TRUE, FALSE, &e, nptr, endptr, base);
	if (e)
		errno = e;
	return r;
#endif
}

__int64 __msreturn __cdecl _tcstoi64(const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef ERRNO
	return INTERNAL_FUNCTION(FALSE, TRUE, &ERRNO, nptr, endptr, base);
#else
	errno_t e = 0;
	__int64 r = INTERNAL_FUNCTION(FALSE, TRUE, &e, nptr, endptr, base);
	if (e)
		errno = e;
	return r;
#endif
}

unsigned __int64 __msreturn __cdecl _tcstoui64(const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef ERRNO
	return INTERNAL_FUNCTION(TRUE, TRUE, &ERRNO, nptr, endptr, base);
#else
	errno_t e = 0;
	unsigned __int64 r = INTERNAL_FUNCTION(TRUE, TRUE, &e, nptr, endptr, base);
	if (e)
		errno = e;
	return r;
#endif
}
#endif

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
            _addcarry_u32(0, (uint32_t)x, (uint32_t)((value = __emulu((uint32_t)value, base) + c) >> 32), &HI(value)))
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
#ifdef _MSC_VER
#pragma warning(disable:4102)
#define align16 align 16
#else
#define align16
#endif

#ifdef __BORLANDC__
#pragma warn -8070
#define _errno __errno
#endif

#if MAKE_CRT
__declspec(naked) long __cdecl _tcstol(const TCHAR *nptr, TCHAR **endptr, int base)
{
	__asm
	{
		#define nptr   (esp + 4)
		#define endptr (esp + 8)
		#define base   (esp + 12)

#ifdef ERRNO
		mov     edx, dword ptr [base]
		mov     ecx, dword ptr [endptr]
		mov     eax, dword ptr [nptr]
		push    edx
		push    ecx
		push    eax
		push    offset ERRNO
		push    FALSE
		push    FALSE
		call    INTERNAL_FUNCTION
		ret
#else
		mov     eax, dword ptr [base]
		push    0
		mov     edx, esp
		push    eax
		mov     eax, dword ptr [endptr + 8]
		mov     ecx, dword ptr [nptr + 8]
		push    eax
		push    ecx
		push    edx
		push    FALSE
		push    FALSE
		call    INTERNAL_FUNCTION
		pop     ecx
		test    ecx, ecx
		jz      L1
		push    eax
		push    ecx
		call    _errno
		pop     ecx
		mov     dword ptr [eax], ecx
		pop     eax
	L1:
		ret
#endif

		#undef nptr
		#undef endptr
		#undef base
	}
}

__declspec(naked) unsigned long __cdecl _tcstoul(const TCHAR *nptr, TCHAR **endptr, int base)
{
	__asm
	{
		#define nptr   (esp + 4)
		#define endptr (esp + 8)
		#define base   (esp + 12)

#ifdef ERRNO
		mov     edx, dword ptr [base]
		mov     ecx, dword ptr [endptr]
		mov     eax, dword ptr [nptr]
		push    edx
		push    ecx
		push    eax
		push    offset ERRNO
		push    FALSE
		push    TRUE
		call    INTERNAL_FUNCTION
		ret
#else
		mov     eax, dword ptr [base]
		push    0
		mov     edx, esp
		push    eax
		mov     eax, dword ptr [endptr + 8]
		mov     ecx, dword ptr [nptr + 8]
		push    eax
		push    ecx
		push    edx
		push    FALSE
		push    TRUE
		call    INTERNAL_FUNCTION
		pop     ecx
		test    ecx, ecx
		jz      L1
		push    eax
		push    ecx
		call    _errno
		pop     ecx
		mov     dword ptr [eax], ecx
		pop     eax
	L1:
		ret
#endif

		#undef nptr
		#undef endptr
		#undef base
	}
}

__declspec(naked) __int64 __msreturn __cdecl _tcstoi64(const TCHAR *nptr, TCHAR **endptr, int base)
{
	__asm
	{
		#define nptr   (esp + 4)
		#define endptr (esp + 8)
		#define base   (esp + 12)

#ifdef ERRNO
		mov     edx, dword ptr [base]
		mov     ecx, dword ptr [endptr]
		mov     eax, dword ptr [nptr]
		push    edx
		push    ecx
		push    eax
		push    offset ERRNO
		push    TRUE
		push    FALSE
		call    INTERNAL_FUNCTION
		ret
#else
		mov     eax, dword ptr [base]
		push    0
		mov     edx, esp
		push    eax
		mov     eax, dword ptr [endptr + 8]
		mov     ecx, dword ptr [nptr + 8]
		push    eax
		push    ecx
		push    edx
		push    TRUE
		push    FALSE
		call    INTERNAL_FUNCTION
		pop     ecx
		test    ecx, ecx
		jz      L1
		push    eax
		push    edx
		push    ecx
		call    _errno
		pop     ecx
		pop     edx
		mov     dword ptr [eax], ecx
		pop     eax
	L1:
		ret
#endif

		#undef nptr
		#undef endptr
		#undef base
	}
}

__declspec(naked) unsigned __int64 __msreturn __cdecl _tcstoui64(const TCHAR *nptr, TCHAR **endptr, int base)
{
	__asm
	{
		#define nptr   (esp + 4)
		#define endptr (esp + 8)
		#define base   (esp + 12)

#ifdef ERRNO
		mov     edx, dword ptr [base]
		mov     ecx, dword ptr [endptr]
		mov     eax, dword ptr [nptr]
		push    edx
		push    ecx
		push    eax
		push    offset ERRNO
		push    TRUE
		push    TRUE
		call    INTERNAL_FUNCTION
		ret
#else
		mov     eax, dword ptr [base]
		push    0
		mov     edx, esp
		push    eax
		mov     eax, dword ptr [endptr + 8]
		mov     ecx, dword ptr [nptr + 8]
		push    eax
		push    ecx
		push    edx
		push    TRUE
		push    TRUE
		call    INTERNAL_FUNCTION
		pop     ecx
		test    ecx, ecx
		jz      L1
		push    eax
		push    edx
		push    ecx
		call    _errno
		pop     ecx
		pop     edx
		mov     dword ptr [eax], ecx
		pop     eax
	L1:
		ret
#endif

		#undef nptr
		#undef endptr
		#undef base
	}
}
#endif

__declspec(naked) unsigned __int64 __msreturn __stdcall INTERNAL_FUNCTION(BOOL is_unsigned, BOOL is_int64, errno_t *errnoptr, const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define sizeof_tchar 2
	#define tchar_mask   0FFFFH
#else
	#define tchar_ptr    byte ptr
	#define sizeof_tchar 1
	#define tchar_mask   0FFH
#endif

	__asm
	{
#ifdef _UNICODE
		#define inc_tchar_ptr add esi, 2
		#define tchar         cx
#else
		#define inc_tchar_ptr inc esi
		#define tchar         cl
#endif

		push    ebx                                     // store register
		push    ebp
		push    esi
		push    edi

		#define is_unsigned (esp + 4 * (4 + 1))
		#define is_int64    (esp + 4 * (4 + 2))
		#define errnoptr    (esp + 4 * (4 + 3))
		#define nptr        (esp + 4 * (4 + 4))
		#define endptr      (esp + 4 * (4 + 5))
		#define base        (esp + 4 * (4 + 6))
		#define sign        (esp + 4 * (4 + 6))

		mov     esi, dword ptr [nptr]                   // esi is our scanning pointer
		xor     eax, eax                                // start with zero
		xor     edx, edx
		mov     tchar, tchar_ptr [esi]                  // read char
		mov     ebx, dword ptr [base]
		jmp     short L2

		align16
	L1:
		mov     tchar, tchar_ptr [esi + sizeof_tchar]   // skip whitespace
		inc_tchar_ptr
	L2:
		cmp     tchar, ' '
		je      short L1
		cmp     tchar, 0DH
		ja      short L3
		cmp     tchar, 09H
		jae     short L1
		jmp     L60

		align16
	L3:
		mov     tchar_ptr [sign], tchar                 // store sign char
		and     ecx, tchar_mask
		cmp     tchar, '-'                              // skip sign
		je      short L4
		cmp     tchar, '+'
		jne     short L5
	L4:
		mov     tchar, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
	L5:
		cmp     ebx, 1
		jae     short L8
		cmp     tchar, '0'                              // determine base free-lance, based on first two chars of string
		jne     short L6
		mov     tchar, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
		cmp     tchar, 'x'
		je      short L7
		cmp     tchar, 'X'
		je      short L7
		mov     ebx, 8
		jmp     L33
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
		mov     ecx, dword ptr [errnoptr]               // bad base!
		mov     dword ptr [ecx], EINVAL
		jmp     L60

		align16
	L10:
		sub     tchar, '0'                              // base == 10
		jl      short L11
		cmp     tchar, '9' - '0'
		jbe     short L12
	L11:
		jmp     L60                                     // no number there; return 0 and point to beginning of string

		align16
	L12:
		inc_tchar_ptr
		lea     eax, [eax + eax * 4]
		lea     eax, [ecx + eax * 2]
		mov     tchar, tchar_ptr [esi]                  // read next char
		sub     tchar, '0'                              // check and convert char to value
		jl      short L14
		cmp     tchar, '9' - '0'
		ja      short L14
		cmp     eax, 19999999H
		jb      short L12
		jne     short L13
		cmp     tchar, 5
		jbe     short L12
	L13:
		cmp     dword ptr [is_int64], 0
		jne     short L15
		jmp     L50
	L14:
		jmp     L61

		align16
	L15:
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
		mov     tchar, tchar_ptr [esi]                  // read next char
		sub     tchar, '0'                              // check and convert char to value
		jl      short L17
		cmp     tchar, '9' - '0'
		ja      short L17
		cmp     edx, 19999999H
		jb      short L15
		jne     short L16
		cmp     eax, 99999999H
		jb      short L15
		jne     short L16
		cmp     tchar, 5
		jbe     short L15
	L16:
		jmp     L51
	L17:
		jmp     L63

		align16
	L20:
		cmp     tchar, '0'                              // base == 16
		jne     short L22
		mov     tchar, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
		cmp     tchar, 'x'
		je      short L21
		cmp     tchar, 'X'
		jne     short L25
	L21:
		mov     tchar, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
	L22:
#ifdef _UNICODE
		cmp     tchar, 'f'
		ja      short L23
#endif
		mov     cl, byte ptr atoitbl [ecx]
		cmp     cl, 16
		jb      short L24
	L23:
		jmp     L60                                     // no number there; return 0 and point to beginning of string

		align16
	L24:
		shl     eax, 4
		inc_tchar_ptr
		or      eax, ecx
		mov     tchar, tchar_ptr [esi]                  // read next char
	L25:
#ifdef _UNICODE
		cmp     tchar, 'f'
		ja      short L26
#endif
		mov     cl, byte ptr atoitbl [ecx]              // check and convert char to value
		cmp     cl, 16
		jae     short L26
		test    eax, 0F0000000H
		jz      short L24
		cmp     dword ptr [is_int64], 0
		jne     short L27
		jmp     L50
	L26:
		jmp     L61

		align16
	L27:
		shld    edx, eax, 4
		shl     eax, 4
		inc_tchar_ptr
		or      eax, ecx
		mov     tchar, tchar_ptr [esi]                  // read next char
#ifdef _UNICODE
		cmp     tchar, 'f'
		ja      short L28
#endif
		mov     cl, byte ptr atoitbl [ecx]              // check and convert char to value
		cmp     cl, 16
		jae     short L28
		test    edx, 0F0000000H
		jz      short L27
		jmp     L51
	L28:
		jmp     L63

		align16
	L30:
		sub     tchar, '0'                              // base == 8
		jl      short L31
		cmp     tchar, '7' - '0'
		jbe     short L32
	L31:
		jmp     L60                                     // no number there; return 0 and point to beginning of string

		align16
	L32:
		shl     eax, 3
		inc_tchar_ptr
		or      eax, ecx
		mov     tchar, tchar_ptr [esi]                  // read next char
	L33:
		sub     tchar, '0'                              // check and convert char to value
		jl      short L34
		cmp     tchar, '7' - '0'
		ja      short L34
		test    eax, 0E0000000H
		jz      short L32
		cmp     dword ptr [is_int64], 0
		jne     short L35
		jmp     L50
	L34:
		jmp     L61

		align16
	L35:
		inc_tchar_ptr
		add     eax, eax
		adc     edx, edx
		add     eax, eax
		adc     edx, edx
		add     eax, eax
		adc     edx, edx
		or      eax, ecx
		mov     tchar, tchar_ptr [esi]                  // read next char
		sub     tchar, '0'                              // check and convert char to value
		jl      short L36
		cmp     tchar, '7' - '0'
		ja      short L36
		test    edx, 0E0000000H
		jz      short L35
		jmp     L51
	L36:
		jmp     L63

		align16
	L40:
#ifdef _UNICODE
		cmp     tchar, 'z'
		ja      short L41
#endif
		mov     al, byte ptr atoitbl [ecx]              // base > 1 && base <= 36 && base != 10 && base != 16 && base != 8
		cmp     al, bl
		jb      short L43
		xor     eax, eax
	L41:
		jmp     L60                                     // no number there; return 0 and point to beginning of string

		align16
	L42:
		mul     ebx
		add     eax, ecx
		adc     edx, 0
		jnz     short L45
	L43:
		mov     tchar, tchar_ptr [esi + sizeof_tchar]   // read next char
		inc_tchar_ptr
#ifdef _UNICODE
		cmp     tchar, 'z'
		ja      short L44
#endif
		mov     cl, byte ptr atoitbl [ecx]              // check and convert char to value
		cmp     cl, bl
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
		mov     tchar, tchar_ptr [esi + sizeof_tchar]   // read next char
		inc_tchar_ptr
#ifdef _UNICODE
		cmp     tchar, 'z'
		ja      short L48
#endif
		mov     cl, byte ptr atoitbl [ecx]
		cmp     cl, bl
		jb      short L46
	L48:
		jmp     L63

		align16
	L50:
		mov     eax, dword ptr [is_unsigned]            // overflow there
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
		mov     eax, -1
		mov     edx, -1
	L53:
		mov     ecx, dword ptr [errnoptr]
		test    edi, edi
		mov     dword ptr [ecx], ERANGE
		jnz     short L54
		jmp     L68

		align16
	L54:
		mov     tchar, tchar_ptr [esi + sizeof_tchar]   // point to end of string
		inc_tchar_ptr
#ifdef _UNICODE
		cmp     tchar, 'z'
		ja      short L55
#endif
		cmp     byte ptr atoitbl [ecx], bl
		jb      short L54
	L55:
		jmp     L67

		align16
	L60:
		mov     esi, dword ptr [nptr]                   // store beginning of string in endptr
		mov     edi, dword ptr [endptr]
		jmp     short L66
	L61:
		mov     tchar, tchar_ptr [sign]
		mov     edi, dword ptr [endptr]
		cmp     tchar, '-'
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
		neg     eax                                     // negate result if there was a neg sign (x <= ULONG_MAX)
		sbb     edx, edx
		or      ecx, ebx
		jnz     short L66
		test    eax, eax
		jle     short L66
		mov     eax, 80000000H
		jmp     short L65
	L63:
		mov     tchar, tchar_ptr [sign]
		mov     edi, dword ptr [endptr]
		cmp     tchar, '-'
		je      short L64
		cmp     dword ptr [is_unsigned], 0
		jne     short L66
		test    edx, edx
		jns     short L66
		xor     eax, eax
		mov     edx, 7FFFFFFFH
		dec     eax
		jmp     short L65
	L64:
		neg     edx                                     // negate result if there was a neg sign (x > ULONG_MAX)
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
		mov     dword ptr [edi], esi                    // store pointer to char that stopped the scan
	L68:
		pop     edi                                     // restore register
		pop     esi
		pop     ebp
		pop     ebx
		ret     24

		#undef inc_tchar_ptr
		#undef tchar
		#undef is_unsigned
		#undef is_int64
		#undef errnoptr
		#undef nptr
		#undef endptr
		#undef base
		#undef sign
	}
	#undef tchar_ptr
	#undef sizeof_tchar
	#undef tchar_mask
}
#endif
