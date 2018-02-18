#ifdef _tcstoui64
#include <windows.h>
#include <errno.h>

#ifdef __BORLANDC__
#pragma warn -8058
#endif

#ifndef _M_IX86
unsigned __int64 __cdecl _tcstoui64(const TCHAR *nptr, TCHAR **endptr, int base)
{
#ifdef _UNICODE
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
    if ((schar_t)(c -= 'A') < 0) {
        if ((schar_t)(c += 'A' - '0') < 0 || c > '9' - '0')
            goto NONUMBER;
    } else if (c > 'Z' - 'A' && ((schar_t)(c -= 'a' - 'A') < 0 || c > 'z' - 'a'))
        goto NONUMBER;
    else
        c += 0x0A;
    if (c >= (uchar_t)base)
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
		push    ebp

		#define nptr   (esp + 4 * (4 + 1))
		#define endptr (esp + 4 * (4 + 2))
		#define base   (esp + 4 * (4 + 3))
		#define sign   (esp + 4 * (4 + 3))

		mov     esi, dword ptr [nptr]                   // esi is our scanning pointer
		xor     eax, eax                                // start with zero
		xor     edx, edx
		mov     t(c), tchar ptr [esi]                   // read char
		mov     ebx, dword ptr [base]
		jmp     L2

		align16
	L1:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // skip whitespace
		inc_tchar(esi)
	L2:
		cmp     t(c), ' '
		je      L1
		cmp     t(c), 0DH
		ja      L3
		cmp     t(c), 09H
		jae     L1
		jmp     L120

	L3:
		mov     tchar ptr [sign], t(c)                  // store sign char
		and     ecx, 0FFH
		cmp     t(c), '-'                               // skip sign
		je      L4
		cmp     t(c), '+'
		jne     L5
	L4:
		mov     t(c), tchar ptr [esi + sizeof_tchar]
		inc_tchar(esi)

	L5:
		cmp     ebx, 1
		jae     L6
		cmp     t(c), '0'                               // determine base free-lance, based on first two chars of string
		jne     L10
		mov     t(c), tchar ptr [esi + sizeof_tchar]
		inc_tchar(esi)
		cmp     t(c), 'x'
		je      L31
		cmp     t(c), 'X'
		je      L31
		jmp     L73

	L6:
		je      L7
		cmp     ebx, 10
		je      L10
		cmp     ebx, 16
		je      L30
		cmp     ebx, 8
		je      L70
		cmp     ebx, 10 + 'Z' - 'A' + 1
		jbe     L90
	L7:
		call    _errno                                  // bad base!
		mov     dword ptr [eax], EINVAL
		xor     eax, eax
		xor     edx, edx
		jmp     L120

		align16
	L10:
		sub     t(c), '0'                               // base == 10
		jl      L11
		cmp     t(c), '9' - '0'
		jbe     L12
	L11:
		jmp     L120                                    // no number there; return 0 and point to beginning of string

		align16
	L12:
		inc_tchar(esi)
		lea     eax, [eax + eax * 4]
		lea     eax, [ecx + eax * 2]
		mov     t(c), tchar ptr [esi]                   // read next char
		sub     t(c), '0'                               // check and convert char to value
		jl      L21
		cmp     t(c), '9' - '0'
		ja      L21
		cmp     eax, 19999999H
		jb      L12
		jne     L13
		cmp     t(c), 5
		jbe     L12

		align16
	L13:
		mov     ebx, eax
		mov     edi, edx
		inc_tchar(esi)
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
		mov     t(c), tchar ptr [esi]                   // read next char
		sub     t(c), '0'                               // check and convert char to value
		jl      L21
		cmp     t(c), '9' - '0'
		ja      L21
		cmp     edx, 19999999H
		jb      L13
		jne     L20
		cmp     eax, 99999999H
		jb      L13
		jne     L20
		cmp     t(c), 5
		jbe     L13

	L20:
		call    _errno                                  // overflow there
		mov     dword ptr [eax], ERANGE
		mov     ebx, dword ptr [endptr]
		mov     eax, 0FFFFFFFFH
		test    ebx, ebx
		mov     edx, eax
		jnz     L22
		jmp     L124
	L21:
		jmp     L122

		align16
	L22:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // point to end of string
		inc_tchar(esi)
		cmp     t(c), '0'
		jl      L23
		cmp     t(c), '9'
		jbe     L22
	L23:
		jmp     L121

		align16
	L30:
		cmp     t(c), '0'                               // base == 16
		jne     L32
		mov     t(c), tchar ptr [esi + sizeof_tchar]
		inc_tchar(esi)
		cmp     t(c), 'x'
		je      L31
		cmp     t(c), 'X'
		jne     L37
	L31:
		mov     t(c), tchar ptr [esi + sizeof_tchar]
		inc_tchar(esi)
	L32:
		sub     t(c), 'A'
		jge     L34
		add     t(c), 'A' - '0'
		jnc     L33
		cmp     t(c), '9' - '0'
		jbe     L36
	L33:
		jmp     L120                                    // no number there; return 0 and point to beginning of string
	L34:
		cmp     t(c), 'F' - 'A'
		jbe     L35
		sub     t(c), 'a' - 'A'
		jb      L33
		cmp     t(c), 'f' - 'a'
		ja      L33
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
		jge     L39
		add     t(c), 'A' - '0'
		jnc     L38
		cmp     t(c), '9' - '0'
		jbe     L41
	L38:
		jmp     L122
	L39:
		cmp     t(c), 'F' - 'A'
		jbe     L40
		sub     t(c), 'a' - 'A'
		jb      L38
		cmp     t(c), 'f' - 'a'
		ja      L38
	L40:
		add     t(c), 10
	L41:
		test    eax, 0F0000000H
		jz      L36

		align16
	L50:
		shld    edx, eax, 4
		shl     eax, 4
		inc_tchar(esi)
		or      eax, ecx
		mov     t(c), tchar ptr [esi]                   // read next char
		sub     t(c), 'A'                               // check and convert char to value
		jge     L52
		add     t(c), 'A' - '0'
		jnc     L51
		cmp     t(c), '9' - '0'
		jbe     L54
	L51:
		jmp     L122
	L52:
		cmp     t(c), 'F' - 'A'
		jbe     L53
		sub     t(c), 'a' - 'A'
		jb      L51
		cmp     t(c), 'f' - 'a'
		ja      L51
	L53:
		add     t(c), 10
	L54:
		test    edx, 0F0000000H
		jz      L50

	L60:
		call    _errno                                  // overflow there
		mov     dword ptr [eax], ERANGE
		mov     ebx, dword ptr [endptr]
		mov     eax, 0FFFFFFFFH
		test    ebx, ebx
		mov     edx, eax
		jnz     L61
		jmp     L124

		align16
	L61:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // point to end of string
		inc_tchar(esi)
		sub     t(c), 'A'
		jge     L63
		add     t(c), 'A' - '0'
		jnc     L62
		cmp     t(c), '9' - '0'
		jbe     L61
	L62:
		jmp     L121
	L63:
		cmp     t(c), 'F' - 'A'
		jbe     L61
		sub     t(c), 'a' - 'A'
		jb      L64
		cmp     t(c), 'f' - 'a'
		jbe     L61
	L64:
		jmp     L121

		align16
	L70:
		sub     t(c), '0'                               // base == 8
		jl      L71
		cmp     t(c), '7' - '0'
		jbe     L72
	L71:
		jmp     L120                                    // no number there; return 0 and point to beginning of string

		align16
	L72:
		shl     eax, 3
		inc_tchar(esi)
		or      eax, ecx
		mov     t(c), tchar ptr [esi]                   // read next char
	L73:
		sub     t(c), '0'                               // check and convert char to value
		jl      L83
		cmp     t(c), '7' - '0'
		ja      L83
		test    eax, 0E0000000H
		jz      L72

		align16
	L74:
		inc_tchar(esi)
		add     eax, eax
		adc     edx, edx
		add     eax, eax
		adc     edx, edx
		add     eax, eax
		adc     edx, edx
		or      eax, ecx
		mov     t(c), tchar ptr [esi]                   // read next char
		sub     t(c), '0'                               // check and convert char to value
		jl      L83
		cmp     t(c), '7' - '0'
		ja      L83
		test    edx, 0E0000000H
		jz      L74

	L80:
		call    _errno                                  // overflow there
		mov     dword ptr [eax], ERANGE
		mov     ebx, dword ptr [endptr]
		mov     eax, 0FFFFFFFFH
		test    ebx, ebx
		mov     edx, eax
		jnz     L81
		jmp     L124

		align16
	L81:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // point to end of string
		inc_tchar(esi)
		cmp     t(c), '0'
		jl      L82
		cmp     t(c), '7'
		jbe     L81
	L82:
		jmp     L121
	L83:
		jmp     L122

		align16
	L90:
		sub     t(c), 'A'                               // base > 1 && base <= 36 && base != 10 && base != 16 && base != 8
		jge     L92
		add     t(c), 'A' - '0'
		jnc     L91
		cmp     t(c), '9' - '0'
		jbe     L94
	L91:
		jmp     L120                                    // no number there; return 0 and point to beginning of string
	L92:
		cmp     t(c), 'Z' - 'A'
		jbe     L93
		sub     t(c), 'a' - 'A'
		jb      L91
		cmp     t(c), 'z' - 'a'
		ja      L91
	L93:
		add     t(c), 10
	L94:
		cmp     t(c), t(b)
		jae     L91

		align16
	L95:
		mul     ebx
		inc_tchar(esi)
		add     eax, ecx
		adc     edx, 0
		mov     t(c), tchar ptr [esi]                   // read next char
		sub     t(c), 'A'                               // check and convert char to value
		jge     L97
		add     t(c), 'A' - '0'
		jnc     L96
		cmp     t(c), '9' - '0'
		jbe     L99
	L96:
		jmp     L122
	L97:
		cmp     t(c), 'Z' - 'A'
		jbe     L98
		sub     t(c), 'a' - 'A'
		jb      L96
		cmp     t(c), 'z' - 'a'
		ja      L96
	L98:
		add     t(c), 10
	L99:
		cmp     t(c), t(b)
		jae     L96
		test    edx, edx
		jz      L95

		align16
	L100:
		mov     ebp, eax
		mov     eax, edx
		mul     ebx
		jc      L110
		mov     edi, eax
		mov     eax, ebp
		mul     ebx
		add     eax, ecx
		adc     edx, edi
		jc      L110
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // read next char
		inc_tchar(esi)
		sub     t(c), 'A'                               // check and convert char to value
		jge     L101
		add     t(c), 'A' - '0'
		jnc     L122
		cmp     t(c), '9' - '0'
		jbe     L103
		jmp     L122
	L101:
		cmp     t(c), 'Z' - 'A'
		jbe     L102
		sub     t(c), 'a' - 'A'
		jb      L122
		cmp     t(c), 'z' - 'a'
		ja      L122
	L102:
		add     t(c), 10
	L103:
		cmp     t(c), t(b)
		jb      L100
		jmp     L122

	L110:
		call    _errno                                  // overflow there
		mov     dword ptr [eax], ERANGE
		mov     ebx, dword ptr [endptr]
		mov     eax, 0FFFFFFFFH
		test    ebx, ebx
		mov     edx, eax
		jz      L124

		align16
	L111:
		mov     t(c), tchar ptr [esi + sizeof_tchar]    // point to end of string
		inc_tchar(esi)
		sub     t(c), 'A'
		jge     L112
		add     t(c), 'A' - '0'
		jnc     L121
		cmp     t(c), '9' - '0'
		jbe     L114
		jmp     L121
	L112:
		cmp     t(c), 'Z' - 'A'
		jbe     L113
		sub     t(c), 'a' - 'A'
		jb      L121
		cmp     t(c), 'z' - 'a'
		ja      L121
	L113:
		add     t(c), 10
	L114:
		cmp     t(c), t(b)
		jb      L111
		jmp     L121

		align16
	L120:
		mov     ebx, dword ptr [endptr]                 // store beginning of string in endptr
		mov     esi, dword ptr [nptr]                   // return 0
		test    ebx, ebx
		jz      L124
	L121:
		mov     dword ptr [ebx], esi
		jmp     L124
	L122:
		mov     ecx, dword ptr [endptr]
		test    ecx, ecx
		jz      L123
		mov     dword ptr [ecx], esi                    // store pointer to char that stopped the scan
	L123:
		cmp     tchar ptr [sign], '-'
		jne     L124
		neg     edx                                     // negate result if there was a neg sign
		neg     eax
		sbb     edx, 0
	L124:
		pop     ebp                                     // restore register
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef nptr
		#undef endptr
		#undef base
		#undef sign
	}
	#undef tchar
	#undef t
	#undef sizeof_tchar
	#undef inc_tchar
}
#endif
#endif
