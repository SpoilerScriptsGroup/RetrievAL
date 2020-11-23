//***
//llmul.c - long multiply routine
//
//       Copyright (c) Microsoft Corporation. All rights reserved.
//
//Purpose:
//       Defines long multiply routine
//       Both signed and unsigned routines are the same, since multiply's
//       work out the same in 2's complement
//       creates the following routine:
//           __allmul
//
//*******************************************************************************

#define LOWORD(x) [x]
#define HIWORD(x) [(x) + 4]

//***
//llmul - long multiply routine
//
//Purpose:
//       Does a long multiply (same for signed/unsigned)
//       Parameters are not changed.
//
//Entry:
//       Parameters are passed on the stack:
//               1st pushed: multiplier (QWORD)
//               2nd pushed: multiplicand (QWORD)
//
//Exit:
//       EDX:EAX - product of multiplier and multiplicand
//       NOTE: parameters are removed from the stack
//
//Uses:
//       ECX
//
//Exceptions:
//
//*******************************************************************************

#ifdef CFUNCTION
#define __cdecl __fastcall
#define _allmul _llmul
#endif

__declspec(naked) __int64 __cdecl _allmul(__int64 multiplicand, __int64 multiplier)
{
#if 0
	__asm
	{
		#define A (esp + 4)                 // stack address of a
		#define B (esp + 12)                // stack address of b

		//
		//       AHI, BHI : upper 32 bits of A and B
		//       ALO, BLO : lower 32 bits of A and B
		//
		//             ALO * BLO
		//       ALO * BHI
		// +     BLO * AHI
		// ---------------------
		//

		mov     eax, HIWORD(A)
		mov     ecx, HIWORD(B)
		or      ecx, eax                    // test for both hiwords zero.
		mov     ecx, LOWORD(B)
		jnz     short hard                  // both are zero, just mult ALO and BLO

		mov     eax, LOWORD(A)
		mul     ecx

		ret     16                          //  callee restores the stack

	hard:
		push    ebx

		// must redefine A and B since esp has been altered

		#define A2 (esp + 8)                // stack address of a
		#define B2 (esp + 16)               // stack address of b

		mul     ecx                         // eax has AHI, ecx has BLO, so AHI * BLO
		mov     ebx, eax                    // save result

		mov     eax, LOWORD(A2)
		mul     dword ptr HIWORD(B2)        // ALO * BHI
		add     ebx, eax                    // ebx = ((ALO * BHI) + (AHI * BLO))

		mov     eax, LOWORD(A2)             // ecx = BLO
		mul     ecx                         // so edx:eax = ALO * BLO
		add     edx, ebx                    // now edx has all the LO * HI stuff

		pop     ebx

		ret     16                          // callee restores the stack

		#undef A
		#undef B
		#undef A2
		#undef B2
	}
#else
	__asm
	{
		#define A (esp + 4)                 // stack address of a
		#define B (esp + 12)                // stack address of b

		//
		//       AHI, BHI : upper 32 bits of A and B
		//       ALO, BLO : lower 32 bits of A and B
		//
		//             ALO * BLO
		//       ALO * BHI
		// +     BLO * AHI
		// ---------------------
		//

		push    ebx
		mov     eax, LOWORD(A + 4)
		mov     ecx, HIWORD(A + 4)
		mov     edx, LOWORD(B + 4)
		mov     ebx, HIWORD(B + 4)
		test    ecx, ecx                    // test for AHI is zero
		jz      L1
		imul    ecx, edx                    // ecx = AHI * BLO
	L1:
		test    ebx, ebx                    // test for BHI is zero
		jz      L2
		imul    ebx, eax                    // ebx = BHI * ALO
		add     ecx, ebx                    // ecx = (AHI * BLO) + (BHI * ALO)
	L2:
		mul     edx                         // edx:eax = ALO * BLO
		add     edx, ecx                    // now edx has all the LO * HI stuff
		pop     ebx
		ret     16                          // callee restores the stack

		#undef A
		#undef B
	}
#endif
}
