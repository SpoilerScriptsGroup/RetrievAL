#ifndef _INTRINSIC_H_
#define _INTRINSIC_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#if defined(_MSC_VER) && _MSC_VER >= 1310
#include <intrin.h>
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 uint64_t;
#else
typedef unsigned long long uint64_t;
#endif

#ifdef __BORLANDC__
#pragma warn -8027
#pragma warn -8098
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
	((BYTE)(c) >= (BYTE)0x81 && ((BYTE)(c) <= (BYTE)0x9F || ((BYTE)(c) >= (BYTE)0xE0 && (BYTE)(c) <= (BYTE)0xFC)))

#define __intrinsic_istrailbyte_cp932(c) \
	((BYTE)(c) >= (BYTE)0x40 && (BYTE)(c) <= (BYTE)0xFC && (BYTE)(c) != (BYTE)0x7F)

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

#define __intrinsic_isascii(c) \
	((char)(c) >= 0)

#define __intrinsic_isdigit(c) \
	((unsigned)((c) - '0') <= '9' - '0')

#define __intrinsic_isxdigit(c) \
	((char)(c) >= '0' && (BYTE)(c) <= 'f' && ((BYTE)(c) <= '9' || (BYTE)(c) >= 'A' && ((BYTE)(c) <= 'F' || (BYTE)(c) >= 'a')))

#define __intrinsic_isspace(c) \
	((c) == ' ' || (BYTE)(c) <= '\r' && (BYTE)(c) >= '\t')

#define __intrinsic_isblank(c) \
	((c) == ' ' || (c) == '\t')

#define __intrinsic_isspace_without_return(c) \
	(__intrinsic_isblank(c) || (c) == '\v' || (c) == '\f')

#define __intrinsic_isupper(c) \
	((unsigned)((c) - 'A') <= 'Z' - 'A')

#define __intrinsic_islower(c) \
	((unsigned)((c) - 'a') <= 'z' - 'a')

#define __intrinsic_isalpha(c) \
	(__intrinsic_isupper(c) || __intrinsic_islower(c))

#define __intrinsic_isalnum(c) \
	(__intrinsic_isalpha(c) || __intrinsic_isdigit(c))

#define __intrinsic_iscsym(c) \
	(__intrinsic_isalnum(c) || (c) == '_')

#define __intrinsic_iscsymf(c) \
	(__intrinsic_isalpha(c) || (c) == '_')

#define __intrinsic_isprint(c) \
	((char)(c) >= 0x20 && (BYTE)(c) <= 0x7E)

#define __intrinsic_isgraph(c) \
	((char)(c) >= 0x21 && (BYTE)(c) <= 0x7E)

#define __intrinsic_iscntrl(c) \
	((BYTE)(c) <= 0x1F || (c) == 0x7F)

#define __intrinsic_ispunct(c) \
	(__intrinsic_isgraph(c) && !__intrinsic_isalnum(c))

#define __intrinsic_iskana(c) \
	((BYTE)(c) >= 0xA1 && (BYTE)(c) <= 0xDF)

#define __intrinsic_ismbchira(c) \
	((c) >= 0x829F && (c) <= 0x82F1)

#define __intrinsic_ismbckata(c) \
	((c) >= 0x8340 && (c) <= 0x8396)

#define __intrinsic_ismbclegal(c) \
	((c) >= 0x8140 && (c) <= 0xFCFC && ((c) <= 0x9FFC || (c) >= 0xE040) && (BYTE)(c) != 0x7F)

#define __intrinsic_ismbcsymbol(c) \
	((c) >= 0x8141 && (c) <= 0x81AC)

#define __intrinsic_ismbcl0(c) \
	((c) >= 0x8140 && (c) <= 0x889E)

#define __intrinsic_ismbcl1(c) \
	((c) >= 0x889F && (c) <= 0x9872)

#define __intrinsic_ismbcl2(c) \
	((c) >= 0x989F && (c) <= 0xEA9E)

#define __intrinsic_tolower(c) \
	(__intrinsic_isupper(c) ? ((c) + ('a' - 'A')) : (c))

#define __intrinsic_toupper(c) \
	(__intrinsic_islower(c) ? ((c) - ('a' - 'A')) : (c))

#define __intrinsic_toascii(c) \
	((c) & 0x7F)

#define __intrinsic_iswascii(c) \
	((c) <= 0x7F)

#define __intrinsic_iswdigit(c) \
	((unsigned)((c) - L'0') <= L'9' - L'0')

#define __intrinsic_iswxdigit(c) \
	((c) <= 'f' && (c) >= '0' && ((c) <= '9' || (c) >= 'A' && ((c) <= 'F' || (c) >= 'a')))

#define __intrinsic_iswspace(c) \
	((c) == L' ' || ((c) <= L'\r' && (c) >= L'\t'))

#define __intrinsic_iswblank(c) \
	((c) == L' ' || (c) == L'\t')

#define __intrinsic_iswspace_without_return(c) \
	(__intrinsic_iswblank(c) || (c) == L'\v' || (c) == L'\f')

#define __intrinsic_iswupper(c) \
	((unsigned)((c) - L'A') <= L'Z' - L'A')

#define __intrinsic_iswlower(c) \
	((unsigned)((c) - L'a') <= L'z' - L'a')

#define __intrinsic_iswalpha(c) \
	(__intrinsic_iswupper(c) || __intrinsic_iswlower(c))

#define __intrinsic_iswalnum(c) \
	(__intrinsic_iswalpha(c) || __intrinsic_iswdigit(c))

#define __intrinsic_iswcsym(c) \
	(__intrinsic_iswalnum(c) || (c) == L'_')

#define __intrinsic_iswcsymf(c) \
	(__intrinsic_iswalpha(c) || (c) == L'_')

#define __intrinsic_iswprint(c) \
	((c) <= 0x7E && (c) >= 0x20)

#define __intrinsic_iswgraph(c) \
	((c) <= 0x7E && (c) >= 0x21)

#define __intrinsic_iswcntrl(c) \
	((c) <= 0x1F || (c) == 0x7F)

#define __intrinsic_iswpunct(c) \
	(__intrinsic_iswgraph(c) && !__intrinsic_iswalnum(c))

#define __intrinsic_towlower(c) \
	(__intrinsic_iswupper(c) ? ((c) + (L'a' - L'A')) : (c))

#define __intrinsic_towupper(c) \
	(__intrinsic_iswlower(c) ? ((c) - (L'a' - L'A')) : (c))

#define __intrinsic_towascii(c) \
	((c) & 0x7F)

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
#define __intrinsic_bswap24(value) _byteswap_ulong((DWORD)(value) << 8)
#define __intrinsic_bswap32 _byteswap_ulong
#define __intrinsic_bswap40(value) _byteswap_uint64((uint64_t)(value) << 24)
#define __intrinsic_bswap48(value) _byteswap_uint64((uint64_t)(value) << 16)
#define __intrinsic_bswap56(value) _byteswap_uint64((uint64_t)(value) << 8)
#define __intrinsic_bswap64 _byteswap_uint64
#else
__forceinline unsigned short __intrinsic_bswap16(unsigned short value) { return BSWAP16 (value); }
__forceinline unsigned long  __intrinsic_bswap24(unsigned long  value) { return BSWAP24 (value); }
__forceinline unsigned long  __intrinsic_bswap32(unsigned long  value) { return BSWAP32 (value); }
__forceinline uint64_t       __intrinsic_bswap40(uint64_t       value) { return BSWAP40 (value); }
__forceinline uint64_t       __intrinsic_bswap48(uint64_t       value) { return BSWAP48 (value); }
__forceinline uint64_t       __intrinsic_bswap56(uint64_t       value) { return BSWAP56 (value); }
__forceinline uint64_t       __intrinsic_bswap64(uint64_t       value) { return BSWAP64 (value); }
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
#define _rotl64(value, shift) __fastcall_rotl64((DWORD)(value), (DWORD)((uint64_t)(value) >> 32), shift)
#define _rotr64(value, shift) __fastcall_rotr64((DWORD)(value), (DWORD)((uint64_t)(value) >> 32), shift)
#else
#define _rotl(value, shift) (((unsigned int)(value) << (shift)) | ((unsigned int)(value) >> (32 - (shift))))
#define _rotr(value, shift) (((unsigned int)(value) >> (shift)) | ((unsigned int)(value) << (32 - (shift))))
#define _rotl64(value, shift) (((uint64_t)(value) << (shift)) | ((uint64_t)(value) >> (64 - (shift))))
#define _rotr64(value, shift) (((uint64_t)(value) >> (shift)) | ((uint64_t)(value) << (64 - (shift))))
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
    __asm   mov     ecx, dword ptr [_Count]                     \
    __asm   mov     esi, dword ptr [_Source]                    \
    __asm   mov     edi, dword ptr [_Destination]               \
    __asm   rep movsb                                           \
} while (0)
#define __movsw(Destination, Source, Count)                      \
do                                                               \
{                                                                \
    unsigned short *      __restrict _Destination = Destination; \
    const unsigned short *__restrict _Source      = Source;      \
    size_t                           _Count       = Count;       \
                                                                 \
    __asm   mov     ecx, dword ptr [_Count]                      \
    __asm   mov     esi, dword ptr [_Source]                     \
    __asm   mov     edi, dword ptr [_Destination]                \
    __asm   rep movsw                                            \
} while (0)
#define __movsd(Destination, Source, Count)                     \
do                                                              \
{                                                               \
    unsigned long *      __restrict _Destination = Destination; \
    const unsigned long *__restrict _Source      = Source;      \
    size_t                          _Count       = Count;       \
                                                                \
    __asm   mov     ecx, dword ptr [_Count]                     \
    __asm   mov     esi, dword ptr [_Source]                    \
    __asm   mov     edi, dword ptr [_Destination]               \
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
    __asm   mov     ecx, dword ptr [_Count]   \
    __asm   mov     al, byte ptr [_Data]      \
    __asm   mov     edi, dword ptr [_Dest]    \
    __asm   rep stosb                         \
} while (0)
#define __stosw(Dest, Data, Count)             \
do                                             \
{                                              \
    unsigned short *__restrict _Dest  = Dest;  \
    unsigned short             _Data  = Data;  \
    size_t                     _Count = Count; \
                                               \
    __asm   mov     ecx, dword ptr [_Count]    \
    __asm   mov     ax, word ptr [_Data]       \
    __asm   mov     edi, dword ptr [_Dest]     \
    __asm   rep stosw                          \
} while (0)
#define __stosd(Dest, Data, Count)            \
do                                            \
{                                             \
    unsigned long *__restrict _Dest  = Dest;  \
    unsigned long             _Data  = Data;  \
    size_t                    _Count = Count; \
                                              \
    __asm   mov     ecx, dword ptr [_Count]   \
    __asm   mov     eax, dword ptr [_Data]    \
    __asm   mov     edi, dword ptr [_Dest]    \
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
#define __stosq(Dest, Data, Count)               \
do                                               \
{                                                \
    unsigned __int64 *__restrict _Dest  = Dest;  \
    unsigned __int64             _Data  = Data;  \
    size_t                       _Count = Count; \
                                                 \
    while (_Count--)                             \
        *(_Dest++) = _Data;                      \
} while (0)
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

#if defined(_MSC_VER) && _MSC_VER >= 1920
#pragma intrinsic(_udiv64)
__forceinline unsigned int __udiv64(unsigned __int64 dividend, unsigned int divisor)
{
	unsigned int remainder;

	return _udiv64(dividend, divisor, &remainder);
}
#elif defined(_MSC_VER) && _MSC_VER < 1920 && defined(_M_IX86)
__forceinline unsigned int _udiv64(unsigned __int64 dividend, unsigned int divisor, unsigned int *remainder)
{
	__asm
	{
		mov     eax, dword ptr [dividend]
		mov     edx, dword ptr [dividend + 4]
		mov     ecx, dword ptr [divisor]
		div     ecx
		mov     ecx, dword ptr [remainder]
		mov     dword ptr [ecx], edx
	}
}
__forceinline unsigned int __udiv64(unsigned __int64 dividend, unsigned int divisor)
{
	__asm
	{
		mov     eax, dword ptr [dividend]
		mov     edx, dword ptr [dividend + 4]
		mov     ecx, dword ptr [divisor]
		div     ecx
	}
}
#elif defined(__BORLANDC__)
unsigned int __fastcall __fastcall_udiv64(DWORD low, DWORD high, unsigned int divisor, unsigned int *remainder);
unsigned int __fastcall __fastcall__udiv64(DWORD low, DWORD high, unsigned int divisor);
#define _udiv64(dividend, divisor, remainder) __fastcall_udiv64((DWORD)(dividend), (DWORD)((uint64_t)(dividend) >> 32), divisor, remainder)
#define __udiv64(dividend, divisor) __fastcall__udiv64((DWORD)(dividend), (DWORD)((uint64_t)(dividend) >> 32), divisor)
#else
__forceinline unsigned int _udiv64(unsigned __int64 dividend, unsigned int divisor, unsigned int *remainder)
{
	*remainder = dividend % divisor;
	return (unsigned int)(dividend / divisor);
}
__forceinline unsigned int __udiv64(unsigned __int64 dividend, unsigned int divisor)
{
	return (unsigned int)(dividend / divisor);
}
#endif

// for constant value
#define BSF(value) (                       \
    ((value) &  (uint64_t)1       ) ?  0 : \
    ((value) & ((uint64_t)1 <<  1)) ?  1 : \
    ((value) & ((uint64_t)1 <<  2)) ?  2 : \
    ((value) & ((uint64_t)1 <<  3)) ?  3 : \
    ((value) & ((uint64_t)1 <<  4)) ?  4 : \
    ((value) & ((uint64_t)1 <<  5)) ?  5 : \
    ((value) & ((uint64_t)1 <<  6)) ?  6 : \
    ((value) & ((uint64_t)1 <<  7)) ?  7 : \
    ((value) & ((uint64_t)1 <<  8)) ?  8 : \
    ((value) & ((uint64_t)1 <<  9)) ?  9 : \
    ((value) & ((uint64_t)1 <<  0)) ?  0 : \
    ((value) & ((uint64_t)1 << 11)) ? 11 : \
    ((value) & ((uint64_t)1 << 12)) ? 12 : \
    ((value) & ((uint64_t)1 << 13)) ? 13 : \
    ((value) & ((uint64_t)1 << 14)) ? 14 : \
    ((value) & ((uint64_t)1 << 15)) ? 15 : \
    ((value) & ((uint64_t)1 << 16)) ? 16 : \
    ((value) & ((uint64_t)1 << 17)) ? 17 : \
    ((value) & ((uint64_t)1 << 18)) ? 18 : \
    ((value) & ((uint64_t)1 << 19)) ? 19 : \
    ((value) & ((uint64_t)1 << 20)) ? 20 : \
    ((value) & ((uint64_t)1 << 21)) ? 21 : \
    ((value) & ((uint64_t)1 << 22)) ? 22 : \
    ((value) & ((uint64_t)1 << 23)) ? 23 : \
    ((value) & ((uint64_t)1 << 24)) ? 24 : \
    ((value) & ((uint64_t)1 << 25)) ? 25 : \
    ((value) & ((uint64_t)1 << 26)) ? 26 : \
    ((value) & ((uint64_t)1 << 27)) ? 27 : \
    ((value) & ((uint64_t)1 << 28)) ? 28 : \
    ((value) & ((uint64_t)1 << 29)) ? 29 : \
    ((value) & ((uint64_t)1 << 30)) ? 30 : \
    ((value) & ((uint64_t)1 << 31)) ? 31 : \
    ((value) & ((uint64_t)1 << 32)) ? 32 : \
    ((value) & ((uint64_t)1 << 33)) ? 33 : \
    ((value) & ((uint64_t)1 << 34)) ? 34 : \
    ((value) & ((uint64_t)1 << 35)) ? 35 : \
    ((value) & ((uint64_t)1 << 36)) ? 36 : \
    ((value) & ((uint64_t)1 << 37)) ? 37 : \
    ((value) & ((uint64_t)1 << 38)) ? 38 : \
    ((value) & ((uint64_t)1 << 39)) ? 39 : \
    ((value) & ((uint64_t)1 << 40)) ? 40 : \
    ((value) & ((uint64_t)1 << 41)) ? 41 : \
    ((value) & ((uint64_t)1 << 42)) ? 42 : \
    ((value) & ((uint64_t)1 << 43)) ? 43 : \
    ((value) & ((uint64_t)1 << 44)) ? 44 : \
    ((value) & ((uint64_t)1 << 45)) ? 45 : \
    ((value) & ((uint64_t)1 << 46)) ? 46 : \
    ((value) & ((uint64_t)1 << 47)) ? 47 : \
    ((value) & ((uint64_t)1 << 48)) ? 48 : \
    ((value) & ((uint64_t)1 << 49)) ? 49 : \
    ((value) & ((uint64_t)1 << 50)) ? 50 : \
    ((value) & ((uint64_t)1 << 51)) ? 51 : \
    ((value) & ((uint64_t)1 << 52)) ? 52 : \
    ((value) & ((uint64_t)1 << 53)) ? 53 : \
    ((value) & ((uint64_t)1 << 54)) ? 54 : \
    ((value) & ((uint64_t)1 << 55)) ? 55 : \
    ((value) & ((uint64_t)1 << 56)) ? 56 : \
    ((value) & ((uint64_t)1 << 57)) ? 57 : \
    ((value) & ((uint64_t)1 << 58)) ? 58 : \
    ((value) & ((uint64_t)1 << 59)) ? 59 : \
    ((value) & ((uint64_t)1 << 60)) ? 60 : \
    ((value) & ((uint64_t)1 << 61)) ? 61 : \
    ((value) & ((uint64_t)1 << 62)) ? 62 : \
    ((value) & ((uint64_t)1 << 63)) ? 63 : -1)

// for constant value
#define BSR(value) (                       \
    ((value) & ((uint64_t)1 << 63)) ? 63 : \
    ((value) & ((uint64_t)1 << 62)) ? 62 : \
    ((value) & ((uint64_t)1 << 61)) ? 61 : \
    ((value) & ((uint64_t)1 << 60)) ? 60 : \
    ((value) & ((uint64_t)1 << 59)) ? 59 : \
    ((value) & ((uint64_t)1 << 58)) ? 58 : \
    ((value) & ((uint64_t)1 << 57)) ? 57 : \
    ((value) & ((uint64_t)1 << 56)) ? 56 : \
    ((value) & ((uint64_t)1 << 55)) ? 55 : \
    ((value) & ((uint64_t)1 << 54)) ? 54 : \
    ((value) & ((uint64_t)1 << 53)) ? 53 : \
    ((value) & ((uint64_t)1 << 52)) ? 52 : \
    ((value) & ((uint64_t)1 << 51)) ? 51 : \
    ((value) & ((uint64_t)1 << 50)) ? 50 : \
    ((value) & ((uint64_t)1 << 49)) ? 49 : \
    ((value) & ((uint64_t)1 << 48)) ? 48 : \
    ((value) & ((uint64_t)1 << 47)) ? 47 : \
    ((value) & ((uint64_t)1 << 46)) ? 46 : \
    ((value) & ((uint64_t)1 << 45)) ? 45 : \
    ((value) & ((uint64_t)1 << 44)) ? 44 : \
    ((value) & ((uint64_t)1 << 43)) ? 43 : \
    ((value) & ((uint64_t)1 << 42)) ? 42 : \
    ((value) & ((uint64_t)1 << 41)) ? 41 : \
    ((value) & ((uint64_t)1 << 40)) ? 40 : \
    ((value) & ((uint64_t)1 << 39)) ? 39 : \
    ((value) & ((uint64_t)1 << 38)) ? 38 : \
    ((value) & ((uint64_t)1 << 37)) ? 37 : \
    ((value) & ((uint64_t)1 << 36)) ? 36 : \
    ((value) & ((uint64_t)1 << 35)) ? 35 : \
    ((value) & ((uint64_t)1 << 34)) ? 34 : \
    ((value) & ((uint64_t)1 << 33)) ? 33 : \
    ((value) & ((uint64_t)1 << 32)) ? 32 : \
    ((value) & ((uint64_t)1 << 31)) ? 31 : \
    ((value) & ((uint64_t)1 << 30)) ? 30 : \
    ((value) & ((uint64_t)1 << 29)) ? 29 : \
    ((value) & ((uint64_t)1 << 28)) ? 28 : \
    ((value) & ((uint64_t)1 << 27)) ? 27 : \
    ((value) & ((uint64_t)1 << 26)) ? 26 : \
    ((value) & ((uint64_t)1 << 25)) ? 25 : \
    ((value) & ((uint64_t)1 << 24)) ? 24 : \
    ((value) & ((uint64_t)1 << 23)) ? 23 : \
    ((value) & ((uint64_t)1 << 22)) ? 22 : \
    ((value) & ((uint64_t)1 << 21)) ? 21 : \
    ((value) & ((uint64_t)1 << 20)) ? 20 : \
    ((value) & ((uint64_t)1 << 19)) ? 19 : \
    ((value) & ((uint64_t)1 << 18)) ? 18 : \
    ((value) & ((uint64_t)1 << 17)) ? 17 : \
    ((value) & ((uint64_t)1 << 16)) ? 16 : \
    ((value) & ((uint64_t)1 << 15)) ? 15 : \
    ((value) & ((uint64_t)1 << 14)) ? 14 : \
    ((value) & ((uint64_t)1 << 13)) ? 13 : \
    ((value) & ((uint64_t)1 << 12)) ? 12 : \
    ((value) & ((uint64_t)1 << 11)) ? 11 : \
    ((value) & ((uint64_t)1 << 10)) ? 10 : \
    ((value) & ((uint64_t)1 <<  9)) ?  9 : \
    ((value) & ((uint64_t)1 <<  8)) ?  8 : \
    ((value) & ((uint64_t)1 <<  7)) ?  7 : \
    ((value) & ((uint64_t)1 <<  6)) ?  6 : \
    ((value) & ((uint64_t)1 <<  5)) ?  5 : \
    ((value) & ((uint64_t)1 <<  4)) ?  4 : \
    ((value) & ((uint64_t)1 <<  3)) ?  3 : \
    ((value) & ((uint64_t)1 <<  2)) ?  2 : \
    ((value) & ((uint64_t)1 <<  1)) ?  1 : \
    ((value) &  (uint64_t)1       ) ?  0 : -1)

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned char _BitScanForward(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		mov     eax, dword ptr [Mask]
		mov     ecx, dword ptr [Index]
		bsf     eax, eax
		mov     dword ptr [ecx], eax
		setnz   al
	}
}
__forceinline unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		mov     eax, dword ptr [Mask]
		mov     ecx, dword ptr [Index]
		bsr     eax, eax
		mov     dword ptr [ecx], eax
		setnz   al
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
		unsigned long i;

		for (i = 0; !(Mask & 1); Mask >>= 1)
			i++;
		*Index = i;
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
		unsigned long i;

		for (i = 31; (long)Mask >= 0; Mask <<= 1)
			i--;
		*Index = i;
		return 1;
	}
	else
	{
		return 0;
	}
}
#endif

#if defined(_MSC_VER) && defined(_M_X64)
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned char _BitScanForward64(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		bsf     eax, dword ptr [Mask]
		mov     ecx, dword ptr [Index]
		jnz     L1
		bsf     eax, dword ptr [Mask + 4]
		lea     eax, [eax + 32]
	L1:
		mov     dword ptr [ecx], eax
		setnz   al
	}
}
__forceinline unsigned char _BitScanReverse64(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		mov     ecx, dword ptr [Index]
		bsr     eax, dword ptr [Mask + 4]
		lea     eax, [eax + 32]
		jnz     L1
		bsr     eax, dword ptr [Mask]
	L1:
		mov     dword ptr [ecx], eax
		setnz   al
	}
}
#elif defined(__BORLANDC__)
unsigned char __fastcall __fastcall_BitScanForward64(DWORD low, DWORD high, unsigned long *Index);
unsigned char __fastcall __fastcall_BitScanReverse64(DWORD low, DWORD high, unsigned long *Index);
#define _BitScanForward64(Index, Mask) __fastcall_BitScanForward64((DWORD)(Mask), (DWORD)((uint64_t)(Mask) >> 32), Index)
#define _BitScanReverse64(Index, Mask) __fastcall_BitScanReverse64((DWORD)(Mask), (DWORD)((uint64_t)(Mask) >> 32), Index)
#else
__forceinline unsigned char _BitScanForward64(unsigned long *Index, uint64_t Mask)
{
	unsigned char Result;

	if (!(Result = _BitScanForward(Index, (unsigned long)Mask)))
		if (Result = _BitScanForward(Index, (unsigned long)(Mask >> 32)))
			*Index += 32;
	return Result;
}
__forceinline unsigned char _BitScanReverse64(unsigned long *Index, uint64_t Mask)
{
	unsigned char Result;

	if (Result = _BitScanReverse(Index, (unsigned long)(Mask >> 32)))
		*Index += 32;
	else
		Result = _BitScanReverse(Index, (unsigned long)Mask);
	return Result;
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_addcarry_u32)
#define _add_u32(a, b, out) _addcarry_u32(0, a, b, out)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned char _add_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	__asm
	{
		mov     ecx, dword ptr [a]
		mov     edx, dword ptr [b]
		add     ecx, edx
		mov     edx, dword ptr [out]
		setc    al
		mov     dword ptr [edx], ecx
	}
}
__forceinline unsigned char _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *out)
{
	__asm
	{
		mov     al, byte ptr [c_in]
		mov     ecx, dword ptr [a]
		add     al, -1
		mov     edx, dword ptr [b]
		adc     ecx, edx
		mov     edx, dword ptr [out]
		setc    al
		mov     dword ptr [edx], ecx
	}
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _add_u32(unsigned int a, unsigned int b, unsigned int *out);
unsigned char __fastcall _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *out);
#else
__forceinline unsigned char _add_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	return (*out = a + b) < b;
}
__forceinline unsigned char _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *out)
{
	return ((*out = a + b) < b) | (c_in && !++(*out));
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310
#pragma intrinsic(_subborrow_u32)
#define _sub_u32(a, b, out) _subborrow_u32(0, a, b, out)
#elif defined(_MSC_VER) && _MSC_VER < 1310 && defined(_M_IX86)
__forceinline unsigned char _sub_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	__asm
	{
		mov     ecx, dword ptr [a]
		mov     edx, dword ptr [b]
		sub     ecx, edx
		mov     edx, dword ptr [out]
		setc    al
		mov     dword ptr [edx], ecx
	}
}
__forceinline unsigned char _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *out)
{
	__asm
	{
		mov     al, byte ptr [b_in]
		mov     ecx, dword ptr [a]
		add     al, -1
		mov     edx, dword ptr [b]
		sbb     ecx, edx
		mov     edx, dword ptr [out]
		setc    al
		mov     dword ptr [edx], ecx
	}
}
#elif defined(__BORLANDC__)
unsigned char __fastcall _sub_u32(unsigned int a, unsigned int b, unsigned int *out);
unsigned char __fastcall _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *out);
#else
__forceinline unsigned char _sub_u32(unsigned int a, unsigned int b, unsigned int *out)
{
	return (*out = a - b) > a;
}
__forceinline unsigned char _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *out)
{
	return ((*out = a - b) > a) | (b_in && !(*out)--);
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_WIN64)
#pragma intrinsic(_addcarry_u64)
#define _add_u64(a, b, out) _addcarry_u64(0, a, b, out)
#else
__forceinline unsigned char _add_u64(uint64_t a, uint64_t b, uint64_t *out)
{
	return (*out = a + b) < b;
}
__forceinline unsigned char _addcarry_u64(unsigned char c_in, uint64_t a, uint64_t b, uint64_t *out)
{
	return ((*out = a + b) < b) | (c_in && !++(*out));
}
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1310 && defined(_WIN64)
#pragma intrinsic(_subborrow_u64)
#define _sub_u64(a, b, out) _subborrow_u64(0, a, b, out)
#else
__forceinline unsigned char _sub_u64(uint64_t a, uint64_t b, uint64_t *out)
{
	return (*out = a - b) > a;
}
__forceinline unsigned char _subborrow_u64(unsigned char b_in, uint64_t a, uint64_t b, uint64_t *out)
{
	return ((*out = a - b) > a) | (b_in && !(*out)--);
}
#endif

#ifndef _WIN64
#define _add_uintptr       _add_u32
#define _addborrow_uintptr _addborrow_u32
#define _sub_uintptr       _sub_u32
#define _subborrow_uintptr _subborrow_u32
#else
#define _add_uintptr       _add_u64
#define _addborrow_uintptr _addborrow_u64
#define _sub_uintptr       _sub_u64
#define _subborrow_uintptr _subborrow_u64
#endif

#ifdef __cplusplus
}
#endif

#endif	// _INTRINSIC_H_
