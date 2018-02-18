#ifdef _tcstoul
#include <windows.h>
#include <errno.h>

#ifdef __BORLANDC__
#pragma warn -8058
#endif

#ifndef _M_IX86
unsigned long __cdecl _tcstoul(const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef _UNICODE
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
    if ((schar_t)(c -= 'A') < 0) {
        if ((schar_t)(c += 'A' - '0') < 0 || c > '9' - '0')
            goto NONUMBER;
    } else if (c > 'Z' - 'A' && ((schar_t)(c -= 'a' - 'A') < 0 || c > 'z' - 'a'))
        goto NONUMBER;
    else
        c += 0x0A;

    number = 0;                     // start with zero

    // if our number exceeds this, we will overflow on multiply
    maxval = ULONG_MAX / base;
    maxrem = (uchar_t)(ULONG_MAX % base);

    for (; ; )                      // exit in middle of loop
    {
        number = number * base + c; // we won't overflow, go ahead and multiply

        c = *(++p);                 // read next digit

        // convert c to value
        if ((schar_t)(c -= 'A') < 0) {
            if ((schar_t)(c += 'A' - '0') < 0 || c > '9' - '0')
                break;
        } else if (c > 'Z' - 'A' && ((schar_t)(c -= 'a' - 'A') < 0 || c > 'z' - 'a'))
            break;
        else
            c += 0x0A;

        // exit loop if bad digit found
        if (c >= (uchar_t)base)
            break;

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
        do
        {
            c = *(++p);             // read next digit
            if ((schar_t)(c -= 'A') < 0) {
                if ((schar_t)(c += 'A' - '0') < 0 || c > '9' - '0')
                    break;
            } else if (c > 'Z' - 'A' && ((schar_t)(c -= 'a' - 'A') < 0 || c > 'z' - 'a'))
                break;
            else
                c += 0x0A;
        } while (c < (uchar_t)base);

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
	#define far
#ifdef _UNICODE
	#define tchar        word
	#define t(r)         r##x
	#define sizeof_tchar 2
	#define inc_tchar(r) add r, 2
#else
	#define tchar        byte
	#define t(r)         r##l
	#define sizeof_tchar 1
	#define inc_tchar(r) inc r
#endif

	__asm
	{
		push    ebx                                     // store register
		push    esi
		push    edi

		#define nptr   (esp + 4 * (3 + 1))
		#define endptr (esp + 4 * (3 + 2))
		#define base   (esp + 4 * (3 + 3))
		#define sign   (esp + 4 * (3 + 3))

		mov     esi, dword ptr [nptr]                   // esi is our scanning pointer
		xor     eax, eax                                // start with zero
		mov     t(c), tchar ptr [esi]                   // read char
		mov     ebx, dword ptr [base]
		jmp     short L2

		align16
	L1:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // skip whitespace
		inc_tchar(esi)
	L2:
		cmp     t(c), ' '
		je      short L1
		cmp     t(c), 0DH
		ja      short L3
		cmp     t(c), 09H
		jae     short L1
		jmp     far L120

	L3:
		mov     tchar ptr [sign], t(c)                  // store sign char
		and     ecx, 0FFH
		cmp     t(c), '-'                               // skip sign
		je      short L4
		cmp     t(c), '+'
		jne     short L5
	L4:
		mov     t(c), tchar ptr [esi + sizeof_tchar]
		inc_tchar(esi)

	L5:
		cmp     ebx, 1
		jae     short L6
		cmp     t(c), '0'                               // determine base free-lance, based on first two chars of string
		jne     short L10
		mov     t(c), tchar ptr [esi + sizeof_tchar]
		inc_tchar(esi)
		cmp     t(c), 'x'
		je      far L31
		cmp     t(c), 'X'
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
		call    _errno                                  // bad base!
		mov     dword ptr [eax], EINVAL
		xor     eax, eax
		jmp     far L120

		align16
	L10:
		sub     t(c), '0'                               // base == 10
		jl      short L11
		cmp     t(c), '9' - '0'
		jbe     short L12
	L11:
		jmp     far L120                                // no number there; return 0 and point to beginning of string

		align16
	L12:
		inc_tchar(esi)
		lea     eax, [eax + eax * 4]
		lea     eax, [ecx + eax * 2]
		mov     t(c), tchar ptr [esi]                   // read next char
		sub     t(c), '0'                               // check and convert char to value
		jl      short L21
		cmp     t(c), '9' - '0'
		ja      short L21
		cmp     eax, 19999999H
		jb      short L12
		jne     short L20
		cmp     t(c), 5
		jbe     short L12

	L20:
		call    _errno                                  // overflow there
		mov     ebx, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    ebx, ebx
		mov     eax, 0FFFFFFFFH
		jnz     short L22
		jmp     far L124
	L21:
		jmp     far L122

		align16
	L22:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // point to end of string
		inc_tchar(esi)
		cmp     t(c), '0'
		jl      short L23
		cmp     t(c), '9'
		jbe     short L22
	L23:
		jmp     far L121

		align16
	L30:
		cmp     t(c), '0'                               // base == 16
		jne     short L32
		mov     t(c), tchar ptr [esi + sizeof_tchar]
		inc_tchar(esi)
		cmp     t(c), 'x'
		je      short L31
		cmp     t(c), 'X'
		jne     short L37
	L31:
		mov     t(c), tchar ptr [esi + sizeof_tchar]
		inc_tchar(esi)
	L32:
		sub     t(c), 'A'
		jge     short L34
		add     t(c), 'A' - '0'
		jnc     short L33
		cmp     t(c), '9' - '0'
		jbe     short L36
	L33:
		jmp     far L120                                // no number there; return 0 and point to beginning of string
	L34:
		cmp     t(c), 'F' - 'A'
		jbe     short L35
		sub     t(c), 'a' - 'A'
		jb      short L33
		cmp     t(c), 'f' - 'a'
		ja      short L33
	L35:
		add     t(c), 10

		align16
	L36:
		shl     eax, 4
		inc_tchar(esi)
		or      eax, ecx
		mov     t(c), tchar ptr [esi]                   // read next char
	L37:
		sub     t(c), 'A'                               // check and convert char to value
		jge     short L39
		add     t(c), 'A' - '0'
		jnc     short L38
		cmp     t(c), '9' - '0'
		jbe     short L41
	L38:
		jmp     far L122
	L39:
		cmp     t(c), 'F' - 'A'
		jbe     short L40
		sub     t(c), 'a' - 'A'
		jb      short L38
		cmp     t(c), 'f' - 'a'
		ja      short L38
	L40:
		add     t(c), 10
	L41:
		test    eax, 0F0000000H
		jz      short L36

	L60:
		call    _errno                                  // overflow there
		mov     ebx, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    ebx, ebx
		mov     eax, 0FFFFFFFFH
		jnz     short L61
		jmp     far L124

		align16
	L61:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // point to end of string
		inc_tchar(esi)
		sub     t(c), 'A'
		jge     short L63
		add     t(c), 'A' - '0'
		jnc     short L62
		cmp     t(c), '9' - '0'
		jbe     short L61
	L62:
		jmp     far L121
	L63:
		cmp     t(c), 'F' - 'A'
		jbe     short L61
		sub     t(c), 'a' - 'A'
		jb      short L64
		cmp     t(c), 'f' - 'a'
		jbe     short L61
	L64:
		jmp     far L121

		align16
	L70:
		sub     t(c), '0'                               // base == 8
		jl      short L71
		cmp     t(c), '7' - '0'
		jbe     short L72
	L71:
		jmp     far L120                                // no number there; return 0 and point to beginning of string

		align16
	L72:
		shl     eax, 3
		inc_tchar(esi)
		or      eax, ecx
		mov     t(c), tchar ptr [esi]                   // read next char
	L73:
		sub     t(c), '0'                               // check and convert char to value
		jl      short L81
		cmp     t(c), '7' - '0'
		ja      short L81
		test    eax, 0E0000000H
		jz      short L72

	L80:
		call    _errno                                  // overflow there
		mov     ebx, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    ebx, ebx
		mov     eax, 0FFFFFFFFH
		jnz     short L82
		jmp     far L124
	L81:
		jmp     far L122

		align16
	L82:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // point to end of string
		inc_tchar(esi)
		cmp     t(c), '0'
		jl      short L83
		cmp     t(c), '7'
		jbe     short L82
	L83:
		jmp     far L121

		align16
	L90:
		sub     t(c), 'A'                               // base > 1 && base <= 36 && base != 10 && base != 16 && base != 8
		jge     short L92
		add     t(c), 'A' - '0'
		jnc     short L91
		cmp     t(c), '9' - '0'
		jbe     short L94
	L91:
		jmp     far L120                                // no number there; return 0 and point to beginning of string
	L92:
		cmp     t(c), 'Z' - 'A'
		jbe     short L93
		sub     t(c), 'a' - 'A'
		jb      short L91
		cmp     t(c), 'z' - 'a'
		ja      short L91
	L93:
		add     t(c), 10
	L94:
		cmp     t(c), t(b)
		jae     short L91

		align16
#ifndef _UNICODE
		#define L104 L122
#endif
	L95:
		mul     ebx
		jc      short L110
		add     eax, ecx
		jc      short L110
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // read next char
		inc_tchar(esi)
		sub     t(c), 'A'                               // check and convert char to value
		jge     short L97
		add     t(c), 'A' - '0'
		jnc     short L104
		cmp     t(c), '9' - '0'
		jbe     short L99
		jmp     short L104
	L97:
		cmp     t(c), 'F' - 'A'
		jbe     short L98
		sub     t(c), 'a' - 'A'
		jb      short L104
		cmp     t(c), 'f' - 'a'
		ja      short L104
	L98:
		add     t(c), 10
	L99:
		cmp     t(c), t(b)
		jb      short L95
#ifndef _UNICODE
		#undef L104
#else
	L104:
#endif
		jmp     short L122

	L110:
		call    _errno                                  // overflow there
		mov     ebx, dword ptr [endptr]
		mov     dword ptr [eax], ERANGE
		test    ebx, ebx
		mov     eax, 0FFFFFFFFH
		jz      short L124

		align16
	L111:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // point to end of string
		inc_tchar(esi)
		sub     t(c), 'A'
		jge     short L112
		add     t(c), 'A' - '0'
		jnc     short L121
		cmp     t(c), '9' - '0'
		jbe     short L114
		jmp     short L121
	L112:
		cmp     t(c), 'Z' - 'A'
		jbe     short L113
		sub     t(c), 'a' - 'A'
		jb      short L121
		cmp     t(c), 'z' - 'a'
		ja      short L121
	L113:
		add     t(c), 10
	L114:
		cmp     t(c), t(b)
		jb      short L111
		jmp     short L121

		align16
	L120:
		mov     ebx, dword ptr [endptr]                 // store beginning of string in endptr
		mov     esi, dword ptr [nptr]                   // return 0
		test    ebx, ebx
		jz      short L124
	L121:
		mov     dword ptr [ebx], esi
		jmp     short L124
	L122:
		mov     ecx, dword ptr [endptr]
		test    ecx, ecx
		jz      short L123
		mov     dword ptr [ecx], esi                    // store pointer to char that stopped the scan
	L123:
		cmp     tchar ptr [sign], '-'
		jne     short L124
		neg     eax                                     // negate result if there was a neg sign
	L124:
		pop     edi                                     // restore register
		pop     esi
		pop     ebx
		ret

		#undef nptr
		#undef endptr
		#undef base
		#undef sign
	}
	#undef far
	#undef tchar
	#undef t
	#undef sizeof_tchar
	#undef inc_tchar
}
#endif
#endif
