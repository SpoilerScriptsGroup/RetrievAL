#ifdef _tcstoui64
#include <windows.h>
#include <errno.h>

#ifdef __BORLANDC__
#pragma warn -8058
#endif

#ifndef _M_IX86
unsigned __int64 __cdecl _tcstoui64(const TCHAR *nptr, TCHAR **endptr, int base)
{
#if defined(_UNICODE)
    typedef wchar_t       uchar_t;
    typedef short         schar_t;
#else
    typedef unsigned char uchar_t;
    typedef char          schar_t;
#endif

    const uchar_t    *p;
    uchar_t          c;
    uchar_t          sign;
    unsigned __int64 number;
    unsigned __int64 maxval;
    uchar_t          maxrem;

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
    if ((schar_t)(c -= 'A') < 0 ?
        (schar_t)(c += 'A' - '0') < 0 || c >= (uchar_t)base :
        (c += 0x0A) >= (uchar_t)base && ((schar_t)(c -= 'a' - 'A' + 0x0A) < 0 || (c += 0x0A) >= (uchar_t)base))
        goto NONUMBER;

    number = 0;                     // start with zero

    // if our number exceeds this, we will overflow on multiply
    maxval = _UI64_MAX / base;
    maxrem = (uchar_t)(_UI64_MAX % base);

    for (; ; )                      // exit in middle of loop
    {
        number = number * base + c; // we won't overflow, go ahead and multiply

        c = *(++p);                 // read next digit

        // convert c to value
        if ((schar_t)(c -= 'A') < 0 ?
            (schar_t)(c += 'A' - '0') < 0 || c >= (uchar_t)base :
            (c += 0x0A) >= (uchar_t)base && ((schar_t)(c -= 'a' - 'A' + 0x0A) < 0 || (c += 0x0A) >= (uchar_t)base))
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
        number = -(__int64)number;  // negate result if there was a neg sign

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
        while ((schar_t)(c = *(++p) - 'A') < 0 ?
            (schar_t)(c += 'A' - '0') >= 0 && c < (uchar_t)base :
            (c += 0x0A) < (uchar_t)base || (schar_t)(c -= 'a' - 'A' + 0x0A) >= 0 && (c += 0x0A) < (uchar_t)base);
        *endptr = (TCHAR *)p;       // store pointer to char that stopped the scan
    }
    errno = ERANGE;
    return _UI64_MAX;
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

__declspec(naked) unsigned __int64 __cdecl _tcstoui64(const TCHAR *nptr, TCHAR **endptr, int base)
{
	#define far
#if defined(_UNICODE)
	#define tchar_ptr    word ptr
	#define sizeof_tchar 2
#else
	#define tchar_ptr    byte ptr
	#define sizeof_tchar 1
#endif

	__asm
	{
#if defined(_UNICODE)
		#define inc_tchar add esi, 2
		#define t         cx
		#define r_base    bx
		#define T98       L98
#else
		#define inc_tchar inc esi
		#define t         cl
		#define r_base    bh
		#define T98       L110
#endif

		push    ebx                                 // store register
		push    esi
		push    edi
		push    ebp

		#define nptr   (esp + 4 * (4 + 1))
		#define endptr (esp + 4 * (4 + 2))
		#define base   (esp + 4 * (4 + 3))
		#define sign   (esp + 4 * (4 + 3))

		mov     esi, dword ptr [nptr]               // esi is our scanning pointer
		xor     eax, eax                            // start with zero
		xor     edx, edx
		mov     t, tchar_ptr [esi]                  // read char
		mov     ebx, dword ptr [base]
		jmp     short L2

		align16
	L1:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // skip whitespace
		inc_tchar
	L2:
		cmp     t, ' '
		je      short L1
		cmp     t, 0DH
		ja      short L3
		cmp     t, 09H
		jae     short L1
		jmp     far L111

	L3:
		mov     tchar_ptr [sign], t                 // store sign char
		and     ecx, 0FFH
		cmp     t, '-'                              // skip sign
		je      short L4
		cmp     t, '+'
		jne     short L5
	L4:
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar

	L5:
		cmp     ebx, 1
		jae     short L6
		cmp     t, '0'                              // determine base free-lance, based on first two chars of string
		jne     short L10
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar
		cmp     t, 'x'
		je      far L31
		cmp     t, 'X'
		je      far L31
		jmp     far L73

	L6:
		je      short L7
		cmp     ebx, 10
		je      short L10
		cmp     ebx, 16
		je      far L30
		cmp     ebx, 8
		je      far L70
		cmp     ebx, 10 + 'Z' - 'A' + 1
		jbe     far L90
	L7:
		call    _errno                              // bad base!
		mov     dword ptr [eax], EINVAL
		xor     eax, eax
		xor     edx, edx
		jmp     far L111

		align16
	L10:
		sub     t, '0'                              // base == 10
		jl      short L11
		cmp     t, '9' - '0'
		jbe     short L12
	L11:
		jmp     far L111                            // no number there; return 0 and point to beginning of string

		align16
	L12:
		inc_tchar
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

		align16
	L13:
		mov     ebx, eax
		mov     edi, edx
		inc_tchar
		add     eax, eax
		adc     edx, edx
		add     eax, eax
		adc     edx, edx
		add     eax, ebx
		adc     edx, edi
		add     eax, eax
		adc     edx, edx
		add     eax, ecx
		adc     edx, 0
		mov     t, tchar_ptr [esi]                  // read next char
		sub     t, '0'                              // check and convert char to value
		jl      short L14
		cmp     t, '9' - '0'
		ja      short L14
		cmp     edx, 19999999H
		jb      short L13
		jne     short L20
		cmp     eax, 99999999H
		jb      short L13
		jne     short L20
		cmp     t, 5
		jbe     short L13
		jmp     short L20
	L14:
		jmp     far L110

	L20:
		call    _errno                              // overflow there
		mov     dword ptr [eax], ERANGE
		mov     edi, dword ptr [endptr]
		mov     eax, 0FFFFFFFFH
		test    edi, edi
		mov     edx, eax
		jnz     short L22
		jmp     far L114

		align16
	L22:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // point to end of string
		inc_tchar
		cmp     t, '0'
		jl      short L23
		cmp     t, '9'
		jbe     short L22
	L23:
		jmp     far L113

		align16
	L30:
		cmp     t, '0'                              // base == 16
		jne     short L32
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar
		cmp     t, 'x'
		je      short L31
		cmp     t, 'X'
		jne     short L37
	L31:
		mov     t, tchar_ptr [esi + sizeof_tchar]
		inc_tchar
	L32:
		sub     t, 'A'
		jge     short L34
		add     t, 'A' - '0'
		jnc     short L33
		cmp     t, '9' - '0'
		jbe     short L36
	L33:
		jmp     far L111                            // no number there; return 0 and point to beginning of string
	L34:
		cmp     t, 'F' - 'A'
		jbe     short L35
		sub     t, 'a' - 'A'
		jb      short L33
		cmp     t, 'f' - 'a'
		ja      short L33
	L35:
		add     t, 10

		align16
	L36:
		shl     eax, 4
		inc_tchar
		or      eax, ecx
		mov     t, tchar_ptr [esi]                  // read next char
	L37:
		sub     t, 'A'                              // check and convert char to value
		jge     short L39
		add     t, 'A' - '0'
		jnc     short L38
		cmp     t, '9' - '0'
		jbe     short L41
	L38:
		jmp     far L110
	L39:
		cmp     t, 'F' - 'A'
		jbe     short L40
		sub     t, 'a' - 'A'
		jb      short L38
		cmp     t, 'f' - 'a'
		ja      short L38
	L40:
		add     t, 10
	L41:
		test    eax, 0F0000000H
		jz      short L36

		align16
	L50:
		shld    edx, eax, 4
		shl     eax, 4
		inc_tchar
		or      eax, ecx
		mov     t, tchar_ptr [esi]                  // read next char
		sub     t, 'A'                              // check and convert char to value
		jge     short L52
		add     t, 'A' - '0'
		jnc     short L51
		cmp     t, '9' - '0'
		jbe     short L54
	L51:
		jmp     far L110
	L52:
		cmp     t, 'F' - 'A'
		jbe     short L53
		sub     t, 'a' - 'A'
		jb      short L51
		cmp     t, 'f' - 'a'
		ja      short L51
	L53:
		add     t, 10
	L54:
		test    edx, 0F0000000H
		jz      short L50

	L60:
		call    _errno                              // overflow there
		mov     dword ptr [eax], ERANGE
		mov     edi, dword ptr [endptr]
		mov     eax, 0FFFFFFFFH
		test    edi, edi
		mov     edx, eax
		jnz     short L61
		jmp     far L114

		align16
	L61:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // point to end of string
		inc_tchar
		sub     t, 'A'
		jge     short L63
		add     t, 'A' - '0'
		jnc     short L62
		cmp     t, '9' - '0'
		jbe     short L61
	L62:
		jmp     far L113
	L63:
		cmp     t, 'F' - 'A'
		jbe     short L61
		sub     t, 'a' - 'A'
		jb      short L64
		cmp     t, 'f' - 'a'
		jbe     short L61
	L64:
		jmp     far L113

		align16
	L70:
		sub     t, '0'                              // base == 8
		jl      short L71
		cmp     t, '7' - '0'
		jbe     short L72
	L71:
		jmp     far L111                            // no number there; return 0 and point to beginning of string

		align16
	L72:
		shl     eax, 3
		inc_tchar
		or      eax, ecx
		mov     t, tchar_ptr [esi]                  // read next char
	L73:
		sub     t, '0'                              // check and convert char to value
		jl      short L81
		cmp     t, '7' - '0'
		ja      short L81
		test    eax, 0E0000000H
		jz      short L72

		align16
	L74:
		inc_tchar
		add     eax, eax
		adc     edx, edx
		add     eax, eax
		adc     edx, edx
		add     eax, eax
		adc     edx, edx
		or      eax, ecx
		mov     t, tchar_ptr [esi]                  // read next char
		sub     t, '0'                              // check and convert char to value
		jl      short L81
		cmp     t, '7' - '0'
		ja      short L81
		test    edx, 0E0000000H
		jz      short L74

	L80:
		call    _errno                              // overflow there
		mov     dword ptr [eax], ERANGE
		mov     edi, dword ptr [endptr]
		mov     eax, 0FFFFFFFFH
		test    edi, edi
		mov     edx, eax
		jnz     short L82
		jmp     far L114
	L81:
		jmp     far L110

		align16
	L82:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // point to end of string
		inc_tchar
		cmp     t, '0'
		jl      short L83
		cmp     t, '7'
		jbe     short L82
	L83:
		jmp     far L113

		align16
	L90:
		sub     t, 'A'                              // base > 1 && base <= 36 && base != 10 && base != 16 && base != 8
		jge     short L92
		add     t, 'A' - '0'
		jnc     short L91
		cmp     t, r_base
		jb      short L93
	L91:
		jmp     far L111                            // no number there; return 0 and point to beginning of string
	L92:
		add     t, 10
		cmp     t, r_base
		jb      short L93
		sub     t, 'a' - 'A'
		add     t, 10
		jb      short L91
		cmp     t, r_base
		jae     short L91

		align16
	L93:
		mul     ebx
		inc_tchar
		add     eax, ecx
		adc     edx, 0
		mov     t, tchar_ptr [esi]                  // read next char
		sub     t, 'A'                              // check and convert char to value
		jge     short L95
		add     t, 'A' - '0'
		jnc     short L94
		cmp     t, r_base
		jb      short L96
	L94:
		jmp     far L110
	L95:
		add     t, 10
		cmp     t, r_base
		jb      short L96
		sub     t, 'a' - 'A' + 10
		jb      short L94
		add     t, 10
		cmp     t, r_base
		jae     short L94
	L96:
		test    edx, edx
		jz      short L93

		align16
	L97:
		mov     edi, eax
		mov     eax, edx
		mul     ebx
		jc      short L100
		mov     ebp, eax
		mov     eax, edi
		mul     ebx
		add     eax, ecx
		adc     edx, ebp
		jc      short L100
		mov     t, tchar_ptr [esi + sizeof_tchar]   // read next char
		inc_tchar
		sub     t, 'A'                              // check and convert char to value
		jge     short L99
		add     t, 'A' - '0'
		jnc     short T98
		cmp     t, r_base
		jb      short L97
	L98:
		jmp     L110
	L99:
		add     t, 10
		cmp     t, r_base
		jb      short L97
		sub     t, 'a' - 'A' + 10
		jb      short L110
		add     t, 10
		cmp     t, r_base
		jb      short L97
		jmp     short L110

	L100:
		call    _errno                              // overflow there
		mov     dword ptr [eax], ERANGE
		mov     edi, dword ptr [endptr]
		mov     eax, 0FFFFFFFFH
		test    edi, edi
		mov     edx, eax
		jz      short L114

		align16
	L101:
		mov     t, tchar_ptr [esi + sizeof_tchar]   // point to end of string
		inc_tchar
		sub     t, 'A'
		jge     short L102
		add     t, 'A' - '0'
		jnc     short L113
		cmp     t, r_base
		jb      short L101
		jmp     short L113
	L102:
		add     t, 10
		cmp     t, r_base
		jb      short L101
		sub     t, 'a' - 'A' + 10
		jb      short L113
		add     t, 10
		cmp     t, r_base
		jb      short L101
		jmp     short L113

		align16
	L110:
		mov     t, tchar_ptr [sign]
		mov     edi, dword ptr [endptr]
		cmp     t, '-'
		jne     short L112
		neg     edx                                 // negate result if there was a neg sign
		neg     eax
		sbb     edx, 0
		jmp     short L112
	L111:
		mov     edi, dword ptr [endptr]             // store beginning of string in endptr
		mov     esi, dword ptr [nptr]
	L112:
		test    edi, edi
		jz      short L114
	L113:
		mov     dword ptr [edi], esi                // store pointer to char that stopped the scan
	L114:
		pop     ebp                                 // restore register
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef inc_tchar
		#undef t
		#undef r_base
		#undef T98
		#undef nptr
		#undef endptr
		#undef base
		#undef sign
	}
	#undef far
	#undef tchar_ptr
	#undef sizeof_tchar
}
#endif
#endif
