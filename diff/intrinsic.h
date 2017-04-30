#ifndef _INTRINSIC_H_
#define _INTRINSIC_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#include <limits.h>
#ifndef MAXQWORD
#define MAXQWORD _UI64_MAX
typedef ULONG64    QWORD;
typedef QWORD near *PQWORD;
typedef QWORD far  *LPQWORD;
#endif

#if defined(__BORLANDC__)
#pragma warn -8098
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

#define __intrinsic__iscsym(c) \
	(__intrinsic_isdigit(c) || __intrinsic_isupper(c) || (BYTE)(c) == (BYTE)'_' || __intrinsic_islower(c))

#define __intrinsic_iskana(c) \
	((BYTE)(c) >= (BYTE)0xA1 && (BYTE)(c) <= (BYTE)0xDF)

#define __intrinsic_tolower(c) \
	(__intrinsic_isupper(c) ? (char)((char)(c) + (char)('a' - 'A')) : (char)(c))

#define __intrinsic_toupper(c) \
	(__intrinsic_islower(c) ? (char)((char)(c) - (char)('a' - 'A')) : (char)(c))

#if defined(_MSC_VER) && defined(_M_IX86)
// small, slow
__inline size_t __intrinsic_strlen(const char *string)
{
	__asm
	{
		xor     eax, eax
		mov     edx, string
		mov     ecx, -1
		mov     edi, edx
		xor     edx, ecx
		repne scasb
		lea     eax, [edi + edx]
	}
}
#else
#define __intrinsic_strlen strlen
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// small, slow
__inline int __intrinsic_strcmp(const char *string1, const char *string2)
{
	__asm
	{
		xor     eax, eax
		mov     edx, string1
		mov     ecx, -1
		mov     esi, edx
		mov     edi, edx
		xor     edx, ecx
		repne scasb
		lea     ecx, [edi + edx + 1]
		mov     edi, string2
		repe cmpsb
		mov     ecx, eax
		setl    al
		setle   cl
		lea     eax, [eax + ecx - 1]
	}
}
#else
#define __intrinsic_strcmp strcmp
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// small, slow
__inline int __intrinsic_strncmp(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		mov     ecx, count
		xor     eax, eax
		test    ecx, ecx
		jz      L1
		mov     edx, string1
		mov     esi, edx
		mov     edi, edx
		neg     edx
		repne scasb
		lea     ecx, [edi + edx]
		mov     edi, string2
		repe cmpsb
		mov     ecx, eax
		setl    al
		setle   cl
		lea     eax, [eax + ecx - 1]
	L1:
	}
}
#else
#define __intrinsic_strncmp strncmp
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// small, slow
__inline int __intrinsic_memcmp(const void *buf1, const void *buf2, size_t count)
{
	__asm
	{
		mov     ecx, count
		xor     eax, eax
		test    ecx, ecx
		jz      L1
		mov     esi, buf2
		mov     edi, buf1
		repe cmpsb
		seta    al
		sbb     eax, 0
	L1:
	}
}
#else
#define __intrinsic_memcmp memcmp
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// small, slow
__inline BOOLEAN __intrinsic_string_n_compare(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		mov     ecx, count
		mov     esi, string2
		mov     edi, string1
		repe cmpsb
		sete    al
	}
}
#else
#define __intrinsic_string_n_compare(string1, string2, count) (strncmp(string1, string2, count) == 0)
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// small, slow
__inline int __intrinsic_memory_compare(const void *buf1, const void *buf2, size_t count)
{
	__asm
	{
		xor     eax, eax
		mov     ecx, count
		mov     esi, buf2
		mov     edi, buf1
		repe cmpsb
		seta    al
		sbb     eax, 0
	}
}
#else
#define __intrinsic_memory_compare memcmp
#endif

// for const value
#define BSWAP16(value) (WORD)(((WORD)(value) >> 8) | ((WORD)(value) << 8))

// for const value
#define BSWAP24(value) (DWORD)(             \
    (((DWORD)(value) >> 16) & 0x000000FF) | \
    ( (DWORD)(value)        & 0x0000FF00) | \
    (((DWORD)(value) << 16) & 0x00FF0000))

// for const value
#define BSWAP32(value) (DWORD)(             \
    ( (DWORD)(value) >> 24              ) | \
    (((DWORD)(value) >>  8) & 0x0000FF00) | \
    (((DWORD)(value) <<  8) & 0x00FF0000) | \
    ( (DWORD)(value) << 24              ))

// for const value
#define BSWAP40(value) (QWORD)(                     \
    (((QWORD)(value) >> 32) & 0x00000000000000FF) | \
    (((QWORD)(value) >> 16) & 0x000000000000FF00) | \
    ( (QWORD)(value)        & 0x0000000000FF0000) | \
    (((QWORD)(value) << 16) & 0x00000000FF000000) | \
    (((QWORD)(value) << 32) & 0x000000FF00000000))

// for const value
#define BSWAP48(value) (QWORD)(                     \
    (((QWORD)(value) >> 40) & 0x00000000000000FF) | \
    (((QWORD)(value) >> 24) & 0x000000000000FF00) | \
    (((QWORD)(value) >>  8) & 0x0000000000FF0000) | \
    (((QWORD)(value) <<  8) & 0x00000000FF000000) | \
    (((QWORD)(value) << 24) & 0x000000FF00000000) | \
    (((QWORD)(value) << 40) & 0x0000FF0000000000))

// for const value
#define BSWAP56(value) (QWORD)(                     \
    (((QWORD)(value) >> 48) & 0x00000000000000FF) | \
    (((QWORD)(value) >> 32) & 0x000000000000FF00) | \
    (((QWORD)(value) >> 16) & 0x0000000000FF0000) | \
    ( (QWORD)(value)        & 0x00000000FF000000) | \
    (((QWORD)(value) << 16) & 0x000000FF00000000) | \
    (((QWORD)(value) << 32) & 0x0000FF0000000000) | \
    (((QWORD)(value) << 48) & 0x00FF000000000000))

// for const value
#define BSWAP64(value) (QWORD)(                     \
    ( (QWORD)(value) >> 56                      ) | \
    (((QWORD)(value) >> 40) & 0x000000000000FF00) | \
    (((QWORD)(value) >> 24) & 0x0000000000FF0000) | \
    (((QWORD)(value) >>  8) & 0x00000000FF000000) | \
    (((QWORD)(value) <<  8) & 0x000000FF00000000) | \
    (((QWORD)(value) << 24) & 0x0000FF0000000000) | \
    (((QWORD)(value) << 40) & 0x00FF000000000000) | \
    ( (QWORD)(value) << 56                      ))

#if defined(_MSC_VER) && defined(_M_IX86)
// fast and small
__inline WORD __intrinsic_bswap16(WORD value)
{
	__asm
	{
		mov     ax, word ptr [value]
		xchg    al, ah
	}
}
#elif defined(__BORLANDC__)
__declspec(naked) WORD __fastcall __intrinsic_bswap16(WORD value);
#else
#define __intrinsic_bswap16 BSWAP16
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// fast and small
__inline DWORD __intrinsic_bswap24(DWORD value)
{
	__asm
	{
		mov     eax, dword ptr [value]
		shl     eax, 8
		bswap   eax
	}
}
#elif defined(__BORLANDC__)
__declspec(naked) DWORD __fastcall __intrinsic_bswap24(DWORD value);
#else
#define __intrinsic_bswap24 BSWAP24
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// fast and small
__inline DWORD __intrinsic_bswap32(DWORD value)
{
	__asm
	{
		mov     eax, dword ptr [value]
		bswap   eax
	}
}
#elif defined(__BORLANDC__)
__declspec(naked) DWORD __fastcall __intrinsic_bswap32(DWORD value);
#else
#define __intrinsic_bswap32 BSWAP32
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// fast and small
__inline QWORD __intrinsic_bswap40(QWORD value)
{
	__asm
	{
		mov     edx, dword ptr [value]
		mov     eax, dword ptr [value + 4]
		shld    eax, edx, 24
		shl     edx, 24
		bswap   eax
		bswap   edx
	}
}
#elif defined(__BORLANDC__)
__declspec(naked) __msreturn QWORD __fastcall __fastcall_bswap40(DWORD high, DWORD low);
#define __intrinsic_bswap40(value) __fastcall_bswap40((QWORD)(value) >> 32, (DWORD)(value))
#else
#define __intrinsic_bswap40 BSWAP40
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// fast and small
__inline QWORD __intrinsic_bswap48(QWORD value)
{
	__asm
	{
		mov     edx, dword ptr [value]
		mov     eax, dword ptr [value + 4]
		shld    eax, edx, 16
		shl     edx, 16
		bswap   eax
		bswap   edx
	}
}
#elif defined(__BORLANDC__)
__declspec(naked) __msreturn QWORD __fastcall __fastcall_bswap48(DWORD high, DWORD low);
#define __intrinsic_bswap48(value) __fastcall_bswap48((QWORD)(value) >> 32, (DWORD)(value))
#else
#define __intrinsic_bswap48 BSWAP48
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// fast and small
__inline QWORD __intrinsic_bswap56(QWORD value)
{
	__asm
	{
		mov     edx, dword ptr [value]
		mov     eax, dword ptr [value + 4]
		shld    eax, edx, 8
		shl     edx, 8
		bswap   eax
		bswap   edx
	}
}
#elif defined(__BORLANDC__)
__declspec(naked) __msreturn QWORD __fastcall __fastcall_bswap56(DWORD high, DWORD low);
#define __intrinsic_bswap56(value) __fastcall_bswap56((QWORD)(value) >> 32, (DWORD)(value))
#else
#define __intrinsic_bswap56 BSWAP56
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// fast and small
__inline QWORD __intrinsic_bswap64(QWORD value)
{
	__asm
	{
		mov     edx, dword ptr [value]
		mov     eax, dword ptr [value + 4]
		bswap   edx
		bswap   eax
	}
}
#elif defined(__BORLANDC__)
__declspec(naked) __msreturn QWORD __fastcall __fastcall_bswap64(DWORD high, DWORD low);
#define __intrinsic_bswap64(value) __fastcall_bswap64((QWORD)(value) >> 32, (DWORD)(value))
#else
#define __intrinsic_bswap64 BSWAP64
#endif

#if defined(_MSC_VER)
#pragma intrinsic(_lrotl)
#pragma intrinsic(_lrotr)
#elif defined(__BORLANDC__)
__declspec(naked) unsigned long __fastcall __fastcall_lrotl(unsigned long value, unsigned char shift);
__declspec(naked) unsigned long __fastcall __fastcall_lrotr(unsigned long value, unsigned char shift);
#define _lrotl(value, shift) __fastcall_lrotl(value, (unsigned char)(shift))
#define _lrotr(value, shift) __fastcall_lrotr(value, (unsigned char)(shift))
#else
#define _lrotl(value, shift) (((unsigned long)(value) << (shift)) | ((unsigned long)(value) >> (32 - (shift))))
#define _lrotr(value, shift) (((unsigned long)(value) >> (shift)) | ((unsigned long)(value) << (32 - (shift))))
#endif

#if defined(_MSC_VER) && defined(_M_IX86)
// fast
__inline QWORD _lrotl64(QWORD value, int shift)
{
	__asm
	{
		mov     ecx, dword ptr [shift]
		test    ecx, 32
		jnz     L1
		mov     eax, dword ptr [value]
		mov     edx, dword ptr [value + 4]
		jmp     L2
	L1:
		mov     edx, dword ptr [value]
		mov     eax, dword ptr [value + 4]
	L2:
		and     ecx, 31
		mov     edi, eax
		shld    eax, edx, cl
		shld    edx, edi, cl
	}
}
// fast
__inline QWORD _lrotr64(QWORD value, int shift)
{
	__asm
	{
		mov     ecx, dword ptr [shift]
		test    ecx, 32
		jnz     L1
		mov     eax, dword ptr [value]
		mov     edx, dword ptr [value + 4]
		jmp     L2
	L1:
		mov     edx, dword ptr [value]
		mov     eax, dword ptr [value + 4]
	L2:
		and     ecx, 31
		mov     edi, eax
		shrd    eax, edx, cl
		shrd    edx, edi, cl
	}
}
#elif defined(__BORLANDC__)
__declspec(naked) __msreturn QWORD __fastcall __fastcall_lrotl64(DWORD low, DWORD high, int shift);
__declspec(naked) __msreturn QWORD __fastcall __fastcall_lrotr64(DWORD low, DWORD high, int shift);
#define _lrotl64(value, shift) __fastcall_lrotl64((DWORD)(value), (QWORD)(value) >> 32, shift)
#define _lrotr64(value, shift) __fastcall_lrotr64((DWORD)(value), (QWORD)(value) >> 32, shift)
#else
#define _lrotl64(value, shift) (((QWORD)(value) << (shift)) | ((QWORD)(value) >> (64 - (shift))))
#define _lrotr64(value, shift) (((QWORD)(value) >> (shift)) | ((QWORD)(value) << (64 - (shift))))
#endif

#if defined(_MSC_VER)
// small
#pragma intrinsic(__movsb)
#pragma intrinsic(__movsw)
#pragma intrinsic(__movsd)
#elif defined(__BORLANDC__)
__declspec(naked) void __fastcall __movsb(unsigned char *Destination, const unsigned char *Source, size_t Count);
__declspec(naked) void __fastcall __movsw(unsigned short *Destination, const unsigned short *Source, size_t Count);
__declspec(naked) void __fastcall __movsd(unsigned long *Destination, const unsigned long *Source, size_t Count);
#else
#define __movsb memcpy
#define __movsw(Destination, Source, Count) memcpy(Destination, Source, (size_t)(Count) * 2)
#define __movsd(Destination, Source, Count) memcpy(Destination, Source, (size_t)(Count) * 4)
#endif

#if defined(_MSC_VER)
#pragma intrinsic(__emul)
#pragma intrinsic(__emulu)
#elif defined(__BORLANDC__)
__declspec(naked) __msreturn __int64 __fastcall __emul(int a, int b);
__declspec(naked) __msreturn unsigned __int64 __fastcall __emulu(unsigned int a, unsigned int b);
#else
#define __emul(a, b) ((__int64)(int)(a) * (int)(b))
#define __emulu(a, b) ((unsigned __int64)(unsigned int)(a) * (unsigned int)(b))
#endif

#if defined(_MSC_VER)
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#elif defined(__BORLANDC__)
__declspec(naked) unsigned char __fastcall _BitScanForward(unsigned long *Index, unsigned long Mask);
__declspec(naked) unsigned char __fastcall _BitScanReverse(unsigned long *Index, unsigned long Mask);
#else
__inline unsigned char _BitScanForward(unsigned long *Index, unsigned long Mask)
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
__inline unsigned char _BitScanReverse(unsigned long *Index, unsigned long Mask)
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
__inline unsigned char _BitScanForward64(unsigned long *Index, unsigned __int64 Mask)
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
__inline unsigned char _BitScanReverse64(unsigned long *Index, unsigned __int64 Mask)
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
