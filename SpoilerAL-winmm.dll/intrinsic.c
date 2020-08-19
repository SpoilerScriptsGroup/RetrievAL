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

__declspec(naked) unsigned int __fastcall __fastcall_udiv64(uint32_t low, uint32_t high, unsigned int divisor, unsigned int *remainder)
{
	__asm
	{
		div     ecx
		mov     ecx, dword ptr [esp + 4]
		mov     dword ptr [ecx], edx
		ret     4
	}
}

__declspec(naked) unsigned int __fastcall __fastcall__udiv64(uint32_t low, uint32_t high, unsigned int divisor)
{
	__asm
	{
		div     ecx
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
#endif

#if !defined(_MSC_VER) || !defined(_M_X64)
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
		mov     ecx, dword ptr [LO(Multiplier + 16)]
		mul     ecx
		mov     ebx, eax
		mov     eax, dword ptr [HI(Multiplicand + 16)]
		mov     ebp, edx
		xor     esi, esi
		mul     ecx
		add     ebp, eax
		mov     eax, dword ptr [LO(Multiplicand + 16)]
		adc     esi, edx
		mov     ecx, dword ptr [HI(Multiplier + 16)]
		mul     ecx
		xor     edi, edi
		add     ebp, eax
		adc     esi, edx
		mov     eax, dword ptr [HI(Multiplicand + 16)]
		adc     edi, edi
		mul     ecx
		add     esi, eax
		adc     edi, edx
		mov     ecx, dword ptr [HighProduct + 16]
		mov     eax, ebx
		mov     edx, ebp
		mov     dword ptr [LO(ecx)], esi
		mov     dword ptr [HI(ecx)], edi
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
		mov     ecx, dword ptr [HI(Multiplicand + 16)]
		mov     esi, dword ptr [LO(Multiplier + 16)]
		mov     ebx, ecx
		sar     ecx, 31
		mov     edi, dword ptr [HI(Multiplier + 16)]
		xor     eax, ecx
		mov     ebp, edi
		sar     edi, 31
		sub     eax, ecx
		adc     ebx, ecx
		xor     esi, edi
		xor     ebp, edi
		sub     esi, edi
		sbb     ebp, edi
		xor     edi, ecx
		xor     ebx, ecx
		push    edi
		push    ebx
		push    eax
		mul     esi
		mov     ecx, eax
		mov     eax, ebx
		mov     edi, edx
		xor     ebx, ebx
		mul     esi
		add     edi, eax
		pop     eax
		adc     ebx, edx
		mov     esi, ebp
		mul     ebp
		xor     ebp, ebp
		add     edi, eax
		adc     ebx, edx
		pop     eax
		adc     ebp, ebp
		mul     esi
		pop     esi
		add     ebx, eax
		adc     ebp, edx
		xor     ecx, esi
		xor     edi, esi
		xor     ebx, esi
		xor     ebp, esi
		mov     eax, ecx
		mov     edx, edi
		sub     eax, esi
		sbb     edx, esi
		mov     ecx, dword ptr [HighProduct + 16]
		sbb     ebx, esi
		pop     edi
		sbb     ebp, esi
		pop     esi
		mov     dword ptr [LO(ecx)], ebx
		mov     dword ptr [HI(ecx)], ebp
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
uint64_t __msreturn __stdcall _udiv128(uint64_t highDividend, uint64_t lowDividend, uint64_t divisor, uint64_t *remainder)
{
	uint64_t quotient;

	UDIV128(highDividend, lowDividend, divisor, &quotient, remainder);
	return quotient;
}
#else
__declspec(naked) uint64_t __msreturn __stdcall _udiv128(uint64_t highDividend, uint64_t lowDividend, uint64_t divisor, uint64_t *remainder)
{
	void __cdecl _allshl();
	void __cdecl _aullshr();
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
		mov     ecx, dword ptr [LO(lowDividend) + 16]
		mov     ebx, dword ptr [HI(lowDividend) + 16]
		mov     esi, dword ptr [LO(divisor) + 16]
		mov     edi, dword ptr [HI(divisor) + 16]
		xor     ebp, ebp
		push    ebx
		push    ecx
		push    ebp
		push    ebp
		push    ebp
		mov     ecx, eax
		sub     esp, 16
		or      eax, edx
		jz      L7
		push    edi
		push    esi
		push    edx
		push    ecx
		call    _aullrem
		mov     ecx, eax
		mov     ebx, eax
		or      ecx, edx
		jz      L7
		bsr     ebp, edi
		lea     ebp, [ebp + 32]
		jnz     L1
		bsr     ebp, esi
	L1:
		bsr     ecx, edx
		lea     ecx, [ecx + 32]
		jnz     L2
		bsr     ecx, eax
	L2:
		mov     dword ptr [esp], 0
		mov     dword ptr [esp + 4], eax
		mov     dword ptr [esp + 8], edx
		mov     edi, 80000000H
		sub     ebp, ecx
		jz      L3
		lea     ecx, [ebp - 1]
		xor     eax, eax
		mov     edx, edi
		call    _aullshr
		mov     dword ptr [esp], eax
		mov     edi, edx
		jmp     L4

	L3:
		inc     ebp
	L4:
		mov     edx, dword ptr [HI(divisor) + 52]
		mov     eax, esi
		mov     ecx, ebp
		call    _aullshr
		mov     ebx, eax
		mov     esi, edx
		mov     eax, dword ptr [LO(divisor) + 52]
		mov     ecx, 64
		mov     edx, dword ptr [HI(divisor) + 52]
		sub     ecx, ebp
		call    _allshl
		mov     ebp, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 28]
		mov     dword ptr [esp + 16], ebp
		mov     dword ptr [esp + 12], eax
		mov     ebp, eax
		jmp     L6

		align   16
	L5:
		shr     edi, 1
		mov     eax, dword ptr [esp]
		rcr     eax, 1
		mov     ecx, dword ptr [esp + 8]
		shr     esi, 1
		mov     ebp, dword ptr [esp + 12]
		rcr     ebx, 1
		mov     dword ptr [esp], eax
		rcr     edx, 1
		mov     dword ptr [esp + 16], ecx
		rcr     ebp, 1
		mov     ecx, dword ptr [esp + 28]
		mov     dword ptr [esp + 12], ebp
	L6:
		mov     eax, dword ptr [esp + 32]
		sub     ecx, ebp
		sbb     eax, edx
		mov     ebp, dword ptr [esp + 4]
		sbb     ebp, ebx
		sbb     dword ptr [esp + 16], esi
		jb      L5
		mov     dword ptr [esp + 4], ebp
		mov     dword ptr [esp + 28], ecx
		mov     ecx, dword ptr [esp + 20]
		mov     ebp, dword ptr [esp]
		or      ecx, ebp
		mov     ebp, dword ptr [esp + 24]
		mov     dword ptr [esp + 20], ecx
		or      ebp, edi
		mov     dword ptr [esp + 32], eax
		mov     dword ptr [esp + 24], ebp
		mov     ecx, dword ptr [esp + 16]
		mov     eax, dword ptr [esp + 4]
		mov     dword ptr [esp + 16], ebp
		mov     dword ptr [esp + 8], ecx
		or      eax, ecx
		jnz     L5
		mov     edi, dword ptr [HI(divisor) + 52]
		mov     esi, dword ptr [LO(divisor) + 52]
		mov     ebp, dword ptr [esp + 20]

		align   16
	L7:
		mov     edx, dword ptr [esp + 32]
		mov     eax, dword ptr [esp + 28]
		push    edi
		push    esi
		push    edx
		push    eax
		call    _aulldvrm
		mov     esi, dword ptr [remainder + 52]
		add     eax, ebp
		adc     edx, dword ptr [esp + 16]
		add     esp, 36
		mov     dword ptr [esi], ecx
		mov     dword ptr [esi + 4], ebx
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
