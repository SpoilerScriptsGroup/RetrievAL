#ifndef _INTRINSIC_H_
#define _INTRINSIC_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#endif

#include <limits.h>
#ifndef MAXQWORD
#define MAXQWORD _UI64_MAX
typedef ULONG64    QWORD;
typedef QWORD near *PQWORD;
typedef QWORD far  *LPQWORD;
#endif

#ifdef __BORLANDC__
#pragma warn -8098
#endif

#if (defined(_MSC_VER) && _MSC_VER < 1200) || defined(__BORLANDC__)
#define __forceinline __inline
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CODEPAGE_SUPPORT 0

#define __intrinsic_isleadbyte_cp932(c) \
	((BYTE)(c) >= (BYTE)0x81 && ((BYTE)(c) <= (BYTE)0x9F || ((BYTE)(c) >= (BYTE)0xE0 && (BYTE)(c) <= (BYTE)0xFC)))

#define __intrinsic_istailbyte_cp932(c) \
	((BYTE)(c) >= (BYTE)0x40 && ((BYTE)(c) <= (BYTE)0x7E || ((char)(c) < 0 && (BYTE)(c) <= (BYTE)0xEC)))

#if defined(CODEPAGE_SUPPORT) && CODEPAGE_SUPPORT
#include <mbctype.h>
#define __intrinsic_isleadbyte _ismbblead
#define __intrinsic_istailbyte _ismbbtrail
#else
#define __intrinsic_isleadbyte __intrinsic_isleadbyte_cp932
#define __intrinsic_istailbyte __intrinsic_istailbyte_cp932
#endif

#define case_unsigned_leadbyte_cp932                                                        \
               case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: \
    case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F: \
    case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: \
    case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F: \
    case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: case 0xE6: case 0xE7: \
    case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEE: case 0xEF: \
    case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF6: case 0xF7: \
    case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC

#define __intrinsic_isascii(c) \
	((char)(c) >= 0)

#define __intrinsic_isdigit(c) \
	((char)(c) >= '0' && (BYTE)(c) <= (BYTE)'9')

#define __intrinsic_isspace(c) \
	((c) == ' ' || ((BYTE)(c) <= (BYTE)'\r' && (BYTE)(c) >= (BYTE)'\t'))

#define __intrinsic_isspace_without_return(c) \
	((c) == ' ' || (c) == '\t' || (c) == '\v' || (c) == '\f')

#define __intrinsic_isupper(c) \
	((char)(c) >= 'A' && (BYTE)(c) <= (BYTE)'Z')

#define __intrinsic_islower(c) \
	((char)(c) >= 'a' && (BYTE)(c) <= (BYTE)'z')

#define __intrinsic_isalpha(c) \
	(__intrinsic_isupper(c) || __intrinsic_islower(c))

#define __intrinsic_isalnum(c) \
	(__intrinsic_isalpha(c) || __intrinsic_isdigit(c))

#define __intrinsic_iscsym(c) \
	(__intrinsic_isalnum(c) || (c) == '_')

#define __intrinsic_iskana(c) \
	((BYTE)(c) >= (BYTE)0xA1 && (BYTE)(c) <= (BYTE)0xDF)

#define __intrinsic_tolower(c) \
	(__intrinsic_isupper(c) ? (char)((char)(c) + (char)('a' - 'A')) : (char)(c))

#define __intrinsic_toupper(c) \
	(__intrinsic_islower(c) ? (char)((char)(c) - (char)('a' - 'A')) : (char)(c))

// for constant value
#define BSWAP16(value) (WORD)( \
    ((WORD)(value) >> 8) |     \
    ((WORD)(value) << 8))

// for constant value
#define BSWAP24(value) (DWORD)(             \
    (((DWORD)(value) >> 16) & 0x000000FF) | \
    ( (DWORD)(value)        & 0x0000FF00) | \
    (((DWORD)(value) << 16) & 0x00FF0000))

// for constant value
#define BSWAP32(value) (DWORD)(             \
    ( (DWORD)(value) >> 24              ) | \
    (((DWORD)(value) >>  8) & 0x0000FF00) | \
    (((DWORD)(value) <<  8) & 0x00FF0000) | \
    ( (DWORD)(value) << 24              ))

// for constant value
#define BSWAP40(value) (QWORD)(                     \
    (((QWORD)(value) >> 32) & 0x00000000000000FF) | \
    (((QWORD)(value) >> 16) & 0x000000000000FF00) | \
    ( (QWORD)(value)        & 0x0000000000FF0000) | \
    (((QWORD)(value) << 16) & 0x00000000FF000000) | \
    (((QWORD)(value) << 32) & 0x000000FF00000000))

// for constant value
#define BSWAP48(value) (QWORD)(                     \
    (((QWORD)(value) >> 40) & 0x00000000000000FF) | \
    (((QWORD)(value) >> 24) & 0x000000000000FF00) | \
    (((QWORD)(value) >>  8) & 0x0000000000FF0000) | \
    (((QWORD)(value) <<  8) & 0x00000000FF000000) | \
    (((QWORD)(value) << 24) & 0x000000FF00000000) | \
    (((QWORD)(value) << 40) & 0x0000FF0000000000))

// for constant value
#define BSWAP56(value) (QWORD)(                     \
    (((QWORD)(value) >> 48) & 0x00000000000000FF) | \
    (((QWORD)(value) >> 32) & 0x000000000000FF00) | \
    (((QWORD)(value) >> 16) & 0x0000000000FF0000) | \
    ( (QWORD)(value)        & 0x00000000FF000000) | \
    (((QWORD)(value) << 16) & 0x000000FF00000000) | \
    (((QWORD)(value) << 32) & 0x0000FF0000000000) | \
    (((QWORD)(value) << 48) & 0x00FF000000000000))

// for constant value
#define BSWAP64(value) (QWORD)(                     \
    ( (QWORD)(value) >> 56                      ) | \
    (((QWORD)(value) >> 40) & 0x000000000000FF00) | \
    (((QWORD)(value) >> 24) & 0x0000000000FF0000) | \
    (((QWORD)(value) >>  8) & 0x00000000FF000000) | \
    (((QWORD)(value) <<  8) & 0x000000FF00000000) | \
    (((QWORD)(value) << 24) & 0x0000FF0000000000) | \
    (((QWORD)(value) << 40) & 0x00FF000000000000) | \
    ( (QWORD)(value) << 56                      ))

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned short _byteswap_ushort(unsigned short val)
{
	__asm
	{
		mov     ax, word ptr [val]
		xchg    al, ah
	}
}
__forceinline unsigned long _byteswap_ulong(unsigned long val)
{
	__asm
	{
		mov     eax, dword ptr [val]
		bswap   eax
	}
}
__forceinline unsigned __int64 _byteswap_uint64(unsigned __int64 val)
{
	__asm
	{
		mov     edx, dword ptr [val]
		mov     eax, dword ptr [val + 4]
		bswap   edx
		bswap   eax
	}
}
#elif defined(__BORLANDC__)
unsigned short __fastcall _byteswap_ushort(unsigned short val);
unsigned long __fastcall _byteswap_ulong(unsigned long val);
unsigned __int64 __msreturn __fastcall __fastcall_byteswap_uint64(unsigned long low, unsigned long high);
#define _byteswap_uint64(val) __fastcall_byteswap_uint64((unsigned long)(val), (unsigned long)((unsigned __int64)(val) >> 32))
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define __intrinsic_bswap16 _byteswap_ushort
#define __intrinsic_bswap24(value) _byteswap_ulong((DWORD)(value) << 8)
#define __intrinsic_bswap32 _byteswap_ulong
#define __intrinsic_bswap40(value) _byteswap_uint64((QWORD)(value) << 24)
#define __intrinsic_bswap48(value) _byteswap_uint64((QWORD)(value) << 16)
#define __intrinsic_bswap56(value) _byteswap_uint64((QWORD)(value) << 8)
#define __intrinsic_bswap64 _byteswap_uint64
#else
#define __intrinsic_bswap16 BSWAP16
#define __intrinsic_bswap24 BSWAP24
#define __intrinsic_bswap32 BSWAP32
#define __intrinsic_bswap40 BSWAP40
#define __intrinsic_bswap48 BSWAP48
#define __intrinsic_bswap56 BSWAP56
#define __intrinsic_bswap64 BSWAP64
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotr)
#pragma intrinsic(_rotl64)
#pragma intrinsic(_rotr64)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned int _rotl(unsigned int value, int shift)
{
	__asm
	{
		mov     cl, byte ptr [shift]
		mov     eax, dword ptr [value]
		rol     eax, cl
	}
}
__forceinline unsigned int _rotr(unsigned int value, int shift)
{
	__asm
	{
		mov     cl, byte ptr [shift]
		mov     eax, dword ptr [value]
		ror     eax, cl
	}
}
__forceinline unsigned __int64 _rotl64(unsigned __int64 value, int shift)
{
	__asm
	{
		mov     eax, dword ptr [value]
		mov     edx, dword ptr [value + 4]
		mov     cl, byte ptr [shift]
		mov     edi, eax
		test    cl, 32
		jz      L1
		mov     edi, edx
		mov     edx, eax
		mov     eax, edi
	L1:
		and     cl, 31
		shld    eax, edx, cl
		shld    edx, edi, cl
	}
}
__forceinline unsigned __int64 _rotr64(unsigned __int64 value, int shift)
{
	__asm
	{
		mov     eax, dword ptr [value]
		mov     edx, dword ptr [value + 4]
		mov     cl, byte ptr [shift]
		mov     edi, eax
		test    cl, 32
		jz      L1
		mov     edi, edx
		mov     edx, eax
		mov     eax, edi
	L1:
		and     cl, 31
		shrd    eax, edx, cl
		shrd    edx, edi, cl
	}
}
#elif defined(__BORLANDC__)
unsigned int __fastcall __fastcall_rotl(unsigned int value, int shift);
unsigned int __fastcall __fastcall_rotr(unsigned int value, int shift);
unsigned __int64 __msreturn __fastcall __fastcall_rotl64(DWORD low, DWORD high, int shift);
unsigned __int64 __msreturn __fastcall __fastcall_rotr64(DWORD low, DWORD high, int shift);
#define _rotl __fastcall_rotl
#define _rotr __fastcall_rotr
#define _rotl64(value, shift) __fastcall_rotl64((DWORD)(value), (DWORD)((QWORD)(value) >> 32), shift)
#define _rotr64(value, shift) __fastcall_rotr64((DWORD)(value), (DWORD)((QWORD)(value) >> 32), shift)
#else
#define _rotl(value, shift) (((unsigned int)(value) << (shift)) | ((unsigned int)(value) >> (32 - (shift))))
#define _rotr(value, shift) (((unsigned int)(value) >> (shift)) | ((unsigned int)(value) << (32 - (shift))))
#define _rotl64(value, shift) (((QWORD)(value) << (shift)) | ((QWORD)(value) >> (64 - (shift))))
#define _rotr64(value, shift) (((QWORD)(value) >> (shift)) | ((QWORD)(value) << (64 - (shift))))
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(__movsb)
#pragma intrinsic(__movsw)
#pragma intrinsic(__movsd)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline void __movsb(unsigned char *Destination, const unsigned char *Source, size_t Count)
{
	__asm
	{
		mov     ecx, dword ptr [Count]
		mov     esi, dword ptr [Source]
		mov     edi, dword ptr [Destination]
		rep movsb
	}
}
__forceinline void __movsw(unsigned short *Destination, const unsigned short *Source, size_t Count)
{
	__asm
	{
		mov     ecx, dword ptr [Count]
		mov     esi, dword ptr [Source]
		mov     edi, dword ptr [Destination]
		rep movsw
	}
}
__forceinline void __movsd(unsigned long *Destination, const unsigned long *Source, size_t Count)
{
	__asm
	{
		mov     ecx, dword ptr [Count]
		mov     esi, dword ptr [Source]
		mov     edi, dword ptr [Destination]
		rep movsd
	}
}
#elif defined(__BORLANDC__)
void __fastcall __movsb(unsigned char *Destination, const unsigned char *Source, size_t Count);
void __fastcall __movsw(unsigned short *Destination, const unsigned short *Source, size_t Count);
void __fastcall __movsd(unsigned long *Destination, const unsigned long *Source, size_t Count);
#else
#define __movsb memcpy
#define __movsw(Destination, Source, Count) memcpy(Destination, Source, (size_t)(Count) * 2)
#define __movsd(Destination, Source, Count) memcpy(Destination, Source, (size_t)(Count) * 4)
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(__emul)
#pragma intrinsic(__emulu)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline __int64 __emul(int a, int b)
{
	__asm
	{
		mov     edx, dword ptr [b]
		mov     eax, dword ptr [a]
		imul    edx
	}
}
__forceinline unsigned __int64 __emulu(unsigned int a, unsigned int b)
{
	__asm
	{
		mov     edx, dword ptr [b]
		mov     eax, dword ptr [a]
		mul     edx
	}
}
#elif defined(__BORLANDC__)
__int64 __msreturn __fastcall __emul(int a, int b);
unsigned __int64 __msreturn __fastcall __emulu(unsigned int a, unsigned int b);
#else
#define __emul(a, b) ((__int64)(int)(a) * (int)(b))
#define __emulu(a, b) ((unsigned __int64)(unsigned int)(a) * (unsigned int)(b))
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned char _BitScanForward(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		mov     edx, dword ptr [Mask]
		xor     eax, eax
		test    edx, edx
		jz      L1
		bsf     edx, edx
		mov     ecx, dword ptr [Index]
		inc     eax
		mov     dword ptr [ecx], edx
	L1:
	}
}
__forceinline unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		mov     edx, dword ptr [Mask]
		xor     eax, eax
		test    edx, edx
		jz      L1
		bsr     edx, edx
		mov     ecx, dword ptr [Index]
		inc     eax
		mov     dword ptr [ecx], edx
	L1:
	}
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _BitScanForward(unsigned long *Index, unsigned long Mask);
unsigned char __fastcall _BitScanReverse(unsigned long *Index, unsigned long Mask);
#else
__forceinline unsigned char _BitScanForward(unsigned long *Index, unsigned long Mask)
{
	if (Mask)
	{
		unsigned long shift;

		for (shift = 0; !((1UL << shift) & Mask); shift++);
		*Index = shift;
		return 1;
	}
	else
	{
		return 0;
	}
}
__forceinline unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	if (Mask)
	{
		unsigned long shift;

		for (shift = 31; !((1UL << shift) & Mask); shift--);
		*Index = shift;
		return 1;
	}
	else
	{
		return 0;
	}
}
#endif

#if defined(_MSC_VER) && defined(_M_IX64)
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#else
__forceinline unsigned char _BitScanForward64(unsigned long *Index, unsigned __int64 Mask)
{
	if ((unsigned long)Mask)
	{
		_BitScanForward(Index, (unsigned long)Mask);
		return 1;
	}
	else if ((unsigned long)(Mask >> 32))
	{
		_BitScanForward(Index, (unsigned long)(Mask >> 32));
		*Index += 32;
		return 1;
	}
	else
	{
		return 0;
	}
}
__forceinline unsigned char _BitScanReverse64(unsigned long *Index, unsigned __int64 Mask)
{
	if ((unsigned long)(Mask >> 32))
	{
		_BitScanReverse(Index, (unsigned long)(Mask >> 32));
		*Index += 32;
		return 1;
	}
	else if ((unsigned long)Mask)
	{
		_BitScanReverse(Index, (unsigned long)Mask);
		return 1;
	}
	else
	{
		return 0;
	}
}
#endif

#ifdef __cplusplus
}
#endif

#endif	// _INTRINSIC_H_
