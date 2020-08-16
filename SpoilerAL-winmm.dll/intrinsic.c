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
uint64_t __msreturn __stdcall __umulh(uint64_t a, uint64_t b)
{
	uint64_t HighProduct;

	__UMULH(&HighProduct, a, b);
	return HighProduct;
}

uint64_t __msreturn __stdcall _umul128(uint64_t Multiplicand, uint64_t Multiplier, uint64_t *HighProduct)
{
	uint64_t LowProduct;

	_UMUL128(&LowProduct, Multiplicand, Multiplier, HighProduct);
	return LowProduct;
}

int64_t __msreturn __stdcall _mul128(int64_t Multiplicand, int64_t Multiplier, int64_t *HighProduct)
{
	int64_t LowProduct;

	_MUL128(&LowProduct, Multiplicand, Multiplier, HighProduct);
	return LowProduct;
}

uint64_t __msreturn __stdcall _udiv128(uint64_t highDividend, uint64_t lowDividend, uint64_t divisor, uint64_t *remainder)
{
	uint64_t quatient;

	_UDIV128(&quatient, highDividend, lowDividend, divisor, remainder);
	return quatient;
}
#endif
