#include <windows.h>
#include <stdint.h>
#include "intrinsic.h"
#include "digitstbl.h"

#if (!defined(_MSC_VER) || _MSC_VER < 1200) && !defined(__assume)
#define __assume(expression)
#endif

#ifdef _UNICODE
typedef wchar_t tuchar_t;
typedef uint32_t tchar2_t;
#define digits100T ((tchar2_t *)digits100W)
#else
typedef unsigned char tuchar_t;
typedef uint16_t tchar2_t;
#define digits100T ((tchar2_t *)digits100A)
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define T2(c) (tchar2_t)(tuchar_t)(c)
#define LO(x) ((uint32_t *)&x)[0]
#define HI(x) ((uint32_t *)&x)[1]
#else
#define T2(c) ((tchar2_t)(tuchar_t)(c) << (8 * sizeof(THCAR)))
#define LO(x) ((uint32_t *)&x)[1]
#define HI(x) ((uint32_t *)&x)[0]
#endif

#ifdef _UNICODE
#define _UI32TONT(n) _ui32to##n##w
#define _UI64TONT(n) _ui64to##n##w
#else
#define _UI32TONT(n) _ui32to##n##a
#define _UI64TONT(n) _ui64to##n##a
#endif

#define _ui32to10t _UI32TONT(10)

#define _ui64to10t _UI64TONT(10)
#define _ui64to2t  _UI64TONT(2)
#define _ui64to4t  _UI64TONT(4)
#define _ui64to8t  _UI64TONT(8)
#define _ui64to16t _UI64TONT(16)
#define _ui64to32t _UI64TONT(32)
#define _ui64tont  _UI64TONT(n)

size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer);

#define RECIPROCAL_DIVISION(dividend, divisor, precision) \
	(((dividend) * (((1 << (precision)) + (divisor) - 1) / (divisor))) >> (precision))

#ifndef _M_IX86
size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer)
{
	if (!(value >> 32))
	{
		return _ui32to10t((uint32_t)value, buffer);
	}
	else
	{
		const uint64_t reciprocal_u8 = ((uint64_t)1 << (32 + 25)) / 10000000u;
		const uint32_t reciprocal_lo = (uint32_t)reciprocal_u8;
		const uint32_t reciprocal_hi = (uint32_t)(reciprocal_u8 >> 32);

		TCHAR *p;

		p = buffer;
		if (value >= 10000000000000000)
			if (value >= 1000000000000000000)
				if (value >= 10000000000000000000)
					goto LENGTH20;
				else
					goto LENGTH19;
			else
				if (value >= 100000000000000000)
					goto LENGTH18;
				else
					goto LENGTH17;
		else
			if (value >= 10000000000000)
				if (value >= 1000000000000000)
					goto LENGTH16;
				else
					if (value >= 100000000000000)
						goto LENGTH15;
					else
						goto LENGTH14;
			else
				if (value >= 100000000000)
					if (value >= 1000000000000)
						goto LENGTH13;
					else
						goto LENGTH12;
				else
					if (value >= 10000000000)
						goto LENGTH11;
					else
						goto LENGTH10;

		#define PUTCHAR(power10)                                  \
		do                                                        \
		{                                                         \
		    if ((int64_t)(value -= 5 * power10) >= 0)             \
		    {                                                     \
		        if ((int64_t)(value -= 2 * power10) >= 0)         \
		        {                                                 \
		            if ((int64_t)(value -= 1 * power10) >= 0)     \
		            {                                             \
		                if ((int64_t)(value -= 1 * power10) >= 0) \
		                {                                         \
		                    *(p++) = TEXT('9');                   \
		                    break;                                \
		                }                                         \
		                *(p++) = TEXT('8');                       \
		            }                                             \
		            else                                          \
		            {                                             \
		                *(p++) = TEXT('7');                       \
		            }                                             \
		        }                                                 \
		        else                                              \
		        {                                                 \
		            if ((int64_t)(value += 1 * power10) >= 0)     \
		            {                                             \
		                *(p++) = TEXT('6');                       \
		                break;                                    \
		            }                                             \
		            *(p++) = TEXT('5');                           \
		        }                                                 \
		    }                                                     \
		    else                                                  \
		    {                                                     \
		        if ((int64_t)(value += 3 * power10) >= 0)         \
		        {                                                 \
		            if ((int64_t)(value -= 1 * power10) >= 0)     \
		            {                                             \
		                if ((int64_t)(value -= 1 * power10) >= 0) \
		                {                                         \
		                    *(p++) = TEXT('4');                   \
		                    break;                                \
		                }                                         \
		                *(p++) = TEXT('3');                       \
		            }                                             \
		            else                                          \
		            {                                             \
		                *(p++) = TEXT('2');                       \
		            }                                             \
		        }                                                 \
		        else                                              \
		        {                                                 \
		            if ((int64_t)(value += 1 * power10) >= 0)     \
		            {                                             \
		                *(p++) = TEXT('1');                       \
		                break;                                    \
		            }                                             \
		            *(p++) = TEXT('0');                           \
		        }                                                 \
		    }                                                     \
		    value += 1 * power10;                                 \
		} while (0)

	LENGTH20:
		value -= 10000000000000000000;
		*(p++) = TEXT('1');
	LENGTH19:
		PUTCHAR(1000000000000000000);
	LENGTH18:
		PUTCHAR(100000000000000000);
	LENGTH17:
		PUTCHAR(10000000000000000);
	LENGTH16:
		PUTCHAR(1000000000000000);
	LENGTH15:
		PUTCHAR(100000000000000);
	LENGTH14:
		PUTCHAR(10000000000000);
	LENGTH13:
		PUTCHAR(1000000000000);
	LENGTH12:
		PUTCHAR(100000000000);
	LENGTH11:
		PUTCHAR(10000000000);

		do
		{
			if (HI(value))
			{
	LENGTH10:
				if ((int64_t)(value -= 7000000000) >= 0)
				{
					if ((int32_t)(LO(value) -= 1000000000) >= 0)
					{
						if ((int32_t)(LO(value) -= 1000000000) >= 0)
						{
							p[0] = TEXT('9');
							break;
						}
						p[0] = TEXT('8');
					}
					else
					{
						p[0] = TEXT('7');
					}
				}
				else
				{
					if ((int32_t)(LO(value) += 2000000000) >= 0)
					{
						if ((int32_t)(LO(value) -= 1000000000) >= 0)
						{
							p[0] = TEXT('6');
							break;
						}
						p[0] = TEXT('5');
					}
					else
					{
						p[0] = TEXT('4');
					}
				}
			}
			else
			{
				if (LO(value) >= 2000000000)
				{
					if ((int32_t)(LO(value) -= 3000000000) >= 0)
					{
						if ((int32_t)(LO(value) -= 1000000000) >= 0)
						{
							p[0] = TEXT('4');
							break;
						}
						p[0] = TEXT('3');
					}
					else
					{
						p[0] = TEXT('2');
					}
				}
				else
				{
					if ((int32_t)(LO(value) -= 1000000000) >= 0)
					{
						p[0] = TEXT('1');
						break;
					}
					p[0] = TEXT('0');
				}
			}
			LO(value) += 1000000000;
		} while (0);

		#define value LO(value)

		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 2;
		*(tchar2_t *)&p[1] = digits100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&p[3] = digits100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&p[5] = digits100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&p[7] = digits100T[value >> 25]; value = (value & 0x01FFFFFF) * (10 >> 1);
		*(tchar2_t *)&p[9] = T2((value >> (25 - 1)) + TEXT('0'));

		#undef value

		return p + 10 - buffer;
	}
}
#else
__declspec(naked) size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define digits        digits100W
	#define tchar         word
	#define tchar2        dword
	#define sizeof_tchar2 4
	#define inc_tchar(r)  add r, 2
	#define t(r)          r##x
	#define t2(r)         e##r##x
#else
	#define digits        digits100A
	#define tchar         byte
	#define tchar2        word
	#define sizeof_tchar2 2
	#define inc_tchar(r)  inc r
	#define t(r)          r##l
	#define t2(r)         r##x
#endif

	__asm
	{
		mov     eax, dword ptr [esp + 4]
		mov     edx, dword ptr [esp + 8]
		test    edx, edx
		jnz     L1
		pop     eax
		mov     edx, ecx
		mov     dword ptr [esp + 4], eax
		pop     ecx
		jmp     _ui32to10t

	L1:
		cmp     edx, 0x002386F2	// 10000000000000000 >> 32
		ja      L2
		jne     L9
		cmp     eax, 0x6FC10000	// 10000000000000000 & 0xFFFFFFFF
		jb      L9
	L2:
		cmp     edx, 0x0DE0B6B3	// 1000000000000000000 >> 32
		ja      L3
		jne     L6
		cmp     eax, 0xA7640000	// 1000000000000000000 & 0xFFFFFFFF
		jb      L6
	L3:
		cmp     edx, 0x8AC72304	// 10000000000000000000 >> 32
		ja      L4
		jne     L5
		cmp     eax, 0x89E80000	// 10000000000000000000 & 0xFFFFFFFF
		jb      L5
	L4:
		mov     dword ptr [esp + 4], 20
		jmp     LENGTH20
	L5:
		mov     dword ptr [esp + 4], 19
		jmp     LENGTH19
	L6:
		cmp     edx, 0x01634578	// 100000000000000000 >> 32
		ja      L7
		jne     L8
		cmp     eax, 0x5D8A0000	// 100000000000000000 & 0xFFFFFFFF
		jb      L8
	L7:
		mov     dword ptr [esp + 4], 18
		jmp     LENGTH18
	L8:
		mov     dword ptr [esp + 4], 17
		jmp     LENGTH17
	L9:
		cmp     edx, 0x00000918	// 10000000000000 >> 32
		ja      L10
		jne     L15
		cmp     eax, 0x4E72A000	// 10000000000000 & 0xFFFFFFFF
		jb      L15
	L10:
		cmp     edx, 0x00038D7E	// 1000000000000000 >> 32
		ja      L11
		jne     L12
		cmp     eax, 0xA4C68000	// 1000000000000000 & 0xFFFFFFFF
		jb      L12
	L11:
		mov     dword ptr [esp + 4], 16
		jmp     LENGTH16
	L12:
		cmp     edx, 0x00005AF3	// 100000000000000 >> 32
		ja      L13
		jne     L14
		cmp     eax, 0x107A4000	// 100000000000000 & 0xFFFFFFFF
		jb      L14
	L13:
		mov     dword ptr [esp + 4], 15
		jmp     LENGTH15
	L14:
		mov     dword ptr [esp + 4], 14
		jmp     LENGTH14
	L15:
		cmp     edx, 0x00000017	// 100000000000 >> 32
		ja      L16
		jne     L19
		cmp     eax, 0x4876E800	// 100000000000 & 0xFFFFFFFF
		jb      L19
	L16:
		cmp     edx, 0x000000E8	// 1000000000000 >> 32
		ja      L17
		jne     L18
		cmp     eax, 0xD4A51000	// 1000000000000 & 0xFFFFFFFF
		jb      L18
	L17:
		mov     dword ptr [esp + 4], 13
		jmp     LENGTH13
	L18:
		mov     dword ptr [esp + 4], 12
		jmp     LENGTH12
	L19:
		cmp     edx, 0x00000002	// 10000000000 >> 32
		ja      L20
		jne     L21
		cmp     eax, 0x540BE400	// 10000000000 & 0xFFFFFFFF
		jb      L21
	L20:
		mov     dword ptr [esp + 4], 11
		jmp     LENGTH11
	L21:
		mov     dword ptr [esp + 4], 10
		jmp     LENGTH10

	LENGTH20:
		mov     tchar ptr [ecx], '1'
		sub     eax, 0x89E80000	// 10000000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x8AC72304	// 10000000000000000000 >> 32
		inc_tchar(ecx)

	LENGTH19:
		sub     eax, 0x44F40000	// 5000000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x45639182	// 5000000000000000000 >> 32
		js      L26
		sub     eax, 0x4EC80000	// 2000000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x1BC16D67	// 2000000000000000000 >> 32
		js      L24
		sub     eax, 0xA7640000	// 1000000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x0DE0B6B3	// 1000000000000000000 >> 32
		js      L22
		sub     eax, 0xA7640000	// 1000000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x0DE0B6B3	// 1000000000000000000 >> 32
		js      L23
		mov     tchar ptr [ecx], '9'
		jmp     L32
	L22:
		mov     tchar ptr [ecx], '7'
		jmp     L31
	L23:
		mov     tchar ptr [ecx], '8'
		jmp     L31
	L24:
		add     eax, 0xA7640000	// 1000000000000000000 & 0xFFFFFFFF
		adc     edx, 0x0DE0B6B3	// 1000000000000000000 >> 32
		js      L25
		mov     tchar ptr [ecx], '6'
		jmp     L32
	L25:
		mov     tchar ptr [ecx], '5'
		jmp     L31
	L26:
		add     eax, 0xF62C0000	// 3000000000000000000 & 0xFFFFFFFF
		adc     edx, 0x29A2241A	// 3000000000000000000 >> 32
		js      L29
		sub     eax, 0xA7640000	// 1000000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x0DE0B6B3	// 1000000000000000000 >> 32
		js      L27
		sub     eax, 0xA7640000	// 1000000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x0DE0B6B3	// 1000000000000000000 >> 32
		js      L28
		mov     tchar ptr [ecx], '4'
		jmp     L32
	L27:
		mov     tchar ptr [ecx], '2'
		jmp     L31
	L28:
		mov     tchar ptr [ecx], '3'
		jmp     L31
	L29:
		add     eax, 0xA7640000	// 1000000000000000000 & 0xFFFFFFFF
		adc     edx, 0x0DE0B6B3	// 1000000000000000000 >> 32
		js      L30
		mov     tchar ptr [ecx], '1'
		jmp     L32
	L30:
		mov     tchar ptr [ecx], '0'
	L31:
		add     eax, 0xA7640000	// 1000000000000000000 & 0xFFFFFFFF
		adc     edx, 0x0DE0B6B3	// 1000000000000000000 >> 32
	L32:
		inc_tchar(ecx)

	LENGTH18:
		sub     eax, 0xD3B20000	// 500000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x06F05B59	// 500000000000000000 >> 32
		js      L37
		sub     eax, 0xBB140000	// 200000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x02C68AF0	// 200000000000000000 >> 32
		js      L35
		sub     eax, 0x5D8A0000	// 100000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x01634578	// 100000000000000000 >> 32
		js      L33
		sub     eax, 0x5D8A0000	// 100000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x01634578	// 100000000000000000 >> 32
		js      L34
		mov     tchar ptr [ecx], '9'
		jmp     L43
	L33:
		mov     tchar ptr [ecx], '7'
		jmp     L42
	L34:
		mov     tchar ptr [ecx], '8'
		jmp     L42
	L35:
		add     eax, 0x5D8A0000	// 100000000000000000 & 0xFFFFFFFF
		adc     edx, 0x01634578	// 100000000000000000 >> 32
		js      L36
		mov     tchar ptr [ecx], '6'
		jmp     L43
	L36:
		mov     tchar ptr [ecx], '5'
		jmp     L42
	L37:
		add     eax, 0x189E0000	// 300000000000000000 & 0xFFFFFFFF
		adc     edx, 0x0429D069	// 300000000000000000 >> 32
		js      L40
		sub     eax, 0x5D8A0000	// 100000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x01634578	// 100000000000000000 >> 32
		js      L38
		sub     eax, 0x5D8A0000	// 100000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x01634578	// 100000000000000000 >> 32
		js      L39
		mov     tchar ptr [ecx], '4'
		jmp     L43
	L38:
		mov     tchar ptr [ecx], '2'
		jmp     L42
	L39:
		mov     tchar ptr [ecx], '3'
		jmp     L42
	L40:
		add     eax, 0x5D8A0000	// 100000000000000000 & 0xFFFFFFFF
		adc     edx, 0x01634578	// 100000000000000000 >> 32
		js      L41
		mov     tchar ptr [ecx], '1'
		jmp     L43
	L41:
		mov     tchar ptr [ecx], '0'
	L42:
		add     eax, 0x5D8A0000	// 100000000000000000 & 0xFFFFFFFF
		adc     edx, 0x01634578	// 100000000000000000 >> 32
	L43:
		inc_tchar(ecx)

	LENGTH17:
		sub     eax, 0x2EC50000	// 50000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00B1A2BC	// 50000000000000000 >> 32
		js      L48
		sub     eax, 0xDF820000	// 20000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00470DE4	// 20000000000000000 >> 32
		js      L46
		sub     eax, 0x6FC10000	// 10000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x002386F2	// 10000000000000000 >> 32
		js      L44
		sub     eax, 0x6FC10000	// 10000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x002386F2	// 10000000000000000 >> 32
		js      L45
		mov     tchar ptr [ecx], '9'
		jmp     L54
	L44:
		mov     tchar ptr [ecx], '7'
		jmp     L53
	L45:
		mov     tchar ptr [ecx], '8'
		jmp     L53
	L46:
		add     eax, 0x6FC10000	// 10000000000000000 & 0xFFFFFFFF
		adc     edx, 0x002386F2	// 10000000000000000 >> 32
		js      L47
		mov     tchar ptr [ecx], '6'
		jmp     L54
	L47:
		mov     tchar ptr [ecx], '5'
		jmp     L53
	L48:
		add     eax, 0x4F430000	// 30000000000000000 & 0xFFFFFFFF
		adc     edx, 0x006A94D7	// 30000000000000000 >> 32
		js      L51
		sub     eax, 0x6FC10000	// 10000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x002386F2	// 10000000000000000 >> 32
		js      L49
		sub     eax, 0x6FC10000	// 10000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x002386F2	// 10000000000000000 >> 32
		js      L50
		mov     tchar ptr [ecx], '4'
		jmp     L54
	L49:
		mov     tchar ptr [ecx], '2'
		jmp     L53
	L50:
		mov     tchar ptr [ecx], '3'
		jmp     L53
	L51:
		add     eax, 0x6FC10000	// 10000000000000000 & 0xFFFFFFFF
		adc     edx, 0x002386F2	// 10000000000000000 >> 32
		js      L52
		mov     tchar ptr [ecx], '1'
		jmp     L54
	L52:
		mov     tchar ptr [ecx], '0'
	L53:
		add     eax, 0x6FC10000	// 10000000000000000 & 0xFFFFFFFF
		adc     edx, 0x002386F2	// 10000000000000000 >> 32
	L54:
		inc_tchar(ecx)

	LENGTH16:
		sub     eax, 0x37E08000	// 5000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x0011C379	// 5000000000000000 >> 32
		js      L59
		sub     eax, 0x498D0000	// 2000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00071AFD	// 2000000000000000 >> 32
		js      L57
		sub     eax, 0xA4C68000	// 1000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00038D7E	// 1000000000000000 >> 32
		js      L55
		sub     eax, 0xA4C68000	// 1000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00038D7E	// 1000000000000000 >> 32
		js      L56
		mov     tchar ptr [ecx], '9'
		jmp     L65
	L55:
		mov     tchar ptr [ecx], '7'
		jmp     L64
	L56:
		mov     tchar ptr [ecx], '8'
		jmp     L64
	L57:
		add     eax, 0xA4C68000	// 1000000000000000 & 0xFFFFFFFF
		adc     edx, 0x00038D7E	// 1000000000000000 >> 32
		js      L58
		mov     tchar ptr [ecx], '6'
		jmp     L65
	L58:
		mov     tchar ptr [ecx], '5'
		jmp     L64
	L59:
		add     eax, 0xEE538000	// 3000000000000000 & 0xFFFFFFFF
		adc     edx, 0x000AA87B	// 3000000000000000 >> 32
		js      L62
		sub     eax, 0xA4C68000	// 1000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00038D7E	// 1000000000000000 >> 32
		js      L60
		sub     eax, 0xA4C68000	// 1000000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00038D7E	// 1000000000000000 >> 32
		js      L61
		mov     tchar ptr [ecx], '4'
		jmp     L65
	L60:
		mov     tchar ptr [ecx], '2'
		jmp     L64
	L61:
		mov     tchar ptr [ecx], '3'
		jmp     L64
	L62:
		add     eax, 0xA4C68000	// 1000000000000000 & 0xFFFFFFFF
		adc     edx, 0x00038D7E	// 1000000000000000 >> 32
		js      L63
		mov     tchar ptr [ecx], '1'
		jmp     L65
	L63:
		mov     tchar ptr [ecx], '0'
	L64:
		add     eax, 0xA4C68000	// 1000000000000000 & 0xFFFFFFFF
		adc     edx, 0x00038D7E	// 1000000000000000 >> 32
	L65:
		inc_tchar(ecx)

	LENGTH15:
		sub     eax, 0x52634000	// 500000000000000 & 0xFFFFFFFF
		sbb     edx, 0x0001C6BF	// 500000000000000 >> 32
		js      L70
		sub     eax, 0x20F48000	// 200000000000000 & 0xFFFFFFFF
		sbb     edx, 0x0000B5E6	// 200000000000000 >> 32
		js      L68
		sub     eax, 0x107A4000	// 100000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00005AF3	// 100000000000000 >> 32
		js      L66
		sub     eax, 0x107A4000	// 100000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00005AF3	// 100000000000000 >> 32
		js      L67
		mov     tchar ptr [ecx], '9'
		jmp     L76
	L66:
		mov     tchar ptr [ecx], '7'
		jmp     L75
	L67:
		mov     tchar ptr [ecx], '8'
		jmp     L75
	L68:
		add     eax, 0x107A4000	// 100000000000000 & 0xFFFFFFFF
		adc     edx, 0x00005AF3	// 100000000000000 >> 32
		js      L69
		mov     tchar ptr [ecx], '6'
		jmp     L76
	L69:
		mov     tchar ptr [ecx], '5'
		jmp     L75
	L70:
		add     eax, 0x316EC000	// 300000000000000 & 0xFFFFFFFF
		adc     edx, 0x000110D9	// 300000000000000 >> 32
		js      L73
		sub     eax, 0x107A4000	// 100000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00005AF3	// 100000000000000 >> 32
		js      L71
		sub     eax, 0x107A4000	// 100000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00005AF3	// 100000000000000 >> 32
		js      L72
		mov     tchar ptr [ecx], '4'
		jmp     L76
	L71:
		mov     tchar ptr [ecx], '2'
		jmp     L75
	L72:
		mov     tchar ptr [ecx], '3'
		jmp     L75
	L73:
		add     eax, 0x107A4000	// 100000000000000 & 0xFFFFFFFF
		adc     edx, 0x00005AF3	// 100000000000000 >> 32
		js      L74
		mov     tchar ptr [ecx], '1'
		jmp     L76
	L74:
		mov     tchar ptr [ecx], '0'
	L75:
		add     eax, 0x107A4000	// 100000000000000 & 0xFFFFFFFF
		adc     edx, 0x00005AF3	// 100000000000000 >> 32
	L76:
		inc_tchar(ecx)

	LENGTH14:
		sub     eax, 0x883D2000	// 50000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00002D79	// 50000000000000 >> 32
		js      L81
		sub     eax, 0x9CE54000	// 20000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00001230	// 20000000000000 >> 32
		js      L79
		sub     eax, 0x4E72A000	// 10000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000918	// 10000000000000 >> 32
		js      L77
		sub     eax, 0x4E72A000	// 10000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000918	// 10000000000000 >> 32
		js      L78
		mov     tchar ptr [ecx], '9'
		jmp     L87
	L77:
		mov     tchar ptr [ecx], '7'
		jmp     L86
	L78:
		mov     tchar ptr [ecx], '8'
		jmp     L86
	L79:
		add     eax, 0x4E72A000	// 10000000000000 & 0xFFFFFFFF
		adc     edx, 0x00000918	// 10000000000000 >> 32
		js      L80
		mov     tchar ptr [ecx], '6'
		jmp     L87
	L80:
		mov     tchar ptr [ecx], '5'
		jmp     L86
	L81:
		add     eax, 0xEB57E000	// 30000000000000 & 0xFFFFFFFF
		adc     edx, 0x00001B48	// 30000000000000 >> 32
		js      L84
		sub     eax, 0x4E72A000	// 10000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000918	// 10000000000000 >> 32
		js      L82
		sub     eax, 0x4E72A000	// 10000000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000918	// 10000000000000 >> 32
		js      L83
		mov     tchar ptr [ecx], '4'
		jmp     L87
	L82:
		mov     tchar ptr [ecx], '2'
		jmp     L86
	L83:
		mov     tchar ptr [ecx], '3'
		jmp     L86
	L84:
		add     eax, 0x4E72A000	// 10000000000000 & 0xFFFFFFFF
		adc     edx, 0x00000918	// 10000000000000 >> 32
		js      L85
		mov     tchar ptr [ecx], '1'
		jmp     L87
	L85:
		mov     tchar ptr [ecx], '0'
	L86:
		add     eax, 0x4E72A000	// 10000000000000 & 0xFFFFFFFF
		adc     edx, 0x00000918	// 10000000000000 >> 32
	L87:
		inc_tchar(ecx)

	LENGTH13:
		sub     eax, 0x27395000	// 5000000000000 & 0xFFFFFFFF
		sbb     edx, 0x0000048C	// 5000000000000 >> 32
		js      L92
		sub     eax, 0xA94A2000	// 2000000000000 & 0xFFFFFFFF
		sbb     edx, 0x000001D1	// 2000000000000 >> 32
		js      L90
		sub     eax, 0xD4A51000	// 1000000000000 & 0xFFFFFFFF
		sbb     edx, 0x000000E8	// 1000000000000 >> 32
		js      L88
		sub     eax, 0xD4A51000	// 1000000000000 & 0xFFFFFFFF
		sbb     edx, 0x000000E8	// 1000000000000 >> 32
		js      L89
		mov     tchar ptr [ecx], '9'
		jmp     L98
	L88:
		mov     tchar ptr [ecx], '7'
		jmp     L97
	L89:
		mov     tchar ptr [ecx], '8'
		jmp     L97
	L90:
		add     eax, 0xD4A51000	// 1000000000000 & 0xFFFFFFFF
		adc     edx, 0x000000E8	// 1000000000000 >> 32
		js      L91
		mov     tchar ptr [ecx], '6'
		jmp     L98
	L91:
		mov     tchar ptr [ecx], '5'
		jmp     L97
	L92:
		add     eax, 0x7DEF3000	// 3000000000000 & 0xFFFFFFFF
		adc     edx, 0x000002BA	// 3000000000000 >> 32
		js      L95
		sub     eax, 0xD4A51000	// 1000000000000 & 0xFFFFFFFF
		sbb     edx, 0x000000E8	// 1000000000000 >> 32
		js      L93
		sub     eax, 0xD4A51000	// 1000000000000 & 0xFFFFFFFF
		sbb     edx, 0x000000E8	// 1000000000000 >> 32
		js      L94
		mov     tchar ptr [ecx], '4'
		jmp     L98
	L93:
		mov     tchar ptr [ecx], '2'
		jmp     L97
	L94:
		mov     tchar ptr [ecx], '3'
		jmp     L97
	L95:
		add     eax, 0xD4A51000	// 1000000000000 & 0xFFFFFFFF
		adc     edx, 0x000000E8	// 1000000000000 >> 32
		js      L96
		mov     tchar ptr [ecx], '1'
		jmp     L98
	L96:
		mov     tchar ptr [ecx], '0'
	L97:
		add     eax, 0xD4A51000	// 1000000000000 & 0xFFFFFFFF
		adc     edx, 0x000000E8	// 1000000000000 >> 32
	L98:
		inc_tchar(ecx)

	LENGTH12:
		sub     eax, 0x6A528800	// 500000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000074	// 500000000000 >> 32
		js      L103
		sub     eax, 0x90EDD000	// 200000000000 & 0xFFFFFFFF
		sbb     edx, 0x0000002E	// 200000000000 >> 32
		js      L101
		sub     eax, 0x4876E800	// 100000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000017	// 100000000000 >> 32
		js      L99
		sub     eax, 0x4876E800	// 100000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000017	// 100000000000 >> 32
		js      L100
		mov     tchar ptr [ecx], '9'
		jmp     L109
	L99:
		mov     tchar ptr [ecx], '7'
		jmp     L108
	L100:
		mov     tchar ptr [ecx], '8'
		jmp     L108
	L101:
		add     eax, 0x4876E800	// 100000000000 & 0xFFFFFFFF
		adc     edx, 0x00000017	// 100000000000 >> 32
		js      L102
		mov     tchar ptr [ecx], '6'
		jmp     L109
	L102:
		mov     tchar ptr [ecx], '5'
		jmp     L108
	L103:
		add     eax, 0xD964B800	// 300000000000 & 0xFFFFFFFF
		adc     edx, 0x00000045	// 300000000000 >> 32
		js      L106
		sub     eax, 0x4876E800	// 100000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000017	// 100000000000 >> 32
		js      L104
		sub     eax, 0x4876E800	// 100000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000017	// 100000000000 >> 32
		js      L105
		mov     tchar ptr [ecx], '4'
		jmp     L109
	L104:
		mov     tchar ptr [ecx], '2'
		jmp     L108
	L105:
		mov     tchar ptr [ecx], '3'
		jmp     L108
	L106:
		add     eax, 0x4876E800	// 100000000000 & 0xFFFFFFFF
		adc     edx, 0x00000017	// 100000000000 >> 32
		js      L107
		mov     tchar ptr [ecx], '1'
		jmp     L109
	L107:
		mov     tchar ptr [ecx], '0'
	L108:
		add     eax, 0x4876E800	// 100000000000 & 0xFFFFFFFF
		adc     edx, 0x00000017	// 100000000000 >> 32
	L109:
		inc_tchar(ecx)

	LENGTH11:
		sub     eax, 0xA43B7400	// 50000000000 & 0xFFFFFFFF
		sbb     edx, 0x0000000B	// 50000000000 >> 32
		js      L114
		sub     eax, 0xA817C800	// 20000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000004	// 20000000000 >> 32
		js      L112
		sub     eax, 0x540BE400	// 10000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000002	// 10000000000 >> 32
		js      L110
		sub     eax, 0x540BE400	// 10000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000002	// 10000000000 >> 32
		js      L111
		mov     tchar ptr [ecx], '9'
		jmp     L120
	L110:
		mov     tchar ptr [ecx], '7'
		jmp     L119
	L111:
		mov     tchar ptr [ecx], '8'
		jmp     L119
	L112:
		add     eax, 0x540BE400	// 10000000000 & 0xFFFFFFFF
		adc     edx, 0x00000002	// 10000000000 >> 32
		js      L113
		mov     tchar ptr [ecx], '6'
		jmp     L120
	L113:
		mov     tchar ptr [ecx], '5'
		jmp     L119
	L114:
		add     eax, 0xFC23AC00	// 30000000000 & 0xFFFFFFFF
		adc     edx, 0x00000006	// 30000000000 >> 32
		js      L117
		sub     eax, 0x540BE400	// 10000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000002	// 10000000000 >> 32
		js      L115
		sub     eax, 0x540BE400	// 10000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000002	// 10000000000 >> 32
		js      L116
		mov     tchar ptr [ecx], '4'
		jmp     L120
	L115:
		mov     tchar ptr [ecx], '2'
		jmp     L119
	L116:
		mov     tchar ptr [ecx], '3'
		jmp     L119
	L117:
		add     eax, 0x540BE400	// 10000000000 & 0xFFFFFFFF
		adc     edx, 0x00000002	// 10000000000 >> 32
		js      L118
		mov     tchar ptr [ecx], '1'
		jmp     L120
	L118:
		mov     tchar ptr [ecx], '0'
	L119:
		add     eax, 0x540BE400	// 10000000000 & 0xFFFFFFFF
		adc     edx, 0x00000002	// 10000000000 >> 32
	L120:
		inc_tchar(ecx)

		test    edx, edx
		jz      L126
	LENGTH10:
		sub     eax, 0xA13B8600	// 7000000000 & 0xFFFFFFFF
		sbb     edx, 0x00000001	// 7000000000 >> 32
		js      L123
		sub     eax, 1000000000
		jb      L121
		sub     eax, 1000000000
		jb      L122
		mov     tchar ptr [ecx], '9'
		jmp     L132
	L121:
		mov     tchar ptr [ecx], '7'
		jmp     L131
	L122:
		mov     tchar ptr [ecx], '8'
		jmp     L131
	L123:
		add     eax, 2000000000
		jnc     L124
		sub     eax, 1000000000
		jb      L125
		mov     tchar ptr [ecx], '6'
		jmp     L132
	L124:
		mov     tchar ptr [ecx], '4'
		jmp     L131
	L125:
		mov     tchar ptr [ecx], '5'
		jmp     L131
	L126:
		sub     eax, 2000000000
		jb      L129
		sub     eax, 1000000000
		jb      L127
		sub     eax, 1000000000
		jb      L128
		mov     tchar ptr [ecx], '4'
		jmp     L132
	L127:
		mov     tchar ptr [ecx], '2'
		jmp     L131
	L128:
		mov     tchar ptr [ecx], '3'
		jmp     L131
	L129:
		add     eax, 1000000000
		jnc     L130
		mov     tchar ptr [ecx], '1'
		jmp     L132
	L130:
		mov     tchar ptr [ecx], '0'
	L131:
		add     eax, 1000000000;
	L132:
		inc_tchar(ecx)

		mov     dword ptr [esp + 8], ebx
		lea     ebx, [eax + eax * 2]
		mov     edx, 0x5AFE5357	// (0x200000000000000 / 10000000) & 0xFFFFFFFF
		mul     edx
		lea     edx, [edx + ebx + 3]
		mov     eax, edx
		shr     edx, 25
		and     eax, 0x01FFFFFF
		imul    eax, 100
		mov     t2(b), tchar2 ptr [digits + edx * sizeof_tchar2]
		mov     edx, eax
		mov     tchar2 ptr [ecx], t2(b)
		add     ecx, sizeof_tchar2
		shr     edx, 25
		and     eax, 0x01FFFFFF
		imul    eax, 100
		mov     t2(b), tchar2 ptr [digits + edx * sizeof_tchar2]
		mov     edx, eax
		mov     tchar2 ptr [ecx], t2(b)
		add     ecx, sizeof_tchar2
		shr     edx, 25
		and     eax, 0x01FFFFFF
		imul    eax, 100
		mov     t2(b), tchar2 ptr [digits + edx * sizeof_tchar2]
		mov     edx, eax
		mov     tchar2 ptr [ecx], t2(b)
		add     ecx, sizeof_tchar2
		shr     edx, 25
		and     eax, 0x01FFFFFF
		mov     t2(b), tchar2 ptr [digits + edx * sizeof_tchar2]
		lea     edx, [eax + eax * 4]
		mov     tchar2 ptr [ecx], t2(b)
		add     ecx, sizeof_tchar2
		shr     edx, 24
		mov     eax, dword ptr [esp + 4]
#if __BYTE_ORDER == __LITTLE_ENDIAN
		add     t2(d), '0'
		mov     ebx, dword ptr [esp + 8]
		mov     tchar2 ptr [ecx], t2(d)
#else
		add     t(d), '0'
		mov     ebx, dword ptr [esp + 8]
		mov     tchar ptr [ecx], t(d)
		mov     tchar ptr [ecx + sizeof_tchar2], '\0'
#endif
		ret     8
	}

	#undef digits
	#undef tchar
	#undef tchar2
	#undef sizeof_tchar2
	#undef inc_tchar
	#undef t
	#undef t2
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui64to2t(uint64_t value, TCHAR *buffer)
{
	size_t length;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	length = (unsigned long)length + 1;
	*(buffer += length) = TEXT('\0');
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = (value & 0x01) + TEXT('0');
	while (value >>= 1);
#else
	while (HI(value))
	{
		*(--buffer) = (value & 0x01) + TEXT('0');
		value >>= 1;
	}
	do
		*(--buffer) = (LO(value) & 0x01) + TEXT('0');
	while (LO(value) >>= 1);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to2t(uint64_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		mov     edx, dword ptr [esp + 8]
		push    ebx
		mov     ebx, dword ptr [esp + 4 + 4]

		bsr     eax, edx
		lea     eax, [eax + 32 + 1]
		jnz     L1
		bsr     eax, ebx
		lea     eax, [eax + 1]
		jnz     L1
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		jmp     L5

		align   16
	L1:
		push    eax
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		add     ecx, eax
#endif
		mov     tchar ptr [ecx], '\0'
		jmp     L3

		align   16
	L2:
		mov     eax, ebx
		dec_tchar(ecx)
		and     eax, 1
		shr     edx, 1
		lea     eax, [eax + '0']
		rcr     ebx, 1
		mov     tchar ptr [ecx], t(a)
	L3:
		test    edx, edx
		jnz     L2

		align   16
	L4:
		mov     eax, ebx
		dec_tchar(ecx)
		and     eax, 1
		shr     ebx, 1
		lea     eax, [eax + '0']
		mov     tchar ptr [ecx], t(a)
		jnz     L4

		pop     eax
	L5:
		pop     ebx
		ret     8
	}

	#undef tchar
	#undef tchar2
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui64to4t(uint64_t value, TCHAR *buffer)
{
	size_t length;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	length = ((unsigned long)length / 2) + 1;
	*(buffer += length) = TEXT('\0');
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = (value & 0x03) + TEXT('0');
	while (value >>= 2);
#else
	while (HI(value))
	{
		*(--buffer) = (value & 0x03) + TEXT('0');
		value >>= 2;
	}
	do
		*(--buffer) = (LO(value) & 0x03) + TEXT('0');
	while (LO(value) >>= 2);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to4t(uint64_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		mov     edx, dword ptr [esp + 8]
		push    ebx
		mov     ebx, dword ptr [esp + 4 + 4]

		bsr     eax, edx
		lea     eax, [eax + 32 + 2]
		jnz     L1
		bsr     eax, ebx
		lea     eax, [eax + 2]
		jnz     L1
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		jmp     L5

		align   16
	L1:
		shr     eax, 1
		test    edx, edx
		push    eax
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		lea     ecx, [ecx + eax]
#endif
		mov     tchar ptr [ecx], '\0'
		jz      L4

		align   16
	L2:
		mov     eax, ebx
		dec_tchar(ecx)
		and     eax, 3
		shr     edx, 1
		lea     eax, [eax + '0']
		rcr     ebx, 1
		mov     tchar ptr [ecx], t(a)
		shr     edx, 1
		jz      L3
		rcr     ebx, 1
		jmp     L2
	L3:
		rcr     ebx, 1

		align   16
	L4:
		mov     eax, ebx
		dec_tchar(ecx)
		and     eax, 3
		shr     ebx, 2
		lea     eax, [eax + '0']
		mov     tchar ptr [ecx], t(a)
		jnz     L4

		pop     eax
	L5:
		pop     ebx
		ret     8
	}

	#undef tchar
	#undef tchar2
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer)
{
	size_t length;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	__assume((unsigned long)length < 64);
#if 0
	length = ((unsigned long)length / 3) + 1;
#else
	length = RECIPROCAL_DIVISION((unsigned long)length, 3, 7) + 1;
#endif
	*(buffer += length) = TEXT('\0');
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = (value & 0x07) + TEXT('0');
	while (value >>= 3);
#else
	while (HI(value))
	{
		*(--buffer) = (value & 0x07) + TEXT('0');
		value >>= 3;
	}
	do
		*(--buffer) = (LO(value) & 0x07) + TEXT('0');
	while (LO(value) >>= 3);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		push    ebx
		push    esi
		mov     ebx, dword ptr [esp + 8 + 4]
		mov     edx, dword ptr [esp + 8 + 8]

		bsr     eax, edx
		lea     eax, [eax + 32 + 3]
		jnz     L1
		bsr     eax, ebx
		lea     eax, [eax + 3]
		jnz     L1
		pop     esi
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		jmp     L5

		align   16
	L1:
		lea     esi, [eax + eax * 4]
		lea     eax, [eax + eax * 2]
		lea     eax, [eax + esi * 8]
		pop     esi
		shr     eax, 7
		test    edx, edx
		push    eax
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		lea     ecx, [ecx + eax]
#endif
		mov     tchar ptr [ecx], '\0'
		jz      L4

		align   16
	L2:
		mov     eax, ebx
		dec_tchar(ecx)
		shr     edx, 1
		rcr     ebx, 1
		and     eax, 7
		shr     edx, 1
		lea     eax, [eax + '0']
		rcr     ebx, 1
		mov     tchar ptr [ecx], t(a)
		shr     edx, 1
		jz      L3
		rcr     ebx, 1
		jmp     L2
	L3:
		rcr     ebx, 1

		align   16
	L4:
		mov     eax, ebx
		dec_tchar(ecx)
		and     eax, 7
		shr     ebx, 3
		lea     eax, [eax + '0']
		mov     tchar ptr [ecx], t(a)
		jnz     L4

		pop     eax
	L5:
		pop     ebx
		ret     8
	}

	#undef tchar
	#undef tchar2
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper)
{
	size_t              length;
	const unsigned char *digits;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	length = ((unsigned long)length / 4) + 1;
	*(buffer += length) = TEXT('\0');
	digits = upper ? digitsLarge : digitsSmall;
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = digits[(size_t)value & 0x0F];
	while (value >>= 4);
#else
	while (HI(value))
	{
		*(--buffer) = digits[(size_t)value & 0x0F];
		value >>= 4;
	}
	do
		*(--buffer) = digits[LO(value) & 0x0F];
	while (LO(value) >>= 4);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		push    ebx
		push    esi
		mov     ebx, dword ptr [esp + 8 + 4]
		mov     esi, dword ptr [esp + 8 + 8]

		bsr     eax, esi
		lea     eax, [eax + 32 + 4]
		jnz     L1
		bsr     eax, ebx
		lea     eax, [eax + 4]
		jnz     L1
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		jmp     L6

		align   16
	L1:
		shr     eax, 2
		test    edx, edx
		push    eax
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		lea     ecx, [ecx + eax]
#endif
		mov     tchar ptr [ecx], '\0'
		jz      L2
		mov     edx, offset digitsLarge
		jmp     L3
	L2:
		mov     edx, offset digitsSmall
	L3:
		test    esi, esi
		jz      L5

		align   16
	L4:
		mov     eax, ebx
		dec_tchar(ecx)
		shrd    ebx, esi, 4
		and     eax, 15
		shr     esi, 4
		mov     al, byte ptr [eax + edx]
		mov     tchar ptr [ecx], t(a)
		jnz     L4

		align   16
	L5:
		mov     eax, ebx
		dec_tchar(ecx)
		and     eax, 15
		shr     ebx, 4
		mov     al, byte ptr [eax + edx]
		mov     tchar ptr [ecx], t(a)
		jnz     L5

		pop     eax
	L6:
		pop     esi
		pop     ebx
		ret     8
	}

	#undef tchar
	#undef tchar2
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper)
{
	size_t              length;
	const unsigned char *digits;

	if (!_BitScanReverse64((unsigned long *)&length, value))
	{
		*(tchar2_t *)buffer = T2(TEXT('0'));
		return 1;
	}
	__assume((unsigned long)length < 64);
#if 0
	length = ((unsigned long)length / 5) + 1;
#else
	length = RECIPROCAL_DIVISION((unsigned long)length, 5, 6) + 1;
#endif
	*(buffer += length) = TEXT('\0');
	digits = upper ? digitsLarge : digitsSmall;
#if INTPTR_MAX != INT32_MAX
	do
		*(--buffer) = digits[(size_t)value & 0x1F];
	while (value >>= 5);
#else
	while (HI(value))
	{
		*(--buffer) = digits[(size_t)value & 0x1F];
		value >>= 5;
	}
	do
		*(--buffer) = digits[LO(value) & 0x1F];
	while (LO(value) >>= 5);
#endif
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper)
{
#ifdef _UNICODE
	#define tchar        word
	#define tchar2       dword
	#define dec_tchar(r) sub r, 2
	#define t(r)         r##x
#else
	#define tchar        byte
	#define tchar2       word
	#define dec_tchar(r) dec r
	#define t(r)         r##l
#endif

	__asm
	{
		push    ebx
		push    esi
		mov     ebx, dword ptr [esp + 8 + 4]
		mov     esi, dword ptr [esp + 8 + 8]

		bsr     eax, edx
		lea     eax, [eax + 32]
		jnz     L1
		bsr     eax, esi
		jnz     L1
		xor     eax, eax
#if __BYTE_ORDER == __LITTLE_ENDIAN
		mov     tchar2 ptr [ecx], '0'
#else
		mov     tchar2 ptr [ecx], '0' << (8 * sizeof_tchar)
#endif
		inc     eax
		jmp     L6

		align   16
	L1:
		push    edi
		lea     edi, [eax + eax * 4]
		lea     eax, [edi + eax * 8 + (1 << 6)]
		pop     edi
		shr     eax, 6
		test    edx, edx
		push    eax
#ifdef _UNICODE
		lea     ecx, [ecx + eax * 2]
#else
		lea     ecx, [ecx + eax]
#endif
		mov     tchar ptr [ecx], '\0'
		jz      L2
		mov     edx, offset digitsLarge
		jmp     L3
	L2:
		mov     edx, offset digitsSmall
	L3:
		test    esi, esi
		jz      L5

		align   16
	L4:
		mov     eax, ebx
		dec_tchar(ecx)
		shrd    ebx, esi, 5
		and     eax, 31
		shr     esi, 5
		mov     al, byte ptr [eax + edx]
		mov     tchar ptr [ecx], t(a)
		jnz     L4

		align   16
	L5:
		mov     eax, ebx
		dec_tchar(ecx)
		and     eax, 31
		shr     ebx, 5
		mov     al, byte ptr [eax + edx]
		mov     tchar ptr [ecx], t(a)
		jnz     L5

		pop     eax
	L6:
		pop     esi
		pop     ebx
		ret     8
	}

	#undef tchar
	#undef tchar2
	#undef dec_tchar
	#undef t
}
#endif

#ifndef _M_IX86
size_t __fastcall _ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	size_t              length;
	const unsigned char *digits;
	TCHAR               *p1, *p2;

	if (radix < 2 || radix > 36)
	{
		*buffer = TEXT('\0');
		return 0;
	}
	digits = upper ? digitsLarge : digitsSmall;
	p1 = buffer;
#if INTPTR_MAX != INT32_MAX
	do
	{
		unsigned int remainder;

		remainder = value % radix;
		value /= radix;
		*(p1++) = (TCHAR)digits[remainder];
	} while (value);
#else
	while (HI(value))
	{
		unsigned int remainder;

		remainder = value % radix;
		value /= radix;
		*(p1++) = (TCHAR)digits[remainder];
	}
	do
	{
		unsigned int remainder;

		remainder = LO(value) % radix;
		LO(value) /= radix;
		*(p1++) = (TCHAR)digits[remainder];
	} while (LO(value));
#endif
	length = p1 - (p2 = buffer);
	*(p1--) = TEXT('\0');
	while (p1 > p2)
	{
		TCHAR c1, c2;

		c1 = *p1;
		c2 = *p2;
		*(p1--) = c2;
		*(p2++) = c1;
	}
	return length;
}
#else
__declspec(naked) size_t __fastcall _ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
#ifdef _UNICODE
	#define t(r)         r##x
	#define tchar        word
	#define sizeof_tchar 2
	#define inc_tchar(r) add r, 2
	#define dec_tchar(r) sub r, 2
#else
	#define t(r)         r##l
	#define tchar        byte
	#define sizeof_tchar 1
	#define inc_tchar(r) inc r
	#define dec_tchar(r) dec r
#endif

	__asm
	{
		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     ebx, dword ptr [esp + 16 + 4]
		mov     esi, dword ptr [esp + 16 + 8]
		mov     edi, dword ptr [esp + 16 + 12]
		mov     eax, ebx

		#define lo     ebx
		#define hi     esi
		#define buffer (esp + 16 + 4)
		#define upper  edx
		#define radix  edi
		#define digits ebp
		#define p1     ecx
		#define p2     esi

		cmp     radix, 2
		jl      L1
		cmp     radix, 36
		jbe     L2
	L1:
		mov     tchar ptr [p1], '\0'
		xor     eax, eax
		jmp     L9

		align   16
	L2:
		mov     dword ptr [buffer], p1
		dec_tchar(p1)
		test    upper, upper
		jz      L3
		mov     digits, offset digitsLarge
		jmp     L4
	L3:
		mov     digits, offset digitsSmall
	L4:
		test    hi, hi
		jz      L6

		align   16
	L5:
		mov     eax, hi
		xor     edx, edx
		div     radix
		mov     hi, eax
		mov     eax, lo
		div     radix
		mov     dl, byte ptr [digits + edx]
		inc_tchar(p1)
		mov     lo, eax
		test    hi, hi
		mov     tchar ptr [p1], t(d)
		jnz     L5

		align   16
	L6:
		xor     edx, edx
		inc_tchar(p1)
		div     radix
		mov     dl, byte ptr [digits + edx]
		test    eax, eax
		mov     tchar ptr [p1], t(d)
		jnz     L6

		lea     eax, [p1 + sizeof_tchar]
		mov     p2, dword ptr [buffer]
		mov     tchar ptr [eax], '\0'
		sub     eax, p2
#ifdef _UNICODE
		shr     eax, 1
#endif
		jmp     L8

		align   16
	L7:
		mov     t(b), tchar ptr [p1]
		mov     t(d), tchar ptr [p2]
		mov     tchar ptr [p1], t(d)
		mov     tchar ptr [p2], t(b)
		dec_tchar(p1)
		inc_tchar(p2)
	L8:
		cmp     p1, p2
		ja      L7

	L9:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret     12

		#undef lo
		#undef hi
		#undef buffer
		#undef upper
		#undef radix
		#undef digits
		#undef p1
		#undef p2
	}

	#undef t
	#undef tchar
	#undef sizeof_tchar
	#undef inc_tchar
	#undef dec_tchar
}
#endif
