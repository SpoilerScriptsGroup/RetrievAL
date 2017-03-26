#include "intrinsic.h"

#ifdef __BORLANDC__
#pragma warn -8070
#endif

#ifdef __BORLANDC__
__declspec(naked) WORD __fastcall __intrinsic_bswap16(WORD value)
{
	__asm
	{
		xchg    al, ah
		ret
	}
}

__declspec(naked) DWORD __fastcall __intrinsic_bswap24(DWORD value)
{
	__asm
	{
		shl     eax, 8
		bswap   eax
		ret
	}
}

__declspec(naked) DWORD __fastcall __intrinsic_bswap32(DWORD value)
{
	__asm
	{
		bswap   eax
		ret
	}
}

__declspec(naked) __msreturn QWORD __fastcall __fastcall_bswap40(DWORD high, DWORD low)
{
	__asm
	{
		shld    eax, edx, 24
		shl     edx, 24
		bswap   eax
		bswap   edx
		ret
	}
}

__declspec(naked) __msreturn QWORD __fastcall __fastcall_bswap48(DWORD high, DWORD low)
{
	__asm
	{
		shld    eax, edx, 16
		shl     edx, 16
		bswap   eax
		bswap   edx
		ret
	}
}

__declspec(naked) __msreturn QWORD __fastcall __fastcall_bswap56(DWORD high, DWORD low)
{
	__asm
	{
		shld    eax, edx, 8
		shl     edx, 8
		bswap   eax
		bswap   edx
		ret
	}
}

__declspec(naked) __msreturn QWORD __fastcall __fastcall_bswap64(DWORD high, DWORD low)
{
	__asm
	{
		bswap   edx
		bswap   eax
		ret
	}
}

__declspec(naked) unsigned long __fastcall __fastcall_lrotl(unsigned long value, unsigned char shift)
{
	__asm
	{
		mov     cl, dl
		rol     eax, cl
		ret
	}
}

__declspec(naked) unsigned long __fastcall __fastcall_lrotr(unsigned long value, unsigned char shift)
{
	__asm
	{
		mov     cl, dl
		ror     eax, cl
		ret
	}
}

__declspec(naked) __msreturn QWORD __fastcall __fastcall_lrotl64(DWORD low, DWORD high, int shift)
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

__declspec(naked) __msreturn QWORD __fastcall __fastcall_lrotr64(DWORD low, DWORD high, int shift)
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
		pop     esi
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

__declspec(naked) __msreturn __int64 __fastcall __emul(int a, int b)
{
	__asm
	{
		imul    edx
		ret
	}
}

__declspec(naked) __msreturn unsigned __int64 __fastcall __emulu(unsigned int a, unsigned int b)
{
	__asm
	{
		mul     edx
		ret
	}
}

__declspec(naked) unsigned char __fastcall _BitScanForward(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		mov     ecx, eax
		xor     eax, eax
		test    edx, edx
		jz      L1
		bsf     edx, edx
		inc     eax
		mov     dword ptr [ecx], edx
	L1:
		ret
	}
}

__declspec(naked) unsigned char __fastcall _BitScanReverse(unsigned long *Index, unsigned long Mask)
{
	__asm
	{
		mov     ecx, eax
		xor     eax, eax
		test    edx, edx
		jz      L1
		bsr     edx, edx
		inc     eax
		mov     dword ptr [ecx], edx
	L1:
		ret
	}
}
#endif
