//***
//llshl.c - long shift left
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       define long shift left routine (signed and unsigned are same)
//           __allshl
//
//*******************************************************************************

//***
//llshl - long shift left
//
//Purpose:
//       Does a Long Shift Left (signed and unsigned are identical)
//       Shifts a long left any number of bits.
//
//Entry:
//       EDX:EAX - long value to be shifted
//       CL    - number of bits to shift by
//
//Exit:
//       EDX:EAX - shifted value
//
//Uses:
//       CL is destroyed.
//
//Exceptions:
//
//*******************************************************************************

#ifdef CFUNCTION
#define __cdecl __fastcall
#define _allshl _llshl
#endif

__declspec(naked) __int64 __cdecl _allshl(__int64 value, unsigned char shift)
{
#ifdef CFUNCTION
	__asm
	{
		mov     edx, dword ptr [esp + 8]
		pop     eax
		mov     dword ptr [esp + 4], eax
		pop     eax

		align   16
	}
#endif
#if 0
	__asm
	{
		//
		// Handle shifts of 64 or more bits (all get 0)
		//
		cmp     cl, 64
		jae     short RETZERO

		//
		// Handle shifts of between 0 and 31 bits
		//
		cmp     cl, 32
		jae     short MORE32
		shld    edx, eax, cl
		shl     eax, cl
		ret

		//
		// Handle shifts of between 32 and 63 bits
		//
	MORE32:
		mov     edx, eax
		xor     eax, eax
		and     cl, 31
		shl     edx, cl
		ret

		//
		// return 0 in edx:eax
		//
	RETZERO:
		xor     eax, eax
		xor     edx, edx
		ret
	}
#else
	__asm
	{
		//
		// Handle shifts of 64 or more bits (all get 0)
		//
		cmp     cl, 64
		jae     retzero

		//
		// Handle shifts of between 0 and 31 bits
		//
		cmp     cl, 32
		jae     more32
		shld    edx, eax, cl
		shl     eax, cl
		ret

		//
		// Handle shifts of between 32 and 63 bits
		//
		align   16
	more32:
		mov     edx, eax
		je      equal32
		and     cl, 31
		xor     eax, eax
		shl     edx, cl
		ret

		//
		// return 0 in edx:eax
		//
		align   16
	retzero:
		xor     edx, edx
	equal32:
		xor     eax, eax
		ret
	}
#endif
}
