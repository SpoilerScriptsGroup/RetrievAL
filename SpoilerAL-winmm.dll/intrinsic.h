#ifndef _INTRINSIC_H_
#define _INTRINSIC_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#endif

#if (!defined(_MSC_VER) || _MSC_VER >= 1600) && !defined(__BORLANDC__)
#include <stdint.h>
#else
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef __int64          int64_t;
typedef unsigned __int64 uint64_t;
#endif

#ifdef __BORLANDC__
#pragma warn -8027
#pragma warn -8098
#endif

#ifndef __BORLANDC__
#define __msreturn
#define __msfastcall __fastcall
#endif

#if (defined(_MSC_VER) && _MSC_VER < 1200) || defined(__BORLANDC__)
#define __forceinline static __inline
#endif

#if (defined(_MSC_VER) && _MSC_VER < 1400) || defined(__BORLANDC__)
#define __restrict
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CODEPAGE_SUPPORT 0

#define __intrinsic_isleadbyte_cp932(c) \
	((unsigned char)((c) - 0x81) < 0x9F - 0x81 + 1 || (unsigned char)((c) - 0xE0) < 0xFC - 0xE0 + 1)

#define __intrinsic_istrailbyte_cp932(c) \
	((unsigned char)((c) - 0x40) < 0x7E - 0x40 + 1 || (unsigned char)((c) - 0x80) < 0xEC - 0x80 + 1)

#define case_unsigned_leadbyte_cp932                                                        \
               case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: \
    case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F: \
    case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: \
    case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F: \
    case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: case 0xE6: case 0xE7: \
    case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEE: case 0xEF: \
    case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF6: case 0xF7: \
    case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC

#if CODEPAGE_SUPPORT
#define __intrinsic_isleadbyte(c) IsDBCSLeadByteEx(CP_THREAD_ACP, c)
#else
#define __intrinsic_isleadbyte __intrinsic_isleadbyte_cp932
#define __intrinsic_istrailbyte __intrinsic_istrailbyte_cp932
#define case_unsigned_leadbyte case_unsigned_leadbyte_cp932
#endif

#define ___intrinsic_isalpha(type, c) \
	((unsigned type)(((c) | ('a' - 'A')) - 'a') < 'z' - 'a' + 1)
#define __intrinsic_isalpha(c) ___intrinsic_isalpha(char, c)
#define __intrinsic_iswalpha(c) ___intrinsic_isalpha(short, c)

#define ___intrinsic_isupper(type, c) \
	((unsigned type)((c) - 'A') < 'Z' - 'A' + 1)
#define __intrinsic_isupper(c) ___intrinsic_isupper(char, c)
#define __intrinsic_iswupper(c) ___intrinsic_isupper(short, c)

#define ___intrinsic_islower(type, c) \
	((unsigned type)((c) - 'a') < 'z' - 'a' + 1)
#define __intrinsic_islower(c) ___intrinsic_islower(char, c)
#define __intrinsic_iswlower(c) ___intrinsic_islower(short, c)

#define ___intrinsic_isdigit(type, c) \
	((unsigned type)((c) - '0') < '9' - '0' + 1)
#define __intrinsic_isdigit(c) ___intrinsic_isdigit(char, c)
#define __intrinsic_iswdigit(c) ___intrinsic_isdigit(short, c)

#define ___intrinsic_isxdigit(type, c) \
	(___intrinsic_isdigit(type, c) || (unsigned type)(((c) | ('a' - 'A')) - 'a') < 'f' - 'a' + 1)
#define __intrinsic_isxdigit(c) ___intrinsic_isxdigit(char, c)
#define __intrinsic_iswxdigit(c) ___intrinsic_isxdigit(short, c)

#define ___intrinsic_isspace(type, c) \
	((c) == ' ' || (unsigned type)((c) - '\t') < '\r' - '\t' + 1)
#define __intrinsic_isspace(c) ___intrinsic_isspace(char, c)
#define __intrinsic_iswspace(c) ___intrinsic_isspace(short, c)

#define ___intrinsic_isspace_without_return(type, c) \
	((c) == ' ' || (unsigned type)((c) - '\t') < '\f' - '\t' + 1 && (c) != '\n')
#define __intrinsic_isspace_without_return(c) ___intrinsic_isspace_without_return(char, c)
#define __intrinsic_iswspace_without_return(c) ___intrinsic_isspace_without_return(short, c)

#define ___intrinsic_ispunct(type, c) \
	((unsigned type)((c) - 0x21) < 0x7E - 0x21 + 1 && (unsigned type)(((c) | ('a' - 'A')) - 'z' - 1) < (unsigned type)('a' - 'z' - 1) && (unsigned type)((c) - '9' - 1) < (unsigned type)('0' - '9' - 1))
#define __intrinsic_ispunct(c) ___intrinsic_ispunct(char, c)
#define __intrinsic_iswpunct(c) ___intrinsic_ispunct(short, c)

#define __intrinsic_isblank(c) \
	((c) == ' ' || (c) == '\t')
#define __intrinsic_iswblank __intrinsic_isblank

#define ___intrinsic_isalnum(type, c) \
	(___intrinsic_isalpha(type, c) || ___intrinsic_isdigit(type, c))
#define __intrinsic_isalnum(c) ___intrinsic_isalnum(char, c)
#define __intrinsic_iswalnum(c) ___intrinsic_isalnum(short, c)

#define ___intrinsic_isprint(type, c) \
	((unsigned type)((c) - 0x20) < 0x7E - 0x20 + 1)
#define __intrinsic_isprint(c) ___intrinsic_isprint(char, c)
#define __intrinsic_iswprint(c) ___intrinsic_isprint(short, c)

#define ___intrinsic_isgraph(type, c) \
	((unsigned type)((c) - 0x21) < 0x7E - 0x21 + 1)
#define __intrinsic_isgraph(c) ___intrinsic_isgraph(char, c)
#define __intrinsic_iswgraph(c) ___intrinsic_isgraph(short, c)

#define ___intrinsic_iscntrl(type, c) \
	((unsigned type)(c) < 0x1F + 1 || (c) == 0x7F)
#define __intrinsic_iscntrl(c) ___intrinsic_iscntrl(char, c)
#define __intrinsic_iswcntrl(c) ___intrinsic_iscntrl(short, c)

#define __intrinsic_isascii(c) \
	((char)(c) >= 0)

#define __intrinsic_iswascii(c) \
	((wchar_t)(c) <= 0x7F)

#define ___intrinsic_iscsymf(type, c) \
	(___intrinsic_isalpha(type, c) || (c) == '_')
#define __intrinsic_iscsymf(c) ___intrinsic_iscsymf(char, c)
#define __intrinsic_iswcsymf(c) ___intrinsic_iscsymf(short, c)

#define ___intrinsic_iscsym(type, c) \
	(___intrinsic_isalnum(type, c) || (c) == '_')
#define __intrinsic_iscsym(c) ___intrinsic_iscsym(char, c)
#define __intrinsic_iswcsym(c) ___intrinsic_iscsym(short, c)

#define __intrinsic_iskana(c) \
	((unsigned char)((c) - 0xA1) < 0xDF - 0xA1 + 1)

#define __intrinsic_ismbchira(c) \
	((unsigned short)((c) - 0x829F) < 0x82F1 - 0x829F + 1)

#define __intrinsic_ismbckata(c) \
	((unsigned short)((c) - 0x8340) < 0x8396 - 0x8340 + 1)

#define __intrinsic_ismbclegal(c) \
	(((unsigned short)((c) - 0x8140) < 0x9FFC - 0x8140 + 1 || (unsigned short)((c) - 0xE040) < 0xFCFC - 0xE040 + 1) && (unsigned char)(c) != 0x7F)

#define __intrinsic_ismbcsymbol(c) \
	((unsigned short)((c) - 0x8141) < 0x81AC - 0x8141 + 1)

#define __intrinsic_ismbcl0(c) \
	((unsigned short)((c) - 0x8140) < 0x889E - 0x8140 + 1)

#define __intrinsic_ismbcl1(c) \
	((unsigned short)((c) - 0x889F) < 0x9872 - 0x889F + 1)

#define __intrinsic_ismbcl2(c) \
	((unsigned short)((c) - 0x989F) < 0xEA9E - 0x989F + 1)

#define ___intrinsic_tolower(type, c) \
	(!___intrinsic_isupper(type, c) ? (c) : (c) + 'a' - 'A')
#define __intrinsic_tolower(c) ___intrinsic_tolower(char, c)
#define __intrinsic_towlower(c) ___intrinsic_tolower(short, c)

#define ___intrinsic_toupper(type, c) \
	(!___intrinsic_islower(type, c) ? (c) : (c) + 'A' - 'a')
#define __intrinsic_toupper(c) ___intrinsic_toupper(char, c)
#define __intrinsic_towupper(c) ___intrinsic_toupper(short, c)

#define __intrinsic_toascii(c) \
	((c) & 0x7F)
#define __intrinsic_towascii __intrinsic_toascii

// for constant value
#define BSWAP16(value) (uint16_t)( \
    ((uint16_t)(value) >> 8) |     \
    ((uint16_t)(value) << 8))

// for constant value
#define BSWAP24(value) (uint32_t)(             \
    (((uint32_t)(value) >> 16) & 0x000000FF) | \
    ( (uint32_t)(value)        & 0x0000FF00) | \
    (((uint32_t)(value) << 16) & 0x00FF0000))

// for constant value
#define BSWAP32(value) (uint32_t)(             \
    ( (uint32_t)(value) >> 24              ) | \
    (((uint32_t)(value) >>  8) & 0x0000FF00) | \
    (((uint32_t)(value) <<  8) & 0x00FF0000) | \
    ( (uint32_t)(value) << 24              ))

// for constant value
#define BSWAP40(value) (uint64_t)(                     \
    (((uint64_t)(value) >> 32) & 0x00000000000000FF) | \
    (((uint64_t)(value) >> 16) & 0x000000000000FF00) | \
    ( (uint64_t)(value)        & 0x0000000000FF0000) | \
    (((uint64_t)(value) << 16) & 0x00000000FF000000) | \
    (((uint64_t)(value) << 32) & 0x000000FF00000000))

// for constant value
#define BSWAP48(value) (uint64_t)(                     \
    (((uint64_t)(value) >> 40) & 0x00000000000000FF) | \
    (((uint64_t)(value) >> 24) & 0x000000000000FF00) | \
    (((uint64_t)(value) >>  8) & 0x0000000000FF0000) | \
    (((uint64_t)(value) <<  8) & 0x00000000FF000000) | \
    (((uint64_t)(value) << 24) & 0x000000FF00000000) | \
    (((uint64_t)(value) << 40) & 0x0000FF0000000000))

// for constant value
#define BSWAP56(value) (uint64_t)(                     \
    (((uint64_t)(value) >> 48) & 0x00000000000000FF) | \
    (((uint64_t)(value) >> 32) & 0x000000000000FF00) | \
    (((uint64_t)(value) >> 16) & 0x0000000000FF0000) | \
    ( (uint64_t)(value)        & 0x00000000FF000000) | \
    (((uint64_t)(value) << 16) & 0x000000FF00000000) | \
    (((uint64_t)(value) << 32) & 0x0000FF0000000000) | \
    (((uint64_t)(value) << 48) & 0x00FF000000000000))

// for constant value
#define BSWAP64(value) (uint64_t)(                     \
    ( (uint64_t)(value) >> 56                      ) | \
    (((uint64_t)(value) >> 40) & 0x000000000000FF00) | \
    (((uint64_t)(value) >> 24) & 0x0000000000FF0000) | \
    (((uint64_t)(value) >>  8) & 0x00000000FF000000) | \
    (((uint64_t)(value) <<  8) & 0x000000FF00000000) | \
    (((uint64_t)(value) << 24) & 0x0000FF0000000000) | \
    (((uint64_t)(value) << 40) & 0x00FF000000000000) | \
    ( (uint64_t)(value) << 56                      ))

// for constant value
#define MASM_BSWAP16(value) (       \
    (((value) shr 8) and 0x00FF) or \
    (((value) shl 8) and 0xFF00))

// for constant value
#define MASM_BSWAP24(value) (            \
    (((value) shr 16) and 0x000000FF) or \
    ( (value)         and 0x0000FF00) or \
    (((value) shl 16) and 0x00FF0000))

// for constant value
#define MASM_BSWAP32(value) (            \
    (((value) shr 24) and 0x000000FF) or \
    (((value) shr  8) and 0x0000FF00) or \
    (((value) shl  8) and 0x00FF0000) or \
    (((value) shl 24) and 0xFF000000))

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
unsigned __int64 __msreturn __fastcall __fastcall_byteswap_uint64(unsigned long high, unsigned long low);
#define _byteswap_uint64(val) __fastcall_byteswap_uint64((unsigned long)((unsigned __int64)(val) >> 32), (unsigned long)(val))
#else
__forceinline unsigned short _byteswap_ushort(unsigned short val) { return BSWAP16 (val); }
__forceinline unsigned long  _byteswap_ulong (unsigned long  val) { return BSWAP32 (val); }
__forceinline uint64_t       _byteswap_uint64(uint64_t       val) { return BSWAP64 (val); }
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define __intrinsic_bswap16 _byteswap_ushort
#define __intrinsic_bswap24(value) (_byteswap_ulong((uint32_t)(value)) >> 8)
#define __intrinsic_bswap32 _byteswap_ulong
#define __intrinsic_bswap40(value) (_byteswap_uint64((uint64_t)(value)) >> 24)
#define __intrinsic_bswap48(value) (_byteswap_uint64((uint64_t)(value)) >> 16)
#define __intrinsic_bswap56(value) (_byteswap_uint64((uint64_t)(value)) >> 8)
#define __intrinsic_bswap64 _byteswap_uint64
#else
__forceinline uint16_t __intrinsic_bswap16(uint16_t value) { return BSWAP16 (value); }
__forceinline uint32_t __intrinsic_bswap24(uint32_t value) { return BSWAP24 (value); }
__forceinline uint32_t __intrinsic_bswap32(uint32_t value) { return BSWAP32 (value); }
__forceinline uint64_t __intrinsic_bswap40(uint64_t value) { return BSWAP40 (value); }
__forceinline uint64_t __intrinsic_bswap48(uint64_t value) { return BSWAP48 (value); }
__forceinline uint64_t __intrinsic_bswap56(uint64_t value) { return BSWAP56 (value); }
__forceinline uint64_t __intrinsic_bswap64(uint64_t value) { return BSWAP64 (value); }
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_rotl8)
#pragma intrinsic(_rotr8)
#pragma intrinsic(_rotl16)
#pragma intrinsic(_rotr16)
#pragma intrinsic(_rotl)
#pragma intrinsic(_rotr)
#pragma intrinsic(_rotl64)
#pragma intrinsic(_rotr64)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned char _rotl8(unsigned char value, unsigned char shift)
{
	__asm
	{
		mov     al, byte ptr [value]
		mov     cl, byte ptr [shift]
		rol     al, cl
	}
}
__forceinline unsigned char _rotr8(unsigned char value, unsigned char shift)
{
	__asm
	{
		mov     al, byte ptr [value]
		mov     cl, byte ptr [shift]
		ror     al, cl
	}
}
__forceinline unsigned short _rotl16(unsigned short value, unsigned char shift)
{
	__asm
	{
		mov     ax, word ptr [value]
		mov     cl, byte ptr [shift]
		rol     ax, cl
	}
}
__forceinline unsigned short _rotr16(unsigned short value, unsigned char shift)
{
	__asm
	{
		mov     ax, word ptr [value]
		mov     cl, byte ptr [shift]
		ror     ax, cl
	}
}
__forceinline unsigned int _rotl(unsigned int value, int shift)
{
	__asm
	{
		mov     eax, dword ptr [value]
		mov     cl, byte ptr [shift]
		rol     eax, cl
	}
}
__forceinline unsigned int _rotr(unsigned int value, int shift)
{
	__asm
	{
		mov     eax, dword ptr [value]
		mov     cl, byte ptr [shift]
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
unsigned char __fastcall __fastcall_rotl8(unsigned char value, unsigned char shift);
unsigned char __fastcall __fastcall_rotr8(unsigned char value, unsigned char shift);
unsigned short __fastcall __fastcall_rotl16(unsigned short value, unsigned char shift);
unsigned short __fastcall __fastcall_rotr16(unsigned short value, unsigned char shift);
unsigned int __fastcall __fastcall_rotl(unsigned int value, int shift);
unsigned int __fastcall __fastcall_rotr(unsigned int value, int shift);
unsigned __int64 __msreturn __fastcall __fastcall_rotl64(uint32_t low, uint32_t high, int shift);
unsigned __int64 __msreturn __fastcall __fastcall_rotr64(uint32_t low, uint32_t high, int shift);
#define _rotl8 __fastcall_rotl8
#define _rotr8 __fastcall_rotr8
#define _rotl16 __fastcall_rotl16
#define _rotr16 __fastcall_rotr16
#define _rotl __fastcall_rotl
#define _rotr __fastcall_rotr
#define _rotl64(value, shift) __fastcall_rotl64((uint32_t)(value), (uint32_t)((uint64_t)(value) >> 32), shift)
#define _rotr64(value, shift) __fastcall_rotr64((uint32_t)(value), (uint32_t)((uint64_t)(value) >> 32), shift)
#else
__forceinline unsigned char _rotl8(unsigned char value, unsigned char shift)
{
	shift &= 7;
	return (value << shift) | (value >> (8 - shift));
}
__forceinline unsigned char _rotr8(unsigned char value, unsigned char shift)
{
	shift &= 7;
	return (value >> shift) | (value << (8 - shift));
}
__forceinline unsigned short _rotl16(unsigned short value, unsigned char shift)
{
	shift &= 15;
	return (value << shift) | (value >> (16 - shift));
}
__forceinline unsigned short _rotr16(unsigned short value, unsigned char shift)
{
	shift &= 15;
	return (value >> shift) | (value << (16 - shift));
}
__forceinline unsigned int _rotl(unsigned int value, int shift)
{
	shift &= 31;
	return (value << shift) | (value >> (32 - shift));
}
__forceinline unsigned int _rotr(unsigned int value, int shift)
{
	shift &= 31;
	return (value >> shift) | (value << (32 - shift));
}
__forceinline uint64_t _rotl64(uint64_t value, int shift)
{
	shift &= 63;
	return (value << shift) | (value >> (64 - shift));
}
__forceinline uint64_t _rotr64(uint64_t value, int shift)
{
	shift &= 63;
	return (value >> shift) | (value << (64 - shift));
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(__movsb)
#pragma intrinsic(__movsw)
#pragma intrinsic(__movsd)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
#define __movsb(Destination, Source, Count)                     \
do                                                              \
{                                                               \
    unsigned char *      __restrict _Destination = Destination; \
    const unsigned char *__restrict _Source      = Source;      \
    size_t                          _Count       = Count;       \
                                                                \
    __asm   mov     edi, dword ptr [_Destination]               \
    __asm   mov     esi, dword ptr [_Source]                    \
    __asm   mov     ecx, dword ptr [_Count]                     \
    __asm   rep movsb                                           \
} while (0)
#define __movsw(Destination, Source, Count)                      \
do                                                               \
{                                                                \
    unsigned short *      __restrict _Destination = Destination; \
    const unsigned short *__restrict _Source      = Source;      \
    size_t                           _Count       = Count;       \
                                                                 \
    __asm   mov     edi, dword ptr [_Destination]                \
    __asm   mov     esi, dword ptr [_Source]                     \
    __asm   mov     ecx, dword ptr [_Count]                      \
    __asm   rep movsw                                            \
} while (0)
#define __movsd(Destination, Source, Count)                     \
do                                                              \
{                                                               \
    unsigned long *      __restrict _Destination = Destination; \
    const unsigned long *__restrict _Source      = Source;      \
    size_t                          _Count       = Count;       \
                                                                \
    __asm   mov     edi, dword ptr [_Destination]               \
    __asm   mov     esi, dword ptr [_Source]                    \
    __asm   mov     ecx, dword ptr [_Count]                     \
    __asm   rep movsd                                           \
} while (0)
#elif defined(__BORLANDC__)
void __fastcall __movsb(unsigned char *Destination, const unsigned char *Source, size_t Count);
void __fastcall __movsw(unsigned short *Destination, const unsigned short *Source, size_t Count);
void __fastcall __movsd(unsigned long *Destination, const unsigned long *Source, size_t Count);
#else
#define __movsb memcpy
#define __movsw(Destination, Source, Count) memcpy(Destination, Source, (size_t)(Count) * 2)
#define __movsd(Destination, Source, Count) memcpy(Destination, Source, (size_t)(Count) * 4)
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#pragma intrinsic(__movsq)
#else
#define __movsq(Destination, Source, Count) __movsd(Destination, Source, (size_t)(Count) * 2)
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(__stosb)
#pragma intrinsic(__stosw)
#pragma intrinsic(__stosd)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
#define __stosb(Dest, Data, Count)            \
do                                            \
{                                             \
    unsigned char *__restrict _Dest  = Dest;  \
    unsigned char             _Data  = Data;  \
    size_t                    _Count = Count; \
                                              \
    __asm   mov     edi, dword ptr [_Dest]    \
    __asm   mov     al, byte ptr [_Data]      \
    __asm   mov     ecx, dword ptr [_Count]   \
    __asm   rep stosb                         \
} while (0)
#define __stosw(Dest, Data, Count)             \
do                                             \
{                                              \
    unsigned short *__restrict _Dest  = Dest;  \
    unsigned short             _Data  = Data;  \
    size_t                     _Count = Count; \
                                               \
    __asm   mov     edi, dword ptr [_Dest]     \
    __asm   mov     ax, word ptr [_Data]       \
    __asm   mov     ecx, dword ptr [_Count]    \
    __asm   rep stosw                          \
} while (0)
#define __stosd(Dest, Data, Count)            \
do                                            \
{                                             \
    unsigned long *__restrict _Dest  = Dest;  \
    unsigned long             _Data  = Data;  \
    size_t                    _Count = Count; \
                                              \
    __asm   mov     edi, dword ptr [_Dest]    \
    __asm   mov     eax, dword ptr [_Data]    \
    __asm   mov     ecx, dword ptr [_Count]   \
    __asm   rep stosd                         \
} while (0)
#elif defined(__BORLANDC__)
__declspec(naked) void __fastcall __fastcall_stosb(unsigned char Data, unsigned char *Dest, size_t Count);
__declspec(naked) void __fastcall __fastcall_stosw(unsigned short Data, unsigned short *Dest, size_t Count);
__declspec(naked) void __fastcall __fastcall_stosd(unsigned long Data, unsigned long *Dest, size_t Count);
#define __stosb(Dest, Data, Count) __fastcall_stosb(Data, Dest, Count)
#define __stosw(Dest, Data, Count) __fastcall_stosw(Data, Dest, Count)
#define __stosd(Dest, Data, Count) __fastcall_stosd(Data, Dest, Count)
#else
#define __stosb memset
#define __stosw(Dest, Data, Count)             \
do                                             \
{                                              \
    unsigned short *__restrict _Dest  = Dest;  \
    unsigned short             _Data  = Data;  \
    size_t                     _Count = Count; \
                                               \
    while (_Count--)                           \
        *(_Dest++) = _Data;                    \
} while (0)
#define __stosd(Dest, Data, Count)            \
do                                            \
{                                             \
    unsigned long *__restrict _Dest  = Dest;  \
    unsigned long             _Data  = Data;  \
    size_t                    _Count = Count; \
                                              \
    while (_Count--)                          \
        *(_Dest++) = _Data;                   \
} while (0)
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#pragma intrinsic(__stosq)
#else
#define __stosq(Dest, Data, Count)       \
do                                       \
{                                        \
    uint64_t *__restrict _Dest  = Dest;  \
    uint64_t             _Data  = Data;  \
    size_t               _Count = Count; \
                                         \
    while (_Count--)                     \
        *(_Dest++) = _Data;              \
} while (0)
#endif

// for constant value
#define _BSF8(x, default) ( \
    ((x) & 0x01) ? 0 :      \
    ((x) & 0x02) ? 1 :      \
    ((x) & 0x04) ? 2 :      \
    ((x) & 0x08) ? 3 :      \
    ((x) & 0x10) ? 4 :      \
    ((x) & 0x20) ? 5 :      \
    ((x) & 0x40) ? 6 :      \
    ((x) & 0x80) ? 7 :      \
    (default))
#define _BSF16(x, default) _BSF8(x, _BSF8((x) >> 8, (default) - 8) + 8)
#define _BSF32(x, default) _BSF16(x, _BSF16((x) >> 16, (default) - 16) + 16)
#define _BSF64(x, default) _BSF32(x, _BSF32((x) >> 32, (default) - 32) + 32)
#define BSF8(x) _BSF8(x, -1)
#define BSF16(x) _BSF16(x, -1)
#define BSF32(x) _BSF32(x, -1)
#define BSF64(x) _BSF64(x, -1)

// for constant value
#define MASM_BSF32(x) (                                      -1 + \
    (  (x)         and 1                                 )      + \
    ((((x) shr  1) and 1) and (((x) and 0x00000001) eq 0)) *  2 + \
    ((((x) shr  2) and 1) and (((x) and 0x00000003) eq 0)) *  3 + \
    ((((x) shr  3) and 1) and (((x) and 0x00000007) eq 0)) *  4 + \
    ((((x) shr  4) and 1) and (((x) and 0x0000000F) eq 0)) *  5 + \
    ((((x) shr  5) and 1) and (((x) and 0x0000001F) eq 0)) *  6 + \
    ((((x) shr  6) and 1) and (((x) and 0x0000003F) eq 0)) *  7 + \
    ((((x) shr  7) and 1) and (((x) and 0x0000007F) eq 0)) *  8 + \
    ((((x) shr  8) and 1) and (((x) and 0x000000FF) eq 0)) *  9 + \
    ((((x) shr  9) and 1) and (((x) and 0x000001FF) eq 0)) * 10 + \
    ((((x) shr 10) and 1) and (((x) and 0x000003FF) eq 0)) * 11 + \
    ((((x) shr 11) and 1) and (((x) and 0x000007FF) eq 0)) * 12 + \
    ((((x) shr 12) and 1) and (((x) and 0x00000FFF) eq 0)) * 13 + \
    ((((x) shr 13) and 1) and (((x) and 0x00001FFF) eq 0)) * 14 + \
    ((((x) shr 14) and 1) and (((x) and 0x00003FFF) eq 0)) * 15 + \
    ((((x) shr 15) and 1) and (((x) and 0x00007FFF) eq 0)) * 16 + \
    ((((x) shr 16) and 1) and (((x) and 0x0000FFFF) eq 0)) * 17 + \
    ((((x) shr 17) and 1) and (((x) and 0x0001FFFF) eq 0)) * 18 + \
    ((((x) shr 18) and 1) and (((x) and 0x0003FFFF) eq 0)) * 19 + \
    ((((x) shr 19) and 1) and (((x) and 0x0007FFFF) eq 0)) * 20 + \
    ((((x) shr 20) and 1) and (((x) and 0x000FFFFF) eq 0)) * 21 + \
    ((((x) shr 21) and 1) and (((x) and 0x001FFFFF) eq 0)) * 22 + \
    ((((x) shr 22) and 1) and (((x) and 0x003FFFFF) eq 0)) * 23 + \
    ((((x) shr 23) and 1) and (((x) and 0x007FFFFF) eq 0)) * 24 + \
    ((((x) shr 24) and 1) and (((x) and 0x00FFFFFF) eq 0)) * 25 + \
    ((((x) shr 25) and 1) and (((x) and 0x01FFFFFF) eq 0)) * 26 + \
    ((((x) shr 26) and 1) and (((x) and 0x03FFFFFF) eq 0)) * 27 + \
    ((((x) shr 27) and 1) and (((x) and 0x07FFFFFF) eq 0)) * 28 + \
    ((((x) shr 28) and 1) and (((x) and 0x0FFFFFFF) eq 0)) * 29 + \
    ((((x) shr 29) and 1) and (((x) and 0x1FFFFFFF) eq 0)) * 30 + \
    ((((x) shr 30) and 1) and (((x) and 0x3FFFFFFF) eq 0)) * 31 + \
    ((((x) shr 31) and 1) and (((x) and 0x7FFFFFFF) eq 0)) * 32)

// for constant value
#define _BSR8(x, default) ( \
    ((x) & 0x80) ? 7 :      \
    ((x) & 0x40) ? 6 :      \
    ((x) & 0x20) ? 5 :      \
    ((x) & 0x10) ? 4 :      \
    ((x) & 0x08) ? 3 :      \
    ((x) & 0x04) ? 2 :      \
    ((x) & 0x02) ? 1 :      \
    ((x) & 0x01) ? 0 :      \
    (default))
#define _BSR16(x, default) (_BSR8((uint16_t)(x) >> 8, _BSR8(x, default) - 8) + 8)
#define _BSR32(x, default) (_BSR16((uint32_t)(x) >> 16, _BSR16(x, default) - 16) + 16)
#define _BSR64(x, default) (_BSR32((uint64_t)(x) >> 32, _BSR32(x, default) - 32) + 32)
#define BSR8(x) _BSR8(x, -1)
#define BSR16(x) _BSR16(x, -1)
#define BSR32(x) _BSR32(x, -1)
#define BSR64(x) _BSR64(x, -1)

// for constant value
#define MASM_BSR32(x) (                                      -1 + \
    ( ((x) shr 31) and 1                                 ) * 32 + \
    ((((x) shr 30) and 1) and (((x) and 0x80000000) eq 0)) * 31 + \
    ((((x) shr 29) and 1) and (((x) and 0xC0000000) eq 0)) * 30 + \
    ((((x) shr 28) and 1) and (((x) and 0xE0000000) eq 0)) * 29 + \
    ((((x) shr 27) and 1) and (((x) and 0xF0000000) eq 0)) * 28 + \
    ((((x) shr 26) and 1) and (((x) and 0xF8000000) eq 0)) * 27 + \
    ((((x) shr 25) and 1) and (((x) and 0xFC000000) eq 0)) * 26 + \
    ((((x) shr 24) and 1) and (((x) and 0xFE000000) eq 0)) * 25 + \
    ((((x) shr 23) and 1) and (((x) and 0xFF000000) eq 0)) * 24 + \
    ((((x) shr 22) and 1) and (((x) and 0xFF800000) eq 0)) * 23 + \
    ((((x) shr 21) and 1) and (((x) and 0xFFC00000) eq 0)) * 22 + \
    ((((x) shr 20) and 1) and (((x) and 0xFFE00000) eq 0)) * 21 + \
    ((((x) shr 19) and 1) and (((x) and 0xFFF00000) eq 0)) * 20 + \
    ((((x) shr 18) and 1) and (((x) and 0xFFF80000) eq 0)) * 19 + \
    ((((x) shr 17) and 1) and (((x) and 0xFFFC0000) eq 0)) * 18 + \
    ((((x) shr 16) and 1) and (((x) and 0xFFFE0000) eq 0)) * 17 + \
    ((((x) shr 15) and 1) and (((x) and 0xFFFF0000) eq 0)) * 16 + \
    ((((x) shr 14) and 1) and (((x) and 0xFFFF8000) eq 0)) * 15 + \
    ((((x) shr 13) and 1) and (((x) and 0xFFFFC000) eq 0)) * 14 + \
    ((((x) shr 12) and 1) and (((x) and 0xFFFFE000) eq 0)) * 13 + \
    ((((x) shr 11) and 1) and (((x) and 0xFFFFF000) eq 0)) * 12 + \
    ((((x) shr 10) and 1) and (((x) and 0xFFFFF800) eq 0)) * 11 + \
    ((((x) shr  9) and 1) and (((x) and 0xFFFFFC00) eq 0)) * 10 + \
    ((((x) shr  8) and 1) and (((x) and 0xFFFFFE00) eq 0)) *  9 + \
    ((((x) shr  7) and 1) and (((x) and 0xFFFFFF00) eq 0)) *  8 + \
    ((((x) shr  6) and 1) and (((x) and 0xFFFFFF80) eq 0)) *  7 + \
    ((((x) shr  5) and 1) and (((x) and 0xFFFFFFC0) eq 0)) *  6 + \
    ((((x) shr  4) and 1) and (((x) and 0xFFFFFFE0) eq 0)) *  5 + \
    ((((x) shr  3) and 1) and (((x) and 0xFFFFFFF0) eq 0)) *  4 + \
    ((((x) shr  2) and 1) and (((x) and 0xFFFFFFF8) eq 0)) *  3 + \
    ((((x) shr  1) and 1) and (((x) and 0xFFFFFFFC) eq 0)) *  2 + \
    (( (x)         and 1) and (((x) and 0xFFFFFFFE) eq 0)))

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __reg64return_BitScanForward(unsigned long Mask)
{
	__asm
	{
		bsf     edx, dword ptr [Mask]
		setnz   al
	}
}
__forceinline unsigned char _BitScanForward(unsigned long *Index, unsigned long Mask)
{
	unsigned __int64 x = __reg64return_BitScanForward(Mask);
	*Index = (unsigned long)(x >> 32);
	return (unsigned char)x;
}
__forceinline unsigned __int64 __reg64return_BitScanReverse(unsigned long Mask)
{
	__asm
	{
		bsr     edx, dword ptr [Mask]
		setnz   al
	}
}
__forceinline unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	unsigned __int64 x = __reg64return_BitScanReverse(Mask);
	*Index = (unsigned long)(x >> 32);
	return (unsigned char)x;
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _BitScanForward(unsigned long *Index, unsigned long Mask);
unsigned char __fastcall _BitScanReverse(unsigned long *Index, unsigned long Mask);
#else
__forceinline unsigned char _BitScanForward(unsigned long *Index, unsigned long Mask)
{
	unsigned long i;

	if (!Mask)
		return 0;
	for (i = 0; !(Mask & 1); Mask >>= 1)
		i++;
	*Index = i;
	return 1;
}
__forceinline unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	unsigned long i;

	if (!Mask)
		return 0;
	for (i = 31; (long)Mask >= 0; Mask <<= 1)
		i--;
	*Index = i;
	return 1;
}
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __reg64return_BitScanForward64(unsigned __int64 Mask)
{
	__asm
	{
		bsf     edx, dword ptr [Mask]
		jnz     L1
		bsf     edx, dword ptr [Mask + 4]
		lea     edx, [edx + 32]
	L1:
		setnz   al
	}
}
__forceinline unsigned char _BitScanForward64(unsigned long *Index, unsigned __int64 Mask)
{
	unsigned __int64 x = __reg64return_BitScanForward64(Mask);
	*Index = (unsigned long)(x >> 32);
	return (unsigned char)x;
}
__forceinline unsigned __int64 __reg64return_BitScanReverse64(unsigned __int64 Mask)
{
	__asm
	{
		bsr     edx, dword ptr [Mask + 4]
		lea     edx, [edx + 32]
		jnz     L1
		bsr     edx, dword ptr [Mask]
	L1:
		setnz   al
	}
}
__forceinline unsigned char _BitScanReverse64(unsigned long *Index, unsigned __int64 Mask)
{
	unsigned __int64 x = __reg64return_BitScanReverse64(Mask);
	*Index = (unsigned long)(x >> 32);
	return (unsigned char)x;
}
#elif defined(__BORLANDC__)
unsigned char __fastcall __fastcall_BitScanForward64(uint32_t low, uint32_t high, unsigned long *Index);
unsigned char __fastcall __fastcall_BitScanReverse64(uint32_t low, uint32_t high, unsigned long *Index);
#define _BitScanForward64(Index, Mask) __fastcall_BitScanForward64((uint32_t)(Mask), (uint32_t)((uint64_t)(Mask) >> 32), Index)
#define _BitScanReverse64(Index, Mask) __fastcall_BitScanReverse64((uint32_t)(Mask), (uint32_t)((uint64_t)(Mask) >> 32), Index)
#else
__forceinline unsigned char _BitScanForward64(unsigned long *Index, uint64_t Mask)
{
	if (!_BitScanForward(Index, (unsigned long)Mask))
	{
		if (!_BitScanForward(Index, (unsigned long)(Mask >> 32)))
			return 0;
		*Index += 32;
	}
	return 1;
}
__forceinline unsigned char _BitScanReverse64(unsigned long *Index, uint64_t Mask)
{
	if (!_BitScanReverse(Index, (unsigned long)(Mask >> 32)))
	{
		if (!_BitScanReverse(Index, (unsigned long)Mask))
			return 0;
	}
	else
	{
		*Index += 32;
	}
	return 1;
}
#endif

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
#pragma intrinsic(__ll_lshift)
#else
#define __ll_lshift(Mask, nBit) ((uint64_t)(Mask) << ((nBit) & 31))
#endif

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
#pragma intrinsic(__ll_rshift)
#else
#define __ll_rshift(Mask, nBit) ((int64_t)(Mask) >> ((nBit) & 31))
#endif

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
#pragma intrinsic(__ull_rshift)
#else
#define __ull_rshift(Mask, nBit) ((uint64_t)(Mask) >> ((nBit) & 31))
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#pragma intrinsic(__shiftleft128)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __shiftleft128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift)
{
	__asm
	{
		mov     cl, byte ptr [Shift]
		mov     edx, dword ptr [HighPart]
		and     cl, 63
		jz      L1
		sub     cl, 32
		jb      L2
		mov     eax, dword ptr [LowPart + 4]
		jz      L4
		mov     edi, dword ptr [LowPart]
		jmp     L3

	L1:
		mov     eax, edx
		mov     edx, dword ptr [HighPart + 4]
		jmp     L4

	L2:
		add     cl, 32
		mov     eax, edx
		mov     edx, dword ptr [HighPart + 4]
		mov     edi, dword ptr [LowPart + 4]
	L3:
		shld    edx, eax, cl
		shld    eax, edi, cl
	L4:
	}
}
#elif defined(__BORLANDC__)
unsigned __int64 __msreturn __msfastcall __shiftleft128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift);
#elif defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_M_IX86)
#pragma intrinsic(__ll_lshift)
__forceinline unsigned __int64 __shiftleft128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift)
{
	if (Shift &= 63)
		if (Shift < 32)
			HighPart = ((__ll_lshift(HighPart, Shift) >> 32) << 32) | (__ll_lshift((LowPart >> 32) | (HighPart << 32), Shift) >> 32);
		else if (Shift &= 31)
			HighPart = ((__ll_lshift((LowPart >> 32) | (HighPart << 32), Shift) >> 32) << 32) | (__ll_lshift(LowPart, Shift) >> 32);
		else
			HighPart = (LowPart >> 32) | (HighPart << 32);
	return HighPart;
}
#else
__forceinline uint64_t __shiftleft128(uint64_t LowPart, uint64_t HighPart, unsigned char Shift)
{
	if (Shift &= 63)
		HighPart = (HighPart << Shift) | (LowPart >> (64 - Shift));
	return HighPart;
}
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#pragma intrinsic(__shiftright128)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __shiftright128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift)
{
	__asm
	{
		mov     cl, byte ptr [Shift]
		mov     eax, dword ptr [LowPart + 4]
		and     cl, 63
		jz      L1
		sub     cl, 32
		jb      L2
		mov     edx, dword ptr [HighPart]
		jz      L4
		mov     edi, dword ptr [HighPart + 4]
		jmp     L3

	L1:
		mov     edx, eax
		mov     eax, dword ptr [LowPart]
		jmp     L4

	L2:
		add     cl, 32
		mov     edx, eax
		mov     eax, dword ptr [LowPart]
		mov     edi, dword ptr [HighPart]
	L3:
		shrd    eax, edx, cl
		shrd    edx, edi, cl
	L4:
	}
}
#elif defined(__BORLANDC__)
unsigned __int64 __msreturn __msfastcall __shiftright128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift);
#elif defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_M_IX86)
#pragma intrinsic(__ull_rshift)
__forceinline unsigned __int64 __shiftright128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift)
{
	if (Shift &= 63)
		if (Shift < 32)
			LowPart = (uint32_t)__ull_rshift(LowPart, Shift) | (__ull_rshift((LowPart >> 32) | (HighPart << 32), Shift) << 32);
		else if (Shift &= 31)
			LowPart = (uint32_t)__ull_rshift((LowPart >> 32) | (HighPart << 32), Shift) | (__ull_rshift(HighPart, Shift) << 32);
		else
			LowPart = (LowPart >> 32) | (HighPart << 32);
	return LowPart;
}
#else
__forceinline uint64_t __shiftright128(uint64_t LowPart, uint64_t HighPart, unsigned char Shift)
{
	if (Shift &= 63)
		LowPart = (LowPart >> Shift) | (HighPart << (64 - Shift));
	return LowPart;
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_addcarry_u32)
#define _add_u32(a, b, out) _addcarry_u32(0, a, b, out)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __reg64return_add_u32(unsigned int a, unsigned int b)
{
	__asm
	{
		mov     edx, dword ptr [a]
		mov     eax, dword ptr [b]
		add     edx, eax
		setc    al
	}
}
__forceinline unsigned char _add_u32(unsigned int a, unsigned int b, unsigned int *_out)
{
	unsigned __int64 x = __reg64return_add_u32(a, b);
	*_out = (unsigned int)(x >> 32);
	return (unsigned char)x;
}
__forceinline unsigned __int64 __reg64return_addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b)
{
	__asm
	{
		mov     al, byte ptr [c_in]
		mov     edx, dword ptr [a]
		mov     ecx, dword ptr [b]
		add     al, -1
		adc     edx, ecx
		setc    al
	}
}
__forceinline unsigned char _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *_out)
{
	unsigned __int64 x = __reg64return_addcarry_u32(c_in, a, b);
	*_out = (unsigned int)(x >> 32);
	return (unsigned char)x;
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _add_u32(unsigned int a, unsigned int b, unsigned int *_out);
unsigned char __fastcall _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *_out);
#else
__forceinline unsigned char _add_u32(unsigned int a, unsigned int b, unsigned int *_out)
{
	return (*_out = a + b) < b;
}
__forceinline unsigned char _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *_out)
{
	return ((*_out = a + b) < b) | (c_in && !++(*_out));
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_subborrow_u32)
#define _sub_u32(a, b, out) _subborrow_u32(0, a, b, out)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __reg64return_sub_u32(unsigned int a, unsigned int b)
{
	__asm
	{
		mov     edx, dword ptr [a]
		mov     eax, dword ptr [b]
		sub     edx, eax
		setc    al
	}
}
__forceinline unsigned char _sub_u32(unsigned int a, unsigned int b, unsigned int *_out)
{
	unsigned __int64 x = __reg64return_sub_u32(a, b);
	*_out = (unsigned int)(x >> 32);
	return (unsigned char)x;
}
__forceinline unsigned __int64 __reg64return_subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b)
{
	__asm
	{
		mov     al, byte ptr [b_in]
		mov     edx, dword ptr [a]
		mov     ecx, dword ptr [b]
		add     al, -1
		sbb     edx, ecx
		setc    al
	}
}
__forceinline unsigned char _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *_out)
{
	unsigned __int64 x = __reg64return_subborrow_u32(b_in, a, b);
	*_out = (unsigned int)(x >> 32);
	return (unsigned char)x;
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _sub_u32(unsigned int a, unsigned int b, unsigned int *_out);
unsigned char __fastcall _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *_out);
#else
__forceinline unsigned char _sub_u32(unsigned int a, unsigned int b, unsigned int *_out)
{
	return (*_out = a - b) > a;
}
__forceinline unsigned char _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *_out)
{
	return ((*_out = a - b) > a) | (b_in && !(*_out)--);
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_WIN64)
#pragma intrinsic(_addcarry_u64)
#define _add_u64(a, b, out) _addcarry_u64(0, a, b, out)
#elif defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_addcarry_u32)
__forceinline unsigned char _add_u64(uint64_t a, uint64_t b, uint64_t *_out)
{
	return
		_addcarry_u32(
			_addcarry_u32(
				0,
				(uint32_t)a,
				(uint32_t)b,
				(uint32_t*)_out),
			(uint32_t)(a >> 32),
			(uint32_t)(b >> 32),
			(uint32_t *)_out + 1);
}
__forceinline unsigned char _addcarry_u64(unsigned char c_in, uint64_t a, uint64_t b, uint64_t *_out)
{
	return
		_addcarry_u32(
			_addcarry_u32(
				c_in,
				(uint32_t)a,
				(uint32_t)b,
				(uint32_t *)_out),
			(uint32_t)(a >> 32),
			(uint32_t)(b >> 32),
			(uint32_t *)_out + 1);
}
#elif defined(_MSC_VER) && defined(_M_IX86)
__forceinline unsigned char _add_u64(uint64_t a, uint64_t b, uint64_t *_out)
{
	__asm
	{
		mov     eax, dword ptr [a]
		mov     edx, dword ptr [a + 4]
		mov     ecx, dword ptr [b]
		add     eax, ecx
		mov     ecx, dword ptr [b + 4]
		adc     edx, ecx
		mov     ecx, dword ptr [_out]
		mov     dword ptr [ecx], eax
		mov     dword ptr [ecx + 4], edx
		setc    al
	}
}
__forceinline unsigned char _addcarry_u64(unsigned char c_in, uint64_t a, uint64_t b, uint64_t *_out)
{
	__asm
	{
		mov     cl, byte ptr [c_in]
		mov     eax, dword ptr [a]
		add     cl, -1
		mov     edx, dword ptr [a + 4]
		mov     ecx, dword ptr [b]
		adc     eax, ecx
		mov     ecx, dword ptr [b + 4]
		adc     edx, ecx
		mov     ecx, dword ptr [_out]
		mov     dword ptr [ecx], eax
		mov     dword ptr [ecx + 4], edx
		setc    al
	}
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _add_u64(uint64_t a, uint64_t b, uint64_t *_out);
unsigned char __fastcall _addcarry_u64(unsigned char c_in, uint64_t a, uint64_t b, uint64_t *_out);
#else
__forceinline unsigned char _add_u64(uint64_t a, uint64_t b, uint64_t *_out)
{
	return (*_out = a + b) < b;
}
__forceinline unsigned char _addcarry_u64(unsigned char c_in, uint64_t a, uint64_t b, uint64_t *_out)
{
	return ((*_out = a + b) < b) | (c_in && !++(*_out));
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_WIN64)
#pragma intrinsic(_subborrow_u64)
#define _sub_u64(a, b, out) _subborrow_u64(0, a, b, out)
#elif defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_subborrow_u32)
__forceinline unsigned char _sub_u64(uint64_t a, uint64_t b, uint64_t *_out)
{
	return
		_subborrow_u32(
			_subborrow_u32(
				0,
				(uint32_t)a,
				(uint32_t)b,
				(uint32_t *)_out),
			(uint32_t)(a >> 32),
			(uint32_t)(b >> 32),
			(uint32_t *)_out + 1);
}
__forceinline unsigned char _subborrow_u64(unsigned char b_in, uint64_t a, uint64_t b, uint64_t *_out)
{
	return
		_subborrow_u32(
			_subborrow_u32(
				b_in,
				(uint32_t)a,
				(uint32_t)b,
				(uint32_t *)_out),
			(uint32_t)(a >> 32),
			(uint32_t)(b >> 32),
			(uint32_t *)_out + 1);
}
#elif defined(_MSC_VER) && defined(_M_IX86)
__forceinline unsigned char _sub_u64(uint64_t a, uint64_t b, uint64_t *_out)
{
	__asm
	{
		mov     eax, dword ptr [a]
		mov     edx, dword ptr [a + 4]
		mov     ecx, dword ptr [b]
		sub     eax, ecx
		mov     ecx, dword ptr [b + 4]
		sbb     edx, ecx
		mov     ecx, dword ptr [_out]
		mov     dword ptr [ecx], eax
		mov     dword ptr [ecx + 4], edx
		setc    al
	}
}
__forceinline unsigned char _subborrow_u64(unsigned char b_in, uint64_t a, uint64_t b, uint64_t *_out)
{
	__asm
	{
		mov     cl, byte ptr [b_in]
		mov     eax, dword ptr [a]
		add     cl, -1
		mov     edx, dword ptr [a + 4]
		mov     ecx, dword ptr [b]
		sbb     eax, ecx
		mov     ecx, dword ptr [b + 4]
		sbb     edx, ecx
		mov     ecx, dword ptr [_out]
		mov     dword ptr [ecx], eax
		mov     dword ptr [ecx + 4], edx
		setc    al
	}
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _sub_u64(uint64_t a, uint64_t b, uint64_t *_out);
unsigned char __fastcall _subborrow_u64(unsigned char b_in, uint64_t a, uint64_t b, uint64_t *_out);
#else
__forceinline unsigned char _sub_u64(uint64_t a, uint64_t b, uint64_t *_out)
{
	return (*_out = a - b) > a;
}
__forceinline unsigned char _subborrow_u64(unsigned char b_in, uint64_t a, uint64_t b, uint64_t *_out)
{
	return ((*_out = a - b) > a) | (b_in && !(*_out)--);
}
#endif

#ifndef _WIN64
#define _add_uintptr       _add_u32
#define _addcarry_uintptr  _addcarry_u32
#define _sub_uintptr       _sub_u32
#define _subborrow_uintptr _subborrow_u32
#else
#define _add_uintptr       _add_u64
#define _addcarry_uintptr  _addcarry_u64
#define _sub_uintptr       _sub_u64
#define _subborrow_uintptr _subborrow_u64
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(__emul)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline __int64 __emul(int a, int b)
{
	__asm
	{
		mov     eax, dword ptr [a]
		mov     edx, dword ptr [b]
		imul    edx
	}
}
#elif defined(__BORLANDC__)
__int64 __msreturn __fastcall __emul(int a, int b);
#else
#define __emul(a, b) ((int64_t)(int)(a) * (int)(b))
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(__emulu)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned __int64 __emulu(unsigned int a, unsigned int b)
{
	__asm
	{
		mov     eax, dword ptr [a]
		mov     edx, dword ptr [b]
		mul     edx
	}
}
#elif defined(__BORLANDC__)
unsigned __int64 __msreturn __fastcall __emulu(unsigned int a, unsigned int b);
#else
#define __emulu(a, b) ((uint64_t)(unsigned int)(a) * (unsigned int)(b))
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#pragma intrinsic(__mulh)
#pragma intrinsic(__umulh)
#pragma intrinsic(_mul128)
#pragma intrinsic(_umul128)
#define MULH(Multiplicand, Multiplier, HighProduct) \
	do { *(HighProduct) = __mulh(Multiplicand, Multiplier); } while (0)
#define UMULH(Multiplicand, Multiplier, HighProduct) \
	do { *(HighProduct) = __umulh(Multiplicand, Multiplier); } while (0)
#define MUL128(Multiplicand, Multiplier, LowProduct, HighProduct) \
	do { *(LowProduct) = _mul128(Multiplicand, Multiplier, HighProduct); } while (0)
#define UMUL128(Multiplicand, Multiplier, LowProduct, HighProduct) \
	do { *(LowProduct) = _umul128(Multiplicand, Multiplier, HighProduct); } while (0)
#else
int64_t __msreturn __stdcall __mulh(int64_t Multiplicand, int64_t Multiplier);
uint64_t __msreturn __stdcall __umulh(uint64_t Multiplicand, uint64_t Multiplier);
int64_t __msreturn __stdcall _mul128(int64_t Multiplicand, int64_t Multiplier, int64_t *HighProduct);
uint64_t __msreturn __stdcall _umul128(uint64_t Multiplicand, uint64_t Multiplier, uint64_t *HighProduct);
#define UMULH(Multiplicand, Multiplier, HighProduct)                                              \
do                                                                                                \
{                                                                                                 \
    uint64_t _Multiplicand = Multiplicand;                                                        \
    uint64_t _Multiplier   = Multiplier;                                                          \
                                                                                                  \
    uint64_t x, y;                                                                                \
                                                                                                  \
    x = (__emulu((uint32_t)_Multiplicand, (uint32_t)_Multiplier) >> 32) +                         \
        __emulu((uint32_t)(_Multiplicand >> 32), (uint32_t)_Multiplier);                          \
    y = __emulu((uint32_t)_Multiplicand, (uint32_t)(_Multiplier >> 32));                          \
    *((uint32_t *)&x + 1) = _addcarry_u32(                                                        \
            _add_u32(                                                                             \
                (uint32_t)x,                                                                      \
                (uint32_t)y,                                                                      \
                (uint32_t *)&y),                                                                  \
            (uint32_t)(x >> 32),                                                                  \
            (uint32_t)(y >> 32),                                                                  \
            (uint32_t *)&x);                                                                      \
    *(HighProduct) = x + __emulu((uint32_t)(_Multiplicand >> 32), (uint32_t)(_Multiplier >> 32)); \
} while (0)
#define UMUL128(Multiplicand, Multiplier, LowProduct, HighProduct)                            \
do                                                                                            \
{                                                                                             \
    uint64_t             _Multiplicand = Multiplicand;                                        \
    uint64_t             _Multiplier   = Multiplier;                                          \
    uint64_t *__restrict _LowProduct   = LowProduct;                                          \
    uint64_t *__restrict _HighProduct  = HighProduct;                                         \
                                                                                              \
    uint64_t x, y;                                                                            \
                                                                                              \
    x = __emulu((uint32_t)_Multiplicand, (uint32_t)_Multiplier);                              \
    *(uint32_t *)_LowProduct = (uint32_t)x;                                                   \
    x = (x >> 32) + __emulu((uint32_t)(_Multiplicand >> 32), (uint32_t)_Multiplier);          \
    y = __emulu((uint32_t)_Multiplicand, (uint32_t)(_Multiplier >> 32));                      \
    *((uint32_t *)_HighProduct + 1) = _addcarry_u32(                                          \
            _add_u32(                                                                         \
                (uint32_t)x,                                                                  \
                (uint32_t)y,                                                                  \
                (uint32_t *)_LowProduct + 1),                                                 \
            (uint32_t)(x >> 32),                                                              \
            (uint32_t)(y >> 32),                                                              \
            (uint32_t *)_HighProduct);                                                        \
    *_HighProduct += __emulu((uint32_t)(_Multiplicand >> 32), (uint32_t)(_Multiplier >> 32)); \
} while (0)
#ifndef _WIN64
#define MULH(Multiplicand, Multiplier, HighProduct)                    \
do                                                                     \
{                                                                      \
    int64_t             __Multiplicand = Multiplicand;                 \
    int64_t             __Multiplier   = Multiplier;                   \
    int64_t *__restrict __HighProduct  = HighProduct;                  \
                                                                       \
    uint64_t LowProduct;                                               \
    uint32_t sign, x;                                                  \
                                                                       \
    sign = (uint32_t)(__Multiplicand >> 63);                           \
    __Multiplicand ^= sign | ((uint64_t)sign << 32);                   \
    __Multiplicand -= sign | ((uint64_t)sign << 32);                   \
    sign ^= x = (uint32_t)(__Multiplier >> 63);                        \
    __Multiplier ^= x | ((uint64_t)x << 32);                           \
    __Multiplier -= x | ((uint64_t)x << 32);                           \
    UMUL128(__Multiplicand, __Multiplier, &LowProduct, __HighProduct); \
    LowProduct ^= sign | ((uint64_t)sign << 32);                       \
    *__HighProduct ^= sign | ((uint64_t)sign << 32);                   \
    _subborrow_u64(                                                    \
        _sub_u64(                                                      \
            LowProduct,                                                \
            sign | ((uint64_t)sign << 32),                             \
            &LowProduct),                                              \
        *__HighProduct,                                                \
        sign | ((uint64_t)sign << 32),                                 \
        (uint64_t *)__HighProduct);                                    \
} while (0)
#else
#define MULH(Multiplicand, Multiplier, HighProduct)                    \
do                                                                     \
{                                                                      \
    int64_t             __Multiplicand = Multiplicand;                 \
    int64_t             __Multiplier   = Multiplier;                   \
    int64_t *__restrict __HighProduct  = HighProduct;                  \
                                                                       \
    uint64_t LowProduct, sign, x;                                      \
                                                                       \
    sign = __Multiplicand >> 63;                                       \
    __Multiplicand ^= sign;                                            \
    __Multiplicand -= sign;                                            \
    sign ^= x = __Multiplier >> 63;                                    \
    __Multiplier ^= x;                                                 \
    __Multiplier -= x;                                                 \
    UMUL128(__Multiplicand, __Multiplier, &LowProduct, __HighProduct); \
    LowProduct ^= sign;                                                \
    *__HighProduct ^= sign;                                            \
    _subborrow_u64(                                                    \
        _sub_u64(                                                      \
            LowProduct,                                                \
            sign,                                                      \
            &LowProduct),                                              \
        *__HighProduct,                                                \
        sign,                                                          \
        (uint64_t *)__HighProduct);                                    \
} while (0)
#endif
#ifndef _WIN64
#define MUL128(Multiplicand, Multiplier, LowProduct, HighProduct)       \
do                                                                      \
{                                                                       \
    int64_t             __Multiplicand = Multiplicand;                  \
    int64_t             __Multiplier   = Multiplier;                    \
    int64_t *__restrict __LowProduct   = LowProduct;                    \
    int64_t *__restrict __HighProduct  = HighProduct;                   \
                                                                        \
    uint32_t sign, x;                                                   \
                                                                        \
    sign = (uint32_t)(__Multiplicand >> 63);                            \
    __Multiplicand ^= sign | ((uint64_t)sign << 32);                    \
    __Multiplicand -= sign | ((uint64_t)sign << 32);                    \
    sign ^= x = (uint32_t)(__Multiplier >> 63);                         \
    __Multiplier ^= x | ((uint64_t)x << 32);                            \
    __Multiplier -= x | ((uint64_t)x << 32);                            \
    UMUL128(__Multiplicand, __Multiplier, __LowProduct, __HighProduct); \
    *__LowProduct ^= sign | ((uint64_t)sign << 32);                     \
    *__HighProduct ^= sign | ((uint64_t)sign << 32);                    \
    _subborrow_u64(                                                     \
        _sub_u64(                                                       \
            *__LowProduct,                                              \
            sign | ((uint64_t)sign << 32),                              \
            (uint64_t *)__LowProduct),                                  \
        *__HighProduct,                                                 \
        sign | ((uint64_t)sign << 32),                                  \
        (uint64_t *)__HighProduct);                                     \
} while (0)
#else
#define MUL128(Multiplicand, Multiplier, LowProduct, HighProduct)       \
do                                                                      \
{                                                                       \
    int64_t             __Multiplicand = Multiplicand;                  \
    int64_t             __Multiplier   = Multiplier;                    \
    int64_t *__restrict __LowProduct   = LowProduct;                    \
    int64_t *__restrict __HighProduct  = HighProduct;                   \
                                                                        \
    uint64_t sign, x;                                                   \
                                                                        \
    sign = __Multiplicand >> 63;                                        \
    __Multiplicand ^= sign;                                             \
    __Multiplicand -= sign;                                             \
    sign ^= x = __Multiplier >> 63;                                     \
    __Multiplier ^= x;                                                  \
    __Multiplier -= x;                                                  \
    UMUL128(__Multiplicand, __Multiplier, __LowProduct, __HighProduct); \
    *__LowProduct ^= sign;                                              \
    *__HighProduct ^= sign;                                             \
    _subborrow_u64(                                                     \
        _sub_u64(                                                       \
            *__LowProduct,                                              \
            sign,                                                       \
            (uint64_t *)__LowProduct),                                  \
        *__HighProduct,                                                 \
        sign,                                                           \
        (uint64_t *)__HighProduct);                                     \
} while (0)
#endif
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1920
#pragma intrinsic(_div64)
#pragma intrinsic(_udiv64)
#elif defined(_MSC_VER) && _MSC_VER < 1920 && defined(_M_IX86)
__forceinline unsigned __int64 __reg64return_div64(__int64 dividend, int divisor)
{
	__asm
	{
		mov     eax, dword ptr [dividend]
		mov     edx, dword ptr [dividend + 4]
		idiv    dword ptr [divisor]
	}
}
__forceinline int _div64(__int64 dividend, int divisor, int *remainder)
{
	unsigned __int64 x = __reg64return_div64(dividend, divisor);
	*remainder = (int)(x >> 32);
	return (int)x;
}
__forceinline unsigned __int64 __reg64return_udiv64(unsigned __int64 dividend, unsigned int divisor)
{
	__asm
	{
		mov     eax, dword ptr [dividend]
		mov     edx, dword ptr [dividend + 4]
		div     dword ptr [divisor]
	}
}
__forceinline unsigned int _udiv64(unsigned __int64 dividend, unsigned int divisor, unsigned int *remainder)
{
	unsigned __int64 x = __reg64return_udiv64(dividend, divisor);
	*remainder = (unsigned int)(x >> 32);
	return (unsigned int)x;
}
#elif defined(__BORLANDC__)
int __fastcall __fastcall_dvrm64(int32_t low, int32_t high, int divisor, int *remainder);
int __fastcall __fastcall_div64(int32_t low, int32_t high, int divisor);
unsigned int __fastcall __fastcall_udvrm64(uint32_t low, uint32_t high, unsigned int divisor, unsigned int *remainder);
unsigned int __fastcall __fastcall_udiv64(uint32_t low, uint32_t high, unsigned int divisor);
#define _div64(dividend, divisor, remainder) __fastcall_dvrm64((int32_t)(dividend), (int32_t)((uint64_t)(int64_t)(dividend) >> 32), divisor, remainder)
#define _udiv64(dividend, divisor, remainder) __fastcall_udvrm64((uint32_t)(dividend), (uint32_t)((uint64_t)(dividend) >> 32), divisor, remainder)
#else
__forceinline int _div64(int64_t dividend, int divisor, int *remainder)
{
	*remainder = dividend % divisor;
	return (int)(dividend / divisor);
}
__forceinline unsigned int _udiv64(uint64_t dividend, unsigned int divisor, unsigned int *remainder)
{
	*remainder = dividend % divisor;
	return (unsigned int)(dividend / divisor);
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1920 && defined(_M_X64)
#pragma intrinsic(_div128)
#pragma intrinsic(_udiv128)
#define DIV128(highDividend, lowDividend, divisor, quotient, remainder) \
	do { *(quotient) = _div128(highDividend, lowDividend, divisor, remainder); } while (0)
#define UDIV128(highDividend, lowDividend, divisor, quotient, remainder) \
	do { *(quotient) = _udiv128(highDividend, lowDividend, divisor, remainder); } while (0)
#else
int64_t __msreturn __stdcall _div128(int64_t highDividend, int64_t lowDividend, int64_t divisor, int64_t *remainder);
uint64_t __msreturn __stdcall _udiv128(uint64_t highDividend, uint64_t lowDividend, uint64_t divisor, uint64_t *remainder);
#define UDIV128(highDividend, lowDividend, divisor, quotient, remainder)       \
do                                                                             \
{                                                                              \
    uint64_t             _highDividend = highDividend;                         \
    uint64_t             _lowDividend  = lowDividend;                          \
    uint64_t             _divisor      = divisor;                              \
    uint64_t *__restrict _quotient     = quotient;                             \
    uint64_t *__restrict _remainder    = remainder;                            \
                                                                               \
    *_quotient = 0;                                                            \
    if (_highDividend && (_highDividend %= _divisor))                          \
    {                                                                          \
        unsigned long shift, index;                                            \
        uint64_t addend, lowSubtrahend, highSubtrahend, low, high;             \
                                                                               \
        _BitScanReverse64(&shift, _divisor);                                   \
        _BitScanReverse64(&index, _highDividend);                              \
        addend = 0x8000000000000000;                                           \
        if (shift -= index)                                                    \
            addend >>= shift - 1;                                              \
        else                                                                   \
            shift++;                                                           \
        lowSubtrahend = __shiftright128(0, _divisor, (unsigned char)shift);    \
        highSubtrahend = _divisor >> shift;                                    \
        for (; ;                                                               \
            addend >>= 1,                                                      \
            lowSubtrahend = __shiftright128(lowSubtrahend, highSubtrahend, 1), \
            highSubtrahend >>= 1)                                              \
        {                                                                      \
            if (!_subborrow_u64(                                               \
                    _sub_u64(                                                  \
                        _lowDividend,                                          \
                        lowSubtrahend,                                         \
                        &low),                                                 \
                    _highDividend,                                             \
                    highSubtrahend,                                            \
                    &high))                                                    \
            {                                                                  \
                _lowDividend = low;                                            \
                _highDividend = high;                                          \
                *_quotient |= addend;                                          \
                if (!_highDividend)                                            \
                    break;                                                     \
            }                                                                  \
        }                                                                      \
    }                                                                          \
    *_remainder = _lowDividend % _divisor;                                     \
    *_quotient += _lowDividend / _divisor;                                     \
} while (0)
#ifndef _WIN64
#define DIV128(highDividend, lowDividend, divisor, quotient, remainder)         \
do                                                                              \
{                                                                               \
    int64_t             __highDividend = highDividend;                          \
    int64_t             __lowDividend  = lowDividend;                           \
    int64_t             __divisor      = divisor;                               \
    int64_t *__restrict __quotient     = quotient;                              \
    int64_t *__restrict __remainder    = remainder;                             \
                                                                                \
    uint32_t sign, x;                                                           \
                                                                                \
    sign = (uint32_t)(__highDividend >> 63);                                    \
    __lowDividend ^= sign | ((uint64_t)sign << 32);                             \
    __highDividend ^= sign | ((uint64_t)sign << 32);                            \
    _subborrow_u64(                                                             \
        _sub_u64(                                                               \
            __lowDividend,                                                      \
            sign | ((uint64_t)sign << 32),                                      \
            (uint64_t *)&__lowDividend),                                        \
        __highDividend,                                                         \
        sign | ((uint64_t)sign << 32),                                          \
        (uint64_t *)&__highDividend);                                           \
    sign ^= x = (uint32_t)(__divisor >> 63);                                    \
    __divisor ^= x | ((uint64_t)x << 32);                                       \
    __divisor -= x | ((uint64_t)x << 32);                                       \
    UDIV128(__highDividend, __lowDividend, __divisor, __quotient, __remainder); \
    *__quotient ^= sign | ((uint64_t)sign << 32);                               \
    *__quotient -= sign | ((uint64_t)sign << 32);                               \
    *__remainder ^= sign | ((uint64_t)sign << 32);                              \
    *__remainder -= sign | ((uint64_t)sign << 32);                              \
} while (0)
#else
#define DIV128(highDividend, lowDividend, divisor, quotient, remainder)         \
do                                                                              \
{                                                                               \
    int64_t             __highDividend = highDividend;                          \
    int64_t             __lowDividend  = lowDividend;                           \
    int64_t             __divisor      = divisor;                               \
    int64_t *__restrict __quotient     = quotient;                              \
    int64_t *__restrict __remainder    = remainder;                             \
                                                                                \
    uint64_t sign, x;                                                           \
                                                                                \
    sign = __highDividend >> 63;                                                \
    __lowDividend ^= sign;                                                      \
    __highDividend ^= sign;                                                     \
    _subborrow_u64(                                                             \
        _sub_u64(                                                               \
            __lowDividend,                                                      \
            sign,                                                               \
            (uint64_t *)&__lowDividend),                                        \
        __highDividend,                                                         \
        sign,                                                                   \
        (uint64_t *)&__highDividend);                                           \
    sign ^= x = __divisor >> 63;                                                \
    __divisor ^= x;                                                             \
    __divisor -= x;                                                             \
    UDIV128(__highDividend, __lowDividend, __divisor, __quotient, __remainder); \
    *__quotient ^= sign;                                                        \
    *__quotient -= sign;                                                        \
    *__remainder ^= sign;                                                       \
    *__remainder -= sign;                                                       \
} while (0)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif	// _INTRINSIC_H_
