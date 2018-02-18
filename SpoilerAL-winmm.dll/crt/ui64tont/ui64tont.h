#if defined(_UI32TONT) && defined(INTERNAL_UI32TONT) && defined(_UI64TONT) && defined(INTERNAL_UI64TONT)
#include <windows.h>
#include "intrinsic.h"
#include <stdint.h>
#include "digitstbl.h"

#ifdef _MSC_VER
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __BYTE_ORDER    __LITTLE_ENDIAN
#endif

#ifdef _UNICODE
typedef uint32_t tchar2_t;
#define digitsDec100T ((tchar2_t *)digitsDec100W)
#else
typedef uint16_t tchar2_t;
#define digitsDec100T ((tchar2_t *)digitsDec100A)
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define TO_TCHAR2(c) (tchar2_t)(c)
#define LO(x) ((uint32_t *)&x)[0]
#define HI(x) ((uint32_t *)&x)[1]
#else
#define TO_TCHAR2(c) ((tchar2_t)(c) << (sizeof(TCHAR) * 8))
#define LO(x) ((uint32_t *)&x)[1]
#define HI(x) ((uint32_t *)&x)[0]
#endif

#define _ui32to10t         _UI32TONT(10)
#define _ui32to2t          _UI32TONT(2)
#define _ui32to4t          _UI32TONT(4)
#define _ui32to8t          _UI32TONT(8)
#define _ui32to16t         _UI32TONT(16)
#define _ui32to32t         _UI32TONT(32)
#define internal_ui32tont  INTERNAL_UI32TONT(n)

#define _ui64to10t         _UI64TONT(10)
#define _ui64to2t          _UI64TONT(2)
#define _ui64to4t          _UI64TONT(4)
#define _ui64to8t          _UI64TONT(8)
#define _ui64to16t         _UI64TONT(16)
#define _ui64to32t         _UI64TONT(32)
#define _ui64tont          _UI64TONT(n)

#define internal_ui64to10t INTERNAL_UI64TONT(10)
#define internal_ui64to2t  INTERNAL_UI64TONT(2)
#define internal_ui64to4t  INTERNAL_UI64TONT(4)
#define internal_ui64to8t  INTERNAL_UI64TONT(8)
#define internal_ui64to16t INTERNAL_UI64TONT(16)
#define internal_ui64to32t INTERNAL_UI64TONT(32)
#define internal_ui64tont  INTERNAL_UI64TONT(n)

size_t __fastcall _ui32to10t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to2t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to4t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to8t(uint32_t value, TCHAR *buffer);
size_t __fastcall _ui32to16t(uint32_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall _ui32to32t(uint32_t value, TCHAR *buffer, BOOL upper);
size_t __fastcall internal_ui32tont(uint32_t value, TCHAR *buffer, BOOL upper, unsigned int radix);
size_t __fastcall internal_ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix);

#ifndef _M_IX86
size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer)
{
	if (!(value >> 32))
	{
		return _ui32to10t((uint32_t)value, buffer);
	}
	else
#else
__declspec(naked) size_t __fastcall _ui64to10t(uint64_t value, TCHAR *buffer)
{
	static size_t __fastcall internal_ui64to10t(uint64_t value, TCHAR *buffer);

	__asm
	{
		cmp     dword ptr [esp + 8], 0
		jne     L1
		pop     eax
		mov     edx, ecx
		mov     dword ptr [esp + 4], eax
		pop     ecx
		jmp     _ui32to10t
	L1:
		jmp     internal_ui64to10t
	}
}

static size_t __fastcall internal_ui64to10t(uint64_t value, TCHAR *buffer)
{
#endif
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

		#undef PUTCHAR

		do
		{
			if (HI(value))
			{
	LENGTH10:
				if ((int64_t)(value -= 7000000000) >= 0)
				{
					if ((int64_t)(value -= 1000000000) >= 0)
					{
						if ((int64_t)(value -= 1000000000) >= 0)
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
					if ((int64_t)(value += 2000000000) >= 0)
					{
						if ((int64_t)(value -= 1000000000) >= 0)
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
				if (LO(value) >= 1000000000)
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
						if ((int32_t)(LO(value) += 1000000000) >= 0)
						{
							p[0] = TEXT('2');
							break;
						}
						p[0] = TEXT('1');
					}
				}
				else
				{
					p[0] = TEXT('0');
					break;
				}
			}
			LO(value) += 1000000000;
		} while (0);

		#define value LO(value)

		value = (uint32_t)(__emulu(value, reciprocal_lo) >> 32)
			+ value * reciprocal_hi
			+ 2;
		*(tchar2_t *)&p[1] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&p[3] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&p[5] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 100;
		*(tchar2_t *)&p[7] = digitsDec100T[value >> 25]; value = (value & 0x01FFFFFF) * 10;
		*(tchar2_t *)&p[9] = TO_TCHAR2((value >> 25) + TEXT('0'));

		#undef value

		return p + 10 - buffer;
	}
}

#ifndef _M_IX86
size_t __fastcall _ui64to2t(uint64_t value, TCHAR *buffer)
{
	if (!(value >> 32))
	{
		return _ui32to2t((uint32_t)value, buffer);
	}
	else
#else
__declspec(naked) size_t __fastcall _ui64to2t(uint64_t value, TCHAR *buffer)
{
	static size_t __fastcall internal_ui64to2t(uint64_t value, TCHAR *buffer);

	__asm
	{
		cmp     dword ptr [esp + 8], 0
		jne     L1
		pop     eax
		mov     edx, ecx
		mov     dword ptr [esp + 4], eax
		pop     ecx
		jmp     _ui32to2t
	L1:
		jmp     internal_ui64to2t
	}
}

static size_t __fastcall internal_ui64to2t(uint64_t value, TCHAR *buffer)
{
#endif
	{
		size_t length;

		_BitScanReverse((unsigned long *)&length, HI(value));
		length = (unsigned long)length + 32 + 1;
		*(buffer += length) = TEXT('\0');
		do
			*(--buffer) = ((TCHAR)value & 0x01) + TEXT('0');
		while (value >>= 1);
		return length;
	}
}

#ifndef _M_IX86
size_t __fastcall _ui64to4t(uint64_t value, TCHAR *buffer)
{
	if (!(value >> 32))
	{
		return _ui32to4t((uint32_t)value, buffer);
	}
	else
#else
__declspec(naked) size_t __fastcall _ui64to4t(uint64_t value, TCHAR *buffer)
{
	static size_t __fastcall internal_ui64to4t(uint64_t value, TCHAR *buffer);

	__asm
	{
		cmp     dword ptr [esp + 8], 0
		jne     L1
		pop     eax
		mov     edx, ecx
		mov     dword ptr [esp + 4], eax
		pop     ecx
		jmp     _ui32to4t
	L1:
		jmp     internal_ui64to4t
	}
}

static size_t __fastcall internal_ui64to4t(uint64_t value, TCHAR *buffer)
{
#endif
	{
		size_t length;

		_BitScanReverse((unsigned long *)&length, HI(value));
		length = (unsigned long)length / 2 + 16 + 1;
		*(buffer += length) = TEXT('\0');
		do
			*(--buffer) = ((TCHAR)value & 0x03) + TEXT('0');
		while (value >>= 2);
		return length;
	}
}

#ifndef _M_IX86
size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer)
{
	if (!(value >> 32))
	{
		return _ui32to8t((uint32_t)value, buffer);
	}
	else
#else
__declspec(naked) size_t __fastcall _ui64to8t(uint64_t value, TCHAR *buffer)
{
	static size_t __fastcall internal_ui64to8t(uint64_t value, TCHAR *buffer);

	__asm
	{
		cmp     dword ptr [esp + 8], 0
		jne     L1
		pop     eax
		mov     edx, ecx
		mov     dword ptr [esp + 4], eax
		pop     ecx
		jmp     _ui32to8t
	L1:
		jmp     internal_ui64to8t
	}
}

static size_t __fastcall internal_ui64to8t(uint64_t value, TCHAR *buffer)
{
#endif
	{
		size_t length;

		if (HI(value) >= (uint32_t)(01000000000000000000u >> 32))
			if (HI(value) >= (uint32_t)(0100000000000000000000u >> 32))
				if (HI(value) >= (uint32_t)(01000000000000000000000u >> 32))
					length = 22;
				else
					length = 21;
			else
				if (HI(value) >= (uint32_t)(010000000000000000000u >> 32))
					length = 20;
				else
					length = 19;
		else
			if (HI(value) >= (uint32_t)(0100000000000000u >> 32))
				if (HI(value) >= (uint32_t)(010000000000000000u >> 32))
					if (HI(value) >= (uint32_t)(0100000000000000000u >> 32))
						length = 18;
					else
						length = 17;
				else
					if (HI(value) >= (uint32_t)(01000000000000000u >> 32))
						length = 16;
					else
						length = 15;
			else
				if (HI(value) >= (uint32_t)(01000000000000u >> 32))
					if (HI(value) >= (uint32_t)(010000000000000u >> 32))
						length = 14;
					else
						length = 13;
				else
					if (HI(value) >= (uint32_t)(0100000000000u >> 32))
						length = 12;
					else
						length = 11;
		*(buffer += length) = TEXT('\0');
		do
			*(--buffer) = ((TCHAR)value & 0x07) + TEXT('0');
		while (value >>= 3);
		return length;
	}
}

#ifndef _M_IX86
size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper)
{
	if (!(value >> 32))
	{
		return _ui32to16t((uint32_t)value, buffer, radix);
	}
	else
#else
__declspec(naked) size_t __fastcall _ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper)
{
	static size_t __fastcall internal_ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper);

	__asm
	{
		cmp     dword ptr [esp + 8], 0
		jne     L1
		mov     dword ptr [esp + 8], edx
		pop     eax
		mov     edx, ecx
		pop     ecx
		push    eax
		jmp     _ui32to16t
	L1:
		jmp     internal_ui64to16t
	}
}

static size_t __fastcall internal_ui64to16t(uint64_t value, TCHAR *buffer, BOOL upper)
{
#endif
	{
		size_t     length;
		const char *digits;

		if (HI(value) >= 0x10000)
			if (HI(value) >= 0x1000000)
				if (HI(value) >= 0x10000000)
					length = 16;
				else
					length = 15;
			else
				if (HI(value) >= 0x100000)
					length = 14;
				else
					length = 13;
		else
			if (HI(value) >= 0x100)
				if (HI(value) >= 0x1000)
					length = 12;
				else
					length = 11;
			else
				if (HI(value) >= 0x10)
					length = 10;
				else
					length = 9;
		*(buffer += length) = TEXT('\0');
		digits = upper ? digitsHexLarge : digitsHexSmall;
		do
			*(--buffer) = digits[(size_t)value & 0x0F];
		while (value >>= 4);
		return length;
	}
}

#ifndef _M_IX86
size_t __fastcall _ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper)
{
	if (!(value >> 32))
	{
		return _ui32to32t((uint32_t)value, buffer, radix);
	}
	else
#else
__declspec(naked) size_t __fastcall _ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper)
{
	static size_t __fastcall internal_ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper);

	__asm
	{
		cmp     dword ptr [esp + 8], 0
		jne     L1
		mov     dword ptr [esp + 8], edx
		pop     eax
		mov     edx, ecx
		pop     ecx
		push    eax
		jmp     _ui32to32t
	L1:
		jmp     internal_ui64to32t
	}
}

static size_t __fastcall internal_ui64to32t(uint64_t value, TCHAR *buffer, BOOL upper)
{
#endif
	{
		size_t     length;
		const char *digits;

		if (HI(value) >= 0x4000000000000)
			if (HI(value) >= 0x1000000000000000)
				length = 13;
			else
				if (HI(value) >= 0x80000000000000)
					length = 12;
				else
					length = 11;
		else
			if (HI(value) >= 0x10000000000)
				if (HI(value) >= 0x200000000000)
					length = 10;
				else
					length = 9;
			else
				if (HI(value) >= 0x800000000)
					length = 8;
				else
					length = 7;
		*(buffer += length) = TEXT('\0');
		digits = upper ? digitsHexLarge : digitsHexSmall;
		do
			*(--buffer) = digits[(size_t)value & 0x1F];
		while (value >>= 5);
		return length;
	}
}

#ifndef _M_IX86
size_t __fastcall _ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	if (radix >= 2 && radix <= 36)
	{
		return internal_ui64tont(value, buffer, upper, radix);
	}
	else
	{
		*buffer = TEXT('\0');
		return 0;
	}
}

size_t __fastcall internal_ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	if (!(value >> 32))
	{
		return internal_ui32tont((uint32_t)value, buffer, radix);
	}
	else
	{
		size_t     length;
		const char *digits;
		TCHAR      *p1, *p2;

		digits = upper ? digitsHexLarge : digitsHexSmall;
		p1 = buffer;
		do
		{
			unsigned long remainder;

			remainder = value % radix;
			value /= radix;
			*(p1++) = (TCHAR)digits[remainder];
		} while (value);
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
}
#else
__declspec(naked) size_t __fastcall _ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	__asm
	{
#ifndef _UNICODE
		#define tchar byte
#else
		#define tchar word
#endif

		mov     eax, dword ptr [esp + 12]
		cmp     eax, 2
		jl      L1
		cmp     eax, 36
		ja      L1
		jmp     internal_ui64tont
	L1:
		mov     tchar ptr [ecx], '\0'
		xor     eax, eax
		ret     8

		#undef tchar
	}
}

__declspec(naked) size_t __fastcall internal_ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	static size_t __fastcall _internal_ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix);

	__asm
	{
		cmp     dword ptr [esp + 8], 0
		jne     L1
		mov     dword ptr [esp + 8], edx
		pop     eax
		mov     edx, ecx
		pop     ecx
		push    eax
		jmp     internal_ui32tont
	L1:
		jmp     _internal_ui64tont
	}
}

__declspec(naked) static size_t __fastcall _internal_ui64tont(uint64_t value, TCHAR *buffer, BOOL upper, unsigned int radix)
{
	__asm
	{
#ifndef _UNICODE
		#define t(r)           r##l
		#define tchar          byte
		#define movt           mov
		#define inc_tchar(r)   inc r
		#define dec_tchar(r)   dec r
#else
		#define t(r)           r##x
		#define tchar          word
		#define movt           movzx
		#define inc_tchar(r)   add r, 2
		#define dec_tchar(r)   sub r, 2
#endif

		#define param_value_lo (esp + 16 + 4)
		#define param_value_hi (esp + 16 + 8)
		#define param_buffer   ecx
		#define param_upper    edx
		#define param_radix    (esp + 16 + 12)

		#define lo             ebx
		#define hi             esi
		#define radix          edi
		#define buffer         (esp + 16 + 4)
		#define length         eax
		#define digits         ebp
		#define p1             ecx
		#define p2             esi

		push    ebx
		push    esi
		push    edi
		push    ebp

		mov     lo, dword ptr [param_value_lo]
		mov     hi, dword ptr [param_value_hi]
		mov     radix, dword ptr [param_radix]
		mov     dword ptr [buffer], param_buffer
		test    param_upper, param_upper
		jz      L1
		mov     digits, offset digitsHexLarge
		jmp     L2
	L1:
		mov     digits, offset digitsHexSmall

		align   16
	L2:
		mov     eax, hi
		xor     edx, edx
		div     radix
		mov     hi, eax
		mov     eax, lo
		div     radix
		mov     lo, eax
		movt    t(a), byte ptr [digits + edx]
		mov     edx, lo
		mov     tchar ptr [p1], t(a)
		inc_tchar(p1)
		or      edx, hi
		jnz     L2

		mov     p2, dword ptr [buffer]
		mov     tchar ptr [p1], '\0'
		mov     length, p1
		dec_tchar(p1)
		sub     length, p2
		jmp     L4

		align   16
	L3:
		mov     t(b), tchar ptr [p1]
		mov     t(d), tchar ptr [p2]
		mov     tchar ptr [p1], t(d)
		mov     tchar ptr [p2], t(b)
		dec_tchar(p1)
		inc_tchar(p2)
	L4:
		cmp     p1, p2
		ja      L3

		pop     ebp
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef t
		#undef tchar
		#undef movt
		#undef inc_tchar
		#undef dec_tchar
		#undef param_value_lo
		#undef param_value_hi
		#undef param_buffer
		#undef param_upper
		#undef param_radix
		#undef lo
		#undef hi
		#undef radix
		#undef buffer
		#undef length
		#undef digits
		#undef p1
		#undef p2
	}
}
#endif
#endif
