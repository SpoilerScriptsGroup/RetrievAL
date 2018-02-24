#ifdef _tcstoul
#include <windows.h>
#include <errno.h>
#include "atoitbl.h"

#ifdef __BORLANDC__
#pragma warn -8058
#endif

#ifndef _M_IX86
unsigned long __cdecl _tcstoul(const TCHAR *nptr, TCHAR **endptr, int base)
{
#if defined(_UNICODE)
    typedef wchar_t       uchar_t;
    typedef short         schar_t;
#else
    typedef unsigned char uchar_t;
    typedef char          schar_t;
#endif

    const uchar_t *p;
    uchar_t       c;
    uchar_t       sign;
    unsigned long number;
    unsigned long maxval;
    uchar_t       maxrem;

    p = nptr;                       // p is our scanning pointer

    c = *p;                         // read char
    while (c == ' ' || (c <= '\r' && c >= '\t'))
        c = *(++p);                 // skip whitespace

    sign = c;                       // remember sign char
    if (c == '-' || c == '+')
        c = *(++p);                 // skip sign

    do {
        if (base == 1 || (unsigned int)base > 11 + 'Z' - 'A')
            goto INVALID;           // bad base!
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
#if defined(_UNICODE)
    if (c > (uchar_t)'z' || (c = atoitbl[c]) >= (uchar_t)base)
#else
    if ((c = atoitbl[c]) >= (uchar_t)base)
#endif
        goto NONUMBER;

    number = 0;                     // start with zero

    // if our number exceeds this, we will overflow on multiply
    maxval = ULONG_MAX / base;
    maxrem = (uchar_t)(ULONG_MAX % base);

    for (; ; )                      // exit in middle of loop
    {
        number = number * base + c; // we won't overflow, go ahead and multiply

        c = *(++p);                 // read next digit

        // convert c to value
#if defined(_UNICODE)
        if (c > (uchar_t)'z' || (c = atoitbl[c]) >= (uchar_t)base)
#else
        if ((c = atoitbl[c]) >= (uchar_t)base)
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

    if (endptr)
        *endptr = (TCHAR *)p;       // store pointer to char that stopped the scan

    if (sign == '-')
        number = -(long)number;     // negate result if there was a neg sign

    return number;                  // done.

INVALID:
    errno = EINVAL;

NONUMBER:
    // no number there
    if (endptr)
        *endptr = (TCHAR *)nptr;    // store beginning of string in endptr later on
    return 0;                       // return 0

OVERFLOW:
    // overflow occurred
    if (endptr)
    {
        // point to end of string
#if defined(_UNICODE)
        while ((c = *(++p)) <= (uchar_t)'z' && atoitbl[c] < (unsigned char)base);
#else
        while (atoitbl[*(++p)] < (unsigned char)base);
#endif
        *endptr = (TCHAR *)p;       // store pointer to char that stopped the scan
    }
    errno = ERANGE;
    return ULONG_MAX;
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

__declspec(naked) unsigned long __cdecl _tcstoul(const TCHAR *nptr, TCHAR **endptr, int base)
{
#if defined(_UNICODE)
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
#if defined(_UNICODE)
		#define inc_tchar_ptr add esi, 2
		#define t             cx
#else
		#define inc_tchar_ptr inc esi
		#define t             cl
#endif

		push    ebx                                 // store register
		push    esi
		push    edi

		#define nptr   (esp + 4 * (3 + 1))
		#define endptr (esp + 4 * (3 + 2))
		#define base   (esp + 4 * (3 + 3))
		#define sign   (esp + 4 * (3 + 3))

		mov     esi, dword ptr [nptr]               // esi is our scanning pointer
		xor     eax, eax                            // start with zero
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
		jmp     L81

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
		jae     short L6
		cmp     t, '0'                              // determine base free-lance, based on first two chars of string
		jne     short L10
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
		cmp     t, 'x'
		je      L31
		cmp     t, 'X'
		je      L31
		jmp     L43

	L6:
		je      short L7
		cmp     ebx, 10
		je      short L10
		cmp     ebx, 16
		je      L30
		cmp     ebx, 8
		je      L40
		cmp     ebx, 10 + 'Z' - 'A' + 1
		jbe     L60
	L7:
		call    _errno                              // bad base!
		mov     dword ptr [eax], EINVAL
		xor     eax, eax
		jmp     L81

		align16
	L10:
		sub     t, '0'                              // base == 10
		jl      short L11
		cmp     t, '9' - '0'
		jbe     short L12
	L11:
		jmp     L81                                 // no number there; return 0 and point to beginning of string

		align16
	L12:
		inc_tchar_ptr
		lea     eax, [eax + eax * 4]
		lea     eax, [ecx + eax * 2]
		mov     t, tchar_ptr [esi]                  // read next char
		sub     t, '0'                              // check and convert char to value
		jl      short L13
		cmp     t, '9' - '0'
		ja      short L13
		cmp     eax, 19999999H
		jb      short L12
		jne     short L20
		cmp     t, 5
		jbe     short L12
		jmp     short L20
	L13:
		jmp     L80

		align16
	L20:
		call    _errno                              // overflow there
		mov     edi, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    edi, edi
		mov     eax, 0FFFFFFFFH
		jnz     short L22
		jmp     L84

		align16
	L22:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // point to end of string
		inc_tchar_ptr
		cmp     t, '0'
		jl      short L23
		cmp     t, '9'
		jbe     short L22
	L23:
		jmp     L83

		align16
	L30:
		cmp     t, '0'                              // base == 16
		jne     short L32
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
		cmp     t, 'x'
		je      short L31
		cmp     t, 'X'
		jne     short L35
	L31:
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar_ptr
	L32:
#if defined(_UNICODE)
		cmp     t, 'f'
		ja      short L33
#endif
		mov     cl, byte ptr atoitbl [ecx]
		cmp     cl, 16
		jb      short L34
	L33:
		jmp     L81                                 // no number there; return 0 and point to beginning of string

		align16
	L34:
		shl     eax, 4
		inc_tchar_ptr
		or      eax, ecx
		mov     t, tchar_ptr [esi]                  // read next char
	L35:
#if defined(_UNICODE)
		cmp     t, 'f'
		ja      short L36
#endif
		mov     cl, byte ptr atoitbl [ecx]          // check and convert char to value
		cmp     cl, 16
		jae     short L36
		test    eax, 0F0000000H
		jz      short L34
		jmp     L70
	L36:
		jmp     L80

		align16
	L40:
		sub     t, '0'                              // base == 8
		jl      short L41
		cmp     t, '7' - '0'
		jbe     short L42
	L41:
		jmp     L81                                 // no number there; return 0 and point to beginning of string

		align16
	L42:
		shl     eax, 3
		inc_tchar_ptr
		or      eax, ecx
		mov     t, tchar_ptr [esi]                  // read next char
	L43:
		sub     t, '0'                              // check and convert char to value
		jl      short L44
		cmp     t, '7' - '0'
		ja      short L44
		test    eax, 0E0000000H
		jz      short L42
		jmp     short L50
	L44:
		jmp     L80

		align16
	L50:
		call    _errno                              // overflow there
		mov     edi, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    edi, edi
		mov     eax, 0FFFFFFFFH
		jnz     short L51
		jmp     L84

		align16
	L51:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // point to end of string
		inc_tchar_ptr
		cmp     t, '0'
		jl      short L52
		cmp     t, '7'
		jbe     short L51
	L52:
		jmp     L83

		align16
	L60:
#if defined(_UNICODE)
		cmp     t, 'z'
		ja      short L61
#endif
		mov     cl, byte ptr atoitbl [ecx]          // base > 1 && base <= 36 && base != 10 && base != 16 && base != 8
		cmp     cl, bl
		jb      short L62
	L61:
		jmp     L81                                 // no number there; return 0 and point to beginning of string

		align16
	L62:
		mul     ebx
		jc      short L70
		add     eax, ecx
		jc      short L70
		mov     t, tchar_ptr [esi + sizeof_tchar]   // read next char
		inc_tchar_ptr
#if defined(_UNICODE)
		cmp     t, 'z'
		ja      short L80
#endif
		mov     cl, byte ptr atoitbl [ecx]          // check and convert char to value
		cmp     cl, bl
		jb      short L62
		jmp     short L80

		align16
	L70:
		call    _errno                              // overflow there
		mov     edi, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    edi, edi
		mov     eax, 0FFFFFFFFH
		jz      short L84

		align16
	L71:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // point to end of string
		inc_tchar_ptr
#if defined(_UNICODE)
		cmp     t, 'z'
		ja      short L83
#endif
		mov     cl, byte ptr atoitbl [ecx]
		cmp     cl, bl
		jb      short L71
		jmp     short L83

		align16
	L80:
		mov     t, tchar_ptr [sign]
		mov     edi, dword ptr [endptr]
		cmp     t, '-'
		jne     short L82
		neg     eax                                 // negate result if there was a neg sign
		jmp     short L82
	L81:
		mov     edi, dword ptr [endptr]             // store beginning of string in endptr
		mov     esi, dword ptr [nptr]
	L82:
		test    edi, edi
		jz      short L84
	L83:
		mov     dword ptr [edi], esi                // store pointer to char that stopped the scan
	L84:
		pop     edi                                 // restore register
		pop     esi
		pop     ebx
		ret

		#undef inc_tchar_ptr
		#undef t
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
#endif
