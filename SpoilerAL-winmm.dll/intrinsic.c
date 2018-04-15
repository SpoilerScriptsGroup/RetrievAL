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

__declspec(naked) unsigned __int64 __msreturn __fastcall __fastcall_rotl64(DWORD low, DWORD high, int shift)
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

__declspec(naked) unsigned __int64 __msreturn __fastcall __fastcall_rotr64(DWORD low, DWORD high, int shift)
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

__declspec(naked) unsigned __int64 __msreturn __fastcall __emulu(unsigned int a, unsigned int b)
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

__declspec(naked) unsigned char __fastcall __fastcall_BitScanForward64(unsigned long *Index, DWORD low, DWORD high)
{
	__asm
	{
		bsf     edx, edx
		jnz     L1
		bsf     edx, ecx
		lea     edx, [edx + 32]
	L1:
		mov     dword ptr [eax], edx
		setnz   al
		ret
	}
}

__declspec(naked) unsigned char __fastcall __fastcall_BitScanReverse64(unsigned long *Index, DWORD low, DWORD high)
{
	__asm
	{
		bsr     ecx, ecx
		lea     ecx, [ecx + 32]
		jnz     L1
		bsr     ecx, edx
	L1:
		mov     dword ptr [eax], ecx
		setnz   al
		ret
	}
}

__declspec(naked) unsigned char __msfastcall _addcarry_u32(unsigned char c_in, unsigned int a, unsigned int b, unsigned int *out)
{
	__asm
	{
		#define b   (esp + 4)
		#define out (esp + 8)

		xor     al, al
		cmp     al, cl
		mov     eax, dword ptr [b]
		adc     edx, eax
		mov     eax, dword ptr [out]
		mov     dword ptr [eax], edx
		setc    al
		ret     8

		#undef b
		#undef out
	}
}

__declspec(naked) unsigned char __msfastcall _subborrow_u32(unsigned char b_in, unsigned int a, unsigned int b, unsigned int *out)
{
	__asm
	{
		#define b   (esp + 4)
		#define out (esp + 8)

		xor     al, al
		cmp     al, cl
		mov     eax, dword ptr [b]
		sbb     edx, eax
		mov     eax, dword ptr [out]
		mov     dword ptr [eax], edx
		setc    al
		ret     8

		#undef b
		#undef out
	}
}
#endif
