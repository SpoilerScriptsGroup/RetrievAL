#include "intrinsic.h"

#ifdef __BORLANDC__
#pragma warn -8070
#endif

#ifdef __BORLANDC__
__declspec(naked) unsigned short __fastcall _byteswap_ushort(unsigned short val)
{
	__asm
	{
		xchg    al, ah
		ret
	}
}

__declspec(naked) unsigned long __fastcall _byteswap_ulong(unsigned long val)
{
	__asm
	{
		bswap   eax
		ret
	}
}

__declspec(naked) unsigned __int64 __msreturn __fastcall __fastcall_byteswap_uint64(unsigned long high, unsigned long low)
{
	__asm
	{
		bswap   eax
		bswap   edx
		ret
	}
}

unsigned char __fastcall __fastcall_rotl8(unsigned char value, unsigned char shift)
{
	__asm
	{
		mov     cl, dl
		rol     al, cl
		ret
	}
}

unsigned char __fastcall __fastcall_rotr8(unsigned char value, unsigned char shift)
{
	__asm
	{
		mov     cl, dl
		ror     al, cl
		ret
	}
}

unsigned short __fastcall __fastcall_rotl16(unsigned short value, unsigned char shift)
{
	__asm
	{
		mov     cl, dl
		rol     ax, cl
		ret
	}
}

unsigned short __fastcall __fastcall_rotr16(unsigned short value, unsigned char shift)
{
	__asm
	{
		mov     cl, dl
		ror     ax, cl
		ret
	}
}

__declspec(naked) unsigned int __fastcall __fastcall_rotl(unsigned int value, int shift)
{
	__asm
	{
		mov     cl, dl
		rol     eax, cl
		ret
	}
}

__declspec(naked) unsigned int __fastcall __fastcall_rotr(unsigned int value, int shift)
{
	__asm
	{
		mov     cl, dl
		ror     eax, cl
		ret
	}
}

__declspec(naked) unsigned __int64 __msreturn __fastcall __fastcall_rotl64(uint32_t low, uint32_t high, int shift)
{
	__asm
	{
		push    ebx
		mov     ebx, eax
		test    ecx, 32
		jz      L1
		mov     ebx, edx
		mov     edx, eax
		mov     eax, ebx
	L1:
		and     ecx, 31
		shld    eax, edx, cl
		shld    edx, ebx, cl
		pop     ebx
		ret
	}
}

__declspec(naked) unsigned __int64 __msreturn __fastcall __fastcall_rotr64(uint32_t low, uint32_t high, int shift)
{
	__asm
	{
		push    ebx
		mov     ebx, eax
		test    ecx, 32
		jz      L1
		mov     ebx, edx
		mov     edx, eax
		mov     eax, ebx
	L1:
		and     ecx, 31
		shrd    eax, edx, cl
		shrd    edx, ebx, cl
		pop     ebx
		ret
	}
}

__declspec(naked) void __fastcall __movsb(unsigned char *Destination, const unsigned char *Source, size_t Count)
{
	__asm
	{
		push    esi
		push    edi
		mov     esi, edx
		mov     edi, eax
		rep movsb
		pop     edi
		pop     esi
		ret
	}
}

__declspec(naked) void __fastcall __movsw(unsigned short *Destination, const unsigned short *Source, size_t Count)
{
	__asm
	{
		push    esi
		push    edi
		mov     esi, edx
		mov     edi, eax
		rep movsw
		pop     edi
		pop     esi
		ret
	}
}

__declspec(naked) void __fastcall __movsd(unsigned long *Destination, const unsigned long *Source, size_t Count)
{
	__asm
	{
		push    esi
		push    edi
		mov     esi, edx
		mov     edi, eax
		rep movsd
		pop     edi
		pop     esi
		ret
	}
}

__declspec(naked) void __fastcall __fastcall_stosb(unsigned char Data, unsigned char *Dest, size_t Count)
{
	__asm
	{
		push    edi
		mov     edi, edx
		rep stosb
		pop     edi
		ret
	}
}

__declspec(naked) void __fastcall __fastcall_stosw(unsigned short Data, unsigned short *Dest, size_t Count)
{
	__asm
	{
		push    edi
		mov     edi, edx
		rep stosw
		pop     edi
		ret
	}
}

__declspec(naked) void __fastcall __fastcall_stosd(unsigned long Data, unsigned long *Dest, size_t Count)
{
	__asm
	{
		push    edi
		mov     edi, edx
		rep stosd
		pop     edi
		ret
	}
}

__declspec(naked) unsigned char __fastcall _BitScanForward(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		bsf     edx, edx
		mov     dword ptr [eax], edx
		setnz   al
		ret
	}
}

__declspec(naked) unsigned char __fastcall _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		bsr     edx, edx
		mov     dword ptr [eax], edx
		setnz   al
		ret
	}
}

__declspec(naked) unsigned char __fastcall __fastcall_BitScanForward64(uint32_t low, uint32_t high, unsigned long *Index)
{
	__asm
	{
		bsf     eax, eax
		jnz     L1
		bsf     eax, edx
		lea     eax, [eax + 32]
	L1:
		mov     dword ptr [ecx], eax
		setnz   al
		ret
	}
}

__declspec(naked) unsigned char __fastcall __fastcall_BitScanReverse64(uint32_t low, uint32_t high, unsigned long *Index)
{
	__asm
	{
		bsr     edx, edx
		lea     edx, [edx + 32]
		jnz     L1
		bsr     edx, eax
	L1:
		mov     dword ptr [ecx], edx
		setnz   al
		ret
	}
}

__declspec(naked) unsigned __int64 __msreturn __msfastcall __shiftleft128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift)
{
	#define LO(x) (x)
	#define HI(x) ((x) + 4)

	__asm
	{
		#define LowPart  (esp + 4)
		#define HighPart (esp + 12)

		and     cl, 63
		jz      L1
		sub     cl, 32
		jb      L2
		mov     eax, dword ptr [HI(LowPart)]
		mov     edx, dword ptr [LO(HighPart)]
		push    esi
		jz      L4
		mov     esi, dword ptr [LO(LowPart) + 4]
		jmp     L3

	L1:
		mov     eax, dword ptr [LO(HighPart)]
		mov     edx, dword ptr [HI(HighPart)]
		jmp     L5

	L2:
		push    esi
		add     cl, 32
		mov     edx, dword ptr [HI(HighPart) + 4]
		mov     eax, dword ptr [LO(HighPart) + 4]
		mov     esi, dword ptr [HI(LowPart) + 4]
	L3:
		shld    edx, eax, cl
		shld    eax, esi, cl
	L4:
		pop     esi
	L5:
		ret     16

		#undef LowPart
		#undef HighPart
	}

	#undef LO
	#undef HI
}

__declspec(naked) unsigned __int64 __msreturn __msfastcall __shiftright128(unsigned __int64 LowPart, unsigned __int64 HighPart, unsigned char Shift)
{
	#define LO(x) (x)
	#define HI(x) ((x) + 4)

	__asm
	{
		#define LowPart  (esp + 4)
		#define HighPart (esp + 12)

		and     cl, 63
		jz      L1
		sub     cl, 32
		jb      L2
		mov     eax, dword ptr [HI(LowPart)]
		mov     edx, dword ptr [LO(HighPart)]
		push    esi
		jz      L4
		mov     esi, dword ptr [HI(HighPart) + 4]
		jmp     L3

	L1:
		mov     eax, dword ptr [LO(LowPart)]
		mov     edx, dword ptr [HI(LowPart)]
		jmp     L5

	L2:
		push    esi
		add     cl, 32
		mov     eax, dword ptr [LO(LowPart) + 4]
		mov     edx, dword ptr [HI(LowPart) + 4]
		mov     esi, dword ptr [LO(HighPart) + 4]
	L3:
		shrd    eax, edx, cl
		shrd    edx, esi, cl
	L4:
		pop     esi
	L5:
		ret     16

		#undef LowPart
		#undef HighPart
	}

	#undef LO
	#undef HI
}

__declspec(naked) unsigned char __fastcall _add_u32(unsigned int a, unsigned int b, unsigned int *_out)
{
	__asm
	{
		add     eax, edx
		mov     dword ptr [ecx], eax
		setc    al
		ret
	}
}

__declspec(naked) unsigned char __fastcall _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *_out)
{
	__asm
	{
		#define out (esp + 4)

		add     al, -1
		adc     edx, ecx
		mov     eax, dword ptr [out]
		mov     dword ptr [eax], edx
		setc    al
		ret     4

		#undef out
	}
}

__declspec(naked) unsigned char __fastcall _sub_u32(unsigned int a, unsigned int b, unsigned int *_out)
{
	__asm
	{
		sub     eax, edx
		mov     dword ptr [ecx], eax
		setc    al
		ret
	}
}

__declspec(naked) unsigned char __fastcall _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *_out)
{
	__asm
	{
		#define out (esp + 4)

		add     al, -1
		sbb     edx, ecx
		mov     eax, dword ptr [out]
		mov     dword ptr [eax], edx
		setc    al
		ret     4

		#undef out
	}
}

__declspec(naked) unsigned char __fastcall _add_u64(uint64_t a, uint64_t b, uint64_t *_out)
{
	__asm
	{
		#define a   (esp + 4)
		#define b   (esp + 12)
		#define out eax

		mov     ecx, dword ptr [a]
		mov     edx, dword ptr [b]
		add     ecx, edx
		mov     edx, dword ptr [a + 4]
		adc     edx, dword ptr [b + 4]
		mov     dword ptr [eax], ecx
		mov     dword ptr [eax + 4], edx
		setc    al
		ret     16

		#undef a
		#undef b
		#undef out
	}
}

__declspec(naked) unsigned char __fastcall _addcarry_u64(unsigned char c_in, uint64_t a, uint64_t b, uint64_t *_out)
{
	__asm
	{
		#define c_in al
		#define a    (esp + 4)
		#define b    (esp + 12)
		#define out  edx

		add     al, -1
		mov     ecx, dword ptr [a]
		adc     ecx, dword ptr [b]
		mov     eax, dword ptr [a + 4]
		adc     eax, dword ptr [b + 4]
		mov     dword ptr [edx], ecx
		mov     dword ptr [edx + 4], eax
		setc    al
		ret     16

		#undef c_in
		#undef a
		#undef b
		#undef out
	}
}

__declspec(naked) unsigned char __fastcall _sub_u64(uint64_t a, uint64_t b, uint64_t *_out)
{
	__asm
	{
		#define a   (esp + 4)
		#define b   (esp + 12)
		#define out eax

		mov     ecx, dword ptr [a]
		mov     edx, dword ptr [b]
		sub     ecx, edx
		mov     edx, dword ptr [a + 4]
		sbb     edx, dword ptr [b + 4]
		mov     dword ptr [eax], ecx
		mov     dword ptr [eax + 4], edx
		setc    al
		ret     16

		#undef a
		#undef b
		#undef out
	}
}

__declspec(naked) unsigned char __fastcall _subborrow_u64(unsigned char b_in, uint64_t a, uint64_t b, uint64_t *_out)
{
	__asm
	{
		#define b_in al
		#define a    (esp + 4)
		#define b    (esp + 12)
		#define out  edx

		add     al, -1
		mov     ecx, dword ptr [a]
		sbb     ecx, dword ptr [b]
		mov     eax, dword ptr [a + 4]
		sbb     eax, dword ptr [b + 4]
		mov     dword ptr [edx], ecx
		mov     dword ptr [edx + 4], eax
		setc    al
		ret     16

		#undef b_in
		#undef a
		#undef b
		#undef out
	}
}

__declspec(naked) __int64 __msreturn __fastcall __emul(int a, int b)
{
	__asm
	{
		imul    edx
		ret
	}
}

__declspec(naked) unsigned __int64 __msreturn __fastcall __emulu(unsigned int a, unsigned int b)
{
	__asm
	{
		mul     edx
		ret
	}
}

__declspec(naked) int __fastcall __fastcall_dvrm64(int32_t low, int32_t high, int divisor, int *remainder)
{
	__asm
	{
		idiv    ecx
		mov     ecx, dword ptr [esp + 4]
		mov     dword ptr [ecx], edx
		ret     4
	}
}

__declspec(naked) int __fastcall __fastcall_div64(int32_t low, int32_t high, int divisor)
{
	__asm
	{
		idiv    ecx
		ret
	}
}

__declspec(naked) unsigned int __fastcall __fastcall_udvrm64(uint32_t low, uint32_t high, unsigned int divisor, unsigned int *remainder)
{
	__asm
	{
		div     ecx
		mov     ecx, dword ptr [esp + 4]
		mov     dword ptr [ecx], edx
		ret     4
	}
}

__declspec(naked) unsigned int __fastcall __fastcall_udiv64(uint32_t low, uint32_t high, unsigned int divisor)
{
	__asm
	{
		div     ecx
		ret
	}
}
#endif

#if !defined(_MSC_VER) || !defined(_M_X64)
#ifndef _M_IX86
int64_t __msreturn __stdcall __mulh(int64_t Multiplicand, int64_t Multiplier)
{
	int64_t HighProduct;

	MULH(Multiplicand, Multiplier, &HighProduct);
	return HighProduct;
}
#else
__declspec(naked) int64_t __msreturn __stdcall __mulh(int64_t Multiplicand, int64_t Multiplier)
{
	#define LO(x) (x)
	#define HI(x) ((x) + 4)

	__asm
	{
		#define Multiplicand (esp + 4)
		#define Multiplier   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     eax, dword ptr [LO(Multiplicand + 16)]
		mov     ecx, dword ptr [HI(Multiplicand + 16)]
		mov     ebp, dword ptr [LO(Multiplier + 16)]
		mov     esi, ecx
		sar     ecx, 31
		mov     ebx, dword ptr [HI(Multiplier + 16)]
		xor     eax, ecx
		mov     edi, ebx
		sar     ebx, 31
		sub     eax, ecx
		adc     esi, ecx
		xor     ebp, ebx
		xor     edi, ebx
		sub     ebp, ebx
		sbb     edi, ebx
		xor     ebx, ecx
		xor     esi, ecx
		push    ebx
		push    esi
		push    eax
		mul     ebp
		mov     ecx, eax
		mov     eax, esi
		mov     ebx, edx
		xor     esi, esi
		mul     ebp
		add     ebx, eax
		pop     eax
		adc     esi, edx
		mov     ebp, edi
		mul     edi
		xor     edi, edi
		add     ebx, eax
		adc     esi, edx
		pop     eax
		adc     edi, edi
		mul     ebp
		pop     ebp
		add     eax, esi
		adc     edx, edi
		xor     ecx, ebp
		xor     ebx, ebp
		xor     eax, ebp
		xor     edx, ebp
		sub     ecx, ebp
		sbb     ebx, ebp
		pop     edi
		sbb     eax, ebp
		pop     esi
		sbb     edx, ebp
		pop     ebp
		pop     ebx
		ret     16

		#undef Multiplicand
		#undef Multiplier
	}

	#undef LO
	#undef HI
}
#endif

#ifndef _M_IX86
uint64_t __msreturn __stdcall __umulh(uint64_t Multiplicand, uint64_t Multiplier)
{
	uint64_t HighProduct;

	UMULH(Multiplicand, Multiplier, &HighProduct);
	return HighProduct;
}
#else
__declspec(naked) uint64_t __msreturn __stdcall __umulh(uint64_t Multiplicand, uint64_t Multiplier)
{
	#define LO(x) (x)
	#define HI(x) ((x) + 4)

	__asm
	{
		#define Multiplicand (esp + 4)
		#define Multiplier   (esp + 12)

		push    esi
		push    edi
		mov     eax, dword ptr [LO(Multiplicand + 8)]
		mov     ecx, dword ptr [LO(Multiplier + 8)]
		mul     ecx
		mov     eax, dword ptr [HI(Multiplicand + 8)]
		mov     esi, edx
		mul     ecx
		xor     edi, edi
		add     esi, eax
		adc     edi, edx
		mov     eax, dword ptr [LO(Multiplicand + 8)]
		mov     ecx, dword ptr [HI(Multiplier + 8)]
		mul     ecx
		add     eax, esi
		mov     esi, 0
		adc     edi, edx
		mov     eax, dword ptr [HI(Multiplicand + 8)]
		adc     esi, esi
		mul     ecx
		add     eax, edi
		pop     edi
		adc     edx, esi
		pop     esi
		ret     16

		#undef Multiplicand
		#undef Multiplier
	}

	#undef LO
	#undef HI
}
#endif

#ifndef _M_IX86
int64_t __msreturn __stdcall _mul128(int64_t Multiplicand, int64_t Multiplier, int64_t *HighProduct)
{
	int64_t LowProduct;

	MUL128(Multiplicand, Multiplier, &LowProduct, HighProduct);
	return LowProduct;
}
#else
__declspec(naked) int64_t __msreturn __stdcall _mul128(int64_t Multiplicand, int64_t Multiplier, int64_t *HighProduct)
{
	#define LO(x) (x)
	#define HI(x) ((x) + 4)

	__asm
	{
		#define Multiplicand (esp + 4)
		#define Multiplier   (esp + 12)
		#define HighProduct  (esp + 20)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     eax, dword ptr [LO(Multiplicand + 16)]
		mov     esi, dword ptr [HI(Multiplicand + 16)]
		mov     ebp, dword ptr [LO(Multiplier + 16)]
		mov     ecx, esi
		sar     esi, 31
		mov     edi, dword ptr [HI(Multiplier + 16)]
		xor     eax, esi
		mov     ebx, edi
		sar     edi, 31
		sub     eax, esi
		adc     ecx, esi
		xor     ebp, edi
		xor     ebx, edi
		sub     ebp, edi
		sbb     ebx, edi
		xor     edi, esi
		xor     ecx, esi
		push    edi
		push    ecx
		push    eax
		mul     ebp
		mov     esi, eax
		mov     eax, ecx
		mov     edi, edx
		xor     ecx, ecx
		mul     ebp
		add     edi, eax
		pop     eax
		adc     ecx, edx
		mov     ebp, ebx
		mul     ebx
		xor     ebx, ebx
		add     edi, eax
		adc     ecx, edx
		pop     eax
		adc     ebx, ebx
		mul     ebp
		pop     ebp
		add     ecx, eax
		adc     ebx, edx
		xor     esi, ebp
		xor     edi, ebp
		xor     ecx, ebp
		xor     ebx, ebp
		sub     esi, ebp
		sbb     edi, ebp
		mov     eax, esi
		sbb     ecx, ebp
		mov     esi, dword ptr [HighProduct + 16]
		sbb     ebx, ebp
		mov     edx, edi
		mov     dword ptr [LO(esi)], ecx
		mov     dword ptr [HI(esi)], ebx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret     20

		#undef Multiplicand
		#undef Multiplier
		#undef HighProduct
	}

	#undef LO
	#undef HI
}
#endif

#ifndef _M_IX86
uint64_t __msreturn __stdcall _umul128(uint64_t Multiplicand, uint64_t Multiplier, uint64_t *HighProduct)
{
	uint64_t LowProduct;

	UMUL128(Multiplicand, Multiplier, &LowProduct, HighProduct);
	return LowProduct;
}
#else
__declspec(naked) uint64_t __msreturn __stdcall _umul128(uint64_t Multiplicand, uint64_t Multiplier, uint64_t *HighProduct)
{
	#define LO(x) (x)
	#define HI(x) ((x) + 4)

	__asm
	{
		#define Multiplicand (esp + 4)
		#define Multiplier   (esp + 12)
		#define HighProduct  (esp + 20)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     eax, dword ptr [LO(Multiplicand + 16)]
		mov     ebp, dword ptr [LO(Multiplier + 16)]
		mul     ebp
		mov     esi, eax
		mov     eax, dword ptr [HI(Multiplicand + 16)]
		mov     edi, edx
		xor     ecx, ecx
		mul     ebp
		add     edi, eax
		mov     eax, dword ptr [LO(Multiplicand + 16)]
		adc     ecx, edx
		mov     ebp, dword ptr [HI(Multiplier + 16)]
		mul     ebp
		xor     ebx, ebx
		add     edi, eax
		adc     ecx, edx
		mov     eax, dword ptr [HI(Multiplicand + 16)]
		adc     ebx, ebx
		mul     ebp
		add     ecx, eax
		adc     ebx, edx
		mov     ebp, dword ptr [HighProduct + 16]
		mov     eax, esi
		mov     edx, edi
		mov     dword ptr [LO(ebp)], ecx
		mov     dword ptr [HI(ebp)], ebx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret     20

		#undef Multiplicand
		#undef Multiplier
		#undef HighProduct
	}

	#undef LO
	#undef HI
}
#endif
#endif

#if !defined(_MSC_VER) || _MSC_VER < 1920 || !defined(_M_X64)
#ifndef _M_IX86
int64_t __msreturn __stdcall _div128(int64_t highDividend, int64_t lowDividend, int64_t divisor, int64_t *remainder)
{
	#define PREV_UDIV128 UDIV128
	#undef UDIV128
	#define UDIV128 _udiv128

	int64_t quotient;

	DIV128(highDividend, lowDividend, divisor, &quotient, remainder);
	return quotient;

	#undef UDIV128
	#define UDIV128 PREV_UDIV128
	#undef PREV_UDIV128
}
#else
__declspec(naked) int64_t __msreturn __stdcall _div128(int64_t highDividend, int64_t lowDividend, int64_t divisor, int64_t *remainder)
{
	#define LO(x) (x)
	#define HI(x) ((x) + 4)

	__asm
	{
		#define highDividend (esp + 4)
		#define lowDividend  (esp + 12)
		#define divisor      (esp + 20)
		#define remainder    (esp + 28)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     esi, dword ptr [HI(divisor) + 16]
		mov     edi, dword ptr [LO(divisor) + 16]
		sar     esi, 31
		mov     eax, dword ptr [HI(divisor) + 16]
		add     edi, esi
		mov     ebp, dword ptr [HI(highDividend) + 16]
		adc     eax, esi
		mov     edx, ebp
		sar     ebp, 31
		mov     ecx, dword ptr [remainder + 16]
		xor     eax, esi
		xor     edi, esi
		push    ecx
		push    eax
		mov     eax, dword ptr [LO(lowDividend) + 24]
		xor     esi, ebp
		add     eax, ebp
		mov     ecx, dword ptr [LO(highDividend) + 24]
		adc     edx, ebp
		mov     ebx, dword ptr [HI(highDividend) + 24]
		adc     ecx, ebp
		push    edi
		adc     ebx, ebp
		mov     edi, dword ptr [remainder + 28]
		xor     ebx, ebp
		xor     ecx, ebp
		xor     edx, ebp
		xor     eax, ebp
		push    ebx
		push    ecx
		push    edx
		push    eax
		call    _udiv128
		mov     ecx, dword ptr [edi]
		mov     ebx, dword ptr [edi + 4]
		xor     edx, esi
		add     eax, esi
		sbb     edx, esi
		add     ecx, esi
		adc     ebx, esi
		xor     eax, esi
		xor     ecx, esi
		xor     ebx, esi
		mov     dword ptr [edi], ecx
		mov     dword ptr [edi + 4], ebx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret     28

		#undef highDividend
		#undef lowDividend
		#undef divisor
		#undef remainder
	}

	#undef LO
	#undef HI
}
#endif

#ifndef _M_IX86
uint64_t __msreturn __stdcall _udiv128(uint64_t highDividend, uint64_t lowDividend, uint64_t divisor, uint64_t *remainder)
{
	uint64_t quotient;

	UDIV128(highDividend, lowDividend, divisor, &quotient, remainder);
	return quotient;
}
#else
__declspec(naked) uint64_t __msreturn __stdcall _udiv128(uint64_t highDividend, uint64_t lowDividend, uint64_t divisor, uint64_t *remainder)
{
	void __cdecl _aullrem();
	void __cdecl _aulldvrm();

	#define LO(x) (x)
	#define HI(x) ((x) + 4)

	__asm
	{
		#define highDividend (esp + 4)
		#define lowDividend  (esp + 12)
		#define divisor      (esp + 20)
		#define remainder    (esp + 28)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     eax, dword ptr [LO(highDividend) + 16]
		mov     edx, dword ptr [HI(highDividend) + 16]
		mov     esi, dword ptr [LO(lowDividend) + 16]
		mov     edi, dword ptr [HI(lowDividend) + 16]
		mov     ebx, dword ptr [LO(divisor) + 16]
		mov     ebp, dword ptr [HI(divisor) + 16]
		sub     esp, 28
		mov     ecx, eax
		push    edi
		push    esi
		xor     edi, edi
		xor     esi, esi

		#define dividend1   (esp)
		#define dividend2   (esp + 4)
		#define dividend3   (esp + 8)
		#define dividend4   (esp + 12)
		#define quotient1   (esp + 16)
		#define quotient2   (esp + 20)
		#define addend1     (esp + 24)
		#define addend2     (esp + 28)
		#define subtrahend1 (esp + 32)

		or      eax, edx
		jz      L10
		push    ebp
		push    ebx
		push    edx
		push    ecx
		call    _aullrem
		mov     dword ptr [dividend3], eax
		mov     dword ptr [dividend4], edx
		test    edx, edx
		jnz     L1
		test    eax, eax
		jz      L10
		test    ebp, ebp
		jnz     L3
		bsr     ecx, ebx
		bsr     esi, eax
		jmp     L2

		align   16
	L1:
		bsr     ecx, ebp
		bsr     esi, edx
	L2:
		mov     edi, 80000000H
		sub     ecx, esi
		xor     eax, eax
		mov     esi, ecx
		sub     ecx, 1
		ja      L5
		sub     esi, ecx
		jmp     L6

		align   16
	L3:
		bsr     ecx, ebp
		bsr     esi, eax
		sub     ecx, esi
		jbe     L4
		mov     eax, 80000000H
		mov     esi, ecx
		xor     edi, edi
		add     esi, 32
		dec     ecx
		jz      L6
		shr     eax, cl
		jmp     L6

		align   16
	L4:
		mov     edi, 80000000H
		add     ecx, 31
		xor     eax, eax
		lea     esi, [ecx + 1]
	L5:
		shr     edi, cl
	L6:
		xor     edx, edx
		mov     ecx, esi
		mov     dword ptr [quotient1], edx
		mov     dword ptr [quotient2], edx
		mov     dword ptr [addend1], eax
		xor     eax, eax
		and     ecx, 31
		jz      L7
		shrd    eax, ebx, cl
		shrd    ebx, ebp, cl
		shr     ebp, cl
		cmp     esi, 32
		jae     L7
		mov     edx, ebp
		mov     ebp, ebx
		mov     ebx, eax
		xor     eax, eax
	L7:
		mov     dword ptr [addend2], edi
		mov     dword ptr [subtrahend1], eax
		mov     esi, dword ptr [dividend1]
		jmp     L9

		align   16
	L8:
		shr     edx, 1
		mov     eax, dword ptr [subtrahend1]
		rcr     ebp, 1
		mov     edi, dword ptr [addend2]
		rcr     ebx, 1
		mov     ecx, dword ptr [addend1]
		rcr     eax, 1
		mov     esi, dword ptr [dividend1]
		shr     edi, 1
		mov     dword ptr [subtrahend1], eax
		rcr     ecx, 1
		mov     dword ptr [addend2], edi
		mov     dword ptr [addend1], ecx
	L9:
		sub     esi, eax
		mov     edi, dword ptr [dividend2]
		sbb     edi, ebx
		mov     eax, dword ptr [dividend3]
		sbb     eax, ebp
		mov     ecx, dword ptr [dividend4]
		sbb     ecx, edx
		jb      L8
		mov     dword ptr [dividend1], esi
		mov     dword ptr [dividend2], edi
		mov     esi, dword ptr [quotient1]
		mov     edi, dword ptr [quotient2]
		or      esi, dword ptr [addend1]
		or      edi, dword ptr [addend2]
		mov     dword ptr [dividend3], eax
		mov     dword ptr [dividend4], ecx
		mov     dword ptr [quotient1], esi
		mov     dword ptr [quotient2], edi
		or      eax, ecx
		jnz     L8
		mov     ebx, dword ptr [LO(divisor) + 52]
		mov     ebp, dword ptr [HI(divisor) + 52]
	L10:
		mov     dword ptr [esp + 8], ebx
		mov     dword ptr [esp + 12], ebp
		call    _aulldvrm
		mov     ebp, dword ptr [remainder + 36]
		add     eax, esi
		adc     edx, edi
		add     esp, 20
		mov     dword ptr [ebp], ecx
		mov     dword ptr [ebp + 4], ebx

		#undef dividend1
		#undef dividend2
		#undef dividend3
		#undef dividend4
		#undef quotient1
		#undef quotient2
		#undef addend1
		#undef addend2
		#undef subtrahend1

		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret     28

		#undef highDividend
		#undef lowDividend
		#undef divisor
		#undef remainder
	}

	#undef LO
	#undef HI
}
#endif
#endif
