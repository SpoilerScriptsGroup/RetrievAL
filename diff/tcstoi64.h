#include <windows.h>
#include <stdlib.h>
#include <errno.h>
#ifdef __BORLANDC__
typedef int errno_t;
#endif
#include <tchar.h>
#ifdef __BORLANDC__
#ifdef _UNICODE
#define _tcstol    wcstol
#define _tcstoul   wcstoul
#define _tcstoi64  _wcstoi64
#define _tcstoui64 _wcstoui64
#else
#define _tcstol    strtol
#define _tcstoul   strtoul
#define _tcstoi64  _strtoi64
#define _tcstoui64 _strtoui64
#endif
#endif
#include "atoitbl.h"

#if MAKE_CRT && defined(_MSC_VER)
EXTERN_C errno_t _terrno;
#endif

#ifndef __BORLANDC__
#define __msreturn
#endif

#ifdef _UNICODE
#define INTERNAL_FUNCTION internal_wcstoi64
#else
#define INTERNAL_FUNCTION internal_strtoi64
#endif

unsigned __int64 __msreturn __stdcall INTERNAL_FUNCTION(BOOL is_unsigned, BOOL is_int64, errno_t *errnoptr, const TCHAR *nptr, TCHAR **endptr, int base);

#ifdef __M_IX86
#ifdef __BORLANDC__
#pragma warn -8058
#endif

#if MAKE_CRT
#ifdef _MSC_VER
#define errno _terrno
#endif

long __cdecl _tcstol(const TCHAR *nptr, TCHAR **endptr, int base)
{
	return (long)INTERNAL_FUNCTION(FALSE, FALSE, &errno, nptr, endptr, base);
}

unsigned long __cdecl _tcstoul(const TCHAR *nptr, TCHAR **endptr, int base)
{
	return (unsigned long)INTERNAL_FUNCTION(TRUE, FALSE, &errno, nptr, endptr, base);
}

__int64 __msreturn __cdecl _tcstoi64(const TCHAR *nptr, TCHAR **endptr, int base)
{
	return INTERNAL_FUNCTION(FALSE, TRUE, &errno, nptr, endptr, base);
}

unsigned __int64 __msreturn __cdecl _tcstoui64(const TCHAR *nptr, TCHAR **endptr, int base)
{
	return INTERNAL_FUNCTION(TRUE, TRUE, &errno, nptr, endptr, base);
}
#endif

unsigned __int64 __msreturn __stdcall INTERNAL_FUNCTION(BOOL is_unsigned, BOOL is_int64, errno_t *errnoptr, const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef _UNICODE
    typedef wchar_t       uchar_t;
    typedef short         schar_t;
#else
    typedef unsigned char uchar_t;
    typedef char          schar_t;
#endif

    #define base (*(unsigned int *)&base)

    const uchar_t    *p;
    uchar_t          c;
    uchar_t          sign;
    unsigned __int64 number;
    unsigned __int64 maxval;
    uchar_t          maxrem;

    p = nptr;                           // p is our scanning pointer

    c = *p;                             // read char
    while (c == ' ' || (c <= '\r' && c >= '\t'))
        c = *(++p);                     // skip whitespace

    sign = c;                           // remember sign char
    if (c == '-' || c == '+')
        c = *(++p);                     // skip sign

    do {
        if (base == 1 || (unsigned int)base > 11 + 'Z' - 'A')
            goto INVALID;               // bad base!
        else if (base == 0)
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
        else if (base != 16 || c != '0' || p[1] != 'x' && p[1] != 'X')
            break;
        c = *(p += 2);
    } while (0);

    // convert c to value
#ifdef _UNICODE
    if (c > (uchar_t)'z' || (c = ATOITBL(c)) >= (uchar_t)base)
#else
    if ((c = ATOITBL(c)) >= (uchar_t)base)
#endif
        goto NONUMBER;

    number = 0;                         // start with zero

    if (!is_int64)
    {
        #define number (*(unsigned long *)&number)
        #define maxval (*(unsigned long *)&maxval)

        // if our number exceeds this, we will overflow on multiply
        maxval = !is_unsigned ? sign != '-' ? LONG_MAX : -LONG_MIN : ULONG_MAX;
        maxrem = (uchar_t)(maxval % base);
        maxval = maxval / base;

        for (; ; )                      // exit in middle of loop
        {
            number = number * base + c; // we won't overflow, go ahead and multiply

            c = *(++p);                 // read next digit

            // convert c to value
#if defined(_UNICODE)
            if (c > (uchar_t)'z' || (c = ATOITBL(c)) >= (uchar_t)base)
#else
            if ((c = ATOITBL(c)) >= (uchar_t)base)
#endif
                break;                  // exit loop if bad digit found

            /* we now need to compute number = number * base + digit,
               but we need to know if overflow occured.  This requires
               a tricky pre-check. */
            if (number < maxval || number == maxval && c <= maxrem)
                continue;
            else
                goto OVERFLOW;          // we would have overflowed
        }

        if (sign == '-')
            number = -(long)number;     // negate result if there was a neg sign

        #undef number
        #undef maxval
    }
    else
    {
        // if our number exceeds this, we will overflow on multiply
        maxval = !is_unsigned ? sign != '-' ? _I64_MAX : -_I64_MIN : _UI64_MAX;
        maxrem = (uchar_t)(maxval % base);
        maxval = maxval / base;

        for (; ; )                      // exit in middle of loop
        {
            number = number * base + c; // we won't overflow, go ahead and multiply

            c = *(++p);                 // read next digit

            // convert c to value
#ifdef _UNICODE
            if (c > (uchar_t)'z' || (c = ATOITBL(c)) >= (uchar_t)base)
#else
            if ((c = ATOITBL(c)) >= (uchar_t)base)
#endif
                break;                  // exit loop if bad digit found

            /* we now need to compute number = number * base + digit,
               but we need to know if overflow occured.  This requires
               a tricky pre-check. */
            if (number < maxval || number == maxval && c <= maxrem)
                continue;
            else
                goto OVERFLOW;          // we would have overflowed
        }

        if (sign == '-')
            number = -(__int64)number;  // negate result if there was a neg sign
    }

    if (endptr)
        *endptr = (TCHAR *)p;           // store pointer to char that stopped the scan

    return number;                      // done.

INVALID:
    *errnoptr = EINVAL;

NONUMBER:
    // no number there
    if (endptr)
        *endptr = (TCHAR *)nptr;        // store beginning of string in endptr later on
    return 0;                           // return 0

OVERFLOW:
    // overflow occurred
    if (endptr)
    {
        // point to end of string
#ifdef _UNICODE
        while ((c = *(++p)) <= (uchar_t)'z' && ATOITBL(c) < (unsigned char)base);
#else
        while (ATOITBL(*(++p)) < (unsigned char)base);
#endif
        *endptr = (TCHAR *)p;           // store pointer to char that stopped the scan
    }
    *errnoptr = ERANGE;
    return
        !is_unsigned ?
            !is_int64 ?
                sign != '-' ?
                    (unsigned long)LONG_MAX :
                    (unsigned long)LONG_MIN :
                sign != '-' ?
                    _I64_MAX :
                    _I64_MIN :
            _UI64_MAX;

    #undef base
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
		mov     edx, dword ptr [esp + 12]
		mov     ecx, dword ptr [esp + 8]
		mov     eax, dword ptr [esp + 4]
		push    edx
		push    ecx
		push    eax
#ifdef _MSC_VER
		push    offset _terrno
#else
		call    _errno
		push    eax
#endif
		push    FALSE
		push    FALSE
		call    INTERNAL_FUNCTION
		ret
	}
}

__declspec(naked) unsigned long __cdecl _tcstoul(const TCHAR *nptr, TCHAR **endptr, int base)
{
	__asm
	{
		mov     edx, dword ptr [esp + 12]
		mov     ecx, dword ptr [esp + 8]
		mov     eax, dword ptr [esp + 4]
		push    edx
		push    ecx
		push    eax
#ifdef _MSC_VER
		push    offset _terrno
#else
		call    _errno
		push    eax
#endif
		push    TRUE
		push    FALSE
		call    INTERNAL_FUNCTION
		ret
	}
}

__declspec(naked) __int64 __msreturn __cdecl _tcstoi64(const TCHAR *nptr, TCHAR **endptr, int base)
{
	__asm
	{
		mov     edx, dword ptr [esp + 12]
		mov     ecx, dword ptr [esp + 8]
		mov     eax, dword ptr [esp + 4]
		push    edx
		push    ecx
		push    eax
#ifdef _MSC_VER
		push    offset _terrno
#else
		call    _errno
		push    eax
#endif
		push    FALSE
		push    TRUE
		call    INTERNAL_FUNCTION
		ret
	}
}

__declspec(naked) unsigned __int64 __msreturn __cdecl _tcstoui64(const TCHAR *nptr, TCHAR **endptr, int base)
{
	__asm
	{
		mov     edx, dword ptr [esp + 12]
		mov     ecx, dword ptr [esp + 8]
		mov     eax, dword ptr [esp + 4]
		push    edx
		push    ecx
		push    eax
#ifdef _MSC_VER
		push    offset _terrno
#else
		call    _errno
		push    eax
#endif
		push    TRUE
		push    TRUE
		call    INTERNAL_FUNCTION
		ret
	}
}
#endif

__declspec(naked) unsigned __int64 __msreturn __stdcall INTERNAL_FUNCTION(BOOL is_unsigned, BOOL is_int64, errno_t *errnoptr, const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define sizeof_tchar 2
	#define tchar_max    0FFFFH
#else
	#define tchar_ptr    byte ptr
	#define sizeof_tchar 1
	#define tchar_max    0FFH
#endif

	__asm
	{
#ifdef _UNICODE
		#define inc_tchar_ptr add esi, 2
		#define t             cx
#else
		#define inc_tchar_ptr inc esi
		#define t             cl
#endif

		push    ebx                                 // store register
		push    esi
		push    edi
		push    ebp

		#define is_unsigned (esp + 4 * (4 + 1))
		#define is_int64    (esp + 4 * (4 + 2))
		#define errnoptr    (esp + 4 * (4 + 3))
		#define nptr        (esp + 4 * (4 + 4))
		#define endptr      (esp + 4 * (4 + 5))
		#define base        (esp + 4 * (4 + 6))
		#define sign        (esp + 4 * (4 + 6))

		mov     esi, dword ptr [nptr]               // esi is our scanning pointer
		xor     eax, eax                            // start with zero
		xor     edx, edx
		mov     t, tchar_ptr [esi]                  // read char
		mov     ebx, dword ptr [base]
		jmp     short L2

		align16
	L1:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // skip whitespace
		inc_tchar_ptr
	L2:
		cmp     t, ' '
		je      short L1
		cmp     t, 0DH
		ja      short L3
		cmp     t, 09H
		jae     short L1
		jmp     L66

		align16
	L3:
		mov     tchar_ptr [sign], t                 // store sign char
		and     ecx, tchar_max
		cmp     t, '-'                              // skip sign
		je      short L4
		cmp     t, '+'
		jne     short L5
	L4:
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr

	L5:
		cmp     ebx, 1
		jae     short L8
		cmp     t, '0'                              // determine base free-lance, based on first two chars of string
		jne     short L6
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
		cmp     t, 'x'
		je      short L7
		cmp     t, 'X'
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
		mov     ecx, dword ptr [errnoptr]           // bad base!
		mov     dword ptr [ecx], EINVAL
		jmp     L66

		align16
	L10:
		sub     t, '0'                              // base == 10
		jl      short L11
		cmp     t, '9' - '0'
		jbe     short L12
	L11:
		jmp     L66                                 // no number there; return 0 and point to beginning of string

		align16
	L12:
		inc_tchar_ptr
		lea     eax, [eax + eax * 4]
		lea     eax, [ecx + eax * 2]
		mov     t, tchar_ptr [esi]                  // read next char
		sub     t, '0'                              // check and convert char to value
		jl      short L14
		cmp     t, '9' - '0'
		ja      short L14
		cmp     eax, 19999999H
		jb      short L12
		jne     short L13
		cmp     t, 5
		jbe     short L12
	L13:
		cmp     dword ptr [is_int64], 0
		jne     short L15
		jmp     L50
	L14:
		jmp     L60

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
		mov     t, tchar_ptr [esi]                  // read next char
		sub     t, '0'                              // check and convert char to value
		jl      short L17
		cmp     t, '9' - '0'
		ja      short L17
		cmp     edx, 19999999H
		jb      short L15
		jne     short L16
		cmp     eax, 99999999H
		jb      short L15
		jne     short L16
		cmp     t, 5
		jbe     short L15
	L16:
		jmp     L51
	L17:
		jmp     L64

		align16
	L20:
		cmp     t, '0'                              // base == 16
		jne     short L22
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
		cmp     t, 'x'
		je      short L21
		cmp     t, 'X'
		jne     short L25
	L21:
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
	L22:
#ifdef _UNICODE
		cmp     t, 'f'
		ja      short L23
#endif
		mov     cl, byte ptr atoitbl [ecx]
		cmp     cl, 16
		jb      short L24
	L23:
		jmp     L66                                 // no number there; return 0 and point to beginning of string

		align16
	L24:
		shl     eax, 4
		inc_tchar_ptr
		or      eax, ecx
		mov     t, tchar_ptr [esi]                  // read next char
	L25:
#ifdef _UNICODE
		cmp     t, 'f'
		ja      short L26
#endif
		mov     cl, byte ptr atoitbl [ecx]          // check and convert char to value
		cmp     cl, 16
		jae     short L26
		test    eax, 0F0000000H
		jz      short L24
		cmp     dword ptr [is_int64], 0
		jne     short L27
		jmp     L50
	L26:
		jmp     L60

		align16
	L27:
		shld    edx, eax, 4
		shl     eax, 4
		inc_tchar_ptr
		or      eax, ecx
		mov     t, tchar_ptr [esi]                  // read next char
#ifdef _UNICODE
		cmp     t, 'f'
		ja      short L28
#endif
		mov     cl, byte ptr atoitbl [ecx]          // check and convert char to value
		cmp     cl, 16
		jae     short L28
		test    edx, 0F0000000H
		jz      short L27
		jmp     L51
	L28:
		jmp     L64

		align16
	L30:
		sub     t, '0'                              // base == 8
		jl      short L31
		cmp     t, '7' - '0'
		jbe     short L32
	L31:
		jmp     L66                                 // no number there; return 0 and point to beginning of string

		align16
	L32:
		shl     eax, 3
		inc_tchar_ptr
		or      eax, ecx
		mov     t, tchar_ptr [esi]                  // read next char
	L33:
		sub     t, '0'                              // check and convert char to value
		jl      short L34
		cmp     t, '7' - '0'
		ja      short L34
		test    eax, 0E0000000H
		jz      short L32
		cmp     dword ptr [is_int64], 0
		jne     short L35
		jmp     L50
	L34:
		jmp     L60

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
		mov     t, tchar_ptr [esi]                  // read next char
		sub     t, '0'                              // check and convert char to value
		jl      short L36
		cmp     t, '7' - '0'
		ja      short L36
		test    edx, 0E0000000H
		jz      short L35
		jmp     L51
	L36:
		jmp     L64

		align16
	L40:
#ifdef _UNICODE
		cmp     t, 'z'
		ja      short L41
#endif
		mov     cl, byte ptr atoitbl [ecx]          // base > 1 && base <= 36 && base != 10 && base != 16 && base != 8
		cmp     cl, bl
		jb      short L42
	L41:
		jmp     L66                                 // no number there; return 0 and point to beginning of string

		align16
	L42:
		mul     ebx
		inc_tchar_ptr
		add     eax, ecx
		adc     edx, 0
		mov     t, tchar_ptr [esi]                  // read next char
#ifdef _UNICODE
		cmp     t, 'z'
		ja      short L43
#endif
		mov     cl, byte ptr atoitbl [ecx]          // check and convert char to value
		cmp     cl, bl
		jae     short L43
		test    edx, edx
		jz      short L42
		cmp     dword ptr [is_int64], 0
		jne     short L44
		jmp     short L50
	L43:
		jmp     L60

		align16
	L44:
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
		mov     t, tchar_ptr [esi + sizeof_tchar]   // read next char
		inc_tchar_ptr
#ifdef _UNICODE
		cmp     t, 'z'
		ja      short L45
#endif
		mov     cl, byte ptr atoitbl [ecx]
		cmp     cl, bl
		jb      short L44
	L45:
		jmp     L64

		align16
	L50:
		mov     eax, dword ptr [is_unsigned]        // overflow there
		mov     edi, dword ptr [endptr]
		test    eax, eax
		jz      short L52
		xor     eax, eax
		cmp     tchar_ptr [sign], '-'
		sete    al
		add     eax, 7FFFFFFFH
		jmp     short L53

		align16
	L51:
		mov     eax, dword ptr [is_unsigned]
		mov     edi, dword ptr [endptr]
		test    eax, eax
		jz      short L52
		xor     eax, eax
		cmp     tchar_ptr [sign], '-'
		sete    al
		xor     edx, edx
		add     eax, -1
		adc     edx, 7FFFFFFFH
		jmp     short L53

		align16
	L52:
		dec     eax
		mov     edx, -1
	L53:
		mov     ebp, dword ptr [errnoptr]
		test    edi, edi
		mov     dword ptr [ebp], ERANGE
		jnz     short L54
		jmp     L69

		align16
	L54:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // point to end of string
		inc_tchar_ptr
#ifdef _UNICODE
		cmp     t, 'z'
		ja      short L55
#endif
		cmp     byte ptr atoitbl [ecx], bl
		jb      short L54
	L55:
		jmp     L68

		align16
	L60:
		mov     t, tchar_ptr [sign]
		mov     edi, dword ptr [endptr]
		cmp     t, '-'
		je      short L62
		mov     ecx, dword ptr [is_unsigned]
		mov     ebx, dword ptr [is_int64]
		or      ecx, ebx
		jnz     short L61
		test    eax, eax
		jns     short L61
		mov     ecx, dword ptr [errnoptr]
		mov     eax, 7FFFFFFFH
		mov     dword ptr [ecx], ERANGE
	L61:
		jmp     L67

		align16
	L62:
		dec     eax                                 // negate result if there was a neg sign (x <= ULONG_MAX)
		mov     edx, -1
		xor     eax, -1
		jle     short L63
		mov     ecx, dword ptr [is_unsigned]
		mov     ebx, dword ptr [is_int64]
		or      ecx, ebx
		jnz     short L63
		mov     ecx, dword ptr [errnoptr]
		mov     eax, 80000000H
		mov     dword ptr [ecx], ERANGE
	L63:
		jmp     short L67

		align16
	L64:
		mov     t, tchar_ptr [sign]
		mov     edi, dword ptr [endptr]
		cmp     t, '-'
		je      short L65
		cmp     dword ptr [is_unsigned], 0
		jne     short L67
		test    edx, edx
		jns     short L67
		mov     ecx, dword ptr [errnoptr]
		mov     edx, 7FFFFFFFH
		mov     dword ptr [ecx], ERANGE
		mov     eax, -1
		jmp     short L67

		align16
	L65:
		neg     edx                                 // negate result if there was a neg sign (x > ULONG_MAX)
		neg     eax
		sbb     edx, 0
		js      short L67
		cmp     dword ptr [is_unsigned], 0
		jne     short L67
		mov     ecx, dword ptr [errnoptr]
		mov     edx, 80000000H
		mov     dword ptr [ecx], ERANGE
		xor     eax, eax
		jmp     short L67

		align16
	L66:
		mov     edi, dword ptr [endptr]             // store beginning of string in endptr
		mov     esi, dword ptr [nptr]
	L67:
		test    edi, edi
		jz      short L69
	L68:
		mov     dword ptr [edi], esi                // store pointer to char that stopped the scan
	L69:
		pop     ebp                                 // restore register
		pop     edi
		pop     esi
		pop     ebx
		ret     24

		#undef inc_tchar_ptr
		#undef t
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
	#undef tchar_max
}
#endif
